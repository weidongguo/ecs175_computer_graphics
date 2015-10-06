#include <GL/glut.h>
#include <algorithm> // for std::fill
#include <cstdio>
#include <cmath>

#define WINDOW_WIDTH 500 
#define WINDOW_HEIGHT 250

#ifdef DEBUG_PRINT
  #define DPRINT printf
#else
  #define DPRINT(...) 
#endif

#define MIN(x,y) ((x<=y)?x:y)
#define MAX(x,y) ((x>y)?x:y)

typedef struct {
  int x;
  int y;
} Point; 


void callback_display();
int drawPixel(int, int, float, float, float);
int drawLine(Point p1, Point p2, float, float, float);
int dda( Point p1, Point p2,  float r, float g, float b); 


float PixelBuffer[ WINDOW_WIDTH * WINDOW_HEIGHT * 3 ]; //global pixel buffer for the window

int main(int argc, char *argv[]){
  glutInit(&argc, argv);  //initialize GL Utility Toolkit(GLUT) and  extract command line arguments for GLUT and keep the counts for the remaining arguments 
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB );

  glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
  glutInitWindowPosition(100, 100); 
  int windowID = glutCreateWindow("First Window");
  
  glClearColor(1.0,1.0,1.0,0.0); 

  glutDisplayFunc(callback_display);
  
  //std::fill(PixelBuffer, PixelBuffer+WINDOW_WIDTH*WINDOW_HEIGHT*3, 0.9); 
  drawPixel(WINDOW_WIDTH/2, WINDOW_HEIGHT/2, 1, 1, 1 );
  Point p = {0, 100};
  Point q = {WINDOW_WIDTH/2, 0};
  Point r = {450, 200};
  drawLine( p, q , (double)0xdc/255, (double)0x14/255, (double)0x3c/255 );
  drawLine( q, r , 0, 0, 1);
  drawLine( r, p , 1, 1, 0);
  
  drawLine( {0,0} , {50, 220}, 1,0,1); 
  drawLine( {50, 220}, {100, 220}, 1,0,1); 
  drawLine( {100, 220}, {100, 0}, 1,0,1); 
  drawLine( {50, 220}, {100,0}, 1,0,1 );
  drawLine( {100,220} , {120, 200}, 1, 0, 1 ); 
  glutMainLoop();
  return 0;
}


void callback_display(){
  glClear(GL_COLOR_BUFFER_BIT);
  //glLoadIdentity();
  glDrawPixels(WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGB, GL_FLOAT, PixelBuffer);
  
  //glEnd();
  glFlush(); //force all GL commands to be executed by the actual rendering engine
}

int drawPixel(int x, int y, float r, float g, float b){
  if( x >= WINDOW_WIDTH || x < 0 ||  y >= WINDOW_HEIGHT || y < 0){
    DPRINT("ERROR: INVALID POINTS\n"); 
    return -1;
  }
  PixelBuffer[ y * WINDOW_WIDTH * 3 + x * 3 ] = r;
  PixelBuffer[ y * WINDOW_WIDTH * 3 + x * 3 + 1 ] = g;
  PixelBuffer[ y * WINDOW_WIDTH * 3 + x * 3 + 2 ] = b;

  return 0;
}

int drawLine( Point p1, Point p2,  float r, float g, float b){ 
  dda(p1, p2, r, g, b);
}

int dda( Point p1, Point p2,  float r, float g, float b){ 
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

