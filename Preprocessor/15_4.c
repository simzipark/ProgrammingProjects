
#include <stdio.h>

#define SQUARE1(x) (x*x)
#define SQUARE2(x) (x)*(x)
#define SQUARE3(x) ((x)*(x))

#define DEBUG 1

int main(){

	printf("3�� ���� = %d\n", SQUARE1(1+2)); //��� 5
	printf("3�� ���� = %d\n", SQUARE2(1+2)); //��� 9
	printf("3�� ���� = %d\n\n", SQUARE3(1+2)); //��� 9

	printf("SQUARE1(2),8 : %d\n",SQUARE1(2),8); //��� 4
	printf("SQUARE2(2),8 : %d\n",SQUARE2(2),8); //��� 4
	printf("SQUARE3(2) : %d\n\n",SQUARE3(2)); //��� 4

	printf("SQUARE1(1+2),9 : %d\n",SQUARE1(1+2),9); //��� 5
	printf("SQUARE2(1+2),9 : %d\n",SQUARE2(1+2),9); //��� 9
	printf("SQUARE3(1+2),9 : %d\n\n",SQUARE3(1+2),9); //��� 9

	//SQURE()�� ������ �� �� ���� �����ݷ��� ���̸� ������ ����

	if(DEBUG == 1)
	{
		printf("__DATE__ : %s\n", __DATE__);
		printf("__TIME__ : %s\n", __TIME__);
		printf("__LINE__ : %d\n", __LINE__);
		printf("__FILE__ : %s\n\n", __FILE__);
	}

	return 0;
}