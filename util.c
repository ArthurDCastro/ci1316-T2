#include "util.h"

long long *create_vector(int size)
{
    if (size <= 0)
    {
        return NULL; // Tamanho inválido
    }
    return (long long *)malloc(size * sizeof(long long));
}

int *create_pos_vector(int size)
{
    if (size <= 0)
    {
        return NULL; // Tamanho inválido
    }

    // Aloca memória para o vetor Pos
    int *pos = (int *)malloc(size * sizeof(int));
    if (pos == NULL)
    {
        return NULL; // Falha na alocação
    }

    // Inicializa todos os elementos com zero
    for (int i = 0; i < size; i++)
    {
        pos[i] = 0;
    }

    return pos;
}

void destroy_vector(long long *vec)
{
    if (vec != NULL)
    {
        free(vec);
    }
}

void destroy_pos_vector(int *pos)
{
    if (pos != NULL)
    {
        free(pos); // Libera a memória alocada para o vetor Pos
    }
}

void print_pos_vector(const int *pos, int size, const char *label)
{
    if (label != NULL)
    {
        printf("%s:\n", label);
    }

    printf("[");
    for (int i = 0; i < size; i++)
    {
        printf("%d", pos[i]);
        if (i < size - 1)
        {
            printf(", ");
        }
    }
    printf("]\n");
}

void print_vector(const long long *arr, int size, const char *label)
{
    if (label != NULL)
    {
        printf("%s:\n", label);
    }

    printf("[");
    for (int i = 0; i < size; i++)
    {
        printf("%lld", arr[i]);
        if (i < size - 1)
        {
            printf(", ");
        }
    }
    printf("]\n");
}

long long *generate_random_vector(int size, int is_partition)
{
    if (size <= 0)
    {
        return NULL; // Tamanho inválido
    }

    // Criar o vetor usando a função create_vector
    long long *vector = create_vector(size);
    if (vector == NULL)
    {
        return NULL; // Falha na alocação
    }

    // Preencher com números aleatórios
    for (int i = 0; i < size; i++)
    {
        vector[i] = geraAleatorioLL();
    }

    // Ajustar vetor de partições (se necessário)
    if (is_partition)
    {
        vector[size - 1] = LLONG_MAX; // Última posição como LLONG_MAX
        // Ordenar o vetor de partições
        qsort(vector, size, sizeof(long long), compare_long_long);
    }

    return vector;
}

long long geraAleatorioLL()
{
    int a = rand();
    int b = rand();
    return (long long)a * 100 + b;
}

int compare_long_long(const void *a, const void *b)
{
    long long arg1 = *(const long long *)a;
    long long arg2 = *(const long long *)b;
    return (arg1 > arg2) - (arg1 < arg2);
}
