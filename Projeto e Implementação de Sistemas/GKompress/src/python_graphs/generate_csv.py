import os
import csv

def generate_file_sizes_csv(directory_path, output_csv):
    with open(output_csv, mode='w', newline='') as file:
        writer = csv.writer(file)
        
        writer.writerow(["File Name", "Original Size (bytes)", "Compressed Size (bytes)"])

        for file_name in os.listdir(directory_path):
            file_path = os.path.join(directory_path, file_name)

            if os.path.isfile(file_path):
                if file_name.endswith('.bmp') or file_name.endswith('.wav') or file_name.endswith('.txt'):
                    original_size = os.path.getsize(file_path)

                    compressed_file_name = file_name.replace('decompressed', '') + '.GK' if not file_name.endswith('.GK') else file_name
                    compressed_file_path = os.path.join(directory_path, compressed_file_name)

                    if os.path.isfile(compressed_file_path):
                        compressed_size = os.path.getsize(compressed_file_path)
                        writer.writerow([file_name, original_size, compressed_size])
                    else:
                        print(f'Compressed file not found for {file_name}')
        print(f"CSV file '{output_csv}' has been generated.")


def generate_csv_for_all_directories(root_directory, base_csv_name):
    for dirpath, dirnames, filenames in os.walk(root_directory):
        if dirpath == root_directory:
            continue
        dir_name = os.path.basename(dirpath)
        output_csv = f"{base_csv_name}_{dir_name}.csv"

        generate_file_sizes_csv(dirpath, output_csv)


root_directory = "../../data/output"
base_csv_name = "file_sizes"

generate_csv_for_all_directories(root_directory, base_csv_name)