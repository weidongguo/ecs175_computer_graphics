#ifndef CURVE_H
#define CURVE_H

#include "common_type.h"
#include "graph.h"
#include <list>

class Curve {
  protected: 
  Graph *graph;
  std::list<Point> ctrlPoints;
  std::list<Point> ctrlPointsNDC;
  int numberOfCtrlPoints;

  public:
  Curve(Graph *_graph, Point *_ctrlPoints, int _numberOfCtrlPoints);
  void print();
  virtual void printAttributes() = 0;
  virtual void drawControlPolygon() = 0; 
  virtual void drawCurve(float res) = 0;

  static void normalizeCtrlPoints(std::list<Curve*> *curves);
  static void findNDCParam(std::list<Curve*> *curves, float*xMin, float*yMin, float *delta);
};
#endif

