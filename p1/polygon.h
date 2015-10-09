#ifndef POLYGON_H
#define POLYGON_H

#include "common_type.h"
#include "graph.h"
#include <list>

class Polygon{
  friend class Graph ;
  Point *listOfPoints; 
  Point *listOfPointsOriginal;
  Point *listOfPointsAfterClipping;
  int numberOfPointsOriginal;
  int numberOfPointsAfterClipping;
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
    void rasterize(float r, float g, float b);
    void _storeContourPoint(int x, int y);
    int  _storeLinePoints( Point p1, Point p2);
    int  _bresenham(Point pt1, Point pt2);
    void storeOriginalPointsToContourPoints(); //just add the original points to the contour points buffer(std::list<Point> listOfCOntourPoints);
    void storeContourPoints();// all the points along the contour of the polygon
    void clearContourPoints();
    void printListOfContourPoints();
    void clip(int xMin, int yMin, int xMax, int yMax);
    void clipXMin(int xMin);
    void clipYmin(int yMin);
    void ClipXMax(int xMax);
    void clipYMax(int yMax);
};
#endif
