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
int temp_x[4], temp_y[4];	//0 :是 拭辞採斗 獣域号狽 1: 神献 2: 焼掘 3: 図
int choice[3][2];
int comX=0, comY=0;
int numX =0, numY =0;
int uWin=0, cWin=0;


int*** u_route; // 軒巴傾戚遂 匂昔斗
int*** c_route;
int*** u_route2;
int*** c_route2;
int u_t = 1;
int c_t = 1;

void show_menu();
// 疎妊葵 拝雁 敗呪級
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

//軒巴傾戚遂 敗呪級
int*** TriArr(int number, int height, int width);
void FreeArr(int*** route, int number, int height);
int set_u_route(int route[X][Y], int z); //z澗 戚腰戚 護腰属 渡昔走 , z=1,,2,,3... 
int set_c_route(int route[X][Y], int z);
void replay(int*** u_route, int*** c_route);


int main(int argc, char *argv[])
{

	int menu;
	int s; //惟績 掻 掻走研 是廃 痕呪
	int k=0; //耕稽亜 室特吉 板拭幻 惟績聖 亜管馬惟 馬奄 是廃 痕呪
	int i,j;
	FFLUSH;

	//軒巴傾戚採歳
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
					printf("戚耕 耕稽 室特戚 刃戟鞠醸柔艦陥. 陥獣 室特馬形檎 3聖 脊径背爽室推");
					continue;
				}
			break;
			case 2:  
				if(k==0){
					system("clear");
					printf("'1.耕稽 室特'聖 胡煽 叔楳背 爽室推\n");
					continue;
				}
				if(k==1){
					system("clear");
					s=playing();
					if(s==0){ //replay重硲亜 人辞 陥獣 惟績 遭楳馬奄 是背 
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
		printf("ΞΜΜΜΜΜΜΜΜΜ<THE GAME OF MAZE>ΜΜΜΜΜΜΜΜΜΟ");
		gotoxy(25,9);
		printf("Ν************************************Ν");
		gotoxy(25,10);
		printf("Ν*	    	            	    *Ν");
		gotoxy(25,11);
		printf("Ν* 	[1]	MAZE SETTING        *Ν");
		gotoxy(25,12);
		printf("Ν*	[2]	BATTLE START 	    *Ν");
		gotoxy(25,13);
		printf("Ν*	[3]	RESETTING	    *Ν");
		gotoxy(25,14);
		printf("Ν*	[4]	QUIT    	    *Ν");
		gotoxy(25,15);
		printf("Ν*	    	            	    *Ν");
		gotoxy(25,16);
		printf("Ν************************************Ν");
		gotoxy(25,17);
		printf("ΡΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΠ");
		gotoxy(25,18);
		printf("Ρ");
}

/////////////////////////////	 疎妊葵 拝雁聖 是廃 敗呪級.		///////////////////////////////////////////////

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


//////////////////// 		壕伸 窒径 敗呪.			////////////////////////////////////////
		
void print_maze(int maze[X][Y]){ 	
	int i,j;  	
	gotoxy(10,5); 	
	for (i=0;i<X;i++){ 		
		for (j=0;j<Y;j++){ 		
			switch(maze[i][j]){ 			
			case 0:printf("÷"); break;
			case 1:printf("＝"); break;
			case 2:printf("X");  break;
			case 3:printf("】"); break;
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
			case 0:printf("÷"); break;
			case 1:printf("＝"); break;
			case 2:printf("÷");  break;
			case 3:printf("÷"); break;
		} 	
	} 	
	 printf("\n");
	gotoxy(10,i+6);
	}
}

///////////////////////		USER 掩幻級奄!!!!!!!!!		/////////////////////////////////////////

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
		printf(" 耕稽幻級奄 匂奄 :  TAB \n");
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
		printf("益員生稽 耕稽研 幻級呪澗 蒸柔艦陥.");
		gotoxy(40,11);
		printf("陥献掩聖 識澱背爽室推");

		gotoxy(x,y);
	}
	else if(arr[x][y]==1){
		gotoxy(40,10);
		printf("戚耕走蟹紳 掩脊艦陥. 陥獣識澱馬室推.");
	}
	else 
		return 1;
}



///////////////////////////////////	森須坦軒級 乞製 敗呪級		//////////////////////////


int find_previous(int arr[X][Y],int x,int y) // 鎧亜 識澱廃 依 郊稽穿 耕稽税 掩, 是帖研 達澗陥. (爽痕税 依級聖 2稽 郊荷嬢 耕稽研 幻級走 公馬惟 号走)
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


int change_previousSurr(int arr[X][Y],int x,int y) // 蟹 切重税 是帖 爽痕税 依級聖 乞砧 2稽 郊荷嬢層陥.
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

int change_path(int arr[X][Y],int x,int y) // 鎧亜 掩聖 識澱馬檎 益爽痕拭 哀 呪 赤澗 掩聖 妊獣背層陥.
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

void make_exceptions(int maze[X][Y], int x, int y) // 森須坦軒 戚敗呪拭 乞砧隔奄.
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

void maze_initialize(int maze[X][Y])	// 厳陥献 掩稽 亀含馬心聖井酔 or 陥獣 坦製採斗 陥獣獣拙拝 井酔  => 壕伸段奄鉢
{
	int i;int j;
    for(i=0;i<X;i++)
		for(j=0;j<Y;j++)
			maze[i][j]=0;
	maze[0][0]=1; maze[0][1]=3; maze[1][0]=3;
	comX=0;comY=0;
	numX=0;numY=0;
}





///////////////////////////////		陳濃斗 切疑 持失 奄管級		//////////////////////////////////////


void surr_setting(int x, int y)	// 爽痕葵聖 胡煽 穿蝕痕呪級拭 隔奄
{
	temp_x[0]=x;		temp_y[0]=y-1;
	temp_x[1]=x+1; 	temp_y[1]=y;
	temp_x[2]=x;		temp_y[2]=y+1;
	temp_x[3]=x-1; 	temp_y[3]=y;
}
void computer_input(int x, int y,int maze[X][Y]) //陳濃斗 耕稽 幻級奄 敗呪
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

void com_input(int x,int y,int maze[X][Y]) //収切 郊荷嬢爽壱 団隔奄
{
	int i, j;
	
	if(maze[x][y] == 3) //識澱拝 呪 赤澗 path析 凶
	{
		comX=x; comY=y;
		maze[x][y] = 1; //識澱廃 号拭 1, 聡 path研 隔壱
		
		find_previous(maze, x,y);
		change_path(maze,x,y);
		make_exceptions(maze,x,y);
	}
}

void array_copy(int a[X][Y], int b[X][Y]) //穿蝕痕呪 壕伸稽 差紫
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



///////////////////////////////		GAME PLAY 研 是廃 敗呪級			//////////////////////////////////


int playing(){
	
	int ur=1,cr=1;//com戚蟹 user税 惟績 唖 渡 衣引研 鋼発馬澗 痕呪 
	//0: 雌企亜 幻窮 耕稽研 限蓄走 公背 雌企 渡  1: 限仲辞 域紗 遭楳拝 呪 赤製 2: 耕稽 刃失! 3: 曽戟 紫昔戚 人辞 曽戟	
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
		printf("\n\n<3<3<3逐馬 逐馬 逐馬 逐馬<3<3<3\n\n");
		printf("user: %d渋 %d鳶\n",uWin,cWin);
		printf("com : %d渋 %d鳶\n",cWin,uWin);

		/*printf("軒巴傾戚研 左獣畏柔艦猿? (y/n): ");
		char q= 'n';
		scanf("%c", &q);
		if(q == 'y')replay(u_route, c_route);
		*/

		replay(u_route, c_route);
		
		printf("仙亀穿馬獣畏柔艦猿?\n 1.森 2.焼艦推(曽戟)\n:");
		scanf("%d",&re);
		if(re==1) return 0;
		if(re==2) return 1;
	}
	if(cr==2){
		system("clear");
		cWin++;
		printf("\n\n\nUser LoseTT\n");
		print_gameMaze(cGame);
		printf("com : %d渋 %d鳶\n",cWin,uWin);
		printf("user: %d渋 %d鳶\n",uWin,cWin);
		printf("\n\nばばばば仙亀穿 馬室推!ばばばば\n\n");
		/*
		printf("軒巴傾戚研 左獣畏柔艦猿? (y/n): ");
		char q = 'n';
		scanf("%c", &q);
`		if(q == 'y')replay(u_route, c_route);	
		*/
		replay(u_route, c_route);
		
		printf("仙亀穿馬獣畏柔艦猿?\n 1.森 2.焼艦推(曽戟)");
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

	if(who==0){ //紫遂切税 渡
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
		printf(" 耕稽幻級奄 匂奄 :  TAB \n");
		//system("cls");	
		}
	}

	if(who==1){ //陳濃斗税 渡
		printf("com税 衣舛\n");
		
		Sleep(500);
		printf("薄仙: %d%d -> 識澱:",comX,comY);
		if(cCheck ==1){  //背雁 牒拭辞 叔鳶廃 旋戚 赤嬢辞 戚耕 戚疑 亜管廃 牒拭 企廃 壕伸戚 幻級嬢閃 赤聖 凶
			srand( (unsigned)time(NULL));
			num = rand()%theN;
			cX=choice[num][0];
			cY=choice[num][1];
			Sleep(800);
			printf(" %d%d\n",cX,cY);
		}
		if(cCheck ==0 ){ //背雁 牒拭辞 坦製 亀穿析 凶->背雁 牒拭辞 戚疑 亜管廃 牒税 壕伸聖 坦製 幻糾(comGame敗呪)
			theN=comGame(game,comX,comY);
			srand( (unsigned)time(NULL));
			num = rand()%theN;
			cX=choice[num][0];
			cY=choice[num][1];
			Sleep(800);
			printf(" %d%d\n",cX,cY);
		}

		if(game[cX][cY] == 3){ //識澱拝 呪 赤澗 path析 凶
		if(maze[cX][cY]==1)
		{ //舛岩 耕稽人 企繕
			game[cX][cY] = 1; //識澱廃 号拭 1, 聡 path研 隔壱
			
			if(game[9][9] == 1)
				return 2;

			cCheck =0;//戚疑梅生糠稽 陥製 牒拭辞澗 戚疑亜管廃 牒税 壕伸聖 陥獣 坦製採斗 幻級嬢醤 馬糠稽

			if(who==1){ //戚疑梅生糠稽 薄仙 疎妊 郊会
				comX = cX;
				comY = cY;
			}
			change_path(game,cX,cY);
			find_previous(game,cX,cY);
			make_exceptions(game,cX,cY);
			return 1;

		} 
		else 
		{ //雌企亜 幻窮 耕稽人 堂形辞 雌企拭惟 渡戚 角嬢姶
			printf("堂携柔艦陥! 渡戚 雌企拭惟 角嬢逢艦陥!\n\n");
			
			//陳濃斗税 井酔 掻差吉 識澱聖 馬走 省惟 馬奄 是背 据掘 糎仙馬揮 壕伸拭辞 識澱梅揮 杏 ��層陥.
			if(who==1){
				cCheck =1; //戚耕 幻級嬢走 壕伸聖 紫遂背醤 馬糠稽
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
		printf("雌企拭惟 渡戚 宜焼逢艦陥.");
		gotoxy(x,y);
		return 0;
	}
	else if(game[x][y]==1){
		gotoxy(40,10);
		printf("戚耕 走蟹紳 掩脊艦陥. 陥獣識澱馬室推.");
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




///////////////////////// 		REPLAY 是廃 敗呪級 		////////////////////////////////////////


void show_maze(int maze[X][Y]) // replay研 是廃 敗呪戚陥.
{
	int i, j;

	//maze 窒径
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

// 3託据 壕伸聖 疑旋生稽 拝雁
int*** TriArr(int number, int height, int width) //湛 昔畿什澗 唖切税 渡, 砧腰属 室腰属澗 value 級嬢哀 疎妊.
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

    // 乞窮 壕伸据社 段奄鉢
    for (i = 0; i < number; i++)
        for (j = 0; j < height; j++)
            for (k = 0; k < width; k++)
                ptr[i][j][k] = '\0';

    return ptr;
}



// 疑旋生稽 拝雁 吉 五乞軒 背薦
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



//政煽 食殿猿走 渡級 3託据 壕伸拭 煽舌
int set_u_route(int route[X][Y], int z){ //z澗 戚腰戚 護腰属 渡昔走 , z=1,,2,,3... 

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
//陳濃斗 食殿猿走 渡級 3託据 壕伸拭 煽舌
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
		printf("\n政煽\t\t\t陳濃斗\n");
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
