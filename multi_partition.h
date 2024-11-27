#ifndef MULTI_PARTITION_H
#define MULTI_PARTITION_H

#include <pthread.h>
#include <limits.h> // Para LLONG_MAX
#include <stdlib.h>

/**
 * @brief Estrutura para armazenar os dados de entrada das threads.
 */
typedef struct
{
    int start;                  // Índice inicial da parte do vetor Input processada pela thread.
    int end;                    // Índice final da parte do vetor Input processada pela thread.
    long long *Input;           // Ponteiro para o vetor de entrada.
    long long *P;               // Ponteiro para o vetor de partições.
    int np;                     // Número de partições no vetor P.
    int *local_counts;          // Ponteiro para o vetor de contagem local da thread.
    pthread_mutex_t *mutex;     // Mutex compartilhado (não utilizado nesta versão).
    pthread_barrier_t *barrier; // Barreira para sincronização entre threads.
} thread_data_t;

/**
 * @brief Função principal para particionar um vetor de entrada em múltiplas faixas.
 *
 * @param Input Ponteiro para o vetor de entrada com n elementos.
 * @param n Número de elementos no vetor de entrada.
 * @param P Ponteiro para o vetor de partições, que deve estar ordenado.
 * @param np Número de partições no vetor P.
 * @param Output Ponteiro para o vetor de saída, que será particionado em np faixas.
 * @param Pos Ponteiro para o vetor que indica os índices iniciais de cada faixa no Output.
 * @param nThreads Número de threads
 *
 * A função gerencia o fluxo geral do algoritmo, incluindo:
 * - Divisão do trabalho entre threads.
 * - Sincronização e paralelização da contagem de elementos.
 * - Combinação das contagens locais em um vetor global.
 */
void multi_partition(long long *Input, int n, long long *P, int np, long long *Output, int *Pos, int nThreads);

/**
 * @brief Função executada por cada thread para contar os elementos em suas faixas.
 *
 * @param arg Estrutura de dados do tipo `thread_data_t` contendo:
 *            - Índices de início e fim para processamento.
 *            - Ponteiros para os vetores Input, P, e contagens locais.
 *            - Número de partições e barreiras/mutexes compartilhados.
 *
 * A função realiza:
 * - Contagem local de elementos em cada faixa para a parte do vetor atribuída à thread.
 * - Armazena os resultados no vetor de contagem local associado à thread.
 */
void *thread_count_partition(void *arg);

/**
 * @brief Combina os vetores de contagem locais das threads em um vetor global.
 *
 * @param global_counts Ponteiro para o vetor global de contagens (tamanho np + 1).
 * @param local_counts Vetor bidimensional contendo os resultados das threads (nThreads x (np + 1)).
 * @param nThreads Número total de threads utilizadas.
 * @param np Número de partições no vetor P.
 *
 * A função realiza:
 * - Soma os valores correspondentes de cada vetor de contagem local.
 * - Armazena o resultado no vetor global.
 */
void merge_counts(int *global_counts, int **local_counts, int nThreads, int np);

/**
 * @brief Cria e inicializa uma estrutura thread_data_t com os dados fornecidos.
 *
 * @param start Índice inicial da parte do vetor Input processada pela thread.
 * @param end Índice final da parte do vetor Input processada pela thread.
 * @param Input Ponteiro para o vetor de entrada.
 * @param P Ponteiro para o vetor de partições.
 * @param np Número de partições no vetor P.
 * @param local_counts Ponteiro para o vetor de contagem local da thread.
 * @param mutex Ponteiro para o mutex compartilhado (pode ser NULL se não utilizado).
 * @param barrier Ponteiro para a barreira compartilhada entre threads.
 * @return thread_data_t* Ponteiro para a estrutura thread_data_t alocada e inicializada.
 *
 * A estrutura retornada deve ser liberada manualmente pelo usuário com `free` após o uso.
 */
thread_data_t *create_thread_data(int start, int end, long long *Input, long long *P, int np, int *local_counts, pthread_mutex_t *mutex, pthread_barrier_t *barrier);

/**
 * @brief Preenche os vetores Output e Pos com base nos resultados de global_counts.
 *
 * @param Input Vetor de entrada contendo os números a serem particionados.
 * @param n Número de elementos no vetor Input.
 * @param P Vetor de partições (ordenado).
 * @param np Número de partições.
 * @param Output Vetor de saída particionado.
 * @param Pos Vetor que indica os índices iniciais de cada faixa no Output.
 * @param global_counts Vetor contendo as contagens globais de cada faixa.
 */
void fill_output(long long *Input, int n, long long *P, int np, long long *Output, int *Pos, int *global_counts);


/**
 * @brief Verifica se o particionamento foi realizado corretamente.
 *
 * @param Input Vetor de entrada.
 * @param n Tamanho do vetor de entrada.
 * @param P Vetor de partições (ordenado).
 * @param np Número de partições.
 * @param Output Vetor de saída particionado.
 * @param Pos Vetor de posições indicando os inícios das partições.
 */
void verifica_particoes(long long *Input, int n, long long *P, int np, long long *Output, int *Pos);

/**
 * @brief Busca binária para determinar o índice da partição correspondente ao valor.
 *
 * @param arr Vetor de partições (ordenado).
 * @param size Número de partições (tamanho do vetor `arr`).
 * @param value Valor a ser buscado.
 * @return int Índice da partição correspondente ao valor.
 */
int binary_search_partition(long long *arr, int size, long long value);

#endif // MULTI_PARTITION_H
