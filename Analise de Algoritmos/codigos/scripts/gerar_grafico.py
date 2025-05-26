import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
import numpy as np

# Carregar os dados
df = pd.read_csv("../resultados/resultados.csv")

# Mapear tipos para nomes legíveis
mapa_tipos = {0: "Crescente", 1: "Decrescente", 2: "Aleatório"}
print(df)
df["tipo"] = df["tipo"].map(mapa_tipos)
print(df)

# Estilo bonito com seaborn
sns.set(style="whitegrid")

# Lista de algoritmos únicos
algoritmos = df["algoritmo"].unique()
print(algoritmos)

# Gerar um gráfico para cada algoritmo
for alg in algoritmos:
    dados_alg = df[df["algoritmo"] == alg]
    
    plt.figure(figsize=(8,5))
    
    # Plotar curvas para cada tipo de vetor
    for tipo in dados_alg["tipo"].unique():
        dados_tipo = dados_alg[dados_alg["tipo"] == tipo]
        plt.plot(dados_tipo["tamanho"], dados_tipo["tempo"], marker="o", label=tipo)
    
    plt.title(f"Desempenho do {alg.capitalize()} Sort")
    plt.xlabel("Tamanho do Vetor")
    plt.ylabel("Tempo (s)")
    plt.legend()
    plt.tight_layout()
    plt.savefig(f"../resultados/{alg}_grafico.png")
    plt.close()  # Fechar para não sobrepor gráficos

