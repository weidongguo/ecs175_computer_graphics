#include "userInput.h"
float parseBufferForRotationAngle(std::string *buffer){
  const char *charPtr = buffer->c_str();
  return atof(charPtr);  
}
void parseBufferForScaleFactors(std::string buffer, float *alpha, float *beta){

}
void parseBufferForTranslationFactors(std::string buffer, int *x, int y){

}
