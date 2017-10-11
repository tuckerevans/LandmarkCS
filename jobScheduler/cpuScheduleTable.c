#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <sys/time.h>

#define PROCESS_COUNT 4
#define time_calc(x, y) ((y.tv_sec - x.tv_sec) * 1000000) + (y.tv_usec - x.tv_usec)


/* TODO
add heap for priority
compute priority
*/

int go=1;
int turn=0;
float v;
float throughput;
struct timeval start_time;
int turns;
int completed_jobs;

struct process {
	int virgin;
	int (*proc)(int);
	int turnaround[9];
	struct timeval start_t;
	int runs;
	float save;
	int turn;
};

struct process pt[PROCESS_COUNT]; /*process table*/

void
cpusched(signum)
int signum;
{
	pt[turn].virgin=0;
	pt[turn].save=v;
	go=0;
}

int
f0(x)
int x;
{
	register int i=0;
	register int t;
	signal(SIGUSR1, cpusched);
	if (!x) goto start;
	v=0.0;

start:
	while (go && (v > -200.0)) {
		printf("0");fflush(stdout);
		t = (rand() % 1024);
		v -= 2.0;
		if ((v>0.0) || ((((int) v) % 2) == -1)) {
			printf("f0, found odd or positive, v= %f\n", v);
			exit(1);
		}
		usleep(t*100);
	}
	if (v <= -200.0) pt[0].virgin=1;
	go=1;
}

int
f1(x)
int x;
{
	register int i=0;
	register int t;
	if (!x) goto start2;
	v= -1.0;

start2:
	while (go && (v > -401.0)) {
		printf("1");fflush(stdout);
		t = (rand() % 2048);
		v -= 2.0;
		if ((v>0.0) || ((((int) v) % 2) != -1)) {
			printf("f1, found even or positive\n");
			exit(1);
		}
		usleep(t*100);
	}
	if (v <= -401.0) pt[1].virgin=1;
	go=1;
}

int
f2(x)
int x;
{
	register int i=0;
	register int t;
	if (!x) goto start3;
	v= 1.0;

start3:
	while (go) {
		printf("2");fflush(stdout);
		t = (rand() % 4096);
		v += 2.0;
		if ((v<0.0) || ((((int) v) % 2) != 1)) {
			printf("f2, found even or negative\n");
			exit(1);
		}
		usleep(t*100);
	}
	go=1;
}

int
f3(x)
int x;
{
	register int i=0;
	register int t;
	if (!x) goto start4;
	v= 0.0;

start4:
	while (go) {
		printf("3");fflush(stdout);
		t = (rand() % 4096);
		v += 2.0;
		if ((v<0.0) || ((((int) v) % 2) == 1)) {
			printf("f3, found odd or negative\n");
			exit(1);
		}
		usleep(t*100);
	}
	go=1;
}

void turnaround_calc(p, n)
struct process *p;
int n;
{
	int i;
	p->turnaround[8] = 0;
	for (i = 0; i < n; i++) {
		p->turnaround[8] += p->turnaround[i] / n;
	}
	return;
}

int main(argc, argv, envp)
int argc;
char **argv, **envp;
{
	int pid, i, last;
	struct timeval end_t;
	gettimeofday(&start_time, NULL);

	for (i = 0; i < PROCESS_COUNT; i++) {
		pt[i].virgin = 1;
	}

	pt[0].proc=f0;
	pt[1].proc=f1;
	pt[2].proc=f2;
	pt[3].proc=f3;

	signal(SIGUSR1, cpusched);
	if (pid=fork()) {
		while (1) {
			go = 1;
			sleep(5);
			if (go)
				kill(pid, SIGUSR1);
		}
	} else {
		while (1) {
			printf("turn= %d\n", ++turns);
			v=pt[turn].save;
			if (pt[turn].virgin)
				gettimeofday(&pt[turn].start_t, NULL);

			pt[turn].proc(pt[turn].virgin);

			gettimeofday(&end_t, NULL);

			if (pt[turn].virgin) {
				pt[turn].turnaround[pt[turn].runs % 8] = time_calc(pt[turn].start_t, end_t);
				turnaround_calc(&pt[turn], (pt[turn].runs < 8 ? pt[turn].runs + 1 : 8));
				pt[turn].runs++;
				completed_jobs++;
				kill(getppid(), SIGUSR1);
			} else if (pt[turn].runs == 0){
				pt[turn].turnaround[8] = (time_calc(pt[turn].start_t, end_t)) / 2;
			}

			throughput = completed_jobs / (float)(end_t.tv_sec - start_time.tv_sec);
			pt[turn].turn++;

			if (turns == 1000) {
				kill(getppid(), 9);
				exit(1);
			}

			printf("\n");

			for (i = 0; i < 4; i++){
				printf("\t[%d]\tturnaround= %9d\truns= %3d\tturn= %d\n", i, pt[i].turnaround[8], pt[i].runs, pt[i].turn);
			}

			printf("\ntime= %5d\tthroughput= %9f\tcompleted_jobs= %d\n\n\n", end_t.tv_sec - start_time.tv_sec, throughput, completed_jobs);

			last = turn;
			turn = 0;
			for (i = 1; i < PROCESS_COUNT; i++) {
				turn = ((pt[turn].turnaround[8] < pt[i].turnaround[8]) && (turn != last)) ? turn : i;
			}

		}
	}
}
