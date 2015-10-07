#ifndef POLYGON_H
#define POLYGON_H

#include "common_type.h"
#include "graph.h"

class Polygon{
  friend class Graph ;
  Point *listOfPoints;  
  int numberOfPoints;
  Color color; 
  Graph *graph;
  Point centroid;

  public:
    Polygon(Point * listOfPts, int numOfPoints, Graph *ptr_graph);
    ~Polygon(); 
    void setColor(Color);
    void setCentroid();
    void draw();
    void scale(float alpha, float beta);//factor alpha for scalling the x-value, factor beta for scalling the y-value
    void rotate(float angle);
};
#endif
