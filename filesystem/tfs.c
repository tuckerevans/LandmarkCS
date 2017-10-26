#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include "disk.h"

#define MAX_INODES 1000
#define INODE_START (TRACKS * SECTORS) / (8 * 512)
#define MAX_FILES 50

struct block_ptr {
	char track;
	short sector; 
};
struct blockll{
	struct block_ptr data;
	struct blockll *next;
};

struct meta_data{ 
	char name[8];
	int size;
	int read;
	int write;
	int create;
};

struct inode {
	struct meta_data info; struct block_ptr data[20];
};


struct inode_list{
	struct inode *node;
	struct inode_list *next;
};

struct file {
	struct inode *node;
	int mode;
	int next_sec;
	int free;
};


int inode_list_size = 0;
struct inode_list *root, *end;

char bitmap[TRACKS][SECTORS/8];

struct file files[MAX_FILES];
int size;



int check_bitmap(t,s)
int t,s;
{
	char tmp;
	tmp = bitmap[t][s/8];
	tmp &= (1 << (s % 8));
	return (int) tmp;
}

void set_bitmap(t,s)
int t,s;
{
	bitmap[t][s/8] |= (1 << (s % 8));
	return;
}

void print_bitmap()
{
	int i,j;
	for(i = 0; i < 128; i++){

		printf("\n%4d  ", i);
		for (j = 0; j < 4096/8; j++) {
			printf("%02x", bitmap[i][j]);
			if (j %31 == 0) {
				printf("\n%4d  ",i);
			}
		}
	}
}


/*  TODO
 *  Implement inode table as binary tree to speedup searches
 */
struct inode* inode_search(name)
char *name;
{
	if (strcmp(name,"") == 0) {
		return -1;
	}
	int i;
	struct inode_list *tmp = root;

	for(i = 0; i < MAX_INODES && i < inode_list_size; i++){
		tmp = tmp->next;
		if(strcmp(name, tmp->node->info.name) == 0) 
			return tmp->node;
	}
	return -2;
}

struct blockll* get_blocks(size)
int size;
{
	int i, t, s;
	struct blockll *root, *current = malloc(sizeof(struct blockll));
	root = current;

	for (i = 0; size > 0 && i < (4096 * 128); i++) {
		t = i / 4096;
		s = i % 4096;
		printf("checkedmap%d\t%d: %d\n", t,s,check_bitmap(t,s));
		if (!check_bitmap(t, s)) {
			current->next = calloc(1,sizeof(struct blockll));
			current = current->next;
			current-> next = NULL;
			current->data.track = (char) t;
			current->data.sector = (short) s;

			set_bitmap(t,s);
			size-= 512;
		}
	}

	return i <(4096 * 128) ? root : NULL;
}

struct inode_list* inode_create(name)
char *name;
{
	struct timeval *tmp_time = malloc(sizeof(struct timeval));

	struct inode_list *tmp = calloc(1,sizeof(struct inode_list));
	memcpy(&tmp->node->info.name, name, strlen(name));

	gettimeofday(tmp_time, NULL);
	tmp->node->info.create = tmp_time->tv_sec;
	tmp->node->info.read = tmp_time->tv_sec;
	tmp->node->info.write = tmp_time->tv_sec;
	
	end->next = tmp;
	end = tmp;
	inode_list_size++;

	return tmp;
}

int inode_init()
{
	int n = MAX_INODES / 4;
	int i;
	char *ptr;
	struct inode_list *tmp;

	if (MAX_INODES % 4 > 0)
		n++;

	char *buf = malloc(512 * n);

	for (i =0; i < n; i++) {
		rsector(0, i, buf + (512 * i));
	}
	ptr = buf;

	tmp = root;
	for(i=0; i< MAX_INODES; i++) {
		tmp->next = calloc(1,sizeof(struct inode_list));
		memcpy(&tmp->node, ptr, 64);
		ptr += 64;
		tmp = tmp->next;
		inode_list_size++;
	}
}

/*save inodes to first n sectors on disk*/
void inode_save()
{
	int i, j;
	char *buf = malloc(512);
	struct inode_list *tmp = root;

	for (i = 0; i < MAX_INODES && tmp->next;i++) {
		for (j = 0; j < 4; j++){
			tmp = tmp->next;
			memcpy(buf + j, tmp->node, sizeof(struct inode));
		}
		wsector(0, INODE_START + i, buf);
	}
}

struct inode* inode_from_fd(fd) 
int fd;
{
	int i;
	struct inode_list *tmp = root;

	for (i = 0; i < fd; i++) {
		tmp = tmp->next;
	}

	return tmp->node;
}

int find_fd()
{
	int i;
	for (i = 0; i < size; i++) {
		if (files[i].free)
			return i;
	}
}

int tfs_init()
{
	int i;
	dinit();
	inode_init();
	for (i = 0; i < MAX_FILES; i++) {
		files[i].free = 1;
	}
}


int open(fname, mode)
char *fname, *mode;
{
	struct inode *fnode = inode_search(fname);
	int fd;

	if (fnode == -1)
		return -1;
	
	if (fnode == -2)
		fnode = inode_create(fname)->node;

	fd = find_fd();

	files[fd].node = fnode;
	files[fd].mode = *mode;
	files[fd].next_sec = 0;
	files[fd].free = 0;

	size++;

	return fd;
}

int close(fd)
int fd;
{
	if (files[fd].free)
		return -1;

	files[fd].free = 1;
	return 1;
}

int read(fd, buf)
int fd;
char *buf;
{
	
}

int write(fd, buf)
int fd;
char *buf;
{
	
}

int ulink(fname)
char *fname;
{
	
}

int main() 
{
	tfs_init();
}
