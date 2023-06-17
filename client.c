#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>

#define SHMSZ 1024

int shmid;
char *shm;

void cleanup(int sig) {
    printf("Клиент закончил работу по сигналу %d\n", sig);
    if (shmdt(shm) == -1) {
        perror("shmdt");
        exit(1);
    }
    exit(0);
}

int main() {
    srand(time(0));

    key_t key;
    int *data;
    int num;
    signal(SIGINT, cleanup);
    key = ftok(".", 'x');
    if ((shmid = shmget(key, SHMSZ, 0666)) < 0) {
        perror("shmget");
        exit(1);
    }
    if ((shm = shmat(shmid, NULL, 0)) == (char *) -1) {
        perror("shmat");
        exit(1);
    }
    data = (int *) shm;
    while (1) {
        num = rand() % 50;
        printf("Отправление данных: %d\n", num);
        *data = num;
        sleep(2);
    }
    return 0;
}

