#include "GraphicsIO.h"
#include "IO.h"
#include <stdlib.h>
#include <string.h>


void cleanup_image_allocation(PngImage *image){
    for (int y = 0; y < image->height; y++)
        free(image->row_pointers[y]);
    free(image->row_pointers);
}


void read_png_file(char *file_name, PngImage *image) {
    int x,y;
    char header[8];    // 8 is the maximum size that can be checked

    /* open file and test for it being a png */
    FILE *fp = fopen(file_name, "rb");
    if (!fp){
        // Some error handling: file could not be opened
        print_error("Не удаётся открыть файл. Проверьте, что правильно ввели название файла!");
        exit(1);
    }

    fread(header, 1, 8, fp);
    if (png_sig_cmp(header, 0, 8)){
        // Some error handling: file is not recognized as a PNG
        print_error("Упс.. Кажется вы просите обработать не PNG файл. Наша программа обрабатывает только PNG файлы!");
        exit(1);
    }

    /* initialize stuff */
    image->png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

    if (!image->png_ptr){
        // Some error handling: png_create_read_struct failed
        print_error("Не можем разобраться с вашим PNG файлом. Скорее всего он повреждён!");
        exit(1);
    }

    image->info_ptr = png_create_info_struct(image->png_ptr);
    if (!image->info_ptr){
        // Some error handling: png_create_info_struct failed
        print_error("Не можем разобраться с вашим PNG файлом. Скорее всего он повреждён!");
        exit(1);
    }

    if (setjmp(png_jmpbuf(image->png_ptr))){
        // Some error handling: error during init_io
        print_error("Не можем разобраться с вашим PNG файлом. Скорее всего он повреждён!");
        exit(1);
    }

    png_init_io(image->png_ptr, fp);
    png_set_sig_bytes(image->png_ptr, 8);

    png_read_info(image->png_ptr, image->info_ptr);

    image->width = (int)png_get_image_width(image->png_ptr, image->info_ptr);
    image->height = (int)png_get_image_height(image->png_ptr, image->info_ptr);
    image->color_type = png_get_color_type(image->png_ptr, image->info_ptr);
    image->bit_depth = png_get_bit_depth(image->png_ptr, image->info_ptr);

    image->number_of_passes = png_set_interlace_handling(image->png_ptr);
    png_read_update_info(image->png_ptr, image->info_ptr);

    /* read file */
    if (setjmp(png_jmpbuf(image->png_ptr))){
        // Some error handling: error during read_image
        print_error("Не можем разобраться с вашим PNG файлом. Скорее всего он повреждён!");
        exit(1);
    }

    image->row_pointers = (png_bytep *) malloc(sizeof(png_bytep) * image->height);
    for (y = 0; y < image->height; y++)
        image->row_pointers[y] = (png_byte *) malloc(png_get_rowbytes(image->png_ptr, image->info_ptr));

    png_read_image(image->png_ptr, image->row_pointers);

    fclose(fp);
}


void write_png_file(char *file_name, PngImage *image) {
    /* create file */
    FILE *fp = fopen(file_name, "wb");
    if (!fp){
        // Some error handling: file could not be opened
        print_error("Не удаётся открыть файл. Проверьте, что ввели корректное название файла!");
        exit(1);
    }

    /* initialize stuff */
    image->png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

    if (!image->png_ptr){
        // Some error handling: png_create_write_struct failed
        print_error("Не можем разобраться с вашим PNG файлом. Скорее всего он повреждён!");
        exit(1);
    }

    image->info_ptr = png_create_info_struct(image->png_ptr);
    if (!image->info_ptr){
        // Some error handling: png_create_info_struct failed
        print_error("Не можем разобраться с вашим PNG файлом. Скорее всего он повреждён!");
        exit(1);
    }

    if (setjmp(png_jmpbuf(image->png_ptr))){
        // Some error handling: error during init_io
        print_error("Не можем разобраться с вашим PNG файлом. Скорее всего он повреждён!");
        exit(1);
    }

    png_init_io(image->png_ptr, fp);


    /* write header */
    if (setjmp(png_jmpbuf(image->png_ptr))){
        // Some error handling: error during writing header
        print_error("Не можем разобраться с вашим PNG файлом. Скорее всего он повреждён!");
        exit(1);
    }

    png_set_IHDR(image->png_ptr, image->info_ptr, image->width, image->height,
                 image->bit_depth, image->color_type, PNG_INTERLACE_NONE,
                 PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

    png_write_info(image->png_ptr, image->info_ptr);


    /* write bytes */
    if (setjmp(png_jmpbuf(image->png_ptr))){
        // Some error handling: error during writing bytes
        print_error("Не можем разобраться с вашим PNG файлом. Скорее всего он повреждён!");
        exit(1);
    }

    png_write_image(image->png_ptr, image->row_pointers);


    /* end write */
    if (setjmp(png_jmpbuf(image->png_ptr))){
        // Some error handling: error during end of write
        print_error("Не можем разобраться с вашим PNG файлом. Скорее всего он повреждён!");
        exit(1);
    }

    png_write_end(image->png_ptr, NULL);

    fclose(fp);
}