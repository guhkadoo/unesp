#include "../include/huffman/huffman.hpp"
#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <cstring>
#include <string>
#include <unistd.h>

#define EXIT_WITH_ERROR(msg) (fprintf(stderr, "%s\n", msg), exit(1))

//we're counting the duplicates of a string using an array which countains all the ascii (A to Z upper or lowercase).

int Huffman::set_filepath(std::string str)
{
    filepath = str; 
    FILE *file = fopen(filepath.c_str(), "r");
    if(file)
    {
        fclose(file);
        return 0;
    }
    else
        return -1;
}

std::string Huffman::get_filepath()
{
    return filepath;
}

void Huffman::count_duplicates(uint8_t* data, size_t size)
{
    for(size_t i=0; i<size; i++) {
        duplicates[data[i]]++;
    }
}

void Huffman::print_duplicates()
{
    for(size_t i=0; i<SIZE; i++) {
        if(duplicates[i] != 0) {
            printf("%c (%x) has %u duplicates\n", i, i, duplicates[i]);
        }
    }
}

void Huffman::List::clear()
{
    Node* remove; 
    while(front != nullptr)
    {
        remove = front;
        front = front->next;
        delete remove;
    }
    front = nullptr;
}

void Huffman::List::insert_sorted(Node* node)
{
    if(front == nullptr) {
        front = node;
    } else if(node->duplicates < front->duplicates) {
        node->next = front;
        front = node;
    } else {
        Node* aux = front;
        while(aux->next && aux->next->duplicates < node->duplicates)
            aux = aux->next;
        node->next = aux->next;
        aux->next = node;
    }
    size++;
}

void Huffman::List::fill(std::array<uint32_t, Huffman::SIZE>& duplicates)
{
    for(size_t i=0; i<Huffman::SIZE; i++) {
        if(duplicates[i] != 0) {
            Node* new_node = new Node(static_cast<uint8_t>(i), duplicates[i]); 
            insert_sorted(new_node);
        }
    }
}

void Huffman::List::print()
{
    printf("Sorted list of size %d:\n", size);
    Node* aux = front;
    while(aux) {
        printf("character: %c (%x)    duplicates: %u\n", aux->byte, aux->byte, aux->duplicates);
        aux = aux->next;
    }
    printf("\n");
}


Huffman::Node* Huffman::List::remove_front()
{
    Node* remove = nullptr;
    if(front != nullptr) {
        remove = front;
        front = remove->next;
        remove->next = nullptr;
        size--;
    }
    return remove;
}

void Huffman::HuffmanTree::build(List& list)
{
    Node *first_removed, *second_removed, *new_node;
    while(list.size > 1) {
        first_removed = list.remove_front();
        second_removed = list.remove_front();
        new_node = new Node('*', first_removed->duplicates+second_removed->duplicates); 
        new_node->left = first_removed;
        new_node->right = second_removed;
        new_node->next = nullptr;

        list.insert_sorted(new_node);
    }
    root = list.front;
}

void Huffman::HuffmanTree::print()
{
    print_with_argument(root);
}

void Huffman::HuffmanTree::print_with_argument(Node* root)
{
    static int height = 0;
    if(root->left == nullptr && root->right == nullptr) {
        printf("character: %c    duplicates: %u    height_tree: %d\n", root->byte, root->duplicates, height);
    } else {
        height++;
        print_with_argument(root->left);
        print_with_argument(root->right);
        height--;
    }
}


void Huffman::HuffmanTree::clear(Node* node)
{
    if(node == nullptr) return;
    clear(node->left);
    clear(node->right);
    delete node;
}

Huffman::HuffmanTree::~HuffmanTree()
{
    clear(root);
}

int Huffman::HuffmanTree::height()
{
    return height_with_argument(root);
}

int Huffman::HuffmanTree::height_with_argument(Node* root)
{
    if(root == nullptr)
        return -1;
    int left = height_with_argument(root->left);
    int right = height_with_argument(root->right);
    return (right > left) ? (right + 1) : (left + 1);
}

void Huffman::Dictionary::build(size_t j)
{
    dictionary = new char*[Huffman::SIZE];
    for(size_t i=0; i<Huffman::SIZE; i++)
        dictionary[i] = new char[j+1]();
}

void Huffman::Dictionary::fill(Node* tree_root, char* code, size_t columns, int option)
{
    char left[columns+1], right[columns+1];
    if(tree_root->left == nullptr && tree_root->right == nullptr) {
        dictionary[tree_root->byte] = strdup(code);
        if(option == 2)
            sorted_symbols.push_back({tree_root->byte, strlen(code)});
    } else {
        strcpy(left, code);
        strcpy(right, code);

        strcat(left, "0");
        strcat(right, "1");

        fill(tree_root->left, left, columns, option);
        fill(tree_root->right, right, columns, option);
    }
}

void Huffman::Dictionary::print()
{
    for(size_t i=0; i<Huffman::SIZE; i++) {
        if(strcmp(dictionary[i], "")) {
            printf("%c/%d: %s\n", i, i, dictionary[i]);
        }
    }
}

char*& Huffman::Dictionary::operator[](size_t index) {
    return dictionary[index];
}

void Huffman::Dictionary::generate_canonical_codes()
{
    std::map<uint8_t, std::string> canonical_codes;
    std::sort(sorted_symbols.begin(), sorted_symbols.end(),
                [](const std::pair<uint8_t, size_t>& a, const std::pair<uint8_t, size_t>& b) {
                    return (a.second < b.second) || (a.second == b.second && a.first < b.first);
                 }
             );

    //for (const auto& pair : sorted_symbols) {
    //    std::cout << "{ Symbol: " << static_cast<int>(pair.first) // Cast to int for readability
    //              << ", Code Length: " << pair.second << " }" << std::endl;
    //}

    uint32_t code = 0;
    size_t prev_length = 0;

    for(auto& [symbol, length] : sorted_symbols) {
        if(length > prev_length) {
            code <<= (length - prev_length);
            prev_length = length;
        }
        canonical_codes[symbol] = std::bitset<32>(code).to_string().substr(32 - length);
    //    printf("%s\n", canonical_codes[symbol].c_str());
        code++;
    }
    
    for(const auto& [symbol, can_code] : canonical_codes) {
        if(dictionary == nullptr)
            build(sorted_symbols.back().second);
        //else {
        //    clear();
        //    build(sorted_symbols.back().second);
        //}
        dictionary[symbol] = strdup(can_code.c_str());
    }
}

void Huffman::Dictionary::write(FILE* file)
{
    uint16_t num_symbols = static_cast<uint16_t>(sorted_symbols.size());
    fwrite(&num_symbols, sizeof(uint16_t), 1, file);
    //printf("\nSORTED_SYMBOLS.SIZE(): %d\nNUM_SYMBOLS: %d\n", num_symbols, sorted_symbols.size());
    for(const auto& [symbol, code_length] : sorted_symbols)
    {
        uint8_t length = static_cast<uint8_t>(code_length);
        fwrite(&symbol, sizeof(uint8_t), 1, file);
        fwrite(&length, sizeof(uint8_t), 1, file);
        //printf("symbol: %d\ncode_length: %d\n", symbol, code_length);
    }
}

void Huffman::Dictionary::read(FILE* file)
{
    uint16_t num_symbols;
    fread(&num_symbols, sizeof(uint16_t), 1, file);
    //printf("num_symbols: %d\n", num_symbols);

    sorted_symbols.clear();
    sorted_symbols.reserve(num_symbols);

    for(uint16_t i=0; i<num_symbols; i++)
    {
        uint8_t symbol, code_length;
        fread(&symbol, sizeof(uint8_t), 1, file);
        fread(&code_length, sizeof(uint8_t), 1, file);
        //printf("symbol: %d\ncode_length: %d\n", symbol, code_length);

        sorted_symbols.emplace_back(symbol, code_length);
    }
}

std::map<std::string, uint8_t> Huffman::Dictionary::get_reverse_map() const {
    std::map<std::string, uint8_t> reverse_dict;
    for (int i = 0; i < 256; ++i) {
        if (dictionary[i]) {
            reverse_dict[dictionary[i]] = static_cast<uint8_t>(i);
        }
    }
    return reverse_dict;
}

void Huffman::Dictionary::clear()
{
    for(size_t i=0; i<Huffman::SIZE; i++)
        delete[] dictionary[i];
    delete[] dictionary;
    dictionary = nullptr;
}

Huffman::Dictionary::~Dictionary()
{
    clear();
}

static inline char *strcat_endpointer(char *dest, char *src)
{
    while(*dest) dest++;
    while(*dest++ = *src++);
    return --dest;
}

char* Huffman::encode(uint8_t* data, size_t data_size)
{
    size_t i, size=0;
    for(i=0; i<data_size; i++) {
        size += strlen(dictionary[data[i]]);
    }

    char* code = new char[size](); 
    char* ptr = code;
    for(i=0; i<data_size; i++) {
        ptr = strcat_endpointer(ptr, dictionary[data[i]]); 
    }

    return code;
}

static std::string get_filename_with_ext(std::string filepath)
{
    size_t last_slash = filepath.find_last_of("/\\");
    return filepath.substr(last_slash + 1);
}

static std::string get_filename(std::string filepath)
{
    size_t last_slash = filepath.find_last_of("/\\");
    std::string filename_w_ext = filepath.substr(last_slash + 1);
    size_t dot = filename_w_ext.find_last_of(".");
    return filename_w_ext.substr(0, dot);
}

static std::string get_extension(std::string filepath)
{
    size_t last_slash = filepath.find_last_of("/\\");
    std::string filename_w_ext = filepath.substr(last_slash + 1);
    size_t dot = filename_w_ext.find_last_of(".");
    return filename_w_ext.substr(dot);
}

static std::string get_decompressed_filepath(std::string filepath, int option)
{
    char cwd[1024];
    if(getcwd(cwd, sizeof(cwd)) == NULL)
        EXIT_WITH_ERROR("getcwd() error");
    std::string filename = cwd;
    filename.append("/data/output/");
    std::string aux_str = get_filename(filepath);
    aux_str += "_huffman";
    aux_str += option + '0';
    aux_str += "decompressed";
    aux_str.append(get_extension(filepath));
    filename.append(aux_str);
    
    return filename;
}

static std::string get_compressed_filepath(std::string filepath, int option)
{
    char cwd[1024];
    if(getcwd(cwd, sizeof(cwd)) == NULL)
        EXIT_WITH_ERROR("getcwd() error");
    std::string filename = cwd;
    filename.append("/data/output/");

    std::string aux_str = get_filename(filepath);
    aux_str += "_huffman";
    aux_str += option + '0';
    std::string format = get_extension(filepath);
    format += ".GK";
    aux_str.append(format);

    filename.append(aux_str);
    return filename;
}

int Huffman::Node::is_leaf()
{
    if(left == nullptr && right == nullptr)
        return 1;
    return 0;
}

void Huffman::HuffmanTree::write(FILE* file, Node* node)
{
    char marker;
    if(node->is_leaf())
    {
        marker = '1';
        fwrite(&marker, sizeof(char), 1, file);
        fwrite(&node->byte, sizeof(char), 1, file);
    } else {
        marker = '0';
        fwrite(&marker, sizeof(char), 1, file);
        write(file, node->left);
        write(file, node->right);
    }
}

Huffman::Node* Huffman::HuffmanTree::read(FILE* file)
{
    char marker;
    fread(&marker, sizeof(char), 1, file);
    
    if(marker == '1') // leaf
    {
        uint8_t byte;
        fread(&byte, sizeof(char), 1, file);
        return new Node(byte, 0); // duplicates 0 bcz they dont interest us
    }
    Node* node = new Node('\0', 0);
    node->left = read(file);
    node->right = read(file);
    return node;
}

void Huffman::internal_compress(int option, char* code, void (*write_header)(FILE*, void*), void* filetype)
{
    std::string compressed_filepath = get_compressed_filepath(filepath, option);

    FILE *file = fopen(compressed_filepath.c_str(), "wb");
    char byte = 0;
    if(file != NULL) {
        // WRITE TREE
        if(option == 1) // tree on compressed file 
            tree.write(file, tree.root);
        else if(option == 2) // adaptive
            dictionary.write(file);
        // END WRITE TREE

        if(write_header && filetype)
            write_header(file, filetype);
        int j=7;
        for(size_t i=0; code[i] != '\0'; i++) {
            if(code[i] == '1')
                byte |= (1U << j); 
            j--;
            if(j < 0) {
                fwrite(&byte, sizeof(char), 1, file);
                byte = 0;
                j = 7;
            }
        }
        if(j != 7) {
            fwrite(&byte, sizeof(char), 1, file);
            byte = 7-j; // we will add a last byte to the final because we need to know where to stop if the byte is not multiple of 8
                        // as a consequence, we write the number of elements to be read (byte = 7-j)
            fwrite(&byte, sizeof(char), 1, file);
        } else {
            byte = 7; 
            fwrite(&byte, sizeof(char), 1, file);
        }
        if(option != 0)
            clean_for_next_iteration();
        fclose(file);
    } else {
        EXIT_WITH_ERROR("Error while opening/creating archive in void compress(char *str)");
    }
}

static int is_bit_1(char byte, int i)
{
    return byte &= (1U << i);
}

void Huffman::clean_for_next_iteration()
{
    duplicates.fill(0); // we fill the duplicates with 0 for the next iteration
    tree.clear(tree.root); // we clear the tree for the next iteration
    tree.root = nullptr;
    list.front = nullptr; // we set the front of the list to nullptr for the next iteration 
    list.size = 0; // we set list size to 0 for the next iteration
}


static std::string decompressed_filepath_from_compressed(std::string compressed_path)
{
    size_t pos = compressed_path.find(".txt.GK");
    if (pos != std::string::npos) {
        return compressed_path.substr(0, pos) + "decompressed.txt";
    }

    pos = compressed_path.find(".wav.GK");
    if (pos != std::string::npos) {
        return compressed_path.substr(0, pos) + "decompressed.wav";
    }

    pos = compressed_path.find(".bmp.GK");
    if (pos != std::string::npos) {
        return compressed_path.substr(0, pos) + "decompressed.bmp";
    }

    return compressed_path; // If no known pattern is found, return the original path
}

int Huffman::decompress(int option, void (*write_header)(FILE*, void*), void (*read_header)(FILE*, void*), size_t (*get_pos)(void*), void* filetype)
{
    std::string decompressed_filepath = get_decompressed_filepath(filepath, option);
    std::string compressed_filepath = get_compressed_filepath(filepath, option);
    if(option != 0)
    {
        compressed_filepath = filepath;
        decompressed_filepath = decompressed_filepath_from_compressed(compressed_filepath); 
    }
    FILE *file = fopen(compressed_filepath.c_str(), "rb");
    if(!file) // if we cant open the compressed file (deleted)
    {
        clean_for_next_iteration();
        return -1;
    }
    FILE *out = fopen(decompressed_filepath.c_str(), "wb");
    char byte;
    Node* aux;

    if(file != NULL) {
        if(option == 0) // Tree already on memory
            aux = tree.root;
        // READ TREE
        size_t tree_size = 0;
        size_t dictionary_size = 0;
        std::map<std::string, uint8_t> reverse_dict;
        if(option == 1) // Tree on decompressed file 
        {
            size_t start_pos = ftell(file);
            aux = tree.root = tree.read(file); 
            size_t end_pos = ftell(file);
            tree_size = end_pos - start_pos;
        } else if(option == 2) { // adaptive coding
            size_t start_pos = ftell(file);
            dictionary.read(file);
            dictionary.generate_canonical_codes();
            size_t end_pos = ftell(file);
            dictionary_size = end_pos - start_pos;
            reverse_dict = dictionary.get_reverse_map();
        }
        // END READ TREE
        if(filetype && read_header)
        {
            read_header(file, filetype);
        }
        if(filetype && write_header)
        {
            write_header(out, filetype);
        }

        fseek(file, -1, SEEK_END);
        long last_byte_to_read_pos = ftell(file);
        fread(&byte, sizeof(char), 1, file);
        char quantity = byte;
        size_t pos = 0; //LONG
        if(filetype && get_pos)
        {
            pos = get_pos(filetype);
        }
        if(option != 2)
        {
            pos += tree_size;
            fseek(file, pos, SEEK_SET);
            int condition=0;
            while(fread(&byte, sizeof(char), 1, file)) {
                pos = ftell(file);
                if(pos == last_byte_to_read_pos + 1) {
                    break;
                }
                for(int i=7; i>=condition; i--) {
                    if(pos == last_byte_to_read_pos)
                        condition = 7-quantity+1;

                    if(is_bit_1(byte, i)) {
                        aux = aux->right;
                    } else {
                        aux = aux->left;
                    }
                    if(aux->right == nullptr && aux->left == nullptr) {
                        fwrite(&aux->byte, 1, 1, out);
                        aux = tree.root;
                    }
                } 
            } 
        } else if(option == 2) {
            pos += dictionary_size;
            fseek(file, pos, SEEK_SET);
            int condition=0;
            std::string str = "";
            while(fread(&byte, sizeof(char), 1, file)) {
                pos = ftell(file);
                if(pos == last_byte_to_read_pos + 1) {
                    break;
                }
                for(int i=7; i>=condition; i--) {
                    if(pos == last_byte_to_read_pos)
                        condition = 7-quantity+1;

                    if(is_bit_1(byte, i)) {
                        str.append("1");
                    } else {
                        str.append("0");
                    }
                    if(reverse_dict.find(str) != reverse_dict.end())
                    {
                        fwrite(&reverse_dict[str], sizeof(char), 1, out);
                        str = "";
                    }
                } 
            } 
        }
        fclose(file);
        fclose(out);
        clean_for_next_iteration();
    } else {
        EXIT_WITH_ERROR("Error while opening/creating archive in void decompress(char *str)");
    }
    return 0;
}
