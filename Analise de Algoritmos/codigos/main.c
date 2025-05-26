#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "sort.h"

void preencher_vetor(int *v, int n, int tipo) {
    if (tipo == 0)
        for (int i = 0; i < n; i++) v[i] = i;
    else if (tipo == 1)
        for (int i = 0; i < n; i++) v[i] = n - i;
    else {
        srand(42);
        for (int i = 0; i < n; i++) v[i] = rand()%100000;
    }
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Uso: %s <algoritmo> <tamanho> <tipo>\n", argv[0]);
        printf("Algoritmos: bubble, insertion, merge, quick, heap, radix\n");
        return 1;
    }

    char *alg = argv[1];
    int n = atoi(argv[2]);
    int tipo = atoi(argv[3]);

    int *v = malloc(n * sizeof(int));
    preencher_vetor(v, n, tipo);

    clock_t ini = clock();

    if (strcmp(alg, "bubble") == 0)
        bubble_sort(v, n);
    else if (strcmp(alg, "insertion") == 0)
        insertion_sort(v, n);
    else if (strcmp(alg, "merge") == 0)
        merge_sort(v, 0, n-1);
    else if (strcmp(alg, "quick") == 0)
        quick_sort(v, 0, n-1);
    else if (strcmp(alg, "heap") == 0)
        heap_sort(v, n);
    else if (strcmp(alg, "radix") == 0)
        radix_sort(v, n);
    else {
        fprintf(stderr, "Algoritmo inválido: %s\n", alg);
        free(v);
        return 1;
    }

    clock_t fim = clock();
    double tempo = (double)(fim - ini) / CLOCKS_PER_SEC;

    printf("%s,%d,%d,%.6f\n", alg, n, tipo, tempo);

    free(v);
    return 0;
}
