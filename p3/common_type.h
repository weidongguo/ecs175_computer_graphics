#ifndef COMMON_TYPE_H
#define COMMON_TYPE_H

#include <cstdio>
#include <string>

#define DEBUG_PRINT 1

#ifdef DEBUG_PRINT
  #define DPRINT printf
#else
  #define DPRINT(...) 
#endif

#define MIN(x,y) ( ( (x)<=(y) )? (x):(y) )
#define MAX(x,y) ( ( (x)> (y) )? (x):(y) )

#define PI 3.1415926

typedef unsigned char uint8;
typedef char          int8; 

typedef struct {
  float r;
  float g;
  float b;
}Color;

typedef struct{
  int x;
  int y;
  Color normalizedIntensity; 
} Point;

typedef struct{
  float x;
  float y;
  float z;
} Vector;

typedef struct {
  float x;
  float y;
  float z;
  Color kd; //diffusive  coefficient(the color of a point in the original object) ;
  Color ks; // specular coefficient(the highlightness of the material)
  Color ka; //ambient light coeff
  Vector nn;//normal vector;
  Color intensity; // directly from phont model
  Color normalizedIntensity; // relative to all points in the scene
}Point_3D;


typedef struct {
  int p1Index;  
  int p2Index;
} Edge;

typedef struct {
  int p1Index;
  int p2Index;
  int p3Index;
  Vector normalVector;
} TriSurface;

typedef TriSurface Surface;

typedef struct{
  int xMin;
  int xMax;
  int yMin;
  int yMax;
}ClipRegion;

typedef struct{
  float x_offset;
  float y_offset;
  float z_offset;
  float scale_alpha;
  float rotation_angle;
  Point_3D pairOfPointsForRotAxis[2];
}Transformation;

typedef struct{
  Color ka;
  float Il; 
  float Ia;
  Point_3D ff[3]; //coord of FROM POINTS, view from xy-plane, xz-plane, yz-plane
  Point_3D xx; // coord. of light src
  int n; // controls reflection intensity 
} Scene;

typedef struct{
  int width;
  int height;
  int numberOfPolygons;
  int numberOfPolyhedra; 
  int selectedObject;
  ClipRegion cr;
  Transformation tf;   
  int state;
  std::string *inputBuffer;
  void **graphs;
  Scene scene;
} Window;

#endif
