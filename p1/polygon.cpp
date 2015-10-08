#include "polygon.h"
#include <cstdlib>
#include <cstring>
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

void Polygon:: _storeContourPoint(int x, int y){
  listOfContourPoints[y+WINDOW_HEIGHT/2].push_back( {x,y} ); 
  // y + WINDOW_HEIGHT/2 since we have negative coordinates but we need positive indexing in the array
}

//end points are NOT stored!!
int Polygon::_bresenham(Point pt1, Point pt2){ //for storing points, not drawing pixel
  Point p1 = pt1;
  Point p2 = pt2;
  int x, y, x_end, y_end, p; 
  int dx = (p2.x - p1.x), dy = (p2.y - p1.y); //for determining sign of slope
  bool steep = false;
  float m = (float)dy/(float)dx ; //find the slope first
  bool positive_slope;
  if( m >= 0 )  // positive slope
    positive_slope = true;
  else
    positive_slope = false;
  
  if( fabs(m) <= 1 ){ //shallow
    steep = false; 
  }
  else{ //steep
   steep = true;
   swapXY(&p1);
   swapXY(&p2);
  }
  determineStartAndEndPoints(p1, p2, &x, &y, &x_end, &y_end);
  //DPRINT("x: %d,\ty: %d,\tx_end: %d,\ty_end:%d\n", x, y, x_end, y_end);
  dx = abs(x_end - x);
  dy = abs(y_end - y);
/*  //draw first point  
  if(steep)
    _storeContourPoint(y,x);//x and y was swapped before
  else 
    gstoreContourPoint(x,y); */  //no more first point
  p = 2 * dy - dx;

//  
  x_end--; // do  not store the last point
//
  for( ; x < x_end; ){
    x++;
    if( p >= 0){ // if d1 - d2  >= 0, means d2 is shorter, so advance y one level up
        positive_slope? y++:y--; 
        p = p + 2*dy - 2*dx;
    }
    else // if d1 - d2 < 0; means d1 is shorter, so no change of y;
      p = p + 2*dy;
    
    if(steep)
      _storeContourPoint(y,x);
    else 
      _storeContourPoint(x,y);
  }

  return 0;
}

//NOTE: End points are NOT stored, meaning the original two points
int Polygon::_storeLinePoints( Point p1, Point p2){
  if(p1.x == p2.x){ //vertical line
    int y,y_end; 
    if(p1.y <= p2.y){
      y = p1.y;
      y_end = p2.y;
    }else{
      y = p2.y;
      y_end = p1.y;
    }
    
    y++; // do not store the first point
    y_end--; // do  not store the last point

    for(; y<=y_end; y++)
      _storeContourPoint(p1.x, y);
    return 0;
  }
  else if(p1.y == p2.y){ // horizontal line
    int x, x_end;
    if(p1.x <= p2.x){
      x = p1.x;
      x_end =  p2.x;
    }
    else{
      x = p2.x;
      x_end = p1.x; 
    }

    x++; // do not store the first point
    x_end--; // do  not store the last point

    for(; x <= x_end; x++)
      _storeContourPoint(x, p1.y);
    return 0;
  }

  // all other cases are taken care below
  _bresenham(p1, p2);
  
  return 0;
}

bool compareXValue(Point p1, Point p2){
  return p1.x <= p2.x;
}

void Polygon::clearContourPoints(){
  for(int i = 0; i< WINDOW_HEIGHT; i++){
    listOfContourPoints[i].clear();
  }
}

void Polygon::storeOriginalPointsToContourPoints(){
  Point p;
  for(int i = 0 ; i < numberOfPoints; i++){
    p = listOfPoints[i] ;
    listOfContourPoints[p.y+WINDOW_HEIGHT/2].push_back(p);
  }
}

void Polygon::storeContourPoints(){
  clearContourPoints();//reset all listOfCountourPoints
   
  storeOriginalPointsToContourPoints();//do not want duplicates of the original points, so store it once here and ignore all other instances when encountering the same points
  _storeLinePoints(listOfPoints[0], listOfPoints[numberOfPoints-1]);//line for first point to the last point
  for(int i = 0 ; i < numberOfPoints - 1; i++){ //each line between points
    _storeLinePoints(listOfPoints[i], listOfPoints[i+1]);
  }
  ///sort each scanline points by it's x-value 
  for(int i = 0 ; i < WINDOW_HEIGHT; i++){
    if(!listOfContourPoints[i].empty())
      listOfContourPoints[i].sort(compareXValue);
  }
}

void Polygon::printListOfContourPoints(){
  DPRINT("\n=================START OF CONTOUR POINTS=======================\n");
  for(int i = 0; i< WINDOW_HEIGHT; i++){
    for(std::list<Point>::iterator it = listOfContourPoints[i].begin(); it != listOfContourPoints[i].end(); it++)
      DPRINT("(%d, %d)  ", (*it).x, (*it).y);
    if(!listOfContourPoints[i].empty()) 
      DPRINT("\n"); 
  }
  DPRINT("=================END OF CONTOUR POINTS=========================\n\n");
}

void Polygon::rasterize(float r, float g, float b){
  for(int i = 0; i < WINDOW_HEIGHT; i++){
    if(listOfContourPoints[i].size() > 1){ // if more than 1 point on a scanline
      for(std::list<Point>::iterator it = listOfContourPoints[i].begin(); it != listOfContourPoints[i].end();){
        Point p1 = *it;
        std::advance(it, 1);
        if(it == listOfContourPoints[i].end())
          break;
        Point p2 = *it;
        DPRINT("(%d, %d) ; (%d, %d) \n", p1.x, p1.y, p2.x, p2.y);
        if( abs(p2.x - p1.x) >= 1){
         //DPRINT("SCAN LINE %d\n", i);
         graph->drawLine(p1,p2, r, g, b); 
        }
      }
    }
  }
}
