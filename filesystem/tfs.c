#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "disk.h"

#define MAX_INODES 10
#define INODE_START (TRACKS * SECTORS) / (8 * 512)


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
int search_inodes(name)
char *name;
{
	int i;
	for(i = 0; i < MAX_INODES; i++){
		if(strcmp(name, i_table[i].info.name) == 0) 
			return i;
	}
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
			current->next = malloc(sizeof(struct blockll));
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

int inode_init()
{
	int n = MAX_INODES / 4;
	int i;
	char *ptr;
	if (MAX_INODES % 4 > 0)
		n++;

	char *buf = malloc(512 * n);

	for (i =0; i < n; i++) {
		rsector(0, i, buf + (512 * i));
	}
	ptr = buf;

	for(i=0; i< MAX_INODES; i++) {
		memcpy(&i_table[i], ptr, 64);
		ptr += 64;
	}
}

/*save inodes to first n sectors on disk*/
void inode_save()
{
	int i, j;
	char *buf = malloc(512);

	for (i = 0; i < MAX_INODES && i_table->info.name[0] != 0; i++) {
		for (j = 0; j < 4; j++){
			memcpy(buf + j, &i_table[(i * 4) + j], sizeof(struct inode));
		}
		wsector(0, INODE_START + i, buf);
	}
}

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
	inode_init();
	inode_save();
}
