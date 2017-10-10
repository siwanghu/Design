#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#ifdef MYMPI
#include <mpi.h>
#endif

#ifdef OPENMP
#include <omp.h>
#endif

long DATASIZE;

long WIDTH_M;
long HIGH_M;

long WIDTH_N;
long HIGH_N;

#define TIMESIZE 1000000

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

typedef struct
{
	keyType **data;
	int size;
}Matrix_Result;
