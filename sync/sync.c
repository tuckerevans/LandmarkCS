#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define NSEM 3

union semun {
	int val;
	struct semid_ds *buf;
	ushort *array;
};


int shmid, semid;

void quit(signum)
int signum;
{
	shmctl(shmid, IPC_RMID, NULL);
	semctl(semid, IPC_RMID, 0);
}


int main(argc, argv)
int argc;
char **argv;
{
	int i, pid, n_read, n_write, w;
	char *mem, **arg_r, **arg_w;
	union semun semarg;

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
printf("Sshmid: %x\n", shmid);
	signal(SIGQUIT, quit);

	for (i = 0; i < 1<<14; i++) {
		*(mem + i) = 0x30;
	}

	if ((semid = semget(shmid, NSEM, 0666 | IPC_CREAT)) == -1) {
		perror("Ssemget: ");
		exit(1);
	}

	semarg.val = 1;
	for (i = 0; i < NSEM; i++) {
		if ((semctl(semid, i, SETVAL, semarg)) == -1) {
			perror("semctl: ");
			exit(1);
		}
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

	for (i = 0; i < n_read; i++){
		sprintf(*(arg_r + 1), "%d", i);
		if (pid = fork()) {
			/* printf("starting reader %d...\n", i); */
		} else {
			int ret = execv("./reader", arg_r);
			printf("exec retern %d", ret);
		}
	}

	for (i = 0; i < n_write; i++) {
		sprintf(*(arg_w + 1), "%d", i);
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
		printf("\nReturned with code:%d\n", WEXITSTATUS(w));
	}
	quit();
}
