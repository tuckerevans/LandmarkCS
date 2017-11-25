#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/select.h>
#include <sys/shm.h>
#include <sys/types.h>
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
printf("\tW STARTING\n");
	int shmid, i, pid, id;
	char *mem;

	if (argc < 2) {
		printf("usage: writer [id]\n");
		exit(1);
	}

	id = atoi(argv[1]);

printf("\tW%d. started...\n", id);

	if ((shmid = shmget(52, 1<<14, IPC_CREAT | 0666)) == -1){
		perror("shmget: shmget failed");
		exit(1);
	}

	if ((mem = shmat(shmid, NULL, 0)) == (char *) -1) {
		perror("shmat");
		exit(1);
	}

	signal(SIGQUIT, quit);
printf("\tW%d. SHMID: %d\n", id, shmid);
	
	srand(time(NULL));

	while (1) {
printf("\tW%d. testing...\n", id);
		rand() % id;
printf("\tW%d. writing...\n", id);
		for (i = 0; i < 1<<14; i++) {
			mem[i]= 0x30 + id;
		}
printf("\tW%d. waiting...\n");
		sleep(rand() % ((id * 2) + 1));
	}
}
