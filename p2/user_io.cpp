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

void readFile(std::ifstream &ifs){
  char cinBuffer[256];
  std::cout<<"Please enter name of the input file: "; std::cout.flush();
  std::cin.getline(cinBuffer, 256);
  ifs.open (cinBuffer, std::ifstream::in);
}

/*=====================================================================*/
/* @fn        :     void readHeaders(std::ifstream *ifs, int*window_width, int*window_height, int*numberOfPolygons)
 * @brief     :     prime the pump - reads the header info from the stdin(redirected from a file to stdin), 
 *                  the header contains info about the dimension of the window and the numebr of polygons there are
 * @param[in] :     std::ifstream *ifs    - pointer to the input file stream
 * @param[out]:     int *window_width     - for storing window_width
 * @param[out]:     int *window_height    - for storing window_height
 * @param[out]:     int *numberOfPolygons - for storing the number of polygons
 * @return    :     none
 */

void readHeaders(std::ifstream *ifs, int*window_width, int*window_height, int*numberOfObjects){
  int sizeOfBuffer = 256;
  char buffer[sizeOfBuffer], *charPtr; 
  
  //prime the pump
  ifs->getline(buffer, sizeOfBuffer); // first line contains window dimension info
  charPtr = strtok(buffer, " ");
  *window_width = atoi(charPtr);
  charPtr =strtok(0, "\0 ");
  *window_height = atoi(charPtr);
  ifs->getline(buffer, sizeOfBuffer); //skip a line
  ifs->getline(buffer, sizeOfBuffer); // contains number of polygon
  *numberOfObjects = atoi(buffer);
  DPRINT("window_width: %d, window_height: %d, numberOfPolygons: %d\n", *window_width, *window_height, *numberOfPolygons);
}

/*========================================================================*/
/* @fn        :   void readPolygons(std::ifstream *ifs, Graph *graph, Polygon **polygons, int numberOfPolygons);
 * @brief     :   read data from stdin(a file redirected to stdin) and build polygons using the data init
 * @param[in] :   std::ifstream *ifs   - pointer to the input file stream
 * @param[in] :   Graph *graph         - an object for drawing pixel and related drawing methods, one of the param used to construct a Polygon object
 * @param[in] :   int numberOfPolygons - number of polygons
 * @param[out]:   Polygon **polygons   - the output: polygons created by the info specified in the file
 * @return    :   none
 */

void readPolygons(std::ifstream *ifs, Graph *graph, Polygon **polygons, int numberOfPolygons){ 
  int sizeOfBuffer = 256, numberOfPoints, x, y, numberOfPolygonsAlreadyProcessed = 0;
  char buffer[sizeOfBuffer], *charPtr; 
  Point *listOfPoints;
  
  while(ifs->getline(buffer, sizeOfBuffer)) { // skip a line for before entering the section for describing the next polygon
    ifs->getline(buffer, sizeOfBuffer);//read the number of points for constructing the new polygon 
    numberOfPoints =  atoi(buffer); 
     
    listOfPoints = new Point[numberOfPoints];
    for(int i = 0 ; i < numberOfPoints; i++){ // form a listOfPoints;
      ifs->getline(buffer, sizeOfBuffer); //get a 3D point
      charPtr = strtok(buffer, " ");
      x = atoi(charPtr); //x value;
      charPtr = strtok(0, "\0");
      y = atoi(charPtr);//y value
      listOfPoints[i] = {x,y,0};
      DPRINT("(%d, %d)\n", x, y);
    }
    polygons[numberOfPolygonsAlreadyProcessed] = new Polygon(listOfPoints, numberOfPoints, graph); 
    delete [] listOfPoints;
    
    DPRINT("\n"); 
    if( ++numberOfPolygonsAlreadyProcessed == numberOfPolygons) // already processed the desired number of polygons done!!
      break;
    if(ifs->eof()) //end of file - done!!
      break;
  }
}

/*========================================================================*/
/* @fn        :   void readPolhedra(std::ifstream *ifs, Graph **graphs, Polyhedron **polyhedra, int numberOfPolyhedra);
 * @brief     :   read data from an input file stream  and build polyhedra using the data 
 * @param[in] :   std::ifstream *ifs   - pointer to the input file stream
 * @param[in] :   Graph **graphs         - list of graphs for drawing pixel and related drawing methods, each graph takes up 1 quadrant of screen
 * @param[in] :   int numberOfPolyhedra - number of polyhedra
 * @param[out]:   Polyhedron **polyhedra   - the output: polyhdera created by the info specified in the file
 * @return    :   none
 */

void readPolyhedra(std::ifstream *ifs, Graph **graphs, Polyhedron **polyhedra, int numberOfPolyhedra){ 

  int sizeOfBuffer = 256, numberOfPoints, numberOfEdges, numberOfPolyhedraAlreadyProcessed = 0;
  float x, y , z;
  int p1Index, p2Index; 
  char buffer[sizeOfBuffer], *charPtr; 
  Point_3D *listOfPoints; Edge *listOfEdges;
  while(ifs->getline(buffer, sizeOfBuffer)) { // skip a line for before entering the section for describing the next polyhedron
    ifs->getline(buffer, sizeOfBuffer);//read the number of points for constructing the new polyhedron 
    numberOfPoints =  atoi(buffer); 
     
    listOfPoints = new Point_3D[numberOfPoints];
    for(int i = 0 ; i < numberOfPoints; i++){ // form a listOfPoints;
      ifs->getline(buffer, sizeOfBuffer); //get a 3D point
      charPtr = strtok(buffer, " ");
      x = atof(charPtr); //x value;
      charPtr = strtok(0, " ");
      y = atof(charPtr);//y value
      charPtr = strtok(0, "\0");
      z = atof(charPtr);
      listOfPoints[i] = {x,y,z};
      printf("(%.2f, %.2f, %.2f)\n", x, y, z);
    }
    
    //read edges
    ifs->getline(buffer, sizeOfBuffer); // read the number of edges
    numberOfEdges = atoi(buffer);
    listOfEdges = new Edge[numberOfEdges];
    for(int i = 0, offset = -1 ; i < numberOfEdges; i++){
      ifs->getline(buffer, sizeOfBuffer);
      charPtr = strtok(buffer, " ");
      p1Index = atoi(charPtr);
      charPtr = strtok(0, "\0");
      p2Index = atoi(charPtr);
      listOfEdges[i] = { p1Index + offset, p2Index + offset};  // make point 1 to have index (1 - 1) = 0
      printf("Edge %d %d\n", p1Index, p2Index);
    } 
    
    polyhedra[numberOfPolyhedraAlreadyProcessed] = new Polyhedron(graphs, listOfPoints, numberOfPoints, listOfEdges, numberOfEdges);
    delete [] listOfPoints;
    delete [] listOfEdges;

    DPRINT("\n"); 
    if( ++numberOfPolyhedraAlreadyProcessed == numberOfPolyhedra) // already processed the desired number of polyhedra done!!
      break;
    if(ifs->eof()) //end of file - done!!
      break;
  }
}


