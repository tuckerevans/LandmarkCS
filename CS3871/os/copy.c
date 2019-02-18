#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>

int main(argc, argv)
int argc;
char ** argv;
{
	int val;
	char buf[512], *dev;
	int file, floppy;

	if (argc < 3 ) {
		perror("usage: copy path_to_bootloader path_to_os [path_to_device]\n");
		exit(1);
	}
	if (argc >= 4) {
		dev = argv[3];
	} else {
		dev = "/dev/fd0";
	}

	memset(buf, 0, 512);

	floppy = open(dev, O_RDWR);
	if (file < 0) {
		perror("Could not open floppy: ");
		exit(1);
	}

	/******************************
	 * Write Bootloader to Floppy *
	 ******************************/

	file = open(argv[1], O_RDONLY);
	if (file < 0) {
		perror("Could not open bootloader: ");
		exit(1);
	}

	val = read(file, buf, 510);
	close(file);
	if (val < 0) {
		perror("Could not read bootloader: ");
		exit(1);
	}

	buf[510] = 0x55;
	buf[511] = 0xaa;

	lseek(floppy, 0, SEEK_CUR);

	val = write(floppy, buf, 512);
	if (val < 0) {
		perror("Error writing to floppy: ");
		close(floppy);
		exit(1);
	}
	

	/**********************
 	 * Write OS to Floppy *
	 **********************/

	file = open(argv[2], O_RDONLY);
	if (file < 0) {
		perror("Could not open os: ");
		exit(1);
	}

	val = read(file, buf, 512);
	close(file);
	if (val < 0) {
		perror("Could not read os: ");
		exit(1);
	}

	val = write(floppy, buf, 512);
	if (val < 0) {
		perror("Error writing to floppy: ");
		close(floppy);
		exit(1);
	}

	syncfs(floppy);
	close(floppy);

	printf("Floppy successfully copied!\n");
	return 0;
}
