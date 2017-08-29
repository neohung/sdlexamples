#ifndef _MATRIX_H_
#define _MATRIX_H_

typedef struct _MATRIX{
	double* data;
	int rows;
	int cols;
} Matrix;

Matrix* matrixNew(double* data, int rows, int cols);
void matrixDelete(Matrix* m);
void matrixPrint(Matrix* m);

Matrix* matrixAdd(Matrix* a,Matrix* b);
Matrix* matrixSub(Matrix* a,Matrix* b);

Matrix* matrixMul(Matrix* a,Matrix* b);
void matrixIterMul(Matrix* a,Matrix* b);

Matrix* matrixScale(Matrix* a, double s);
void matrixIterScale(Matrix* a, double s);

#endif
