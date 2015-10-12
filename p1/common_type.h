#include <cstdio>
#include <string>
#ifdef DEBUG_PRINT
  #define DPRINT printf
#else
  #define DPRINT(...) 
#endif

#define MIN(x,y) ((x<=y)?x:y)
#define MAX(x,y) ((x>y)?x:y)

#ifndef COMMON_TYPE_H
#define COMMON_TYPE_H

typedef unsigned char uint8;
typedef char          int8; 

typedef struct {
  int x;
  int y;
} Point; 

typedef Point Vertex;//Point and Vertex means the same thing

typedef struct {
  float r;
  float g;
  float b;
}Color;

typedef struct{
  int xMin;
  int xMax;
  int yMin;
  int yMax;
}ClipRegion;

typedef struct{
  int x_offset;
  int y_offset;
  float scale_alpha;
  float scale_beta;
  float rotation_angle;
}Transformation;

typedef struct{
  int width;
  int height;
  int numberOfPolygons;
  int selectedObject;
  ClipRegion cr;
  Transformation tf;   
  int state;
  std::string *inputBuffer;
} Window;

// for window
#define STATE_GRAB_COMMANDS 0
#define STATE_GRAB_DATA_ROTATION_ANGLE    1
#define STATE_GRAB_DATA_TRANSLATION_FACTORS 2
#define STATE_GRAB_DATA_SCALE_FACTORS 3

//for menu
#define MENU_DDA                      2
#define MENU_BRESENHAM                3
#define MENU_STATUS                   4
#define MENU_GRAB_ROTATION_ANGLE      5
#define MENU_GRAB_TRANSLATION_FACTORS 6
#define MENU_GRAB_SCALE_FACTORS       7
#define MENU_GRAB_CLIP_REGION         8
#endif
