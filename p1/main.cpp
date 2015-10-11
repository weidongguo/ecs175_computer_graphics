#include <GL/glut.h>
#include "graph.h"
#include "polygon.h"
#include "line.h"
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
  graph.fillScreen(1,1,1);
  
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
  /*graph.drawLine( {0,0} , {50, 220}, 1,0,1); 
  graph.drawLine( {50, 220}, {100, 220}, 1,0,1); 
  graph.drawLine( {100, 220}, {100, 0}, 1,0,1); 
  graph.drawLine( {50, 220}, {100,0}, 1,0,1 );
  graph.drawLine( {100,220} , {120, 200}, 1, 0, 1 ); 
  graph.drawLine( {120, 200}, {400, 100}, 1,0,1 );
  
  graph.drawLine( {0, 100}, { WINDOW_WIDTH/2, 0}, 1, 0, 0);
  graph.drawLine( {WINDOW_WIDTH/2,0}, {450, 200}, 0, 0, 1);
  graph.drawLine( {450, 200}, {0, 100}, 1, 1, 0);
  
  graph.drawLine( {-500,-250}, {0,0} , 1, 0.5, 0.5);*/
  int r = 250; 
  for(int i = 0 ; i < r; i++ )
    for(int j = 0; j <r; j++)
      if( i*i + j*j == r * r){
        graph.drawLine( {0,0}, {i,j}, 1,0,1);
        graph.drawLine( {0,0}, {-i,j}, 1,0,1);
        graph.drawLine( {0,0}, {i,-j}, 1,0,1);
        graph.drawLine( {0,0}, {-i,-j}, 1,0,1);
      }
  
  Point points[] = { {0,0}, {50, 50}, {0,100}, {100,100} ,{150,50}, {100, 0}  };
  Polygon poly(points, 6, &graph);  // used the object graph to draw to the screen
  poly.setColor( {0.1, 0.5, 0.3} );
  poly.draw(); 
  poly.clip(60,60, 80,80 );//
  poly.rasterize(0.3,0.4,0.5);

  poly.rotate(45);
  poly.draw();
  poly.scale(1.8, 1.8);
  poly.draw();

  poly.translate(-400, 50);
  
  poly.draw();
  poly.rasterize(1,0,1);
 
  poly.scale(0.2, 0.2);
  poly.rotate(45);
  poly.translate(400, -200);
  poly.draw();
  poly.rasterize(1, 0, 1);

  Point points2[] = { {-200, -200}, {-100, -200}, {-100, -100}, {-200, -100} };
  Polygon poly2(points2, 4, &graph);
  poly2.draw();
  
  poly2.printListOfContourPoints();
  poly2.rasterize(0.5,0.5,0.5);
/* 
  poly2.scale(1.2, 1.2);
  poly2.draw();
  poly2.rotate(45);
  poly2.draw();
  poly2.scale(0.8, 0.8);
  poly2.draw();
  poly2.rotate(60);
  poly2.draw();
  poly2.translate(500, 100);
  poly2.draw();*/
  Point points3[] = { {100, 150}, {150, 150}, {175, 125}, {200, 150}, {275, 150}, { 275, 100}, {200, 100 }, {175, 50}, {150, 100}, {100, 100}};
  Polygon poly3(points3, 10, &graph);
  poly3.translate(200,0);
  poly3.draw();
  poly3.rasterize(0.3, 0.9, 0.4);

  Line line1( {-125, 0}, {-125, 80} , &graph );
  line1.clip( -150, -100, 10, 50 );

  Line line2( {-175, 30}, { -50, 30}, &graph );
  line2.clip( -150, -100, 10, 50 );

  Line line3( {-175,0}, {-125, 80}, &graph );
  line3.draw();
  line3.clip( -150, -100, 10, 50);

  Line line4( {-140, 45}, {-120, 15 }, &graph);
  line4.draw();
  line4.clip(-150, -100, 10, 50);

  Point points4[] = { {200, 100}, {300, -50}, {400, 150} };
  Polygon poly4(points4, 3, &graph);
  poly4.draw();
  poly4.clip( 250, 350, -30, 120);
}


