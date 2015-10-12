#ifndef USERINPUT_H
#define USERINPUT_H
#include <string>
#include <cstring>
#include <cstdlib>
float parseBufferForRotationAngle(std::string *buffer);
void parseBufferForScaleFactors(std::string *buffer, float *alpha, float *beta);
void parseBufferForTranslationFactors(std::string *buffer, int *x, int y);


#endif
