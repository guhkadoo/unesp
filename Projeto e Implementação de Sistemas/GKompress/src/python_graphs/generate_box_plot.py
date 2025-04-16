import os
import csv
import matplotlib.pyplot as plt

def calculate_compression_data(csv_file, file_extensions):
    # Dicionário para armazenar as taxas de compressão por tipo de arquivo
    compression_data = {'txt': [], 'bmp': [], 'wav': []}

    with open(csv_file, newline='') as file:
        reader = csv.DictReader(file)
        for row in reader:
            original_size = int(row["Original Size (bytes)"])
            compressed_size = int(row["Compressed Size (bytes)"])
            file_name = row["File Name"]

            # Verificar tipo de arquivo e calcular taxa de compressão
            if original_size > 0:
                compression_rate = 100 * (1 - (compressed_size / original_size))  # Taxa de compressão em %

                if file_name.endswith('.txt'):
                    compression_data['txt'].append(compression_rate)
                elif file_name.endswith('.bmp'):
                    compression_data['bmp'].append(compression_rate)
                elif file_name.endswith('.wav'):
                    compression_data['wav'].append(compression_rate)

    return compression_data

def plot_compression_boxplot(csv_file, file_extensions):
    compression_data = calculate_compression_data(csv_file, file_extensions)
    
    # Verificar se há dados para cada tipo de arquivo
    if not any(compression_data.values()):
        print(f"Nenhum dado encontrado para os tipos de arquivo solicitados em {csv_file}")
        return
    
    # Criando o gráfico de boxplot
    plt.figure(figsize=(8, 6))
    plt.boxplot([compression_data['txt'], compression_data['bmp'], compression_data['wav']], 
                vert=True, patch_artist=True, 
                labels=['.txt', '.bmp', '.wav'],
                boxprops=dict(facecolor='lightblue', color='lightblue'),
                medianprops=dict(color='blue'))
    
    # Título e rótulos
    plt.title(f"Boxplot: Taxa de Compressão por Tipo de Arquivo\n({os.path.basename(csv_file)})")
    plt.ylabel("Taxa de Compressão (%)")
    plt.grid(True, linestyle='--', alpha=0.6)

    # Salvando o gráfico
    output_image = f"{os.path.splitext(csv_file)[0]}_boxplot.png"
    plt.tight_layout()
    plt.savefig(output_image)
    plt.close()

    print(f"Boxplot salvo: {output_image}")

def generate_all_boxplots(csv_folder='.'):
    for file in os.listdir(csv_folder):
        if file.endswith('.csv') and file.startswith('file_sizes'):
            plot_compression_boxplot(os.path.join(csv_folder, file), ['txt', 'bmp', 'wav'])

# Rodar a função para gerar os boxplots
generate_all_boxplots()
