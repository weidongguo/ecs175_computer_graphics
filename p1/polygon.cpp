#include "polygon.h"
#include <cstdlib>
Polygon::Polygon(Point * listOfPts, int _numberOfPoints, Graph *ptr_graph){
  listOfPoints = new Point[_numberOfPoints];
  for(int i = 0; i < _numberOfPoints; i++) 
    listOfPoints[i] = listOfPts[i];
  
  numberOfPoints = _numberOfPoints;
  color = {0,0,0} ;// by default black
  graph = ptr_graph;
  setCentroid();//set once in the beginning
}
Polygon::~Polygon(){
  DPRINT("DESTRUCTOR CALLED\n"); 
  if(listOfPoints != 0) {
    delete[] listOfPoints; 
    listOfPoints = 0; 
  }
}


void Polygon::setCentroid(){
  int sumOfXs = 0, sumOfYs = 0; 
  for(int i = 0 ; i < numberOfPoints; i++){
    sumOfXs += listOfPoints[i].x;
    sumOfYs += listOfPoints[i].y;
  }//sum of all x's and y's
  
  centroid.x = sumOfXs / numberOfPoints;
  centroid.y = sumOfYs / numberOfPoints;
  DPRINT("centroid is (%d, %d)\n", centroid.x, centroid.y);
}

void Polygon::setColor(Color c){
  color = c;
}

void Polygon::draw(){
  graph->drawPolygon(listOfPoints, numberOfPoints, color.r, color.g, color.b);
}

void Polygon::scale(float alpha, float beta){
  float cx = (float)centroid.x,
        cy = (float)centroid.y,
        new_x, new_y, x, y;
  for(int i = 0 ; i< numberOfPoints; i++){
    x = listOfPoints[i].x;
    y = listOfPoints[i].y;
    new_x = cx - alpha*cx + alpha*x;
    new_y = cy - beta*cy + beta*y; 
    DPRINT("new points (%d, %d)\n", (int)new_x, (int)new_y);
    listOfPoints[i].x = (int)new_x; // must cast from float to integer
    listOfPoints[i].y = (int)new_y;
  }

}

#define PI 3.1415926
void Polygon::rotate(float alpha){ //alpha is the angle
  float cosAlpha = cos(alpha * PI/180),
        sinAlpha = sin(alpha * PI/180),
        cx = (float)centroid.x,
        cy = (float)centroid.y,
        new_x,
        new_y,
        x,y;

  for(int i = 0 ; i< numberOfPoints; i++){
    x = listOfPoints[i].x;
    y = listOfPoints[i].y;
    new_x = cx - cosAlpha*cx + cy*sinAlpha + cosAlpha*x - sinAlpha*y; 
    new_y = cy - cosAlpha*cy - cx*sinAlpha + cosAlpha*y + sinAlpha*x; 
    DPRINT("new points (%d, %d)\n", (int)new_x, (int)new_y);
    listOfPoints[i].x = (int)new_x; // must cast from float to integer
    listOfPoints[i].y = (int)new_y;
  }
}

void Polygon::translate(int x_offset, int y_offset){
  int new_x, new_y;
  for(int i = 0 ; i< numberOfPoints; i++){
    new_x = listOfPoints[i].x + x_offset;
    new_y = listOfPoints[i].y + y_offset;
    DPRINT("new points (%d, %d)\n", (int)new_x, (int)new_y);
    listOfPoints[i].x = new_x; 
    listOfPoints[i].y = new_y;
  }
}

