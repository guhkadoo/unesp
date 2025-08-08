#include "../../include/lzw/lzw.hpp"
#include <unistd.h>
#include <iostream>

int LZW::set_filepath(const std::string& str) {
    filepath = str;
    FILE* file = fopen(filepath.c_str(), "rb");
    if (file) {
        fclose(file);
        return 0;
    }
    return -1;
}

std::string LZW::get_filepath() {
    return filepath;
}

std::string LZW::get_compressed_filepath(int option) {
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) == NULL) EXIT_WITH_ERROR("getcwd() error");
    std::string filename = cwd;
    filename.append("/data/output/");
    std::string aux_str = filepath.substr(filepath.find_last_of("/\\") + 1);
    size_t dot = aux_str.find_last_of(".");
    aux_str = aux_str.substr(0, dot) + "_lzw" + std::to_string(option) + aux_str.substr(dot) + ".GK";
    filename.append(aux_str);
    return filename;
}

std::string LZW::get_decompressed_filepath(int option) {
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) == NULL) EXIT_WITH_ERROR("getcwd() error");
    std::string filename = cwd;
    filename.append("/data/output/");
    std::string aux_str = filepath.substr(filepath.find_last_of("/\\") + 1);
    size_t dot = aux_str.find_last_of(".");
    aux_str = aux_str.substr(0, dot) + "_lzw" + std::to_string(option) + "decompressed" + aux_str.substr(dot);
    filename.append(aux_str);
    return filename;
}

std::string LZW::decompressed_filepath_from_compressed(const std::string& compressed_path) {
    std::string exts[] = {".txt.GK", ".wav.GK", ".bmp.GK"};
    std::string replacements[] = {"decompressed.txt", "decompressed.wav", "decompressed.bmp"};
    for (size_t i = 0; i < 3; ++i) {
        size_t pos = compressed_path.find(exts[i]);
        if (pos != std::string::npos) {
            return compressed_path.substr(0, pos) + replacements[i];
        }
    }
    return compressed_path;
}

void LZW::BitWriter::writeBit(bool bit) {
    buffer = (buffer << 1) | (bit ? 1 : 0);
    bitCount++;
    if (bitCount == 8) flush();
}

void LZW::BitWriter::writeBits(uint32_t value, int bits) {
    std::string bit_string;
    for (int i = bits - 1; i >= 0; i--) {
        bool bit = (value >> i) & 1;
        bit_string += bit ? "1" : "0";
        writeBit(bit);
    }
    std::cout << "Writing code=" << value << " with " << bits << " bits (" << bit_string << ")" << std::endl;
}

void LZW::BitWriter::flush() {
    while (bitCount >= 8) {
        uint8_t byte = (buffer >> (bitCount - 8)) & 0xFF;
        fwrite(&byte, 1, 1, file);
        buffer &= (1 << bitCount) - 1;
        bitCount -= 8;
    }
}

void LZW::BitWriter::finalize() {
    if (bitCount > 0) {
        buffer <<= (8 - bitCount);
        uint8_t byte = buffer & 0xFF;
        fwrite(&byte, 1, 1, file);
        std::cout << "Finalizing: wrote " << bitCount << " bits, byte=" << (int)byte << std::endl;
        buffer = 0;
        bitCount = 0;
    }
}

bool LZW::BitReader::readBit() {
    if (bitCount == 0) {
        uint8_t byte;
        if (fread(&byte, 1, 1, file) != 1) {
            if (feof(file)) {
                std::cout << "Reached EOF in readBit" << std::endl;
                return false;
            }
            EXIT_WITH_ERROR("Error reading byte in BitReader");
        }
        buffer = byte;
        bitCount = 8;
        std::cout << "Read byte: " << (int)byte << std::endl;
    }
    bool bit = (buffer >> (bitCount - 1)) & 1;
    bitCount--;
    return bit;
}

uint32_t LZW::BitReader::readBits(int bits) {
    uint32_t value = 0;
    std::string bit_string;
    for (int i = 0; i < bits; i++) {
        bool bit = readBit();
        bit_string += bit ? "1" : "0";
        value = (value << 1) | (bit ? 1 : 0);
    }
    std::cout << "Read code=" << value << " with " << bits << " bits (" << bit_string << ")" << std::endl;
    return value;
}

std::vector<std::pair<uint16_t, int>> LZW::encode(const uint8_t* data, size_t size) {
    compress_dict.clear();
    for (uint16_t i = 0; i < INITIAL_DICT_SIZE; ++i) {
        compress_dict[std::string(1, static_cast<char>(i))] = i;
    }
    
    std::vector<std::pair<uint16_t, int>> codes;
    std::string current;
    uint16_t next_code = INITIAL_DICT_SIZE;
    int current_bit_width = INITIAL_CODE_WIDTH;

    for (size_t i = 0; i < size; ++i) {
        current += static_cast<char>(data[i]);
        
        if (compress_dict.find(current) == compress_dict.end()) {
            codes.emplace_back(compress_dict[current.substr(0, current.size()-1)], current_bit_width);
            std::cout << "Encode: code=" << compress_dict[current.substr(0, current.size()-1)] 
                      << ", bit_width=" << current_bit_width << ", next_code=" << next_code << std::endl;
            
            if (next_code >= MAX_DICT_SIZE) { // Reset before assigning 4095
                printf("\n\n\nAQUIII\n\n\nRESET_CODE: %ld\n", RESET_CODE);
                codes.emplace_back(RESET_CODE, RESET_CODE_WIDTH);
                compress_dict.clear();
                for (uint16_t j = 0; j < INITIAL_DICT_SIZE; ++j) {
                    compress_dict[std::string(1, static_cast<char>(j))] = j;
                }
                next_code = INITIAL_DICT_SIZE;
                current_bit_width = INITIAL_CODE_WIDTH;
                continue;
            }
            
            compress_dict[current] = next_code++;
            if (next_code > (1 << current_bit_width)) {
                current_bit_width++;
            }
            
            current = current.substr(current.size()-1);
        }
    }
    
    if (!current.empty()) {
        codes.emplace_back(compress_dict[current], current_bit_width);
    }

    printf("current: %s\n", current);
    
    return codes;
}

std::vector<uint8_t> LZW::decode(const std::vector<std::pair<uint16_t, int>>& codes) {
    decompress_dict.clear();
    for (uint16_t i = 0; i < INITIAL_DICT_SIZE; ++i) {
        decompress_dict[i] = std::string(1, static_cast<char>(i));
    }
    
    std::vector<uint8_t> output;
    uint16_t next_code = INITIAL_DICT_SIZE;
    int current_bit_width = INITIAL_CODE_WIDTH;
    std::string previous;

    for (size_t i = 0; i < codes.size(); ++i) {
        const auto& [code, bit_width] = codes[i];
        std::cout << "Decode: code=" << code << ", bit_width=" << bit_width << ", next_code=" << next_code << std::endl;
        if (code == RESET_CODE) {
            decompress_dict.clear();
            for (uint16_t j = 0; j < INITIAL_DICT_SIZE; ++j) {
                decompress_dict[j] = std::string(1, static_cast<char>(j));
            }
            printf("\n\n\n\noiiii\n\n\n\n");
            next_code = INITIAL_DICT_SIZE;
            current_bit_width = INITIAL_CODE_WIDTH;
            previous.clear();
            continue;
        }
        
        std::string entry;
        if (decompress_dict.count(code)) {
            entry = decompress_dict[code];
        } else if (code == next_code && !previous.empty()) {
            entry = previous + previous[0];
        } else {
            std::cout << "Invalid LZW code at index " << i << ": code=" << code 
                      << ", next_code=" << next_code << ", dict_size=" << decompress_dict.size() 
                      << ", previous=" << previous << std::endl;
            EXIT_WITH_ERROR("Invalid LZW code");
        }
        
        output.insert(output.end(), entry.begin(), entry.end());
        
        if (!previous.empty() && next_code < MAX_DICT_SIZE) {
            decompress_dict[next_code++] = previous + entry[0];
            if (next_code > (1 << current_bit_width)) {
                current_bit_width++;
            }
        }
        
        previous = entry;
    }
    
    return output;
}

void LZW::internal_compress(int option, const std::vector<std::pair<uint16_t, int>>& codes, void (*write_header)(FILE*, void*), void* filetype) {
    std::string outputFile = get_compressed_filepath(option);
    FILE* out = fopen(outputFile.c_str(), "wb");
    if (!out) EXIT_WITH_ERROR("Error opening compressed file");

    fwrite("GK", 1, 2, out);
    uint8_t version = 1;
    fwrite(&version, 1, 1, out);
    uint32_t origSize = codes.size();
    fwrite(&origSize, 4, 1, out);
    uint8_t initial_bit_width = INITIAL_CODE_WIDTH;
    fwrite(&initial_bit_width, 1, 1, out);
    std::cout << "Writing header: numCodes=" << origSize << ", initial_bit_width=" << (int)initial_bit_width << std::endl;

    if (write_header && filetype) {
        write_header(out, filetype);
    }

    BitWriter writer(out);
    for (const auto& [code, bit_width] : codes) {
        std::cout << "Compressing: code=" << code << ", bit_width=" << bit_width << std::endl;
        writer.writeBits(code, bit_width);
    }
    writer.finalize();
    fclose(out);
}

int LZW::internal_decompress(int option, void (*write_header)(FILE*, void*), void (*read_header)(FILE*, void*), size_t (*get_pos)(void*), void* filetype) {
    std::string compressed_filepath = (option == 0) ? get_compressed_filepath(option) : filepath;
    std::string outputFile = decompressed_filepath_from_compressed(compressed_filepath);
    FILE* in = fopen(compressed_filepath.c_str(), "rb");
    if (!in) return -1;

    char magic[2];
    if (fread(magic, 1, 2, in) != 2 || magic[0] != 'G' || magic[1] != 'K') {
        fclose(in);
        return -1;
    }
    uint8_t version;
    if (fread(&version, 1, 1, in) != 1) {
        fclose(in);
        return -1;
    }
    uint32_t numCodes;
    if (fread(&numCodes, 4, 1, in) != 1) {
        fclose(in);
        return -1;
    }
    uint8_t initial_bit_width;
    if (fread(&initial_bit_width, 1, 1, in) != 1) {
        fclose(in);
        return -1;
    }
    std::cout << "Reading header: numCodes=" << numCodes << ", initial_bit_width=" << (int)initial_bit_width << std::endl;

    if (read_header && filetype) {
        long startPos = ftell(in);
        read_header(in, filetype);
        long headerSize = get_pos(filetype);
        long currentPos = ftell(in);
        if (currentPos != startPos + headerSize) {
            std::cout << "Header misalignment: expected pos=" << startPos + headerSize 
                      << ", actual pos=" << currentPos << std::endl;
            fseek(in, startPos + headerSize, SEEK_SET);
        }
    }

    BitReader reader(in, initial_bit_width);
    std::vector<std::pair<uint16_t, int>> codes;
    codes.reserve(numCodes);
    int current_bit_width = initial_bit_width;
    uint16_t next_code = INITIAL_DICT_SIZE;

    for (uint32_t i = 0; i < numCodes && !feof(in); ++i) {
        uint16_t code = reader.readBits(current_bit_width);
        std::cout << "Decompress: code=" << code << ", bit_width=" << current_bit_width 
                  << ", next_code=" << next_code << std::endl;
        if (code == RESET_CODE) {
            printf("aquii\n");
            current_bit_width = initial_bit_width;
            reader.setCodeWidth(current_bit_width);
            next_code = INITIAL_DICT_SIZE + 1;
          //  codes.emplace_back(code, current_bit_width);
            continue;
        }
        if (next_code < MAX_DICT_SIZE) {
            next_code++;
            if (next_code >= MAX_DICT_SIZE) {
                current_bit_width = RESET_CODE_WIDTH; // Set to 12 for RESET_CODE
            } else if (next_code > (1U << current_bit_width)) {
                current_bit_width++;
                reader.setCodeWidth(current_bit_width);
                std::cout << "Increasing bit_width to " << current_bit_width << " at next_code=" << next_code << std::endl;
            }
        }
        codes.emplace_back(code, current_bit_width);
    }

    std::vector<uint8_t> buffer = decode(codes);
    FILE* out = fopen(outputFile.c_str(), "wb");
    if (!out) {
        fclose(in);
        return -1;
    }
    if (write_header && filetype) {
        write_header(out, filetype);
    }
    fwrite(buffer.data(), 1, buffer.size(), out);
    fclose(out);
    fclose(in);
    return 0;
}