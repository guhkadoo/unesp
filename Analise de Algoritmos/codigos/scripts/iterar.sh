#!/bin/bash

ALGOS=("bubble" "insertion" "merge" "quick" "heap" "radix")
TAMANHOS=(1000 7600 14200 20800 27400 34000 40600 47200 53800 60400 67000 73600 80200 86800 93400 100000)
TIPOS=(0 1 2) # 0=crescente, 1=decrescente, 2=aleatório

mkdir -p ../resultados
echo "algoritmo,tamanho,tipo,tempo" > ../resultados/resultados.csv

gcc -O2 -o ../sorter ../main.c ../sort.c

for algo in "${ALGOS[@]}"; do
    echo "Executando testes para $algo..."
    for tipo in "${TIPOS[@]}"; do
        for n in "${TAMANHOS[@]}"; do
            soma=0
            for i in {1..5}; do
                resultado=$(../sorter $algo $n $tipo)
                tempo=$(echo "$resultado" | awk -F',' '{print $4}')
                soma=$(echo "$soma + $tempo" | bc -l)
            done
            media=$(echo "$soma / 5" | bc -l)
            echo "$algo,$n,$tipo,$media" >> ../resultados/resultados.csv
        done
    done
done

