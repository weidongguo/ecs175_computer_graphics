#include "curve.h"
#include <cstring>
Curve::Curve(Graph *_graph, Point *_ctrlPoints, int _numberOfCtrlPoints){
  graph = _graph; //for making pixels to screen (line drawing, etc)
  for(int i = 0 ; i < _numberOfCtrlPoints; i++)
    ctrlPoints.push_back(_ctrlPoints[i]);
  
  numberOfCtrlPoints = _numberOfCtrlPoints;
}

