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

void Huffman::Dictionary::fill(Node* tree_root, char* code, size_t columns)
{
    char left[columns+1], right[columns+1];
    if(tree_root->left == nullptr && tree_root->right == nullptr) {
        strcpy(dictionary[tree_root->byte], code);
    } else {
        strcpy(left, code);
        strcpy(right, code);

        strcat(left, "0");
        strcat(right, "1");

        fill(tree_root->left, left, columns);
        fill(tree_root->right, right, columns);
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

Huffman::Dictionary::~Dictionary()
{
    for(size_t i=0; i<Huffman::SIZE; i++)
        delete[] dictionary[i];
    delete[] dictionary;
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

/*
char *decode(char *code, struct Node *tree)
{
    const int size = strlen(code);
    char *decode = (char *)calloc(size+1, sizeof(char));

    struct Node *traverse_node = tree;
    for(int i=0; code[i] != '\0'; i++) {
        if(code[i] == '0') {
            traverse_node = traverse_node->left;
        } else if(code[i] == '1') {
            traverse_node = traverse_node->right;
        }

        if(traverse_node->left == NULL && traverse_node->right == NULL) {
            char concat[2]; concat[0] = traverse_node->character; concat[1] = '\0';
            strcat(decode, concat);

            traverse_node = tree;
        }
    }

    return decode;
}
*/

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

static std::string get_decompressed_filepath(std::string filepath)
{
    char cwd[1024];
    if(getcwd(cwd, sizeof(cwd)) == NULL)
        EXIT_WITH_ERROR("getcwd() error");
    std::string filename = cwd;
    filename.append("/data/output/");
    std::string aux_str = get_filename(filepath);
    aux_str.append("_huffman1decompressed");
    aux_str.append(get_extension(filepath));
    filename.append(aux_str);
    
    return filename;
}

static std::string get_compressed_filepath(std::string filepath)
{
    char cwd[1024];
    if(getcwd(cwd, sizeof(cwd)) == NULL)
        EXIT_WITH_ERROR("getcwd() error");
    std::string filename = cwd;
    filename.append("/data/output/");

    std::string aux_str = get_filename(filepath);
    aux_str.append("_huffman1");
    std::string format = get_extension(filepath);
    format += ".GK";
    aux_str.append(format);

    filename.append(aux_str);
    return filename;
}

void Huffman::internal_compress(char* code, void (*write_header)(FILE*, void*), void* filetype)
{
    std::string compressed_filepath = get_compressed_filepath(filepath);

    FILE *file = fopen(compressed_filepath.c_str(), "wb");
    char byte = 0;
    if(file != NULL) {
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
        fclose(file);
    } else {
        EXIT_WITH_ERROR("Error while opening/creating archive in void compress(char *str)");
    }
}

static int is_bit_1(char byte, int i)
{
    return byte &= (1U << i);
}

int Huffman::decompress(void (*write_header)(FILE*, void*), void (*read_header)(FILE*, void*), size_t (*get_pos)(void*), void* filetype)
{
    std::string decompressed_filepath = get_decompressed_filepath(filepath);
    std::string compressed_filepath = get_compressed_filepath(filepath);
    FILE *file = fopen(compressed_filepath.c_str(), "rb");
    FILE *out = fopen(decompressed_filepath.c_str(), "wb");
    if(!file) // if we cant open the compressed file (deleted)
    {
        fclose(out);
        return -1;
    }
    char byte;
    printf("compressed: %s\ndecompressed: %s\n", compressed_filepath.c_str(), decompressed_filepath.c_str());
    Node *aux = tree.root;

    if(file != NULL) {
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
        fclose(file);
        fclose(out);

        duplicates.fill(0); // we fill the duplicates with 0 for the next iteration
        tree.clear(tree.root); // we clear the tree for the next iteration
        list.front = nullptr; // we set the front of the list to nullptr for the next iteration 
        list.size = 0; // we set list size to 0 for the next iteration
    } else {
        EXIT_WITH_ERROR("Error while opening/creating archive in void decompress(char *str)");
    }
    return 0;
}
