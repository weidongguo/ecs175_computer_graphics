#include <GL/glut.h>
#include "graph.h"
#include "user_io.h"
#include <iostream>
#include <cstring>
#include "curve.h"
#include <unistd.h>
//#include <AntTweakBar.h>

float *PixelBuffer; // global pixel buffer
Graph *globalGraph;
std::list<Curve*> *globalCurves;
std::string input_buffer;

Window window;

void callback_keyboard(unsigned char key, int x, int y);
void callback_display();
void callback_mouse(int button, int state, int x, int y);
void callback_menu(int state);
void createMenu();
void windowInit(Window *window);
void updateScreen(Graph *graph, std::list<Curve*> *curves);

int main(int argc, char *argv[]){
  glutInit(&argc, argv); //initialize GL Utility Toolkit(GLUT) and  extract command line arguments for GLUT and keep the counts for the remaining arguments 
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB );
  
  windowInit(&window);

  std::ifstream ifs;
  readFile(ifs);
  readHeaders(&ifs, &window.numberOfCurves);//read from datafile for the window-dimension and numberOfPolyhedra there are 
  
  glutInitWindowSize(window.width, window.height);
  glutInitWindowPosition(100, 100); 

  // initialize the pixel buffers
  PixelBuffer = new float[window.width*window.height*3];   
  
  // create sub-windows and their callback functions
  int mainWindowID = glutCreateWindow("Project 4: Bezier and B-Spline Curves ");
  
  //callback registration:
  glutDisplayFunc(callback_display);
  glutKeyboardFunc(callback_keyboard); 
  glutMouseFunc(callback_mouse);
  createMenu();
  glutSetCursor(GLUT_CURSOR_CROSSHAIR);
 
  //for updating pixels buffer
  Graph graph(window.width,window.height, PixelBuffer); 
  globalGraph = &graph;//any function that wants to draw can use this pointer(globalGraph) to graph
  graph.fillScreen(1,1,1); // white background
 
  //curves to be manipulated;  
  std::list<Curve*> curves; 
  readCurves(&ifs, &graph, &curves, window.numberOfCurves);
  globalCurves = &curves; 

  updateScreen(globalGraph, globalCurves);
  glutMainLoop();
  return 0;
}

void callback_mouse(int button, int state, int x, int _y){
  int y = window.height - _y; 
  std::list<Curve*>::iterator itc = globalCurves->begin();
  switch(button){
    case GLUT_MIDDLE_BUTTON: 
      if(state == GLUT_UP && window.numberOfCurves > 0){
        std::advance(itc, window.selectedObject); 
        (*itc)->modifySelectedCtrlPoint(x,y);
        updateScreen(globalGraph, globalCurves); 
      }
      break;
    case GLUT_LEFT_BUTTON:
      if(state == GLUT_UP && window.numberOfCurves > 0){
        //printf("%d, %d\n", x, y);
        std::advance(itc, window.selectedObject);
        if((*itc)->selectCtrlPoint(x,y)){
          updateScreen(globalGraph, globalCurves); 
        }
      }
      break;
  }
}

void callback_menu(int state){
  std::list<Curve *>::iterator it = globalCurves->begin();
  switch(state){
    case MENU_STATUS:
      printf("======================Status====================\n");
      printf("Window Size is %d x %d\n", window.width, window.height);
      printf("Res: %.2f\n",window.res);
      printf("=================End of Status==================\n");
      break;
    case MENU_SET_BSPLINE_PARAM:
      std::advance(it, window.selectedObject); 
      if( strcmp( (*it)->className(), "Bspline") == 0 ){
        ( (Bspline*)(*it) )->setParam();
        updateScreen(globalGraph, globalCurves);
      }
      else{
        printf("The selected object is an Bezier curve.\n");
      }
      break;
  }
}

void callback_keyboard(unsigned char key, int x, int _y){
  int y = window.height - _y;
  //DPRINT("ASCII: %d CHAR:%c <==> Cursor at (%d, %d)\n", key, key, x, window.height - y);
    std::list<Curve*>::iterator itc;
   
    switch(key){ // control commands
    //+ resolution
    case '=':  window.res++; 
               break;
    //- resoltuion
    case '-':  if(window.res <= 1) 
                  window.res = 1;
               else
                  window.res--; 
               break;
    //- select curve to the left
    case 'h': if(window.selectedObject == 0)
                window.selectedObject = window.numberOfCurves - 1;  
              else
                window.selectedObject--;
              break;
    // -select curve to the right
    case 'l':  window.selectedObject = (window.selectedObject+1)%window.numberOfCurves;
               break;
    // -delete control point
    case 'd':  if(window.numberOfCurves>0){ 
                 itc = globalCurves->begin(); 
                 std::advance(itc, window.selectedObject);
                 if( (*itc)->deleteSelectedCtrlPoint() ){ // if true, that curve has no more ctrl points;
                  globalCurves->erase(itc); //remove that curve
                  window.numberOfCurves--;
                  window.selectedObject=0; //reset selected object
                 }
               }
               break; 
    // - add a new point to the left of the selected ctrl point
    case 'i': 
      itc = globalCurves->begin();
      std::advance(itc, window.selectedObject);
      (*itc)->insertCtrlPoint(x, y);
      break;   
    // - add a new point to the right of the selected ctrl point 
    case 'a':
      itc = globalCurves->begin();
      std::advance(itc, window.selectedObject);
      (*itc)->addCtrlPoint(x, y);
      break;
    case 's':
      Curve::saveToFile(globalCurves, "output");
      return;  
    default: return;
  }
  
  updateScreen(globalGraph, globalCurves);

  glutPostRedisplay();
}

void callback_display(){
  //glClear(GL_COLOR_BUFFER_BIT);
  glLoadIdentity();
  glDrawPixels(window.width, window.height, GL_RGB, GL_FLOAT, PixelBuffer);
 
  glFlush(); //force all GL commands to be executed by the actual rendering engine

  glutPostRedisplay();
}


//create the pop of menu that can be triggered by right cliking within the opengl window
void createMenu(void){     
  int subMenuId_grabInput = glutCreateMenu(callback_menu);
  glutAddMenuEntry("Set Order K and Knot Values", MENU_SET_BSPLINE_PARAM);
 
  int menuId = glutCreateMenu(callback_menu);
  glutAddMenuEntry("Status", MENU_STATUS);
  glutAddSubMenu("Input", subMenuId_grabInput); 
  glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void windowInit(Window *window){
  window->width = 900;
  window->height = 600;
  window->numberOfPolygons = 0;
  window->numberOfPolyhedra = 0;
  window->cr = { -200, 200, -200, 200};
  window->tf = { 3, 3, 3, 1.2, 3};
  window->state = STATE_GRAB_COMMANDS;
  window->inputBuffer = &input_buffer;
  window->tf.pairOfPointsForRotAxis[0] = { 0,0,0};
  window->tf.pairOfPointsForRotAxis[1] = { 1,1,1};
  window->state = STATE_HALF_TONE_OFF;
  window->res = 30;  
}

void updateScreen(Graph *graph, std::list<Curve*> *curves){
  graph->fillScreen(graph->background_color);
  Curve::normalizeCtrlPoints(curves);
  int i=0;
  for( std::list<Curve*>::iterator it = curves->begin(); it != curves->end(); it++, i++){
    (*it)->printAttributes();
    if(  i== window.selectedObject )
       (*it)->drawControlPolygon({0,1,0}, true);
    else
       (*it)->drawControlPolygon( {0.5,0.5,0.5}, false);
    
    (*it)->drawCurve(window.res);
  }
}

