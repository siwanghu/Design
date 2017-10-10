#pragma once
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#ifdef MYMPI
#include <mpi.h>
#endif

#ifdef OPENMP
#include <omp.h>
#endif

long DATASIZE;
#define TIMESIZE 1000000

typedef int keyType;

typedef struct
{
	keyType *data;
	int length;
}List;
