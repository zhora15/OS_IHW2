#include <fcntl.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#define OBSTACLE '.'
#define EMPTY '0'
#define FIRST_GARDENER 'a'
#define SECOND_GARDENER 'b'

#define MEM "/sh_memory"

typedef struct {
    char state;
    sem_t semaphore;
} cell;

int x_size, y_size, period, spd1, spd2;
cell* field;

void (*prev)(int);

void stop();
void handle_sigint(int sig);
void work(cell*, int);
void print_field();
void work(cell*, int);
void gardener1();
void gardener2();