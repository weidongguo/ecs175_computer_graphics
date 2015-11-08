#ifndef GRAPH_H
#define GRAPH_H

#include <cstdio>
#include <algorithm>
#include <cmath>
#include <cstring>
#include "vector.h"
#include "common_type.h"

class Graph {
  friend class Polygon; //not used
  friend class Line; //not used
  friend class Polyhedron ;
  int window_width;
  int window_height;
  public:Color background_color;
 
  public:
    Graph(int width, int height, float* PixelBufferPtr);
    float *PixelBuffer;
    float *PixelBufferBackup;
    int drawPixel(Point p);
    int drawPixel(int x, int y, float r, float g, float b); 
    int drawPixel(int x, int y, Color c);
    int drawLine(Point p1, Point p2); //using the color specified in each point
    int drawLine(Point p1, Point p2, float r, float g, float b);
    int drawLine(Point p1, Point p2, float r, float g, float b, int method);
    int dda(Point p1, Point p2, float r, float g, float b);
    int bresenham(Point p1, Point p2, float r, float, float);
    int bresenham(Point p1, Point p2);
    int fillScreen(float r, float g, float b);
    int drawPolygon( Point *listOfPoints, int numberOfPoints, float r, float g, float b);
    bool outOfBound(int x, int y);
    void drawMegaPixel(int numberOfPixels, int maxNumberOfPixels, int r, int c, Color k);
    void halfTone(Color color);
    Color readPixel(int x, int y);
    void  backupPixelBuffer();
    void  restorePixelBuffer();

};


void determineStartAndEndPoints(Point p1, Point p2, int*x, int*y, int *x_end, int*y_end);//helper function for bresenham algo
void swapXY(Point *p1);//swap the x value of the point with its y value:  helper funcion for bresenha algo

#define BRESENHAM 0
#define DDA 1

#endif
