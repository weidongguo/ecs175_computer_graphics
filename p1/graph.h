#ifndef GRAPH_H
#define GRAPH_H

#include <cstdio>
#include <algorithm>
#include <cmath>

#include "polygon.h"
#include "common_type.h"

#define WINDOW_WIDTH 1000 
#define WINDOW_HEIGHT 500 


class Graph {
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
    int drawPolygon(Polygon &poly); // have to pass pointer or reference down for Polygon because
                                    // if i pass a copy down, it will make a copy of the Object, so
                                    // when the local copy is out of scope, it will trigger the destructor 
                                    // and delete[] the memory allocated before. When the actual copy is out of scope
                                    // it will trigger its destructor and try to delete[] the same piece of memory that
                                    // has already been freed previosly. This will trigger the double freeing error 
};

#endif
