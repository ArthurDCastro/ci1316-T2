# Paralelização de Particionamento - CI1316 Trabalho 2

Este repositório contém a implementação do **Trabalho 2** da disciplina CI1316, que consiste em paralelizar a função de particionamento de um vetor em múltiplas faixas utilizando **Pthreads**. O objetivo é explorar paralelismo em sistemas multicore e analisar o desempenho do algoritmo com diferentes números de threads.

## **Descrição do Algoritmo**

A função principal, `multi_partition`, divide um vetor de entrada `Input` em várias faixas definidas por um vetor de partições `P`. O processo de particionamento é dividido em duas etapas principais:

1. **Contagem de Elementos em Cada Faixa**:

   - As threads realizam a contagem de elementos localmente em suas respectivas porções do vetor de entrada.
   - Os resultados locais são combinados em um vetor global utilizando uma operação de redução.

2. **Cálculo de Offsets e Escrita no Vetor de Saída**:
   - Cada thread utiliza os offsets calculados a partir das contagens para preencher o vetor de saída `Output`.

A implementação utiliza barreiras e mutexes para sincronizar as threads e garantir a consistência dos dados.

---

## **Estrutura do Repositório**

- **`main.c`**: Contém a função principal e integra as etapas do algoritmo.
- **`multi_partition.c`**: Implementa a função `multi_partition` e organiza o fluxo do algoritmo.
- **`Makefile`**: Automação da compilação do projeto.
- **`README.md`**: Este arquivo.

---

## **Como Compilar**

Para compilar o projeto, utilize o comando:

```bash
make
```

---

## **Como Executar**

O programa exige dois argumentos:

1. Número total de elementos do vetor de entrada (`n`).
2. Número de threads (`nThreads`).

Exemplo de execução:

```bash
./multi_partition 16000000 4
```

---

## **Estrutura das Funções**

1. **`multi_partition`**:

   - Gerencia a execução geral do algoritmo.
   - Inicializa threads e estruturas de sincronização.
   - Combina os resultados das threads.

2. **`thread_count_partition`**:

   - Executada por cada thread para contar elementos do vetor em suas respectivas faixas.

3. **`merge_counts`**:
   - Junta os resultados das contagens locais em um vetor global.

---

## **Análise de Desempenho**

O desempenho do algoritmo será analisado em termos de:

- **Tempo de execução**: Medido para diferentes números de threads.
- **Escalabilidade**: Avaliação da aceleração obtida com mais threads.
- **Throughput**: Elementos processados por segundo.

Os resultados das medições serão organizados em planilhas e gráficos para facilitar a análise.

---

## **Contato**

- **Disciplina**: CI1316 - Sistemas Operacionais
- **Autor**: Arthur Dias Viana de Castro, Eduardo Neves
- **E-mail**: [advc19@inf.ufpr.br](mailto:advc19@inf.ufpr.br)
