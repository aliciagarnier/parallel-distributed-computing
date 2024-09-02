// Example of checking the MPI_Status object from an MPI_Recv call
//
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char** argv) {
  MPI_Init(NULL, NULL);

  int world_size;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);

  if (world_size != 2) {
    fprintf(stderr, "Must use two processes for this example\n");
    MPI_Abort(MPI_COMM_WORLD, 1);
  }

  int world_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

  const int MAX_NUMBERS = 2000;
  const int MIN_NUMBERS = 1000; 
  int numbers[MAX_NUMBERS];
  int number_amount;
  int sum; 

  if (world_rank == 0)
   {

    srand(time(NULL));
    number_amount = MIN_NUMBERS + rand() % (MAX_NUMBERS - MIN_NUMBERS + 1); 

    
    for (int i = 0; i < number_amount; i++) 
    {
      numbers[i] = rand() % 100; 
    }


    printf("0 will send %d numbers to 1\n", number_amount);
    MPI_Send(numbers, number_amount, MPI_INT, 1, 0, MPI_COMM_WORLD);

    MPI_Barrier(MPI_COMM_WORLD); 

    MPI_Status status; 
    MPI_Recv(&sum, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, &status); 
    printf("0 received the sum from 1 equals to: %d. Message source = %d, tag = %d\n", sum, status.MPI_SOURCE, status.MPI_TAG); 


  } else 
    if (world_rank == 1) 
    {

      MPI_Status status; // Contém informações sobre a mensagem recebida 

      // Receive at most MAX_NUMBERS from process zero.
      MPI_Recv(numbers, MAX_NUMBERS, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);

      // After receiving the message, check the status to determine how many
      // numbers were actually received
      MPI_Get_count(&status, MPI_INT, &number_amount);

      // Print off the amount of numbers, and also print additional information
      // in the status object
      printf("1 received %d numbers from 0. Message source = %d, tag = %d\n",
           number_amount, status.MPI_SOURCE, status.MPI_TAG);

      sum = 0; 
      for (int i = 0; i < number_amount; i++) {
        sum += numbers[i]; 
      }

      printf("1 received the sum of values from 0 equal to: %d\n ", sum); 
      MPI_Send(&sum, 1, MPI_INT, 0, 0, MPI_COMM_WORLD); 

      MPI_Barrier(MPI_COMM_WORLD); 

    }
    

  MPI_Barrier(MPI_COMM_WORLD);

  MPI_Finalize();

}

/* Exercícios

1º Faça um programa com a estrutura mestre/escravo, onde:

 - mestre: gera uma quantidade aleatória entre 1000 e 2000 números de valores aleatórios entre 0 e 99, e envia para cada escravo.
 
 - escravo: recebe os valores gerados pelo mestre, calcula a somatória dos valores e devolve para o mestre.
 
 2º No programa acima, realize M vezes a geração de valores, onde M deve ser 5 vezes o número de processos iniciados. O mestre deve enviar os valores gerados para os escravos que terminarem primeiro.
  Para deixar o trabalho dos escravos mais demorados, utilize float ou aumente a quantidade números gerados para cada escravo. */ 