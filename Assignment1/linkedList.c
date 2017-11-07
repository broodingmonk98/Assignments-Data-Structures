#include <stdio.h>
#include <stdlib.h>
typedef struct node
{
	int n;
	struct node *next;
} node;
void display(node*);
node* search(node*);
node* delete(node*);
node* insert(node*); 
int main()
{
	node *top = NULL;
	node *temp;
	int choice=0;
	while(choice != 4){
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
}

void display(node *ptr)
{
	printf("\n");
	while(ptr!=NULL) printf("%d ->",ptr->n),ptr=ptr->next;
	printf("Linked List\n\nMain Menu  :\n");
	printf("\n\t1) Insert new Node");
	printf("\n\t2) Delete  a  Node");
	printf("\n\t3) Search for Node");
	printf("\n\t4) Exit	      ");
	printf("\nEnter your choice : ");
}

node* insert(node *ptr)
{
	node *n = malloc(sizeof(node));
	if(n==NULL) {printf("\nOverflow!!!!!!"); return ptr;}
	printf("\nEnter the name : ");
	scanf("%d",&n->n);
	n->next=ptr;
	return n;
}

node* search(node *ptr)
{
	printf("\nEnter the node to be searched for :");
	int n;
	scanf("%d",&n);

	while(ptr != NULL)
	{
		if(ptr->n == n)
			return ptr;
		else
			ptr = ptr->next;
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
	if(ptr->n == n) 
	{ printf("\nNode successfully deleted."); 
	  node *temp = ptr->next;
	  free (ptr);
	  return temp; 
	}
	while(ptr->next!=NULL)
		if(ptr->next->n == n)
		{
			node *temp=ptr->next;
			ptr->next = temp->next;
			free(temp);
			printf("\nNode successfully deleted.");
			return top;
		}
		else ptr = ptr->next;
	printf("\nNode not found.");
	return ptr;
}
