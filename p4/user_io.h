#ifndef USER_IO_H
#define USER_IO_H
#include <string>
#include <cstring>
#include <cstdlib>
#include <fstream>
#include "common_type.h"
#include "polyhedron.h"
#include <iostream>


void readHeaders(std::ifstream *ifs,  int *numberOfPolygons);
//void readPolygons(std::ifstream *ifs, Graph *graph, Polygon **polygons, int numberOfPolygons);
void readPolyhedra(std::ifstream *ifs, Graph **graphs, Polyhedron **polyhedra, int numberOfPolyhedra); 
void readFile(std::ifstream &ifs);
// for window
#define STATE_GRAB_COMMANDS                 0
#define STATE_GRAB_DATA_ROTATION_ANGLE      1
#define STATE_GRAB_DATA_TRANSLATION_FACTORS 2
#define STATE_GRAB_DATA_SCALE_FACTORS       3
#define STATE_GRAB_DATA_CLIP_REGION         4
#define STATE_GRAB_DATA_DRAW_DDA            5
#define STATE_GRAB_DATA_DRAW_BRESENHAM      6
#define STATE_HALF_TONE_ON                  7
#define STATE_HALF_TONE_OFF                 8

//for menu
#define MENU_CLIP_DDA                      0
#define MENU_CLIP_BRESENHAM                1
#define MENU_DRAW_DDA                      2
#define MENU_DRAW_BRESENHAM                3
#define MENU_STATUS                   4
#define MENU_GRAB_ROTATION_ANGLE      5
#define MENU_GRAB_TRANSLATION_FACTORS 6
#define MENU_GRAB_SCALE_FACTORS       7
#define MENU_GRAB_CLIP_REGION         8
#define MENU_HALF_TONE_TOGGLE         9
#define MENU_SET_HALF_TONE_COLOR     10
#define MENU_SET_PHONG_PARAMETERS    11

#endif
