#include <stdio.h>
#include <stdlib.h>
#define RED   "\x1B[31m"
#define RESET "\x1B[0m"

typedef struct elements{
	int n;
	int rank;
	struct elements* p; 

}elements;



//function definitions:
elements *MakeSet(int n);
void Union(elements*,elements*);
elements* findSet(elements *n);

int main()
{
	int choice =0,N=100;
	elements **arr=malloc(N);
	int i=0;
	while(choice != 4)
	{
		printf(RESET);
		if(i>0)
			printf("Elements in memory :");
		for(int j=0;j<i;j++)
			printf("%d  ",arr[j]->n);
		printf("\n\n1) New set with element\n2) Union\n3) Find set\n4) Exit ");
		printf("\nEnter your choice :");
		scanf("%d",&choice);
		printf(RED);
		if(choice == 1){
			printf("\nEnter the element:");
			scanf("%d",&choice);

			arr[i++] = MakeSet(choice);
			printf("Successfully created new set\n\n\n");
			choice = 1;
		}
		else if(choice == 2){
			printf("\nEnter the two elements whose sets are to be unioned:");
			int n1,n2;
			scanf("%d %d",&n1,&n2);
			int id1=-1,id2=-1;
			for(int j=0;j<i;j++){
				if(arr[j]->n == n1)
					id1 = j;
				if(arr[j]->n == n2)
					id2 = j;
			}
			if(id1==-1||id2==-1){
				printf("Element not found!!!\n");
				continue;
			}

			elements *temp1,*temp2;
			temp1 = findSet(arr[id1]);
			temp2 = findSet(arr[id2]);
			if(temp1 == temp2)
				printf("elements already in the same set\n");
			else{
				Union(temp1,temp2);
				printf("The two sets have been combined\n\n");
			}

		}
		else if(choice == 3){
			printf("\nEnter the element:");
			scanf("%d",&choice);
			int j;
			for(j=0;j<i;j++)
				if(arr[j]->n==choice)
					break;
			if(j==i) printf("\nElement not found\n");
			else
				printf("\nelement in set represented by :%d\n\n",findSet(arr[j])->n);

		}
		if(i> N){ //if we run out of memory we create more
			N*=2;
			arr = realloc(arr,N);
		}
	}
	printf("\nThank you, have a nice day!!!");
	return 0;

}
elements *MakeSet(int n)
{
	elements *e = malloc(sizeof(elements));
	e->n= n;
	e->p = e;
	e->rank = 0;
	return e;
}

void Union(elements *e1,elements *e2)
{
	if(e1->rank>e2->rank)	
		e2->p = e1;
	else
		e1->p = e2;
	if(e1->rank == e2->rank)
		e2->rank++;
}
elements *findSet(elements *e)
{
	if(e != e->p)
		e->p = findSet(e->p);
	return e->p;
}
