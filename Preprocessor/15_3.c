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
	//a부터 unsigned int의 LSB부터 만들어진다.

	bf.a = 0xff;
	bf.b = 0;
	bf.c = 1; //10을 대입하면 절삭오차가 발생한다
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
	// a~f가 제일 낮은 주소에 할당되고, 그 다음으로 g, h 가 할당된다.
	// a~f(0) -> g(8) -> h(16) 이런식으로

}