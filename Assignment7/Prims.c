//lot to be done 
#include <stdio.h>
#include <stdlib.h>
#define INF 0x7fffffff

typedef struct vertex{
	int v;
	int key;
	char bit; //to store whether its in mst or not
	struct vertex *p; //parent
	struct list *adj; //adjacency list's head (edges incident on the vertex)
	int loc; //to store its location in heap
} vertex;
typedef struct list{
	vertex *present;
	struct list *next;
	int weight;
}list;

//function definitions:
void Prims(vertex**,int,int);

list *Nil; //dummy
int main()
{
	Nil= malloc(sizeof(vertex));
	int v_n,e_n;
	printf("Enter the number of vertices :");
	scanf("%d",&v_n);
	vertex *Vs = malloc(sizeof(vertex)*v_n); //The actual vertices are stored in Vs
	vertex **v = malloc(sizeof(vertex*)*v_n); //to store pointers to the vertices

	for(int i=0;i<v_n;i++){
		Vs[i].v = i+1;
		Vs[i].adj = Nil;
		v[i] = Vs+i;
	}
	printf("Vertexes have been named 1-%d\n",v_n);
	printf("Enter the number of edges   :");
	scanf("%d",&e_n);
	printf("Enter the edges (u v weight) :\n");
	int v1,v2,weight;
	for(int i=0;i<e_n;i++)
	{

		scanf("%d %d %d",&v1,&v2,&weight);
		list *l1 = malloc(sizeof(list)*2);

		l1->present = v[v1-1];
		l1->next = v[v2-1]->adj; 
		l1->weight = weight;
		v[v2-1]->adj = l1;

		l1 = l1+1;

		l1->present = v[v2-1];
		l1->next = v[v1-1]->adj; 
		l1->weight = weight;
		v[v1-1]->adj = l1;

	}
	printf("Enter the vertex to grow MST on :");
	int root;
	scanf("%d",&root);

	Prims(v,v_n,root-1);

	//Todo freeing stuff
	free(Nil);
	free(Vs);
	return 0;
}

void Prims(vertex** v,int v_n,int root)
{
	//initializing each vertex
	for(int i=0;i<v_n;i++){
		v[i]->key = INF; //INF is the macro infinity
		v[i]->p= NULL;
		v[i]->loc = i;
		v[i]->bit = 0;
	}
	v[root]->key  = 0; //we start off our mst from here
	//making a min heap of the vertices:
	void minHeapify(vertex**,int,int);
	vertex* extractMin(vertex**,int);

	for(int i=v_n/2;i>=0;i--)
		minHeapify(v-1,v_n,i+1);
	printf("Initial Heap:\n");
	for(int i=0;i<v_n;i++)
		printf("%d   %d\n",v[i]->v,v[i]->key);
	printf("Done with heap\n");

	printf("\nMST:\n");
	//Prims:
	int totalWeight=0;
	while(v_n>0)
	{
		v[0]->bit = 1; //its now in mst
		vertex *min = extractMin(v,v_n);
		printf("%d %d\n",min->v,min->key);
		totalWeight += min->key;

		v_n--; //size of queue decreases.
		for(list *trav = min->adj;trav!=Nil;trav=trav->next ){
			if(trav->present->bit == 0 && trav->present->key > trav->weight){
				trav->present->key = trav->weight;
				trav->present->p   = min;

				//Maintaining heap property
				int i = trav->present->loc; //location of trav->present
				while(i>=0 && v[(i-1)/2]->key > v[i]->key){
					//swap the two and change their loc numbers
					vertex *temp = v[(i-1)/2];
					v[(i-1)/2] = v[i];
					v[i] = temp;
					v[i]->loc = i; v[(i-1)/2]->loc = (i-1)/2;
					i = (i-1)/2;
				}

				
			}
		}
	}
	printf("Done with MST\ntotal weight :%d",totalWeight);
		
	
	
}
void minHeapify(vertex **arr,int n,int i)
{
	if(i>=n) return;
	int l=i<<1, r=(i<<1)+1, min=i;

	if(l<= n&& arr[min]->key >arr[l]->key)
		min = l;
	if(r<= n&& arr[min]->key >arr[r]->key) 
		min = r;

	 //swapping
	vertex *temp= arr[i]; 
	arr[i] = arr[min]; 
	arr[min] = temp;
	arr[i]->loc = i-1;
	arr[min]->loc = min-1;

	if(min != i)
		minHeapify(arr,n,min);
}

vertex* extractMin(vertex** v,int n)
{
	vertex *min = v[0];
	//replacing v[0] with v[n-1];
	v[0] = v[n-1];
	v[0]->loc = 0;
	//preserving heap property
	minHeapify(v-1,n-1,1);

	return min;
}

