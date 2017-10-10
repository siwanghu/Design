#define OPENMP
#include "myMatrix.h"

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

void readFile(Matrix_M &M,Matrix_N &N)
{
	FILE *file = fopen("data.txt", "r");
	if (file==NULL) 
	{
        printf("打开文件失败!");
	}
	for(int i=0;i<DATASIZE;i++)
	{
		for(int j=0;j<DATASIZE;j++)
		{
			fscanf(file,"%d ",&(M.data[i][j]));
		}
	}
	for(int i=0;i<DATASIZE;i++)
	{
		for(int j=0;j<DATASIZE;j++)
		{
			fscanf(file,"%d ",&(N.data[i][j]));
		}
	}
	fclose(file);
}

void matrixMultiOmpParallel(int argc,char * argv[],Matrix_Result &result, Matrix_M &M, Matrix_N &N)
{
	if(argc==2)
	{
		printf("请输入线程数!\n");
	}
	else
	{
		clock_t first,last,time;
		int thread=atol(argv[2]),i,j,k;
		omp_set_num_threads(thread); 
		first=clock();
    	#pragma omp parallel shared(result,M,N) private(i,j,k)  
    	{  
        	#pragma omp for schedule(dynamic)  
        	for(i=0;i<DATASIZE;i++)  
       		{	  
            	for(j=0;j<DATASIZE;j++)  
            	{  
                	result.data[i][j]=0;  
                	for(k=0;k<DATASIZE;k++)  
                	{  
                    	result.data[i][j]+=M.data[i][k]*N.data[k][j];  
                	}  
            	}  
        	}  
    	}  
		last=clock();
		time=last-first;
		printf("TIME:%ld\n",time);
	}
}

void createMatrix(char * argv[],Matrix_Result &R, Matrix_M &M, Matrix_N &N)
{
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
	R.data = (keyType **)malloc(sizeof(keyType *) * DATASIZE);
	for (int i = 0; i < DATASIZE; i++)
	{
		*(R.data + i) = (keyType *)malloc(sizeof(keyType) * DATASIZE);
	}
}

int main(int argc,char * argv[])
{
	if(argc==1)
	{
		printf("请输入数据量!\n");
	}
	else
	{
		
		Matrix_M M;
		Matrix_N N;
		Matrix_Result R;
		createMatrix(argv,R,M,N);
		readFile(M,N);
		matrixMultiOmpParallel(argc,argv,R, M, N);
	}
}
