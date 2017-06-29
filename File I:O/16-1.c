#include <stdio.h>

int main(){

	FILE * fp;
	FILE * fp2;
	char file[100];
	int ch;

	printf("파일명을 입력하세욤 : ");
	scanf("%s", &file);

	fp = fopen(file, "r");
	if(fp == NULL)
	{
		printf("파일 proverb.txt을 열 수 없습니다.\n");
		return 1;
	}

	fp2 = fopen("output.txt", "w");
	if(fp2 == NULL)
	{
		printf("파일 output.txt을 열 수 없습니다.\n");
		return 1;
	}

	//while((ch = fgetc(fp)) != EOF )
	while(1)
	{
		if((ch = fgetc(fp)) == EOF)
			break;
		fputc(ch,stdout);
		
		fputc(ch,fp2);
		
	}

	printf("\n");

	fclose(fp);

	return 0;
}