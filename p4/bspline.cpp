#include "bspline.h"

Bspline::Bspline(Graph *_graph, Point *_ctrlPoints, int _numberOfCtrlPoints, int _k, float *knotValues)
:Curve(_graph, _ctrlPoints, _numberOfCtrlPoints)
{
  
  k = _k;

}

