#include <cstdio>
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
  int translation_x;
  int translation_y;
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
} Window;

#define MENU_DDA 2
#define MENU_BRESENHAM 3
#define MENU_STATUS 4
#endif
