#include <stdio.h>
#include <malloc.h>
#include <string.h>

struct NODE
{
	char data[10];
	struct NODE * link;
};
void insert_node(struct NODE * pre, struct NODE * next)
{
	struct NODE * temp;
	struct NODE * ptr = (struct NODE*)malloc(sizeof(struct NODE));

	temp = pre->link;

	pre->link = ptr;

	ptr->link = temp;

	strcpy(ptr->data, "sat");

}
int delet_node(struct NODE * p, struct NODE * first)
{
	struct NODE * pre;

	pre = first;	

	while(1)
	{
		if(strcmp(p->data, "cat") && p == first){
			first = p->link;
			return 1;
		}

		else if(strcmp(p->data, "cat") && p->link == NULL){
			pre->link = NULL;
			return 1;
		}
		else if(strcmp(p->data, "cat")){
			pre->link = p->link;
			return 1;
		}
		
			pre = p;
			p = p->link;	
		
	}


}



int main(){

	struct NODE * first, *p;
	

	p = (struct NODE*)malloc(sizeof(struct NODE));
	if(p == NULL)
	{
		printf("�����޸� �Ҵ� ����\n");
		return 1;	
	}
	strcpy(p->data, "cat");
	p->link = NULL;
	first = p;

	//�� ��° ��� ����
	p = (struct NODE*)malloc(sizeof(struct NODE));
	if(p == NULL)
	{
		printf("�����޸� �Ҵ� ����\n");
		return 1;	
	}
	strcpy(p->data, "bat");
	p->link = NULL;
	first->link = p;	

	//��� ����
	p = first;
	insert_node(p, p->link);
	//��� ���1
	p = first;
	while(p != NULL)
	{
		printf("%s\n", p->data);
		p = p->link;
	}
	printf("----------------------------\n");

	//��� ����
	p = first;
	delet_node(p, first);
	//��� ���2
	p = first;
	while(p != NULL)
	{
		printf("%s\n", p->data);
		p = p->link;
	}
	printf("----------------------------\n");

	free(p);
	return 0;
}
