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

void Bspline::drawCurve(float res){

}

void Bspline::drawControlPolygon(){
  Point p1, p2;
  for(std::list<Point>::iterator it = ctrlPointsNDC.begin(); it != ctrlPointsNDC.end(); ){
    p1 = (*it);
    it++;
    if(it == ctrlPointsNDC.end())
            break;
    p2 = (*it);          
    
    p1.x = p1.x * (graph->window_width - 1);
    p1.y = p1.y * (graph->window_height -1);
    p2.x = p2.x * (graph->window_width - 1);
    p2.y = p2.y * (graph->window_height -1);

    graph->drawLine(p1,p2,COLOR_CRIMSON);
  }

}
