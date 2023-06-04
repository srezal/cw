#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "Config.h"
#include "IO.h"
#include "GraphicsUtils.h"


#define ERR_CODE 1
#define SUCCESS_CODE 0


int parse_point(char* arg, Point* point){
    if((sscanf(arg, "%d,%d", &point->x, &point->y) != 2)){
        print_error("Недостаёт координаты!");
        return ERR_CODE;
    }
    return SUCCESS_CODE;
}


int parse_color(char* arg, int* r, int* g, int* b){
    if((sscanf(arg, "%d,%d,%d", r, g, b) != 3)){
        print_error("Вы забыли указать цветовой(-ые) канал(-ы)!");
        return ERR_CODE;
    }
    if(check_color(*r, *g, *b) == false){
        print_error("Одна(несколько) из компонент цвета имеет(-ют) недопустимое значение!\n"
                    "Цветовой канал может принимать значения в диапозоне 0-255.");
        return ERR_CODE;
    }
    return SUCCESS_CODE;
}


void cleanup_config_allocation(Config* config){
    if(config->point1 != NULL) free(config->point1);
    if(config->point2 != NULL) free(config->point2);
    if(config->point3 != NULL) free(config->point3);
    if(config->color1 != NULL) free(config->color1);
    if(config->color2 != NULL) free(config->color2);
}


void set_config(Config* config, int opt, char* arg, int optopt){
    Point point;
    int r, g, b;
    switch(opt){
        case 'h':
            print_help();
            cleanup_config_allocation(config);
            exit(1);
        case 'a':
            if(parse_point(arg, &point) == ERR_CODE){
                cleanup_config_allocation(config);
                exit(1);
            }
            config->point1 = malloc(sizeof(Point));
            *config->point1 = point;
            break;
        case 'b':
            if(parse_point(arg, &point) == ERR_CODE){
                cleanup_config_allocation(config);
                exit(1);
            }
            config->point2 = malloc(sizeof(Point));
            *config->point2 = point;
            break;
        case 'c':
            if(parse_point(arg, &point) == ERR_CODE){
                cleanup_config_allocation(config);
                exit(1);
            }
            config->point3 = malloc(sizeof(Point));
            *config->point3 = point;
            break;
        case 't':
            config->line_thickness = atoi(arg);
            break;
        case 'l':
            if(parse_color(arg, &r, &g, &b) == ERR_CODE){
                cleanup_config_allocation(config);
                exit(1);
            }
            config->color1 = malloc(sizeof(Color));
            config->color1->r = (unsigned short) r;
            config->color1->g = (unsigned short) g;
            config->color1->b = (unsigned short) b;
            config->color1->a = 255;
            break;
        case 'i':
            config->is_fill = true;
            break;
        case 'f':
            if(parse_color(arg, &r, &g, &b) == ERR_CODE){
                cleanup_config_allocation(config);
                exit(1);
            }
            config->color2 = malloc(sizeof(Color));
            config->color2->r = (unsigned short) r;
            config->color2->g = (unsigned short) g;
            config->color2->b = (unsigned short) b;
            config->color2->a = 255;
            break;
        case 'x':{
            int x_photos = atoi(arg);
            if(x_photos < 1){
                print_error("Количество фотографий по оси Х не может быть меньше 1!");
                cleanup_config_allocation(config);
                exit(1);
            }
            config->x_photos = x_photos;
            break;
        }
        case 'y':{
            int y_photos = atoi(arg);
            if(y_photos < 1){
                print_error("Количество фотографий по оси Y не может быть меньше 1!");
                cleanup_config_allocation(config);
                exit(1);
            }
            config->y_photos = y_photos;
            break;
        }
        case '?':{
            char warning_msg[100];
            sprintf(warning_msg, "Встречен неизвестный флаг: \'-%c\'", (char) optopt);
            print_warning(warning_msg);
            break;
        }
    }
}


bool check_is_full_config_for_func(char* FUNC, Config* config){
    if(!strcmp(FUNC, "line")){
        if(config->point1 == NULL){
            print_error("Вы забыли указать координаты конца отрезка!");
            return false;
        }
        else if(config->point2 == NULL){
            print_error("Вы забыли указать координаты конца отрезка!");
            return false;
        }
        else if(config->color1 == NULL){
            print_error("Вы забыли указать цвет линии!");
            return false;
        }
        else if(config->line_thickness == 0){
            print_error("Вы забыли указать толщину линии!");
            return false;
        }
    }
    else if(!strcmp(FUNC, "triangle")){
        if(config->point1 == NULL){
            print_error("Вы забыли указать координаты вершины!");
            return false;
        }
        else if(config->point2 == NULL){
            print_error("Вы забыли указать координаты вершины!");
            return false;
        }
        else if(config->point3 == NULL){
            print_error("Вы забыли указать координаты вершины!");
            return false;
        }
        else if(config->color1 == NULL){
            print_error("Вы забыли указать цвет линий!");
            return false;
        }
        else if(config->is_fill == true && config->color2 == NULL){
            print_error("Вы забыли указать цвет для заливки!");
            return false;
        }
        else if(config->line_thickness == 0){
            print_error("Вы забыли указать толщину линий!");
            return false;
        }
    }
    else if(!strcmp(FUNC, "rectangle")){
        if(config->color1 == NULL){
            print_error("Вы забыли указать цвет искомого прямоугольника!");
            return false;
        }
        else if(config->color2 == NULL){
            print_error("Вы забыли указать цвет для заливки!");
            return false;
        }
    }
    else if(!strcmp(FUNC, "collage")){
        if(config->x_photos == 0){
            print_error("Вы забыли указать количество фотографий по оси X!");
            return false;
        }
        else if(config->y_photos == 0){
            print_error("Вы забыли указать количество фотографий по оси Y!");
            return false;
        }
    }
    else{
        print_error("К сожалению, у нашей программы нет функции с таким названием!");
        return false;
    }
    return true;
}