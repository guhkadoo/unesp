#include <gtk/gtk.h>
#include "../Huffman_Coding_Text_File/compression.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>

#define HUFFMAN 0

static GtkWidget* uncompressed_size_label;
static GtkWidget* compressed_size_label;
static GtkWidget* algorithm_combo;
static GtkWidget* file_chooser_button;
static GtkWidget* progress_bar;
struct Node* huffman_tree;

void on_compress_button_clicked(GtkButton* button, gpointer user_data);
void on_decompress_button_clicked(GtkButton* button, gpointer user_data);
static struct Node* huffman_compress_txt(); 
static void huffman_decompress_txt(); 

int main(int argc, char* argv[])
{
    GtkWidget* window;
    GtkWidget* grid;
    GtkWidget* compress_button;
    GtkWidget* decompress_button;

    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Compression");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 200);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);

    // Algorithm selection
    algorithm_combo = gtk_combo_box_text_new(); 
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(algorithm_combo), "Huffman");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(algorithm_combo), "LZ77");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(algorithm_combo), "LZW");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(algorithm_combo), "GZIP");
    gtk_grid_attach(GTK_GRID(grid), algorithm_combo, 0, 0, 1, 1);

    // File chooser with event box
    file_chooser_button = gtk_file_chooser_button_new("Choose a file", GTK_FILE_CHOOSER_ACTION_OPEN); 
    gtk_grid_attach(GTK_GRID(grid), file_chooser_button, 0, 1, 1, 1);

    // Labels to display sizes
    uncompressed_size_label = gtk_label_new("Uncompressed size: ");
    gtk_grid_attach(GTK_GRID(grid), uncompressed_size_label, 0, 2, 1, 1);

    compressed_size_label = gtk_label_new("Compressed size: ");
    gtk_grid_attach(GTK_GRID(grid), compressed_size_label, 0, 3, 1, 1);

    // Compress button
    compress_button = gtk_button_new_with_label("Compress");
    g_signal_connect(compress_button, "clicked", G_CALLBACK(on_compress_button_clicked), NULL);
    gtk_grid_attach(GTK_GRID(grid), compress_button, 0, 4, 1, 1);

    // Decompress button
    decompress_button = gtk_button_new_with_label("Decompress");
    g_signal_connect(decompress_button, "clicked", G_CALLBACK(on_decompress_button_clicked), NULL);
    gtk_grid_attach(GTK_GRID(grid), decompress_button, 0, 5, 1, 1);

    gtk_widget_show_all(window);
    
    gtk_main();

    return 0;
}

void on_compress_button_clicked(GtkButton* button, gpointer user_data)
{
    int selected_algorithm = gtk_combo_box_get_active(GTK_COMBO_BOX(algorithm_combo));
    const gchar* file_path = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(file_chooser_button));
    const gchar* extension = strrchr(file_path, '.');
    printf("%d\n%s\n%s\n", selected_algorithm, file_path, extension);

    if(selected_algorithm == HUFFMAN)
    {
        if(!strcmp(".txt", extension))
        {
            huffman_tree = huffman_compress_txt(file_path);
        } else if(!strcmp(".wav", extension)) {
        } else if(!strcmp(".bmp", extension)) {
        } else {
            EXIT_WITH_ERROR("error while opening file\n");
        }
    }
}

void on_decompress_button_clicked(GtkButton* button, gpointer user_data)
{
    int selected_algorithm = gtk_combo_box_get_active(GTK_COMBO_BOX(algorithm_combo));
    const gchar* file_path = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(file_chooser_button));
    const gchar* extension = strrchr(file_path, '.');
    if(selected_algorithm == HUFFMAN)
    {
        if(!strcmp(".txt", extension))
        {
            huffman_decompress_txt();
        } else if(!strcmp(".wav", extension)) {
        } else if(!strcmp(".bmp", extension)) {
        } else {
            EXIT_WITH_ERROR("error while opening file\n");
        }
    }
}

static struct Node* huffman_compress_txt(const gchar* filename)
{
    unsigned int duplicates[SIZE];
    List list;
    initialize_list(&list);
    char **dictionary;
    unsigned char *str = read_text(filename);
    count_duplicates(str, duplicates);
    fill_list(&list, duplicates);
    struct Node *huffman_tree = create_huffman_tree(&list);
    int columns = height_huffman(huffman_tree);
    dictionary = dictionary_allocation(columns); 
    fill_dictionary(dictionary, huffman_tree, "", columns);
    char *code = encode(dictionary, str);
    compress(code);

    return huffman_tree;
}

static void huffman_decompress_txt()
{
    decompress(huffman_tree);
}
