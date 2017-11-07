#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
int main(int argc,char *argv[])
{
	time_t t;
	srand((unsigned)time(&t));
	int n= argc==1?10000:atoi(argv[1]);
	printf("%d\n",n);
	for(int i=0;i<n;i++)
		printf("%d\n",rand());
		
}
