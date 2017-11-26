#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/select.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#define NSEM 3
#define KEY 52

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
	char filename[50];
	FILE *fd;
	struct sembuf sb;

	if (argc < 2) {
		printf("usage: reader [id]\n");
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

	if ((semid = semget(shmid, NSEM, 0)) == -1) {
		perror("Rsemget: ");
		exit(1);
	}

	signal(SIGQUIT, quit);

	sprintf(filename, "reader.%d", id);
	
	fd = fopen(filename, "a");

	if (!fd) {
		perror("fopen: ");
		exit(1);
	}
	srand(time(NULL));

	while (1) {
		sb.sem_num = 0; sb.sem_op = -1; sb.sem_flg = 0;
		semop(semid, &sb, 1);

		for (i = 0; i < 1<<14; i++) {
			fprintf(fd, "%c", *(mem + i));
			fflush(fd);
		}
		fprintf(fd, "\n");
		fflush(fd);

		sb.sem_op = 1;
		semop(semid, &sb, 1);


		sleep(rand() % (id + 1));
	}
}
