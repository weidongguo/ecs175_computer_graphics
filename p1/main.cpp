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

Window window = {1000, 500, 0, 0, {-200, 200, -200, 200}, {20, 20, 1.2, 1.2, 5}, STATE_GRAB_COMMANDS, &input_buffer };

void callback_keyboard(unsigned char key, int x, int y);
void callback_display();
void callback_menu(int state);
void createMenu();

void readHeaders(int *window_width, int *window_height, int *numberOfPolygons);
void readPolygons(Graph *graph, Polygon **polygons, int numberOfPolygons);

int main(int argc, char *argv[]){
  glutInit(&argc, argv); //initialize GL Utility Toolkit(GLUT) and  extract command line arguments for GLUT and keep the counts for the remaining arguments 
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB );

  readHeaders(&window.width, &window.height, &window.numberOfPolygons);//read from datafile for the window-dimension and numberOfPolygons there are 
  
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
  
  readPolygons(&graph, polygons, window.numberOfPolygons); // read the remaining part of the datafile

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
    case MENU_DDA:
      globalGraph->drawLine({-450, -200}, {-300, -100}, 0.5, 0.5, 0.5, DDA);
      break;
    case MENU_BRESENHAM:
      globalGraph->drawLine({-450, -200}, {-300, -150}, 0.5, 0.5, 0.5, BRESENHAM);
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
  int x_offset =0, y_offset = 0;  float scaleFactor = 1, angle = 0; bool isClipping = false;
  
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
  //glutPostRedisplay();
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


//create the pop of menu that can be triggered by right cliking within the opengl window
void createMenu(void){     
  int subMenuId_drawLine = glutCreateMenu(callback_menu);
  glutAddMenuEntry("DDA", MENU_DDA);
  glutAddMenuEntry("Bresenham", MENU_BRESENHAM);

  int subMenuId_grabInput = glutCreateMenu(callback_menu);
  glutAddMenuEntry("Rotation Angle", MENU_GRAB_ROTATION_ANGLE);
  glutAddMenuEntry("Scaling Factors", MENU_GRAB_SCALE_FACTORS);
  glutAddMenuEntry("Translation Factors", MENU_GRAB_TRANSLATION_FACTORS);
  glutAddMenuEntry("Clip Region", MENU_GRAB_CLIP_REGION); 

  int menuId = glutCreateMenu(callback_menu);
  glutAddSubMenu("Draw Line", subMenuId_drawLine);   
  glutAddMenuEntry("Status", MENU_STATUS);
  glutAddSubMenu("Grab Input", subMenuId_grabInput); 

  glutAttachMenu(GLUT_RIGHT_BUTTON);
}


