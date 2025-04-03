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
#define HUFFMAN_TREE_ON_DECOMPRESSED_FILE 1
#define CANONICAL_HUFFMAN_CODING 2
#define LZ77 3
#define LWZ 4
#define GZIP 5

static bool is_compressed = false;
static GtkWidget* uncompressed_size_label;
static GtkWidget* compressed_size_label;
static GtkWidget* algorithm_combo;
static GtkWidget* file_chooser_button;
static GtkWidget* progress_bar;
static GtkWidget* compress_button; // Other options
static GtkWidget* decompress_button; // Other options
static GtkWidget* compress_decompress_button; // HUFFMAN_TREE_ON_MEMORY
static int selected_algorithm = -1;
const gchar* file_path = nullptr; 
HuffmanTXT archive_txt;
HuffmanBMP archive_bmp;
HuffmanWAV archive_wav;


int on_compress_button_clicked(GtkButton* button, gpointer user_data);
int on_decompress_button_clicked(GtkButton* button, gpointer user_data);
void on_file_selected(GtkFileChooserButton* button, gpointer user_data);
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

    // File chooser with event box
    file_chooser_button = gtk_file_chooser_button_new("Choose a file", GTK_FILE_CHOOSER_ACTION_OPEN); 
    gtk_grid_attach(GTK_GRID(grid), file_chooser_button, 0, 1, 1, 1);
    g_signal_connect(GTK_FILE_CHOOSER(file_chooser_button), "file-set", G_CALLBACK(on_file_selected), NULL);

    // Algorithm selection
    algorithm_combo = gtk_combo_box_text_new(); 
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(algorithm_combo), "Huffman - Tree on memory");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(algorithm_combo), "Huffman - Tree on decompressed file"); 
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(algorithm_combo), "Huffman - Adaptive coding");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(algorithm_combo), "LZ77");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(algorithm_combo), "LZW");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(algorithm_combo), "GZIP");
    gtk_grid_attach(GTK_GRID(grid), algorithm_combo, 0, 0, 1, 1);
    g_signal_connect(GTK_COMBO_BOX_TEXT(algorithm_combo), "changed", G_CALLBACK(on_algorithm_selected),
            file_chooser_button);


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
    gtk_grid_attach(GTK_GRID(grid), decompress_button, 0, 4, 1, 1);

    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_widget_show_all(window);

    gtk_widget_hide(compress_decompress_button);
    gtk_widget_hide(compress_button);
    gtk_widget_hide(decompress_button);
 
    gtk_main();
}

static void create_dialog_and_destroy(const char* msg)
{
    GtkWidget* dialog = gtk_message_dialog_new(NULL,
                                                GTK_DIALOG_MODAL,
                                                GTK_MESSAGE_ERROR,
                                                GTK_BUTTONS_OK,
                                                msg);
    gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER);
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

static int read_selected_algorithm_and_file_path(int* selected_algorithm, const gchar** file_path)
{
    *selected_algorithm = gtk_combo_box_get_active(GTK_COMBO_BOX(algorithm_combo)); 
    if(*selected_algorithm == -1)
    {
        create_dialog_and_destroy("Select a compression algorithm.");
        return -1;
    }
    *file_path = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(file_chooser_button));
    if(*file_path == nullptr) {
        create_dialog_and_destroy("Select a file.");
        return -1;
    }
    return 0;
}

void on_algorithm_selected(GtkComboBox* combo, gpointer user_data)
{
    if(file_path != nullptr)
        on_file_selected(GTK_FILE_CHOOSER_BUTTON(file_chooser_button), NULL);
}

static int valid_decompressed_extension(const gchar* extension)
{
    if(!strcmp(extension, ".txt") || !strcmp(extension, ".wav") || !strcmp(extension, ".bmp"))
        return 1;
    return 0;
}

static int valid_compressed_extension(const gchar* extension)
{
    if(!strcmp(extension, ".txt.GK") || !strcmp(extension, ".wav.GK") || !strcmp(extension, ".bmp.GK"))
        return 1;
    return 0;
}

void on_file_selected(GtkFileChooserButton* button, gpointer user_data)
{
    file_path = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(button));
    if(file_path == nullptr)
        return;
    const gchar* extension = strchr(file_path, '.');
    selected_algorithm = gtk_combo_box_get_active(GTK_COMBO_BOX(algorithm_combo)); 
    if(selected_algorithm == -1)
        return;
    printf("extension: %s\n", extension);
    printf("compressed: %d decompressed: %d\n", valid_compressed_extension(extension), valid_decompressed_extension(extension));
    if(valid_decompressed_extension(extension) && selected_algorithm == HUFFMAN_TREE_ON_MEMORY)
    {
        printf("oi\n");
        gtk_widget_show(compress_decompress_button);
        gtk_widget_hide(compress_button);
        gtk_widget_hide(decompress_button);
        return ;
    } else if(valid_decompressed_extension(extension) && selected_algorithm != HUFFMAN_TREE_ON_MEMORY) {
        printf("oi2\n");
        gtk_widget_hide(compress_decompress_button);
        gtk_widget_show(compress_button);
        gtk_widget_hide(decompress_button);
    } else if(valid_compressed_extension(extension) && selected_algorithm != HUFFMAN_TREE_ON_MEMORY) {
        printf("oi3\n");
        gtk_widget_hide(compress_decompress_button);
        gtk_widget_hide(compress_button);
        gtk_widget_show(decompress_button);
    } else {
        printf("oi4\n");
        gtk_widget_hide(compress_decompress_button);
        gtk_widget_hide(compress_button);
        gtk_widget_hide(decompress_button);
    }
}

void on_compress_decompress_button_clicked(GtkButton* button, gpointer user_data)
{
    if(is_compressed)
    {
        if(on_decompress_button_clicked(button, NULL) != -1)
        {
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
    const gchar* file_path; 
    if(read_selected_algorithm_and_file_path(&selected_algorithm, &file_path) == -1)
        return -1;
    const gchar* extension = strrchr(file_path, '.');
    printf("AQUI: %d\n%s\n%s\n", selected_algorithm, file_path, extension);
    std::string path(file_path); 

    if(selected_algorithm == HUFFMAN_TREE_ON_MEMORY)
    {
        if(!strcmp(".txt", extension)) {
            if(archive_txt.set_filepath(path) == -1)
            {
                create_dialog_and_destroy("File not found.");
                return -1;
            }
            archive_txt.compress(HUFFMAN_TREE_ON_MEMORY);
        } else if(!strcmp(".wav", extension)) {
            if(archive_wav.set_filepath(path) == -1)
            {
                create_dialog_and_destroy("File not found.");
                return -1;
            }
            archive_wav.compress(HUFFMAN_TREE_ON_MEMORY);
        } else if(!strcmp(".bmp", extension)) {
            if(archive_bmp.set_filepath(path) == -1)
            {
                create_dialog_and_destroy("File not found.");
                return -1;
            }
            archive_bmp.compress(HUFFMAN_TREE_ON_MEMORY);
        } else {
            create_dialog_and_destroy("Extension not supported.");
            return -1;
        }
    } else if(selected_algorithm == HUFFMAN_TREE_ON_DECOMPRESSED_FILE) {
        if(!strcmp(".txt", extension)) {
            if(archive_txt.set_filepath(path) == -1)
            {
                create_dialog_and_destroy("File not found.");
                return -1;
            }
            archive_txt.compress(HUFFMAN_TREE_ON_DECOMPRESSED_FILE);
        } else if(!strcmp(".wav", extension)) {
            if(archive_wav.set_filepath(path) == -1)
            {
                create_dialog_and_destroy("File not found.");
                return -1;
            }
            archive_wav.compress(HUFFMAN_TREE_ON_DECOMPRESSED_FILE);
        } else if(!strcmp(".bmp", extension)) {
            if(archive_bmp.set_filepath(path) == -1)
            {
                create_dialog_and_destroy("File not found.");
                return -1;
            }
            archive_bmp.compress(HUFFMAN_TREE_ON_DECOMPRESSED_FILE);
        } else {
            create_dialog_and_destroy("Extension not supported.");
            return -1;
        }
    } else if(selected_algorithm == CANONICAL_HUFFMAN_CODING) {
        if(!strcmp(".txt", extension)) {
            if(archive_txt.set_filepath(path) == -1)
            {
                create_dialog_and_destroy("File not found.");
                return -1;
            }
            archive_txt.compress(CANONICAL_HUFFMAN_CODING);
        } else if(!strcmp(".wav", extension)) {
            if(archive_wav.set_filepath(path) == -1)
            {
                create_dialog_and_destroy("File not found.");
                return -1;
            }
            archive_wav.compress(CANONICAL_HUFFMAN_CODING);
        } else if(!strcmp(".bmp", extension)) {
            if(archive_bmp.set_filepath(path) == -1)
            {
                create_dialog_and_destroy("File not found.");
                return -1;
            }
            archive_bmp.compress(CANONICAL_HUFFMAN_CODING);
        } else {
            create_dialog_and_destroy("Extension not supported.");
            return -1;
        }
    }
    return 0;
}

int on_decompress_button_clicked(GtkButton* button, gpointer user_data)
{
    const gchar* file_path;
    if(read_selected_algorithm_and_file_path(&selected_algorithm, &file_path) == -1)
        return -1;
    const gchar* extension = strchr(file_path, '.');
    if(selected_algorithm == HUFFMAN_TREE_ON_MEMORY)
    {
        if(!strcmp(".txt", extension)) 
        {
            int found_compressed_file = archive_txt.decompress(HUFFMAN_TREE_ON_MEMORY);
            if(found_compressed_file == -1)
            {
                create_dialog_and_destroy("File not found.");
                is_compressed = false;
                gtk_button_set_label(GTK_BUTTON(button), "Compress");
                return -1;
            }
        } else if(!strcmp(".wav", extension)) {
            int found_compressed_file = archive_wav.decompress(HUFFMAN_TREE_ON_MEMORY);
            if(found_compressed_file == -1)
            {
                create_dialog_and_destroy("File not found.");
                is_compressed = false;
                gtk_button_set_label(GTK_BUTTON(button), "Compress");
                return -1;
            }
        } else if(!strcmp(".bmp", extension)) {
            int found_compressed_file = archive_bmp.decompress(HUFFMAN_TREE_ON_MEMORY);
            if(found_compressed_file == -1)
            {
                create_dialog_and_destroy("File not found.");
                is_compressed = false;
                gtk_button_set_label(GTK_BUTTON(button), "Compress");
                return -1;
            }
        } else {
            create_dialog_and_destroy("Extension not supported.");
            return -1;
        }
    } else if(selected_algorithm == HUFFMAN_TREE_ON_DECOMPRESSED_FILE) {
        std::string path = file_path;
        printf("%s\n", file_path);
        if(archive_txt.set_filepath(path) == -1 || archive_wav.set_filepath(path) == -1 || archive_bmp.set_filepath(path) == -1)
        {
            create_dialog_and_destroy("File not found.");
            return -1;
        }
        printf("%s\n", extension);
        if(!strcmp(".txt.GK", extension)) 
        {
            int found_compressed_file = archive_txt.decompress(HUFFMAN_TREE_ON_DECOMPRESSED_FILE);
            if(found_compressed_file == -1)
            {
                create_dialog_and_destroy("File not found.");
                return -1;
            }
        } else if(!strcmp(".wav.GK", extension)) {
            int found_compressed_file = archive_wav.decompress(HUFFMAN_TREE_ON_DECOMPRESSED_FILE);
            if(found_compressed_file == -1)
            {
                create_dialog_and_destroy("File not found.");
                return -1;
            }
        } else if(!strcmp(".bmp.GK", extension)) {
            int found_compressed_file = archive_bmp.decompress(HUFFMAN_TREE_ON_DECOMPRESSED_FILE);
            if(found_compressed_file == -1)
            {
                create_dialog_and_destroy("File not found.");
                return -1;
            }
        } else {
            create_dialog_and_destroy("Extension not supported.");
            return -1;
        }
    } else if(selected_algorithm == CANONICAL_HUFFMAN_CODING) {
        std::string path = file_path;
        printf("%s\n", file_path);
        if(archive_txt.set_filepath(path) == -1 || archive_wav.set_filepath(path) == -1 || archive_bmp.set_filepath(path) == -1)
        {
            create_dialog_and_destroy("File not found.");
            return -1;
        }
        printf("%s\n", extension);
        if(!strcmp(".txt.GK", extension)) 
        {
            int found_compressed_file = archive_txt.decompress(CANONICAL_HUFFMAN_CODING);
            if(found_compressed_file == -1)
            {
                create_dialog_and_destroy("File not found.");
                return -1;
            }
        } else if(!strcmp(".wav.GK", extension)) {
            int found_compressed_file = archive_wav.decompress(CANONICAL_HUFFMAN_CODING);
            if(found_compressed_file == -1)
            {
                create_dialog_and_destroy("File not found.");
                return -1;
            }
        } else if(!strcmp(".bmp.GK", extension)) {
            int found_compressed_file = archive_bmp.decompress(CANONICAL_HUFFMAN_CODING);
            if(found_compressed_file == -1)
            {
                create_dialog_and_destroy("File not found.");
                return -1;
            }
        } else {
            create_dialog_and_destroy("Extension not supported.");
            return -1;
        }
    }
    return 0;
}
