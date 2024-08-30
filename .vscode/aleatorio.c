#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
    // Inicializa o gerador de números aleatórios com a semente baseada no tempo atual
    srand(time(NULL));

    // Define os limites do intervalo
    int min = 1000;
    int max = 2000;

    // Gera o número aleatório dentro do intervalo [min, max]
    int random_value = min + rand() % (max - min + 1);

    printf("Valor aleatório entre %d e %d: %d\n", min, max, random_value);

    return 0;
}
