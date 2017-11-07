#include <stdio.h>
#include <stdlib.h>

typedef struct node
{
	int n;
	struct node *left,*right;
} node;
void display(node*);
node* search(node*);
node* delete(node*);
node* insert(node*); 
int depth;
int idx; //to store index

int main()
{
	node *top = NULL;
	node *temp;
	int choice=1;
	while(choice != 0){
		display(top);
		scanf("%d",&choice);
		switch(choice)
		{
			case 1: top = insert(top);
				break;

			case 2: top = delete(top);
				break;

			case 3: 
				temp  = search(top);
				if(temp== NULL) 
					printf("\nNode not found!!!");
				else
					printf("\nNode found.\n%d",temp->n);
				break;
			case 4: printf("Are you sure you want to quit(y/n)");
				char ch;
				scanf("%c",&ch);
				if(ch!='y') choice =0;
		}
	}
	return 0;
}

void display(node *ptr)
{
	printf("\n");
	idx =0;
	void BFS(node*,int,int*,int*);
	void findDepth(node*,int);
	depth = 0;
	findDepth(ptr,0);
	int *Ordered = malloc(sizeof(int)*(1<<(depth+1)));
	int *keys    = malloc(sizeof(int)*(1<<(depth+1)));
	BFS(ptr,1,Ordered,keys);
	int hgt = 1;
	for(int temp=idx;temp>0;temp>>=1,hgt++);

	for(int j =0;j<hgt;j++,printf("\n%*c",(depth>>3),'\n'))
	for(int i =0;i<idx;i++)
		if(keys[i]!=j+1)
			printf("   ");
		else
			printf("%3d",Ordered[i]);
	printf("\n\nBinary Search Tree:\n\nMain Menu  :\n");
	printf("\n\t1) Insert new Node");
	printf("\n\t2) Delete  a  Node");
	printf("\n\t3) Search for Node");
	printf("\n\t4) Exit	      ");
	printf("\nEnter your choice : ");
	free(Ordered); free(keys);
}
void findDepth(node *ptr,int n)
{
	if(n> depth) depth = n;
	if(ptr== NULL)
		return;
	findDepth(ptr->left,n+1);
	findDepth(ptr->right,n+1);

}
void BFS(node *ptr,int n,int* arr,int *keys)
{ 
	if(n>=depth+1) return;
	if(ptr!=NULL)
		BFS(ptr->left,n+1,arr,keys);
	else
		BFS(ptr,n+1,arr,keys);
	if(ptr!=NULL)
	 //printf("%*c%3d", n*(1<<(7/2)),' ',ptr->n );
	 arr[idx]= ptr->n,keys[idx++]=n;
	else
	 arr[idx]= 0x7fffffff,keys[idx++]=-1;//dummy
	if(ptr!=NULL)
		BFS(ptr->right,n+1,arr,keys);
	else
		BFS(ptr,n+1,arr,keys);
}

node* insert(node *ptr)
{
	node *n = malloc(sizeof(node));
	if(n==NULL) {printf("\nOverflow!!!!!!"); return ptr;}
	printf("\nEnter the name : ");
	scanf("%d",&n->n);
	n->left = n->right = NULL;
	if (ptr == NULL) return n;
	node * trav = ptr;
	node *save = trav;

	while(trav != NULL){
		save = trav; 
		if(trav->n < n->n)
			trav = trav->right;
		else if(trav->n > n->n)
			trav = trav->left;
		else { 
			printf("\nERROR!!! Node already exists ."); 
			return ptr;
		}
	}
	if(save->n < n->n) save->right = n;
	else		   save->left  = n;
	
	return ptr;
}

node* search(node *ptr)
{
	printf("\nEnter the node to be searched for :");
	int n;
	scanf("%d",&n);

	while(ptr != NULL)
	{
		if(ptr->n < n)
			ptr= ptr->right;
		else if(ptr->n > n)
			ptr = ptr->left;
		else
			return ptr;
	}
	return ptr;
}

node *delete(node *ptr)
{
	node *top=ptr;
	printf("\nEnter the node to be deleted :");
	int n;
	scanf("%d",&n);
	if(ptr == NULL) {printf("\nNode not found."); return NULL; }

	node *trav = ptr;
	node *save = trav;
	int l=-1; //If trav happens to be the root
	//first we find the node to delete.
	while(trav != NULL) 
	{
		save = trav;//parent of the node to delete
		//remember if left child or right child of parent.
		if(trav->n > n)
			trav = trav->left ,l=1; 
		else if(trav->n < n)
			trav = trav->right,l=0;
		if(trav->n==n)
			break;
	}
	if(trav == NULL) { printf("\nNode not found."); return ptr; } 
	printf("\n%d %d :)\n",trav->n,save->n);
	//Now as seen in CLRS we break it into three cases:
	//First if there are no children:
	if(trav->left ==NULL && trav->right == NULL)
	{
		if(l==-1)  ptr = NULL; //If trav is root
		else if(l) save->left = NULL;
		else       save->right= NULL;
		free(trav);
		return ptr;
	}
	//Second if there is a single child:
	if(trav->left == NULL || trav->right == NULL) 
	{
		//stores the child of trav (only one possible)
		node* child = trav->right== NULL?trav->left:trav->right;

		if(l == -1) //if trav is the root
			ptr = trav->left == NULL?trav->right:trav->left;
		else if(l == 1)
			save->left = child;
		else
			save->right= child;
		free(trav);
		return ptr;
	}
	//Third, if there are two children:
	//if trav->right has no left subtree:
	if(trav->right->left == NULL)
	{
		if(l == -1) //if trav is the root
			ptr = trav->right;
		else if(l == 1)
			save->left = trav->right;
		else
			save->right= trav->right;
		trav->right->left = trav->left;
		free(trav);
		return ptr;
	}
	//Last case Smallest Right Child:
	//(can't be null, since we took care of that just before this.)
	node *SmallRC = trav->right->left,*itsParent = trav->right;
	//find successor node (left most child in right subtree)
	while(SmallRC->left != NULL)
		itsParent = SmallRC,SmallRC = SmallRC->left;
	//change connections
	itsParent->left = SmallRC->right;
	SmallRC->right = trav->right;
	SmallRC->left  = trav->left ;
	if(l == -1) ptr = SmallRC;
	else if(l==1) save->left = SmallRC;
	else save->right = SmallRC;
	//all connections rewired.
	return ptr;

}
