#include <stdio.h>
#include <stdlib.h>

void maxHeapify(int *arr,int n,int i);
int main()
{
	int n;
	scanf("%d",&n);
	int *arr=malloc(sizeof(int)*(n+1));
	for(int i=1;i<=n;scanf("%d",arr+i),i++);
	//making a heap
	for(int i=n/2;i>=1;i--)
		maxHeapify(arr,n,i);
	for(int i=1,spaces=n*4,level=0;i<=n;i++)
	{
		if(i >= (1<<level)) 
		{
			level++;
			spaces = spaces/2-1;
			printf("\n");
			printf("blah%*c",spaces*5/8,' ');
		}
		else
			printf("%*c",spaces,' ');
		printf("%2d ",arr[i]);
	}
	return 0;	
}

void maxHeapify(int *arr,int n,int i)
{
	int l=i<<1,r=(i<<1)+1,largest=i;
	if(l<= n&& arr[largest]<arr[l]) largest = l;
	if(r<= n&& arr[largest]<arr[r]) largest = r;
	//swapping, note we are using l as temp
	l = arr[i]; arr[i] = arr[largest]; arr[largest] = l;
	if(largest != i)
		maxHeapify(arr,n,largest);
}
