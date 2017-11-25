#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void quit(signum)
int signum;
{
	shmctl(shmid, IPC_RMID, NULL);
}


int main(argc, argv)
int argc;
char **argv;
{
	int shmid, i, pid, n_read, n_write, w;
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
	signal(SIGQUIT, quit);

printf("\nSHMID: %d\nsetting initial memory values...\n", shmid);
	for (i = 0; i < 1<<14; i++) {
		*(mem + i) = 0x30;
	}

	arg_r = malloc(sizeof(char*) * 3);
	arg_w = malloc(sizeof(char*) * 3);
	*arg_r = malloc(sizeof(char) * 10);
	*arg_w = malloc(sizeof(char) * 10);

	*(arg_r + 1) = malloc(sizeof(char) * 50);
	*(arg_w + 1) = malloc(sizeof(char) * 50);

	*arg_r = "reader";
	*arg_w = "writer";

	*(arg_r + 2) = NULL;
	*(arg_w + 2) = NULL;

printf("forking readers...\n");
	for (i = 0; i < n_read; i++){
printf("sprintf...\n");
		sprintf(*(arg_r + 1), "%d", i);
printf("\t%d\n",i);
		if (pid = fork()) {
			/* printf("starting reader %d...\n", i); */
		} else {
			int ret = execv("./reader", arg_r);
			printf("exec retern %d", ret);
		}
	}

printf("forking writers...\n");
	for (i = 0; i < n_write; i++) {
		sprintf(*(arg_w + 1), "%d", i);
printf("\t%d\n", i);
		if (pid = fork()) {
			/* printf("starting writer %d...\n", i); */
		} else {
			execvp("./writer", arg_w);
		}
		
	}

	shmdt(mem);
printf("sync done...\n");
/* TODO
 * why is this returning 8
 */
	for (i = 0; i < (n_write + n_read); i++) {
		wait(&w);
		printf("\nReturned with code:%d\n", WTERMSIG(w));
	}
	quit();
}
