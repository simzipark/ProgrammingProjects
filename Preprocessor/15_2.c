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

int pack_char(char a, char b, char c, char d)
{
	return a << 24 | b << 16 | c << 8 | d;
}

int unpack(int v)
{

	return v >> 24;
}


int main(){

	int v;
	char c[5];
	char original[5];
	int i;
	int j=24;

	printf("문자 4개 입력 : ");
	scanf("%s", &c);

	v = pack_char(c[0], c[1], c[2], c[3]);
	print_binary(v);
		
	for(i=0; i<4; i++)
	{
		original[i] = v>>j; // 0~0 0100 0001
		j -=8;		
	}

	printf("original : %s\n", original);

	return 0;
}