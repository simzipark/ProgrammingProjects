#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <ctype.h>

#define TITLE printf("[201410081, Park Jihyun][Personal Project #3] Vocabulary\n");

typedef struct voca{
	
	char word[50];
	int count;
	struct voca * link;

}VOCA;

void menu();
int get_word(char * word, FILE * fptr);
void lower_case(char * word);
int sort_alphabet(char * word);
VOCA* searching(VOCA * list[27], char temp[]);
void print_wordlist(VOCA * list[27]);
void add_word(VOCA * ptr, char * word);

int main()
{	
	FILE * fptr;	
	char file[256];
	char word[256];
	char temp[256];
	char c;
	int i;
	char m[2];
	char etc[2] = {0, 0};
	int max = 0;
	char alphabet[27][8] = {"[ A ]","[ B ]","[ C ]","[ D ]","[ E ]","[ F ]","[ G ]",
		"[ H ]","[ I ]","[ J ]","[ K ]","[ L ]","[ M ]","[ N ]","[ O ]","[ P ]","[ Q ]","[ R ]","[ S ]",
		"[ T ]","[ U ]","[ V ]","[ W ]","[ X ]","[ Y ]","[ Z ]","[ etc ]"};
	VOCA * list[27];
	VOCA * p;
	VOCA *found = 0;

	for(i=0; i<27; i++)
	{
		list[i] = (VOCA*)malloc(sizeof(VOCA));
		list[i]->link = NULL;	
		strcpy(list[i]->word, alphabet[i]);	
		list[i]->count = i;
	}

	TITLE;
//	printf("File Name(include .txt) : ");
//	fgets(file, 100, stdin);
	file[strlen(file)-1] = 0;
	if( (fptr = fopen("help.txt", "r")) == NULL)
	{
		fprintf(stderr, "%s is cannot be found.\n", file);
		exit(1);
	}
	
	/////////////////////////////////////////////////////////////

	c = get_word(word, fptr);
	while(c != EOF)
	{	
		if(c != '\n' && c != '\t' && c != '\r' && c != '\v' && c != '\f' && c != ' ')
		{
			etc[0] = c;
			
			if(list[26]->link == NULL)
				add_word(list[26], etc);

			//노드의 단어 2번째 이상 저장
			if(list[26]->link != NULL)
			{
				VOCA *found = 0;
				found = searching(list, etc);
				if(found != 0)
					found->count++;

				if(found == 0)
				{
					p = list[26];
					while(p->link != 0)
						p = p->link;

					add_word(p, etc);
				}
				
			} //if(list[sort_alphabet(word)]->link != NULL)
		}
		lower_case(word);
		
		if(strlen(word) > 0 && strlen(word) < 50)
		{
			//노드의 단어 최초 저장
			if(list[sort_alphabet(word)]->link == NULL)
				add_word(list[sort_alphabet(word)], word);

			//노드의 단어 2번째 이상 저장
			if(list[sort_alphabet(word)]->link != NULL)
			{
				VOCA *found = 0;
				found = searching(list, word);
				if(found != 0)
					found->count++;

				if(found == 0)
				{
					p = list[sort_alphabet(word)];
					while(p->link != 0)
						p = p->link;

					add_word(p, word);

				}
			} //if(list[sort_alphabet(word)]->link != NULL)
		} //if(strlen(word) > 0 && strlen(word) < 50)	

		c = get_word(word, fptr);

	} //while(get_word(&c, word, fptr) != EOF)

	


	while(1)
	{
		menu();
		fgets(m,2,stdin);
		fgetc(stdin);
		switch(atoi(m))
		{
		case 1 : {
				printf("input word : ");
				fgets(temp,256,stdin);
				temp[strlen(temp)-1] = 0;
				lower_case(temp);

				found = searching(list, temp);
				if(found != 0)
					found->count++;

				if(found == 0)
				{
					p = list[sort_alphabet(temp)];
					while(p->link != 0)
						p = p->link;

					add_word(p, temp);

				}
				
				} printf("\n\n"); break;
		case 2 : {
				printf("input word : ");
				fgets(temp,256,stdin);
				temp[strlen(temp)-1] = 0;
				lower_case(temp);

				if(searching(list, temp) == 0)
					printf("Cannot Find\n");
			
				else
				{
					found = searching(list, temp);
					p = list[temp[0] - 'a'];

					while(p->link != found)
						p = p->link;
					
					if(found->link == NULL)
						p->link = NULL;
					else
						p->link = found->link;
				}
				 } printf("\n\n"); break;
		case 3 : {
			printf("input word : ");
			fgets(temp,256,stdin);
			temp[strlen(temp)-1] = 0;
			lower_case(temp);

			if(searching(list, temp) == 0)
				printf("Cannot Find\n");
			
			else
				printf("Find\n"); 
			
			} printf("\n\n"); break;	
		case 4 : {
				for(i=0; i<27; i++)
				{
					p = list[i];
					while(p != NULL)
					{
						if(max < p->count)
							max = p->count;
						p = p->link;	
					}
				}
				for(i=0; i<27; i++)
				{
					p = list[i];
					while(p != NULL)
					{
						if(max == p->count)
							printf("maximum word : %s (%2d)\n", p->word, p->count);
						p = p->link;	
					}
				}
				} printf("\n\n"); break;
		case 5 : print_wordlist(list); printf("\n\n"); break;
		default : exit(1); break;
		}
		
	}
	

	return 0;
}

void menu()
{
	printf("== WORD BOOK MENU ==\n");
	printf("   1. INSERT\n");
	printf("   2. DELETE\n"); 
	printf("   3. SEARCH\n"); 
	printf("   4. MAX WORD\n");
	printf("   5. SHOW WORDBOOK\n");
	printf("   ( DEFAULT KEY -> END )\n"); 
	printf("   : ");	
}
VOCA* searching(VOCA * list[27], char temp[])
{
	int i;

	VOCA * p;

	if(isalpha(temp[0]) == 0)
		p = list[26];
	else
		p = list[temp[0] - 'a'];

	while(p != NULL)
	{
		//printf("word : %s / list : %s\n", temp, p->word);
		if(strcmp(temp,p->word) == 0)
		{
			
			return p;
		}
		
		p = p->link;
	}
	return 0;
}
int get_word(char * word, FILE * fptr)
{
	char c;
	word[0] = 0;
	c = fgetc(fptr);

	while(c != EOF)
	{
		if(isalpha(c) == 0)
		{
			//특문일 경우도 리턴
			break;
		}
		word[strlen(word)+1] = 0;
		word[strlen(word)] = c;
	
		c = fgetc(fptr);	
		
	}
 
	return c;
}
void lower_case(char * word)
{
	int i;

	for(i=0; i<strlen(word); i++)
	{
		if(isupper(word[i]))
			word[i] += 32;
	}
}
int sort_alphabet(char * word)
{
	int i;

	for(i=0; i<26; i++)
	{
		if(word[0] == 97+i)
			return i;
	}
	if(isalpha(word[0]) == 0) //알파벳이 아니면
		return 26;		
}
void print_wordlist(VOCA * list[27])
{
	int i;
	VOCA * p;

	for(i=0; i<27; i++)
	{
		p = list[i];
		while(p != NULL)
		{
			printf("%s(%d)  ", p->word, p->count);
			p = p->link;	
		}
		printf("\n\n");
	}
}
void add_word(VOCA * ptr, char * word)
{	
	VOCA * p;

	ptr ->link = (VOCA*)malloc(sizeof(VOCA));
	p = ptr->link;
	p->link = NULL;
	strcpy(p->word, word);
	p->count = 1;
}