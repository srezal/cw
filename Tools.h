#include <stdbool.h>
#include "GraphicsStructs.h"
#include "Color.h"
#include "Point.h"

void DrawLine(PngImage* image, Point start, Point end, int thickness, Color color, bool is_rounded);
void DrawTriangle(PngImage* image, Point a, Point b, Point c, int thickness, Color border_color);
void PaintCircle(PngImage* image, Point O, double r, Color color);
void PaintSquare(PngImage* image, Point O, int side_length, Color color);
void Fill(PngImage* image, int x, int y, Color fill_color);
void FillTriangle(PngImage* image,  Point a, Point b, Point c, Color fill_color);
void MakeCollage(PngImage* image, int x_photos, int y_photos);
void FindMaxRectangleAndRepaint(PngImage* image, Color old_color, Color new_color);