#include <GL/glut.h>
#include "graph.h"
#include "polygon.h"
#include "line.h"
#include <iostream>
#include <cstring>
float *PixelBuffer; // global pixel buffer
int window_width = 1000;//by default
int window_height = 500;//by default
int numberOfPolygons;

void callback_display();
void drawStuff(Graph &);
void readHeaders(int *window_width, int *window_height, int *numberOfPolygons);
void readPolygons(Graph *graph, Polygon **polygons);

int main(int argc, char *argv[]){
  glutInit(&argc, argv);  //initialize GL Utility Toolkit(GLUT) and  extract command line arguments for GLUT and keep the counts for the remaining arguments 
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB );

  readHeaders(&window_width, &window_height, &numberOfPolygons);//read from file for the window-dimension and numberOfPolygons there are 
  
  glutInitWindowSize(window_width, window_height);
  glutInitWindowPosition(100, 100); 
  int windowID = glutCreateWindow("First Window");
  
  glClearColor(1,1,1,0.0); //the background_color_buffer underneath the pixelbuffer
  PixelBuffer = new float[window_width*window_height*3];   
  Graph graph(window_width,window_height, PixelBuffer); 
  graph.fillScreen(1,1,1);
  
  Polygon *polygons[numberOfPolygons];
  readPolygons(&graph, polygons);
  //drawStuff(graph);
  
  glutDisplayFunc(callback_display);
  glutMainLoop();
  return 0;
}


void callback_display(){
  glClear(GL_COLOR_BUFFER_BIT);
  glLoadIdentity();
  glDrawPixels(window_width, window_height, GL_RGB, GL_FLOAT, PixelBuffer);
  
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
  ClipRegion cr = { -300, 300, -150, 150}; 
  Point points[] = { {0,0}, {50, 50}, {0,100}, {100,100} ,{150,50}, {100, 0}  };
  Polygon poly(points, 6, &graph);  // used the object graph to draw to the screen
  poly.setColor( {0.1, 0.5, 0.3} );
  poly.draw(); 
  //poly.clip(60,60, 80,80 );//
  poly.rasterize(0.3,0.4,0.5);

  poly.rotate(45);
  poly.draw();
  poly.scale(2, 2);
  //poly.draw();
  //poly.rasterize();
  //poly.clip(cr); 

  poly.translate(-400, 50);
  poly.draw();
  poly.rasterize(1,0,1);
  poly.clip(cr); 
  
  poly.scale(0.2, 0.2);
  poly.rotate(45);
  poly.translate(400, -200);
  poly.draw();
  //poly.rasterize(0.3, 0.4, 1);
  poly.clip(cr); 
  

  Point points2[] = { {-200, -200}, {-100, -200}, {-100, -100}, {-200, -100} };
  Polygon poly2(points2, 4, &graph);
  poly2.setColor({ 0.5, 0.5, 0.5}); 
  poly2.draw();
  
  poly2.printListOfContourPoints();
  poly2.clip(cr);
 
  poly2.scale(1.2, 1.2);
  poly2.draw();
  poly2.clip(cr);
  poly2.rotate(45);
  poly2.draw();
  poly2.clip(cr);
  poly2.scale(0.8, 0.8);
  poly2.draw();
  poly2.clip(cr); 
  poly2.rotate(60);
  poly2.draw();
  poly2.clip(cr);
  poly2.translate(550, 100);
  poly2.draw();
  poly2.clip(cr); 

  Point points3[] = { {100, 150}, {150, 150}, {175, 125}, {200, 150}, {275, 150}, { 275, 100}, {200, 100 }, {175, 50}, {150, 100}, {100, 100}};
  Polygon poly3(points3, 10, &graph);
  poly3.translate(200,0);
  poly3.draw();
  poly3.rasterize(0.3, 0.9, 0.4);
  poly3.clip(cr);

  
  ClipRegion crl = {-150, -100, 10, 50} ;
  Line line2( {-175, 30}, { -175, 80}, &graph );
  line2.draw(); 
  line2.clip(crl);

  Line line3( {-175,0}, {-125, 80}, &graph );
  line3.draw();
  line3.clip( -150, -100, 10, 50);

  Line line4( {-140, 45}, {-120, 15 }, &graph);
  line4.draw();
  line4.clip(-150, -100, 10, 50);

  Point points4[] = { {200, 100}, {300, -50}, {400, 150} };
  Polygon poly4(points4, 3, &graph);
  poly4.draw();
  poly4.clip(cr); 

  
}

void readHeaders(int*window_width, int*window_height, int*numberOfPolygons){
  int sizeOfBuffer = 256;
  char buffer[sizeOfBuffer], *charPtr; 
  
  //prime the pump
  std::cin.getline(buffer, sizeOfBuffer); // first line contains window dimension info
  charPtr = strtok(buffer, " ");
  *window_width = atoi(charPtr);
  charPtr =strtok(0, "\0");
  *window_height = atoi(charPtr);
  std::cin.getline(buffer, sizeOfBuffer); //skip a line
  std::cin.getline(buffer, sizeOfBuffer); // contains number of polygon
  *numberOfPolygons = atoi(buffer);
  DPRINT("window_width: %d, window_height: %d, numberOfPolygons: %d\n", *window_width, *window_height, *numberOfPolygons);
}

void readPolygons(Graph *graph, Polygon **polygons){ 
  int sizeOfBuffer = 256, numberOfPoints;
  char buffer[sizeOfBuffer], *charPtr; 
  
  while(std::cin.getline(buffer, sizeOfBuffer)) { // skip a line for before entering the section for describing the next polygon
    std::cin.getline(buffer, sizeOfBuffer); 

    numberOfPoints =  atoi(buffer); 
    for(int i = 0 ; i < numberOfPoints; i++){ // read "numberOfPoints" points 
      std::cin.getline(buffer, sizeOfBuffer);
      std::cout << buffer<< std::endl;
    }
    std::cout << std::endl;
    if(std::cin.eof()) //end of file - done!!
      break;

  }
}
