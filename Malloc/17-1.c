#include <stdio.h>
#include <malloc.h>


int main(){

	int i;
	int sum=0;
	int * p;
	int * p2;

	double * pb;
	double dsum =0;

	char * c;
	char temp[50];

	//10개의 정수를 저장할 수 있는 공간
	p = (int *)malloc(sizeof(int)*10);
	if(p == NULL)
	{
		printf("동적메모리 할당 오류\n");
		return 1;	
	}
	for(i=0; i<10; i++)
	{
		p[i] = i+1;	
		printf("p[i] = %d\n", p[i]);
		sum += p[i];
	}
	printf("average = %d\n", sum/10);
	free(p);

	printf("----------------------------\n");

	//10개의 실수를 저장할 수 있는 공간
	pb = (double*)malloc(sizeof(double)*10);
	if(pb == NULL)
	{
		printf("동적메모리 할당 오류\n");
		return 1;	
	}
	for(i=0; i<10; i++)
	{
		pb[i] = i+1;	
		printf("pb[%d] = %lf\n", i, pb[i]);
		dsum += pb[i];
	}
	printf("average = %lf\n", dsum/10);
	free(pb);

	printf("----------------------------\n");

	//50개의 문자를 저장할 수 있는 공간
	c = (char*)malloc(sizeof(char)*50);
	if(c == NULL)
	{
		printf("동적메모리 할당 오류\n");
		return 1;	
	}
	printf("Action speak louder than words.\n");
	fgets(temp,50,stdin);	
	for(i=0; i<50; i++)
	{
		c[i] = temp[i];	
	}
	printf("\n%s\n", c);
	
	printf("----------------------------\n");

	//10개의 정수를 저장할 수 있는 공간(calloc)
	p2 = (int*)calloc(10,sizeof(int));
	if(p2 == NULL)
	{
		printf("동적메모리 할당 오류\n");
		return 1;	
	}
	for(i=0; i<10; i++)
	{
		printf("p2[%d] = %d\n", i, p2[i]);	
	}
	
	printf("----------------------------\n");

	//기존의 메모리 할당의 크기를 2배로 증가(realloc)
	p2 = (int*)realloc(p2, 20*sizeof(int));
	


	return 0;
}