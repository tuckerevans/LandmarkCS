#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>


int main(argc, argv)
int argc;
char **argv;
{
	int shmid, i, pid, n_read, n_write;
	char *mem, **arg_r, **arg_w;

	if (argc < 2) {
		printf("usage: sync [number readers] [number writers]\n");
		exit(1);
	}
	n_read = atoi(argv[1]);
	n_write = atoi(argv[2]);

	if ((shmid = shmget(52, 1<<14, IPC_CREAT | 0666)) == -1){
		perror("shmget: shmget failed");
		exit(1);
	}

	if ((mem = shmat(shmid, NULL, 0)) == (char *) -1) {
		perror("shmat");
		exit(1);
	}

	for (i = 0; i < 1<<14; i++) {
		*(mem + i) = 0x30;
	}

	arg_r = malloc(sizeof(char*) * 2);
	arg_w = malloc(sizeof(char*) * 2);

	*(arg_r + 1) = NULL;
	*(arg_w + 1) = NULL;

	for (i = 0; i < n_read; i++){
		sprintf(*arg_r, "%d", i);
		if (pid = fork()) {
			printf("starting reader %d...\n", i);
		} else {
			execvp("./reader", arg_r);
		}
	}

	for (i = 0; i < n_write; i++) {
		sprintf(*arg_w, "%d", i);
		if (pid = fork()) {
			printf("starting writer %d...\n", i);
		} else {
			execvp("./writer", arg_w);
		}
		
	}
}
