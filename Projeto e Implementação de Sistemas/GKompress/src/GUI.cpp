#include <gtk/gtk.h>
#include "../include/GUI.hpp"
#include "../include/huffman/huffman_txt.hpp"
#include "../include/huffman/huffman_wav.hpp"
#include "../include/huffman/huffman_bmp.hpp"
#include <cstdio>
#include <string>
#include <cstdlib>
#include <cstring>

#define HUFFMAN_TREE_ON_MEMORY 0

static bool is_compressed = false;
static GtkWidget* uncompressed_size_label;
static GtkWidget* compressed_size_label;
static GtkWidget* algorithm_combo;
static GtkWidget* file_chooser_button;
static GtkWidget* progress_bar;
static GtkWidget* compress_button; // Other options
static GtkWidget* decompress_button; // Other options
static GtkWidget* compress_decompress_button; // HUFFMAN_TREE_ON_MEMORY
HuffmanTXT archive_txt;
HuffmanBMP archive_bmp;
HuffmanWAV archive_wav;


int on_compress_button_clicked(GtkButton* button, gpointer user_data);
int on_decompress_button_clicked(GtkButton* button, gpointer user_data);
void on_compress_decompress_button_clicked(GtkButton* button, gpointer user_data);
void on_algorithm_selected(GtkComboBox* combo, gpointer user_data);
void toggle_compress_decompress_button(GtkButton* button, gpointer user_data);
static int read_selected_algorithm_and_file_path(int* selected_algorithm, const gchar** file_path);

void GUI::init(int argc, char* argv[])
{
    GtkWidget* window;
    GtkWidget* grid;

    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Compression");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 200);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);

    // Algorithm selection
    algorithm_combo = gtk_combo_box_text_new(); 
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(algorithm_combo), "Huffman - Tree on memory");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(algorithm_combo), "Huffman - Tree on decompressed file"); 
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(algorithm_combo), "Huffman - Adaptive coding");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(algorithm_combo), "LZ77");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(algorithm_combo), "LZW");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(algorithm_combo), "GZIP");
    gtk_grid_attach(GTK_GRID(grid), algorithm_combo, 0, 0, 1, 1);
    g_signal_connect(GTK_COMBO_BOX_TEXT(algorithm_combo), "changed", G_CALLBACK(on_algorithm_selected), NULL);

    // File chooser with event box
    file_chooser_button = gtk_file_chooser_button_new("Choose a file", GTK_FILE_CHOOSER_ACTION_OPEN); 
    gtk_grid_attach(GTK_GRID(grid), file_chooser_button, 0, 1, 1, 1);

    // Labels to display sizes
    uncompressed_size_label = gtk_label_new("Uncompressed size: ");
    gtk_grid_attach(GTK_GRID(grid), uncompressed_size_label, 0, 2, 1, 1);

    compressed_size_label = gtk_label_new("Compressed size: ");
    gtk_grid_attach(GTK_GRID(grid), compressed_size_label, 0, 3, 1, 1);

    // Single Compress/Decompress button for Huffman Tree
    compress_decompress_button = gtk_button_new_with_label("Compress");
    g_signal_connect(GTK_BUTTON(compress_decompress_button), "clicked", G_CALLBACK(on_compress_decompress_button_clicked), NULL);
    gtk_grid_attach(GTK_GRID(grid), compress_decompress_button, 0, 4, 1, 1);

    // Compress button
    compress_button = gtk_button_new_with_label("Compress");
    g_signal_connect(GTK_BUTTON(compress_button), "clicked", G_CALLBACK(on_compress_button_clicked), NULL);
    gtk_grid_attach(GTK_GRID(grid), compress_button, 0, 4, 1, 1);


    // Decompress button
    decompress_button = gtk_button_new_with_label("Decompress");
    g_signal_connect(GTK_BUTTON(decompress_button), "clicked", G_CALLBACK(on_decompress_button_clicked), NULL);
    gtk_grid_attach(GTK_GRID(grid), decompress_button, 0, 5, 1, 1);

    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_widget_show_all(window);
    
    gtk_main();
}

static int read_selected_algorithm_and_file_path(int* selected_algorithm, const gchar** file_path)
{
    *selected_algorithm = gtk_combo_box_get_active(GTK_COMBO_BOX(algorithm_combo)); 
    if(*selected_algorithm == -1)
    {
        GtkWidget* dialog = gtk_message_dialog_new(NULL,
                                                    GTK_DIALOG_MODAL,
                                                    GTK_MESSAGE_ERROR,
                                                    GTK_BUTTONS_OK,
                                                    "Select a compression algorithm.");
        gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER);
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        return -1;
    }
    *file_path = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(file_chooser_button));
    if(*file_path == nullptr) {
        GtkWidget* dialog = gtk_message_dialog_new(NULL,
                                                    GTK_DIALOG_MODAL,
                                                    GTK_MESSAGE_ERROR,
                                                    GTK_BUTTONS_OK,
                                                    "Select a file.");
        gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER);
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        return -1;
    }
    return 0;
}

void on_algorithm_selected(GtkComboBox* combo, gpointer user_data)
{
    int selected_algorithm = gtk_combo_box_get_active(combo);

    if(selected_algorithm == HUFFMAN_TREE_ON_MEMORY)
    {
        gtk_widget_show(compress_decompress_button);
        gtk_widget_hide(compress_button);
        gtk_widget_hide(decompress_button);
    } else {
        gtk_widget_hide(compress_decompress_button);
        gtk_widget_show(compress_button);
        gtk_widget_show(decompress_button);
    }
}

void on_compress_decompress_button_clicked(GtkButton* button, gpointer user_data)
{
    if(is_compressed)
    {
        if(on_decompress_button_clicked(button, NULL) != -1)
        {
            on_decompress_button_clicked(button, NULL);
            is_compressed = false;
            gtk_button_set_label(GTK_BUTTON(button), "Compress");
        }
    } else {
        if(on_compress_button_clicked(button, NULL) != -1)
        {
            is_compressed = true;
            gtk_button_set_label(GTK_BUTTON(button), "Decompress");
        }
    }
}

int on_compress_button_clicked(GtkButton* button, gpointer user_data)
{
    int selected_algorithm; 
    const gchar* file_path; 
    if(read_selected_algorithm_and_file_path(&selected_algorithm, &file_path) == -1)
        return -1;
    const gchar* extension = strrchr(file_path, '.');
    printf("%d\n%s\n%s\n", selected_algorithm, file_path, extension);
    std::string path(file_path); 

    if(selected_algorithm == HUFFMAN_TREE_ON_MEMORY)
    {
        if(!strcmp(".txt", extension)) {
            archive_txt.set_filepath(path);
            archive_txt.compress();
        } else if(!strcmp(".wav", extension)) {
            archive_wav.set_filepath(path);
            archive_wav.compress();
        } else if(!strcmp(".bmp", extension)) {
            archive_bmp.set_filepath(path);
            archive_bmp.compress();
        } else {
            EXIT_WITH_ERROR("error while opening file\n");
        }
    }
    return 0;
}

int on_decompress_button_clicked(GtkButton* button, gpointer user_data)
{
    int selected_algorithm;
    const gchar* file_path;
    if(read_selected_algorithm_and_file_path(&selected_algorithm, &file_path) == -1)
        return -1;
    const gchar* extension = strrchr(file_path, '.');
    if(selected_algorithm == HUFFMAN_TREE_ON_MEMORY)
    {
        auto fun_not_compressed = []() -> void { printf("Compress the file first.\n"); };
        if(!strcmp(".txt", extension)) 
        {
            archive_txt.get_filepath() == "" ? fun_not_compressed() : archive_txt.decompress();
        } else if(!strcmp(".wav", extension)) {
            archive_wav.get_filepath() == "" ? fun_not_compressed() : archive_wav.decompress();
        } else if(!strcmp(".bmp", extension)) {
            archive_bmp.get_filepath() == "" ? fun_not_compressed() : archive_bmp.decompress();
        } else {
            EXIT_WITH_ERROR("error while opening file\n");
        }
    } 
    return 0;
}
