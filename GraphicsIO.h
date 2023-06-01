#pragma once
#include "GraphicsStructs.h"


void cleanup_image_allocation(PngImage *image);
void read_png_file(char *file_name, PngImage *image);
void write_png_file(char *file_name, PngImage *image);