#ifndef USERINPUT_H
#define USERINPUT_H
#include <string>
#include <cstring>
#include <cstdlib>
#include "common_type.h"
float parseBufferForRotationAngle(std::string *buffer);
void parseBufferForScaleFactors(std::string *buffer, float *alpha, float *beta);
void parseBufferForTranslationFactors(std::string *buffer, int *x, int *y);
void parseBufferForClipRegion(std::string *buffer, ClipRegion *cr);

// for window
#define STATE_GRAB_COMMANDS                 0
#define STATE_GRAB_DATA_ROTATION_ANGLE      1
#define STATE_GRAB_DATA_TRANSLATION_FACTORS 2
#define STATE_GRAB_DATA_SCALE_FACTORS       3
#define STATE_GRAB_DATA_CLIP_REGION         4

//for menu
#define MENU_DDA                      2
#define MENU_BRESENHAM                3
#define MENU_STATUS                   4
#define MENU_GRAB_ROTATION_ANGLE      5
#define MENU_GRAB_TRANSLATION_FACTORS 6
#define MENU_GRAB_SCALE_FACTORS       7
#define MENU_GRAB_CLIP_REGION         8

#endif
