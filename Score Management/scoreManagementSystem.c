#include <stdio.h>
#include <string.h>

//학생수와 과목수 지정
#define MAX_STD 5
#define MAX_SUB 3

//학생 구조체 선언
typedef struct std
{
	char *std_name;
	char *std_num;
	float grade[MAX_SUB];
	float average;
	int warning;

}STUDENT;

//사용할 함수들
int switch_grade(float grade);
float sub_average(STUDENT students[], int i);
float std_average(STUDENT students[], int i);
void show_rank(STUDENT students[]);
void show_students(STUDENT students[]);
void input_grade(STUDENT students[]);
void warning(STUDENT students[]);
void clean_stdin(void);

//고정적으로 사용할 문자열 선언
static char * name[] = {"Isabell", "Oliver", "Lily", "Jack", "Emily"};
static char * idnum[] = {"201410081","201410101","201110301","201620941","201250291"};
static char * subname[] = {"Math", "Programming", "English"};
static char * grade_name[] = {"A+","A","B+","B","C+","C","D+","D","F"};
static char * warning_sign[] = {"X", "O"};

//메인함수
int main(){

	int i, j;
	STUDENT students[MAX_STD];

	//학생 구조체에 이름/학번 부여
	for(i=0; i<MAX_STD; i++){
		students[i].std_name = name[i];
		students[i].std_num = idnum[i];
	}

	printf("##########################################\n");
	printf("[2016PP][201410081,ParkJihyun] project#1\n");
	printf("##########################################\n");

	input_grade(students); //성적 입력
	warning(students); //학사경고여부 확인
	show_students(students); //학생 정보 출력
	show_rank(students); //최고,최저 평점 출력

}
void clean_stdin(void)
{
	int c ;
	do{c = getchar();}	
	while(c!=EOF);
		
}
//학생 정보 출력 함수
void show_students(STUDENT students[]){

	int i, j;

	printf("------------------------------------------------\n");
	for(i=0; i<MAX_STD; i++){
		printf("[%s] %s ", idnum[i], name[i]);
		printf("-------- Bachelor_Warning %s\n", warning_sign[students[i].warning]);
		printf("* Average : %lf\n", std_average(students,i));
		for(j=0; j<MAX_SUB; j++)
			printf("- %s : %s (%f)\n", subname[j], grade_name[switch_grade(students[i].grade[j])], students[i].grade[j]);
		printf("------------------------------------------------\n");
	}
	for(i=0; i<MAX_SUB; i++)
		printf(" %s Average : %lf\n", subname[i], sub_average(students,i));
	printf("------------------------------------------------\n");

}
//성적입력 함수
void input_grade(STUDENT students[]){ //

	int i, j;
	char c[10];

	for (i = 0; i<MAX_STD; i++){
		printf("[%s] %s Grade\n", idnum[i], name[i]);
		for (j = 0; j<MAX_SUB; j++){
			while (1)
			{
				printf("Input Grade %s : ", subname[j]);
				fgets(c, 10, stdin);
				//A,B,D,C,F 입력 오류인지 검사
				if (c[1] == '+' || c[1] == '\n'){
					if (strlen(c) < 4)
					{
						if (65 <= c[0] && c[0] <= 70 && c[0] != 69)
						{
							students[i].grade[j] = (float)(69 - c[0]);
							if (c[0] == 'F')
								students[i].grade[j] = 0;
							else
							{
								if (c[1] == '+')
									students[i].grade[j] += 0.5;
							}
							break;
						}
					}
				}
				printf("!!! Only A+ ~ F !!!\n");
			}
		}
		printf("\n");
	}
}
//학사경고 여부 함수
void warning(STUDENT students[])
{
	int i, j;

	for(i=0; i<MAX_STD; i++){
		for(j=0; j<MAX_SUB; j++)
		{
			if(students[i].grade[j]==0){
				students[i].warning = 1;
				break;}//학사경고 받았으면 1 (sign O)
			else students[i].warning = 0; //학사경고 안받았으면 0 (sign X)
		}	
	}

}
//성적(숫자)값을 성적(문자)값으로 변환해주는 함수
int switch_grade(float grade)
{
	int i, j;

	if(grade==4.5) return 0;
	else if(grade==4) return 1;
	else if(grade==3.5) return 2;
	else if(grade==3) return 3;
	else if(grade==3.5) return 4;
	else if(grade==2) return 5;
	else if(grade==2.5) return 6;
	else if(grade==1) return 7;
	else if(grade==0) return 8;
}
//과목 평균점수
float sub_average(STUDENT students[], int i)
{
	int j;
	float sum=0;
	float average;	 

	for(j=0; j<MAX_STD; j++)
		sum += students[j].grade[i];

	average = sum/MAX_STD; 
	return average;
}
//학생 평균점수
float std_average(STUDENT students[], int i)
{
	int j;
	float sum=0;
	float average;	 

	for(j=0; j<MAX_SUB; j++)
		sum += students[i].grade[j];

	average = sum/MAX_SUB;
	students[i].average = average;

	return average;
}
//평점 높은학생/낮은학생
void show_rank(STUDENT students[])
{
	int i, j;
	float min=4.5;
	float max=0;
	int count=0;


	//최고평점
	for (i = 0; i < MAX_STD; i++){
		if (max <= students[i].average)
			max = students[i].average;	
	}

	printf("[Highest Average Students]\n");
	for (i = 0; i < MAX_STD; i++){
		if (max == students[i].average){
		printf("Name : %s\n", students[i].std_name);
		printf("Average : %lf\n", students[i].average);
		}
	}

	printf("\n");

	//최저평점
	for (i = 0; i < MAX_STD; i++){
		if (min >= students[i].average)
			min = students[i].average;
	}

	printf("[Lowest Average Students]\n");
	for (i = 0; i < MAX_STD; i++){
		if (min == students[i].average){
		printf("Name : %s\n", students[i].std_name);
		printf("Average : %lf\n", students[i].average);
		}
	}

}
