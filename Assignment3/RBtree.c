#include <stdio.h>
#include <stdlib.h>
#define RED   "\x1B[31m"
#define RESET "\x1B[0m"
//to color red nodes red

typedef struct node
{
	int n;
	struct node *left,*right;
	//1 for red and 0 for black
	int color;
} node;
node *nil;
//To display the tree in all its glory
void display(node*);
//To search the tree to find an element
node* search(node*);
//Delete a node from the tree
node* delete(node*);
node* RBDeleteFixUp(node*,node **,int *,int);

//Insert a node into the tree
node* insert(node*); 
node* RBInsertFixUp(node*,node **,int *,int);

//Supporting functions and helpful variables.
int leftRotate(node* );
int rightRotate(node*);
int depth;
int idx; //to store index

int main()
{
	node dummy;
	dummy.color=0;
	nil = &dummy;
	node *top = nil;
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
				if(temp== nil) 
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
	void BFS(node*,int,int*,int*,int*);
	void findDepth(node*,int);
	depth = 0;
	findDepth(ptr,0);
	int *Ordered = malloc(sizeof(int)*(1<<(depth+1)));
	int *colored = malloc(sizeof(int)*(1<<(depth+1)));
	int *keys    = malloc(sizeof(int)*(1<<(depth+1)));
	BFS(ptr,1,Ordered,colored,keys);
	int hgt = 1;
	for(int temp=idx;temp>0;temp>>=1,hgt++);

	for(int j =0;j<hgt;j++,printf("\n%*c",(depth>>3),'\n'))
	for(int i =0;i<idx;i++)
		if(keys[i]!=j+1)
			printf("   ");
		else{
			if(colored[i] == 1)
				printf(RED "%3d" RESET,Ordered[i]);
			else
				printf("%3d",Ordered[i]);
		}
	printf("\n\nBinary Search Tree:\n\nMain Menu  :\n");
	printf("\n\t1) Insert new Node");
	printf("\n\t2) Delete  a  Node");
	printf("\n\t3) Search for Node");
	printf("\n\t4) Exit	      ");
	printf("\nEnter your choice : ");
	free(Ordered); free(keys); free(colored);
}
void findDepth(node *ptr,int n)
{
	if(n> depth) depth = n;
	if(ptr== nil)
		return;
	findDepth(ptr->left,n+1);
	findDepth(ptr->right,n+1);

}
void BFS(node *ptr,int n,int* arr,int* colored,int *keys)
{ 
	if(n>=depth+1) return;
	if(ptr!=nil)
		BFS(ptr->left,n+1,arr,colored,keys);
	else
		BFS(ptr,n+1,arr,colored,keys);
	if(ptr!=nil)
	 //printf("%*c%3d", n*(1<<(7/2)),' ',ptr->n );
	 arr[idx]= ptr->n,colored[idx]=ptr->color,keys[idx++]=n;
	else
	 arr[idx]= 0x7fffffff,keys[idx++]=-1;//dummy
	if(ptr!=nil)
		BFS(ptr->right,n+1,arr,colored,keys);
	else
		BFS(ptr,n+1,arr,colored,keys);
}

node* insert(node *ptr)
{
	//Since we won't be using a parent pointer, as in CLRS, we have to
	//Remember the parents of the inserted node and pass it to RBInsertFixup

	node *n = malloc(sizeof(node));
	if(n==nil) {printf("\nOverflow!!!!!!"); return ptr;}
	printf("\nEnter the name (as a number): ");
	scanf("%d",&n->n);
	n->color = 1; //coloring it red
	n->left = n->right = nil;

	node *(*prt) = malloc(sizeof(node*)*20); int pidx=0; //parent index
	int *lrchild = malloc(sizeof(int)*21);//stores whether left/right child
	//Assuming max height 20, valid as this can store upto 1000 nodes.
	//lrchild is 1 at i if the ith ancestor is a left child.

	if (ptr == nil) {n->color = 0; return n;}
	node * trav = ptr;
	node *save = trav;

	while(trav != nil){
		save = trav; 
		prt[pidx++]= save; //Store the ancestors starting from root

		if(trav->n < n->n)
			trav = trav->right, lrchild[pidx] = 0;
		else if(trav->n > n->n)
			trav = trav->left,  lrchild[pidx] = 1;
		else { 
			printf("\nERROR!!! Node already exists ."); 
			return ptr;
		}
	}
	if(save->n < n->n) save->right = n;
	else		   save->left  = n;
	
	ptr= RBInsertFixUp(n,prt,lrchild,pidx);

	free(prt); free(lrchild);
	return ptr;
}

node* search(node *ptr)
{
	printf("\nEnter the node to be searched for :");
	int n;
	scanf("%d",&n);

	while(ptr != nil)
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
	if(ptr == nil) {printf("\nNode not found."); return NULL; }
	//As in insert, we have to remember the parents.
	node *(*prt) = malloc(sizeof(node*)*20); int pidx=0; //parent index
	int *lrchild = malloc(sizeof(int)*21);//stores whether left/right child
	//Assuming max height 20, valid as this can store upto 1000 nodes.
	//lrchild is 1 at i if the ith ancestor is a left child.

	node *trav = ptr;
	node *save = trav;
	int l=-1; //If trav happens to be the root l=-1
	//first we find the node to delete.
	while(trav != nil) 
	{
		save = trav;//parent of the node to delete
		prt[pidx++]=save; //storing  the ancestors
		//remember if left child or right child of parent.
		if(trav->n > n)
			trav = trav->left ,l=1,lrchild[pidx]=1; 
		else if(trav->n < n)
			trav = trav->right,l=0,lrchild[pidx]=0;
		if(trav->n==n)
			break;
	}
	if(trav == nil) { printf("\nNode not found."); return ptr; } 
	printf("\n%d %d :)\n",trav->n,save->n);

	node *y=trav; //As in CLRS
	int orgColor = y->color;
	node *x=nil; x->color=0;

	//Now as seen in CLRS we break it into three cases:
	//First if there are no children:
	if(trav->left ==nil && trav->right == NULL)
	{
		if(l==-1)  ptr = nil; //If trav is root
		else if(l) save->left = nil;
		else       save->right= nil;
		free(trav); 
		if(orgColor ==0)//if orignal color is black
			RBDeleteFixUp(x,prt,lrchild,pidx);
		return ptr;
	}
	//Second if there is a single child:
	if(trav->left == nil || trav->right == NULL) 
	{
		//stores the child of trav (only one possible)
		x = trav->right== nil?trav->left:trav->right;

		if(l == -1) //if trav is the root
			ptr = x;
		else if(l == 1)
			save->left = x; 
		else
			save->right= x;
		free(trav);
		if(orgColor ==0)//if orignal color is black
			RBDeleteFixUp(x,prt,lrchild,pidx);
		return ptr;
	}
	//Third, if there are two children:
	prt[pidx++] = trav; lrchild[pidx]=0; 
	//if trav->right has no left subtree:
	if(trav->right->left == nil)
	{
		y = trav->right;
		orgColor = y->color;
		x = y->right; 
		if(x==nil) x->color=0; //ensuring nil is black

		if(l == -1) //if trav is the root
			ptr = trav->right;
		else if(l == 1)
			save->left = trav->right;
		else
			save->right= trav->right;
		trav->right->left = trav->left;
		free(trav);
		if(orgColor ==0)//if orignal color is black
			RBDeleteFixUp(x,prt,lrchild,pidx);
		return ptr;
	}
	//Last case Smallest Right Child:
	prt[pidx++]=trav->right; lrchild[pidx]=1;
	//(can't be null, since we took care of that just before this.)
	node *SmallRC = trav->right->left,*itsParent = trav->right;
	//find successor node (left most child in right subtree)
	while(SmallRC->left != nil)
	{
		prt[pidx++]= SmallRC; lrchild[pidx]=1;
		itsParent = SmallRC,SmallRC = SmallRC->left;
	}
	y = SmallRC;
	orgColor = y->color;
	x = y->right; if(x==nil) x->color =0; //ensuring nil is black.
	//change connections
	itsParent->left = SmallRC->right;
	SmallRC->right = trav->right;
	SmallRC->left  = trav->left ;
	if(l == -1) ptr = SmallRC;
	else if(l==1) save->left = SmallRC;
	else save->right = SmallRC;
	//all connections rewired.
	if(orgColor ==0)//if orignal color is black
		RBDeleteFixUp(x,prt,lrchild,pidx);
	return ptr;

}

int leftRotate(node* x)//carries left rotate operation on node at x
{
	node* y = x->right;
	if(y == nil) {
		printf("Error during execution!!! Terminating"); 
		return 0;
	}
	//Swap values in x and y
	int temp = x->n; x->n= y->n; y->n = temp;
	temp = x->color; x->color = y->color; y->color = temp;

	//Changing pointer structures:(from here on x is y and y is x)
	x->right= y->right;
	y->right= y->left;
	y->left = x->left;
	x->left = y;
	//We are now done
	return 1;
}

int rightRotate(node* x) //carries right rotate operation on node at x
{
	node* y = x->left;
	if(y == nil) {
		printf("Error during execution!!! Terminating"); 
		return 0;
	}
	//Swap values in x and y
	int temp = x->n; x->n= y->n; y->n = temp;
	temp = x->color; x->color = y->color; y->color = temp;

	//Changing pointer structures:(from here on x is y and y is x)
	x->left = y->left;
	y->left = y->right;
	y->right= x->right;
	x->right= y;
	//We are now done
	return 1;
}

node* RBInsertFixUp(node *n,node **prts,int *lr,int idx)
{
	while(idx>=1 && prts[idx-1]->color == 1)	//1 stands for red
	{
		if(lr[idx-1]) //If n.parent is a left child : BIG CASE 1:
		{
			node *y = prts[idx-2]->right; //y is uncle of n.
			if(y!= nil && y->color == 1){//case 1 :parent and uncle are red
				prts[idx-1]->color = 0;
				y->color = 0;
				prts[idx-2]->color = 1; //grandparent is red
				idx -= 2; //Make the grandparent the new node
				n = prts[idx];

			}
			else{ //case 2 and 3:

			//case 2: if node is right child, we make it left child.
				if(lr[idx]==0) 
				{//carrying out left rotate 
					leftRotate(prts[idx-1]);
				}//We have now converted case 2 into case 3
			//case 3: if the node is left child:
				printf("TRALAAL %d xx %d\n",prts[idx-1]->n,n->n);
				prts[idx-1]->color = 0; //color parent black
				prts[idx-2]->color = 1;//color grandparent red
				rightRotate(prts[idx-2]);//rightRotate grandpa
				 idx--;
				//The loop shall quit now so no need to make any
				//more changes here
			}
		}
		else 	//If n.parent is a right child: BIG CASE 2:
		{
			node *y = prts[idx-2]->left; //y is uncle of n.
			if(y!=nil && y->color == 1){//case 1 :parent and uncle are red
				prts[idx-1]->color = 0;
				y->color = 0;
				prts[idx-2]->color = 1; //grandparent is red
				idx -= 2; //Make the grandparent the new node
				n = prts[idx];

			}
			else{ //case 2 and 3:

			//case 2: if node is right child, we make it left child.
				if(lr[idx]==1) 
				{//carrying out right rotate 
					rightRotate(prts[idx-1]);
				}//We have now converted case 2 into case 3
			//case 3: if the node is right child:
				prts[idx-1]->color = 0; //color parent black
				prts[idx-2]->color = 1;//color grandparent red
				leftRotate(prts[idx-2]);//leftRotate grandpa
				 idx--;
				//The loop shall quit now so no need to make any
				//more changes here
				
			}
		}
	}
	prts[0]->color = 0;

	return prts[0];
}

node* RBDeleteFixUp(node*n,node **prt,int *lr,int idx)
{
	for(int i =0 ;i<idx;i++)
	{ 
		printf("%d -> ",prt[i]->n );
	}
	nil->color = 0;
	 while(n!=prt[0] && n->color == 0) //0 for black
	{
		//Big CASE1:
		if(lr[idx]) // if n is a left child
		{
			node *w = prt[idx-1]->right;//sibling of n
			if(w->color == 1) //case 1:
			{
				w->color = 0,prt[idx-1]->color = 1;
				leftRotate(prt[idx-1]);
				w = prt[idx-1]->right;
			} nil->color = 0; //just setting nil black now and then to make sure that its color hasn't changed.
			if(w->left->color == 0 && w->right->color==0) //if both children of w are black case 2:
				w->color = 1,n = prt[--idx];

			else if(w->right->color ==0) //case 3:
			{
				w->left->color =0;
				w ->color = 1;
				rightRotate(w);
				w = prt[idx-1]->right;
			}
			w->color = prt[idx-1]->color; //case 4:
			prt[idx-1]->color = 0;
			w ->right->color = 0;
			leftRotate(prt[idx-1]);
			n = prt[0];
		}
		else   //Big CASE 2:
		{
			node *w = prt[idx-1]->left;//sibling of n
			if(w->color == 1) //case 1:
			{
				w->color = 0,prt[idx-1]->color = 1;
				rightRotate(prt[idx-1]);
				w = prt[idx-1]->right;
			} nil->color = 0; //just setting nil black now and then to make sure that its color hasn't changed.
			if(w->left->color == 0 && w->right->color==0) //if both children of w are black case 2:
				w->color = 1,n = prt[--idx];

			else if(w->left->color ==0) //case 3:
			{
				w->right->color =0;
				w ->color = 1;
				leftRotate(w);
				w = prt[idx-1]->left;
			}
			w->color = prt[idx-1]->color; //case 4:
			prt[idx-1]->color = 0;
			w ->right->color = 0;
			rightRotate(prt[idx-1]);
			n = prt[0];
		}
	}
	n->color = 0;
	return prt[0];

}
