#include <stdio.h>
#include <stdlib.h>

#define S 2048
#define N S
#define M S
#define K S

#define MIN(X,Y) ((X)<(Y)?(X):(Y))
#define MAX(X,Y) ((X)>(Y)?(X):(Y))

#define TYPE double
#define MATRIX TYPE**

MATRIX createMatrixDense(unsigned x, unsigned y) {
	TYPE* data = malloc(x * y * sizeof(TYPE));
	TYPE** index = malloc(x * sizeof(TYPE*));
	index[0] = data;
	for (unsigned i = 1; i < x; ++i) {
		index[i] = &(data[i*y]);
	}
	return index;
}

MATRIX createMatrixPadded(unsigned x, unsigned y) {

	size_t y_elems = y;
	if(y_elems % 64 == 0) y_elems += 3;

	TYPE* data = malloc(x * y_elems * sizeof(TYPE));
	TYPE** index = malloc(x * sizeof(TYPE*));
	index[0] = data;
	for (unsigned i = 1; i < x; ++i) {
		index[i] = &(data[i*y_elems]);
	}
	return index;
}

#define createMatrix(_X,_Y) createMatrixPadded(_X,_Y)

void freeMatrix(MATRIX matrix) {
	free(matrix[0]);
	free(matrix);
}

void matrixMultiplyNaive(MATRIX A, MATRIX B, MATRIX C) {
	for (int i=0; i<N; i++) {
		for (int j=0; j<K; j++) {
			TYPE sum = 0;
			for (int k=0; k<M; k++) {
				sum += A[i][k] * B[k][j];
			}
			C[i][j] = sum;
		}
	}
}

#define min(_a,_b) (_a < _b ? _a : _b)

void matrixMultiplyTiled(MATRIX A, MATRIX B, MATRIX C) {
    int tileSize = 32;
    for (int i = 0; i < N; i += tileSize) {
        for (int j = 0; j < K; j += tileSize) {
            for (int k = 0; k < M; k += tileSize) {
                for (int ii = i; ii < min(i + tileSize, N); ii++) {
                    for (int jj = j; jj < min(j + tileSize, K); jj++) {
                        TYPE sum = 0;
                        for (int kk = k; kk < min(k + tileSize, M); kk++) {
                            sum += A[ii][kk] * B[kk][jj];
                        }
                        C[ii][jj] += sum;
                    }
                }
            }
        }
    }
}

void matrixMultiplyTiledVectorized(MATRIX A, MATRIX B, MATRIX C) {
    int tileSize = 64;
    for (int i = 0; i < N; i += tileSize) {
        for (int j = 0; j < K; j += tileSize) {
            for (int k = 0; k < M; k += tileSize) {
                for (int ii = i; ii < min(i + tileSize, N); ii++) {
                    for (int jj = j; jj < min(j + tileSize, K); jj++) {
                        TYPE sum = 0;
						#pragma nounroll
                        for (int kk = k; kk < min(k + tileSize, M); kk++) {
                            sum += A[ii][kk] * B[kk][jj];
                        }
                        C[ii][jj] += sum;
                    }
                }
            }
        }
    }
}

#define matrixMultiply(_A,_B,_C) matrixMultiplyTiledVectorized(_A,_B,_C)

int main(void) {

	// create the matrices
	MATRIX A = createMatrix(N, M);
	MATRIX B = createMatrix(M, K);
	MATRIX C = createMatrix(N, K);

	// initialize the matrices

	// A contains real values
	for (int i=0; i<N; i++) {
		for (int j=0; j<M; j++) {
			A[i][j] = i*j;
		}
	}

	// B is the identity matrix
	for (int i=0; i<M; i++) {
		for (int j=0; j<K; j++) {
			B[i][j] = (i==j)?1:0;
		}
	}

	// conduct multiplication
	matrixMultiply(A, B, C);

	// verify result
	int success = 1;	
	for (int i=0; i<N; i++) {
		for (int j=0; j<MIN(M,K); j++) {
			if (A[i][j] != C[i][j]) {
				success = 0;
			}
		}
		for (int j=MIN(M,K); j<MAX(M,K); j++) {
			if (C[i][j] != 0) {
				success = 0;
			}
		}
	}

	// print verification result
	printf("Verification: %s\n", (success)?"OK":"ERR");

	freeMatrix(A);
	freeMatrix(B);
	freeMatrix(C);

	return success ? EXIT_SUCCESS : EXIT_FAILURE;
}

