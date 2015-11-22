#ifndef BSPLINE_H
#define BSPLINE_H
#include "curve.h"

class Bspline:public Curve{
  int k; // k value is the order of the curve, which specifies the number of point defining one segment 
  float *knotValues;

  public:
  Bspline(Graph *_graph, Point_2D *_ctrlPoints, int _numberOfCtrlPoints, int _k, float *knotValues);
  void printAttributes();// implement a virtual method of its parent, dyn.binding
  void drawControlPolygon();
  void drawCurve(float res);
};

#endif
