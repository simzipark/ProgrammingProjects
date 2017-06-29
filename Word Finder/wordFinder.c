#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
 
#define TRIAL 5

int get_word(char *c, char * word, FILE * fptr, int * line);
int com(char * patt, char * word);

int main(){
 
	FILE * fptr;
	char file[256];
	char word[256];
	char patt[256];
	char buffer[256];
	char c;
	int line=1;
	int i;
	int temp;

	//불러올 파일 이름 입력
	printf("File Name(include .txt) : ");
	fgets(file, 100, stdin);
	file[strlen(file)-1] = 0;
 
	//파일 오픈 에러 검출
	if( (fptr = fopen(file, "r")) == NULL)
	{
		fprintf(stderr, "%s is cannot be found.\n", file);
		exit(1);
	}
	
	//불러온 파일 이름 출력
	printf("\n\"%s\"\n\n", file);

	for(i=1; i<=TRIAL; i++)
	{
		//검색할 패턴 입력
		printf("-- Trial : %d --\n", i);
		printf("INPUT pattern : ");
		fgets(patt, 100, stdin);
		patt[strlen(patt)-1] = 0;
		
		//패턴에 일치하는 단어 찾기
		while(get_word(&c, word, fptr, &line) != EOF)
		{
			//단어를 찾은 line 출력
			if((com(patt,word) == 1) && (temp != line))
			{
				printf("\n");
				printf("Line #%3d : ", line);
			}
			//찾은 단어 출력, 같은라인에서 찾았으면 같은라인에 찍힘.
			if(com(patt,word) == 1)
			{
				printf("%s ",word);
				temp = line;
			}
		}
		
		//다음 trial
		line=1;
		printf("\n");
		fptr = fopen(file, "r");
		printf("\n");
	}
	

	return 0;
}
int get_word(char * c,char * word, FILE * fptr, int * line)
{
	char t;
	word[0] = 0;
	*c = fgetc(fptr);

	while(*c != EOF)
	{
		if(*c == '\n')
			(*line)++;
		else if(isalpha(*c) == 0)
			break;

		word[strlen(word)+1] = 0;
		word[strlen(word)] = *c;
		
		*c = fgetc(fptr);
	}
 
	return *c;
}
int com(char * patt, char * word)
{
	if (strlen(patt) == 0 || strlen(word) == 0)
		return strlen(patt) == 0 && strlen(word) == 0;

	if(*(patt) == *(word))
		return com(patt+1, word+1);

	else if(*(patt) == '*')
	{
		
		if(*(patt+1) == *(word))
			return com(patt+1, word);
			
		else if(*(patt+1) != *(word))
		{			
			if(*(patt+1) == 0)
				return 1;
			else
				return com(patt, word+1);
		}
		
	}

	else if(*(patt) == '?')
	{
		while( *patt == '?' ) 
		{
			if(*(patt+1) == *(word))
				return com(patt+1, word);
			
			else if(*(patt+1) != *(word))
			{
				if(*(patt+1) == *(word+1))
					return com(patt+1, word+1);

				else if(*(patt+1) != *(word+1))
					return 0;
			}
		}
	}


	else
		return 0;
	
 }