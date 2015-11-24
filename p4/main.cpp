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
  std::list<Curve*>::iterator itc;
  std::list<Point_2D>::iterator itp;
  switch(button){
    case GLUT_MIDDLE_BUTTON: 
    case GLUT_LEFT_BUTTON:
      if(state == GLUT_UP && window.numberOfCurves > 0){
        printf("%d, %d\n", x, y);
        itc = globalCurves->begin();
        std::advance(itc, window.selectedObject);
        if((*itc)->selectCtrlPoint(x,y)){
          updateScreen(globalGraph, globalCurves); 
        }
      }
      break;
  }
}

void callback_menu(int state){
  switch(state){
    case MENU_STATUS:
      printf("======================Status====================\n");
      printf("Window Size is %d x %d\n", window.width, window.height);
      printf("Res: %.2f\n",window.res);
      printf("=================End of Status==================\n");
      break;
  }
}

void callback_keyboard(unsigned char key, int x, int y){
  //DPRINT("ASCII: %d CHAR:%c <==> Cursor at (%d, %d)\n", key, key, x, window.height - y);
    std::list<Curve*>::iterator itc;
   
    switch(key){ // control commands
    case '=':  window.res++; 
               break;

    case '-':  if(window.res <= 1) 
                  window.res = 1;
               else
                  window.res--; 
               break;
    case 'h': if(window.selectedObject == 0)
                window.selectedObject = window.numberOfCurves - 1;  
              else
                window.selectedObject--;
              break;
    case 'l':  window.selectedObject = (window.selectedObject+1)%window.numberOfCurves;
               break;
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
  glutAddMenuEntry("Set Half Tone Color", MENU_SET_HALF_TONE_COLOR);
  glutAddMenuEntry("Set Phong Parameters", MENU_SET_PHONG_PARAMETERS);
 
  int subMenuId_halfTone = glutCreateMenu(callback_menu);
  glutAddMenuEntry("Toggle", MENU_HALF_TONE_TOGGLE );

  int menuId = glutCreateMenu(callback_menu);
  glutAddMenuEntry("Status", MENU_STATUS);
  glutAddSubMenu("Input", subMenuId_grabInput); 
  glutAddSubMenu("Half Tone", subMenuId_halfTone);
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
  window->res = 5;  
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

