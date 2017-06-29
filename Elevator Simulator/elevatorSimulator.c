#include <stdio.h>
#include <Windows.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#define FFLUSH  while(getchar() != '\n')
#define buttonX 11
#define buttonY 2

#define KEY_1			49
#define KEY_q			113

#define KEY_2			50
#define KEY_w			119

#define KEY_3			51
#define KEY_e			101

#define KEY_4			52
#define KEY_r			114

#define KEY_5			53
#define KEY_t			116

#define KEY_6			54
#define KEY_y			121

#define KEY_7			55
#define KEY_u			117

#define KEY_8			56
#define KEY_i			105

#define KEY_9			57
#define KEY_o			111

#define KEY_0			48	// 0층
#define KEY_p			112	// 10층

#define TABKEY		9

void print_All(int button[11][2], int elevators[11][6]);
void print_floor();
void print_button(int button[11][2]);
void print_elevator(int arr[11][6]);
int button_enter(int button[11][2],int x,int y);
void gotoxy(int col, int row);
int getch(void);
int getkey() ;
int fMode(int f);

typedef struct elevator{
   int curr;//현재 층
   int start;//출발하는 층
   int dest;//도착하는 층
   int direct; // 대기자가 누른버튼방향
   int flag;//엘리베이터가 가동중에 있는가?	1 : 사용자를 데리러 가는중 , 2: 사용자가 탑승후 목적 층까지.
}ELV;

typedef struct user{
	int start; //대기자가 현재 기다리는 층
	int dest; //대기자의 도착 층
	int direction; // 대기자가 누른 버튼 방향
}USER;

typedef struct node{	// 큐의 노드들
	int floor;			// 현재층
	int but;			// 그층에서 누른 엘레베이터 버튼 값
	struct node* next;
}NODE;

ELV elv[3];
USER user[11];
NODE *head;
int modeF[12]; //최빈층 정보 저장 배열, 리턴값이 최빈층이 되는데 결과값 받을 때를 위해 배열의 갯수를 하나 늘림!

/////////////////////////////////////////////// 대기자 명단 큐 함수

void initQ(){
	head= (NODE*)malloc (sizeof(NODE));
	head->floor = 0;
	head->but = 0;
	head->next = NULL;
}

void inQ(int cur, int button){
	NODE *newNode, *list;

	newNode = (NODE *)malloc(sizeof(NODE));
	newNode->floor = cur;
	newNode->but = button;
	newNode->next = NULL;

	list = head;
	while(list->next != NULL )
		list = list->next;
	list->next = newNode;
	return;
}

int deQ(){
	int temp_b, temp_c;
	int result;
	NODE*temp;
	temp_b = (head->next)->but;
	temp_c = (head->next)->floor;

	result = temp_b*100+temp_c;		
	// 값을 두개 리턴해줘야하므로 . -> 버튼 누른값은  temp_b X 100 해주고  현재층은 temp_c로 해준다.
	temp = head->next;
	head->next = temp->next;

	free(temp);

	return result;

}

int isEmpty(){
	if(head->next ==NULL)
		return 1;
	else
		return 0;
}


void init_elv()		// 엘리베이터 1층으로 초기화
{
	int i;

	for (i=0; i <3; i++){

		elv[i].curr =i*5;
		elv[i].start = 1;
		elv[i].dest = NULL;
		elv[i].direct = NULL;
		elv[i].flag = NULL;

	}

}

//	/	/	/	/	///	/	//			MAIN 함수  	//		/	/	/	/	/	//


int main(void){
	char m[2] = {0, 0};
	int i, j;
	int temp;
	int distance=12;
	int n=99;
	int temp_dir[3]={1,1,1};
	int dir[3]={1,1,1};
	//n은 운행 가능한 엘베의 홋수 dir은 엘레베이터가 내려갈지 올라갈지 -> 1이면 down 0면 up
	int val;
	int floor,button_key;
	int target;
	int mode=0,y=0; //y는 이미 최빈층에 엘베가 존재한다는 뜻. 따로 함수구현이 아니다 보니 빠져 나오려면 설정 필요.




//	system("clear");
	int button[buttonX][buttonY] = {	0, 2,
						0, 2,
						0, 2,
						0, 2,
						0, 2,
						0, 2,
						0, 2,
						0, 2,
						0, 2,
						0, 2,
						0, 2	};
/////////////////////////////////////////////

	int elevators[11][6] ={				// 21 : 그냥 엘레베이터 빈칸 들.
		21, 0, 21, 0, 21, 0,			// 0 : 그냥 벽. -> 탑승하는 순간 목표 층수로 숫자 출력.
		21, 0, 21, 0, 21, 0,			// 22 :  버튼이 눌린후 아래쪽 엘레베이터가 나에게올때
		21, 0, 21, 0, 21, 0,			// 23 : 사용자가 탑승후 위로올라갈때
		21, 0, 21, 0, 21, 0,			// 24 : 버튼이 눌린후 위쪽 엘레베이터가 나에게 올때
		21, 0, 21, 0, 21, 0,			// 25 : 사용자가 탑승후 아래로 내려갈때
		21, 0, 21, 0, 21, 0,
		21, 0, 21, 0, 21, 0,
		21, 0, 21, 0, 21, 0,
		21, 0, 21, 0, 21, 0,
		21, 0, 21, 0, 21, 0,
		21, 0, 21, 0, 21, 0,
		};
	initQ();
	init_elv();
	srand( (unsigned)time(NULL));
//	input_button(button);
	while(1){	
		system("clear");
		print_All(button,elevators);

		if(kbhit()==1)
		{
			fgets(m,2,stdin);
			fgetc(stdin);
			
			switch(m[0]){
				case '0':		button_enter(button,0,0);	break;	// 지하 1층
				case '1':		button_enter(button,1,0);	break;
				case 'q':		button_enter(button,1,1);	break;
				case '2':		button_enter(button,2,0);	break;
				case 'w':		button_enter(button,2,1);	break;
				case '3':		button_enter(button,3,0);	break;
				case 'e':		button_enter(button,3,1);	break;
				case '4':		button_enter(button,4,0);	break;
				case 'r':		button_enter(button,4,1);	break;
				case '5':		button_enter(button,5,0);	break;
				case 't':		button_enter(button,5,1);	break;
				case '6':		button_enter(button,6,0);	break;
				case 'y':		button_enter(button,6,1);	break;
				case '7':		button_enter(button,7,0);	break;
				case 'u':		button_enter(button,7,1);	break;
				case '8':		button_enter(button,8,0);	break;
				case 'i':		button_enter(button,8,1);	break;
				case '9':		button_enter(button,9,0);	break;
				case 'o':		button_enter(button,9,1);	break;
				case 'p':		button_enter(button,10,1);	break;	//	10층

				case 'c':  return 0;
				default:     break;	
			}
		}	//  여기까지 입력이 있으면 받아주는 함수. (head 에다가 정보 입력함.)

	//	printf("%d\n",isEmpty());


		for(i=0;i<3;i++){
			if(elv[i].flag ==3){ //최빈층 가는 중인 엘베
				if(elv[i].curr != elv[i].dest){
					if(elv[i].direct == 1){
						elevators[elv[i].curr][i*2] = 21;
						elv[i].curr++;
						elevators[elv[i].curr][i*2] = 24;
						printf("최빈층(%d)으로 up.",mode);
					}
					else if(elv[i].direct == 3){
						elevators[elv[i].curr][i*2] = 21;
						elv[i].curr--;
						elevators[elv[i].curr][i*2] = 24;
						printf("최빈층(%d)으로 down.",mode);
					}
				}
				else if(elv[i].curr == elv[i].dest){
					elv[i].flag=0;
				}
			}
		
			if(elv[i].flag ==1){	// 대기자를 탑승시키기 위해 데리러 갈때
				if(elv[i].curr != elv[i].dest){
					if(dir[i]==1){		//	1이면 down 0면 up
						elevators[elv[i].curr][i*2] = 21;
						elv[i].curr--;
						elevators[elv[i].curr][i*2] = 24;
						printf("데리러 내려갑니다 : elv[%d] 현재 : %d\n",i,elv[i].curr);
					}
					if(dir[i]==0){
						elevators[elv[i].curr][i*2] = 21;
						elv[i].curr++;
						elevators[elv[i].curr][i*2] = 22;
						printf("데리러 올라갑니다. elv[%d] 현재 : %d\n",i,elv[i].curr);
					}
				}else if(elv[i].curr == elv[i].dest){
					
					if(elv[i].direct==3){
						elv[i].flag =2;
						elv[i].dest = user[floor].dest;
						elv[i].start = user[floor].start;
						elevators[elv[i].curr][i*2] = 25;
						elevators[elv[i].curr][i*2+1] = elv[i].dest;
						button[elv[i].curr][1] =2;
						printf("태웠고 이제 내려가요. elv[%d] : %d\n",i,elv[i].curr);
					}
					if(elv[i].direct==1){
						elv[i].flag =2;
						elv[i].dest = user[floor].dest;
						elv[i].start = user[floor].start;
						button[elv[i].curr][0] = 0;
						elevators[elv[i].curr][i*2] = 23;
						elevators[elv[i].curr][i*2+1] = elv[i].dest;
						
						printf("태웠고 이제 올라가요. elv[%d] : %d\n",i,elv[i].curr);
					}
						printf("사용자탑승함 elv[%d].dest : %d\n", i, elv[i].dest);
					}
				} else if(elv[i].flag == 2){ //사용자가 탐승한 상태
					if(elv[i].curr != elv[i].dest){
						if(elv[i].direct==3){
							elevators[elv[i].curr][i*2] = 21;
							elevators[elv[i].curr][i*2+1] = 0;
							elv[i].curr--;
							elevators[elv[i].curr][i*2] = 25;
							elevators[elv[i].curr][i*2+1] = elv[i].dest;
							printf("데리고 내려감~elv[%d] : %d\n",i,elv[i].curr);
						}
						if(elv[i].direct==1){
							elevators[elv[i].curr][i*2] = 21;
							elevators[elv[i].curr][i*2+1] = 0;
							elv[i].curr++;
							elevators[elv[i].curr][i*2] = 23;
							elevators[elv[i].curr][i*2+1] = elv[i].dest;
							printf("데리고 올라감 elv[%d] : %d\n",i,elv[i].curr);
						}
				}else if(elv[i].curr == elv[i].dest){
					elv[i].flag =0;
					elevators[elv[i].curr][i*2] = 20;
					elevators[elv[i].curr][i*2+1] = 0;
					printf("사용자 도착함 elv[%d].dest : %d\n", i, elv[i].dest);
				}
			}else elevators[elv[i].curr][i*2] =20;
		}

		if(isEmpty() == 0){	// 대기자가 단 한명이라도 존재한다면,
			distance = 12;
			for(i=0;i<3;i++){ 	// 모든 엘레베이터의 값을 바꿔준다.
				if(elv[i].flag ==0){	// 비어있는 엘베만큼 deQ해준다. 
					if(isEmpty() ==1) break;
					val = deQ();	
					floor = val%100;
					button_key = val/100;
					
					if(button_key == 1){	// 대기자가 올라가는 것을 눌렀다면,

							while(1){
								target = rand()%11;//층을 랜덤 발생
								 // 총 11개의 층 0~10
								if(floor < target) break;
							}		
						}
					else if(button_key == 3)	// 대기자가 내려가는 것을 눌렀다면,
					{
						target = rand()%floor;//층을 랜덤 발생
					}


				for(j=0;j<3;j++){ 	// 대기 엘레베이터들의 거리 차이를 비교한다.		
					
					if(elv[j].flag==0){	//운행가능한 엘레베이터가 있는지 확인 후 2개 이상 이라면 거리비교를 위해

						if(button_key == 1 || button_key==3){	// 대기자가 올라가거나(1) 내려가는 것(3)을 눌렀다면,(누른 것 다시 검증)
							user[floor].start = floor;
							user[floor].dest = target;
							user[floor].direction = button_key;
							temp = elv[j].curr -  user[floor].start;				
						}
						
						temp_dir[j] = 1;
						if(temp<0){
							temp = temp*(-1);
							temp_dir[j] = 0;
						}
						if(temp<distance){
							distance = temp;
							n=j;
							dir[j] = temp_dir[j];
						}
					}
				}
				if(n!=99)
				{///////////////////////////////////////////////////////////////////
					if(dir[n] ==1){			//	1이면 down 0면 up
						elv[n].start = elv[n].curr;
						elv[n].dest = user[floor].start;
						elv[n].direct = user[floor].direction;
						elv[n].flag = 1;
					}else if(dir[n] ==0){
						elv[n].start = elv[n].curr;
						elv[n].dest = user[floor].start;
						elv[n].direct = user[floor].direction;
						elv[n].flag = 1;
					}
					printf("elv[%d] : %d\n",n,elv[n].curr);
			
					}
				}
			}
		}	// 대기자 존재하지 않을 경우 끝.
	else if(isEmpty()==1){//대기자가 존재하지 않는데 움직임이 끝난 엘베가 있다면 최빈층으로 움직임.
		
		y=0; //최빈층 엘베 존재 여부 없음으로 초기화. 0은 없음 1은 존재함
		mode = fMode(11); //mode의 결과값 얻으려 할 때 파라미터는 11! 11층은 존재하지 않으므로
		printf("%d\n",__LINE__);
		
		for(j=0;j<3;j++){
			if(elv[j].dest == mode || elv[j].curr == mode){ // 이미 그 층에 엘베가 존재한다면 보내주지 않는다
				printf("%d\n",__LINE__);
				y=1;
				break;
			}
		}
		if(y==1)
			continue;
		
		//검증이 끝났으면 최빈층에 엘베 없으니 이제 최빈층으로 엘베 보내주기.
		for(i=0;i<3;i++){
			if(y==1){ //한대 보내줬을 때 다른 홋수기 또 보내지 않기 위해 검증
				printf("%d\n",__LINE__);
				break;
			}
			if(elv[i].flag==0){
				printf("%d\n",__LINE__);
				temp = elv[i].curr - mode;
				if(temp<0) elv[i].direct = 1; //1이면 업(mode>elv.curr이기때문)
				else elv[i].direct = 3; //3이면 다운(mode<elv.curr)
				elv[i].start = elv[i].curr;
				elv[i].dest = mode;
				elv[i].flag = 3;
				y=1;
			}
		}
	}
	printf("mode: %d\n",mode);
	Sleep(2000);
	}	// while 문의 끝
	

	return 0;

}





////////////////////////////////		배열 출력 함수.			////////////////////////////////////////
void print_All(int button[11][2], int elevators[11][6]){
	print_floor();
	print_button(button);
	print_elevator(elevators);
}
void print_floor(){
	gotoxy(8,5);
	printf("10층 :");
	gotoxy(8,6);
	printf(" 9층 :");
	gotoxy(8,7);
	printf(" 8층 :");
	gotoxy(8,8);
	printf(" 7층 :");
	gotoxy(8,9);
	printf(" 6층 :");
	gotoxy(8,10);
	printf(" 5층 :");
	gotoxy(8,11);
	printf(" 4층 :");
	gotoxy(8,12);
	printf(" 3층 :");
	gotoxy(8,13);
	printf(" 2층 :");
	gotoxy(8,14);
	printf(" 1층 :");
	gotoxy(8,15);
	printf("B1층 :");

}
void print_button(int button[11][2]){

	int i, j;
	gotoxy(17, 15);
	for (i = 0; i<buttonX; i++){
		for (j = 0; j<buttonY; j++){
			switch (button[i][j]){
			case 0:printf("△"); break;
			case 1:printf("▲"); break;
			case 2:printf("▽");  break;
			case 3:printf("▼"); break;
			}
		}
		printf("\n");
		gotoxy(17, 14-i );
	}
}

void print_elevator(int arr[11][6]){

	int i, j;
	gotoxy(24, 15);
	for (i = 0; i<11; i++){
		for (j = 0; j<6; j++){
			switch (arr[i][j]){
			case 0:printf("┃"); break;
			case 20:printf("■■"); break;
			case 21:printf("□□"); break;
			case 22:printf("△△");  break;
			case 23:printf("▲▲"); break;
			case 24:printf("▽▽");  break;
			case 25:printf("▼▼"); break;
			default: printf("%d",arr[i][j]); break;
			}
		}
		printf("\n");
		gotoxy(24, 14-i);
	}
}


int button_enter(int button[11][2],int x,int y){

	system("clear");
	printf("%d %d",x,y);

	if(button[x][y] ==0){
		button[x][y]=1;
		inQ(x,1);
		fMode(x);

	}else if(button[x][y] ==2){
		button[x][y]=3;
		inQ(x,3);
		fMode(x);
	}

	return 0;

}



/////////////////////////////	 좌표값 할당 && 키받는 기능을 위한 함수들.		///////////////////////////////////////////////

void gotoxy(int col, int row) {
	printf("\e[%d%;%dH", row, col);
}

int getch(void)
{
	struct termios oldt, newt;
	int ch;
	tcgetattr(STDIN_FILENO, &oldt);
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	ch = getchar();
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
	return ch;
}

int getkey() {
	int ch;
	if ((ch = getch()) == 0xE0)
		ch = getch();
	printf("%d\n",ch);
	return ch;
}

int kbhit(void)
{
	  struct termios oldt, newt;
	  int ch;
	  int oldf;
	 
	  tcgetattr(STDIN_FILENO, &oldt);
	  newt = oldt;
	  newt.c_lflag &= ~(ICANON | ECHO);
	  tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	  oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
	  fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
	 
	  ch = getchar();
	 
	  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
	  fcntl(STDIN_FILENO, F_SETFL, oldf);
	 
	  if(ch != EOF)
	  {
		    ungetc(ch, stdin);
		    return 1;
	  }
	 
	  return 0;
}

////////////////////////최빈층 정보 저장 및 반환을 위한 함수/////////////////////

int fMode(int f){
	int i;
	int mode=1, floor=1; //최빈수(mode)와 최빈수 해당 층(floor)
	int tMode; //비교 대상 층의 입력 횟수가 저장 될 변수

	modeF[f]++; //호출된 층의 값이++
	for(i=0;i<11;i++){
		tMode = modeF[i];
		if(tMode>mode){
			mode = tMode;
			floor = i;
		}
	}
	
	return floor;

}