#define MYMPI
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

void matrixMulti(Matrix_Result &result,Matrix_M &M,Matrix_N &N) 
{
	if (HIGH_M != WIDTH_N)
		return;
	else
	{
		int i,j,k;
        #pragma omp parallel shared(result,M,N)  private(i,j,k)
        {
            #pragma omp for schedule(dynamic) 
		    for (i = 0; i < WIDTH_M; i++)
		    {
			    for (j = 0; j < HIGH_N; j++)
			    {
				    int sum = 0;
				    for (k = 0; k < WIDTH_N; k++)
				    {
					    sum += (M.data[i][k]*N.data[k][j]);
				    }
				    result.data[i][j] = sum;
			    }
		    }
        }
	}
}

void matrixMultiParallel(int argc,char * argv[],Matrix_Result &result, Matrix_M &M, Matrix_N &N)
{
	int rank,size;
	MPI_Status status;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	if(size==1)
	{
		double first,last,time;
		first=MPI_Wtime();
		matrixMulti(result, M, N);
		last=MPI_Wtime();
		time=last-first;
		printf("TIME:%lf\n",time*TIMESIZE);
	}
	else
	{
		if(rank==0)
		{
			int dest,source,row,offset,flag;
			row = WIDTH_M / size;
			offset=0;
			source=0;
			double first,last,time;
			first=MPI_Wtime();
			for (dest = 1; dest < size; dest++)
			{
				MPI_Send(&offset, 1, MPI_INT, dest, 1, MPI_COMM_WORLD);
				MPI_Send(&row, 1, MPI_INT, dest, 1, MPI_COMM_WORLD);
				offset = offset + row;
			}
            #pragma omp parallel shared(result,M,N) 
    	    {  
                #pragma omp for schedule(dynamic)  
			    for (int i = offset; i < WIDTH_M; i++)
			    {
				    for (int j = 0; j < HIGH_N; j++)
				    {
					    int sum = 0;
					    for (int k = 0; k < WIDTH_N; k++)
					    {
						    sum += (M.data[i][k]*N.data[k][j]);
					    }
					    result.data[i][j] = sum;
				    }
			    }
            }
			for(dest = 1;dest < size; dest++)
			{
				MPI_Recv(&flag, 1, MPI_INT, dest, 2, MPI_COMM_WORLD, &status);
			}
			last=MPI_Wtime();
			time=last-first;
			printf("TIME:%lf\n",time*TIMESIZE);
		}
		if(rank>0)
		{
			int dest,source,row,offset;
			source=0;
			MPI_Recv(&offset, 1, MPI_INT, source, 1, MPI_COMM_WORLD, &status);
			MPI_Recv(&row, 1, MPI_INT, source, 1, MPI_COMM_WORLD, &status);
            #pragma omp parallel shared(result,M,N) 
    	    {  
                #pragma omp for schedule(dynamic) 
			    for (int i = offset; i < (offset+row); i++)
			    {
				    for (int j = 0; j < HIGH_N; j++)
				    {
					    int sum = 0;
					    for (int k = 0; k < WIDTH_N; k++)
					    {
						    sum += (M.data[i][k]*N.data[k][j]);
					    }
					    result.data[i][j] = sum;
				    }
			    }
            }
			MPI_Send(&source, 1, MPI_INT, source, 2, MPI_COMM_WORLD);
		}
		MPI_Finalize();
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
		if(argc==2)
	    {
		    printf("请输入线程数!\n");
	    }
	    else
        {
		    Matrix_M M;
		    Matrix_N N;
		    Matrix_Result R;
		    createMatrix(argv,R,M,N);
		    readFile(M,N);
            int thread=atol(argv[2]);
            omp_set_num_threads(thread); 
		    matrixMultiParallel(argc,argv,R, M, N);
        }
	}
}
