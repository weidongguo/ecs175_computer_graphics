#include "graph.h"
#include <GL/glut.h>

float *PixelBuffer; // global pixel buffer

void callback_display();
void drawStuff(Graph &);

int main(int argc, char *argv[]){
  glutInit(&argc, argv);  //initialize GL Utility Toolkit(GLUT) and  extract command line arguments for GLUT and keep the counts for the remaining arguments 
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB );

  glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
  glutInitWindowPosition(100, 100); 
  int windowID = glutCreateWindow("First Window");
  
  //glClearColor(0.5,0.5,0.5,0.0); 
  PixelBuffer = new float[WINDOW_WIDTH*WINDOW_HEIGHT*3];   
  Graph graph(WINDOW_WIDTH,WINDOW_HEIGHT, PixelBuffer); 
  graph.fillScreen(0.5,0.5,0.5);
  
  drawStuff(graph);
  
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

void drawStuff(Graph &graph){
  graph.drawLine( {0,0} , {50, 220}, 1,0,1); 
  graph.drawLine( {50, 220}, {100, 220}, 1,0,1); 
  graph.drawLine( {100, 220}, {100, 0}, 1,0,1); 
  graph.drawLine( {50, 220}, {100,0}, 1,0,1 );
  graph.drawLine( {100,220} , {120, 200}, 1, 0, 1 ); 
  graph.drawLine( {120, 200}, {400, 100}, 1,0,1 );

  graph.drawLine( {0, 100}, { WINDOW_WIDTH/2, 0}, 1, 0, 0);
  graph.drawLine( {WINDOW_WIDTH/2,0}, {450, 200}, 0, 0, 1);
  graph.drawLine( {450, 200}, {0, 100}, 1, 1, 0);

}


