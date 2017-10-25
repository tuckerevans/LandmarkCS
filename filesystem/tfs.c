#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "disk.h"

#define MAX_INODES 10

struct block_ptr {
	char track;
	short sector; 
};

struct meta_data{ 
	char name[8];
	int size;
	int read;
	int write;
	int create;
 };

struct inode {
	struct meta_data info;
	struct block_ptr data[20];
};

typedef struct filestuff{
	int fd;
	struct inode file;
};


struct inode i_table[MAX_INODES];

char bitmap[128][4096/8];

int check_bitmap(t,s)
int t,s;
{
	char tmp;
	tmp = bitmap[t][s/8];
	tmp &= (1 << (7-(s % 8)));
	return (int) tmp;
}

void set_bitmap(t,s)
int t,s;
{
	bitmap[t][s/8] |= (1 << (7-(s % 8)));
}

void print_bitmap()
{
	int i,j;
	for(i = 0; i < 4096; i++) {
		printf("\n%4d  ", i);
		for (j = 0; j < 128; j++) {
			printf("%02x", bitmap[j][i]);
			if (j == 63) {
				printf("\n%4d  ",i);
			}
		}
	}
}

int search_inodes(name)
char *name;
{

}

/*
int tfs_init()
{
	int n = MAX_INODES / 4;
	int i;
	char *ptr;
	if (MAX_INODES % 4 > 0)
		n++;

	char buf[512 * n];

	for (i =0; i < n; i++) {
		rsector(0, i, &buf + (512 * i));
	}
	ptr = &buf;

	for(i=0; i< MAX_INODES; i++) {
		memcpy(&i_table[i], ptr, 64);
		ptr + 64;
	}
}
*/

int open(fname, mode)
char *fname, *mode;
{
	
}

int close(fd)
int fd;
{
	
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
	setBitmap(1,1);
	setBitmap(2,2);

	printf("test 1,1: %d,  test 2,2: %d\n", checkBitmap(1,1), checkBitmap(2,2));
	printBitmap();
}
