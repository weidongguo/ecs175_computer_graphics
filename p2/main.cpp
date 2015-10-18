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
Graph *globalGraph;
std::string input_buffer;
Line *globalLine[2]; //one for demonstrating dda, the other one for demonstrating bresenham

Window window = {1000, 500, 0, 0, {-200, 200, -200, 200}, {20, 20, 1.2, 1.2, 5}, STATE_GRAB_COMMANDS, &input_buffer };

void callback_keyboard(unsigned char key, int x, int y);
void callback_display();
void callback_menu(int state);
void createMenu();

int main(int argc, char *argv[]){
  glutInit(&argc, argv); //initialize GL Utility Toolkit(GLUT) and  extract command line arguments for GLUT and keep the counts for the remaining arguments 
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB );
  
  std::ifstream ifs;
  readFile(ifs);
  readHeaders(&ifs, &window.width, &window.height, &window.numberOfPolygons);//read from datafile for the window-dimension and numberOfPolygons there are 
  
  glutInitWindowSize(window.width, window.height);
  glutInitWindowPosition(100, 100); 
  int windowID = glutCreateWindow("Project 1");
  
  glClearColor(1,1,1,0.0); //the background_color_buffer underneath the pixelbuffer
  PixelBuffer = new float[window.width*window.height*3];   
  Graph graph(window.width,window.height, PixelBuffer); 
  globalGraph = &graph;//any function that wants to draw can use this pointer(globalGraph) to graph
  graph.fillScreen(1,1,1); // white background
  
  Polygon *polygons[window.numberOfPolygons];
  globalPolygons = polygons;
  
  readPolygons(&ifs, &graph, polygons, window.numberOfPolygons); // read the remaining part of the datafile

  for(int i = 0 ; i< window.numberOfPolygons; i++){ //display the polygons read from the datafile
    polygons[i]->setColor( { (float)i/window.numberOfPolygons , 0.3, 0.4 } );
    polygons[i]->draw();
    polygons[i]->rasterize();
  }
  
  //callback registration:
  glutKeyboardFunc(callback_keyboard); 
  glutDisplayFunc(callback_display);
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
          globalLine[0] = new Line( p1, p2, globalGraph);
          globalLine[0] -> draw(DDA); 
          break;
        case STATE_GRAB_DATA_DRAW_BRESENHAM: 
          parseBufferForLine(window.inputBuffer, &p1, &p2); 
          if(globalLine[1] != 0)
            delete globalLine[1];
          globalLine[1] = new Line( p1, p2, globalGraph);
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
    case 'r': globalPolygons[window.selectedObject]->rotate(window.tf.rotation_angle); break; //rotation
    case 'c': isClipping = true; break; //clipping
    case 's': Polygon::savePolygonsToFile(globalPolygons, &window, "output"); break;// saving the polygons 
    default: return;
  }
  if(isClipping)
    globalPolygons[window.selectedObject]->clip(window.cr); 
  else{ 
    globalPolygons[window.selectedObject]->rasterize();
  }

  glutPostRedisplay();
}

void callback_display(){
  glClear(GL_COLOR_BUFFER_BIT);
  glLoadIdentity();
  glDrawPixels(window.width, window.height, GL_RGB, GL_FLOAT, PixelBuffer);
 
  glFlush(); //force all GL commands to be executed by the actual rendering engine

 // TwDraw();
  //glutSwapBuffers();
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


