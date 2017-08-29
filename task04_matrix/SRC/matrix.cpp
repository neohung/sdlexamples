#include "matrix.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
Matrix* matrixNew(double* data, int rows, int cols)
{
	Matrix* m = (Matrix*) malloc(sizeof(Matrix));
	if (m == NULL){
		printf("%s: Error to malloc m\n",__func__);
		exit(-1);
	}
	int size = 	rows*cols;
	assert(size != 0);
	m->data = (double*) malloc(size*sizeof(double));
	if (m->data == NULL){
		printf("%s: Error to malloc m->data\n",__func__);
		exit(-1);
	}
	if (data)
		memcpy(	m->data, data, size*sizeof(double));
	else{
		memset(	m->data ,0, size*sizeof(double));
	}
    m->rows =rows;
    m->cols = cols;
	return m;
}

void matrixDelete(Matrix* m)
{
	free(m->data);
	free(m);
}

void  matrixPrint(Matrix* m)
{
	assert(m != NULL);
	printf("\n");
	int i,j;
	for(j=0;j<m->rows;j++){
		printf("[ ");
		for(i=0;i<m->cols;i++){
			printf("%2.02f ",m->data[i+j*m->cols]);
		}
		printf("]\n");
	}
	printf("\n");

}

Matrix* matrixAdd(Matrix* a,Matrix* b)
{
	assert((a->rows== b->rows) && (a->cols == b->cols));
	Matrix* m = matrixNew(NULL, a->rows, a->cols);
	int i,j;
		for(j=0;j<a->rows;j++){
			for(i=0;i<a->cols;i++){
				m->data[i+j*m->cols] = a->data[i+j*m->cols]  + b->data[i+j*m->cols];
			}
		}
	return m;
}

Matrix* matrixSub(Matrix* a,Matrix* b)
{
	assert((a->rows== b->rows) && (a->cols == b->cols));
	Matrix* m = matrixNew(NULL, a->rows, a->cols);
	int i,j;
		for(j=0;j<a->rows;j++){
			for(i=0;i<a->cols;i++){
				m->data[i+j*m->cols] = a->data[i+j*m->cols] - b->data[i+j*m->cols];
			}
		}
	return m;
}


//C=AxB
Matrix* matrixMul(Matrix* a,Matrix* b)
{
	assert(a->cols == b->rows);
	Matrix* m = matrixNew(NULL, a->rows, b->cols);
	int i,j,k;
	for(i=0;i<a->rows;i++){
		for(j=0;j<b->cols;j++){
			for(k=0;k<a->cols;k++){
				// m[i][j] = a[i][k]*b[k][j]
				//printf("a [%d,%d]: %f,", i,k,a->data[i*a->cols+k]);
				//printf("b [%d,%d]: %f,", k,j, b->data[k*b->cols+j]);
				m->data[i*m->cols+j] += a->data[i*a->cols+k] * b->data[k*b->cols+j];
				//printf("m [%d,%d]: += %f,", i,j, a->data[i*a->cols+k] * b->data[k*b->cols+j]);
			}
			//printf("\n\n");
		}
	}
	return m;
}

//A=AxB
void matrixIterMul(Matrix* a,Matrix* b)
{
	assert(a->cols == b->rows);
	assert(b->cols == b->rows);
	Matrix* m = matrixNew(NULL, a->rows, b->cols);
	int i,j,k;
	for(i=0;i<a->rows;i++){
		for(j=0;j<b->cols;j++){
			for(k=0;k<a->cols;k++){
				m->data[i*m->cols+j] += a->data[i*a->cols+k] * b->data[k*b->cols+j];
			}
		}
	}
	for(i=0;i<a->rows;i++){
		for(j=0;j<b->cols;j++){
			 a->data[i*a->cols+j] = m->data[i*m->cols+j];
		}
	}
	matrixDelete(m);
}

Matrix* matrixScale(Matrix* a,double s)
{
	assert(a->rows * a->cols);
	Matrix* m = matrixNew(NULL, a->rows, a->cols);
	int i,j;
	for(j=0;j<a->rows;j++){
		for(i=0;i<a->cols;i++){
			m->data[i+j*m->cols] = a->data[i+j*m->cols] * s;
		}
	}
	return m;
}

void matrixIterScale(Matrix* a, double s)
{
	assert(a->rows * a->cols);
	int i,j;
	for(j=0;j<a->rows;j++){
		for(i=0;i<a->cols;i++){
			a->data[i+j*a->cols] *= s;
		}
	}
}
