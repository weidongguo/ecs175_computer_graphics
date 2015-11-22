#ifndef BEZIER_H
#define BEZIER_H
#include "curve.h"
class Bezier:public Curve {
  public:
    Bezier(Graph*graph, Point *ctrlPoints, int numberOfCtrlPoints);        
        
};

#endif
