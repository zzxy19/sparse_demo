#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ROW 40
#define COL 40
#define ELEMENT 8

void init() {
	time_t t;
	/* Intializes random number generator */
	srand((unsigned)time(&t));
}

int compare_function(const void *a, const void *b) {
	int *x = (int *)a;
	int *y = (int *)b;
	return *x - *y;
}

struct MatrixElement {
	int row;
	int col;
	int val;
};

void createDistinctIndices(int result[], int n, int maxIndex) {
	for (int i = 0; i < n; i++) {
		result[i] = 0;
	}
	int count = 0;
	while (count < n) {
		int c = rand() % maxIndex;
		bool found = false;
		for (int i = 0; i < count; i++) {
			if (result[i] == c) {
				found = true;
				break;
			}
		}
		if (found)
			continue;
		else {
			result[count] = c;
			count++;
		}
	}
}

void createSparseMatrix(struct MatrixElement matrix[], int n) {
	int pool[ELEMENT] = {1,-1,2,-2,4,-4,4,-4};
	int result[ELEMENT];
	int poolOrder[ELEMENT];
	for (int r = 0; r < ROW; r++) {
		createDistinctIndices(result, ELEMENT, COL);
		qsort(result, ELEMENT, sizeof(int), compare_function);
		createDistinctIndices(poolOrder, ELEMENT, ELEMENT);
		for (int i = 0; i < ELEMENT; i++) {
			struct MatrixElement ele;
			ele.col = result[i];
			ele.row = r;
			ele.val = pool[poolOrder[i]];
			matrix[r*ELEMENT + i] = ele;
		}
	}
}

void createDenseMatrix(int matrix[], int n) {
	for (int i = 0; i < n; i++) {
		matrix[i] = 0;
	}
	int count1 = 0;
	while (count1 < n / 2) {
		int c = rand() % n;
		if (matrix[c] == 0) {
			matrix[c] = 1;
			count1++;
		}
	}
}

void outputSparseMatrix(struct MatrixElement matrix[], int n) {
	for (int i = 0; i < n; i++) {
		printf("Row: %d, Col: %d, Val: %d\n", matrix[i].row, matrix[i].col, matrix[i].val);
	}
}

void outputDenseMatrix(int matrix[], int n) {
	for (int i = 0; i < n; i++) {
		printf("%d ", matrix[i]);
	}
}

void outputDenseMatrix(double matrix[], int n) {
	for (int i = 0; i < n; i++) {
		printf("%f ", matrix[i]);
	}
}

/*

1 2 3	1
4 5 6	5
7 8 9	9

*/

void matmul(struct MatrixElement matrix1[], int matrix2[], int matrix3[]) {
	for (int r = 0; r < COL; r++) {
		int acc = 0;
		for (int i = 0; i < ELEMENT; i++) {
			int c = matrix1[r*ELEMENT + i].col;
			int val = matrix1[r*ELEMENT + i].val;
			acc += val * matrix2[c];
		}
		matrix3[r] = acc;
	}
}

void run(double result[]) {
	/*
	Task 1:
		create a sparse matrix of size 40*40
		each row has 8 non-zero elements
		eight elements are +-1, +-2, +-4, +-4
	*/
	const int N = ELEMENT*ROW;
	struct MatrixElement matrix1[N];
	createSparseMatrix(matrix1, N);

	/*
	Task 2:
		create a matrix of size 40*1
		half of elements are 0s, half are 1s
	*/
	int matrix2[COL];
	createDenseMatrix(matrix2, COL);

	/*
	Task 3:
		do matrix multiplication on matrix1 and matrix2
		to create output matrix3 of size 40*1
	*/
	int matrix3[COL];
	matmul(matrix1, matrix2, matrix3);
	
	for (int i = 0; i < COL; i++) {
		result[i] += (double) matrix3[i];
	}
}

int main() {
	init();
	double result[COL];
	for (int i = 0; i < COL; i++) {
		result[i] = 0.0;
	}
	for (int i = 0; i < 10000; i++) {
		run(result);
	}
	for (int i = 0; i < COL; i++) {
		result[i] /= 10000.0;
	}
	outputDenseMatrix(result, COL);
	return 0;
}
