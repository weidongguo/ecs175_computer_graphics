#include <GL/glut.h>
#include "graph.h"
#include "user_io.h"
#include <iostream>
#include <cstring>
//#include <AntTweakBar.h>

float *PixelBuffer; // global pixel buffer
Polyhedron **globalPolyhedra;
Graph *globalGraphs[4];
std::string input_buffer;

Window window;

float *SubWindowPixelBuffer1;
float *SubWindowPixelBuffer2;
float *SubWindowPixelBuffer3;

void callback_keyboard(unsigned char key, int x, int y);
void callback_display();
void callback_subdisplay1();
void callback_subdisplay2();
void callback_subdisplay3();
void callback_menu(int state);
void createMenu();
void windowInit(Window *window);
void drawPolyhedra(Polyhedron **polyhedra);
void updateScreen(Polyhedron **polyhedra);

int main(int argc, char *argv[]){
  glutInit(&argc, argv); //initialize GL Utility Toolkit(GLUT) and  extract command line arguments for GLUT and keep the counts for the remaining arguments 
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB );
  
  windowInit(&window);

  std::ifstream ifs;
  readFile(ifs);
  readHeaders(&ifs, &window.numberOfPolyhedra);//read from datafile for the window-dimension and numberOfPolyhedra there are 
  
  glutInitWindowSize(window.width, window.height);
  glutInitWindowPosition(100, 100); 

  // initialize the pixel buffers
  PixelBuffer = new float[window.width*window.height*3];   
  SubWindowPixelBuffer1 = new float[ (window.width* window.height/4) * 3];
  SubWindowPixelBuffer2 = new float[ (window.width* window.height/4) * 3];
  SubWindowPixelBuffer3 = new float[ (window.width* window.height/4) * 3];
  
  // create sub-windows and their callback functions
  int mainWindowID = glutCreateWindow("Project 2");
  glutDisplayFunc(callback_display);

  int subWindowID1 = glutCreateSubWindow(mainWindowID, 0, 0, window.width/2, window.height/2);
  glutDisplayFunc(callback_subdisplay1);
  glutKeyboardFunc(callback_keyboard); 
  createMenu();


  int subWindowID2 = glutCreateSubWindow(mainWindowID, window.width/2, 0, window.width/2, window.height/2);
  glutDisplayFunc(callback_subdisplay2);
  glutKeyboardFunc(callback_keyboard); 
  createMenu();

  int subWindowID3 = glutCreateSubWindow(mainWindowID, 0, window.height/2, window.width/2, window.height/2);
  glutDisplayFunc(callback_subdisplay3);
  glutKeyboardFunc(callback_keyboard); 
  createMenu();


  Graph graph(window.width,window.height, PixelBuffer); 
  globalGraphs[3] = &graph;//any function that wants to draw can use this pointer(globalGraph) to graph
  graph.fillScreen(1,1,1); // white background
  
  Graph subGraph1(window.width/2, window.height/2, SubWindowPixelBuffer1);
  subGraph1.fillScreen(0.9,0.9,0.9);
  globalGraphs[0] = &subGraph1; 

  Graph subGraph2(window.width/2, window.height/2, SubWindowPixelBuffer2);
  subGraph2.fillScreen(0.7, 0.7, 0.7);
  globalGraphs[1] = &subGraph2; 

  Graph subGraph3(window.width/2, window.height/2, SubWindowPixelBuffer3);
  subGraph3.fillScreen(0.5,0.5,0.5);
  globalGraphs[2] = &subGraph3;

  Polyhedron *polyhedra[window.numberOfPolyhedra];
  globalPolyhedra = polyhedra;
  DPRINT("Read polyhedra, number of polyhdra:%d\n", window.numberOfPolyhedra); 
  readPolyhedra(&ifs, globalGraphs, polyhedra, window.numberOfPolyhedra); 
  drawPolyhedra(polyhedra); // draw polyhedra(objects) the first time 

  //callback registration:
  glutSetWindow(mainWindowID);
  glutKeyboardFunc(callback_keyboard); 
  createMenu();
  
  glutSetCursor(GLUT_CURSOR_CROSSHAIR);
  glutMainLoop();
  return 0;
}

void callback_menu(int state){
  Point_3D p1, p2;
  switch(state){
    case MENU_STATUS:
      printf("======================Status====================\n");
      printf("Window Size is %d x %d\n", window.width, window.height);
      printf("Half Tone Color R: %.2f, G: %.2f B: %.2f\n", window.halfTone.r, window.halfTone.g, window.halfTone.b); 
      printf("=================End of Status==================\n");
      break;
    case MENU_HALF_TONE_TOGGLE:
      if( window.state == STATE_HALF_TONE_OFF){
        window.state = STATE_HALF_TONE_ON;
        for(int i = 0 ; i < 3; i ++) 
          globalGraphs[i]->halfTone( window.halfTone ); //turn half tone mode on
      }
      else {
        window.state = STATE_HALF_TONE_OFF;
        for(int i = 0 ; i < 3; i ++) 
          globalGraphs[i]->restorePixelBuffer(); // turn half tone mode off
      }
      break;
    case MENU_SET_HALF_TONE_COLOR: 
      printf("Please enter the color for half toning (value between 0.0 and 1.0) \n");  
      printf("R: "); 
      scanf("%f", &window.halfTone.r);
      printf("G: ");
      scanf("%f", &window.halfTone.g);
      printf("B: ");
      scanf("%f", &window.halfTone.b);
      printf("half tone color recorded\n");
      break;
  }
}

void callback_keyboard(unsigned char key, int x, int y){
  //DPRINT("ASCII: %d CHAR:%c <==> Cursor at (%d, %d)\n", key, key, x-window.width/2, window.height/2 - y);
  if( isdigit(key) ){ // selecting object to be manipulated, object are represtend by numeric id e.g. 0, 1, 2 ...
    window.selectedObject = key % (window.numberOfPolyhedra - 1); // -1 to make it not possible to select the rotional axis, the last element
    printf("\nObject with ID %d is selected\n", window.selectedObject);
    globalPolyhedra[window.selectedObject]->printAttributes();
    return;
  }
  switch(key){ // control commands
    case 't': globalPolyhedra[window.selectedObject]->translate(window.tf.x_offset, window.tf.y_offset, window.tf.z_offset); break; //translation
    case 'z': globalPolyhedra[window.selectedObject]->scale(window.tf.scale_alpha); break; //scale
    case 'r': globalPolyhedra[window.selectedObject]->rotate(window.tf.pairOfPointsForRotAxis[0], window.tf.pairOfPointsForRotAxis[1],window.tf.rotation_angle); break; //rotation
    case 's': Polyhedron::savePolyhedraToFile(globalPolyhedra, &window, "output"); break;// saving the polygons 
    default: return;
  }
  
  updateScreen(globalPolyhedra);

  glutPostRedisplay();
}

void callback_display(){
  //glClear(GL_COLOR_BUFFER_BIT);
  glLoadIdentity();
  glDrawPixels(window.width, window.height, GL_RGB, GL_FLOAT, PixelBuffer);
 
  glFlush(); //force all GL commands to be executed by the actual rendering engine

  glutPostRedisplay();
}

void callback_subdisplay1(){
  //glClear(GL_COLOR_BUFFER_BIT);
  glDrawPixels(window.width/2, window.height/2, GL_RGB, GL_FLOAT, SubWindowPixelBuffer1);
  glFlush(); //force all GL commands to be executed by the actual rendering engine
  glutPostRedisplay();
}

void callback_subdisplay2(){
  //glClear(GL_COLOR_BUFFER_BIT);
  glDrawPixels(window.width/2, window.height/2, GL_RGB, GL_FLOAT, SubWindowPixelBuffer2);
  glFlush(); //force all GL commands to be executed by the actual rendering engine
  glutPostRedisplay();
}

void callback_subdisplay3(){
  //glClear(GL_COLOR_BUFFER_BIT);
  glDrawPixels(window.width/2, window.height/2, GL_RGB, GL_FLOAT, SubWindowPixelBuffer3);
  glFlush(); //force all GL commands to be executed by the actual rendering engine
  glutPostRedisplay();
}

//create the pop of menu that can be triggered by right cliking within the opengl window
void createMenu(void){     
  int subMenuId_grabInput = glutCreateMenu(callback_menu);
  glutAddMenuEntry("Set Half Tone Color", MENU_SET_HALF_TONE_COLOR);
 
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
  window->graphs = (void**)globalGraphs;
  window->tf.pairOfPointsForRotAxis[0] = { 0,0,0};
  window->tf.pairOfPointsForRotAxis[1] = { 1,1,1};
  window->state = STATE_HALF_TONE_OFF;
  
  window->scene.xx = {10,10,10};
  window->scene.ff[0] = {0, 0, 10}; //looking from on top of xy plane
  window->scene.ff[1] = {0, 10, 0}; //looking from on top of xz plane
  window->scene.ff[2] = {10, 0, 0}; //looking from on top of yz plane
  window->scene.Il = 0.7;
  window->scene.Ia = 0.1;
  window->scene.n = 9;
  window->scene.ka = {0.3, 0.3, 0.3};
  window->scene.kd = {1,0,0};
  window->scene.ks = {1,0,0}; 
  window->halfTone = COLOR_CRIMSON;
}

void updateScreen(Polyhedron **polyhedra){
  for(int i = 0 ; i < window.numberOfPolyhedra; i++){ // clear what we have before
    polyhedra[i]->erase(); // make sure that there are something for it to be erased
  }
  drawPolyhedra(polyhedra);  
}

void drawPolyhedra(Polyhedron **polyhedra){
  float delta, xMin, yMin, zMin;
  Polyhedron::findNDCParams(polyhedra, window.numberOfPolyhedra, &delta, &xMin, &yMin, &zMin); 
  
  //setPhoneParams sets normal vectors for each point and the ka, kd, and ks 
  Polyhedron::setPhongParams(polyhedra, window.numberOfPolyhedra, window.scene.ka, window.scene.kd, window.scene.ks ); 
  
  Polyhedron::applyPhong(polyhedra, window.numberOfPolyhedra, window.scene.Ia, window.scene.Il, window.scene.ff[0], window.scene.xx,  window.scene.n);
  //Polyhedron::normalizeIntensities(polyhedra, window.numberOfPolyhedra);
  //please note that for calculating the intensities for the original vertices, NON-NDC coord is used.
  //for later calculating the intensities for the edges and scanlines, NDC coord is used for the linear-interpolation.
  //this is not a problem because linear-interpolation takes ratio. e.g. (j - j2) / (j1 - j2) * I1 + ...
  //
  //Polyhedron::paintersAlgo(polyhedra, window.numberOfPolyhedra, window.scene.ff[0]);//get ready for painter's algo

  for(int i = 0; i < window.numberOfPolyhedra ; i++){ // all the polyhedra EXCEPT for the last one
    polyhedra[i]->normalizeIntensities(); //needs to normalize intensities before copying setNDC
    polyhedra[i]->setNDC(delta, xMin, yMin, zMin);  //update new ndc
  }

  Polyhedron::paintersAlgo(polyhedra, window.numberOfPolyhedra, 0);//xy-plane, sort base on z-depth 
  for(int i = 0 ; i < window.numberOfPolyhedra; i++){  
   polyhedra[i]->rasterize(0);
  }
  
  Polyhedron::paintersAlgo(polyhedra, window.numberOfPolyhedra, 1);//xz-plane, sort base on y-depth 
  for(int i = 0 ; i < window.numberOfPolyhedra; i++){  
   polyhedra[i]->rasterize(1);
  }
  Polyhedron::paintersAlgo(polyhedra, window.numberOfPolyhedra, 2);//yz-plane, sort base on x-depth 
  for(int i = 0 ; i < window.numberOfPolyhedra; i++){  
   polyhedra[i]->rasterize(2);
   polyhedra[i]->printAttributes();
  //polyhedra[i]->printContourPoints();
  }
  
}

