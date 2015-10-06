#include "graph.h"
#include <GL/glut.h>

float *PixelBuffer; // global pixel buffer

void callback_display();

int main(int argc, char *argv[]){
  glutInit(&argc, argv);  //initialize GL Utility Toolkit(GLUT) and  extract command line arguments for GLUT and keep the counts for the remaining arguments 
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB );

  glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
  glutInitWindowPosition(100, 100); 
  int windowID = glutCreateWindow("First Window");
  
  //glClearColor(0.5,0.5,0.5,0.0); 
  PixelBuffer = new float[WINDOW_WIDTH*WINDOW_HEIGHT*3];   
  Graph graph(WINDOW_WIDTH,WINDOW_HEIGHT, PixelBuffer); 
  graph.fillScreen(1,1,1);
    
  
  glutDisplayFunc(callback_display);
  glutMainLoop();
  return 0;
}


void callback_display(){
  //glClear(GL_COLOR_BUFFER_BIT);
  glLoadIdentity();
  glDrawPixels(WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGB, GL_FLOAT, PixelBuffer);
  
  glFlush(); //force all GL commands to be executed by the actual rendering engine
}




