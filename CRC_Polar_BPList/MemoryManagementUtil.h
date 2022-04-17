#ifndef MemoryManagementUtil_H
#define MemoryManagementUtil_H

int* createIntegerArray(int m);
void deleteIntegerArray(int* array);
int** createIntegerArray(int m, int n);
void deleteIntegerArray(int** array, int m);
double* createDoubleArray(int m);
void deleteDoubleArray(double* array);
double** createDoubleArray(int m, int n);
void deleteDoubleArray(double** array, int m);
double*** createDoubleArray(int m, int n, int h);
void deleteDoubleArray(double*** array, int m, int n);
void* createVoidArray(int size, int m);
void deleteVoidArray(void* array);
#endif