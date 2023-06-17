#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <signal.h>

#define SHMSZ 1024

int shmid;
char *shm;

void cleanup(int sig) {
    printf("Сервер закончил работу по сигналу %d\n", sig);
    if (shmdt(shm) == -1) {
        perror("shmdt");
        exit(1);
    }
    if (shmctl(shmid, IPC_RMID, 0) == -1) {
        perror("shmctl");
        exit(1);
    }
    printf("Shmem close.\n");
    exit(0);
}

int main() {
    key_t key;
    int *data;
    signal(SIGINT, cleanup);
    key = ftok(".", 'x');
    if ((shmid = shmget(key, SHMSZ, IPC_CREAT | 0666)) < 0) {
        perror("shmget");
        exit(1);
    }
    if ((shm = shmat(shmid, NULL, 0)) == (char *) -1) {
        perror("shmat");
        exit(1);
    }
    data = (int *) shm;
    while (1) {
        printf("Ожидание данных..\n");
        while (*data == 0)
            sleep(2);
        printf("Получены данные: %d\n", *data);
        *data = 0;
    }

    return 0;
}

