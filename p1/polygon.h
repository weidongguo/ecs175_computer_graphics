#ifndef POLYGON_H
#define POLYGON_H

#include "common_type.h"
#include "graph.h"
#include <list>

class Polygon{
  friend class Graph ;
  Point *listOfPoints;  // the original points for creating the shape
  int numberOfPoints;
  Color color; 
  Graph *graph;
  Point centroid;
  std::list<Point> listOfContourPoints[WINDOW_HEIGHT];//the original points + the points along the lines between the original points
                                                      // the y-value is used for indexing contour points at each scanline

  public:
    Polygon(Point * listOfPts, int numOfPoints, Graph *ptr_graph);
    ~Polygon(); 
    void setColor(Color);
    void setCentroid();
    void draw();
    void scale(float alpha, float beta);//factor alpha for scalling the x-value, factor beta for scalling the y-value
    void rotate(float angle);
    void translate(int x, int y); //translating meaning shifting
    void rasterize();
    void _storeContourPoint(int x, int y);
    int  _storeLinePoints( Point p1, Point p2);
    int  _bresenham(Point pt1, Point pt2);
    void storeContourPoints();
    void printListOfContourPoints();
};
#endif
