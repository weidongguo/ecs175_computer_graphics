#include "bspline.h"

Bspline::Bspline(Graph *_graph, Point_2D *_ctrlPoints, int _numberOfCtrlPoints, int _k, float *_knotValues)
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

  for(std::list<Point_2D>::iterator it = ctrlPoints.begin(); it!=ctrlPoints.end(); ++it){
    printf("(%.2f, %.2f)\n", (*it).x, (*it).y) ;
  }
  
  printf("Knot Values: "); 
  for(int i = 0 ; i < numberOfCtrlPoints + k; i++){
    printf("%.2f ", knotValues[i]); 
  }
  printf("\n");
}

void Bspline::drawCurve(float res, Color c){
  float inc = 1.0/res;
  Point_2D p1, p2;
  for(float u = knotValues[k-1] ; u <= knotValues[numberOfCtrlPoints]-inc; u=u+inc){
    p1 = cc(u);
    p2 = cc(u+inc);
    DPRINT("Bspline: (%.2f, %.2f) @ u: %.2f\n", p1.x, p1.y, u);
    graph->drawLine(p1, p2, c);
  }
}

Point_2D Bspline::cc(float u){
    std::list<Point_2D>::iterator it = ctrlPointsNDC.begin();
    float leftRatio, rightRatio; 
    int I;

    //build a table 
    Point_2D lb[numberOfCtrlPoints][numberOfCtrlPoints], c_t;
    for(int c = 0; c < numberOfCtrlPoints; c++, it++){
      lb[0][c] = (*it);        
    } //init the 0th generation(j=0) control points

    //determine which segment I does the u lies in
    for(int i = 0 ; i < k+numberOfCtrlPoints; i++){
      if( u < knotValues[i]){
              I = i-1;
              break;
      }
    }
    //DPRINT("I: %d\n",I); 
    for(int j = 1 ; j <= k-1; j++){ //starting from the first generation
      for(int i = (I - (k-1)) ; i <= I-j; i++){
        rightRatio = (knotValues[i+k] - u) / (knotValues[i+k] - knotValues[i+j]) ;
        leftRatio  = (u - knotValues[i+j]) / (knotValues[i+k] - knotValues[i+j]) ;
        lb[j][i] = add( multByScalar( lb[j-1][i], rightRatio ) ,  multByScalar( lb[j-1][i+1], leftRatio ) );            
      }
    }
    
    c_t.x = lb[k-1][I-(k-1)].x * (graph->window_width - 1); 
    c_t.y = lb[k-1][I-(k-1)].y * (graph->window_height -1);
    return c_t;
}


void Bspline::drawControlPolygon(){
  Point_2D p1, p2;
  for(std::list<Point_2D>::iterator it = ctrlPointsNDC.begin(); it != ctrlPointsNDC.end(); ){
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
