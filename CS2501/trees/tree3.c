#include <stdlib.h>
#include <string.h>
#include <stdio.h>


typedef struct node {
	int id;
	struct node *left;
	struct node *right;
} treenode;



int countLevels(t)
treenode *t;
{
	if(!t) return 0;
	return 1 + ((countLevels(t->left) > countLevels(t->right)) ? countLevels(t->left) : countLevels(t->right));
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
	t->left->left->left = malloc(sizeof(treenode));

	t->right->right->right = 0;
	t->right->right->left = 0;
	t->right->left->right = 0;
	t->right->left->left = 0;
	t->left->right->right = 0;
	t->left->right->left = 0;
	t->left->left->right = 0;


	t->left->left->left->left = 0;
	t->left->left->left->right = 0;

	t->id = 1;
	t->right->id = 3;
	t->left->id = 2;

	t->right->right->id = 7;
	t->right->left->id = 6;
	t->left->right->id = 5;
	t->left->left->id = 4;

	printf("the tree has %d levels\n", countLevels(t));
}
