#include <pthread.h>
#include <limits.h> // Para LLONG_MAX
#include <stdlib.h>
#include <stdio.h>

#include "multi_partition.h"
#include "util.h"
#include "chrono.h"

#define MAX_THREADS 64        // Limite de threads permitido
#define MAX_PARTITIONS 100000 // Limite de partições permitido
#define NTIMES 10

int main(int argc, char *argv[])
{
    int n, np, nThreads;

    chronometer_t parallelReductionTime;

    // Verifica se o número correto de argumentos foi passado
    if (argc != 4)
    {
        fprintf(stderr, "Uso: %s <nTotalElements> <nPartitions> <nThreads>\n", argv[0]);
        return 1;
    }

    // Converte os argumentos da linha de comando
    n = atoi(argv[1]);        // Número total de elementos
    np = atoi(argv[2]);       // Número de partições
    nThreads = atoi(argv[3]); // Número de threads
    // np = 100000;       // Número de partições
    nThreads = atoi(argv[3]); // Número de threads

    // Validação dos argumentos
    if (n <= 0)
    {
        fprintf(stderr, "Erro: <nTotalElements> deve ser maior que 0.\n");
        return 1;
    }

    if (np <= 0 || np > MAX_PARTITIONS)
    {
        fprintf(stderr, "Erro: <nPartitions> deve ser entre 1 e %d.\n", MAX_PARTITIONS);
        return 1;
    }

    if (nThreads <= 0 || nThreads > MAX_THREADS)
    {
        fprintf(stderr, "Erro: <nThreads> deve ser entre 1 e %d.\n", MAX_THREADS);
        return 1;
    }

    // Exibe os valores lidos
    printf("Executando com %d elementos, %d partições e %d threads.\n", n, np, nThreads);

    long long *Input = generate_random_vector(n, 0); // Vetor de entrada aleatório
    long long *P = generate_random_vector(np, 1);    // Vetor de partições aleatório, ordenado
    long long *Output = create_vector(n);            // Vetor de saída (não inicializado)
    int *Pos = create_pos_vector(np);                // Vetor Pos com np + 1 posições

    // Testa sucesso da alocação
    if (Input == NULL || P == NULL || Output == NULL || Pos == NULL)
    {
        fprintf(stderr, "Erro ao alocar memória para os vetores.\n");

        // Libera memória alocada parcialmente
        destroy_vector(Input);
        destroy_vector(P);
        destroy_vector(Output);
        free(Pos);

        return 1;
    }

    // Imprimir vetores para validação
    printf("\n--- Vetores Gerados com Sucesso ---\n");

    chrono_reset(&parallelReductionTime);
    chrono_start(&parallelReductionTime);

    for (int i = 0; i < NTIMES; i++)
        multi_partition(Input, n, P, np, Output, Pos, nThreads);

    chrono_stop(&parallelReductionTime);

    printf("\n--- Vetor particionado ---\n");

    verifica_particoes(Input, n, P, np, Output, Pos);

    chrono_reportTime(&parallelReductionTime, "parallelReductionTime");

    // calcular e imprimir a VAZAO (numero de operacoes/s)
    double total_time_in_seconds = (double)chrono_gettotal(&parallelReductionTime) /
                                   ((double)1000 * 1000 * 1000);
    printf("total_time_in_seconds: %lf s\n", total_time_in_seconds);

    double OPS = ((double)n * NTIMES) / total_time_in_seconds;
    printf("Throughput: %lf OP/s\n", OPS);

    // Limpa memória
    destroy_vector(Input);
    destroy_vector(P);
    destroy_vector(Output);
    destroy_pos_vector(Pos);

    printf("\nTeste de sucesso concluído! Memória liberada corretamente.\n");
    return 0;
}