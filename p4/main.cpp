#include <GL/glut.h>
#include "graph.h"
#include "user_io.h"
#include <iostream>
#include <cstring>
#include "curve.h"
//#include <AntTweakBar.h>

float *PixelBuffer; // global pixel buffer
Graph *globalGraph;
std::string input_buffer;

Window window;

void callback_keyboard(unsigned char key, int x, int y);
void callback_display();
void callback_menu(int state);
void createMenu();
void windowInit(Window *window);
void updateScreen();

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
  createMenu();
  glutSetCursor(GLUT_CURSOR_CROSSHAIR);
  
  Graph graph(window.width,window.height, PixelBuffer); 

  globalGraph = &graph;//any function that wants to draw can use this pointer(globalGraph) to graph
  graph.fillScreen(1,1,1); // white background
  
  std::list<Curve*> curves; 
  readCurves(&ifs, &graph, &curves, window.numberOfCurves);
  for(std::list<Curve*>::iterator it = curves.begin(); it != curves.end(); it++){
    (*it)->printAttributes();
  }

  glutMainLoop();
  return 0;
}

void callback_menu(int state){
  switch(state){
    case MENU_STATUS:
      printf("======================Status====================\n");
      printf("Window Size is %d x %d\n", window.width, window.height);
      printf("Half Tone Color R: %.2f, G: %.2f B: %.2f\n", window.halfTone.r, window.halfTone.g, window.halfTone.b); 
      printf("** Phong Model Parameter ** \n");
      printf("Light Source Intensity Il: %.2f\n", window.scene.Il);
      printf("Ambient Light Intensity Ia: %.2f\n", window.scene.Ia);
      printf("From Point ff: (%.2f, %.2f, %.2f)\n", window.scene.ff.x, window.scene.ff.y, window.scene.ff.z);
      printf("Light Source xx: (%.2f, %.2f, %.2f)\n", window.scene.xx.x, window.scene.xx.y, window.scene.xx.z); 
      printf("Phong Constant n: %d\n", window.scene.n); 
      printf("Ambient Coeff ka, R: %.2f, G: %.2f B: %.2f\n", window.scene.ka.r, window.scene.ka.g, window.scene.ka.b);
      printf("Diffuse Coeff kd, R: %.2f, G: %.2f B: %.2f\n", window.scene.kd.r, window.scene.kd.g, window.scene.kd.b);
      printf("Specular Coeff ks, R: %.2f, G: %.2f B: %.2f\n", window.scene.ks.r, window.scene.ks.g, window.scene.ks.b);
      printf("=================End of Status==================\n");
      break;
  }
}

void callback_keyboard(unsigned char key, int x, int y){
  //DPRINT("ASCII: %d CHAR:%c <==> Cursor at (%d, %d)\n", key, key, x-window.width/2, window.height/2 - y);
  if( isdigit(key) ){ // selecting object to be manipulated, object are represtend by numeric id e.g. 0, 1, 2 ...
    window.selectedObject = key % (window.numberOfPolyhedra - 1); // -1 to make it not possible to select the rotional axis, the last element
    printf("\nObject with ID %d is selected\n", window.selectedObject);
    //globalPolyhedra[window.selectedObject]->printAttributes();
    return;
  }
  switch(key){ // control commands
    case 't': 
    case 'z': 
    case 'r': 
    case 's':// Polyhedron::savePolyhedraToFile(globalPolyhedra, &window, "output"); break;// saving the polygons 
    default: return;
  }
  
  //updateScreen(globalPolyhedra);

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
  
}

void updateScreen(){

}


