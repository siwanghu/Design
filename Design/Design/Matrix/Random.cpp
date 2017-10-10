#include <stdio.h>
#include <stdlib.h>
#include <time.h>

long DATASIZE;

long WIDTH_M;
long HIGH_M;

long WIDTH_N;
long HIGH_N;


typedef int keyType;


typedef struct
{
	keyType **data;
	int size;
}Matrix_M;


typedef struct
{
	keyType **data;
	int size;
}Matrix_N;


void Print(Matrix_M &M,Matrix_N &N)       
{
	for (int i = 0; i < WIDTH_M; i++)
	{
		for (int j = 0; j < HIGH_M; j++)
		{
			printf("%d  ", M.data[i][j]);
		}
		printf("\n");
	}
	printf("-------------------------------------------------\n");
	for (int i = 0; i < WIDTH_N; i++)
	{
		for (int j = 0; j < HIGH_N; j++)
		{
			printf("%d  ", N.data[i][j]);
		}
		printf("\n");
	}
	printf("-------------------------------------------------\n");
}


void createMatrix(Matrix_M &M,Matrix_N &N)    
{
	srand((unsigned)time(NULL));
	for (int i = 0; i < WIDTH_M; i++)
	{
		for (int j = 0; j < HIGH_M; j++)
		{
			M.data[i][j] = rand() % 100;
		}
	}
	M.size = WIDTH_M*HIGH_N;

	srand((unsigned)time(NULL));
	for (int i = 0; i < WIDTH_N; i++)
	{
		for (int j = 0; j < HIGH_N; j++)
		{
			N.data[i][j] = rand() % 100;
		}
	}
	N.size = WIDTH_M*HIGH_N;
}

void printFile(Matrix_M &M,Matrix_N &N)
{
	FILE *file = fopen("data.txt", "w");
	if (file==NULL) 
	{
        printf("打开文件失败!");
	}
	for(int i=0;i<DATASIZE;i++)
	{
		for(int j=0;j<DATASIZE;j++)
		{
			fprintf(file,"%d ",M.data[i][j]);
		}
	}
	for(int i=0;i<DATASIZE;i++)
	{
		for(int j=0;j<DATASIZE;j++)
		{
			fprintf(file,"%d ",N.data[i][j]);
		}
	}
	fclose(file);
}


int main(int argc,char *argv[])
{
	if(argc==1)
	{
		printf("请输入数据量!\n");
	}
	else
	{
		Matrix_M M;
		Matrix_N N;
		DATASIZE=atol(argv[1]);
		WIDTH_M=DATASIZE;
		HIGH_M=DATASIZE;
		WIDTH_N=DATASIZE;
		HIGH_N=DATASIZE;

		M.data = (keyType **)malloc(sizeof(keyType *) * DATASIZE);
		for (int i = 0; i < DATASIZE; i++)
		{
			*(M.data + i) = (keyType *)malloc(sizeof(keyType) * DATASIZE);
		}
		N.data = (keyType **)malloc(sizeof(keyType *) * DATASIZE);
		for (int i = 0; i < DATASIZE; i++)
		{
			*(N.data + i) = (keyType *)malloc(sizeof(keyType) * DATASIZE);
		}
		createMatrix(M,N);
		printFile(M,N);
	}
}





























