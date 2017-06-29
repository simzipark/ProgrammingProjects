#include <stdio.h>
#include <Windows.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>  

#define UPKEY			65 
#define DOWNKEY		66 
#define LEFTKEY		68 
#define RIGHTKEY		67 
#define ESCKEY		27 
#define TABKEY		9 
#define ENTERKEY		10 
#define SPACEKEY		32 
#define FFLUSH  while(getchar() != '\n')
#define LEFT y-1
#define RIGHT y+1
#define UP x-1
#define DOWN x+1

#define X 10
#define Y 10

int uGame[X][Y];
int cGame[X][Y];
int madeByU[X][Y];
int madeByC[X][Y];
int comMaze[X][Y];
int temp_x[4], temp_y[4];	//0 :�� �������� �ð���� 1: ���� 2: �Ʒ� 3: ��
int choice[3][2];
int comX=0, comY=0;
int numX =0, numY =0;
int uWin=0, cWin=0;


int*** u_route; // ���÷��̿� ������
int*** c_route;
int*** u_route2;
int*** c_route2;
int u_t = 1;
int c_t = 1;

void show_menu();
// ��ǥ�� �Ҵ� �Լ���
void gotoxy(int col, int row);
int getch(void);
int getkey();

void print_maze(int maze[X][Y]);
int playing();
int game_enter(int game[X][Y],int maze[X][Y], int y,int x);
int thisTurn(int game[X][Y], int maze[X][Y],int who);
int input_path(int maze[X][Y]);
void maze_initialize(int maze[X][Y]);
void com_input(int x,int y,int maze[X][Y]);
int mazeSetting(int maze[X][Y]);
void computer_input(int x, int y,int maze[X][Y]);
void surr_setting(int x, int y);
int change_previousSurr(int arr[X][Y],int x,int y);
int find_previous(int arr[X][Y],int x,int y);
void array_copy(int a[X][Y], int b[X][Y]);
void show_maze(int maze[X][Y]);
int count_three(int maze[X][Y]);
int comGame(int game[X][Y],int x, int y);

//���÷��̿� �Լ���
int*** TriArr(int number, int height, int width);
void FreeArr(int*** route, int number, int height);
int set_u_route(int route[X][Y], int z); //z�� �̹��� ���° ������ , z=1,,2,,3... 
int set_c_route(int route[X][Y], int z);
void replay(int*** u_route, int*** c_route);


int main(int argc, char *argv[])
{

	int menu;
	int s; //���� �� ������ ���� ����
	int k=0; //�̷ΰ� ���õ� �Ŀ��� ������ �����ϰ� �ϱ� ���� ����
	int i,j;
	FFLUSH;

	//���÷��̺κ�
	u_route = TriArr(50, X, Y);
 	c_route = TriArr(50, X, Y);	
	//

	while(1){
		show_menu();
		printf("selection  >>  ");
		scanf("%d",&menu);

		switch(menu){
			case 1:
				if(k!=1){
					maze_initialize(madeByU);					
					while(madeByU[9][9] != 1){
						system("clear");
						print_maze(madeByU);
						s=input_path(madeByU);
						if(s==0) return 0;
					}

					maze_initialize(comMaze);
					maze_initialize(madeByC);
					srand( (unsigned)time(NULL));
					mazeSetting(madeByC);
					
					printf("\n== COMPUTER MAZE ==\n");
					show_maze(madeByC);

					k=1;
					continue;
				}
				if(k==1){
					system("clear");
					printf("�̹� �̷� ������ �Ϸ�Ǿ����ϴ�. �ٽ� �����Ϸ��� 3�� �Է����ּ���");
					continue;
				}
			break;
			case 2:  
				if(k==0){
					system("clear");
					printf("'1.�̷� ����'�� ���� ������ �ּ���\n");
					continue;
				}
				if(k==1){
					system("clear");
					s=playing();
					if(s==0){ //replay��ȣ�� �ͼ� �ٽ� ���� �����ϱ� ���� 
						maze_initialize(madeByU);
						maze_initialize(madeByC);
						k=0;
						FFLUSH;
						continue;
					}
					if(s==1) return 0;
				}
				
			case 3: 
				maze_initialize(madeByU);
				maze_initialize(madeByC);		
				k=0;
				FFLUSH;

			
				continue;
			
			case 0: return 0;
			
			default: break;
			FFLUSH;
		}
		system("cls");

	}

	FreeArr(u_route, X, Y);
	FreeArr(u_route2, X, Y);
          FreeArr(c_route, X, Y);
          FreeArr(c_route2, X, Y);

	return 0;
}

void show_menu()
{		
		system("clear");
		gotoxy(25,8);
		printf("��������������������<THE GAME OF MAZE>��������������������");
		gotoxy(25,9);
		printf("��************************************��");
		gotoxy(25,10);
		printf("��*	    	            	    *��");
		gotoxy(25,11);
		printf("��* 	[1]	MAZE SETTING        *��");
		gotoxy(25,12);
		printf("��*	[2]	BATTLE START 	    *��");
		gotoxy(25,13);
		printf("��*	[3]	RESETTING	    *��");
		gotoxy(25,14);
		printf("��*	[4]	QUIT    	    *��");
		gotoxy(25,15);
		printf("��*	    	            	    *��");
		gotoxy(25,16);
		printf("��************************************��");
		gotoxy(25,17);
		printf("����������������������������������������������������������������������������");
		gotoxy(25,18);
		printf("��");
}

/////////////////////////////	 ��ǥ�� �Ҵ��� ���� �Լ���.		///////////////////////////////////////////////

void gotoxy(int col, int row) { 	
	 printf("\e[%d%;%dH", row, col);
} 
int getch(void)
{
    struct termios oldt,newt;
    int ch;
    tcgetattr( STDIN_FILENO, &oldt );
    newt = oldt;
    newt.c_lflag &= ~( ICANON | ECHO );
    tcsetattr( STDIN_FILENO, TCSANOW, &newt );
    ch = getchar();
    tcsetattr( STDIN_FILENO, TCSANOW, &oldt );
    return ch;
}
int getkey() {     
	int ch;
	if ((ch=getch())==0xE0)        
		ch=getch();    
	return ch; 
}


//////////////////// 		�迭 ��� �Լ�.			////////////////////////////////////////
		
void print_maze(int maze[X][Y]){ 	
	int i,j;  	
	gotoxy(10,5); 	
	for (i=0;i<X;i++){ 		
		for (j=0;j<Y;j++){ 		
			switch(maze[i][j]){ 			
			case 0:printf("��"); break;
			case 1:printf("��"); break;
			case 2:printf("X");  break;
			case 3:printf("��"); break;
		} 	
	} 	
	 printf("\n");
	gotoxy(10,i+6);
	}
}

void print_gameMaze(int maze[X][Y]){ 	
	int i,j;  	
	gotoxy(10,5); 	
	for (i=0;i<X;i++){ 		
		for (j=0;j<Y;j++){ 		
			switch(maze[i][j]){ 			
			case 0:printf("��"); break;
			case 1:printf("��"); break;
			case 2:printf("��");  break;
			case 3:printf("��"); break;
		} 	
	} 	
	 printf("\n");
	gotoxy(10,i+6);
	}
}

///////////////////////		USER �游���!!!!!!!!!		/////////////////////////////////////////

int input_path(int maze[X][Y]){
	
	int i,j;
	int x=0; int y=0;
	int num1,num2;

	void maze_initialize(maze);

	while(1){

		print_maze(maze);
		gotoxy(10+x,5+y);
		
		switch(getkey())
		{
			case LEFTKEY:	 	if(x>0 && (numX==x || x==numX+1)&& numY==y) x--;			break;
			case RIGHTKEY:	 if(x<(X-1) && (numX-1 == x || x==numX)&& numY==y) x++;		break;
			case UPKEY:		 if(y>0 && (y ==numY+1 ||y ==numY) && numX==x) y--;			break;
			case DOWNKEY:	 if(y<Y-1 && (y ==numY-1 ||y ==numY)&& numX==x) y++;		break;
			case ENTERKEY:  
				enter(maze,x,y);	
				if(x ==9 && y==9)
					return 1;
				break;
			case TABKEY:  return 3;
			default:     break;
		}
		gotoxy(42,20);
		printf(" �̷θ���� ���� :  TAB \n");
		//system("cls");	
	}
}

int enter(int arr[X][Y],int y,int x)
{

	if(arr[x][y]==3){
		numX=y;
		numY=x;
		arr[x][y]=1;
		change_path(arr,x,y);
		find_previous(arr,x,y);
		make_exceptions(arr,x,y);
	}
	else if(arr[x][y]==2){
		gotoxy(40,10);
		printf("�װ����� �̷θ� ������� �����ϴ�.");
		gotoxy(40,11);
		printf("�ٸ����� �������ּ���");

		gotoxy(x,y);
	}
	else if(arr[x][y]==1){
		gotoxy(40,10);
		printf("�̹������� ���Դϴ�. �ٽü����ϼ���.");
	}
	else 
		return 1;
}



///////////////////////////////////	����ó���� ���� �Լ���		//////////////////////////


int find_previous(int arr[X][Y],int x,int y) // ���� ������ �� �ٷ��� �̷��� ��, ��ġ�� ã�´�. (�ֺ��� �͵��� 2�� �ٲپ� �̷θ� ������ ���ϰ� ����)
{
	if(arr[x-1][y]==1){
		change_previousSurr(arr,x-1,y); return 0;}
	else if(arr[x][y-1]==1){
		change_previousSurr(arr,x,y-1); return 0;}
	else if(arr[x][y+1]==1){
		change_previousSurr(arr,x,y+1); return 0;}
	else if(arr[x+1][y]==1){
		change_previousSurr(arr,x+1,y); return 0;}
}


int change_previousSurr(int arr[X][Y],int x,int y) // �� �ڽ��� ��ġ �ֺ��� �͵��� ��� 2�� �ٲپ��ش�.
{
	if(arr[x-1][y]==3 && x!=0)
		arr[x-1][y]=2;
	if(arr[x][y-1]==3 && y!=0)
		arr[x][y-1]=2;
	if((arr[x][y+1]==3) && (y!=Y-1))
		arr[x][y+1]=2;
	if((arr[x+1][y]==3)&&(x!=X-1))
		arr[x+1][y]=2;
	if(arr[0][1]==1)
		arr[1][0]=2;
	return 0;
}

int change_path(int arr[X][Y],int x,int y) // ���� ���� �����ϸ� ���ֺ��� �� �� �ִ� ���� ǥ�����ش�.
{
	if(arr[x-1][y]==0 && x!=0)
		arr[x-1][y]=3;
	if(arr[x][y-1]==0 && y!=0)
		arr[x][y-1]=3;
	if((arr[x][y+1]==0) && (y!=Y-1))
		arr[x][y+1]=3;
	if((arr[x+1][y]==0)&&(x!=X-1))
		arr[x+1][y]=3;
	return 0;
}

void make_exceptions(int maze[X][Y], int x, int y) // ����ó�� ���Լ��� ��γֱ�.
{

		if(x == 0 && maze[x][LEFT] != 2 && maze[x][LEFT] != 1)
			maze[x][LEFT] = 2;

		if(y == 0 && maze[UP][y] != 2 && maze[UP][y] != 1)
			maze[UP][y] = 2;

		//////////////////////////////////////////////////////////////

		if((x == 1 || x == 8) && (y != 0 && maze[x][LEFT] != 1))
			maze[x][LEFT] = 2;

		if((y == 1 || y == 8) && (x != 0 && maze[UP][y] != 1))
			maze[UP][y] = 2;

		//////////////////////////////////////////////////////////////

		if(y == 9 && maze[UP][y] != 1)
			maze[UP][y] = 2;
		if(x == 9 && maze[x][LEFT] != 1)
			maze[x][LEFT] =2;

		//////////////////////////////////////////////////////////////

		if(maze[UP-1][y]==2 && maze[UP][LEFT]==2 && maze[UP][RIGHT] ==2)
			maze[UP][y] =2;

		if(maze[UP][RIGHT]==2 && maze[DOWN][RIGHT] == 2 && maze[x][RIGHT+1] == 2)
			maze[x][RIGHT] =2;
}

void maze_initialize(int maze[X][Y])	// ���ٸ� ��� �����Ͽ������ or �ٽ� ó������ �ٽý����� ���  => �迭�ʱ�ȭ
{
	int i;int j;
    for(i=0;i<X;i++)
		for(j=0;j<Y;j++)
			maze[i][j]=0;
	maze[0][0]=1; maze[0][1]=3; maze[1][0]=3;
	comX=0;comY=0;
	numX=0;numY=0;
}





///////////////////////////////		��ǻ�� �ڵ� ���� ��ɵ�		//////////////////////////////////////


void surr_setting(int x, int y)	// �ֺ����� ���� ���������鿡 �ֱ�
{
	temp_x[0]=x;		temp_y[0]=y-1;
	temp_x[1]=x+1; 	temp_y[1]=y;
	temp_x[2]=x;		temp_y[2]=y+1;
	temp_x[3]=x-1; 	temp_y[3]=y;
}
void computer_input(int x, int y,int maze[X][Y]) //��ǻ�� �̷� ����� �Լ�
{
	int num ;
	
	num=rand()%4;
	surr_setting(x,y);
	com_input(temp_x[num],temp_y[num],maze);
//	print_maze(maze);
}

int mazeSetting(int maze[X][Y])
{
	
	while(maze[9][9] != 1) 
	{
		if(maze[9][9] == 1){
			array_copy(madeByC,maze);
			break;
		}

		else if( count_three(maze) == 0 )
		{
			maze_initialize(maze);
		}

		computer_input(comX,comY,maze);

	}
}

void com_input(int x,int y,int maze[X][Y]) //���� �ٲپ��ְ� �ĳֱ�
{
	int i, j;
	
	if(maze[x][y] == 3) //������ �� �ִ� path�� ��
	{
		comX=x; comY=y;
		maze[x][y] = 1; //������ �濡 1, �� path�� �ְ�
		
		find_previous(maze, x,y);
		change_path(maze,x,y);
		make_exceptions(maze,x,y);
	}
}

void array_copy(int a[X][Y], int b[X][Y]) //�������� �迭�� ����
{
    int i;int j;
    for(i=0;i<X;i++)
		for(j=0;j<Y;j++)
			a[i][j]=b[i][j];
}

int count_three(int maze[X][Y])
{
	int i, j;
	int count=0;

	for(i=0; i<X; i++)
	{
		for(j=0; j<Y; j++)
			if(maze[i][j] == 3)
				count++;
	}

	return count;
}



///////////////////////////////		GAME PLAY �� ���� �Լ���			//////////////////////////////////


int playing(){
	
	int ur=1,cr=1;//com�̳� user�� ���� �� �� ����� ��ȯ�ϴ� ���� 
	//0: ��밡 ���� �̷θ� ������ ���� ��� ��  1: ���缭 ��� ������ �� ���� 2: �̷� �ϼ�! 3: ���� ������ �ͼ� ����	
	int re = 0;
	maze_initialize(uGame);
	maze_initialize(cGame);

	while(ur!=2||cr!=2){
		while(ur!=0){ 
			system("clear");
			gotoxy(0,0);
			printf("User's turn\n");	
			print_gameMaze(uGame);
			ur = thisTurn(uGame,madeByC,0);
			
			if(ur==3)	return 1;
			if(ur==2)	break;
			cr=1;
		}
		if(ur==2)	break;
	
		while(cr!=0){
			system("clear");
			gotoxy(0,0);
			printf("Com's turn\n");
			print_gameMaze(cGame);
			cr = thisTurn(cGame,madeByU,1);
		
			if(cr==2)	break;
			ur=1;
		}
		if(cr==2)	break;
	}
	if(ur==2){
		system("clear");
		uWin++;
		printf("\n\n\nUser Win<3\n");
		print_gameMaze(uGame);
		printf("\n\n<3<3<3���� ���� ���� ����<3<3<3\n\n");
		printf("user: %d�� %d��\n",uWin,cWin);
		printf("com : %d�� %d��\n",cWin,uWin);

		/*printf("���÷��̸� ���ðڽ��ϱ�? (y/n): ");
		char q= 'n';
		scanf("%c", &q);
		if(q == 'y')replay(u_route, c_route);
		*/

		replay(u_route, c_route);
		
		printf("�絵���Ͻðڽ��ϱ�?\n 1.�� 2.�ƴϿ�(����)\n:");
		scanf("%d",&re);
		if(re==1) return 0;
		if(re==2) return 1;
	}
	if(cr==2){
		system("clear");
		cWin++;
		printf("\n\n\nUser LoseTT\n");
		print_gameMaze(cGame);
		printf("com : %d�� %d��\n",cWin,uWin);
		printf("user: %d�� %d��\n",uWin,cWin);
		printf("\n\n�ФФФ��絵�� �ϼ���!�ФФФ�\n\n");
		/*
		printf("���÷��̸� ���ðڽ��ϱ�? (y/n): ");
		char q = 'n';
		scanf("%c", &q);
`		if(q == 'y')replay(u_route, c_route);	
		*/
		replay(u_route, c_route);
		
		printf("�絵���Ͻðڽ��ϱ�?\n 1.�� 2.�ƴϿ�(����)");
		scanf("%d",&re);
		if(re==1) return 0;
		if(re==2) return 1;
	}
}


int thisTurn(int game[X][Y], int maze[X][Y],int who){

	int temp;
	int uX=0, uY=0;
	static int cX=0, cY=0;
	int i, j;
	static int theN;
	int s;
	int num;
	static int cCheck=0;

	if(who==0){ //������� ��
		uX = numX;
		uY = numY;
		while(1){
		print_gameMaze(game);
		gotoxy(10+uX,5+uY);
//		printf("%d %d\n",uX,uY );
		switch(getkey())
		{
			case LEFTKEY:	 	if(uX>0 && (numX==uX || uX==numX+1)&& numY==uY) uX--;			break;
			case RIGHTKEY:	 if(uX<(X-1) && (numX-1 == uX || uX==numX)&& numY==uY) uX++;		break;
			case UPKEY:		 if(uY>0 && (uY ==numY+1 ||uY ==numY) && numX==uX) uY--;			break;
			case DOWNKEY:	 if(uY<Y-1 && (uY ==numY-1 ||uY ==numY)&& numX==uX) uY++;		break;
			case ENTERKEY:  
				s=game_enter(game,maze,uX,uY);	
				if(s ==0||s==1)
					return s;
				if(uX ==9 && uY==9)
					return 2;
				break;
			case TABKEY:  return 3;
			default:     break;
		}
		gotoxy(42,20);
		printf(" �̷θ���� ���� :  TAB \n");
		//system("cls");	
		}
	}

	if(who==1){ //��ǻ���� ��
		printf("com�� ����\n");
		
		Sleep(500);
		printf("����: %d%d -> ����:",comX,comY);
		if(cCheck ==1){  //�ش� ĭ���� ������ ���� �־ �̹� �̵� ������ ĭ�� ���� �迭�� ������� ���� ��
			srand( (unsigned)time(NULL));
			num = rand()%theN;
			cX=choice[num][0];
			cY=choice[num][1];
			Sleep(800);
			printf(" %d%d\n",cX,cY);
		}
		if(cCheck ==0 ){ //�ش� ĭ���� ó�� ������ ��->�ش� ĭ���� �̵� ������ ĭ�� �迭�� ó�� ����(comGame�Լ�)
			theN=comGame(game,comX,comY);
			srand( (unsigned)time(NULL));
			num = rand()%theN;
			cX=choice[num][0];
			cY=choice[num][1];
			Sleep(800);
			printf(" %d%d\n",cX,cY);
		}

		if(game[cX][cY] == 3){ //������ �� �ִ� path�� ��
		if(maze[cX][cY]==1)
		{ //���� �̷ο� ����
			game[cX][cY] = 1; //������ �濡 1, �� path�� �ְ�
			
			if(game[9][9] == 1)
				return 2;

			cCheck =0;//�̵������Ƿ� ���� ĭ������ �̵������� ĭ�� �迭�� �ٽ� ó������ ������ �ϹǷ�

			if(who==1){ //�̵������Ƿ� ���� ��ǥ �ٲ�
				comX = cX;
				comY = cY;
			}
			change_path(game,cX,cY);
			find_previous(game,cX,cY);
			make_exceptions(game,cX,cY);
			return 1;

		} 
		else 
		{ //��밡 ���� �̷ο� Ʋ���� ��뿡�� ���� �Ѿ
			printf("Ʋ�Ƚ��ϴ�! ���� ��뿡�� �Ѿ�ϴ�!\n\n");
			
			//��ǻ���� ��� �ߺ��� ������ ���� �ʰ� �ϱ� ���� ���� �����ϴ� �迭���� �����ߴ� �� ���ش�.
			if(who==1){
				cCheck =1; //�̹� ������� �迭�� ����ؾ� �ϹǷ�
				while(num!=theN){
					choice[num][0]=choice[num+1][0];
					choice[num][1]=choice[num+1][1];
					num=num+1;
				}
				theN--;
			}

			return 0;
		}
			
	} 
	}

	
}

int game_enter(int game[X][Y],int maze[X][Y], int y,int x)
{

	if(maze[x][y]==1 && game[x][y]==3){
		numX=y;
		numY=x;
		game[x][y]=1;
		change_path(game,x,y);
		find_previous(game,x,y);
		make_exceptions(game,x,y);
		return 1;
	}
	else if(maze[x][y]!=1){
		gotoxy(40,10);
		printf("��뿡�� ���� ���ư��ϴ�.");
		gotoxy(x,y);
		return 0;
	}
	else if(game[x][y]==1){
		gotoxy(40,10);
		printf("�̹� ������ ���Դϴ�. �ٽü����ϼ���.");
		return 1;
	}
}



int comGame(int game[X][Y],int x, int y){
	int i=0;
	if(x>0){
		if(game[x-1][y]==3){
			choice[i][0]=x-1;
			choice[i][1]=y;
			i++;
		}
	}
	if(y>0){
		if(game[x][y-1]==3){
			choice[i][0]=x;
			choice[i][1]=y-1;
			i++;
		}
	}
	if(y<9){
		if(game[x][y+1]==3){
			choice[i][0]=x;
			choice[i][1]=y+1;
			i++;
		}
	}
	if(x<9){
		if(game[x+1][y]==3){
			choice[i][0]=x+1;
			choice[i][1]=y;
			i++;
		}
	}
	return i;
}




///////////////////////// 		REPLAY ���� �Լ��� 		////////////////////////////////////////


void show_maze(int maze[X][Y]) // replay�� ���� �Լ��̴�.
{
	int i, j;

	//maze ���
	for(i=0; i<X; i++)

	{
		for(j=0; j<Y; j++)
		{
			if(maze[i][j] == 1)
				printf("# ", maze[i][j]);
			else 
				printf("_ ");
			if(j==Y-1)
				printf("\n");
		}
	}
}

// 3���� �迭�� �������� �Ҵ�
int*** TriArr(int number, int height, int width) //ù �ε����� ������ ��, �ι�° ����°�� value �� ��ǥ.
{
    int i, j, k;
    int*** ptr;
   
    if ( ( ptr = (int***)malloc(number * sizeof(int**)) ) == NULL ) 
    {
        printf("\nMemory allocation failure\n");
        
    }

    for (i = 0; i < number; i++)
        if ( ( ptr[i] = (int**)malloc(height * sizeof(int*)) ) == NULL ) 
        {
            printf("\nMemory allocation failure\n");
           
        }

    for (i = 0; i < number; i++)
        for (j = 0; j < height; j++)
            if ( ( ptr[i][j] = (int*)malloc(width * sizeof(int)) ) == NULL ) 
            {
                printf("\nMemory allocation failure\n");
                
            }

    // ��� �迭���� �ʱ�ȭ
    for (i = 0; i < number; i++)
        for (j = 0; j < height; j++)
            for (k = 0; k < width; k++)
                ptr[i][j][k] = '\0';

    return ptr;
}



// �������� �Ҵ� �� �޸� ����
void FreeArr(int*** route, int number, int height)
{
    int i,j;
    for(i=0; i<number; i++)
    {
        for(j=0; j<height; j++)
        {
            free(route[i][j]);


        }
            free(route[i]);

    }
            free(route);

}



//���� ���±��� �ϵ� 3���� �迭�� ����
int set_u_route(int route[X][Y], int z){ //z�� �̹��� ���° ������ , z=1,,2,,3... 

   int i, j, k;
	
  if( z < 50 ){	
   for(i = 0; i < X; i++){

	for(j = 0; j < X; j++){

		u_route[z-1][i][j] = route[i][j];			
			

	}		
   }
   }
 

   else if(z == 50){
      
      u_route2 = TriArr(z+100, X, Y);
 
      for(k = 0; k < (z-1); k++){

      for(i = 0; i < X; i++){

         for(j = 0; j <Y; j++){
               
            u_route2[k][i][j] = u_route[k][i][j];         
                        
         }

      }
      }

      for(i = 0; i < X; i++){

         for(j = 0; j <Y; j++){
               
            u_route2[z-1][i][j] = route[i][j];         
                        
         }

      }
 
   FreeArr(u_route, X, Y);
   }

   else if(z > 50){

		   for(i = 0; i < X; i++){

			for(j = 0; j < X; j++){

				u_route2[z-1][i][j] = route[i][j];			
			

	     		}		
		   }
	   

   }	
   return 1;

	
}
//��ǻ�� ���±��� �ϵ� 3���� �迭�� ����
int set_c_route(int route[X][Y], int z){

   int i, j, k;
	
	
  if( z < 50 ){	
   for(i = 0; i < X; i++){

	for(j = 0; j < X; j++){

		c_route[z-1][i][j] = route[i][j];			
			

	}		
   }
   }
 

   else if(z == 50){
      
      c_route2 = TriArr(z+100, X, Y);
 
      for(k = 0; k < (z-1); k++){

      for(i = 0; i < X; i++){

         for(j = 0; j <Y; j++){
               
            c_route2[k][i][j] = c_route[k][i][j];         
                        
         }

      }
      }

      for(i = 0; i < X; i++){

         for(j = 0; j <Y; j++){
               
            c_route2[z-1][i][j] = route[i][j];         
                        
         }

      }
 
   FreeArr(c_route, X, Y);
   }

   else if(z > 50){

		
		   for(i = 0; i < X; i++){

			for(j = 0; j < X; j++){

				c_route2[z-1][i][j] = route[i][j];			
			

	     		}		
		   }
	   

   }	 
   return 1;

}

void replay(int*** u_route, int*** c_route){

	int i, j, k, z;
	int value = 0;

	if(u_t > c_t){

		z = u_t;
	}
	else{
		z = c_t;
	}
	printf("%d", z);
	for(k = 0; k < (z-1); k++){
		Sleep(700);
		printf("\n����\t\t\t��ǻ��\n");
		for(i=0; i<10; i++)
		{

		
			for(j=0; j<Y; j++)
			{	
			    if(k > (u_t -2)){
				
				if(u_t < 50) value = u_route[u_t-2][i][j];
				else value = u_route2[u_t-2][i][j];
				if(value == 1)
					printf("# ");
				else 
					printf("_ ");
			    }

			    else{
			
				if(u_t < 50) value = u_route[k][i][j];
				else value = u_route2[k][i][j];
	
				if(value == 1)
					printf("# ");
				else 
					printf("_ ");
			
				}
                         }
	 			printf("\t");				

	
			for(j=0; j<Y; j++)
			{	
			    if(k > (c_t -2)){
				
				if(c_t < 50) value = c_route[c_t-2][i][j];
				else value = c_route2[c_t-2][i][j];
				if(value == 1)
					printf("# ");
				else 
					printf("_ ");
				if(j==Y-1)
					printf("\n");
			    }

			    else{
				if(c_t < 50) value = c_route[k][i][j];
				else value = c_route2[k][i][j];
	
				if(value == 1)
					printf("# ");
				else 
					printf("_ ");
				if(j==Y-1)
					printf("\n");		
			
				}
                        }

			
		}

	}

}
