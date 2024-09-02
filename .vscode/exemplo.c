#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_NUMBERS 100

int main(int argc, char** argv) {
    MPI_Init(NULL, NULL);

    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    if (world_size < 2) {
        fprintf(stderr, "Must use at least two processes for this example\n");
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    int M = 5 * (world_size - 1); // Número total de tarefas
    int numbers[MAX_NUMBERS];
    int number_amount;
    float sum = 0.0;

    if (world_rank == 0) {
        srand(time(NULL));

        for (int i = 0; i < M; i++) {
            MPI_Status status;
            int dest_rank;

            // Espera até que um escravo esteja disponível
            MPI_Recv(&dest_rank, 1, MPI_INT, MPI_ANY_SOURCE, 2, MPI_COMM_WORLD, &status);

            // Gera a tarefa
            number_amount = (rand() / (float)RAND_MAX) * MAX_NUMBERS;
            for (int j = 0; j < number_amount; j++) {
                numbers[j] = rand() % 100;
            }

            // Envia a tarefa para o escravo disponível
            MPI_Ssend(&number_amount, 1, MPI_INT, dest_rank, 0, MPI_COMM_WORLD);
            MPI_Ssend(numbers, number_amount, MPI_INT, dest_rank, 0, MPI_COMM_WORLD);

            printf("Mestre enviou %d números para o processo %d\n", number_amount, dest_rank);

            // Recebe a soma do escravo
            float received_sum;
            MPI_Recv(&received_sum, 1, MPI_FLOAT, dest_rank, 1, MPI_COMM_WORLD, &status);
            sum += received_sum;
            printf("Mestre recebeu a soma %f de um escravo\n", received_sum);
        }

        // Envia mensagens de finalização para todos os escravos
        for (int i = 1; i < world_size; i++) {
            MPI_Ssend(NULL, 0, MPI_INT, i, 2, MPI_COMM_WORLD); // Tag 2 para sinalizar fim
        }

        printf("Soma total recebida pelo mestre: %f\n", sum);
    } else {
        while (1) {
            MPI_Status status;
            // Informa ao mestre que está pronto para receber nova tarefa
            MPI_Ssend(NULL, 0, MPI_INT, 0, 2, MPI_COMM_WORLD);

            MPI_Probe(0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            int tag = status.MPI_TAG;
            if (tag == 2) break; // Recebeu sinal de fim

            int recv_count;
            MPI_Recv(&recv_count, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
            MPI_Recv(numbers, recv_count, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);

            // Calcula a soma dos números recebidos
            float local_sum = 0.0;
            for (int i = 0; i < recv_count; i++) {
                local_sum += numbers[i];
            }

            // Envia a soma de volta para o mestre
            MPI_Ssend(&local_sum, 1, MPI_FLOAT, 0, 1, MPI_COMM_WORLD);
        }
    }

    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Finalize();
}
