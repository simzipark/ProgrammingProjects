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

		//4��Ʈ���� �����̽� ���
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

	printf("ù��° ���� : ");
	print_binary(x);
	printf("�ι�° ���� : ");
	print_binary(y);

	printf("���� : "); print_binary(x&y);
	printf("���� : "); print_binary(x|y);
	printf("��Ÿ�� ���� : "); print_binary(x^y);
	printf("ù��° ��Ʈ �������� : "); print_binary(~x);

	return 0;

}