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


