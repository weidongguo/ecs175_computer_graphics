#include "graph.h"
Graph::Graph(int width, int height, float *PixelBufferPtr){
  PixelBuffer = PixelBufferPtr;
  window_width = width;
  window_height = height;
}

int Graph::drawPixel(int x, int y, float r, float g, float b){
  /*if( abs(x) > window_width/2 || abs(y) > window_height/2){
    DPRINT("ERROR: INVALID POINTS (%d, %d)\n", x,y); 
    return -1;
  } */
  /* now negative x or y values are allowed for input */
  //y+=(window_height/2); //add offset to y to work with the buffer
  //x+=(window_width/2); //add offset to x to work with the buffer
  PixelBuffer[ y * window_width * 3 + x * 3 ] = r;
  PixelBuffer[ y * window_width * 3 + x * 3 + 1 ] = g;
  PixelBuffer[ y * window_width * 3 + x * 3 + 2 ] = b;

  return 0;
}

int Graph::drawLine( Point p1, Point p2,  float r, float g, float b){
  return drawLine(p1,p2,r,g,b,BRESENHAM);
}
int Graph::drawLine( Point p1, Point p2,  float r, float g, float b, int method){ 
  if(p1.x == p2.x){ //vertical line
    int y,y_end; 
    if(p1.y <= p2.y){
      y = p1.y;
      y_end = p2.y;
    }else{
      y = p2.y;
      y_end = p1.y;
    }
    for(; y<=y_end; y++)
      drawPixel(p1.x, y, r,g,b);
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
    for(; x <= x_end; x++)
      drawPixel(x, p1.y, r,g,b);
    return 0;
  }

  // all other cases are taken care below
  if(method == BRESENHAM) 
    bresenham(p1, p2, r, g, b);
  else
    dda(p1,p2,r,g,b);
  //
  return 0;
}

int Graph::dda( Point p1, Point p2,  float r, float g, float b){ 
  int delta_x, delta_y, x,y, x_begin, x_end, y_begin, y_end; double m;//slope
  delta_x = p2.x - p1.x;
  delta_y = p2.y - p1.y;
  m = (double)delta_y / delta_x;
  DPRINT("Slope m : %.2lf\n", m);
 
  if( (0 < m && m <= 1) ||  (-1 <= m && m < 0) ){ //case: 0 < |m| <=1   
    if( p1.x <= p2.x ){ // check for a point with smaller x;
      x_begin = p1.x;
      x_end = p2.x;
      y_begin = p1.y;
    }
    else{
      x_begin = p2.x;
      x_end = p1.x;
      y_begin = p2.y;
    }
    for( x = x_begin; x <= x_end; x++){
      y = lround( (x - x_begin)*m + y_begin );
      drawPixel(x,y,r,g,b);
    }
  }
  else if( m > 1 || m < -1) { 
    double m_bar = 1/m; 
    if(p1.y <= p2.y){
      y_begin = p1.y;
      y_end = p2.y;
      x_begin = p1.x;
    }
    else{
      y_begin = p2.y;
      y_end = p1.y;
      x_begin = p2.x;
    }
    for( y = y_begin; y <= y_end; y++){
      x = lround( ( y - y_begin) * m_bar + x_begin ) ; 
      drawPixel(x,y,r,g,b);
    }
  }
  else if( m == 0 ){ // horizontal line parallel to x-axis
    for( x = MIN(p1.x, p2.x); x <= MAX(p1.x, p2.x); x++){
      drawPixel(x, p1.y, r,g,b);
    }
  }
  else{
    DPRINT("m is bad\n");
    return -1; 
  }
  return 0;
}

/*===================================================*/
/* @fn        :   determineStartAndEndpoints(Point p1, Point p2, int *x, int *y, int *end, int*y_end
 * @brief     :   determine start/end point with respect to the x value of the point, if x < x_end , x is the start
 * @param[in] :   Point p1 - point 1, Point it's struct of { int x,y};
 *                Point p2 - point 2,
 * @param[out]:   int *x - the x value of the start point
 *                int *y - the y value of the start point
 *                int *x_end - the x value of the end point
 *                int *y_end - the y value of the end point
 * @note      :   helper function for bresenham algorithm
 */
void determineStartAndEndPoints(Point p1, Point p2, int*x, int*y, int *x_end, int*y_end){
  if(p1.x <= p2.x){
    *x = p1.x;
    *y = p1.y;
    *x_end = p2.x;
    *y_end = p2.y;
  }
  else{
    *x = p2.x;
    *y = p2.y;
    *x_end = p1.x;
    *y_end = p1.y;
  }
}

//helper function for bresenham algo
void swapXY(Point *p1){
  int tmp;
  tmp = p1->x;
  p1->x = p1->y;
  p1->y = tmp;
}

int Graph::bresenham(Point pt1, Point pt2, float r, float g, float b ){
  Point p1 = pt1;
  Point p2 = pt2;
  int x, y, x_end, y_end, p; 
  int dx = (p2.x - p1.x), dy = (p2.y - p1.y); //for determining sign of slope
  bool steep = false;
  float m = (float)dy/(float)dx ; //find the slope first
  DPRINT("The slope is %.2f,\tline with color %.2f,%.2f,%.2f\n", m, r,g,b); 
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
  //draw first point  
  if(steep)
    drawPixel(y,x,r,g,b);//x and y was swapped before
  else 
    drawPixel(x,y,r,g,b);
  p = 2 * dy - dx;
  for( ; x < x_end; ){
    x++;
    if( p >= 0){ // if d1 - d2  >= 0, means d2 is shorter, so advance y one level up
        positive_slope? y++:y--; 
        p = p + 2*dy - 2*dx;
    }
    else // if d1 - d2 < 0; means d1 is shorter, so no change of y;
      p = p + 2*dy;
    
    if(steep)
      drawPixel(y,x,r,g,b);//x and y was swapped before
    else 
      drawPixel(x,y,r,g,b);
  }

  return 0;
}

int Graph::fillScreen(float r, float g, float b){
  int bufferSize = window_width * window_height * 3;
  for(int i = 0; i < bufferSize; i+=3){
    PixelBuffer[i] = r;
    PixelBuffer[i+1] = g;
    PixelBuffer[i+2] = b;
  }
  background_color = {r,g,b}; // save the background_color
  return 0;
}

int Graph::drawPolygon( Point *listOfPoints, int numberOfPoints, float r, float g, float b){
  //draw a line between the first point and the last point first
  if(numberOfPoints < 2)
    return -1;
  drawLine(listOfPoints[0], listOfPoints[numberOfPoints-1], r,g,b); 
  for(int i = 0 ; i < numberOfPoints - 1; i++){
    drawLine(listOfPoints[i], listOfPoints[i+1], r,g,b);
  }
  return 0;
}

bool Graph::outOfBound(int x, int y){
  return ( abs(x) > window_width/2 || abs(y) > window_height/2);
}
