#include <stdlib.h>
#include <string.h>
#include "ToolsManager.h"
#include "Utils.h"
#include "IO.h"
#include "GraphicsIO.h"
#include "Tools.h"

#define ERR_CODE 1


PngImage* get_images(char** input_images, int n){
    PngImage* images = malloc(sizeof(PngImage) * n);
    for(int i = 0; i < n; i++){
        char* image_path;
        if(create_path_to_file(&image_path, input_images[i]) == ERR_CODE){
            print_error("Не удаётся открыть файл. Проверьте, что правильно ввели название файла!");
        }
        read_png_file(image_path, images + i);
        free(image_path);
    }
    return images;
}


void ProcessImage(char* function, char** input_images_names, int n_input_images, char* output_image_name, Config config){
    if(check_is_full_config_for_func(function, &config) == false) return;
    PngImage* images = get_images(input_images_names, n_input_images);
    if(!strcmp("line", function)) DrawLine(images, *config.point1, *config.point2, config.line_thickness, *config.color1, false);
    else if(!strcmp("triangle", function)){
        if(config.is_fill == true) FillTriangle(images, *config.point1, *config.point2, *config.point3, *config.color2);
        DrawTriangle(images, *config.point1, *config.point2, *config.point3, config.line_thickness, *config.color1);
    }
    else if(!strcmp("rectangle", function)) FindMaxRectangleAndRepaint(images, *config.color1, *config.color2);
    else if(!strcmp("collage", function)) MakeCollage(images, config.x_photos, config.y_photos);
    char* output_image_path;
    if(create_path_to_file(&output_image_path, output_image_name) == ERR_CODE){
        print_error("Не удаётся открыть файл. Проверьте, что правильно ввели название файла!");
    }
    write_png_file(output_image_path, images); // output image here soon
    free(output_image_path);
    for(int i = 0; i < n_input_images; i++) cleanup_image_allocation(images + i);
}