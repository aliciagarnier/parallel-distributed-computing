#include <mpi.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define MAX_VALUE 1000.0


void random_matrix(double *matrix, int rows, int columns)
{
  for (int i = 0; i < rows; i++)
  {
    for (int j = 0; j < columns; j++)
    {
      matrix[i * columns + j] = ((float)rand() / (float)RAND_MAX) * MAX_VALUE;
    }
  }
}

void print_matrix(double *matrix, int rows, int cols)
{
  for (int i = 0; i < rows; i++)
  {
    for (int j = 0; j < cols; j++)
    {
      printf(" %f ", matrix[i * cols + j]);
    }
    printf("\n");
  }
}

void dot_product(double *A, double *B, double *C, int columnsA, int columnsB)
{
  for (int j = 0; j < columnsB; j++)
  {
    C[j] = 0;
    for (int k = 0; k < columnsA; k++)
    {
      C[j] += A[k] * B[k * columnsB + j];
    }
  }
}


int main(int argc, char *argv[])
{
  if (argc != 2)
  {
    printf("Usage: %s <number of columns in matrix A (and rows in matrix B)>\n", argv[0]);
    return EXIT_FAILURE; 
  }

 // A quantidade de processos definirá a quantidade de colunas da primeira matriz e linhas da segunda matriz. 
  MPI_Init(&argc, &argv);

  int columnsA, rowsB;
  columnsA = rowsB = atoi(argv[1]);

  int rank;
  int size;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int rowsA, columnsB;
  rowsA = columnsB = size;

  srand(time(NULL) + rank);

  double *matrix_A = NULL;
  double *matrix_B = (double *)malloc((rowsB * columnsB) * sizeof(double));
  double *matrix_C = NULL;

  if (rank == 0)
  {
    matrix_A = (double *)malloc((rowsA * columnsA) * sizeof(double));
    matrix_C = (double *)malloc((rowsA * columnsB) * sizeof(double));

    random_matrix(matrix_A, rowsA, columnsA);
    random_matrix(matrix_B, rowsB, columnsB);
  }

  int chunk_size_a = columnsA;
  int chunk_size_c = columnsB;


  double *local_A = (double *)malloc(chunk_size_a * sizeof(double));
  double *local_C = (double *)malloc(chunk_size_c * sizeof(double));

  
  MPI_Scatter(matrix_A, chunk_size_a, MPI_DOUBLE, local_A, chunk_size_a, MPI_DOUBLE, 0, MPI_COMM_WORLD);

  MPI_Bcast(matrix_B, (rowsB * columnsB), MPI_DOUBLE, 0, MPI_COMM_WORLD);

  dot_product(local_A, matrix_B, local_C, columnsA, columnsB);

  MPI_Gather(local_C, chunk_size_c, MPI_DOUBLE, matrix_C, chunk_size_c, MPI_DOUBLE, 0, MPI_COMM_WORLD);

  if (rank == 0)
  {
    printf("\nMATRIX A = \n");
    print_matrix(matrix_A, size, columnsA);

    printf("\nMATRIX B = \n");
    print_matrix(matrix_B, rowsB, size);

    printf("\nMATRIX C (AxB) = \n");
    print_matrix(matrix_C, rowsA, columnsB);
  }

  MPI_Finalize();
}