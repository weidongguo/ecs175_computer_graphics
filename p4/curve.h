#ifndef CURVE_H
#define CURVE_H

#include "common_type.h"

class Curve {
  Point *ctrlPoints;
  int numberOfCtrlPoints;

  public:
  Curve(Point *_ctrlPoints, int _numberOfCtrlPoints);
  Curve(const Curve &curve); //copy constructor
  ~Curve();

};
#endif

