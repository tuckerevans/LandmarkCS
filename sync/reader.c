#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <sys/select.h>


int main(argc, argv)
int argc;
char **argv;
{
	int shmid, i, pid, id;
	char *mem, filename[50];
	FILE *fd;
	struct timeval *s;

	if (argc != 1) {
		printf("usage: reader [id]\n");
		exit(1);
	}

	id = atoi(argv[1]);

	if (argc < 2) {
		printf("usage: sync [number readers] [number writers]\n");
		exit(1);
	}

	if ((shmid = shmget(52, 1<<14, IPC_CREAT | 0666)) == -1){
		perror("shmget: shmget failed");
		exit(1);
	}

	if ((mem = shmat(shmid, NULL, 0)) == (char *) -1) {
		perror("shmat");
		exit(1);
	}

	sprintf(filename, "reader.%d", id);
	
	fd = fopen(filename, "a");

	if (!fd) {
		perror("fopen: ");
		exit(1);
	}
	srand(time(NULL));

	while (1) {
		s->tv_sec = rand() % id;
		for (i = 0; i < 1<<14; i++) {
			fprintf(fd, "%c", *(mem + i));
			fflush(fd);
		}

		select(0, NULL, NULL, NULL, s);
	}
}
