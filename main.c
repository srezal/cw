#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "GraphicsIO.h"
#include "Tools.h"
#include "Config.h"
#include "Utils.h"
#include "IO.h"


int main(int argc, char **argv) {
    char* origin_file_name = argv[1];
    char* processed_file_name = argv[2];
    char* FUNC = argv[3];
    Config config = parse_opts(argc, argv);
    char* ORIGIN_PATH;
    char* PROCESSED_PATH;
    if(create_path_to_file(&ORIGIN_PATH, origin_file_name) == 1){
        cleanup_config_allocation(&config);
        print_error("Не удаётся открыть файл. Проверьте, что правильно ввели название файла!");
        exit(1);
    }
    if(create_path_to_file(&PROCESSED_PATH, processed_file_name) == 1){
        cleanup_config_allocation(&config);
        print_error("Не удаётся открыть файл. Проверьте, что правильно ввели название файла!");
        exit(1);
    }
    PngImage image;
    read_png_file(ORIGIN_PATH, &image);
    if(check_is_full_config_for_func(FUNC, &config) == false){
        cleanup_image_allocation(&image);
        cleanup_config_allocation(&config);
        free(ORIGIN_PATH);
        free(PROCESSED_PATH);
        exit(1);
    }
    print_image_info(&image);
    if(!strcmp("line", FUNC)) DrawLine(&image, *config.point1, *config.point2, config.line_thickness, *config.color1, false);
    else if(!strcmp("triangle", FUNC)){
        if(config.is_fill == true) FillTriangle(&image, *config.point1, *config.point2, *config.point3, *config.color2);
        DrawTriangle(&image, *config.point1, *config.point2, *config.point3, config.line_thickness, *config.color1);
    }
    else if(!strcmp("rectangle", FUNC)) FindMaxRectangleAndRepaint(&image, *config.color1, *config.color2);
    else if(!strcmp("collage", FUNC)) MakeCollage(&image, config.x_photos, config.y_photos);
    else if(!strcmp("red", FUNC)) OutlineRedPixels(&image, config.side_length, *config.color1);
    else{
        print_error("К сожалению, наша программа пока что не умеет выполнять такую функцию :(");
        cleanup_image_allocation(&image);
        cleanup_config_allocation(&config);
        free(ORIGIN_PATH);
        free(PROCESSED_PATH);
        exit(1);
    }
    write_png_file(PROCESSED_PATH, &image);
    cleanup_config_allocation(&config);
    print_success("Программа отработала успешно. Можете насладиться результатом!");
    free(ORIGIN_PATH);
    free(PROCESSED_PATH);
    return 0;
}
