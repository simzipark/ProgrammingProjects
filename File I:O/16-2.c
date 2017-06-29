#include <stdio.h>

int main(){

	FILE * fp;
	FILE * fp2;
	char name[80];
	int k, m, e;
	double average;
	int ch;
	int i=1;

	fp = fopen("input.txt", "r");
	if(fp == NULL)
	{
		printf("파일 input.txt을 열 수 없습니다.\n");
		return 1;
	}
	fp2 = fopen("output.txt", "w");
	if(fp2 == NULL)
	{
		printf("파일 output.txt을 열 수 없습니다.\n");
		return 1;
	}

	while(1)
	{
		if(fscanf(fp, "%s %d %d %d", name, &k, &m, &e) != 4)
			break;

		average = (k+m+e)/3.0;
		printf("%d : %10s %-10.3f\n", i,name, average);	
		
		fprintf(fp2, "%d : %10s %-10.3f\n", i, name, average);
		i++;
	}

	fclose(fp);


	return 0;
}