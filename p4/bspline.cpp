#include "bspline.h"

Bspline::Bspline(Graph *_graph, Point *_ctrlPoints, int _numberOfCtrlPoints, int _k, float *_knotValues)
:Curve(_graph, _ctrlPoints, _numberOfCtrlPoints)
{
  k = _k;
  knotValues = new float[_numberOfCtrlPoints + _k];// n+1+k
  memcpy(knotValues, _knotValues, sizeof(float)*(_numberOfCtrlPoints+_k));

}

void Bspline::printAttributes(){
  printf("\n==B-Spline curve==\n");
  printf("degree n: %d\n", numberOfCtrlPoints - 1);
  printf("K value : %d\n", k);

  for(std::list<Point>::iterator it = ctrlPoints.begin(); it!=ctrlPoints.end(); ++it){
    printf("(%.2f, %.2f)\n", (*it).x, (*it).y) ;
  }
  
  printf("Knot Values: "); 
  for(int i = 0 ; i < numberOfCtrlPoints + k; i++){
    printf("%.2f ", knotValues[i]); 
  }
  printf("\n");
}

