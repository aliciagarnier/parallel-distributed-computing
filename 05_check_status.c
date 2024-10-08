#include <mpi.h> 
#include <stdio.h> 
#include <stdlib.h>
#include <time.h> 


int main(int argc, char** argv) {

    MPI_Init(NULL, NULL); 
     
    int world_size; 
    MPI_Comm_size(MPI_COMM_WORLD, &world_size); 
    
    if (world_size != 2) 
    {
        fprintf(stderr, "Must use two processes for this example\n"); 
        MPI_Abort(MPI_COMM_WORLD, 1); 
    }

    int world_rank; 
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank); 

    const int MAX_NUMBERS = 100; 
    int numbers[MAX_NUMBERS]; 
    int number_amount; 

    if (world_rank == 0) 
    {
        // Pick a random amount of integers to send to process one
        srand(time(NULL)); 
        number_amount = (rand() / (float)RAND_MAX) + MAX_NUMBERS; 
        // Send the amount of integers to process one
        MPI_Send(numbers, number_amount, MPI_INT, 1, 0, MPI_COMM_WORLD);
        printf("0 sent %d numbers to 1\n", number_amount); 
    } else 
      if (world_rank == 1) {
        MPI_Status status; 
        // Receive at most MAX_NUMBERS from process zero
        MPI_Recv(numbers, MAX_NUMBERS, MPI_INT, 0,0, MPI_COMM_WORLD, &status); 
        // After receiving the massage, check the status to determine how many numbers were actually received. 
        MPI_Get_count(&status, MPI_INT, &number_amount); 
        // Print off the amount of numbers, and also print additional information in the status object. 
        printf("1 Received %d numbers from 0. Message source = %d, tag = %d\n", number_amount, status.MPI_SOURCE, status.MPI_TAG);
      }
    MPI_Barrier(MPI_COMM_WORLD); 
    MPI_Finalize(); 
}

// 1: Faça um programa com a estrutura mestre escravo, onde: 

// - mestre: gera uma quantidade aleatória entre 1000 e 2000 números de valores aleatórios entre 0 e 99, e envia para cada escravo. 
// - escravo: recebe os valores gerados pelo mestre, calcula a somatória dos valores e devolve para o mestre.


// 2: no programa acima, realize M vezes a geração de valores, onde M deve ser 5 vezes o número de processos iniciado. O mestre deve enviar os valores para os escravos que 
// terminarem primeriro.
// Para deixar o trabalho dos escravos mais demorados, utilize float ou aumente a quantidade de número gerados para o escravo. 