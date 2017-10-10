#!/bin/bash

Matrix="Matrix"
Max="Max"
Sort="Sort"
pattern=$1
processNumber=$2
threadNumber=$3
dataNumber=$4

if [ $pattern == $Matrix ]
then
    rm -f ./result.txt
    touch result.txt
    cd ..
    cd Matrix
    mpic++ Matrix.cpp -o Matrix
    g++ -fopenmp Omp_Matrix.cpp -o Omp_Matrix
    mpic++ -fopenmp -o Omp_MPI_Matrix Omp_MPI_Matrix.cpp
    g++ Random.cpp -o Random
    ./Random $dataNumber
    echo "MPI Program" >> ../UI/result.txt
	mpirun -np $processNumber ./Matrix $dataNumber >> ../UI/result.txt
	echo "openMP Program" >> ../UI/result.txt
	./Omp_Matrix $dataNumber $threadNumber >> ../UI/result.txt
	echo "openMP and MPI Program" >> ../UI/result.txt
	mpirun -np $processNumber ./Omp_MPI_Matrix $dataNumber $threadNumber >> ../UI/result.txt
fi

if [ $pattern == $Max ]
then
    rm -f ./result.txt
    touch result.txt
    cd ..
    cd Max
    mpic++ Max.cpp -o Max
    g++  -fopenmp Omp_Max.cpp -o Omp_Max
    mpic++ -fopenmp -o Omp_MPI_Max Omp_MPI_Max.cpp
    g++ Random.cpp -o Random
    ./Random $dataNumber
    echo "MPI Program" >> ../UI/result.txt
	mpirun -np $processNumber ./Max $dataNumber>> ../UI/result.txt
	echo "openMP Program" >> ../UI/result.txt
	./Omp_Max $dataNumber $threadNumber >> ../UI/result.txt
	echo "openMP and MPI Program" >> ../UI/result.txt
	mpirun -np $processNumber ./Omp_MPI_Max $dataNumber $threadNumber >> ../UI/result.txt
fi

if [ $pattern == $Sort ]
then
    rm -f ./result.txt
    touch result.txt
    cd ..
    cd Sort
    mpic++ Sort.cpp -o Sort
    g++ -fopenmp Omp_Sort.cpp -o Omp_Sort
    mpic++ -fopenmp -o Omp_MPI_Sort Omp_MPI_Sort.cpp
    g++ Random.cpp -o Random
    ./Random $dataNumber
    echo "MPI Program" >> ../UI/result.txt
	mpirun -np $processNumber ./Sort $dataNumber >> ../UI/result.txt
	echo "openMP Program" >> ../UI/result.txt
	./Omp_Sort $dataNumber $threadNumber >> ../UI/result.txt
	echo "openMP and MPI Program" >> ../UI/result.txt
	mpirun -np $processNumber ./Omp_MPI_Sort $dataNumber $threadNumber >> ../UI/result.txt
fi

