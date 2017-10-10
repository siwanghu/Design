#include <stdio.h>
#include <stdlib.h>
#include <time.h>

long DATASIZE;

typedef int keyType;

typedef struct
{
	keyType *data;
	int length;
}List;

void Print(List &L)  
{
	for (int i = 0; i < DATASIZE; i++)
	{
		printf("%d\n", L.data[i]);
	}
}

bool IsRepetition(List &L, int Length, keyType param) 
{
	int i;
	for (i = 0; i < Length; i++)
	{
		if (L.data[i] == param)
			return true;
	}
	return false;
}

void Random(List &L)    
{
	srand((unsigned)time(NULL));
	int i = 0;
	while (i < DATASIZE)
	{
		int temp = rand();
		L.data[i] = temp;
		i++;
	}
	L.length = DATASIZE;
}


void printFile(List &L)
{
	FILE *file = fopen("data.txt", "w");
	if (file==NULL) 
	{
        printf("打开文件失败!");
	}
	for(int i=0;i<DATASIZE;i++)
	{
		fprintf(file,"%d ",L.data[i]);
	}
	fclose(file);
}

int main(int argc,char *argv[])
{
    if(argc==1)
    {
        printf("请输入要产生的随机数个数！\n");
    }
    else
    {
        List L;
        DATASIZE=atol(argv[1]);
        L.data=(keyType*)malloc(sizeof(keyType)*DATASIZE);
        Random(L);
        printFile(L);
    }
}