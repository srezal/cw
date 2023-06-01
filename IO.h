#include "Config.h"
#include "GraphicsStructs.h"


void print_help();
void print_image_info(PngImage* image);
void print_success(char* message);
void print_error(char* message);
void print_warning(char* message);
Config parse_opts(int argc, char **argv);