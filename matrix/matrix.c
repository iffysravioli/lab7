
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define MAX 20  // Define matrix size to 20x20

int matA[MAX][MAX];
int matB[MAX][MAX];

int matSumResult[MAX][MAX];
int matDiffResult[MAX][MAX];
int matProductResult[MAX][MAX];

typedef struct {
    int startRow;
    int endRow;
} ThreadData;

void fillMatrix(int matrix[MAX][MAX]) {
    for(int i = 0; i < MAX; i++) {
        for(int j = 0; j < MAX; j++) {
            matrix[i][j] = rand() % 10 + 1;  // Fill with random values 1-10
        }
    }
}

void printMatrix(int matrix[MAX][MAX]) {
    for(int i = 0; i < MAX; i++) {
        for(int j = 0; j < MAX; j++) {
            printf("%5d", matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

// Compute matrix addition in parallel
void* computeSum(void* args) {
    ThreadData* data = (ThreadData*) args;
    for (int i = data->startRow; i < data->endRow; i++) {
        for (int j = 0; j < MAX; j++) {
            matSumResult[i][j] = matA[i][j] + matB[i][j];
        }
    }
    pthread_exit(0);
}

// Compute matrix subtraction in parallel
void* computeDiff(void* args) {
    ThreadData* data = (ThreadData*) args;
    for (int i = data->startRow; i < data->endRow; i++) {
        for (int j = 0; j < MAX; j++) {
            matDiffResult[i][j] = matA[i][j] - matB[i][j];
        }
    }
    pthread_exit(0);
}

// Compute matrix dot product in parallel
void* computeProduct(void* args) {
    ThreadData* data = (ThreadData*) args;
    for (int i = data->startRow; i < data->endRow; i++) {
        for (int j = 0; j < MAX; j++) {
            matProductResult[i][j] = 0;
            for (int k = 0; k < MAX; k++) {
                matProductResult[i][j] += matA[i][k] * matB[k][j];
            }
        }
    }
    pthread_exit(0);
}

void parallelMatrixOperations() {
    pthread_t threads[10];
    ThreadData threadData[10];

    int rowsPerThread = MAX / 10;

    // Spawn threads for sum, diff, and product
    for (int i = 0; i < 10; i++) {
        threadData[i].startRow = i * rowsPerThread;
        threadData[i].endRow = (i + 1) * rowsPerThread;

        pthread_create(&threads[i], NULL, computeSum, &threadData[i]);
    }
    for (int i = 0; i < 10; i++) {
        pthread_join(threads[i], NULL);
    }

    for (int i = 0; i < 10; i++) {
        pthread_create(&threads[i], NULL, computeDiff, &threadData[i]);
    }
    for (int i = 0; i < 10; i++) {
        pthread_join(threads[i], NULL);
    }

    for (int i = 0; i < 10; i++) {
        pthread_create(&threads[i], NULL, computeProduct, &threadData[i]);
    }
    for (int i = 0; i < 10; i++) {
        pthread_join(threads[i], NULL);
    }
}

int main() {
    srand(time(0));
    fillMatrix(matA);
    fillMatrix(matB);

    printf("Matrix A:\n");
    printMatrix(matA);

    printf("Matrix B:\n");
    printMatrix(matB);

    parallelMatrixOperations();

    printf("Sum of matrices:\n");
    printMatrix(matSumResult);

    printf("Difference of matrices:\n");
    printMatrix(matDiffResult);

    printf("Product of matrices:\n");
    printMatrix(matProductResult);

    return 0;
}