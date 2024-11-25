#ifndef UTIL_H
#define UTIL_H

#include <stdlib.h> // Para malloc e free
#include <stdio.h>  // Para printf
#include <limits.h> // Para LLONG_MAX

/**
 * @brief Cria um vetor de inteiros longos com o tamanho especificado.
 *
 * @param size Tamanho do vetor a ser criado.
 * @return long long* Ponteiro para o vetor criado ou NULL em caso de falha.
 *
 * O vetor retornado deve ser liberado com a função `destroy_vector`.
 */
long long *create_vector(int size);

/**
 * @brief Cria e inicializa o vetor `Pos` com zeros.
 *
 * @param size Tamanho do vetor `Pos` (geralmente `np + 1`).
 * @return int* Ponteiro para o vetor criado ou NULL em caso de falha.
 *
 * O vetor retornado deve ser liberado com `free` pelo usuário.
 */
int *create_pos_vector(int size);

/**
 * @brief Libera a memória alocada para um vetor.
 *
 * @param vec Ponteiro para o vetor a ser liberado.
 */
void destroy_vector(long long *vec);

/**
 * @brief Libera a memória alocada para o vetor `Pos`.
 * @param pos Ponteiro para o vetor `Pos` a ser liberado.
 */
void destroy_pos_vector(int *pos);

/**
 * @brief Imprime os elementos do vetor `Pos`.
 * @param pos Ponteiro para o vetor `Pos` a ser impresso.
 * @param size Tamanho do vetor `Pos`.
 * @param label Rótulo opcional para identificar o vetor na saída.
 */
void print_pos_vector(const int *pos, int size, const char *label);

/**
 * @brief Imprime os elementos de um vetor de inteiros longos.
 *
 * @param arr Ponteiro para o vetor a ser impresso.
 * @param size Número de elementos no vetor.
 * @param label Rótulo opcional para identificar o vetor na saída.
 */
void print_vector(const long long *arr, int size, const char *label);

/**
 * @brief Gera um vetor aleatório de inteiros longos.
 *
 * @param size Tamanho do vetor a ser criado.
 * @param is_partition Define se o vetor é de partições:
 *                     - 0: Apenas números aleatórios.
 *                     - 1: Gera os primeiros (size-1) elementos aleatórios
 *                          e preenche a última posição com LLONG_MAX.
 * @return long long* Ponteiro para o vetor gerado ou NULL em caso de falha.
 *
 * O vetor retornado deve ser liberado pelo usuário com `destroy_vector`.
 */
long long *generate_random_vector(int size, int is_partition);

/**
 * @brief Função auxiliar para gerar números aleatórios longos.
 *
 * @return long long Número aleatório gerado.
 */
long long geraAleatorioLL();

/**
 * @brief Função de comparação para o qsort de long long.
 *
 * @param a Ponteiro para o primeiro elemento.
 * @param b Ponteiro para o segundo elemento.
 * @return int Resultado da comparação:
 *         - Negativo se *a < *b.
 *         - Zero se *a == *b.
 *         - Positivo se *a > *b.
 */
int compare_long_long(const void *a, const void *b);

#endif // UTIL_H
