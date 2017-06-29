#include <stdio.h>

struct test{
	unsigned a:8;
	unsigned b:1;
	unsigned c:3;
	unsigned d:4;
	unsigned e:12;
	unsigned f:4;
	double g;
	int h;
};
// 0100 0000 0000 1100 1111 001/0 1111 1111 (a~f)

int main(){

	struct test bf;
	//a���� unsigned int�� LSB���� ���������.

	bf.a = 0xff;
	bf.b = 0;
	bf.c = 1; //10�� �����ϸ� ��������� �߻��Ѵ�
	bf.d = 0xf;
	bf.e = 3;
	bf.f = 4;
	bf.g = 4.5;
	bf.h = 300;

	printf("bf = %x\n", bf);
	printf("&bf   = %u\n", &bf);
	printf("&bf.g = %u\n", &bf.g);
	printf("&bf.h = %u\n", &bf.h);
	printf("sizeof(bf) : %d\n", sizeof(bf));
	// a~f�� ���� ���� �ּҿ� �Ҵ�ǰ�, �� �������� g, h �� �Ҵ�ȴ�.
	// a~f(0) -> g(8) -> h(16) �̷�������

}