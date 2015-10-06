#include "graph.h"
#include "bres_dda.h"
Graph::Graph(int width, int height, float *PixelBufferPtr){
  PixelBuffer = PixelBufferPtr;
  window_width = width;
  window_height = height;
}

int Graph::drawPixel(int x, int y, float r, float g, float b){
  if( x >= WINDOW_WIDTH || x < 0 ||  y >= WINDOW_HEIGHT || y < 0){
    DPRINT("ERROR: INVALID POINTS (%d, %d)\n", x,y); 
    return -1;
  }
  PixelBuffer[ y * WINDOW_WIDTH * 3 + x * 3 ] = r;
  PixelBuffer[ y * WINDOW_WIDTH * 3 + x * 3 + 1 ] = g;
  PixelBuffer[ y * WINDOW_WIDTH * 3 + x * 3 + 2 ] = b;

  return 0;
}

int Graph::drawLine( Point p1, Point p2,  float r, float g, float b){ 
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
  //bresenham(p1, p2, r, g, b, drawPixel);//last arg is the function ptr for drawing pixel
  dda(p1,p2,r,g,b);
}

int Graph::dda( Point p1, Point p2,  float r, float g, float b){ 
  int delta_x, delta_y, x,y, x_begin, x_end, y_begin, y_end; double m;//slope
  delta_x = p2.x - p1.x;
  delta_y = p2.y - p1.y;
  m = (double)delta_y / delta_x;
  DPRINT("Slope m : %.2lf\n", m);
 
  if( 0 < m && m <= 1 ||  -1 <= m && m < 0 ){ //case: 0 < |m| <=1   
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
  else
    DPRINT("m is bad\n");

}

int Graph::fillScreen(float r, float g, float b){
  int bufferSize = window_width * window_height * 3;
  for(int i = 0; i < bufferSize; i+=3){
    PixelBuffer[i] = r;
    PixelBuffer[i+1] = g;
    PixelBuffer[i+2] = b;
  }
}


