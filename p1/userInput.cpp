#include "userInput.h"
float parseBufferForRotationAngle(std::string *buffer){
  const char *charPtr = buffer->c_str();
  return atof(charPtr);  
}
void parseBufferForScaleFactors(std::string *buffer, float *alpha, float *beta){
 char *c_str = new char [buffer->length()+1], *char_ptr; // since buffer->c_str return s const char ptr, have to make a copy
 std::strcpy (c_str, buffer->c_str());
 
 char_ptr = strtok(c_str, " "); 
 *alpha = atof(char_ptr);
 char_ptr = strtok(c_str, "\0");
 *beta = atof(char_ptr);
 delete [] c_str;
}
void parseBufferForTranslationFactors(std::string *buffer, int *x, int y){

}
