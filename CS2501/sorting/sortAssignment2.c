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


/* implement (only) one of your choosing */
quickSort(d, n)
int *d, n;
{
}

mergeSort(d, n)
int *d, n;
{
	if (n == 1)
		return;


	int i, n1, n2, *tmp1, *tmp2;

	n1 = n/2;
	n2 = (n%2 == 0) ? n1 : (n1 + 1);

	/*
	printf("recursion!%d %d %d %x\n", n1, n2, n, d );
	*/
	mergeSort(d, n1);
	mergeSort(&d[n1], n2);

	tmp1 = malloc(sizeof(int) * n1);
	tmp2 = malloc(sizeof(int) * n2);

	memcpy(tmp1, d, sizeof(int) * n1);
	memcpy(tmp2, &d[n1], sizeof(int) * n2);

	
	for (i = n-1; i >= 0; i--) {
		if(n2 == 0)
			d[i] = tmp1[--n1];
	
		else if (n1 == 0)
			d[i] = tmp2[--n2];
		 else
			d[i] = (tmp1[n1-1] < tmp2[n2-1]) ? tmp1[--n1] : tmp2[--n2];
	}	
	printf("\n");
	free(tmp1);
	free(tmp2);
	return;
}

heapSort(d, n)
int *d, n;
{
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

	mergeSort(data, n);
	printf("After sort\n");
	printData(data, n);
}
