
#include <stdio.h> 
#include <stdlib.h>

#include <unistd.h>
#include <mpi.h> 


int main(int argc, char** argv) {

    MPI_Init(NULL,NULL); 
    

    int world_size; 
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);  //  world_size todos os processos tem conhecimento da quantidade de processos que também estão executando
 

    int world_rank; 
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank); // world_rank os processos tem conhecimento do seu número (identificação) dentre todos os processos existentes; 
        // Comm: este é o comunicador (processo?)
        // Enumeração (COMM_WORLD): estabelece que todos os processos possuem um mecanismo de comunicação (comunicação interprocessos)

    char processor_name[MPI_MAX_PROCESSOR_NAME];   
    int name_len; 

    MPI_Get_processor_name(processor_name, &name_len);

       if (world_rank == 0) {
         sleep(50); 
       }

    printf("Hello world from processor %s, rank %d out of %d processors\n", processor_name, world_rank, world_size);

    //os processos nao avançam(finalizing)
    MPI_Barrier(MPI_COMM_WORLD); // SINCRONIZAÇÃO
    printf("Finalizing process %d out of %d processors\n", world_rank, world_size); 

    MPI_Finalize(); // interrompe todos 
}