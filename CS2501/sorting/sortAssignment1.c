#include <stdio.h>
#include <string.h>
#include <stdlib.h>


printData(d,n)
int *d,n;
{
	int i;
	for (i=0; i<n; i++)
		printf("data[%d] = %d\n", i, d[i]);
}


selectionSortAscending(d, n)
int *d, n;
{
	int i, j, tmp;

	for (i = 0; i < n; i++) {
		tmp = d[i];
		for (j = i + 1; j < n; j++) {
			if(tmp > d[j]){
				d[i] = d[j];
				d[j] = tmp;
				tmp = d[i];
			}
		}

	}
}

insertionSortDescending(d, n)
int *d, n;
{
	int i,j,tmp;

	for (i = 1; i < n; i++){
		for (j = i; d[j] > d[j-1] && j != 0; j--){
			tmp = d[j];
			d[j] = d[j-1];
			d[j-1] = tmp;
		}
	}
}




main(argc, argv, envp)
int argc;
char **argv, **envp;
{
	char buf[1024];
	int n, *data, i;
	if (argc !=2 ) {
		printf("usage: sortAssignment1 #elements\n");
		exit(1);
	}

	n = atoi(argv[1]);
	data = calloc(n, sizeof(int));
	for (i=0; i<n; i++) {
		gets(buf);
		data[i] = atoi(buf);
	}


	printf("unsorted data:\n"); sleep(1);
	printData(data, n);


	selectionSortAscending(data, n);
	printf("After selection sort\n"); sleep(1);
	printData(data, n);

	insertionSortDescending(data, n);
	printf("After insertion sort\n"); sleep(1);
	printData(data, n);
}
