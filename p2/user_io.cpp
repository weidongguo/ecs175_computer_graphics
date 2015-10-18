#include "user_io.h"
float parseBufferForRotationAngle(std::string *buffer){
  const char *charPtr = buffer->c_str();
  return atof(charPtr);  
}
void parseBufferForScaleFactors(std::string *buffer, float *alpha, float *beta){
 char *c_str = new char [buffer->length()+1], *char_ptr; //buffer->c_str return a const char ptr, so have to make a copy since strtok has side effect
 std::strcpy (c_str, buffer->c_str());
 char_ptr = strtok(c_str, " "); 
 *alpha = atof(char_ptr);
 char_ptr = strtok(0, "\0");
 *beta = atof(char_ptr);
 delete [] c_str; //deallocate memory
}
void parseBufferForTranslationFactors(std::string *buffer, int *x_offset, int *y_offset){
  char *c_str = new char[buffer->length()+1], *char_ptr;
  strcpy(c_str, buffer->c_str());
  char_ptr = strtok(c_str, " "); // ends at a space
  *x_offset = atoi(char_ptr);
  char_ptr = strtok(0, "\0");
  *y_offset = atoi(char_ptr);
  delete [] c_str; // deallocate memroy
}

void parseBufferForClipRegion(std::string *buffer, ClipRegion *cr){
  char *c_str = new char[buffer->length()+1], *char_ptr;
  strcpy(c_str, buffer->c_str());
  char_ptr = strtok(c_str, " ");
  cr->xMin = atoi(char_ptr);
  char_ptr = strtok(0, " ");
  cr->xMax = atoi(char_ptr);
  char_ptr = strtok(0, " ");
  cr->yMin = atoi(char_ptr);
  char_ptr = strtok(0, "\0");
  cr->yMax = atoi(char_ptr);
  delete [] c_str; //deallocate unused memory
}

void parseBufferForLine(std::string *buffer, Point *p1, Point *p2){
  char *c_str = new char[buffer->length()+1], *char_ptr;
  strcpy(c_str, buffer->c_str() );
  char_ptr = strtok(c_str, " ");
  p1->x = atoi(char_ptr);
  char_ptr = strtok(0, " ");
  p1->y = atoi(char_ptr);
  char_ptr = strtok(0, " ");
  p2->x = atoi(char_ptr);
  char_ptr = strtok(0, "\0");
  p2->y = atoi(char_ptr);
  delete [] c_str; //deallocate mem
}

bool isGrabbingData(int state){
  return ( state == STATE_GRAB_DATA_ROTATION_ANGLE || state == STATE_GRAB_DATA_SCALE_FACTORS || state == STATE_GRAB_DATA_TRANSLATION_FACTORS ||
           state == STATE_GRAB_DATA_CLIP_REGION  || state == STATE_GRAB_DATA_DRAW_DDA || state == STATE_GRAB_DATA_DRAW_BRESENHAM) ;
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


