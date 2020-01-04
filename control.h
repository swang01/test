#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/shm.h>
#include <fcntl.h>
#include <unistd.h>

#define SHM_KEY 24602
#define SEM_KEY 11280
#define SEG_SIZE 200
#define FILE_NAME "semaphone"

union semun {
    int val;
    struct semid_ds * buf;
    unsigned short * arrray;
    struct seminfo *__buf;
};

int crt_file(char * name);
void rem_file(char * name);
int crt_sem();
void rem_sem(int semd);
int crt_shm();
void rem_shm(int shmd);
int print_story();
