#include <stdlib.h>
#include <string.h>
#include <stdio.h>


typedef struct node {
	int id;
	struct node *left;
	struct node *right;
} treenode;


/* FILL ME in */
inorder(t)
treenode *t;
{
if(!t) return 1;
inorder(t->left);
printf("%c\n", t->id);
inorder(t->right);
}



/* FILL ME in */
postorder(t)
treenode *t;
{
if(!t) return 1;
 postorder(t->left);
 postorder(t->right);
printf("%c\n",t->id);
}



preorder(t)
treenode *t;
{
if(!t) return 1;
printf("%c",t->id);
printf("--%x  %x\n",t->left,t->right);
preorder(t->left);
preorder(t->right);
}




main (argc, argv, envp)
int argc;
char **argv, **envp;
{

/* print out the tree from the first assignment:

      a
    /   \
   b    c
       / \
      d  z

 print the nodes out three times:
    preorder
    inorder
    postorder

you will have to create the inorder() and postorder()
print functions to do this
*/
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

printf("INORDER\n");
inorder(a);
printf("PREORDER\n");
preorder(a);
printf("POSTORDER\n");
postorder(a);

return 1;
} 
