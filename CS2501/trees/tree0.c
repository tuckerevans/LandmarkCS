#include <stdlib.h>
#include <string.h>
#include <stdio.h>


typedef struct node {
	int id;
	struct node *left;
	struct node *right;
} treenode;


main (argc, argv, envp)
int argc;
char **argv, **envp;
{

	treenode *a, *b, *c, *d, *z;

	z = malloc(sizeof(treenode));
	z->id = 'z';
	z->left = 0;
	z->right= 0;

	d = malloc(sizeof(treenode));
	d->id = 'd';
	d->left = 0;
	d->right= 0;

	c = malloc(sizeof(treenode));
	c->id = 'c';
	c->left = d;
	c->right= z;

	b = malloc(sizeof(treenode));
	b->id = 'b';
	b->left = 0;
	b->right= 0;

	a = malloc(sizeof(treenode));
	a->id = 'a';
	a->left = b;
	a->right= c;

/* manually create nodes to build this tree:

      a
    /   \
   b    c
       / \
      d  z

*/
}
