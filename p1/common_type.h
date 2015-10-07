#include <cstdio>
#ifdef DEBUG_PRINT
  #define DPRINT printf
#else
  #define DPRINT(...) 
#endif

#define MIN(x,y) ((x<=y)?x:y)
#define MAX(x,y) ((x>y)?x:y)

#ifndef COMMON_TYPE_H
#define COMMON_TYPE_H
typedef struct {
  int x;
  int y;
} Point; 

typedef Point Vertex;//Point and Vertex means the same thing

typedef struct {
  float r;
  float g;
  float b;
}Color;
#endif
