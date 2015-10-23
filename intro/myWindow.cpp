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

void callback_display();
int drawPixel(int, int, float, float, float);
int drawLine(int[], int[], float, float, float);

float PixelBuffer[ WINDOW_WIDTH * WINDOW_HEIGHT * 3 ]; //global pixel buffer for the window

int main(int argc, char *argv[]){
  glutInit(&argc, argv);  //initialize GL Utility Toolkit(GLUT) and  extract command line arguments for GLUT and keep the counts for the remaining arguments 
  glutInitDisplayMode(GLUT_SINGLE);

  glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
  glutInitWindowPosition(100, 100); 
  int windowID = glutCreateWindow("First Window");
 
  glClearColor(0,0,0,0); 
  glutDisplayFunc(callback_display);
  
  //std::fill(PixelBuffer, PixelBuffer+WINDOW_WIDTH*WINDOW_HEIGHT*3, 0.5); 
  drawPixel(WINDOW_WIDTH/2, WINDOW_HEIGHT/2, 1, 1, 1 );
  int p[] = {0, 100};
  int q[] = {WINDOW_WIDTH/2, 0};
  int r[] = {450, 200};
  drawLine( p, q , (double)0xdc/255, (double)0x14/255, (double)0x3c/255 );
  drawLine( q, r , 0, 0, 1);
  drawLine( r, p , 1, 1, 0);
  
  int s[] = {0, 0};
  int e[] = {50, 220};
  drawLine(s,e, 1,0,1); 

int m_Width = 100, m_Height = 100;
glViewport ( 0, 0, m_Width, m_Height );
glMatrixMode ( GL_PROJECTION );
glLoadIdentity ();
glOrtho ( 0.0f, m_Width, 0.0, m_Height, 1.0, -1.0 );
glMatrixMode ( GL_MODELVIEW );  
glLoadIdentity ();

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
  if( x >= WINDOW_WIDTH)
    return -1;
  else if( y >= WINDOW_HEIGHT)
    return -1;
  PixelBuffer[ y * WINDOW_WIDTH * 3 + x * 3 ] = r;
  PixelBuffer[ y * WINDOW_WIDTH * 3 + x * 3 + 1 ] = g;
  PixelBuffer[ y * WINDOW_WIDTH * 3 + x * 3 + 2 ] = b;

  return 0;
}

int drawLine(int point1[], int point2[], float r, float g, float b){ 
  int x1, y1, x2, y2;  
  if( point1[0] < point2[0] ){
    x1 = point1[0];
    y1 = point1[1];
    x2 = point2[0];
    y2 = point2[1];
  }
  else {
    x1 = point2[0];
    y1 = point2[1];
    x2 = point1[0];
    y2 = point1[1];
  }

  float m = (float)(y2 - y1) / (float)(x2 - x1);
  
  DPRINT("Slope of the line, m = %.2f \n", m);
  
  int x, y;
  for( x = x1; x <= x2; x++){
    y = lround( m * ( x - x1 ) + y1 );
    drawPixel(x, y, r, g, b);
  }

}
