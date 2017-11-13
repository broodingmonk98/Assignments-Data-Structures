#include <stdio.h>
#include <stdlib.h>
//for colored output
#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define BLU   "\x1B[34m"
#define MAG   "\x1B[35m"
#define CYN   "\x1B[36m"
#define WHT   "\x1B[37m"
#define RESET "\x1B[0m"

#define TSIZE 2
#define NoOfKEYS 3
#define NoOfCHILD 4

typedef struct node{
	int leaf;
	int pos; //stores the nodes position in the file (for writing it back)
	int n;
	int key[NoOfKEYS];
	int child[NoOfCHILD]; //stored as offsets in file

} node;

//function definitions:

node* BTreeCreate();
node* BTreeSplitChild(node*,int);
node* BTreeInsert(node *,int);
int BTreeInsertNonFull(node *,int);
int DiskWrite(node*,int);
node *DiskRead(int); 
void display(node*,int);
int delete(node*,int);

//Main
int main()
{
	//creating a BTree or opening one if it exists
	node root,*temp;
	temp = BTreeCreate();
	root = *temp;
	free(temp); //We want to always keep the root with us, without using dynamic memory 
	while(1)
	{
		display(&root,0);
		int n;
		scanf("%d",&n);
		if(n==-1)
			break;
		temp = malloc(sizeof(node));
		*temp = root; //we don't pass root itself since we might end up freeing it
		temp = BTreeInsert(temp,n);
		root = *temp;
		free(temp);
	}
	while(1)
	{
		display(&root,0);
		int n;
		scanf("%d",&n);
		if(n==-1)
			break;
		delete(&root,n);
	}
	FILE *fp = fopen(".rootpos","w");
	fwrite(&(root.pos),sizeof(int),1,fp);
	fclose(fp);
	
	return 0;
}
node *BTreeCreate() //returns the node created (it is the root)
{
	int n;
	node *x;
	//If Btree is already in memory
	FILE *fp = fopen(".rootpos","r");
	if(fp!=NULL) {
	fread(&n,sizeof(int),1,fp);
	x = DiskRead(n); 
	fclose(fp);
	printf("Already present :%d",n);
	return x;
	}
	x = malloc(sizeof(node)); //if its not there, create it
	x->leaf = 1;//it is a leaf
	x->n	= 0;
	//for (int abcd = 0;abcd<2*TSIZE+1;abcd++) x->child[abcd] = -1;
	DiskWrite(x,-1);
	return x;

}

//supporting functions:
int DiskWrite(node *x,int n)
{
	FILE *fp;
	if(n == -1){
	fp = fopen(".nodes.dat","a");
	if(fp == NULL) return -1;
	int offset = ftell(fp); 
	x->pos = offset;
	fwrite(x,sizeof(node),1,fp);
	fclose(fp);
	return offset;
	}
	else {
	fp = fopen(".nodes.dat","r+");
	if(fp == NULL) {printf("\nERROR. File doesn't exist!!!\n"); return -1; }
	fseek(fp,n,SEEK_SET);
	int offset = ftell(fp); 
	fwrite(x,sizeof(node),1,fp);
	fclose(fp);
	printf("Offset : %d\n", offset);
	return offset;
	}
}
node *DiskRead(int n) 
{ //Returns the node with the offset n in the given file
	FILE *fp = fopen(".nodes.dat","r");
	if(fp == NULL) { printf("ERROR!!!"); return NULL; }
	fseek(fp,n,SEEK_SET);
	node *x = malloc(sizeof(node));
	fread(x,sizeof(node),1,fp);
	fclose(fp);
	return x;
}
node *BTreeSplitChild(node*x ,int i)
{
	node* z = malloc(sizeof(node));
	int j;
	node* y = DiskRead(x->child[i]);
	z->leaf = y->leaf;
	z->n = TSIZE - 1;
	for(j=0;j<TSIZE - 1;j++)
		z->key[j] = y->key[TSIZE + j];
	if(y->leaf==0){ //if its not a leaf
		for(j =0;j<TSIZE;j++)
			z->child[j] = y->child[j+TSIZE];
	}else
		for(j =0;j<TSIZE;j++)
			z->child[j] = -1; 
	
	DiskWrite(z,-1);

	y->n = TSIZE-1;
	for(j=x->n;j>=i;j--) //CHECK THIS
		x->child[j+1] = x->child[j];
	x->child[i+1] = z->pos;
	for(j=x->n-1;j>=i;j--)
		x->key[j+1]=x->key[j];
	x->key[i]=y->key[TSIZE-1];
	x->n = x->n+1; 

	DiskWrite(y,y->pos);
	DiskWrite(x,x->pos);
	free(y);
	return z; //the new node

	//still have to take care of freeing stuff (if anything has to be freed that is
}

node* BTreeInsert(node *head,int x) //inserts x into head 
{
	if(head->n == 2*TSIZE-1) //if the root is full
	{
		node *s = malloc(sizeof(node)); // we are creating a new root
		s->leaf = 0; //not a leaf
		s->n = 0; //check
		s->child[0] = head->pos;

		for (int abcd = 1;abcd<=2*TSIZE-1;abcd++) s->child[abcd] = -1;

		s->pos = -1; //it has not yet been written to disk
		BTreeSplitChild(s,0); 
		BTreeInsertNonFull(s,x);
		//free(head);
		return s; //new root 
	}
	BTreeInsertNonFull(head,x);
	return head; //the root is unchanged
}
int BTreeInsertNonFull(node *head, int k)
{
	int i = head->n-1;
	if(head->leaf) //if its a leaf node
	{
		while(i>=0 && k > head->key[i]){
			head->key[i+1] = head->key[i];
			i--;
		}
		head->key[i+1] = k;
		head->n = head->n+1;
		DiskWrite(head,head->pos);
	}
	else
	{ 
		while(i>=0 && k > head->key[i])//CHANGED
			i--;
		i++;
		node* child = DiskRead(head->child[i]),*newchild;
		if(child->n == TSIZE*2 - 1){
			newchild = BTreeSplitChild(head,i);
			child    = DiskRead(head->child[i]);

			printf("\nHIHIHI: %d\n",child->n);
			if(k < head->key[i])
			{	free(child); child = newchild; }
			else free(newchild);
			BTreeInsertNonFull(child,k);
			free(child);
			return 0;
		}
		//or maybe not://free(head); //we are freeing head so should remember the root in the main function separately.
		BTreeInsertNonFull(child,k);
		free(child); 
	}
	return 0;
}

void display(node *x,int level)
{
	int n = x->n;
	if(level == 0)  printf("ROOT @%4d:",x->pos);
	else 		printf(" @%4d :",x->pos);
	if(level%4 == 0)	printf("\n"RED);
	if(level%4 == 1)	printf("\n"YEL);
	if(level%4 == 2)	printf("\n"BLU);
	if(level%4 == 3)	printf("\n"GRN);
	for(int i=0;i<n;i++)
		printf("%6d",x->key[i]);
	printf("\n"RESET);
	

	if(!(x->leaf)){ //if its not a leaf
	for(int i=0;i<=n;i++)
		if(x->child[i] != -1){
			printf("Child of %d at level %d :",x->pos,level+1);
			display(DiskRead(x->child[i]),level+1);
		}
	}else
		printf("---------------------\n");

	if(level) free(x);
}

int delete(node *x,int k)
{
	//we assume that the node we have descended into has atleast t 
	//as opposed to t-1 elements condition for the BTree
	int i = x->n - 1;
	int j;
	for(j = 0;j<=i;j++)
		if(x->key[j] == k)
			break;
	if(x->leaf) //if its a leaf
	{
		//delete the key from the leaf directly	
		if(j>i) //if the key doesn't exist
		{printf("\nKey Not Found"); return -1; }
		//if the key exists shift the elements after key one unit left
		while(j<i)
			x->key[j] = x->key[j+1],j++;
		printf("\nkey successfully deleted");
		x->n--;
		DiskWrite(x,x->pos);
		return 0;
		
	}

	if (x->leaf==0 && j<=i) //if its an internal node and contains k
	{
		//if child preceeding k has atleast t keys, find predecessor 
		//of k in the tree recursively and replace k by its 
		//predecessor deleting k each time

		node *leftChild = DiskRead(x->child[j]);
		if(leftChild -> n >= TSIZE)
		{
			//substitute key[j]'s predecessor in place of key[j]
			//TODOwhile(leftChild->n
			x->key[j] = leftChild->key[leftChild->n - 1];
			DiskWrite(x,x->pos);
			delete(leftChild, x->key[j]);
			//free(x);
			return 0;
		} 
		//if child suceeding  k has atleast t keys, find successor 
		//of k in the tree recursively and replace k by its successor
		//deleting k each time
		node *rightChild =DiskRead(x->child[j+1]);
		if(rightChild -> n >= TSIZE)
		{
			//substitute key[j]'s successor in place of key[j]
			x->key[j] = rightChild->key[0];
			DiskWrite(x,x->pos);
			delete(rightChild, x->key[j]);
			//free(x);
			return 0;
		}
		//if both have TSIZE-1 keys
		//merging everything into leftChild
		leftChild->n = TSIZE*2-1;
		leftChild->key[TSIZE-1] = k;
		for(int tempI = 0;tempI<TSIZE-1;tempI++)
			leftChild->key[TSIZE+tempI] = rightChild->key[tempI];
		for(int tempI = 0;tempI<TSIZE;tempI++)
			leftChild->child[TSIZE+tempI] =rightChild->child[tempI];
		DiskWrite(leftChild,leftChild->pos);
		//shifting things in x and throwing away pointer to rightChild
		for(int tempI = j;tempI<i;tempI++){
			x->key[tempI]    = x->key[tempI+1];
			x->child[tempI+1]=x->child[tempI+2];
		}
		x->n--;
		DiskWrite(x,x->pos);
		delete(leftChild,k);
		return 0;

	}

	if (x->leaf==0 && j>i) //This has to be the only there case
	{//if x is internal node and does not contain k
		//find the child that could contain k
		j=0;
		while(j<=i&&x->key[j] > k)
			j++;
		node *child = DiskRead(x->child[j]);
		//if the child has atleast t keys descend into it
		if(child->n >= TSIZE){
			delete(child,k);
			return 0;
		}
		//if the child has t-1 keys then check if an immediate sibling
		//has t keys, and 'borrow' a key from a sibling if one 
		//such sibling exists
		if(j>0){ 
			node *left = DiskRead(x->child[j-1]);
			if(left->n>=TSIZE)
			{
				//shift everything one unit to the right
				for(int tempI=TSIZE-1;tempI>0;tempI--){
					child->key[tempI]=child->key[tempI-1];
					child->child[tempI+1] = child->child[tempI];
				} child->child[1]=child->child[0];
				child->key[0]  = x->key[j-1];
				x->key[j-1] = left->key[left->n - 1];
				child->child[0]= left->key[left->n];
				left->n--;
				DiskWrite(left,left->pos);
				DiskWrite(x,x->pos);
				DiskWrite(child,child->pos);
				//because child now has t children
				delete(child,k);
				return 0;
			}
		}else if(j<=i)
		{
			node *right = DiskRead(x->child[j+1]);
			if(right->n>=TSIZE)
			{
				child->key[TSIZE-1]  = x->key[j];
				child->child[TSIZE]  = right->child[0];
				x->key[j] = right->child[0];
				//shift everything one unit to the left in right
				for(int tempI=0;tempI<right->n-1;tempI++){
					right->key[tempI]=right->key[tempI+1];
					right->child[tempI] = right->child[tempI+1];
				} right->child[right->n-1]=right->child[right->n];
				right->n--;
				DiskWrite(right,right->pos);
				DiskWrite(x,x->pos);
				DiskWrite(child,child->pos);
				//because child now has t children
				delete(child,k);
				return 0;
			}
		}
		//if the child has t-1 keys and both its siblings have t-1 keys,
		//merge the child with one of its siblings, and then descend into it
		if(j<=i) //to take care of j==0(Case 1:)
		{
			node *right = DiskRead(x->child[j+1]);
			if(right->n==TSIZE-1)
			{
				//merging child and right
				child->key[TSIZE-1]=x->key[j];
				for(int tempI=TSIZE;tempI<2*TSIZE-1;tempI++)
				{
					child->key[tempI] = right->key[tempI-TSIZE];
					child->child[tempI] = right->child[tempI-TSIZE];
				}child->child[TSIZE*2-1] = right->child[TSIZE-1];
				//shifting things in x
				while(j<=i-1){
					x->key[j]=x->key[j+1];
					x->child[j+1]=x->child[j+2];
				}
				x->n--;
				child->n=TSIZE*2-1;
				DiskWrite(x,x->pos);
				DiskWrite(child,child->pos);
				delete(child,k);

			}

		}else //if j == i+1 that is the rightmost child of x
		{
			node *left= DiskRead(x->child[j-1]);
			if(left->n==TSIZE-1)
			{
				//merging child and right
				left->key[TSIZE-1]=x->key[j-1];
				for(int tempI=TSIZE;tempI<2*TSIZE-1;tempI++)
				{
					left->key[tempI] = child->key[tempI-TSIZE];
					left->child[tempI] = child->child[tempI-TSIZE];
				}left->child[TSIZE*2-1] = child->child[TSIZE-1];
				//shifting things in x
				while(j<=i-1){
					x->key[j-1]=x->key[j];
					x->child[j]=x->child[j+1];
				}
				x->n--;
				left->n=TSIZE*2-1;
				DiskWrite(x,x->pos);
				DiskWrite(left,left->pos);
				delete(left,k);

			}

		}
	}
	return 5;
} 
