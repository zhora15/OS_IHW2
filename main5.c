#include "utils.h"

void stop()
{
    for (int y = 0; y < y_size; ++y) {
        for (int x = 0; x < x_size; ++x) {
            sem_destroy(&(field[y * x_size + x].semaphore));
        }
    }
    munmap(field, x_size * y_size * sizeof(cell));
    shm_unlink(MEM);
    printf("Deallocated\n");
}

int main()
{
    prev = signal(SIGINT, handle_sigint);
    srand(time(NULL));
    printf("Введите сколько мс будет принято за единицу отсчета: ");
    scanf("%d", &period);
    if (period < 1) {
        perror("Incorrect input. Finished...");
        return 0;
    }
    printf(
        "Введите время, затрачиваемое первым садовникам для обработки участва "
        "в единицах отсчета: ");
    scanf("%d", &spd1);
    printf(
        "Введите время, затрачиваемое вторым садовникам для обработки участва "
        "в единицах отсчета: ");
    scanf("%d", &spd2);
    if ((spd1 < 1) || (spd2 < 1)) {
        perror("Incorrect input. Finished...");
        return 0;
    }
    printf("Введите размер поля по двум координатам: ");
    scanf("%d %d", &x_size, &y_size);
    if ((x_size < 1) || (y_size < 1)) {
        perror("Incorrect input. Finished...");
        return 0;
    }

    int shm_fd = shm_open(MEM, O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("An error occurred while opennig shared memory\n");
        return 0;
    }

    if (ftruncate(shm_fd, x_size * y_size * sizeof(cell)) == -1) {
        perror("An error occurred while truncating");
        return 0;
    }
    field = mmap(NULL, x_size * y_size * sizeof(cell), PROT_READ | PROT_WRITE,
        MAP_SHARED, shm_fd, 0);
    if (field == MAP_FAILED) {
        perror("An error occured while mapping shared memory");
        return 0;
    }

    for (int y = 0; y < y_size; ++y) {
        for (int x = 0; x < x_size; ++x) {
            if (sem_init(&field[y * x_size + x].semaphore, 1, 1)) {
                perror("An error occured while creating semaphore");
            }
            field[y * x_size + x].state = EMPTY;
        }
    }
    int amount_obst = (int)((0.1 * x_size * y_size + rand() % x_size * y_size * 0.2));
    while (amount_obst > 0) {
        int x = rand() % x_size;
        int y = rand() % y_size;
        if (field[y * x_size + x].state == EMPTY) {
            field[y * x_size + x].state = OBSTACLE;
            --amount_obst;
        }
    }
    pid_t chpid = fork();

    if (chpid == -1) {
        printf("An error occurred while creating first child\n");
        exit(-1);
    } else if (chpid == 0) { // child process
        signal(SIGINT, prev);
        gardener1(field, y_size, x_size);
        exit(0);
    }

    gardener2(field, y_size, x_size);
    pid_t wpid;
    int status = 0;
    while ((wpid = wait(&status) > 0)) {
        stop();
    }
    return 0;
}