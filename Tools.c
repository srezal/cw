#include <math.h>
#include <stdlib.h>
#include "Tools.h"
#include "GraphicsStructs.h"
#include "GraphicsUtils.h"
#include "Utils.h"


void DrawLine(PngImage* image, Point start, Point end, int thickness, Color color, bool is_rounded){
    int delta_x = (end.x - start.x);
    int delta_y = (end.y - start.y);
    double k = fabs((double)(MIN(abs(delta_x), abs(delta_y))) / (MAX(abs(delta_x), abs(delta_y))));
    double err = 0;
    Point current = start;
    while(1){
        if(is_rounded == true) PaintCircle(image, current, thickness / 2.0, color);
        else PaintSquare(image, current, thickness, color);
        if(points_equal(current, end)) break;
        err += k;
        if(err > 0.5){
            if(abs(delta_x) > abs(delta_y)) current.y += 1 * (SIGNUM(delta_y));
            else current.x += 1 * (SIGNUM(delta_x));
            err -= 1;
        }
        if(abs(delta_x) > abs(delta_y)) current.x += 1 * (SIGNUM(delta_x));
        else current.y += 1 * (SIGNUM(delta_y));
    }
}


void PaintCircle(PngImage* image, Point O, double r, Color color) {
    for(int x = O.x - r + 1; x < O.x + r; x++){
        for(int y = O.y - r + 1; y < O.y + r; y++){
            int r_x = abs(O.x - x);
            int r_y = abs(O.y - y);
            if((r_x * r_x + r_y * r_y) <= (r * r)) PaintPixel(image, x, y, color);
        }
    }
}


void PaintSquare(PngImage* image, Point O, int side_length, Color color){
    int bias = side_length / 2;
    for(int x = O.x - bias; x < O.x + (side_length - bias); x++){
        for(int y = O.y - bias; y < O.y + (side_length - bias); y++){
            PaintPixel(image, x, y, color);
        }
    }
}


void DrawTriangle(PngImage* image, Point a, Point b, Point c, int thickness, Color border_color){
    DrawLine(image, a, b, thickness, border_color, true);
    DrawLine(image, b, c, thickness, border_color, true);
    DrawLine(image, c, a, thickness, border_color, true);
}


void Fill(PngImage* image, int x, int y, Color fill_color){
    if(!(x >= 0 && x < image->width && y >= 0 && y < image->height)) return;
    Color pixel_color = GetPixelColor(image, x, y);
    if(!colors_equal(pixel_color, fill_color)){
        PaintPixel(image, x, y, fill_color);
        Fill(image, x + 1, y, fill_color);
        Fill(image, x, y + 1, fill_color);
        Fill(image, x - 1, y, fill_color);
        Fill(image, x, y - 1, fill_color);
    }
    return;
}


void FillTriangle(PngImage* image,  Point a, Point b, Point c, Color fill_color){
    for(int x = MIN(MIN(a.x, b.x), c.x); x <= MAX(MAX(a.x, b.x), c.x); x++){
        for(int y = MIN(MIN(a.y, b.y), c.y); y <= MAX(MAX(a.y, b.y), c.y); y++){
            int k1 = (a.x - x) * (b.y - a.y) - (b.x - a.x) * (a.y - y);
            int k2 = (b.x - x) * (c.y - b.y) - (c.x - b.x) * (b.y - y);
            int k3 = (c.x - x) * (a.y - c.y) - (a.x - c.x) * (c.y - y);
            if(((SIGNUM(k1)) == (SIGNUM(k2))) && ((SIGNUM(k2)) == (SIGNUM(k3)))){
                PaintPixel(image, x, y, fill_color);
            }
        }
    }
}


void PaintRectangle(PngImage* image, Point left_up_corner, Point right_bottom_corner, Color color){
    for(int x = left_up_corner.x; x <= right_bottom_corner.x; x++){
        for(int y = left_up_corner.y; y <= right_bottom_corner.y; y++){
            PaintPixel(image, x, y, color);
        }
    }
}


void MakeCollage(PngImage* image, int x_photos, int y_photos){
    int new_width = image->width * x_photos;
    int new_height = image->height * y_photos;
    int width_pixel = n_color_channels(image);
    png_byte** new_pixel_arr = (png_byte**) malloc(sizeof(png_byte*) * new_height);
    for (int y = 0; y < new_height; y++)
        new_pixel_arr[y] = (png_byte*) malloc(sizeof(png_byte) * new_width * width_pixel);
    for(int x_photo = 0; x_photo < x_photos; x_photo++){
        for(int y_photo = 0; y_photo < y_photos; y_photo++){
            Point src_left_up = {0, 0};
            Point src_right_bottom = {image->width - 1, image->height - 1};
            Point dest_left_up = {x_photo * image->width, y_photo * image->height};
            copy_area(new_pixel_arr, image->row_pointers, src_left_up, src_right_bottom, dest_left_up, width_pixel);
        }
    }
    for(int i = 0; i < image->height; i++){
        free(image->row_pointers[i]);
    }
    free(image->row_pointers);
    image->width = new_width;
    image->height = new_height;
    image->row_pointers = new_pixel_arr;
}


void FindMaxRectangleAndRepaint(PngImage* image, Color old_color, Color new_color){
    int max_square = 0;
    Point left_up = {0, 0};
    Point right_bottom = {0, 0};
    for(int y = 0; y < image->height; y++){
        for(int x = 0; x < image->width; x++){
            Color pixel_color = GetPixelColor(image, x, y);
            if(colors_equal(pixel_color, old_color)){
                int init_h_border = get_horizontal_color_border(image, x, y, old_color);
                int init_v_border = get_vertical_color_border(image, x, y, old_color);
                int min_h_border = init_h_border;
                int v_border_for_min_h_border = y;
                int cur_square;
                for(int i = y; i <= init_v_border; i++){
                    int cur_h_border = get_horizontal_color_border(image, x, i, old_color);
                    if(cur_h_border >= min_h_border){
                        cur_square = (i - v_border_for_min_h_border + 1) * (min_h_border - x + 1);
                    }
                    else{
                        min_h_border = cur_h_border;
                        v_border_for_min_h_border = i;
                        cur_square = min_h_border - x + 1;
                    }
                    if(cur_square > max_square){
                        max_square = cur_square;
                        left_up.x = x;
                        left_up.y = v_border_for_min_h_border;
                        right_bottom.x = min_h_border;
                        right_bottom.y = i;
                    }
                }
            }
        }
    }
    PaintRectangle(image, left_up, right_bottom, new_color);
}


void FillWithArea(PngImage* image, Point left_up, Point bottom_right){
    int width_pixel = n_color_channels(image);
    int area_width = bottom_right.x - left_up.x + 1;
    int area_height = bottom_right.y - left_up.y + 1;
    png_byte** _area = malloc(sizeof(png_byte*) * area_height);
    for(int i = 0; i < area_height; i++){
        _area[i] = malloc(sizeof(png_byte) * area_width * width_pixel);
    }
    Point dest_left_up = {0, 0};
    copy_area(_area, image->row_pointers, left_up, bottom_right, dest_left_up, width_pixel);
    // fill image ----------------------
    int remainder_pixels_x = image->width % area_width;
    int remainder_pixels_y = image->height % area_height;
    for(int i = 0; i <= (image->height / area_height); i++){
        for(int j = 0; j < (image->width / area_width); j++){
            Point src_left_up = {0, 0};
            Point src_right_bottom = {area_width - 1, area_height - 1};
            if(i == (image->height / area_height)){
                src_right_bottom.y = remainder_pixels_y - 1;
            }
            Point dest_left_up = {area_width * j, area_height * i};
            copy_area(image->row_pointers, _area, src_left_up, src_right_bottom, dest_left_up, width_pixel);
        }
        if(remainder_pixels_x > 0){
            Point src_left_up = {0, 0};
            Point src_right_bottom = { remainder_pixels_x - 1, area_height - 1};
            if(i == (image->height / area_height)){
                src_right_bottom.y = remainder_pixels_y - 1;
            }
            Point dest_left_up = {image->width -  remainder_pixels_x, area_height * i};
            copy_area(image->row_pointers, _area, src_left_up, src_right_bottom, dest_left_up, width_pixel);
        }
    }
    for(int i = 0; i < area_height; i++){
        free(_area[i]);
    }
    free(_area);
}