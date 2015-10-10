#include "common_type.h"
#include "graph.h"

class Line{
  Point *pairOfPoints; // pointer to the current pair of points
  Point pairOfPointsOriginal[2]; // //original two points 
  Point pairOfPointsAfterClipping[2]; //new line after clipping
  Graph *graph; // Graph *graph the object contains all drawing method
  Color color;

  public: 
  Line(Point p1, Point p2, Graph *g); //constructor of a line only requires two points; 
  void setColor(float r, float g, float b); 
  void draw();// draw your self a line
  void clip(int, int, int, int);
  int  cohenSutherland(int xMin, int xMax, int yMin, int yMax);
};

//Helper functions for the conhenSutherland line clipping algorithm
uint8 encode(Point p, int xMin, int xMax, int yMin, int yMax);
bool inside(uint8 region_code); //inside the clipping region
bool reject(uint8 region_code1, uint8 region_code2); 
bool accept(uint8 region_code1, uint8 region_code2); //accept for sure
void swapPoints(Point *p1, Point *p2);



