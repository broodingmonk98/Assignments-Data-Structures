/*
sample input from clrs, pg 633(3rd edition)
9
14
1 2 4
1 8 8
2 8 11
2 3 8
8 9 7
7 9 6
9 3 2
8 7 1
3 4 7
7 6 2
3 6 4
6 4 14
4 5 9
5 6 10
*/
#include <stdio.h>
#include <stdlib.h>

typedef struct vertex{
	int v;
	struct vertex *next;
	struct set *rep; //representative

}vertex;

typedef struct set{
	int size;
	vertex *head,*tail;
}set;

typedef struct edges{
	vertex *u,*v;
	int weight;
}edges;

//function definitions:
edges* MST_KRUSHKALS(vertex*,int,edges* ,int);
void MakeSet(vertex*);
void Union(vertex*,vertex*);
void display(edges*,int);

int main()
{
	int v_n,e_n;
	printf("Enter the number of vertices:");
	scanf("%d",&v_n);

	vertex *v = malloc(sizeof(vertex)*v_n);
	for(int i=0;i<v_n;i++)
		v[i].v = i+1;
	printf("Vertices have been assigned numbers 1-%d\n",v_n);
	
	printf("Enter the number of edges :");
	scanf("%d",&e_n);

	edges *e = malloc(sizeof(edges)*e_n);
	printf("Enter the edges(u v weight) :\n");
	int v1,v2;
	for(int i=0;i<e_n;i++){
		scanf("%d %d %d",&v1,&v2,&(e[i].weight));
		if(v1> v_n || v2> v_n){ //if the vertex doesn't exist
			printf("Edge not possible"); i--;
			continue;
		}
		e[i].u = &v[v1-1]; e[i].v = &v[v2-1];
	}

	edges *mst = MST_KRUSHKALS(v,v_n,e,e_n); //the function returns the array containing mst
	int flag=0;
	for(int i=0;i<v_n;i++)
		if(v[i].rep!=v[0].rep){
			flag=1;	
			break;
		}
	if(flag==1){
		printf("\nMST not possible since graph is not connected!!!!");
		return 1;
	}

	display(mst,v_n-1);
	printf("\nThank you, have a good day!!!:)");


	//freeing stuff
	free(v[0].rep);
	free(e);
	free(v);
	free(mst);
	return 0;
}

edges* MST_KRUSHKALS(vertex *v,int v_n,edges* e,int e_n)
{
	edges *A = malloc(v_n*sizeof(edges)); //to store the mst

	int idx=0;
	for(int i = 0;i<v_n;i++)
		MakeSet(&v[i]);

	//making a minheap out of edges
	void minHeapify(edges *arr,int n,int i);
	for(int i=e_n/2+1;i>=1;i--)
			minHeapify(e-1,e_n,i);


	edges extractMin(edges*,int n);
	for(int i=0;i<e_n;i++){
		edges min = extractMin(e,e_n-i);
		if(min.u->rep!=min.v->rep){ //check for same set
			A[idx++] = min;
			Union(min.u,min.v); //union of sets containing u and v
		}
	}
	return A;
}

void MakeSet(vertex *v)
{
	set *s = malloc(sizeof(set));
	s->head = v;
	s->tail = v;
	s->size = 1;
	v->rep = s;
	v->next = NULL;
}
void Union(vertex *u,vertex *v)
{
	set *s1 = u->rep,*s2 = v->rep;
	if(s1->size > s2->size){//swap them
		set *temp = s1; s1 = s2; s2 = temp;
	}
	//now s1 has to be smaller
	//s2 absorbs s1
	s2->size += s1->size;
	s2->tail->next = s1->head;
	s2->tail = s1->tail;
	for(vertex* trav = s1->head;trav!=NULL;trav=trav->next)
		trav->rep = s2;
	//s2 has now absorbed s1, so we free s1
	free(s1);
}
void minHeapify(edges *arr,int n,int i)
{
	if(i>=n) return;
	int l=i<<1, r=(i<<1)+1, min=i;

	if(l<= n&& arr[min].weight>arr[l].weight)
		min = l;
	if(r<= n&& arr[min].weight>arr[r].weight) 
		min = r;

	 //swapping
	edges temp= arr[i]; 
	arr[i] = arr[min]; 
	arr[min] = temp;

	if(min != i)
		minHeapify(arr,n,min);
										}
edges extractMin(edges* e,int n)
{
	edges min = e[0];
	//replacing e[0] with e[n-1];
	e[0] = e[n-1];
	//preserving heap property
	minHeapify(e-1,n,1);

	return min;
}
void display(edges* mst,int n)
{
	printf("\n\nThe edges in the Minimum Spanning Tree(Krushkals):\n");
	int sum=0;
	for(int i=0;i<n;sum+=mst[i].weight,i++)
		printf("%2d(%c) %2d(%c) %3d\n",mst[i].u->v,mst[i].u->v+96,mst[i].v->v,mst[i].v->v+96,mst[i].weight);
		//printf("%4d %4d\n",mst[i].u->v,mst[i].v->v);
		
	printf("Weight of the tree :%4d\n",sum);
}
