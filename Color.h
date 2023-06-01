#pragma once
#include <stdbool.h>

typedef struct Color{
    unsigned short r;
    unsigned short g;
    unsigned short b;
    unsigned short a;
} Color;


int colors_equal(Color a, Color b);
bool check_color(int r, int g, int b);