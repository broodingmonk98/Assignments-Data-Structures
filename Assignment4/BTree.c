#include <stdio.h>
#include <stdlib.h>
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

//Main
int main()
{
	//creating a BTree or opening one if it exists
	node root,*temp;
	temp = BTreeCreate();
	root = *temp;
	free(temp); //We want to always keep the root with us, without using dynamic memory allocation
	while(1)
	{
		printf("\nSize of tree:%d\n",root.n);
		for(int i=0;i<root.n;i++)
			printf("%10d",root.key[i]);
		printf("\n");
		int n;
		scanf("%d",&n);
		if(n==-1)
			break;
		temp = malloc(sizeof(node));
		*temp = root; //we don't pass root itself since we might end up freeing it
		root = *BTreeInsert(temp,n);
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
	DiskWrite(x,-1);
	return x;

}

//supporting functions:
int DiskWrite(node *x,int n)
{
	FILE *fp;
	if(n == -1){
	fp = fopen("Data/nodes.dat","r+");
	if(fp == NULL) fp = fopen("Data/nodes.dat","a");
	fseek(fp,0,SEEK_END);
	if(fp == NULL) return -1;
	int offset = ftell(fp); 
	x->pos = offset;
	fwrite(x,sizeof(node),1,fp);
	fclose(fp);
	printf("Offset : %d\n", offset);
	return offset;
	}
	else {
	fp = fopen("Data/nodes.dat","r+");
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
	FILE *fp = fopen("Data/nodes.dat","r");
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
	if(y->leaf) //if its not a leaf
		for(j =0;j<TSIZE;j++)
			z->child[j] = y->child[j+TSIZE];
	
	printf("\nWriting z:");
	DiskWrite(z,-1);
	y->n = TSIZE-1;

	for(j=x->n;j>i;j--)
		x->child[j+1] = x->child[j];
	x->child[i+1] = z->pos;
	for(j=x->n-1;j>=i;j--)
		x->key[j+1]=x->key[j];
	x->key[i]=y->key[TSIZE];	
	x->n = x->n+1; 

	printf("\nWriting y%d:",y->pos);
	DiskWrite(y,y->pos);
	printf("\nWriting x");
	DiskWrite(x,-1);
	printf("\nx pos%d:",x->pos);
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
		BTreeSplitChild(s,0); //take care of 0 or 1 business TODO
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
		while(i>=0 && k < head->key[i])
			i--;
		i++;
		node* child = DiskRead(head->child[i]),*newchild;
		if(child->n == TSIZE*2 - 1){
			newchild = BTreeSplitChild(head,i);
			if (k>head->key[i])
				i++;
		}
		if(k>head->key[i])
			child = newchild;
		//or maybe not://free(head); //we are freeing head so should remember the root in the main function separately.
		BTreeInsertNonFull(child,k);
		free(child); if(child!=newchild) free(newchild);
	}
	return 0;
}
