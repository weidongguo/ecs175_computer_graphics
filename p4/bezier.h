#ifndef BEZIER_H
#define BEZIER_H
#include "curve.h"
class Bezier:public Curve {
  public:
    Bezier(Graph*graph, Point_2D *ctrlPoints, int numberOfCtrlPoints);        
    void printAttributes();
    Point_2D cc(float t);
    void drawCurve(float res, Color c);
    void drawControlPolygon();
};

#endif
