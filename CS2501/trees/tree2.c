#include <stdlib.h>
#include <string.h>
#include <stdio.h>


typedef struct node {
	int id;
	struct node *left;
	struct node *right;
} treenode;


int countNodes(t)
treenode *t;
{
	int i = 0;
	if(!t) return 0;
	return 1 + countNodes(t->left) + countNodes(t->right);

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

	printf("the tree has %d nodes\n", countNodes(t));
}
