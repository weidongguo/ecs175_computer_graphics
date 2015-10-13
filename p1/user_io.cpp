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

bool isGrabbingData(int state){
  return ( state == STATE_GRAB_DATA_ROTATION_ANGLE || state == STATE_GRAB_DATA_SCALE_FACTORS || state == STATE_GRAB_DATA_TRANSLATION_FACTORS ||
           state == STATE_GRAB_DATA_CLIP_REGION ) ;
}


/*================================================================*/
/* @fn      :    void savePolygonsToFile(Polygons **polygons, int numberOfPolygons, char *filename)
 * @brief   :    store the number of polygons, and the points for each polygon to a file in the same format as the datafile
 *
 * @return  :    none
 */
/*void savePolygonsToFile(Polygon **polygons, Window *window, const char *filename){
  printf("Saving polygons to a file: %s\n...\nDone.\n", filename);
  std::ofstream ofs(filename, std::ofstream::out);
  ofs << window->width << " " << window->height << "\n\n";  //dimension of opengl window <width><space><height>
  ofs << window->numberOfPolygons; // number of polygons <numberOfPolygons>
  
  int numberOfPoints, numberOfPolygons = window->numberOfPolygons, x, y;
  for(int i = 0 ; i < numberOfPolygons ; i++){
    ofs << "\n\n" ; 
    numberOfPoints = polygons[i]->numberOfPoints;
    ofs << numberOfPoints << "\n";  // <numberOfPoints>
    for(int j = 0 ; j < numberOfPoints; j++){
      x = polygons[i]->listOfPoints[j].x;
      y = polygons[i]->listOfPoints[j].y;
      ofs << x << " " << y << "\n"; // <x><space><y>
    }
  }

  ofs.close();
}*/
