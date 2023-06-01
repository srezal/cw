#include <string.h>
#include "GraphicsUtils.h"


int n_color_channels(PngImage* image){
    if(image == NULL) return 0;
    if(image->color_type == 6) return 4;
    return 3;
}


Color GetPixelColor(PngImage* image, int x, int y){
    if(x >= 0 && x < image->width && y >= 0 && y < image->height) {
        int width_pixel = n_color_channels(image);
        png_byte* pixel_ptr = &(image->row_pointers[y][x * width_pixel]);
        Color pixel_color = {pixel_ptr[0], pixel_ptr[1], pixel_ptr[2], pixel_ptr[3]};
        return pixel_color;
    }
}


void PaintPixel(PngImage* image, int x, int y, Color color){
    if(x >= 0 && x < image->width && y >= 0 && y < image->height) {
        int width_pixel = n_color_channels(image);
        png_byte* pixel_ptr = &(image->row_pointers[y][x * width_pixel]);
        pixel_ptr[0] = color.r;
        pixel_ptr[1] = color.g;
        pixel_ptr[2] = color.b;
        if(width_pixel == 4) pixel_ptr[3] = color.a;
    }
}


void copy_area(png_byte** dest, png_byte** src, Point src_left_up, Point src_right_bottom, Point dest_left_up, int width_pixel){
    int area_width = src_right_bottom.x - src_left_up.x + 1;
    int area_height = src_right_bottom.y - src_left_up.y + 1;
    for(int i = 0; i < area_height; i++){
        memcpy(&(dest[dest_left_up.y + i][dest_left_up.x * width_pixel]), &(src[src_left_up.y + i][src_left_up.x * width_pixel]), area_width * width_pixel * sizeof(png_byte));
    }
}


int get_horizontal_color_border(PngImage* image, int x, int y, Color color){
    int j;
    for(j = x; j < image->width; j++){
        Color pixel_color = GetPixelColor(image, j, y);
        if(!colors_equal(pixel_color, color)) break;
    }
    return j - 1;
}


int get_vertical_color_border(PngImage* image, int x, int y, Color color){
    int i;
    for(i = y; i < image->height; i++){
        Color pixel_color = GetPixelColor(image, x, i);
        if(!colors_equal(pixel_color, color)) break;
    }
    return i - 1;
}