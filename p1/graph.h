#ifndef GRAPH_H
#define GRAPH_H

#include <cstdio>
#include <algorithm>
#include <cmath>

//#include "polygon.h"
#include "common_type.h"

#define WINDOW_WIDTH 1000 
#define WINDOW_HEIGHT 500 


class Graph {
  friend class Polygon; 
  int window_width;
  int window_height;
  Color background_color;
 
  public:
    Graph(int width, int height, float* PixelBufferPtr);
    float *PixelBuffer;
    int drawPixel(int x, int y, float r, float g, float b); 
    int drawLine(Point p1, Point p2, float r, float g, float b);
    int dda(Point p1, Point p2, float r, float g, float b);
    int bresenham(Point p1, Point p2, float r, float, float);
    int fillScreen(float r, float g, float b);
    int drawPolygon( Point *listOfPoints, int numberOfPoints, float r, float g, float b);
};

#endif
