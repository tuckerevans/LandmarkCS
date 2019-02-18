#include <stdlib.h>
#include <string.h>
#include <stdio.h>


typedef struct node {
	int id;
	struct node *left;
	struct node *right;
} treenode;


/* FILL ME in */
breadthFirst(t)
treenode *t;
{
	treenode **q;
	int front, back;
	front = 0;
	back = 0;
	q = malloc(100 * sizeof(treenode*));
	*q = t;
	while(q[back]){
		printf("%d\n", q[back]->id);
		q[++front] = q[back]->left;
		q[++front] = q[back++]->right;
	}
}


main (argc, argv, envp)
int argc;
char **argv, **envp;
{

	treenode *t;
	t = malloc(sizeof(treenode));
	t->right = malloc(sizeof(treenode));
	t->left = malloc(sizeof(treenode));
	t->right->right = malloc(sizeof(treenode));
	t->right->left = malloc(sizeof(treenode));
	t->left->right = malloc(sizeof(treenode));
	t->left->left = malloc(sizeof(treenode));

	t->right->right->right = 0;
	t->right->right->left = 0;
	t->right->left->right = 0;
	t->right->left->left = 0;
	t->left->right->right = 0;
	t->left->right->left = 0;
	t->left->left->right = 0;
	t->left->left->left = 0;

	t->id = 1;
	t->right->id = 3;
	t->left->id = 2;

	t->right->right->id = 7;
	t->right->left->id = 6;
	t->left->right->id = 5;
	t->left->left->id = 4;
	/* build a tree any way you like with at least 5 nodes */
	/* print it out breadth first, use a queue (so write one first) */
	breadthFirst(t);
}
