#include <stdbool.h>
#include "GraphicsStructs.h"
#include "Color.h"
#include "Point.h"


int n_color_channels(PngImage* image);
Color GetPixelColor(PngImage* image, int x, int y);
void PaintPixel(PngImage* image, int x, int y, Color color);
void copy_area(png_byte** dest, png_byte** src, Point src_left_up, Point src_right_bottom, Point dest_left_up, int width_pixel);
int get_horizontal_color_border(PngImage* image, int x, int y, Color color);
int get_vertical_color_border(PngImage* image, int x, int y, Color color);