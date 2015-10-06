#include "bres_dda.h"

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

void swapXY(Point *p1){
  int tmp;
  tmp = p1->x;
  p1->x = p1->y;
  p1->y = tmp;
}

void bresenham(Point pt1, Point pt2, float r, float g, float b, int(*drawPixelFunc)(int, int, float,float,float) ){
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
  
  if( abs(m) <= 1 ){ //shallow
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
  drawPixelFunc(x,y,r,g,b);
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
      drawPixelFunc(y,x,r,g,b);//x and y was swapped before
    else 
      drawPixelFunc(x,y,r,g,b);
  }
}
