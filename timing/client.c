#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/select.h>

#define BUFFER_SIZE 1024

struct number_val {
	int val;
	struct timeval tv;
};

typedef struct number_val num;

int sock_init(argv) 
char **argv;
{
	struct sockaddr_in address, serv_addr;
	int sock = 0, valread;
	char buffer[1024] = {0};

	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("Socket creation error\n");
		return -1;
	}
	
	memset(&serv_addr, '0', sizeof(serv_addr));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(atoi(argv[2]));

	if (inet_pton(AF_INET, argv[1], &serv_addr.sin_addr) <= 0) {
		perror("Invalid address/Address not supported\n");
		return -1;
	}

	if (connect(sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
		perror("Connection Failed");
		return -1;
	}
	return sock;
}

int main(argc, argv)
int argc;
char **argv;
{
	int sock = 0, valread, start_ptr = -1, end_ptr = 0, qset, i;
	double avg = 0, cnt;
	char buffer[1024] = {0};
	num num_buffer[BUFFER_SIZE];	
	fd_set rs, ws, es;
	struct timeval now, *ww, tmp;

	sock = sock_init(argv);

	while (1) {
		FD_ZERO(&rs); FD_ZERO(&ws); FD_ZERO(&es);
		FD_SET(sock, &rs);

		qset = select(sock + 1, &rs, (fd_set *) 0, (fd_set *) 0, ww);
		
		gettimeofday(&now, NULL);
		now.tv_sec = now.tv_sec - 60;

		ww = &tmp;
		tmp.tv_sec = 0;
		tmp.tv_usec = 500000;

		if (FD_ISSET(sock, &rs)) {
			valread = read(sock, buffer, 1024);
			for (i = 0; i < valread; i+=2) {
				if (end_ptr == BUFFER_SIZE)
					end_ptr = 0;
				if (end_ptr != start_ptr) {
					gettimeofday(&(num_buffer[end_ptr].tv), NULL);
					num_buffer[end_ptr].val = atoi(buffer);
					end_ptr++;
				} else {
					printf("dropping number...\n");
				}
			}
		}

		start_ptr = start_ptr == -1 ? 0 : start_ptr;

		if (start_ptr < end_ptr) {
			for (i = start_ptr; i < end_ptr; i++) {
				if (num_buffer[i].tv.tv_sec <= now.tv_sec) {
					start_ptr = i + 1;
				}
			}
		} else {
			for (i = start_ptr; i < BUFFER_SIZE; i++) {
				if (num_buffer[i].tv.tv_sec <= now.tv_sec) {
					start_ptr = i + 1;
				}
			}

			for (i = 0; i < end_ptr; i++) {
				if (num_buffer[i].tv.tv_sec <= now.tv_sec) {
					start_ptr = i + 1;
				}
			}

			start_ptr %= BUFFER_SIZE;
		}

		avg = 0;
		cnt = start_ptr > end_ptr ? (BUFFER_SIZE - start_ptr + end_ptr) : (end_ptr - start_ptr);
		cnt = cnt == 0 ? 10 : cnt;

		if (start_ptr < end_ptr) {
			for (i = start_ptr; i < end_ptr; i++){
				avg += num_buffer[i].val / cnt;
			}
		} else {
			for (i = start_ptr; i < BUFFER_SIZE; i++){
				avg += num_buffer[i].val / cnt;
			}
			for (i = 0; i < end_ptr; i++) {
				avg += num_buffer[i].val /cnt;
			}
		}

		printf("avg: %10.5f\n", avg);
/*
 * valread = read(sock, buffer, 1024);
 */		
	}


}
