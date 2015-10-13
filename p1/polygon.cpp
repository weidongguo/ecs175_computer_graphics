#include "polygon.h"
#include <cstdlib>
#include <cstring>
Polygon::Polygon(Point * listOfPts, int _numberOfPoints, Graph *ptr_graph){
  listOfPoints = listOfPointsOriginal = new Point[_numberOfPoints];
  for(int i = 0; i < _numberOfPoints; i++) 
    listOfPoints[i] = listOfPts[i];
  
  numberOfPoints = numberOfPointsOriginal = _numberOfPoints;
  color = {0,0,0} ;// by default black
  graph = ptr_graph;
  listOfContourPoints = new std::list<Point>[graph->window_height];
   
  listOfPointsAfterClipping = new Point[numberOfPointsOriginal*2];//max number of points that can be after clipping
  
  isRasterized = false;
  setCentroid();//set once in the beginning
}
Polygon::~Polygon(){
  DPRINT("DESTRUCTOR CALLED\n"); 
  if(listOfPointsOriginal != 0) {
    delete[] listOfPointsOriginal; 
    listOfPointsOriginal = 0; 
  }
  if(listOfContourPoints!=0){
    delete[] listOfContourPoints;
    listOfContourPoints = 0;
  }
  if(listOfPointsAfterClipping !=0){
    delete[] listOfPointsAfterClipping;
    listOfPointsAfterClipping = 0;
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
  draw(color.r, color.g, color.b);
}

void Polygon::draw(Color c){
  draw(c.r, c.g, c.b);
}
void Polygon::draw(float r, float g, float b){
  graph->drawPolygon(listOfPoints, numberOfPoints, r,g,b); 
}

void Polygon::scale(float alpha, float beta){
  isRasterized?rasterize(graph->background_color):draw(graph->background_color); // clear the original polygon from the pixel buffer

  float cx = (float)centroid.x,
        cy = (float)centroid.y,
        new_x, new_y, x, y;
  for(int i = 0 ; i< numberOfPoints; i++){
    x = listOfPoints[i].x;
    y = listOfPoints[i].y;
    new_x = cx - alpha*cx + alpha*x;
    new_y = cy - beta*cy + beta*y; 
    DPRINT("new points (%d, %d)\n", (int)new_x, (int)new_y);
    listOfPoints[i].x = (int)round(new_x); // must cast from float to integer
    listOfPoints[i].y = (int)round(new_y);
  }
  setCentroid();//set the new centroid
}

#define PI 3.1415926
void Polygon::rotate(float alpha){ //alpha is the angle
  isRasterized?rasterize(graph->background_color):draw(graph->background_color); // clear the original polygon from the pixel buffer

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
    listOfPoints[i].x = (int)round(new_x); // must cast from float to integer
    listOfPoints[i].y = (int)round(new_y);
  }
  setCentroid();//set the new centroid;
}

void Polygon::translate(int x_offset, int y_offset){
  isRasterized?rasterize(graph->background_color):draw(graph->background_color); // clear the original polygon from the pixel buffer
  
  int new_x, new_y;
  for(int i = 0 ; i< numberOfPoints; i++){
    new_x = listOfPoints[i].x + x_offset;
    new_y = listOfPoints[i].y + y_offset;
    DPRINT("new points (%d, %d)\n", (int)new_x, (int)new_y);
    listOfPoints[i].x = new_x; 
    listOfPoints[i].y = new_y;
  }
  setCentroid(); // set the enw centroid after translating
}

void Polygon:: _storeContourPoint(int x, int y){
  if(! graph->outOfBound(x,y) )
    listOfContourPoints[y+graph->window_height/2].push_back( {x,y} ); 
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
  for(int i = 0; i< graph->window_height; i++){
    listOfContourPoints[i].clear();
  }
}

void Polygon::storeOriginalPointsToContourPoints(){
  Point p;
  for(int i = 0 ; i < numberOfPoints; i++){
    p = listOfPoints[i] ;
    listOfContourPoints[p.y+graph->window_height/2].push_back(p);
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
  for(int i = 0 ; i < graph->window_height; i++){
    if(!listOfContourPoints[i].empty())
      listOfContourPoints[i].sort(compareXValue);
  }
}

void Polygon::printListOfContourPoints(){
  DPRINT("\n=================START OF CONTOUR POINTS=======================\n");
  for(int i = 0; i< graph->window_height; i++){
    for(std::list<Point>::iterator it = listOfContourPoints[i].begin(); it != listOfContourPoints[i].end(); it++)
      DPRINT("(%d, %d)  ", (*it).x, (*it).y);
    if(!listOfContourPoints[i].empty()) 
      DPRINT("\n"); 
  }
  DPRINT("=================END OF CONTOUR POINTS=========================\n\n");
}

void Polygon::rasterize(){
  rasterize(color.r, color.g, color.b);
}

void Polygon::rasterize(Color c){
  rasterize(c.r, c.g, c.b);
}

void Polygon::rasterize(float r, float g, float b){
  isRasterized = true;
  storeContourPoints();// set up all the points for the contour first, so they can be used for rasterizing
  for(int i = 0; i < graph->window_height; i++){ //for each scanline
    if(listOfContourPoints[i].size() > 1){ // if more than 1 point on a scanline
      for(std::list<Point>::iterator it = listOfContourPoints[i].begin(); it != listOfContourPoints[i].end();){
        Point p1 = *it;
        std::advance(it, 1);
        if(it == listOfContourPoints[i].end())
          break;
        Point p2 = *it;
        //DPRINT("(%d, %d) ; (%d, %d) \n", p1.x, p1.y, p2.x, p2.y);
        if( abs(p2.x - p1.x) >= 1 ){
          graph->drawLine(p1,p2, r, g, b); 
         
        }
      }
    }
  }
}

void Polygon::displayClippingRegion(int xMin, int xMax, int yMin, int yMax){
  Point points[] = { {xMin, yMin}, {xMin, yMax}, {xMax, yMax}, {xMax, yMin} }; 
  graph->drawPolygon(points, 4, 0.3, 1.0, 0.2);

}


void Polygon::clip(ClipRegion cr){
  clip(cr.xMin, cr.xMax, cr.yMin, cr.yMax);
}

void Polygon::clip(int xMin, int xMax, int yMin, int yMax){
  //displayClippingRegion(xMin, xMax, yMin, yMax); 
  sutherlandHodgman(xMin, xMax, yMin, yMax);
}

void Polygon::sutherlandHodgman(int xMin, int xMax, int yMin, int yMax){
  std::list<Point> output;
  
  memcpy(listOfPointsAfterClipping, listOfPointsOriginal, numberOfPointsOriginal*sizeof(Point) ); 
  numberOfPointsAfterClipping = numberOfPointsOriginal; //it's true in the beginning 

  
  for(Boundary b = LEFT; b <= TOP; b=b+1){//for each clipping edge
    for(int i = 0 ; i < numberOfPointsAfterClipping; i++){ // for each line to be clipped
      Point p1 = listOfPointsAfterClipping[i]; 
      Point p2 = listOfPointsAfterClipping[ (i+1)% numberOfPointsAfterClipping ];
      if( cross( p1, p2, b, xMin, xMax, yMin, yMax) ){ //in-out or out-in
        if( inside(p1, b, xMin, xMax, yMin, yMax) ){ // in-out -> intersection point
          output.push_back( intersect(p1,p2,b,xMin, xMax, yMin, yMax)); 
          DPRINT("IN-OUT\n");
        }
        else{ // out-in -> intersection point and p2
          output.push_back(intersect(p1,p2,b,xMin, xMax, yMin, yMax)); 
          output.push_back(p2);
          DPRINT("OUT_IN\n");
        }
      }
      else{ //in-in or out-out
        if(inside(p2, b, xMin, xMax, yMin, yMax)){// in-in -> p2
             output.push_back(p2);
             DPRINT("IN-IN\n");
        } //else out-out -> do nothing
        else
          DPRINT("OUT-OUT\n");
      }
    } //end for
    
    // transfer the output to listOfPointsAfterClipping
    numberOfPointsAfterClipping = output.size();
    int i = 0;
    for( std::list<Point>::iterator it = output.begin(); it != output.end(); it++){
      listOfPointsAfterClipping[i] = (*it);
      //DPRINT("After Clipping: (%d, %d)\n", listOfPointsAfterClipping[i].x, listOfPointsAfterClipping[i].y);
      i++;
    }
    DPRINT("\n\n");
    output.clear();
  }
  DPRINT(">>>new size: %d\n", numberOfPointsAfterClipping);
#if 1  
    //erase the original one from the display
  isRasterized?rasterize(graph->background_color):draw(graph->background_color);
    
  //display the clipped polygon;
  if(numberOfPointsAfterClipping > 0){    
    listOfPoints = listOfPointsAfterClipping;
    numberOfPoints = numberOfPointsAfterClipping;
    isRasterized?rasterize():draw();
    listOfPoints = listOfPointsOriginal;
    numberOfPoints = numberOfPointsOriginal;
  }
#endif
}


bool Polygon::inside(Point p, Boundary b, int xMin, int xMax, int yMin, int yMax){ //static method
  switch(b){
    case LEFT:  if(p.x < xMin) return false; break;
    case RIGHT: if(p.x > xMax) return false; break;
    case TOP:   if(p.y > yMax) return false; break;
    case BOTTOM:if(p.y < yMin) return false; break;
  }
  return true;
}

bool Polygon::cross(Point p1, Point p2, Boundary b, int xMin, int xMax, int yMin, int yMax){
  return inside(p1, b, xMin, xMax, yMin, yMax) != inside(p2, b, xMin, xMax, yMin, yMax); 
}

Point Polygon::intersect(Point p1, Point p2, Boundary b, int xMin, int xMax, int yMin, int yMax){
  Point newP;
  float m;
  if( p1.x - p2.x != 0 )  // else, divided by 0
    m = (float)(p2.y - p1.y)/ (p2.x - p1.x);
 
  switch(b){
    case LEFT:
      newP.y = m*(xMin - p1.x) + p1.y;
      newP.x = xMin;
      break;
    case RIGHT:
      newP.y = m*(xMax - p1.x) + p1.y;
      newP.x = xMax;
      break;
    case BOTTOM:
      newP.y = yMin;
      if(p1.x - p2.x !=0) 
        newP.x = (yMin - p1.y)/m + p1.x;
      else  
        newP.x = p1.x;
      break;
    case TOP:
      newP.y = yMax;
      if(p1.x - p2.x !=0)
        newP.x = (yMax - p1.y)/m + p1.x;
      else
        newP.x = p1.x;
      break;
  }
  return newP;
}


/*================================================================*/
/* @fn      :    void savePolygonsToFile(Polygons **polygons, int numberOfPolygons, char *filename)
 * @brief   :    store the number of polygons, and the points for each polygon to a file in the same format as the datafile
 *
 * @return  :    none
 */
void Polygon::savePolygonsToFile(Polygon **polygons, Window *window, const char *filename){
  printf("Saving polygons to a file: %s\n...\nDone.\n", filename);
  std::ofstream ofs(filename, std::ofstream::out);
  ofs << window->width << " " << window->height << "\n\n";  //dimension of opengl window <width><space><height>
  ofs << window->numberOfPolygons << "\n"; // number of polygons <numberOfPolygons>
  
  int numberOfPoints, numberOfPolygons = window->numberOfPolygons, x, y;
  for(int i = 0 ; i < numberOfPolygons ; i++){
    ofs << "\n" ; 
    numberOfPoints = polygons[i]->numberOfPoints;
    ofs << numberOfPoints << "\n";  // <numberOfPoints>
    for(int j = 0 ; j < numberOfPoints; j++){
      x = polygons[i]->listOfPoints[j].x;
      y = polygons[i]->listOfPoints[j].y;
      ofs << x << " " << y << "\n"; // <x><space><y>
    }
  }

  ofs.close();
}
