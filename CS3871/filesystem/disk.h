#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define TRACKS 128
#define SECTORS 4096

/* 10ms per track change */
/* 10ms/SECTORS per unit sector distance > 1 */

void dinit();

void rsector(int t,int s,unsigned char *b);

void wsector(int t,int s,unsigned char *b);


