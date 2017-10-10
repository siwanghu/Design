#define OPENMP
#include "mySort.h"

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

void OddEvenSort(List &L)
{
	int i, j;
    clock_t first,last,time;
    first=clock();
	for (i = 1; i < DATASIZE + 1; i++)
	{
		if (1 == (i % 2 ))
		{
#pragma omp parallel for
			for (j = 1; j < DATASIZE + 1; j+=2)
			{
				if (L.data[j]>L.data[j + 1])
				{
					int temp = L.data[j];
					L.data[j] = L.data[j + 1];
					L.data[j + 1] = temp;
				}
			}
		}
		else
		{
#pragma omp parallel for
			for (j = 2; j < DATASIZE; j += 2) 
			{
				if (L.data[j]>L.data[j + 1])
				{
					int temp = L.data[j];
					L.data[j] = L.data[j + 1];
					L.data[j + 1] = temp;
				}
			}
		}
	}
    last=clock();
	time=last-first;
	printf("TIME:%ld\n",time);
}

int main(int argc, char *argv[])
{
	if(argc==1)
	{
		printf("请输入数据量!");
	}
	else
	{
		List L,S;
       	DATASIZE=atol(argv[1]);
    	L.data=(keyType*)malloc(sizeof(keyType)*(DATASIZE));
		readFile(L);
        int thread=atol(argv[2]);
		omp_set_num_threads(thread); 
		OddEvenSort(L);
	}
}













