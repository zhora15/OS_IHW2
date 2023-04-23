#include "utils.h"

void handle_sigint(int sig)
{
    stop();
    exit(0);
}

void print_field()
{
    printf("Field:\n");
    for (int y = 0; y < y_size; ++y) {
        for (int x = 0; x < x_size; ++x) {
            printf("%c ", field[y * x_size + x].state);
        }
        printf("\n");
    }
}

void work(cell* f, int gardener)
{
    sem_wait(&(*f).semaphore);
    if ((*f).state == EMPTY) {
        if (gardener == FIRST_GARDENER) {
            usleep(spd1 * 1000);
        } else if (gardener == SECOND_GARDENER) {
            usleep(spd2 * 1000);
        }
        (*f).state = gardener;
    } else {
        printf(" skip");
    }
    sem_post(&(*f).semaphore);
    usleep(period * 1000);
    return;
}

void gardener1()
{
    for (int y = 0; y < y_size; ++y) {
        for (int x = y % 2 ? x_size - 1 : 0; y % 2 ? x >= 0 : x < x_size;
             y % 2 ? --x : ++x) {
            printf("1 work with y=%d; x=%d", y, x);
            work(&field[y * x_size + x], FIRST_GARDENER);
            printf("\n");
            print_field(field, y_size, x_size);
        }
    }
    printf("1 all\n");
}

void gardener2()
{
    for (int x = x_size - 1; x >= 0; --x) {
        for (int y = (x_size - 1 - x) % 2 ? 0 : y_size - 1;
             (x_size - 1 - x) % 2 ? y < y_size : y >= 0;
             (x_size - 1 - x) % 2 ? ++y : --y) {
            printf("2 work with y=%d; x=%d", y, x);
            work(&field[y * x_size + x], SECOND_GARDENER);
            printf("\n");
            print_field(field, y_size, x_size);
        }
    }
    printf("2 all\n");
}