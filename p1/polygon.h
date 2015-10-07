#ifndef POLYGON_H
#define POLYGON_H

#include "common_type.h"

class Polygon{
  friend class Graph ;
  Point *listOfPoints;  
  int numberOfPoints;
  Color color; 
  
  public:
    Polygon(Point * listOfPts, int numOfPoints );
    ~Polygon(); 
    void setColor(Color);
    void draw();
};
#endif
