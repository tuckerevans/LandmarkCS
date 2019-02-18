#include <stdio.h>
#include <stdlib.h>

typedef struct agraph{
  int matrix[100][100];
} graph;

extern graph *g = 0;

void  init_graph()
{
  int i;
  g = malloc(sizeof(graph));
  for (i = 0; i < 100; i++){
    g->matrix[i][i] = -1;
  }
}

int adjacent(x,y)
int x,y;
{
  return g->matrix[x][y];
}

int* neighbors(x)
int x;
{
  int i, j;
  j = 1;
  for(i = 0; i < 100; i++)
  {
    if(g->matrix[x][i]){
      j++;
    }
  }
  int *y;
  y = calloc(j * sizeof(int));
  j = 0;
  for(i = 0; i < 100; i++) {
    if (g->matrix[x][i]){
      y[j++] = i;
    }
  }
  return y;
}

int add_vertex(x)
int x;
{
  if(!g){
	init_graph();
  }
  if (g->matrix[x][x] == -1){
    g->matrix[x][x] = 0;
    return 1;
  }
  return 0;
}

int remove_vertex(x)
int x;
{
  if(g->matrix[x][x] == -1){
    return 0;
  }
  int i;
  for(i = 0; i < 100; i++){
    g->matrix[x][i] = 0;
    g->matrix[i][x] = 0;
    }
  return 1;
}

int add_edge(x,y)
int x,y;
{
  if (g->matrix[x][y]){
    return 0;
  }

  g->matrix[x][y] = 1;
  return 1;
}

int remove_edge(x,y)
int x,y;
{
  if(g->matrix[x][y]){
    g->matrix[x][y] = 0;
    return 1;
  }
  return 0;

}

void printEdges()
{
  printf("Edges: \n" );
  int i,j;
  for (i = 0; i < 100; i++){
    for(j = 0; j < 100; j++){
      if(g->matrix[i][j] == 1)
        printf("\t%d -> %d\n", i, j);
    }
  }
}

void printVertices()
{
  printf("Vertices:\n");
  int i;
  for(i = 0; i <100; i++)
    if(g->matrix[i][i] != -1)
      printf("\t%d\n", i);

}
