#include "Color.h"


int colors_equal(Color a, Color b){
    return (a.r == b.r) && (a.g == b.g) && (a.b == b.b) && (a.a == b.a);
}


bool check_color(int r, int g, int b){
    if((r < 0 || r > 255) || (g < 0 || g > 255) || (b < 0 || b > 255)) return false;
    return true;
}