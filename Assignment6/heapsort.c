#include <stdio.h>
#include <stdlib.h>

void maxHeapify(int *arr,int n,int i);
int abc=0;
int main()
{
	int n;
	scanf("%d",&n);
	int *arr=malloc(sizeof(int)*(n+1));
	for(int i=1;i<=n;scanf("%d",arr+i),i++);
	//making a heap
	for(int i=n/2;i>=1;i--)
		maxHeapify(arr,n,i);
	int n0=n;
	while(n>1)
	{ 
		int temp = arr[1];
		arr[1] = arr[n];
		arr[n] = temp;
		n--;
		maxHeapify(arr,n,1);
	}
	for(int i=1;i<=n0;i++)
		printf("%d\n",arr[i]);
	printf("ABC:%d",abc);
	
	return 0;	
}

void maxHeapify(int *arr,int n,int i)
{
	abc++;
	int l=i<<1,r=(i<<1)+1,largest=i;
	if(l<= n&& arr[largest]<arr[l]) largest = l;
	if(r<= n&& arr[largest]<arr[r]) largest = r;
	//swapping, note we are using l as temp
	l = arr[i]; arr[i] = arr[largest]; arr[largest] = l;
	if(largest != i)
		maxHeapify(arr,n,largest);
}
