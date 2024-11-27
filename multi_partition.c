#include <pthread.h>
#include <limits.h> // Para LLONG_MAX
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "multi_partition.h"

void *thread_count_partition(void *arg)
{
    thread_data_t *data = (thread_data_t *)arg;
    int start = data->start, end = data->end;
    long long *Input = data->Input, *P = data->P;
    int np = data->np;
    int *local_counts = data->local_counts;

    // Contagem local com busca binária
    for (int i = start; i < end; i++) {
        int partition = binary_search_partition(P, np, Input[i]);
        local_counts[partition]++;
    }

    pthread_barrier_wait(data->barrier); // Sincroniza threads
    return NULL;
}

void merge_counts(int *global_counts, int **local_counts, int nThreads, int np)
{
    for (int t = 0; t < nThreads; t++)
    {
        for (int j = 0; j < np; j++)
        {
            global_counts[j] += local_counts[t][j];
        }
    }
}

thread_data_t *create_thread_data(int start, int end, long long *Input, long long *P, int np, int *local_counts, pthread_mutex_t *mutex, pthread_barrier_t *barrier)
{
    // Alocar memória para a estrutura
    thread_data_t *data = (thread_data_t *)malloc(sizeof(thread_data_t));
    if (data == NULL)
    {
        return NULL; // Retorna NULL se a alocação falhar
    }

    // Preencher os campos da estrutura
    data->start = start;
    data->end = end;
    data->Input = Input;
    data->P = P;
    data->np = np;
    data->local_counts = local_counts;
    data->mutex = mutex;
    data->barrier = barrier;

    return data;
}

void fill_output(long long *Input, int n, long long *P, int np, long long *Output, int *Pos, int *global_counts) {
    // Vetores temporários para cada faixa no Output
    int *current_index = malloc(np * sizeof(int));
    for (int i = 0; i < np; i++)
    {
        current_index[i] = Pos[i];
    }

    // Preenche o vetor Output particionado
    for (int i = 0; i < n; i++) {
        // Determina a partição do elemento atual de Input

        int partition = binary_search_partition(P, np, Input[i]);

        //barreira
        Output[current_index[partition]] = Input[i];
        current_index[partition]++;
    }

    free(current_index);
}


void multi_partition(long long *Input, int n, long long *P, int np, long long *Output, int *Pos, int nT)
{
    int nThreads = nT; // Número de threads
    pthread_t threads[nThreads];
    pthread_barrier_t barrier;
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

    pthread_barrier_init(&barrier, NULL, nThreads);

    // Estruturas de contagem local/global
    int **local_counts = malloc(nThreads * sizeof(int *));
    for (int i = 0; i < nThreads; i++)
    {
        local_counts[i] = calloc(np + 1, sizeof(int)); // Inicializa contagens locais
    }

    // Divisão de trabalho entre threads
    int chunk_size = (n + nThreads - 1) / nThreads;
    thread_data_t *thread_data[nThreads];

    for (int t = 0; t < nThreads; t++)
    {
        // Criar thread_data_t utilizando a função `create_thread_data`
        thread_data[t] = create_thread_data(
            t * chunk_size,                                      // Início do intervalo
            (t + 1) * chunk_size > n ? n : (t + 1) * chunk_size, // Fim do intervalo
            Input,                                               // Ponteiro para Input
            P,                                                   // Ponteiro para P
            np,                                                  // Número de partições
            local_counts[t],                                     // Vetor de contagem local
            &mutex,                                              // Mutex compartilhado
            &barrier                                             // Barreira compartilhada
        );

        if (thread_data[t] == NULL)
        {
            fprintf(stderr, "Erro ao alocar memória para thread_data_t\n");
            exit(EXIT_FAILURE);
        }

        pthread_create(&threads[t], NULL, thread_count_partition, thread_data[t]);
    }

    // Esperar todas as threads terminarem
    for (int t = 0; t < nThreads; t++)
    {
        pthread_join(threads[t], NULL);
        free(thread_data[t]); // Liberar memória da estrutura thread_data_t
    }

    // Junta os resultados de todas as threads
    int *global_counts = calloc(np, sizeof(int));
    merge_counts(global_counts, local_counts, nThreads, np);    

    // Libera recursos
    for (int i = 0; i < nThreads; i++)
    {
        free(local_counts[i]);
    }
    free(local_counts);

    // Global counts agora pode ser usado para calcular Pos (prefix sum)
    
    // Inicializa o vetor Pos com o prefix sum de global_counts
    Pos[0] = 0;
    for (int i = 1; i < np; i++) {
        Pos[i] = Pos[i - 1] + global_counts[i - 1];
    }

    fill_output(Input, n, P, np, Output, Pos, global_counts);
    
    free(global_counts);

    pthread_barrier_destroy(&barrier);
    pthread_mutex_destroy(&mutex);
}

void verifica_particoes(long long *Input, int n, long long *P, int np, long long *Output, int *Pos) {
    int erro = 0;

    // Verifica cada partição
    for (int i = 0; i < np - 1; i++) {
        for (int j = Pos[i]; j < Pos[i + 1]; j++) {
            if (Output[j] >= P[i]){
                erro = 1;
                break;
            }
        }

        if (erro) {
            break;
        }
    }

    // Resultado da verificação
    if (erro) {
        printf("\n===> particionamento COM ERROS\n");
    } else {
        printf("\n===> particionamento CORRETO\n");
    }
}

int binary_search_partition(long long *arr, int size, long long value) {
    int left = 0, right = size - 1;

    while (left <= right) {
        int mid = left + (right - left) / 2;

        if (value < arr[mid]) {
            right = mid - 1;
        } else {
            left = mid + 1;
        }
    }

    // `left` é o índice da partição em que o valor pertence
    return left;
}
