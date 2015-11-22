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

void Bezier::drawCurve(float res){
  float inc = 1.0/res;
  for(float t = 0; t+inc <= 1.0; t+=inc){
    //DPRINT("%.2f to %.2f\t", t, t+inc);
    Point p1 = cc(t);
    Point p2 = cc(t+inc);
    graph->drawLine( p1, p2, {0,0,0}) ;
    //DPRINT( "(%.2f, %.2f), (%.2f, %.2f)\n",p1.x, p1.y, p2.x, p2.y); 
  }
  
}

Point Bezier::cc(float t){
    std::list<Point>::iterator it = ctrlPointsNDC.begin();

    //build a table 
    Point lb[numberOfCtrlPoints][numberOfCtrlPoints], c_t;
    for(int c = 0; c < numberOfCtrlPoints; c++, it++){
      lb[0][c] = (*it);        
    }

    for(int j = 1 ; j < numberOfCtrlPoints; j++){
      for(int i = 0 ; i < numberOfCtrlPoints-j; i++){
        lb[j][i] = add( multByScalar( lb[j-1][i], (1.0-t) ) ,  multByScalar( lb[j-1][i+1], t ) );            
      }
    }
    
    c_t.x = lb[numberOfCtrlPoints-1][0].x * (graph->window_width - 1); 
    c_t.y = lb[numberOfCtrlPoints-1][0].y * (graph->window_height -1);
    return c_t;
}

void Bezier::drawControlPolygon(){
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
    graph->drawLine(p1, p2, COLOR_ROYAL_BLUE);
  }
}
