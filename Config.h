#pragma once

#include <stdbool.h>
#include "Point.h"
#include "Color.h"
#include "GraphicsStructs.h"


typedef struct Config{
    Point* point1;
    Point* point2;
    Point* point3;
    int line_thickness;
    Color* color1;
    bool is_fill;
    Color* color2;
    int x_photos;
    int y_photos;
    char* second_image_name;
} Config;


int parse_point(char* arg, Point* point);
int parse_color(char* arg, int* r, int* g, int* b);
void cleanup_config_allocation(Config* config);
void set_config(Config* config, int opt, char* arg, int optopt);
bool check_is_full_config_for_func(char* FUNC, Config* config);