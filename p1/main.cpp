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
void readPolygons(Graph *graph, Polygon **polygons, int numberOfPolygons);

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
  readPolygons(&graph, polygons, numberOfPolygons);
  ClipRegion cr = { -150, 150, -150, 150};

  for(int i = 0 ; i< numberOfPolygons; i++){
    polygons[i]->draw();
    polygons[i]->rasterize();
    polygons[i]->rotate(45);
    polygons[i]->rasterize();
    polygons[i]->translate(50, 10); 
    polygons[i]->rasterize();
    polygons[i]->clip(cr);
  }
  
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

/*=====================================================================*/
/* @fn        :     void readHeaders(int*window_width, int*window_height, int*numberOfPolygons)
 * @brief     :     prime the pump - reads the header info from the stdin(redirected from a file to stdin), 
 *                  the header contains info about the dimension of the window and the numebr of polygons there are
 * @param[out]:     int *window_width     - for storing window_width
 * @param[out]:     int *window_height    - for storing window_height
 * @param[out]:     int *numberOfPolygons - for storing the number of polygons
 * @return    :     none
 */

void readHeaders(int*window_width, int*window_height, int*numberOfPolygons){
  int sizeOfBuffer = 256;
  char buffer[sizeOfBuffer], *charPtr; 
  
  //prime the pump
  std::cin.getline(buffer, sizeOfBuffer); // first line contains window dimension info
  charPtr = strtok(buffer, " ");
  *window_width = atoi(charPtr);
  charPtr =strtok(0, "\0 ");
  *window_height = atoi(charPtr);
  std::cin.getline(buffer, sizeOfBuffer); //skip a line
  std::cin.getline(buffer, sizeOfBuffer); // contains number of polygon
  *numberOfPolygons = atoi(buffer);
  DPRINT("window_width: %d, window_height: %d, numberOfPolygons: %d\n", *window_width, *window_height, *numberOfPolygons);
}

/*========================================================================*/
/* @fn        :   void readPolygons(Graph *graph, Polygon **polygons, int numberOfPolygons);
 * @brief     :   read data from stdin(a file redirected to stdin) and build polygons using the data init
 * @param[in] :   Graph *graph         - an object for drawing pixel and related drawing methods, one of the param used to construct a Polygon object
 * @param[in] :   int numberOfPolygons - number of polygons
 * @param[out]:   Polygon **polygons   - the output: polygons created by the info specified in the file
 * @return    :   none
 */

void readPolygons(Graph *graph, Polygon **polygons, int numberOfPolygons){ 
  int sizeOfBuffer = 256, numberOfPoints, x, y, numberOfPolygonsAlreadyProcessed = 0;
  char buffer[sizeOfBuffer], *charPtr; 
  Point *listOfPoints;
  
  while(std::cin.getline(buffer, sizeOfBuffer)) { // skip a line for before entering the section for describing the next polygon
    std::cin.getline(buffer, sizeOfBuffer);//read the number of points for constructing the new polygon 
    numberOfPoints =  atoi(buffer); 
     
    listOfPoints = new Point[numberOfPoints];
    for(int i = 0 ; i < numberOfPoints; i++){ // form a listOfPoints;
      std::cin.getline(buffer, sizeOfBuffer); //get a point
      charPtr = strtok(buffer, " ");
      x = atoi(charPtr); //x value;
      charPtr = strtok(0, "\0 ");
      y = atoi(charPtr);
      listOfPoints[i] = {x,y};
      DPRINT("(%d, %d)\n", x, y);
    }
    polygons[numberOfPolygonsAlreadyProcessed] = new Polygon(listOfPoints, numberOfPoints, graph); 
    delete [] listOfPoints;
    
    DPRINT("\n"); 
    if( ++numberOfPolygonsAlreadyProcessed == numberOfPolygons) // already processed the desired number of polygons done!!
      break;
    if(std::cin.eof()) //end of file - done!!
      break;
  }
}

