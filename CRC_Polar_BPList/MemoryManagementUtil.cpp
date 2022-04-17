#include "MemoryManagementUtil.h"
#include<cstdlib>

int* createIntegerArray(int m)
{
	int* array = new int[m];
	return array;
}

void deleteIntegerArray(int* array)
{
	delete[] array;
}

int** createIntegerArray(int m, int n)
{
	int** array = new int *[m];
	for( int i=0; i<m; i++ )
	{
		array[i] = new int [n]  ;
	}
	return array;
}

void deleteIntegerArray(int** array, int m)
{
	for( int i=0; i<m; i++ )
	{
		delete [] array[i];
	}
	delete[] array;
}

double* createDoubleArray(int m)
{
	double* array = new double[m];
	return array;
}

void deleteDoubleArray(double* array)
{
	delete[] array;
}

double** createDoubleArray(int m, int n)
{
	double** array = new double *[m];
	for( int i=0; i<m; i++ )
	{
		array[i] = new double[n]  ;
	}
	return array;
}

void deleteDoubleArray(double** array, int m)
{
	for( int i=0; i<m; i++ )
	{
		delete[] array[i];
	}
	delete[] array;
}

double*** createDoubleArray(int m, int n, int h)
{
	double*** array = new double **[m];
	for( int i=0; i<m; i++ )
	{
		array[i] = new double *[n];
		for( int j=0; j<n; j++ )
		{
			array[i][j] = new double [h];
		}
	}
	return array;
}

void deleteDoubleArray(double*** array, int m, int n)
{
	for( int i=0; i<m; i++ )
	{
		for( int j=0; j<n; j++ )
		{
			delete[] array[i][j];
		}
		delete[] array[i];
	}
	delete[] array;
}

void* createVoidArray(int size, int m)
{
	void*  array;
	array = malloc( size * m );
	return array;
}

void deleteVoidArray(void* array)
{
	delete[] array;
}