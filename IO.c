#include <getopt.h>
#include <stdio.h>
#include <stdbool.h>
#include "IO.h"
#include "GraphicsStructs.h"


void print_help() {
    printf("Это программа с CLI для редактирования png файлов\n");
    printf("Формат ввода: ./cw [имя исходного файла] [имя выходного файла] [функция] "
           "-[ключ1]/--[полный ключ1] [аргумент1] ...\n");
    printf("Функции/ключи:\n");
    printf("line - рисование прямой линии.\n");
    printf("    -a/--first_point  [<x-координата>,<y-координата>] - начало линии\n");
    printf("    -b/--second_point  [<x-координата>,<y-координата>] - конец линии\n");
    printf("    -t/--line_thickness [<число>] - толщина линии(в пикселях)\n");
    printf("    --color1 [<число>,<число>,<число>] - цвет линии (RGB)\n");
    printf("triangle - рисование треугольника с возможностью его залить и "
           "выбрать "
           "цвет.\n");
    printf("    -a/--first_point  [<x-координата>,<y-координата>] - первая вершина треугольника\n");
    printf("    -b/--second_point  [<x-координата>,<y-координата>] - вторая вершина треугольника\n");
    printf("    -c/--third_point [<x-координата>,<y-координата>] - третья вершина треугольника\n");
    printf("    -t/--line_thickness [<число>] - толщина сторон треугольника(в пикселях)\n");
    printf("    --color1 [<число>,<число>,<число>] - цвет сторон треугольника (RGB)\n");
    printf("    --fill (аргумент не требуется) - требуется ли заливка треугольника\n");
    printf("    --color2 [<число>,<число>,<число>] - цвет заливки треугольника (RGB)\n");
    printf("collage - создается коллаж из изображения.\n");
    printf("    -x/--x_photos  [<число>] - количество изображений по оси X\n");
    printf("    -y/--y_photos  [<число>] - количество изображений по оси Y\n");
    printf("rectangle - поиск самого большого прямоугольника заданного цвета и его "
           "перекраска в заданный цвет.\n");
    printf("    --color1 [<число>,<число>,<число>] - цвет перекраски (RGB)\n");
    printf("    --color2 [<число>,<число>,<число>] - цвет искомого прямоугольника (RGB)\n");
    printf("-h/--help - вывод справки о работе программы.\n");
}


void print_image_info(PngImage* image){
    printf("\033[38;05;74m[IMAGE INFO]\033[0m:\n");
    printf("    разрешение: %dx%d\n", image->width, image->height);
    printf("    цветовая модель: %s\n", (image->color_type == 6) ? "RGBA" : "RGB");
    printf("    количество бит на цветовой канал: %d\n", image->bit_depth);
}


void print_success(char* message){
    printf("\033[38;05;40m[SUCCESS]\033[0m: %s\n", message);
}


void print_error(char* message){
    fprintf(stderr, "\033[31m[ERROR]\033[0m: %s\n", message);
}


void print_warning(char* message){
    printf("\033[01;38;05;172m[WARNING]\033[0m: %s\n", message);
}


Config parse_opts(int argc, char **argv){
    char *short_opts = "a:b:c:t:x:y:I:h";
    int opt;
    int longIndex;
    extern int opterr;
    opterr = 0; // disable standard getopt errors/warnings
    struct option longOpts[] = {
            {"point1",   required_argument, NULL, 'a'},
            {"point2",  required_argument, NULL, 'b'},
            {"point3",  required_argument, NULL, 'c'},
            {"thickness", required_argument, NULL, 't'},
            {"color1",   required_argument, NULL, 'l'},
            {"fill", no_argument, NULL, 'i'},
            {"color2",   required_argument, NULL, 'f'},
            {"x_photos",   required_argument, NULL, 'x'},
            {"y_photos",   required_argument, NULL, 'y'},
            {"second_image", required_argument, NULL, 'I'},
            {"help",    no_argument,       NULL, 'h'},
            {NULL,      no_argument,       NULL, 0}
    };
    Config config = {NULL, NULL, NULL, 0, NULL, false, NULL, 0, 0, NULL};
    while (true) {
        opt = getopt_long(argc, argv, short_opts, longOpts, &longIndex);
        if(opt == -1) break;
        set_config(&config, opt, optarg, optopt);
    }
    return config;
}