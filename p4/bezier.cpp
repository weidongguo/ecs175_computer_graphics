#include "bezier.h"

Bezier::Bezier(Graph *_graph, Point *_ctrlPoints, int _numberOfCtrlPoints)
:Curve(_graph, _ctrlPoints, _numberOfCtrlPoints)
{

}

void Bezier::printAttributes(){
  printf("\n==Bezier Curve==\n");
  printf("degree n : %d\n", numberOfCtrlPoints - 1);
  for(std::list<Point>::iterator it = ctrlPoints.begin(); it != ctrlPoints.end(); ++it){
    printf("(%.2f, %.2f)\n", (*it).x, (*it).y);
  }
}


