#define MYMPI
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

int Max(List &L)       
{
	int max = L.data[0];
	for (int i = 0; i < DATASIZE; i++)
	{
		if (L.data[i] > max)
		{
			max = L.data[i];
		}
	}
	return max;
}



void MaxParallel(int argc,char * argv[],List &L, keyType &max)  
{
	max = L.data[0];
	MPI_Status status;
	int rank, size;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	if (size == 1)
	{
		double  _first, _last;
		double _time;
		_first = MPI_Wtime();	
		max = Max(L);
		_last = MPI_Wtime();
		_time=(_last-_first)*TIMESIZE;
		printf("MAX:%d   TIME:%lf\n", max, _time);
	}
	else
	{
		if (rank == 0)
		{
			double  first, last;
			double time;	
			first = MPI_Wtime();
			int dest, start, end, length, source, i,temp;
			start = 0;
			length = DATASIZE / size;
			end = start + length;
			for (dest = 1; dest < size; dest++)
			{
				MPI_Send(&start, 1, MPI_INT, dest, 1, MPI_COMM_WORLD);
				if(start+length<DATASIZE)
					MPI_Send(&end, 1, MPI_INT, dest, 1, MPI_COMM_WORLD);
				else
					MPI_Send(&L.length, 1, MPI_INT, dest, 1, MPI_COMM_WORLD);
				start += length;
				end += length;
			}
			for (i = start; i < end; i++)
			{
				if (L.data[i] > max)
					max = L.data[i];
			}
			for (i = 1; i < size; i++)
			{
				source = i;
				MPI_Recv(&temp, 1, MPI_INT, source, 2, MPI_COMM_WORLD, &status);
				if (max < temp)
					max = temp;
			}
			last = MPI_Wtime();
			time=last-first;
			printf("MAX:%d   TIME:%lf\n", max, time*TIMESIZE);
		}
		if (rank > 0)
		{
			int start, end, length, dest,source, i, temp;
			source = 0;
			dest = 0;
			MPI_Recv(&start, 1, MPI_INT, source, 1, MPI_COMM_WORLD, &status);
			MPI_Recv(&end, 1, MPI_INT, source, 1, MPI_COMM_WORLD, &status);
			for (i = start; i < end; i++)
			{
				temp = L.data[i];
				if (L.data[i] > temp)
					temp = L.data[i];
			}
			MPI_Send(&temp, 1, MPI_INT, dest, 2, MPI_COMM_WORLD);
		}
		MPI_Finalize();
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
		keyType max;
		List L;
		DATASIZE=atol(argv[1]);
		L.data=(keyType*)malloc(sizeof(keyType)*DATASIZE);
		readFile(L);
		MaxParallel(argc, argv, L, max);
	}
}





