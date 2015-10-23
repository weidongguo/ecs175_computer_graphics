#include <GL/glut.h>
#include "graph.h"
#include "polygon.h"
#include "line.h"
#include "user_io.h"
#include <iostream>
#include <cstring>
//#include <AntTweakBar.h>

float *PixelBuffer; // global pixel buffer
Polygon **globalPolygons;
Polyhedron **globalPolyhedra;
Graph *globalGraphs[4];
std::string input_buffer;
Line *globalLine[2]; //one for demonstrating dda, the other one for demonstrating bresenham

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
void updateScreen(Polyhedron **polyhedra);

int main(int argc, char *argv[]){
  glutInit(&argc, argv); //initialize GL Utility Toolkit(GLUT) and  extract command line arguments for GLUT and keep the counts for the remaining arguments 
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB );
  
  windowInit(&window);

  std::ifstream ifs;
  readFile(ifs);
  readHeaders(&ifs, &window.width, &window.height, &window.numberOfPolyhedra);//read from datafile for the window-dimension and numberOfPolyhedra there are 
  
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

  int subWindowID2 = glutCreateSubWindow(mainWindowID, window.width/2, 0, window.width/2, window.height/2);
  glutDisplayFunc(callback_subdisplay2);

  int subWindowID3 = glutCreateSubWindow(mainWindowID, 0, window.height/2, window.width/2, window.height/2);
  glutDisplayFunc(callback_subdisplay3);


  Graph graph(window.width,window.height, PixelBuffer); 
  globalGraphs[0] = &graph;//any function that wants to draw can use this pointer(globalGraph) to graph
  graph.fillScreen(1,1,1); // white background
  
  Graph subGraph1(window.width/2, window.height/2, SubWindowPixelBuffer1);
  subGraph1.fillScreen(0.9,0.9,0.9);
  globalGraphs[1] = &subGraph1; 

  Graph subGraph2(window.width/2, window.height/2, SubWindowPixelBuffer2);
  subGraph2.fillScreen(0.7, 0.7, 0.7);
  globalGraphs[2] = &subGraph2; 

  Graph subGraph3(window.width/2, window.height/2, SubWindowPixelBuffer3);
  subGraph3.fillScreen(0.5,0.5,0.5);
  globalGraphs[3] = &subGraph3;

  Polyhedron *polyhedra[window.numberOfPolyhedra+1];
  globalPolyhedra = polyhedra;
  DPRINT("Read polyhedra, number of polyhdra:%d\n", window.numberOfPolyhedra); 
  readPolyhedra(&ifs, globalGraphs, polyhedra, window.numberOfPolyhedra); 
  
  //in addition of the polyhedra read from the datafile:
  //add a  an rotional axis
  Edge edge = {0 , 1};
  polyhedra[window.numberOfPolyhedra++] = new Polyhedron(globalGraphs, window.pairOfPointsForRotAxis ,2, &edge, 1);


  float delta, xMin, yMin, zMin;
  for(int i = 0 ; i < window.numberOfPolyhedra ; i++){
    polyhedra[i]->printAttributes();
    Polyhedron::findNDCParams(polyhedra, window.numberOfPolyhedra, &delta, &xMin, &yMin, &zMin); 
    polyhedra[i]->setNDC(delta, xMin, yMin, zMin); 
    polyhedra[i]->draw();
  }
  
  updateScreen(polyhedra);
  
  //callback registration:
  glutSetWindow(mainWindowID);
  glutKeyboardFunc(callback_keyboard); 
  createMenu();
  
  
  glutSetCursor(GLUT_CURSOR_CROSSHAIR);
  glutMainLoop();
  return 0;
}

void callback_menu(int state){
  switch(state){
    case MENU_DRAW_DDA:
      window.state = STATE_GRAB_DATA_DRAW_DDA;
      printf("Please enter two points for line drawing using DDA (format <x0> <y0> <x1> <y1> e.g. 0 0 50 50 )\n");
      break;
    case MENU_DRAW_BRESENHAM:
      printf("Please enter two points for line drawing using BRESENHAM (format <x0> <y0> <x1> <y1> e.g. 0 0 50 50 )\n");
      window.state = STATE_GRAB_DATA_DRAW_BRESENHAM; 
      break;
    case MENU_CLIP_DDA:
      if( globalLine[0] != 0 )
        globalLine[0]->clip(window.cr);
      break;
    case MENU_CLIP_BRESENHAM:
      if( globalLine[1] !=0 )
        globalLine[1]->clip(window.cr);
      break;
    case MENU_STATUS:
      printf("======================Status====================\n");
      printf("Window Size is %d x %d\n", window.width, window.height);
      printf("Clipping Region: xMin = %d, xMax = %d, yMin = %d, yMax = %d\n", window.cr.xMin, window.cr.xMax, window.cr.yMin, window.cr.yMax);
      printf("Rotation Angle: %.2f\n", window.tf.rotation_angle);
      printf("Scale Factor: Alpha = %.2f,  Beta = %.2f\n", window.tf.scale_alpha, window.tf.scale_beta);
      printf("Translation Factor: x = %d, y = %d\n", window.tf.x_offset, window.tf.y_offset);
      printf("=================End of Status==================\n");
      break;
    case MENU_GRAB_ROTATION_ANGLE:
      window.state = STATE_GRAB_DATA_ROTATION_ANGLE;
      printf("Please enter the rotation angle ( format <float> ; e.g. 6.5):\n"); 
      break;
    case MENU_GRAB_SCALE_FACTORS:
      window.state = STATE_GRAB_DATA_SCALE_FACTORS;
      printf("Please enter the scale factors ( format <float><space><float> ; e.g. 1.2 1.2 ):\n");
      break;
    case MENU_GRAB_TRANSLATION_FACTORS:
      window.state = STATE_GRAB_DATA_TRANSLATION_FACTORS; // will be used in callback_keyboard()
      printf("Please enter the translation factors ( format <int><space><int> ; e.g. 30 -40 ):\n");
      break;
    case MENU_GRAB_CLIP_REGION:
      window.state = STATE_GRAB_DATA_CLIP_REGION;
      printf("Please enter the xMin xMax yMin yMax for the clip region ( e.g. -150 150 -200 200 ):\n"); 
      break;
  }
}

void callback_keyboard(unsigned char key, int x, int y){
  //DPRINT("ASCII: %d CHAR:%c <==> Cursor at (%d, %d)\n", key, key, x-window.width/2, window.height/2 - y);
  bool isClipping = false; Point p1, p2;
  
  if( isGrabbingData(window.state) ){
    if(key == '\n' || key =='\r'){ // if press ENTER, process the input data
      switch(window.state){
        case STATE_GRAB_DATA_ROTATION_ANGLE:
          window.tf.rotation_angle = parseBufferForRotationAngle(window.inputBuffer); 
          break;
        case STATE_GRAB_DATA_SCALE_FACTORS:
          parseBufferForScaleFactors(window.inputBuffer, &window.tf.scale_alpha, &window.tf.scale_beta); 
          break;
        case STATE_GRAB_DATA_TRANSLATION_FACTORS:
          parseBufferForTranslationFactors(window.inputBuffer, &window.tf.x_offset, &window.tf.y_offset); 
          break;
        case STATE_GRAB_DATA_CLIP_REGION:
          parseBufferForClipRegion(window.inputBuffer, &window.cr);
          break;
        case STATE_GRAB_DATA_DRAW_DDA:
          parseBufferForLine(window.inputBuffer, &p1, &p2);
          if(globalLine[0] != 0)
            delete globalLine[0]; // free the previous line
          globalLine[0] = new Line( p1, p2, globalGraphs[0]);
          globalLine[0] -> draw(DDA); 
          break;
        case STATE_GRAB_DATA_DRAW_BRESENHAM: 
          parseBufferForLine(window.inputBuffer, &p1, &p2); 
          if(globalLine[1] != 0)
            delete globalLine[1];
          globalLine[1] = new Line( p1, p2, globalGraphs[0]);
          globalLine[1]->draw(BRESENHAM); 
          break;
      } 
      window.state = STATE_GRAB_COMMANDS; // resume to grab commands mode
      window.inputBuffer->clear(); //clear buffer so it'll be ready for next time
      std::cout << "\nData Recorded!\nBack to Command Mode."<< std::endl;
    }
    else{ // else continue storing input in a buffer
      std::cout << key; std::cout.flush(); //echo to the console
      *(window.inputBuffer) += key; //storing part
    }
    return;
  }

  if( isdigit(key) ){ // selecting object to be manipulated, object are represtend by numeric id e.g. 0, 1, 2 ...
    window.selectedObject = key % window.numberOfPolygons;
    return;
  }
  char cinBuffer[256];
  switch(key){ // control commands
    case 't': globalPolygons[window.selectedObject]->translate(window.tf.x_offset, window.tf.y_offset); break; //translation
    case 'z': globalPolygons[window.selectedObject]->scale(window.tf.scale_alpha, window.tf.scale_beta); break; //scale
    case 'r': globalPolyhedra[window.selectedObject]->rotate(window.pairOfPointsForRotAxis[0], window.pairOfPointsForRotAxis[1], 2); break; //rotation
    case 'c': isClipping = true; break; //clipping
    case 's': Polygon::savePolygonsToFile(globalPolygons, &window, "output"); break;// saving the polygons 
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
  int subSubMenuId_lineDraw = glutCreateMenu(callback_menu); 
  glutAddMenuEntry("DDA", MENU_DRAW_DDA);
  glutAddMenuEntry("Bresenham", MENU_DRAW_BRESENHAM);

  int subSubMenuId_lineClip = glutCreateMenu( callback_menu); 
  glutAddMenuEntry("the line drew using DDA", MENU_CLIP_DDA);
  glutAddMenuEntry("the line drew using Bresenham", MENU_CLIP_BRESENHAM);


  int subMenuId_line = glutCreateMenu(callback_menu);
  glutAddSubMenu("Draw", subSubMenuId_lineDraw); 
  glutAddSubMenu("Clip", subSubMenuId_lineClip);

  int subMenuId_grabInput = glutCreateMenu(callback_menu);
  glutAddMenuEntry("Rotation Angle", MENU_GRAB_ROTATION_ANGLE);
  glutAddMenuEntry("Scaling Factors", MENU_GRAB_SCALE_FACTORS);
  glutAddMenuEntry("Translation Factors", MENU_GRAB_TRANSLATION_FACTORS);
  glutAddMenuEntry("Clip Region", MENU_GRAB_CLIP_REGION); 

  int menuId = glutCreateMenu(callback_menu);
  glutAddSubMenu("Line", subMenuId_line);   
  glutAddMenuEntry("Status", MENU_STATUS);
  glutAddSubMenu("Grab Input", subMenuId_grabInput); 

  glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void windowInit(Window *window){
  window->width = 1000;
  window->height = 500;
  window->numberOfPolygons = 0;
  window->numberOfPolyhedra = 0;
  window->cr = { -200, 200, -200, 200};
  window->tf = { 20, 20, 1.2, 1.2, 5};
  window->state = STATE_GRAB_COMMANDS;
  window->inputBuffer = &input_buffer;
  window->graphs = (void**)globalGraphs;
  window->pairOfPointsForRotAxis[0] = { -1.5, -1.5 , -1.5};
  window->pairOfPointsForRotAxis[1] = { 1.5, 1.5, 1.5};
}

void updateScreen(Polyhedron **polyhedra){
  float delta, xMin, yMin, zMin;
  for(int i = 0 ; i < window.numberOfPolyhedra; i++){ // clear what we have before
    DPRINT("ERASING Polygon %d ...\n", i+1); 
    polyhedra[i]->erase(); // make sure that there are something for it to be erased
    DPRINT("ERASED Polygon %d\n", i+1); 
  }
  Polyhedron::findNDCParams(polyhedra, window.numberOfPolyhedra, &delta, &xMin, &yMin, &zMin); 
  for(int i = 0; i < window.numberOfPolyhedra -1; i++){ // all the polyhedra EXCEPT for the last one
    polyhedra[i]->setNDC(delta, xMin, yMin, zMin);  //update new ndc
    polyhedra[i]->draw();
  }
  
  //last one
  //draw the rotional axis - it's always the last polyhedron of the list of polyhedra ( not really a polyhedron, but rather a line living in 3d world)
  int indexOfRotAxis = window.numberOfPolyhedra-1;
  polyhedra[indexOfRotAxis]->setNDC(delta, xMin, yMin, zMin);
  polyhedra[indexOfRotAxis]->draw(1,0,0); // give it green color
}

