#include <stdio.h>
#include <iostream>
#include <unistd.h>
#include <termios.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/sem.h>

#define MAX_RETRIES 10
#define SHM_SIZE 500
#define MES_SIZE 40
union semun {
    int val;
    struct semid_ds *buf;
    ushort *array;
};

void sem(int semid,sembuf sb);
int main(int argc, char *argv[]) {
	if (argc == 1) {
		printf("./chat <name>");
		exit(1);
	}

    char* name = strcat(argv[1], ": ");
    char message[40];
    bool f = true;

    key_t semkey;
    int semid;
    struct sembuf sb_lock_shm;
    struct sembuf sb_unlock_shm;
    struct sembuf sb_lock_output;
    struct sembuf sb_unlock_output;
    union semun init;


    sb_lock_shm.sem_num = 0;
    sb_lock_shm.sem_op = -1;
    sb_lock_shm.sem_flg = SEM_UNDO;

    sb_unlock_shm.sem_num = 0;
    sb_unlock_shm.sem_op = 1;
    sb_unlock_shm.sem_flg = SEM_UNDO;

    if ((semkey = ftok(".", 'L')) == -1) {
        perror("ftok");
        exit(1);
    }

    semid = semget(semkey, 2, IPC_CREAT | IPC_EXCL | 0666);
    if (semid==-1)
        semid = semget(semkey, 2, 0);

    init.val=1;
    semctl(semid, 0, SETVAL, init);
    init.val=0;
    semctl(semid, 1, SETVAL, init);


    semctl(semid, 0,GETVAL);
    semctl(semid, 1,GETVAL);

    key_t key;
    int shmid;
    void *data;

    if ((key = ftok(".", 'R')) == -1) {
        perror("ftok");
        exit(1);
    }
    if ((shmid = shmget(key, SHM_SIZE, 0644 | IPC_CREAT)) == -1) {
        perror("shmget");
        exit(1);
    }
    data = shmat(shmid, (void *)0, 0);
    if (data == (char *)(-1)) {
        perror("shmat");
        exit(1);
    }
    char* s= (char*)data;
    pid_t pid = fork();
    int rv;

    if (pid == -1) {
        perror("fork");
        exit(1);
    }

	int lastLen = -1;
    if (pid == 0) {
        while(1){
            system("clear");
            printf("%s\n",s);
        	lastLen = strlen(s);
        	do {
        		usleep(100);
        	} while (lastLen == strlen(s));

        }
        exit(rv);

    } else {
	char str[MES_SIZE];
		while(1){
			fgets(str, MES_SIZE, stdin);

			sem(semid,sb_lock_shm);
			strcpy(message, "");
			if (f) {
				strcat(message, name);
				f = false;
			}

			for (int i=0; i < MES_SIZE; i++) {
				if (str[i] == '\n')
					f = true;
			}

			strcat(message, str);
			strcat(message, "");

			if ((strlen(s) + strlen(message)) > SHM_SIZE)
				strcpy(s, "");
			strcat(s, message);
			sem(semid,sb_unlock_shm);


		}
    }

    if (shmdt(data) == -1) {
        perror("shmdt");
        exit(1);
    }
    return 0;
}

void sem(int semid,sembuf sb){
    if (semop(semid, &sb, 1) == -1) {
        perror("semop");
        exit(1);
    }
}
