#include "lzw/lzw.hpp"
#include <unistd.h>

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
    for (int i = bits - 1; i >= 0; i--)
        writeBit((value >> i) & 1);
}

void LZW::BitWriter::flush() {
    while (bitCount >= 8) {
        uint8_t byte = (buffer >> (bitCount - 8)) & 0xFF;
        fwrite(&byte, 1, 1, file);
        bitCount -= 8;
        buffer &= (1 << bitCount) - 1;
    }
}

void LZW::BitWriter::finalize() {
    if (bitCount > 0) {
        buffer <<= (8 - bitCount);
        fwrite(&buffer, 1, 1, file);
    }
}

bool LZW::BitReader::readBit() {
    if (bitCount == 0) {
        uint8_t byte;
        if (fread(&byte, 1, 1, file) != 1) return false;
        buffer = byte;
        bitCount = 8;
    }
    bool bit = (buffer >> (bitCount - 1)) & 1;
    bitCount--;
    return bit;
}

uint32_t LZW::BitReader::readBits(int bits) {
    uint32_t value = 0;
    for (int i = 0; i < bits; i++) {
        bool bit = readBit();
        if (!bit && feof(file)) EXIT_WITH_ERROR("Unexpected EOF in BitReader");
        value = (value << 1) | (bit ? 1 : 0);
    }
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
            std::string prefix = current.substr(0, current.size() - 1);
            codes.emplace_back(compress_dict[prefix], current_bit_width);
            if (compress_dict.size() < MAX_DICT_SIZE) {
                compress_dict[current] = next_code++;
                // Update bit width based on next_code
                if (next_code > (1U << current_bit_width)) {
                    current_bit_width++;
                }
            }
            current = current.back();
        }
    }
    if (!current.empty()) {
        codes.emplace_back(compress_dict[current], current_bit_width);
    }
    printf("Input size: %zu bytes, Output codes: %zu, Final dictionary size: %zu\n", size, codes.size(), compress_dict.size());
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

    if (codes.empty()) return output;
    std::string current = decompress_dict[codes[0].first];
    output.insert(output.end(), current.begin(), current.end());

    for (size_t i = 1; i < codes.size(); ++i) {
        uint16_t code = codes[i].first;
        if (code > (1U << codes[i].second)) {
            EXIT_WITH_ERROR("Invalid LZW code");
        }
        std::string entry;
        if (decompress_dict.find(code) != decompress_dict.end()) {
            entry = decompress_dict[code];
        } else if (code == next_code) {
            entry = current + current[0];
        } else {
            EXIT_WITH_ERROR("Invalid LZW code");
        }
        output.insert(output.end(), entry.begin(), entry.end());
        if (decompress_dict.size() < MAX_DICT_SIZE) {
            decompress_dict[next_code++] = current + entry[0];
            if (next_code > (1U << current_bit_width)) {
                current_bit_width++;
            }
        }
        current = entry;
    }
    return output;
}

void LZW::internal_compress(int option, const std::vector<std::pair<uint16_t, int>>& codes, void (*write_header)(FILE*, void*), void* filetype) {
    std::string outputFile = get_compressed_filepath(option);
    FILE* out = fopen(outputFile.c_str(), "wb");
    if (!out) EXIT_WITH_ERROR("Error opening compressed file");

    // Write header
    fwrite("GK", 1, 2, out);
    uint8_t version = 1;
    fwrite(&version, 1, 1, out);
    uint32_t origSize = codes.size();
    fwrite(&origSize, 4, 1, out);
    uint8_t initial_bit_width = INITIAL_CODE_WIDTH;
    fwrite(&initial_bit_width, 1, 1, out); // Use reserved byte for initial bit width

    if (write_header && filetype) {
        write_header(out, filetype);
    }

    BitWriter writer(out);
    for (const auto& [code, bit_width] : codes) {
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
    fread(magic, 1, 2, in);
    if (magic[0] != 'G' || magic[1] != 'K') {
        fclose(in);
        return -1;
    }
    uint8_t version;
    fread(&version, 1, 1, in);
    uint32_t numCodes;
    fread(&numCodes, 4, 1, in);
    uint8_t initial_bit_width;
    fread(&initial_bit_width, 1, 1, in); // Read initial bit width

    size_t headerSize = 0;
    if (read_header && filetype) {
        long startPos = ftell(in);
        read_header(in, filetype);
        headerSize = ftell(in) - startPos;
    }

    BitReader reader(in, initial_bit_width);
    std::vector<std::pair<uint16_t, int>> codes;
    codes.reserve(numCodes);
    int current_bit_width = initial_bit_width;
    uint16_t next_code = INITIAL_DICT_SIZE;

    for (uint32_t i = 0; i < numCodes; ++i) {
        uint16_t code = reader.readBits(current_bit_width);
        codes.emplace_back(code, current_bit_width);
        if (next_code < MAX_DICT_SIZE) {
            next_code++;
            if (next_code > (1U << current_bit_width)) {
                current_bit_width++;
                reader.setCodeWidth(current_bit_width);
            }
        }
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