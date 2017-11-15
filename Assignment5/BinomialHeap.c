#include <stdio.h>
#include <stdlib.h> 
#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define BLU   "\x1B[34m"
#define MAG   "\x1B[35m"
#define CYN   "\x1B[36m"
#define WHT   "\x1B[37m"
#define RESET "\x1B[0m"

typedef struct node{
	int n;
	struct node *child,*sibling;
	int deg;
}node;
node *Nil; //In place of NULL

//function definitions:
node* MakeBHeap();
node* BinomialHeapMin(node*);
void  BinomialLink(node*,node*);
node* BinomialHeapUnion(node*,node*);
node* InsertNode(node*,int);
node* ExtractMin(node*);
void  DecreaseKey(node*,node*,int,node**,int);
node* Delete(node*,int);
void display(node*);

int main()
{
	Nil = malloc(sizeof(node)); //just a dummy
	node* Head;
	Head = MakeBHeap();
	while(1){
		printf("\nEnter the node to be inserted:");
		int n;
		scanf("%d",&n);
		if(n==-1)break;
		Head = InsertNode(Head,n);
		display(Head);
	}
	printf("\n\ncontinually extracting stuff\n\n");
	while(Head!=Nil)
	{
		int n;
		scanf("%d",&n);
		Head=Delete(Head,n);
		if(n == -1) break;		
		display(Head);
	}

	free(Nil);
	return 0;
}	
node *MakeBHeap()
{
	return Nil;
}

node* BinomialHeapMin(node*head)
{
	node *y = Nil;
	int min = 0x7fffffff;
	while(head!=Nil)
	{
		if(head->n < min)
			min = head->n,y = head;
		head = head->sibling;
	}
	return y;
}

void  BinomialLink(node*x,node*y)
{
	y->deg++; //Linking x to y increases y's degree by one
	x->sibling = y->child; //X is now left most child of y
	y->child = x;
} 
node* BinomialHeapUnion(node*h1,node*h2)
{
	Nil->sibling = Nil;
	Nil->deg  = 0;
	if(h1 == Nil) return h2; //trivial case
	else if(h2 == Nil) return h1;
	node* h = MakeBHeap(); //head of new heap
	//Merging h1 and h2:
	//assigning  the first element to h
	if(h1->deg<h2->deg) h = h1,h1= h1->sibling;
	else     h = h2,h2=h2->sibling;
	node *temp = h;
	while(h1!=Nil && h2!=Nil)
	{
		if(h1->deg < h2->deg)
			temp->sibling = h1,temp=h1,h1=h1->sibling;
		else
			temp->sibling = h2,temp=h2,h2=h2->sibling;
	}
	//merging the remaining elements (after one of the lists runs out)
	while(h1!=Nil){ temp->sibling = h1, temp=h1; h1 = h1->sibling; }
	while(h2!=Nil){ temp->sibling = h2, temp=h2; h2 = h2->sibling; }
	//done merging h1 and h2.

	node *prev=Nil,*next=h->sibling;
	temp = h;
	while(next!=Nil) //similar to summing two numbers(binary)
	{
		if(temp->deg!=next->deg || (next->sibling!=Nil && next->sibling->deg == temp->deg)) 
			prev = temp,temp = next;
		else if(temp->n< next->n)
			temp->sibling=next->sibling,BinomialLink(next,temp);
			else{ if(prev==Nil)
				h = next;
			      else
				prev->sibling = next;
			      BinomialLink(temp,next);
			      temp = next;
			}
		next = temp->sibling;

	}
	return h;

}

node* InsertNode(node* h,int n)
{
	node *h1=MakeBHeap();
	h1 = malloc(sizeof(node));
	h1->n = n;
	h1->deg = 0;
	h1->sibling = h1->child = Nil;
	return BinomialHeapUnion(h,h1);
}

node* ExtractMin(node* h)
{
	//Finding root with minimum value
	node* trav = h,*travPrev=Nil;
	node* min=Nil; min->n = 0x7fffffff;
	node* prev = Nil;
	while(trav!=Nil){
		if(trav->n < min->n)
			prev=travPrev,min = trav;
		travPrev = trav;
		trav=trav->sibling;
	}
	//Breaking out that root from the linked list h.
	if(prev!=Nil)
		prev->sibling = min->sibling;
	else
		h = min->sibling;

	//making a linked list of children of min
	node *h1 = MakeBHeap();
	h1 = min->child;
	//reversing this list
	trav=h1;
	node *temp=trav->sibling;
	trav->sibling = Nil;
	while(temp!=Nil)
	{
		node* temp1 = temp->sibling;
		temp -> sibling = trav;
		trav = temp;
		temp = temp1;
	}
	h1 = trav;
	return BinomialHeapUnion(h,h1);
}

void DecreaseKey(node*h,node*x,int k,node **parent,int n)
{
	//Since we don't store a parent, we need an array of ancestors 
	//of x (parent) and its size
	if(k>x->n){
		printf("\nError!!! New Key is larger than the old one\n");
		return;
	}
	while(n>0&& k < parent[n-1]->n) //if the parent is larger then we swap
	{
		//swapping the values in x and its parent
		int temp = parent[n-1]->n;
		parent[n-1]->n = x->n;
		x->n = temp;
		x = parent[n-1]; //moving x one unit up
		n--; //the parent moves up one unit as well

	}

}

node* Delete(node* h,int k)
{ //if have to delete node with value k from the binomial heap
	//First we find the node: O(n)
	if(h==Nil) {printf("\nUnsuccessful\n"); return h;}

	//array of ancestors of node with k
	node **prt = malloc(sizeof(node*)*20); //Assuming max depth 20
	printf("Just allocated prt\n");

	//Shall find k in binomial heap h, and return parent list
	int FindKey(node *h,int k,node** prt,int i);
	int n = FindKey(h,k,prt,0);
	printf("FindKey returned\n");
	if(n == -1){ //key k not found
		printf("\nKey not found. \n");	
		return h;
	}
	//decreasing the key 
	for(int i =0;i<=n;i++)
		printf("\ntra :%d",prt[i]->n);
	printf("\n");
	DecreaseKey(h,prt[n],0xffffffff,prt,n);
	free(prt);
	return ExtractMin(h);

}
int FindKey(node *h,int k,node** prt,int i)
{
	if(i>20) return -1; //prt size is at max 20;
	node* trav=h;
	int check=-1;
	printf("level: %d key: %d",i,trav->n);
	for(trav=h; trav!=Nil; trav= trav->sibling)
	{
		if(trav->n == k){ //if the key is found
			prt[i] = trav;
			return i;
		}
		else if(trav->n > k)
			continue; //can't be in that subtree
		else{
			prt[i] = trav;
			check =FindKey(trav->child,k,prt,i+1);
		}
		if(check!= -1)//Case 2:key in subtree trav(return immediately)
			return check;

	}
	//Case 2: if the key can't be in this list
	if( trav == Nil) //only other possibility
		return -1;
	//shouldn't reach this point
	printf("BLAHBLAHBLAH\n");
	return -1;
}


void display(node* h)
{
	printf(YEL"RootList: ");
	node *trav=h;
	while(trav!=Nil){
		printf("%3d %*c",trav->n,trav->deg * 4,'-');
		trav=trav->sibling;
	}
	trav = h;
	printf(RESET"\n         "); 
	void displayChildren(node*,int);
	while(trav!=Nil)
	{
		node*trav2 = trav->child;
		printf("\nCHILDREN OF %d:\n",trav->n);
		//while(trav2!=Nil)
		//	printf("%d ",trav2->n),trav2=trav2->sibling;
		displayChildren(trav2,1);
		printf("\n_______________________________\n");
		trav = trav->sibling;

	}

}
void displayChildren(node*x,int level)
{
	if(x == Nil) return;
	node *trav = x;
	//changing color based on level
	if(level%4 == 1) printf(GRN);
	else if(level%4==2) printf(BLU);
	else if(level%4==3) printf(MAG);
	else printf(CYN);

	while(trav!=Nil)
		printf("%3d",trav->n),trav=trav->sibling;
	printf(RESET); //Resetting color

	trav = x;
	while(trav!=Nil){
		if(trav->child != Nil)
			printf("\n%3d:",trav ->n),
			displayChildren(trav->child,level+1);
		trav=trav->sibling;
	}
	
}
