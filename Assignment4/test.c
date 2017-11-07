#include <stdio.h>
#include <stdlib.h>

typedef struct node{ int x,y,z; } node;
int main()
{
	FILE *fp = fopen("hi.dat","r+");
	if(fp == NULL) { printf("file doesn't exist"); return 0;}

	node abc; 
	abc.x=1; abc.y = 2; abc.z=3;
	node *temp = malloc(sizeof(node));
	*temp = abc;
	temp->x = 5;

	printf("abc:%d\t \t temp:%d",abc.x,temp->x);
	abc = *temp;
	temp->x = 10;

	printf("abc:%d\t \t temp:%d",abc.x,temp->x);
	fseek(fp,0,SEEK_SET);

	fwrite(&abc,sizeof(node),1,fp);
	fwrite(temp,sizeof(node),1,fp);
	fwrite(temp,sizeof(node),1,fp);
	fwrite(temp,sizeof(node),1,fp);
	fwrite(temp,sizeof(node),1,fp);

	fseek(fp,sizeof(node),SEEK_SET);
	printf("\n%d FTELL",ftell(fp));

	fread(&abc,sizeof(node),1,fp);
	printf("abc:%d\t \t temp:%d",abc.x,temp->x);

	fclose(fp);
	free(temp);

	return 0;
}
