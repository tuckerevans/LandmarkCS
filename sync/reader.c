#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/select.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

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
printf("\tR STARTING\n");
	int shmid, i, pid, id;
	char filename[50];
	FILE *fd;

	if (argc < 2) {
		printf("usage: reader [id]\n");
		exit(1);
	}

	id = atoi(argv[1]);
printf("\tR%d. started...\n", id);


	if ((shmid = shmget(52, 1<<14, IPC_CREAT | 0666)) == -1){
		perror("shmget: shmget failed");
		exit(1);
	}

	if ((mem = shmat(shmid, NULL, 0)) == (char *) -1) {
		perror("shmat");
		exit(1);
	}
	signal(SIGQUIT, quit);
printf("\tR%d. SHMID: %d\n", id, shmid);

	sprintf(filename, "reader.%d", id);
	
printf("\tR%d. opening file \"%s\"...\n", id, filename);
	fd = fopen(filename, "a");

	if (!fd) {
		perror("fopen: ");
		exit(1);
	}
	srand(time(NULL));

	while (1) {
printf("\tR%d. reading...\n", id);
		for (i = 0; i < 1<<14; i++) {
			fprintf(fd, "%c", *(mem + i));
			fflush(fd);
		}
		fprintf(fd, "\n");
		fflush(fd);

printf("\tR%d. waiting...\n", id);
		sleep(rand() % (id + 1));
	}
}
