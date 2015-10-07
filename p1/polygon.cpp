#include "polygon.h"
#include <cstdlib>
Polygon::Polygon(Point * listOfPts, int numOfPts){
  listOfPoints = new Point[numOfPts]; 
  for(int i = 0; i < numOfPts; i++){ 
    listOfPoints[i].x = listOfPts[i].x;
    listOfPoints[i].y = listOfPts[i].y;
  }
  numberOfPoints = numOfPts; 
  color = {0,0,0} ;// be default black
}
Polygon::~Polygon(){
  DPRINT("DESTRUCTOR CALLED\n"); 
  if(listOfPoints != 0) {
    delete[] listOfPoints; 
    listOfPoints = 0; 
  }
}

void Polygon::setColor(Color c){
  color = c;
}

