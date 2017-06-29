#include <stdio.h>
#include <string.h>

long size(FILE *fp, char * name, char * temp);
int show_menu();
void update_record(FILE * fp);
void edit_record(FILE * fp);
void search_record(FILE * fp);

int main(){

	FILE * fp;

		
	while(1)
	{
		fp = fopen("record.txt", "a+");
		if(fp == NULL)
		{
		printf("������ �� �� �����ϴ�.\n");
		return 1;
		}

		switch(show_menu())
		{
			case 1: update_record(fp); break;
			case 2: edit_record(fp); break;
			case 3: search_record(fp); break;
			case 4: return 1;
			default: return 1;
		}

		fclose(fp);
	}

	return 0;
}
long size(FILE *fp, char * name, char * temp) 
{ 

	long fcur; 
	long fsize; 

	fseek(fp, 0, SEEK_SET); 


	while(1)
	{
		fgets(temp, sizeof(temp), fp);
		if( strstr(temp, name) != NULL)
			break;
	}
    fsize = ftell(fp); 

	printf("%u\n", fsize);

    return fsize; 

} 
int show_menu()
{
	int s;

	printf("-------\n");
	printf("1. �߰�\n");
	printf("2. ����\n");
	printf("3. �˻�\n");
	printf("4. ����\n");
	printf("-------\n");
	printf("�޴��� �����ϼ��� : ");
	scanf("%d", &s);

	return s;
}
void update_record(FILE * fp)
{
	char name[100];
	char major[100];
	int age;

	printf("update record\n");
	printf("�̸� : ");
	scanf("%s", &name);
	fprintf(fp, "%s ", name);

	printf("���� : ");
	scanf("%s", &major);
	fprintf(fp, "%s ", major);

	printf("���� : ");
	scanf("%d", &age);
	fprintf(fp, "%d\n", age);

}//1. �߰�
void edit_record(FILE * fp)
{
	char name[100];
	char major[100];
	int age;
	char temp[100];

	printf("edit record\n");
	printf("������ ��� �̸� : ");
	scanf("%s", &name);

	while(1)
	{
		fgets(temp, sizeof(temp), fp);

		if( strstr(temp, name) != NULL)
		{	
			fputs(temp,stdout);

			fseek(fp, 0L ,SEEK_SET);  
			fseek(fp, size(fp,name,temp) ,SEEK_CUR); 
			
			printf("�̸� : ");
			scanf("%s", &name);
			fprintf(fp, "%s ", name);

			printf("���� : ");
			scanf("%s", &major);
			fprintf(fp, "%s ", major);

			printf("���� : ");
			scanf("%d", &age);
			fprintf(fp, "%d\n", age);
			break;
		}
			
	}

}//2. ����
void search_record(FILE * fp)
{
	char name[100];
	char temp[100];

	printf("search record\n");
	printf("ã�� ��� �̸� : ");
	scanf("%s", &name);

	while(1)
	{
		fgets(temp, sizeof(temp), fp);

		if( strstr(temp, name) != NULL)
		{
			fputs(temp,stdout);
			break;
		}
			
	}

}//3. �˻�