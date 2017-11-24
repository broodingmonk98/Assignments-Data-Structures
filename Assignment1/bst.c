#include <stdio.h>
#include <stdlib.h>
#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define BLU   "\x1B[34m"
#define RESET "\x1B[0m"

typedef struct node
{
	int n;
	struct node *left,*right;
} node;

//Function Definitions
void display(node*); // display the tree
node* search(node*); // search for a particular node
node* delete(node*); // delete a node
node* insert(node*); // insert a node

//Global Variables
int depth; //used for convenience in the display function
int idx; //to store index (also used in display)

int main()
{
	node *top = NULL; // stores head of the array
	node *temp;  //just used as a dummy variable
	int choice=1;
	char ch='\0'; //Another dummy variable

	//menu:
	while(choice != 0){
		if(choice != 4)
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
			case 4: printf("Are you sure you want to quit(y/n)\n");
				scanf("%c",&ch);
				if(ch=='y') choice =0;
				break;

			default: printf("\nNot a valid option!!!! Reenter\n");
		}
	}
	//end of menu

	printf("Deleting Tree:\n");

	void flushMemory(node*); //function to delete the tree.
	flushMemory(top);
	printf("Done!!! Have a nice day :)\n");
	//memory freed

	return 0;
}

void display(node *ptr)
{
	printf("\n");
	idx =0;

	//supporting functions
	void BFS(node*,int,int*,int*); //actually DFS, it finds all the nodes
	void findDepth(node*,int);  //it figures out the depth of the tree (to allocate memory)

	depth = 0;
	findDepth(ptr,0);

	int *Ordered = malloc(sizeof(int)*(1<<(depth+1)));
	int *keys    = malloc(sizeof(int)*(1<<(depth+1)));

	//extracts the nodes in order (think of it like flattening the tree)
	BFS(ptr,1,Ordered,keys); 

	int hgt = 1; //height of the tree
	for(int temp=idx;temp>0;temp>>=1,hgt++);

	for(int j =0;j<hgt;j++,printf("\n%*c",(depth>>3),'\n')){

		if(j%4 == 0)
			printf(RED);
		else if(j%4 == 1)
			printf(GRN);
		else if(j%4 == 2)
			printf(BLU);
		else if(j%4 == 3)
			printf(YEL);

		for(int i =0;i<idx;i++)
			if(keys[i]!=j+1)
				printf("   ");
			else
				printf("%3d",Ordered[i]);

		printf(RESET);
	}	

	printf("\n\nBinary Search Tree:\n\nMain Menu  :\n");
	printf("\n\t1) Insert new Node");
	printf("\n\t2) Delete  a  Node");
	printf("\n\t3) Search for Node");
	printf("\n\t4) Exit	      ");
	printf("\nEnter your choice : ");

	free(Ordered); 
	free(keys);
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

node* insert(node *ptr) //returns head of tree after insert operation
{
	node *n = malloc(sizeof(node));
	if(n==NULL) {printf("\nOverflow!!!!!!"); return ptr;}

	printf("\nEnter the name (integer): ");
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
	//returns pointer to node with required key
	return ptr;
}

node *delete(node *ptr)
{
	node *top=ptr;
	printf("\nEnter the node to be deleted :");
	int n;
	scanf("%d",&n);
	//trivial case:
	if(ptr == NULL) {printf("\nNode not found."); return NULL; }

	node *trav = ptr;
	node *save = trav;
	int l=-1; //If trav happens to be the root
	//first we find the node to delete.
	while(trav != NULL) 
	{
		save = trav;//parent of the node to delete
		//remember if left child or right child of parent using l.
		if(trav->n > n)
			trav = trav->left ,l=1; 
		else if(trav->n < n)
			trav = trav->right,l=0;

		if(trav!= NULL && trav->n==n)
			break;
	}

	if(trav == NULL) { 
		printf("\nNode not found."); 
		return ptr; 
	} 

	//Now as seen in CLRS we break it into three cases:
	//First if there are no children:
	if(trav->left ==NULL && trav->right == NULL)
	{
		if(l==-1)  //If trav is root
			ptr = NULL; 
		else if(l) //If trav is left child
			save->left = NULL;
		else       //If trav is right child
			save->right= NULL;

		free(trav);
		return ptr;
	}
	//Second if there is a single child:
	if(trav->left == NULL || trav->right == NULL) 
	{
		//stores the child of trav (only one possible)
		node* child = trav->right== NULL?trav->left:trav->right;

		if(l == -1) //if trav is the root
			ptr = child;
		else if(l == 1) //making parent with parent
			save->left = child;
		else
			save->right= child;

		free(trav);
		return ptr;
	}

	//Thirdly, if there are two children:
	if(trav->right->left == NULL) //if trav->right has no left subtree
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
		itsParent = SmallRC,
		SmallRC = SmallRC->left;

	//change connections
	itsParent->left = SmallRC->right;
	SmallRC->right = trav->right;
	SmallRC->left  = trav->left ;
	if(l == -1) 
		ptr = SmallRC;
	else if(l==1) 
		save->left = SmallRC;
	else 
		save->right = SmallRC;
	//all connections rewired.

	return ptr;

}
void flushMemory(node* top) //recursively deletes nodes
{
	if(top==NULL) return;
	flushMemory(top->left); 
	flushMemory(top->right);
	free(top);
}
