#include "common_type.h"
#include "graph.h"

class Line{
  Point pairOfPoints[2]; // straight line consist of two points only
  Point pairOfPointsOriginal[2]; // //original two points 
  Point pairOfPointsAfterClipping[2]; //new line after clipping
  Graph *graph; // Graph *graph the object contains all drawing method
  Color color;

  public: 
  Line(Point p1, Point p2, Graph *g); //constructor of a line only requires two points; 
  void setColor(float r, float g, float b); 
  void draw();// draw your self a line
  void clip();
  void cohenSutherland();//
};
