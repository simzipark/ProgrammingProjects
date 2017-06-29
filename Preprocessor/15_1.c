#include <stdio.h>

void print_binary(int v)
{
	int i;
	int c=0;

	
	for(i=0; i<32; i++)
	{			
		if( (v << i) & 0x80000000 )
			printf("1");
		else
			printf("0");	
		c++;

		//4비트마다 스페이스 출력
		if(c%4==0)
			printf(" ");
	}
	
	printf("\n");

}

int main(){

	int x, y;

	printf("input x : ");
	scanf("%d", &x);
	printf("input y : ");
	scanf("%d", &y);

	printf("첫번째 정수 : ");
	print_binary(x);
	printf("두번째 정수 : ");
	print_binary(y);

	printf("논리곱 : "); print_binary(x&y);
	printf("논리합 : "); print_binary(x|y);
	printf("배타적 논리합 : "); print_binary(x^y);
	printf("첫번째 비트 부정연산 : "); print_binary(~x);

	return 0;

}