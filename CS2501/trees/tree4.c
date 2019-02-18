#include <stdlib.h>
#include <string.h>
#include <stdio.h>


typedef struct node {
	int id;
	struct node *left;
	struct node *right;
} treenode;



/* WRITE A RECURSIVE routine which swaps the
   left and right child for every node in a given tree */
/* will this operate in pre, in, or post order fashion?
does it matter? if so, why does it matter? */

rotate(t)
treenode *t;
{
	treenode *tmp;
	if(!t) return;
	rotate(t->left);
	rotate(t->right);
	tmp = t->left;
	t->left = t->right;
	t->right = tmp;
}


preorder(t)
treenode *t;
{
	if (!t) return;

	printf("%d\n", t->id);
	preorder(t->left);
	preorder(t->right);
}




main (argc, argv, envp)
int argc;
char **argv, **envp;
{
	treenode *t;
	t = malloc(sizeof(treenode));
	/* build a binary tree with at least 6 nodes */
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

	/* build a binary tree with at least 6 nodes */
	t->id = 1;
	t->right->id = 3;
	t->left->id = 2;

	t->right->right->id = 7;
	t->right->left->id = 6;
	t->left->right->id = 5;
	t->left->left->id = 4;


	/* build a binary tree with at least 6 nodes */

	/* now show your rotate function works */
	preorder(t);
	rotate(t);
	printf("\n");
	preorder(t);

}
