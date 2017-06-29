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

#define KEY_0			48	// 0��
#define KEY_p			112	// 10��

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
   int curr;//���� ��
   int start;//����ϴ� ��
   int dest;//�����ϴ� ��
   int direct; // ����ڰ� ������ư����
   int flag;//���������Ͱ� �����߿� �ִ°�?	1 : ����ڸ� ������ ������ , 2: ����ڰ� ž���� ���� ������.
}ELV;

typedef struct user{
	int start; //����ڰ� ���� ��ٸ��� ��
	int dest; //������� ���� ��
	int direction; // ����ڰ� ���� ��ư ����
}USER;

typedef struct node{	// ť�� ����
	int floor;			// ������
	int but;			// �������� ���� ���������� ��ư ��
	struct node* next;
}NODE;

ELV elv[3];
USER user[11];
NODE *head;
int modeF[12]; //�ֺ��� ���� ���� �迭, ���ϰ��� �ֺ����� �Ǵµ� ����� ���� ���� ���� �迭�� ������ �ϳ� �ø�!

/////////////////////////////////////////////// ����� ��� ť �Լ�

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
	// ���� �ΰ� ����������ϹǷ� . -> ��ư ��������  temp_b X 100 ���ְ�  �������� temp_c�� ���ش�.
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


void init_elv()		// ���������� 1������ �ʱ�ȭ
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

//	/	/	/	/	///	/	//			MAIN �Լ�  	//		/	/	/	/	/	//


int main(void){
	char m[2] = {0, 0};
	int i, j;
	int temp;
	int distance=12;
	int n=99;
	int temp_dir[3]={1,1,1};
	int dir[3]={1,1,1};
	//n�� ���� ������ ������ Ȫ�� dir�� ���������Ͱ� �������� �ö��� -> 1�̸� down 0�� up
	int val;
	int floor,button_key;
	int target;
	int mode=0,y=0; //y�� �̹� �ֺ����� ������ �����Ѵٴ� ��. ���� �Լ������� �ƴϴ� ���� ���� �������� ���� �ʿ�.




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

	int elevators[11][6] ={				// 21 : �׳� ���������� ��ĭ ��.
		21, 0, 21, 0, 21, 0,			// 0 : �׳� ��. -> ž���ϴ� ���� ��ǥ ������ ���� ���.
		21, 0, 21, 0, 21, 0,			// 22 :  ��ư�� ������ �Ʒ��� ���������Ͱ� �����Կö�
		21, 0, 21, 0, 21, 0,			// 23 : ����ڰ� ž���� ���οö󰥶�
		21, 0, 21, 0, 21, 0,			// 24 : ��ư�� ������ ���� ���������Ͱ� ������ �ö�
		21, 0, 21, 0, 21, 0,			// 25 : ����ڰ� ž���� �Ʒ��� ��������
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
				case '0':		button_enter(button,0,0);	break;	// ���� 1��
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
				case 'p':		button_enter(button,10,1);	break;	//	10��

				case 'c':  return 0;
				default:     break;	
			}
		}	//  ������� �Է��� ������ �޾��ִ� �Լ�. (head ���ٰ� ���� �Է���.)

	//	printf("%d\n",isEmpty());


		for(i=0;i<3;i++){
			if(elv[i].flag ==3){ //�ֺ��� ���� ���� ����
				if(elv[i].curr != elv[i].dest){
					if(elv[i].direct == 1){
						elevators[elv[i].curr][i*2] = 21;
						elv[i].curr++;
						elevators[elv[i].curr][i*2] = 24;
						printf("�ֺ���(%d)���� up.",mode);
					}
					else if(elv[i].direct == 3){
						elevators[elv[i].curr][i*2] = 21;
						elv[i].curr--;
						elevators[elv[i].curr][i*2] = 24;
						printf("�ֺ���(%d)���� down.",mode);
					}
				}
				else if(elv[i].curr == elv[i].dest){
					elv[i].flag=0;
				}
			}
		
			if(elv[i].flag ==1){	// ����ڸ� ž�½�Ű�� ���� ������ ����
				if(elv[i].curr != elv[i].dest){
					if(dir[i]==1){		//	1�̸� down 0�� up
						elevators[elv[i].curr][i*2] = 21;
						elv[i].curr--;
						elevators[elv[i].curr][i*2] = 24;
						printf("������ �������ϴ� : elv[%d] ���� : %d\n",i,elv[i].curr);
					}
					if(dir[i]==0){
						elevators[elv[i].curr][i*2] = 21;
						elv[i].curr++;
						elevators[elv[i].curr][i*2] = 22;
						printf("������ �ö󰩴ϴ�. elv[%d] ���� : %d\n",i,elv[i].curr);
					}
				}else if(elv[i].curr == elv[i].dest){
					
					if(elv[i].direct==3){
						elv[i].flag =2;
						elv[i].dest = user[floor].dest;
						elv[i].start = user[floor].start;
						elevators[elv[i].curr][i*2] = 25;
						elevators[elv[i].curr][i*2+1] = elv[i].dest;
						button[elv[i].curr][1] =2;
						printf("�¿��� ���� ��������. elv[%d] : %d\n",i,elv[i].curr);
					}
					if(elv[i].direct==1){
						elv[i].flag =2;
						elv[i].dest = user[floor].dest;
						elv[i].start = user[floor].start;
						button[elv[i].curr][0] = 0;
						elevators[elv[i].curr][i*2] = 23;
						elevators[elv[i].curr][i*2+1] = elv[i].dest;
						
						printf("�¿��� ���� �ö󰡿�. elv[%d] : %d\n",i,elv[i].curr);
					}
						printf("�����ž���� elv[%d].dest : %d\n", i, elv[i].dest);
					}
				} else if(elv[i].flag == 2){ //����ڰ� Ž���� ����
					if(elv[i].curr != elv[i].dest){
						if(elv[i].direct==3){
							elevators[elv[i].curr][i*2] = 21;
							elevators[elv[i].curr][i*2+1] = 0;
							elv[i].curr--;
							elevators[elv[i].curr][i*2] = 25;
							elevators[elv[i].curr][i*2+1] = elv[i].dest;
							printf("������ ������~elv[%d] : %d\n",i,elv[i].curr);
						}
						if(elv[i].direct==1){
							elevators[elv[i].curr][i*2] = 21;
							elevators[elv[i].curr][i*2+1] = 0;
							elv[i].curr++;
							elevators[elv[i].curr][i*2] = 23;
							elevators[elv[i].curr][i*2+1] = elv[i].dest;
							printf("������ �ö� elv[%d] : %d\n",i,elv[i].curr);
						}
				}else if(elv[i].curr == elv[i].dest){
					elv[i].flag =0;
					elevators[elv[i].curr][i*2] = 20;
					elevators[elv[i].curr][i*2+1] = 0;
					printf("����� ������ elv[%d].dest : %d\n", i, elv[i].dest);
				}
			}else elevators[elv[i].curr][i*2] =20;
		}

		if(isEmpty() == 0){	// ����ڰ� �� �Ѹ��̶� �����Ѵٸ�,
			distance = 12;
			for(i=0;i<3;i++){ 	// ��� ������������ ���� �ٲ��ش�.
				if(elv[i].flag ==0){	// ����ִ� ������ŭ deQ���ش�. 
					if(isEmpty() ==1) break;
					val = deQ();	
					floor = val%100;
					button_key = val/100;
					
					if(button_key == 1){	// ����ڰ� �ö󰡴� ���� �����ٸ�,

							while(1){
								target = rand()%11;//���� ���� �߻�
								 // �� 11���� �� 0~10
								if(floor < target) break;
							}		
						}
					else if(button_key == 3)	// ����ڰ� �������� ���� �����ٸ�,
					{
						target = rand()%floor;//���� ���� �߻�
					}


				for(j=0;j<3;j++){ 	// ��� ���������͵��� �Ÿ� ���̸� ���Ѵ�.		
					
					if(elv[j].flag==0){	//���డ���� ���������Ͱ� �ִ��� Ȯ�� �� 2�� �̻� �̶�� �Ÿ��񱳸� ����

						if(button_key == 1 || button_key==3){	// ����ڰ� �ö󰡰ų�(1) �������� ��(3)�� �����ٸ�,(���� �� �ٽ� ����)
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
					if(dir[n] ==1){			//	1�̸� down 0�� up
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
		}	// ����� �������� ���� ��� ��.
	else if(isEmpty()==1){//����ڰ� �������� �ʴµ� �������� ���� ������ �ִٸ� �ֺ������� ������.
		
		y=0; //�ֺ��� ���� ���� ���� �������� �ʱ�ȭ. 0�� ���� 1�� ������
		mode = fMode(11); //mode�� ����� ������ �� �� �Ķ���ʹ� 11! 11���� �������� �����Ƿ�
		printf("%d\n",__LINE__);
		
		for(j=0;j<3;j++){
			if(elv[j].dest == mode || elv[j].curr == mode){ // �̹� �� ���� ������ �����Ѵٸ� �������� �ʴ´�
				printf("%d\n",__LINE__);
				y=1;
				break;
			}
		}
		if(y==1)
			continue;
		
		//������ �������� �ֺ����� ���� ������ ���� �ֺ������� ���� �����ֱ�.
		for(i=0;i<3;i++){
			if(y==1){ //�Ѵ� �������� �� �ٸ� Ȫ���� �� ������ �ʱ� ���� ����
				printf("%d\n",__LINE__);
				break;
			}
			if(elv[i].flag==0){
				printf("%d\n",__LINE__);
				temp = elv[i].curr - mode;
				if(temp<0) elv[i].direct = 1; //1�̸� ��(mode>elv.curr�̱⶧��)
				else elv[i].direct = 3; //3�̸� �ٿ�(mode<elv.curr)
				elv[i].start = elv[i].curr;
				elv[i].dest = mode;
				elv[i].flag = 3;
				y=1;
			}
		}
	}
	printf("mode: %d\n",mode);
	Sleep(2000);
	}	// while ���� ��
	

	return 0;

}





////////////////////////////////		�迭 ��� �Լ�.			////////////////////////////////////////
void print_All(int button[11][2], int elevators[11][6]){
	print_floor();
	print_button(button);
	print_elevator(elevators);
}
void print_floor(){
	gotoxy(8,5);
	printf("10�� :");
	gotoxy(8,6);
	printf(" 9�� :");
	gotoxy(8,7);
	printf(" 8�� :");
	gotoxy(8,8);
	printf(" 7�� :");
	gotoxy(8,9);
	printf(" 6�� :");
	gotoxy(8,10);
	printf(" 5�� :");
	gotoxy(8,11);
	printf(" 4�� :");
	gotoxy(8,12);
	printf(" 3�� :");
	gotoxy(8,13);
	printf(" 2�� :");
	gotoxy(8,14);
	printf(" 1�� :");
	gotoxy(8,15);
	printf("B1�� :");

}
void print_button(int button[11][2]){

	int i, j;
	gotoxy(17, 15);
	for (i = 0; i<buttonX; i++){
		for (j = 0; j<buttonY; j++){
			switch (button[i][j]){
			case 0:printf("��"); break;
			case 1:printf("��"); break;
			case 2:printf("��");  break;
			case 3:printf("��"); break;
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
			case 0:printf("��"); break;
			case 20:printf("���"); break;
			case 21:printf("���"); break;
			case 22:printf("���");  break;
			case 23:printf("���"); break;
			case 24:printf("���");  break;
			case 25:printf("���"); break;
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



/////////////////////////////	 ��ǥ�� �Ҵ� && Ű�޴� ����� ���� �Լ���.		///////////////////////////////////////////////

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

////////////////////////�ֺ��� ���� ���� �� ��ȯ�� ���� �Լ�/////////////////////

int fMode(int f){
	int i;
	int mode=1, floor=1; //�ֺ��(mode)�� �ֺ�� �ش� ��(floor)
	int tMode; //�� ��� ���� �Է� Ƚ���� ���� �� ����

	modeF[f]++; //ȣ��� ���� ����++
	for(i=0;i<11;i++){
		tMode = modeF[i];
		if(tMode>mode){
			mode = tMode;
			floor = i;
		}
	}
	
	return floor;

}