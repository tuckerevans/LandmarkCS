#include<stdlib.h>
#include<stdio.h>
#include <string.h>

typedef struct anode {
	char *name;
	struct anode *r, *l;
} node;



node* contains(tree, q)
node **tree;
char *q;
{
	node *tmp;
	if(!(*tree)) return 0;

	if ( strcmp((*tree)->name, q) == 0){
		return *tree;
	}
	tmp = contains(&(*tree)->l, q);
	return tmp ? tmp : contains(&(*tree)->r, q);
}



insert(tree, item)
node **tree;
node *item;
{
	if(!(*tree)) {
		*tree = item;
		return 1;
	}

	if(strcmp(item->name, (*tree)->name) < 0)
		insert(&(*tree)->l, item);

	else if(strcmp(item->name, (*tree)->name) > 0)
		insert(&(*tree)->r, item);
}

node* getParent(tree,item)
node **tree;
node *item;
{
	node *tmp;
	if(!(*tree)) return 0;
	if(*tree == item)
		return -1;
	if((*tree)->l  == item || (*tree)->r == item)
		return *tree;
	tmp = getParent(&(*tree)->l, item);
	return tmp ? tmp : getParent(&(*tree)->r, item);
}

delete(tree, item)
node **tree;
node *item;
{
	node *tmp, *parent, *tmp_parent;
	int cnt;
	if(!(*tree)) return 1;
	tmp = contains(tree, item->name);
	if(!tmp){
		printf("%s dose not exist\n", item->name);
		return 1;
	}
	parent = getParent(tree, tmp);
	if(!tmp->l && !tmp->r)
	{
		if (parent == -1){
			*tree = 0;
			return 1;
			free(tmp);
		}
		if(parent->l == tmp){
			parent->l = 0;
			free(tmp);
			return 1;
		}
		parent->r = 0;
		free(tmp);
		return 1;
	}
	if(tmp->l && tmp->r){
		item = tmp->r;
		while(item->l)
			item = item->l;

		tmp_parent = getParent(tree, item);
		if(parent == -1){
			*tree = item;
			item->l = tmp->l;
			item->r = tmp->r;

			if(tmp_parent == tmp)
				item->r = 0;
			else
				tmp_parent->l = 0;

			tree = &item;
			free(tmp);
			return 1;

		}
		if(parent->l == tmp){
			parent->l = item;
			item->l = tmp->l;
			item->r = tmp->r;

			if(tmp_parent == tmp)
				item->r = 0;
			else
				tmp_parent->l = 0;

			tree = &item;
			free(tmp);
			return 1;
		}
		parent->r = item;
		item->l = tmp->l;
		item->r = tmp->r;


		if(tmp_parent == tmp)
			item->r = 0;
		else
			tmp_parent->l = 0;

		tree = &item;
		free(tmp);
		return 1;

	}

	if (parent == -1) {
		*tree = tmp->l ? tmp->l : tmp->r;
		free(tmp);
		return 1;
	}
	if(parent->l == tmp){
		parent->l = tmp->l ? tmp->l : tmp->r;
		free(tmp);
		return 1;
	}

	parent->r = tmp->l ? tmp->l : tmp->r;
	return 1;


}




void printTree(tree)
node **tree;
{
	if (!(* tree)) return;
	printTree(&(*tree)->l);
	printTree(&(*tree)->r);
	printf("\t%s\n",(*tree)->name);
	return;
}


/*
The loop below reads from stdin until EOF, then searches
to see if the BST contains the word found in argv[1].

Modify the loop to read a series of lines of one of two types:

i value
d value

Lines starting with the ASCII i are to be inserted into the binary search tree.
Those beginning with 'd' are to be deleted if they exist. If they do not exist,
output a line of text  saying so and do nothing else.

Stop your read loop at EOF (like it is now). Instead of
calling contains(), print out the tree; make it a preorder print */



void main(argc, argv, envp)
int argc;
char **argv, **envp;
{
	int i;
	char buf[1024];
	node *curr, *root;

	root = 0;

	while (gets(buf)) {
		curr = (node *)malloc(sizeof(node));
		curr->l = curr->r = NULL;
		curr->name = strdup(buf + 2);
		if('i' == *buf){
			printf("insert: %s %s\n", buf, curr->name);
			insert(&root, curr);
		}
		else if('d' == *buf){
			printf("delete:%s %s\n", buf, curr->name);
			delete(&root, curr);
		}
		else if ('p' == *buf)
			printTree(&root);

	}
	printTree(&root);

}
