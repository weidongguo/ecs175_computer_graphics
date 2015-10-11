#ifndef POLYGON_H
#define POLYGON_H

#include "common_type.h"
#include "graph.h"
#include "line.h"
#include <list>

#define LEFT 0
#define RIGHT 1
#define BOTTOM 2
#define TOP 3
typedef int Boundary;

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
  std::list<Point> *listOfContourPoints;//the original points + the points along the lines between the original points
                                                      // the y-value is used for indexing contour points at each scanline
  bool isRasterized;

  public:
    Polygon(Point * listOfPts, int numOfPoints, Graph *ptr_graph);
    ~Polygon(); 
    void setColor(Color);
    void setCentroid();
    void draw();
    void draw(Color color);
    void draw(float r, float g, float b);
    void scale(float alpha, float beta);//factor alpha for scalling the x-value, factor beta for scalling the y-value
    void rotate(float angle);
    void translate(int x, int y); //translating meaning shifting
    void rasterize(float r, float g, float b);
    void rasterize(Color color);
    void rasterize();
    void _storeContourPoint(int x, int y);
    int  _storeLinePoints( Point p1, Point p2);
    int  _bresenham(Point pt1, Point pt2);
    void storeOriginalPointsToContourPoints(); //just add the original points to the contour points buffer(std::list<Point> listOfCOntourPoints);
    void storeContourPoints();// all the points along the contour of the polygon
    void clearContourPoints();
    void printListOfContourPoints();
    void clip(int xMin, int xMax, int yMin, int yMax);
    void clip(ClipRegion cr);
    void sutherlandHodgman(int xMin, int xMax, int yMin, int yMax);
    void displayClippingRegion(int xMin, int xMax, int yMin, int yMax);
    
    //helper functions for sutherlandHodgman();   
    static bool inside(Point p, Boundary b, int xMin, int xMax, int yMin, int yMax);
    static bool cross(Point p1, Point p2, Boundary b, int xMin, int xMax, int yMin, int yMax); 
    static Point intersect(Point p1, Point p2, Boundary b, int xMin, int xMax, int yMin, int yMax);
};

#endif
