/*

#include <stdio.h>
#include <stdlib.h>

long size(FILE *fp) ;

int main(){

	FILE * fpt;
	FILE * fpb;
	int i = 1234567890;
	int k,r;
	double j = 1.234567890;

	fpt = fopen("text.txt", "w");
	fpb = fopen("text.dat", "w");

	if(fpt == NULL || fpb == NULL)
	{
		printf("파일을 열 수 없습니다.\n");
		return 1;
	}

	fprintf(fpt, "%d\n", i);
	fwrite(&i, sizeof(int), 1, fpb);
	fwrite("\n", sizeof(int), 1, fpb);

	//난수 100개 저장
	for(k=0; k<100; k++)
	{
		r = rand();
		fprintf(fpt, "%d", r);
		fwrite(&r, sizeof(int), 1, fpb);
		fprintf(fpt, "\n");
		fwrite("\n", sizeof(int), 1, fpb);
	}

	printf("size of fpt : %d\n", size(fpt));
	printf("size of fpb : %d\n", size(fpb));

	fprintf(fpt, "%lf", j);
	fwrite(&j, sizeof(double), 1, fpb);

	fclose(fpt);
	fclose(fpb);

	return 0;
}
long size(FILE *fp) 
{ 

	long fcur; 
	long fsize; 

	fcur = ftell(fp); 

    fseek(fp, 0, SEEK_END); 

    fsize = ftell(fp); 

    fseek(fp, fcur, SEEK_SET); 

    return fsize; 

} 



*/