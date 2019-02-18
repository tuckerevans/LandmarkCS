#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/select.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#define NSEM 3

char *mem;

void quit(signum)
int signum;
{
	shmdt(mem);
	exit(1);
}

int main(argc, argv)
int argc;
char **argv;
{
	int shmid, semid, i, pid, id;
	char *mem;
	struct sembuf sb;

	if (argc < 2) {
		printf("usage: writer [id]\n");
		exit(1);
	}

	id = atoi(argv[1]);


	if ((shmid = shmget(52, 1<<14, IPC_CREAT | 0666)) == -1){
		perror("shmget: shmget failed");
		exit(1);
	}

	if ((mem = shmat(shmid, NULL, 0)) == (char *) -1) {
		perror("shmat");
		exit(1);
	}
printf("Wshmid: %x\n", shmid);

	if ((semid = semget(shmid, NSEM, 0)) == -1) {
		perror("Wsemget: ");
		exit(1);
	}

	signal(SIGQUIT, quit);
	
	srand(time(NULL));

	while (1) {
		rand() % id;
		
		sb.sem_num = 0; sb.sem_op = -1; sb.sem_flg = 0;
		semop(semid, &sb, 1);

		for (i = 0; i < 1<<14; i++) {
			mem[i]= 0x30 + id;
		}
		
		sb.sem_op = 1;
		semop(semid, &sb, 1);

		sleep(rand() % ((id * 2) + 1));
	}
}
