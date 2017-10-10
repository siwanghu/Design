#define MYMPI
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

int Partition(List &L, int start, int end)  
{   
	int i, j, value, temp;
	value = L.data[end];
	i = start - 1;
	for (j = start; j<end; j++)
		if (L.data[j] <= value)
		{
			i++;
			temp = L.data[i];
			L.data[i] = L.data[j];
			L.data[j] = temp;
		}

	temp = L.data[i + 1];
	L.data[i + 1] = L.data[end];
	L.data[end] = temp;	
	return i + 1;
}


void QuickSort(List &L, int start, int end)   
{
	int value;
	if (start < end)
	{
		value = Partition(L, start, end);
		QuickSort(L, start, value - 1);
		QuickSort(L, value + 1, end);
	}
}

void QuickSortParallel(int argc, char *argv[],List &L) 
{
	int rank, size;
	MPI_Status status;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	if(size==1)
	{
		double first,last,time;
		first=MPI_Wtime();
		QuickSort(L,0,DATASIZE);
		last=MPI_Wtime();
		time=last-first;
		printf("TIME:%lf\n",time*TIMESIZE);
	}
	else
	{
		if(rank==0)
		{
			double first,last,time;
			first=MPI_Wtime();
			int start,end,dest,flag;
			start=0;
			end=DATASIZE/size;
			for(dest=1;dest<size;dest++)
			{
				MPI_Send(&start, 1, MPI_INT, dest, 1, MPI_COMM_WORLD);
				MPI_Send(&end, 1, MPI_INT, dest, 1, MPI_COMM_WORLD);
				start=start+end;
			}
			QuickSort(L,start,DATASIZE);
			for(dest=1;dest<size;dest++)
			{
				MPI_Recv(&flag, 1, MPI_INT, dest, 2, MPI_COMM_WORLD, &status);
			}
			QuickSort(L,0,DATASIZE);
			last=MPI_Wtime();
			time=last-first;
			printf("TIME:%lf\n",time*TIMESIZE);
		}
		if(rank>0)
		{
			int start,end,source;
			source=0;
			MPI_Recv(&start, 1, MPI_INT, source, 1, MPI_COMM_WORLD, &status);
			MPI_Recv(&end, 1, MPI_INT, source, 1, MPI_COMM_WORLD, &status);
			QuickSort(L,start,end);
			MPI_Send(&source, 1, MPI_INT, source, 2, MPI_COMM_WORLD);
		}
		MPI_Finalize();
	}
}

void EnumSort(List &L,List &S)
{
    #pragma omp parallel private(i,j,k) 
    {
        #pragma omp for
	    for(int i=0;i<DATASIZE;i++)
	    {
		    int k=0,data=L.data[i];
		    for(int j=0;j<DATASIZE;j++)
		    {
			    if(data>L.data[j])
			    {
				    k++;
			    }
		    }
		    S.data[k]=data;
	    }
    }
}


void EnumSortParallel(int argc, char *argv[],List &L,List &S)
{
	int rank, size;
	MPI_Status status;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	if(size==1)
	{
		double first,last,time;
		first=MPI_Wtime();
		EnumSort(L,S);
		last=MPI_Wtime();
		time=last-first;
		printf("TIME:%lf\n",time*TIMESIZE);
	}
	else
	{
		if(rank==0)
		{
			double first,last,time,flag;
			first=MPI_Wtime();
			int start,end,dest;
			start=0;
			end=DATASIZE/size;
			for(dest=1;dest<size;dest++)
			{
				MPI_Send(&start, 1, MPI_INT, dest, 1, MPI_COMM_WORLD);
				MPI_Send(&end, 1, MPI_INT, dest, 1, MPI_COMM_WORLD);
				start=start+end;
			}
            #pragma omp parallel private(i,j,k) 
            {
                #pragma omp for
			    for(int i=start;i<DATASIZE;i++)
			    {
				    int k=0,data=L.data[i];
				    for(int j=0;j<DATASIZE;j++)
				    {
					    if(data>L.data[j])
					    {
						    k++;
					    }
				    }
				    S.data[k]=data;
			    }
            }
			for(dest=1;dest<size;dest++)
			{
				MPI_Recv(&flag, 1, MPI_INT, dest, 2, MPI_COMM_WORLD, &status);
			}
			last=MPI_Wtime();
			time=last-first;
			printf("TIME:%lf\n",time*TIMESIZE);
		}
		if(rank>0)
		{
			int start,end,source;
			source=0;
			MPI_Recv(&start, 1, MPI_INT, source, 1, MPI_COMM_WORLD, &status);
			MPI_Recv(&end, 1, MPI_INT, source, 1, MPI_COMM_WORLD, &status);
            #pragma omp parallel private(i,j,k) 
            {
                #pragma omp for
			    for(int i=start;i<end;i++)
			    {
				    int k=0,data=L.data[i];
				    for(int j=0;j<DATASIZE;j++)
				    {
					    if(data>L.data[j])
					    {
						    k++;
					    }
				    }
				    S.data[k]=data;
			    }
            }
			MPI_Send(&source, 1, MPI_INT, source, 2, MPI_COMM_WORLD);
		}
		MPI_Finalize();
	}
}


int main(int argc, char *argv[])
{
	if(argc==1)
	{
		printf("请输入数据量!");
	}
	else
	{
        if(argc==2)
	    {
		    printf("请输入线程数!\n");
	    }
	    else
        {
		    List L,S;
       	    DATASIZE=atol(argv[1]);
    	    L.data=(keyType*)malloc(sizeof(keyType)*(DATASIZE));
		    S.data=(keyType*)malloc(sizeof(keyType)*(DATASIZE));
		    readFile(L);
            int thread=atol(argv[2]);
            omp_set_num_threads(thread); 
		    EnumSortParallel(argc,argv,L,S);
        }
	}
}













