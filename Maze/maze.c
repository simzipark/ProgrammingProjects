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
int temp_x[4], temp_y[4];	//0 :위 에서부터 시계방향 1: 오른 2: 아래 3: 왼
int choice[3][2];
int comX=0, comY=0;
int numX =0, numY =0;
int uWin=0, cWin=0;


int*** u_route; // 리플레이용 포인터
int*** c_route;
int*** u_route2;
int*** c_route2;
int u_t = 1;
int c_t = 1;

void show_menu();
// 좌표값 할당 함수들
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

//리플레이용 함수들
int*** TriArr(int number, int height, int width);
void FreeArr(int*** route, int number, int height);
int set_u_route(int route[X][Y], int z); //z는 이번이 몇번째 턴인지 , z=1,,2,,3... 
int set_c_route(int route[X][Y], int z);
void replay(int*** u_route, int*** c_route);


int main(int argc, char *argv[])
{

	int menu;
	int s; //게임 중 중지를 위한 변수
	int k=0; //미로가 세팅된 후에만 게임을 가능하게 하기 위한 변수
	int i,j;
	FFLUSH;

	//리플레이부분
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
					printf("이미 미로 세팅이 완료되었습니다. 다시 세팅하려면 3을 입력해주세요");
					continue;
				}
			break;
			case 2:  
				if(k==0){
					system("clear");
					printf("'1.미로 세팅'을 먼저 실행해 주세요\n");
					continue;
				}
				if(k==1){
					system("clear");
					s=playing();
					if(s==0){ //replay신호가 와서 다시 게임 진행하기 위해 
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
		printf("┏━━━━━━━━━<THE GAME OF MAZE>━━━━━━━━━┓");
		gotoxy(25,9);
		printf("┃************************************┃");
		gotoxy(25,10);
		printf("┃*	    	            	    *┃");
		gotoxy(25,11);
		printf("┃* 	[1]	MAZE SETTING        *┃");
		gotoxy(25,12);
		printf("┃*	[2]	BATTLE START 	    *┃");
		gotoxy(25,13);
		printf("┃*	[3]	RESETTING	    *┃");
		gotoxy(25,14);
		printf("┃*	[4]	QUIT    	    *┃");
		gotoxy(25,15);
		printf("┃*	    	            	    *┃");
		gotoxy(25,16);
		printf("┃************************************┃");
		gotoxy(25,17);
		printf("┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛");
		gotoxy(25,18);
		printf("┗");
}

/////////////////////////////	 좌표값 할당을 위한 함수들.		///////////////////////////////////////////////

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


//////////////////// 		배열 출력 함수.			////////////////////////////////////////
		
void print_maze(int maze[X][Y]){ 	
	int i,j;  	
	gotoxy(10,5); 	
	for (i=0;i<X;i++){ 		
		for (j=0;j<Y;j++){ 		
			switch(maze[i][j]){ 			
			case 0:printf("□"); break;
			case 1:printf("■"); break;
			case 2:printf("X");  break;
			case 3:printf("○"); break;
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
			case 0:printf("□"); break;
			case 1:printf("■"); break;
			case 2:printf("□");  break;
			case 3:printf("□"); break;
		} 	
	} 	
	 printf("\n");
	gotoxy(10,i+6);
	}
}

///////////////////////		USER 길만들기!!!!!!!!!		/////////////////////////////////////////

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
		printf(" 미로만들기 포기 :  TAB \n");
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
		printf("그곳으로 미로를 만들수는 없습니다.");
		gotoxy(40,11);
		printf("다른길을 선택해주세요");

		gotoxy(x,y);
	}
	else if(arr[x][y]==1){
		gotoxy(40,10);
		printf("이미지나온 길입니다. 다시선택하세요.");
	}
	else 
		return 1;
}



///////////////////////////////////	예외처리들 모음 함수들		//////////////////////////


int find_previous(int arr[X][Y],int x,int y) // 내가 선택한 것 바로전 미로의 길, 위치를 찾는다. (주변의 것들을 2로 바꾸어 미로를 만들지 못하게 방지)
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


int change_previousSurr(int arr[X][Y],int x,int y) // 나 자신의 위치 주변의 것들을 모두 2로 바꾸어준다.
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

int change_path(int arr[X][Y],int x,int y) // 내가 길을 선택하면 그주변에 갈 수 있는 길을 표시해준다.
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

void make_exceptions(int maze[X][Y], int x, int y) // 예외처리 이함수에 모두넣기.
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

void maze_initialize(int maze[X][Y])	// 막다른 길로 도달하였을경우 or 다시 처음부터 다시시작할 경우  => 배열초기화
{
	int i;int j;
    for(i=0;i<X;i++)
		for(j=0;j<Y;j++)
			maze[i][j]=0;
	maze[0][0]=1; maze[0][1]=3; maze[1][0]=3;
	comX=0;comY=0;
	numX=0;numY=0;
}





///////////////////////////////		컴퓨터 자동 생성 기능들		//////////////////////////////////////


void surr_setting(int x, int y)	// 주변값을 먼저 전역변수들에 넣기
{
	temp_x[0]=x;		temp_y[0]=y-1;
	temp_x[1]=x+1; 	temp_y[1]=y;
	temp_x[2]=x;		temp_y[2]=y+1;
	temp_x[3]=x-1; 	temp_y[3]=y;
}
void computer_input(int x, int y,int maze[X][Y]) //컴퓨터 미로 만들기 함수
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

void com_input(int x,int y,int maze[X][Y]) //숫자 바꾸어주고 쳐넣기
{
	int i, j;
	
	if(maze[x][y] == 3) //선택할 수 있는 path일 때
	{
		comX=x; comY=y;
		maze[x][y] = 1; //선택한 방에 1, 즉 path를 넣고
		
		find_previous(maze, x,y);
		change_path(maze,x,y);
		make_exceptions(maze,x,y);
	}
}

void array_copy(int a[X][Y], int b[X][Y]) //전역변수 배열로 복사
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



///////////////////////////////		GAME PLAY 를 위한 함수들			//////////////////////////////////


int playing(){
	
	int ur=1,cr=1;//com이나 user의 게임 각 턴 결과를 반환하는 변수 
	//0: 상대가 만든 미로를 맞추지 못해 상대 턴  1: 맞춰서 계속 진행할 수 있음 2: 미로 완성! 3: 종료 사인이 와서 종료	
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
		printf("\n\n<3<3<3축하 축하 축하 축하<3<3<3\n\n");
		printf("user: %d승 %d패\n",uWin,cWin);
		printf("com : %d승 %d패\n",cWin,uWin);

		/*printf("리플레이를 보시겠습니까? (y/n): ");
		char q= 'n';
		scanf("%c", &q);
		if(q == 'y')replay(u_route, c_route);
		*/

		replay(u_route, c_route);
		
		printf("재도전하시겠습니까?\n 1.예 2.아니요(종료)\n:");
		scanf("%d",&re);
		if(re==1) return 0;
		if(re==2) return 1;
	}
	if(cr==2){
		system("clear");
		cWin++;
		printf("\n\n\nUser LoseTT\n");
		print_gameMaze(cGame);
		printf("com : %d승 %d패\n",cWin,uWin);
		printf("user: %d승 %d패\n",uWin,cWin);
		printf("\n\nㅠㅠㅠㅠ재도전 하세요!ㅠㅠㅠㅠ\n\n");
		/*
		printf("리플레이를 보시겠습니까? (y/n): ");
		char q = 'n';
		scanf("%c", &q);
`		if(q == 'y')replay(u_route, c_route);	
		*/
		replay(u_route, c_route);
		
		printf("재도전하시겠습니까?\n 1.예 2.아니요(종료)");
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

	if(who==0){ //사용자의 턴
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
		printf(" 미로만들기 포기 :  TAB \n");
		//system("cls");	
		}
	}

	if(who==1){ //컴퓨터의 턴
		printf("com의 결정\n");
		
		Sleep(500);
		printf("현재: %d%d -> 선택:",comX,comY);
		if(cCheck ==1){  //해당 칸에서 실패한 적이 있어서 이미 이동 가능한 칸에 대한 배열이 만들어져 있을 때
			srand( (unsigned)time(NULL));
			num = rand()%theN;
			cX=choice[num][0];
			cY=choice[num][1];
			Sleep(800);
			printf(" %d%d\n",cX,cY);
		}
		if(cCheck ==0 ){ //해당 칸에서 처음 도전일 때->해당 칸에서 이동 가능한 칸의 배열을 처음 만듦(comGame함수)
			theN=comGame(game,comX,comY);
			srand( (unsigned)time(NULL));
			num = rand()%theN;
			cX=choice[num][0];
			cY=choice[num][1];
			Sleep(800);
			printf(" %d%d\n",cX,cY);
		}

		if(game[cX][cY] == 3){ //선택할 수 있는 path일 때
		if(maze[cX][cY]==1)
		{ //정답 미로와 대조
			game[cX][cY] = 1; //선택한 방에 1, 즉 path를 넣고
			
			if(game[9][9] == 1)
				return 2;

			cCheck =0;//이동했으므로 다음 칸에서는 이동가능한 칸의 배열을 다시 처음부터 만들어야 하므로

			if(who==1){ //이동했으므로 현재 좌표 바뀜
				comX = cX;
				comY = cY;
			}
			change_path(game,cX,cY);
			find_previous(game,cX,cY);
			make_exceptions(game,cX,cY);
			return 1;

		} 
		else 
		{ //상대가 만든 미로와 틀려서 상대에게 턴이 넘어감
			printf("틀렸습니다! 턴이 상대에게 넘어갑니다!\n\n");
			
			//컴퓨터의 경우 중복된 선택을 하지 않게 하기 위해 원래 존재하던 배열에서 선택했던 걸 뺴준다.
			if(who==1){
				cCheck =1; //이미 만들어지 배열을 사용해야 하므로
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
		printf("상대에게 턴이 돌아갑니다.");
		gotoxy(x,y);
		return 0;
	}
	else if(game[x][y]==1){
		gotoxy(40,10);
		printf("이미 지나온 길입니다. 다시선택하세요.");
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




///////////////////////// 		REPLAY 위한 함수들 		////////////////////////////////////////


void show_maze(int maze[X][Y]) // replay를 위한 함수이다.
{
	int i, j;

	//maze 출력
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

// 3차원 배열을 동적으로 할당
int*** TriArr(int number, int height, int width) //첫 인덱스는 각자의 턴, 두번째 세번째는 value 들어갈 좌표.
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

    // 모든 배열원소 초기화
    for (i = 0; i < number; i++)
        for (j = 0; j < height; j++)
            for (k = 0; k < width; k++)
                ptr[i][j][k] = '\0';

    return ptr;
}



// 동적으로 할당 된 메모리 해제
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



//유저 여태까지 턴들 3차원 배열에 저장
int set_u_route(int route[X][Y], int z){ //z는 이번이 몇번째 턴인지 , z=1,,2,,3... 

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
//컴퓨터 여태까지 턴들 3차원 배열에 저장
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
		printf("\n유저\t\t\t컴퓨터\n");
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
