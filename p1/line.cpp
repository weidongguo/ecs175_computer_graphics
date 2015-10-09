#include "line.h"


Line::Line(Point p1, Point p2, Graph *g){
  pairOfPoints[0] = pairOfPointsOriginal[0] = p1;
  pairOfPoints[1] = pairOfPointsOriginal[1] = p2;
  graph = g;
  color.r = 0;//default color is BLACK
  color.g = 0;
  color.b = 0;
}

void Line::setColor(float r, float g, float b){
  color.r = r;
  color.g = g;
  color.b = b;
}

void Line::draw(){
  graph->drawLine(pairOfPoints[0], pairOfPoints[1], color.r, color.g, color.b);
}
