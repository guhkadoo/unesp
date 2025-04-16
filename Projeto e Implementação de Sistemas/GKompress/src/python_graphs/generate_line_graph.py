import os
import csv
import matplotlib.pyplot as plt

def calculate_compression_data(csv_file, file_extension):
    # Lista para armazenar tuplas de (tamanho_em_KB, taxa_de_compressão)
    compression_data = []

    with open(csv_file, newline='') as file:
        reader = csv.DictReader(file)
        for row in reader:
            original_size = int(row["Original Size (bytes)"])
            compressed_size = int(row["Compressed Size (bytes)"])
            file_name = row["File Name"]

            if file_name.endswith(file_extension) and original_size > 0:
                compression_rate = 100 * (1 - (compressed_size / original_size))  # Taxa de compressão em %
                size_in_kb = original_size / 1024  # Convertendo para KB
                compression_data.append((size_in_kb, compression_rate))

    return compression_data

def plot_compression_line(csv_file, file_extension):
    data = calculate_compression_data(csv_file, file_extension)
    
    if not data:
        print(f"Nenhum arquivo '{file_extension}' encontrado em {csv_file}")
        return
    
    # Separando tamanho e taxa de compressão
    sizes_in_kb = [entry[0] for entry in data]
    compression_rates = [entry[1] for entry in data]

    plt.figure(figsize=(8, 6))
    plt.plot(sizes_in_kb, compression_rates, marker='o', color='mediumseagreen', linestyle='-', alpha=0.6)

    # Adicionando título e rótulos
    plt.title(f"Gráfico de Linha: Tamanho do Arquivo vs Taxa de Compressão\n({file_extension.upper()})")
    plt.xlabel("Tamanho do Arquivo (KB)")
    plt.ylabel("Taxa de Compressão (%)")
    plt.grid(True, linestyle='--', alpha=0.6)

    # Salvando o gráfico
    output_image = f"{os.path.splitext(csv_file)[0]}_{file_extension}_line_plot.png"
    plt.tight_layout()
    plt.savefig(output_image)
    plt.close()

    print(f"Gráfico de linha salvo: {output_image}")

def generate_all_line_plots(csv_folder='.'):
    for file in os.listdir(csv_folder):
        if file.endswith('.csv') and file.startswith('file_sizes'):
            for extension in ['txt', 'wav', 'bmp']:
                plot_compression_line(os.path.join(csv_folder, file), extension)

# Rodar a função para gerar os gráficos de linha
generate_all_line_plots()
