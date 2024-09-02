// Alicia Garnier Duarte Franco


#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char** argv) {
  MPI_Init(NULL, NULL);

  int world_size;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);

  if (world_size < 2) {
    fprintf(stderr, "Must be at least two processes for this example\n");
    MPI_Abort(MPI_COMM_WORLD, 1);
  }

  int world_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

  const int MAX_NUMBERS = 2000;
  const int MIN_NUMBERS = 1000; 
  int numbers[MAX_NUMBERS];
  int number_amount;
  int sum; 

  int M = 5 * world_size; 

  if (world_rank == 0)
   {

    
    srand(time(NULL));
    int mensagens_enviadas = 0; 

    for (int i = 0;  i < M; i++) {
        MPI_Status status; 
        int destination_rank;

        MPI_Recv(&destination_rank, 1, MPI_INT, MPI_ANY_SOURCE, 2, MPI_COMM_WORLD, &status); 
        printf("Master receveid message from the process %d communicating that is ready to receive data\n", status.MPI_SOURCE);

        number_amount = MIN_NUMBERS + rand() % (MAX_NUMBERS - MIN_NUMBERS + 1); 

        for (int i = 0; i < number_amount; i++)  { 
            numbers[i] = rand() % 100; 
        }
        
        MPI_Send(numbers, number_amount, MPI_INT, status.MPI_SOURCE, 0, MPI_COMM_WORLD); 
        printf("Master sent %d numbers to process %d \n", number_amount, status.MPI_SOURCE);
        // não itera novamente até que receba alguma resposta

        MPI_Recv(&sum, 1, MPI_INT, status.MPI_SOURCE, 0, MPI_COMM_WORLD, &status); 
        printf("Master received the sum equals to: %d. Message source = %d, tag = %d\n", sum, status.MPI_SOURCE, status.MPI_TAG); 
    }

   for (int i = 1; i < world_size; i++) {
            printf("Master sending termination message to process %d\n", i);
            MPI_Send(NULL, 0, MPI_INT, i, 2, MPI_COMM_WORLD);  
        }

  } else 
    if (world_rank != 0) 
    {

      while(1) {
      MPI_Send(NULL, 0, MPI_INT, 0, 2, MPI_COMM_WORLD); // Processo envia mensagem que está disponível!

      MPI_Status status; // Contém informações sobre a mensagem recebida do mestre

      MPI_Recv(numbers, MAX_NUMBERS, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status); // Recebe a mensagem do mestre (coloquei ANYTAG mas não acho que tá correto)

      if (status.MPI_TAG == 2) { // Se não for TAG 2 são dados e continua a execução e possível próximo recebimento
       printf("Process %d finishing....\n", world_rank); 
       break; 
      }

      // After receiving the message, check the status to determine how many
      // numbers were actually received
      MPI_Get_count(&status, MPI_INT, &number_amount);

      // Print off the amount of numbers, and also print additional information
      // in the status object
      printf("%d received %d numbers from 0. Message source = %d, tag = %d\n",
           world_rank, number_amount, status.MPI_SOURCE, status.MPI_TAG);

      sum = 0; 
      for (int i = 0; i < number_amount; i++) {
        sum += numbers[i]; 
      }



      printf("%d received the sum of values from 0 equal to: %d\n ",world_rank, sum); 
      MPI_Send(&sum, 1, MPI_INT, 0, 0, MPI_COMM_WORLD); 
      // Vai para a próxima iteração
      } 
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