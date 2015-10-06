#include <cstdio>
#include <algorithm>
#include <cmath>

#ifndef GRAPH_H
#define GRAPH_H

#define WINDOW_WIDTH 1000 
#define WINDOW_HEIGHT 500 

#ifdef DEBUG_PRINT
  #define DPRINT printf
#else
  #define DPRINT(...) 
#endif

#define MIN(x,y) ((x<=y)?x:y)
#define MAX(x,y) ((x>y)?x:y)

typedef struct {
  int x;
  int y;
} Point; 

class Graph {
  int window_width;
  int window_height;
  
  public:
    Graph(int width, int height, float* PixelBufferPtr);
    float *PixelBuffer;
    int drawPixel(int x, int y, float r, float g, float b); 
    int drawLine(Point p1, Point p2, float r, float g, float b);
    int dda(Point p1, Point p2, float r, float g, float b);
    int bresenham(Point p1, Point p2, float r, float, float);
    int fillScreen(float r, float g, float b);
};


#endif
