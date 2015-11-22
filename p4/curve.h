#ifndef CURVE_H
#define CURVE_H

#include "common_type.h"
#include "graph.h"
#include <list>

class Curve {
  protected: 
  Graph *graph;
  std::list<Point> ctrlPoints;
  int numberOfCtrlPoints;

  public:
  Curve(Graph *_graph, Point *_ctrlPoints, int _numberOfCtrlPoints);
  virtual void printAttributes() = 0;
};
#endif

