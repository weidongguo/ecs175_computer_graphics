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
void bresenham(Point p1, Point p2, float r, float g, float b, int (*drawPixelFunct)(int,int,float,float,float) );


float PixelBuffer[ WINDOW_WIDTH * WINDOW_HEIGHT * 3 ]; //global pixel buffer for the window

int main(int argc, char *argv[]){
  glutInit(&argc, argv);  //initialize GL Utility Toolkit(GLUT) and  extract command line arguments for GLUT and keep the counts for the remaining arguments 
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB );

  glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
  glutInitWindowPosition(100, 100); 
  int windowID = glutCreateWindow("First Window");
  
  glClearColor(1.0,1.0,1.0,0.0); 

  glutDisplayFunc(callback_display);
  
  std::fill(PixelBuffer, PixelBuffer+WINDOW_WIDTH*WINDOW_HEIGHT*3, 0.9); 
  drawPixel(WINDOW_WIDTH/2, WINDOW_HEIGHT/2, 1, 1, 1 );
  Point p = {0, 100};
  Point q = {WINDOW_WIDTH/2, 0};
  Point r = {450, 200};
  drawLine( p, q , (double)0xdc/255, (double)0x14/255, (double)0x3c/255 );
  drawLine( q, r , 0, 0, 1);
  drawLine( r, p , 1, 1, 0);
  
  drawLine( {0,0} , {50, 220}, 1,1,1); 
  drawLine( {50, 220}, {100, 220}, 1,0,1); 
  drawLine( {100, 220}, {100, 0}, 1,0,1); 
  drawLine( {50, 220}, {100,0}, 1,0,1 );
  drawLine( {100,220} , {120, 200}, 1, 0, 1 ); 
  drawLine( {120, 200}, {400, 100}, 1,0,1 );
  glutMainLoop();
  return 0;
}


void callback_display(){
  glClear(GL_COLOR_BUFFER_BIT);
  //glLoadIdentity();
  glDrawPixels(WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGB, GL_FLOAT, PixelBuffer);
  
  glFlush(); //force all GL commands to be executed by the actual rendering engine
}

int drawPixel(int x, int y, float r, float g, float b){
  if( x >= WINDOW_WIDTH || x < 0 ||  y >= WINDOW_HEIGHT || y < 0){
    DPRINT("ERROR: INVALID POINTS (%d, %d)\n", x,y); 
    return -1;
  }
  PixelBuffer[ y * WINDOW_WIDTH * 3 + x * 3 ] = r;
  PixelBuffer[ y * WINDOW_WIDTH * 3 + x * 3 + 1 ] = g;
  PixelBuffer[ y * WINDOW_WIDTH * 3 + x * 3 + 2 ] = b;

  return 0;
}

int drawLine( Point p1, Point p2,  float r, float g, float b){ 
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

  bresenham(p1, p2, r, g, b, drawPixel);//last arg is the function ptr for drawing pixel
}


//  Octants:
//   \2|1/
//   3\|/0
//  ---+---
//   4/|\7
//   /5|6\

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
