#include "curve.h"
#include <cstring>
Curve::Curve(Point *_ctrlPoints, int _numberOfCtrlPoints){
  ctrlPoints = new Point[_numberOfCtrlPoints];
  memcpy(ctrlPoints, _ctrlPoints, sizeof(Point) * _numberOfCtrlPoints);
  numberOfCtrlPoints = _numberOfCtrlPoints;
}

Curve::Curve(const Curve &curve){ //copy constructor
  int _numberOfCtrlPoints = curve.numberOfCtrlPoints;
  ctrlPoints = new Point[_numberOfCtrlPoints];
  memcpy(ctrlPoints, curve.ctrlPoints, sizeof(Point) * _numberOfCtrlPoints);
  numberOfCtrlPoints = _numberOfCtrlPoints;
}

Curve::~Curve(){
  if( ctrlPoints != 0 ){
    delete [] ctrlPoints;
    ctrlPoints = 0 ;
  }
}
