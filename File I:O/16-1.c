#include <stdio.h>

int main(){

	FILE * fp;
	FILE * fp2;
	char file[100];
	int ch;

	printf("���ϸ��� �Է��ϼ��� : ");
	scanf("%s", &file);

	fp = fopen(file, "r");
	if(fp == NULL)
	{
		printf("���� proverb.txt�� �� �� �����ϴ�.\n");
		return 1;
	}

	fp2 = fopen("output.txt", "w");
	if(fp2 == NULL)
	{
		printf("���� output.txt�� �� �� �����ϴ�.\n");
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