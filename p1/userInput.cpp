#include "userInput.h"
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
