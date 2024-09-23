#include <mpi.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define MAX_FLOAT 10.0

// Nome: Alicia Garnier Duarte Franco


//Faça a soma de duas matrizes em uma terceira matriz paralelamente.
// Os valores serão gerados somente no processo de rank 0.
// As quantidades de linhas e colunas serão conhecidas na linha de comando.

void random_matrix(float *matrix, int rows, int columns)
{
  for (int i = 0; i < rows; i++)
  {
    for (int j = 0; j < columns; j++)
      matrix[i * columns + j] = ((float)rand() / (float)RAND_MAX) * MAX_FLOAT;
  }
}

void sum_matrix(float *A, float *B, float *C, int dim)
{
  for (int i = 0; i < dim; i++)
    C[i] = A[i] + B[i];
}

void print_matrix(float *matrix, int rows, int columns)
{
  for (int i = 0; i < rows; i++)
  {
    for (int j = 0; j < columns; j++)
    {
      printf("%f ", matrix[i * columns + j]);
    }
    printf("\n");
  }
}

int main(int argc, char *argv[])
{
  if (argc != 3)
  {
    printf("Usage: number of rows and number of columns");
    return EXIT_FAILURE;
  }
  int rows = atoi(argv[1]);
  int columns = atoi(argv[2]);
  int dimension = rows * columns;

  MPI_Init(&argc, &argv);
  int rank;
  int size;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  srand(time(NULL) + rank);

  if (rows % size != 0)
  {
    printf("Number of processes must be compatible with the number of rows\n");
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Finalize();
  }

  float *matrix_A = NULL;
  float *matrix_B = NULL;
  float *matrix_C = NULL;

  if (rank == 0)
  {
    matrix_A = (float *)malloc(dimension * sizeof(float));
    matrix_B = (float *)malloc(dimension * sizeof(float));
    matrix_C = (float *)malloc(dimension * sizeof(float));

    random_matrix(matrix_A, rows, columns);
    random_matrix(matrix_B, rows, columns);
  }

  int chunk_size = (rows / size) * columns;

  float *division_A = (float *)malloc(chunk_size * sizeof(float));
  float *division_B = (float *)malloc(chunk_size * sizeof(float));

  MPI_Scatter(matrix_A, chunk_size, MPI_INT, division_A, chunk_size, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Scatter(matrix_B, chunk_size, MPI_INT, division_B, chunk_size, MPI_INT, 0, MPI_COMM_WORLD);

  float *division_C = (float *)malloc(chunk_size * sizeof(float));
  sum_matrix(division_A, division_B, division_C, chunk_size);

  MPI_Gather(division_C, chunk_size, MPI_INT, matrix_C, chunk_size, MPI_INT, 0, MPI_COMM_WORLD);

  if (rank == 0)
  {
    printf("\n Number of process: %d \n %d rows per process \n %d elements per process of %d total\n\n",
           size, chunk_size / columns, chunk_size, dimension);

    printf("Matrix A: \n");
    print_matrix(matrix_A, rows, columns);

    printf("\nMatrix B: \n");
    print_matrix(matrix_B, rows, columns);

    // soma 
    printf("\nMatrix C: \n");
    print_matrix(matrix_C, rows, columns);

    free(matrix_A);
    free(matrix_B);
    free(matrix_C);
  }

  free(division_A);
  free(division_B);
  free(division_C);

  MPI_Barrier(MPI_COMM_WORLD);
  MPI_Finalize();
}