import os
import csv
import matplotlib.pyplot as plt
from collections import defaultdict

def calculate_compression_rates(csv_file):
    compression_data = defaultdict(list)

    with open(csv_file, newline='') as file:
        reader = csv.DictReader(file)
        for row in reader:
            original_size = int(row["Original Size (bytes)"])
            compressed_size = int(row["Compressed Size (bytes)"])
            file_name = row["File Name"]

            if original_size > 0:
                compression_rate = compressed_size / original_size
                if file_name.endswith('.wav'):
                    compression_data['wav'].append(compression_rate)
                elif file_name.endswith('.bmp'):
                    compression_data['bmp'].append(compression_rate)
                elif file_name.endswith('.txt'):
                    compression_data['txt'].append(compression_rate)

    return compression_data

def plot_compression_bar_graph(csv_file):
    data = calculate_compression_rates(csv_file)

    file_types = ['wav', 'bmp', 'txt']
    avg_rates = []

    for filetype in file_types:
        rates = data.get(filetype, [])
        avg_rate = sum(rates) / len(rates) if rates else 0
        avg_rates.append(avg_rate)

    plt.figure(figsize=(6, 5))
    bars = plt.bar(file_types, avg_rates, color=['skyblue', 'lightcoral', 'mediumseagreen'])

    for bar, rate in zip(bars, avg_rates):
        plt.text(bar.get_x() + bar.get_width()/2, bar.get_height() + 0.02, f"{rate:.2f}", ha='center', fontsize=10)

    plt.ylim(0, 1.2)
    plt.title(f"Average Compression Rates\n({os.path.basename(csv_file)})")
    plt.ylabel("Compression Rate (Compressed / Original)")
    plt.xlabel("File Type")
    plt.grid(axis='y', linestyle='--', alpha=0.7)

    output_image = f"{os.path.splitext(csv_file)[0]}_compression.png"
    plt.tight_layout()
    plt.savefig(output_image)
    plt.close()
    print(f"Saved graph: {output_image}")

def generate_all_graphs_from_csvs(csv_folder='.'):
    for file in os.listdir(csv_folder):
        if file.endswith('.csv') and file.startswith('file_sizes'):
            plot_compression_bar_graph(os.path.join(csv_folder, file))

generate_all_graphs_from_csvs()
