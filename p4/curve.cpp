#include "curve.h"
#include <cstring>
Curve::Curve(Graph *_graph, Point_2D *_ctrlPoints, int _numberOfCtrlPoints){
  graph = _graph; //for making pixels to screen (line drawing, etc)
  for(int i = 0 ; i < _numberOfCtrlPoints; i++){
    ctrlPoints.push_back(_ctrlPoints[i]);
    ctrlPointsNDC.push_back(_ctrlPoints[i]);//init first, later normalize it
  }
  
  numberOfCtrlPoints = _numberOfCtrlPoints;
}

void Curve::print(){
  printf("Curve::Print()\n");
}

void Curve::normalizeCtrlPoints(std::list<Curve*> *curves){ //static method
  float xMin, yMin, delta;
  std::list<Point_2D>::iterator itp;

  findNDCParam(curves, &xMin, &yMin, &delta);
  DPRINT("xMin: %.2f, yMin: %.2f, delta: %.2f\n", xMin, yMin, delta);
 
  for(std::list<Curve*>::iterator itc = curves->begin(); itc!=curves->end(); itc++){
    for(itp = (*itc)->ctrlPointsNDC.begin() ; itp != (*itc)->ctrlPointsNDC.end(); itp++){
      (*itp).x = ( (*itp).x - xMin ) / delta; //normalize the x value
      (*itp).y = ( (*itp).y - yMin ) / delta; //normalize the y value
      DPRINT("Normalized (%.2f, %.2f)\n", (*itp).x, (*itp).y);
    }
  }
}

void Curve::findNDCParam(std::list<Curve*> *curves, float*_xMin, float*_yMin, float *delta){ //static method
  static bool isInit = true; 
  std::list<Point_2D> ctrlPoints;
  std::list<Point_2D>::iterator itp;
  float xMin, yMin, xMax, yMax; 

  for(std::list<Curve*>::iterator itc = curves->begin(); itc!=curves->end(); itc++){
    ctrlPoints = (*itc)->ctrlPoints;
    for(itp = ctrlPoints.begin() ; itp != ctrlPoints.end(); itp++){
      if(isInit){
        xMin = xMax = (*itp).x;
        yMin = yMax = (*itp).y; 
        isInit = false;        
      }
      else{
        if( (*itp).x < xMin )
                xMin = (*itp).x;
        else if( (*itp).x > xMax )
                xMax = (*itp).x;
        if( (*itp).y < yMin)
                yMin = (*itp).y;
        else if( (*itp).y > yMax)
                yMax = (*itp).y;
      }
    }
  }
  *_xMin = xMin;
  *_yMin = yMin;
  *delta = std::max(xMax-xMin, yMax-yMin);
}

