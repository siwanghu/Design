#define OPENMP
#include "myMax.h"

void Print(List &L)  
{
	for (int i = 0; i < DATASIZE; i++)
	{
		printf("%d\n", L.data[i]);
	}
}

void readFile(List &L)
{
	FILE *file = fopen("data.txt", "r");
	if (file==NULL) 
	{
        printf("打开文件失败!");
	}
	for(int i=0;i<DATASIZE;i++)
	{
		fscanf(file,"%d ",&(L.data[i]));
	}
	fclose(file);
}

void OmpMaxParallel(List &L,keyType &max)
{
    clock_t first,last,time;
    first=clock();
    max = L.data[0];
    #pragma omp parallel for
	for (int i = 0; i < DATASIZE; i++)
	{
		if (L.data[i] > max)
		{
			max = L.data[i];
		}
	}
    last=clock();
	time=last-first;
    printf("MAX:%d   TIME:%ld\n", max, time);
}

int main(int argc,char * argv[])
{
	if(argc==1)
	{
		printf("请输入数据量!\n");
	}
	else
	{
		keyType max;
		List L;
		DATASIZE=atol(argv[1]);
		L.data=(keyType*)malloc(sizeof(keyType)*DATASIZE);
		readFile(L);
        int thread=atol(argv[2]);
		omp_set_num_threads(thread); 
		OmpMaxParallel(L, max);
	}
}





