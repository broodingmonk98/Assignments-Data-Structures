#include<stdio.h>
#include<stdlib.h>
typedef struct node{
	int n[10];
}node;
int main()
{
	node **ptr = malloc(sizeof(node*)*10);
	printf("%d %d %d",sizeof(struct node),sizeof(node*),sizeof(*ptr[5]));

	
}
