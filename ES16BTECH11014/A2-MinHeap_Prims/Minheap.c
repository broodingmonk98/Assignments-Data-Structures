//since all the functions are small we shall implement them in main itself
//min heap
#include <stdio.h>
#include <stdlib.h>
#define BLU   "\x1B[34m"
#define RESET "\x1B[0m"

int main()
{
	int choice=0,n=100,idx = 0;
	int *arr = malloc(n);
	
	while(choice != 4)
	{
		printf("\nHeap:\n"BLU);
		for(int i=0,disp=2;i<idx;i++){
			if(i>disp-2)
				printf("\n"),disp<<=1;
			printf("%2d ",arr[i]);
		}
		printf(RESET"\n");

		printf("\nMain Menu:\n1) Insert\n2) Find Min\n3) Extract Min\n4) Exit");
		printf("\nEnter your choice :");
		scanf("%d",&choice); 
		if(choice == 1){
			printf("\nEnter the element to insert :");
			scanf("%d",&choice);

			//Inserting it into heap arr
			arr[idx++]=choice;

			//maintaining heap property
			for(int i=idx-1;i>0;i=(i-1)/2)
				if(arr[i]<arr[(i-1)/2]){
					//swapping
					choice = arr[i];
					arr[i] = arr[(i-1)/2];
					arr[(i-1)/2] = choice;
				}
			choice = 0;

			if(idx>n){//if we run out of memory
				n*=2;
				arr= realloc(arr,n);
			}

		}
		else if(choice == 2){
			printf("\nMin :%d\n",arr[0]);
		}
		else if(choice == 3){
			arr[0] = arr[--idx]; //overwriting min with last element
			//maintaining heap property
			//making it bubble down
			for(int i=0,l,r,min;i*2+1<idx;i=min){
				l = i*2+1; r = i*2+2;
				min=i;
				if(l<idx && arr[l]<arr[i])
					min = l;
				if(r<idx && arr[r]< arr[min])
					min = r;

				if(min == i) break;
				//swapping i with min
				choice = arr[min];
				arr[min] = arr[i];
				arr[i] = choice;
				

			}
			choice = 0;
		}

		
	}
	printf("\nThank you, have a nice day!!!\n");
	return 0;
}
