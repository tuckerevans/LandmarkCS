#include <stdlib.h>
#include <string.h>
#include <stdio.h>


typedef struct node {
	char *name;
	struct node *left;
	struct node *right;
} treenode;

typedef struct queue {
	int front,back;
	treenode **data;
} queue;




inorder(t)
treenode *t;
{
	if (!t) return;
	inorder(t->left);
	printf("%s\n",t->name);
	inorder(t->right);
}



addNode(n,v,q)
treenode **n;
char *v;
queue *q;
{
	treenode *nxt;
	nxt = malloc(sizeof(treenode));
	memset(nxt, 0, sizeof(nxt));

	nxt->name = v; /* safe - caller creates new storage for each */
	enqueue(nxt,q);

}

enqueue(t,q)
treenode *t;
queue *q;
{
	q->data[(q->front)++] = t;
}




main (argc, argv, envp)
int argc;
char **argv, **envp;
{

/*

prompt the user for a series of names and add them
in the order given to create a complete binary tree


the user enters
  giselle
  magda
  aviva
  ahnk
  havi
  minna
  monique
  ^d


you build:


         giselle
         /  \
    magda    aviva
    / \       /  \
 ahnk havi minna monique


then print it out inorder
this should work for any number of name entered.
(consider using a queue)
*/

	treenode *t = 0;
	char buf[1024];
	queue q;
	q.back = 0;
	q.front = 0;
	q.data = calloc(100,sizeof(treenode*));

	while (gets(buf))
		addNode(&t, strdup(buf),&q);

	printf("q.back %d q.front %d", q.back, q.front);
	while(q.back < q.front){
		printf("bulid tree pass %d %x\n", q.back,q.data[q.back]);
		q.data[q.back]->left = q.data[((2 * q.back) +1)];
		q.data[q.back]->right = q.data[((2 * q.back) + 2)];
		q.back++;
	}
	t = q.data[0];
	inorder(t);
}
