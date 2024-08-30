
#include <stdio.h> 
#include <stdlib.h>
#include <unistd.h>
#include <mpi.h> 

#define PING_PONG_LIMIT 10


int main(int argc, char** argv) {

    MPI_Init(NULL,NULL); 
    
    int world_size; 
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);  
 

    int world_rank; 
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank); 
        
    int number; 
    number = world_rank; 
     
    if (world_rank == 0) {
        MPI_Ssend(&number, 1, MPI_INT, world_rank+1, 0, MPI_COMM_WORLD); 
        MPI_Recv(&number,1, MPI_INT,  world_size-1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE); 
        printf("Process %d received number %d from other process\n", world_rank, number); 

    } else {
        MPI_Recv(&number,1, MPI_INT,  world_size-1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE); 
        MPI_Ssend(&number, 1, MPI_INT, (world_rank+1)%world_size, 0, MPI_COMM_WORLD); 
        printf("Process %d received number %d from other process\n", world_rank, number); 
    }

    MPI_Finalize(); 
}

// Exercício:  Envie o número do processo para o processo acima, e o último para o primeiro, para N processos, usando send/recv, sincronizados. 

