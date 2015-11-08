#include "vector.h"

Vector ctov(Color c){
  return {c.r, c.g, c.b}; 
}

Color vtoc(Vector v){
  return {v.x, v.y, v.z};
}

Vector minus(Point_3D p2, Point_3D p1){
  return { p2.x - p1.x, p2.y-p1.y, p2.z - p1.z };
}
Vector minus(Vector v1, Vector v2) {
  return { v1.x - v2.x, v1.y-v2.y, v1.z - v2.z };
}

Vector unitVector(Point_3D p){
  float magnitude = sqrt(p.x * p.x + p.y * p.y + p.z * p.z);
  return { p.x / magnitude, p.y / magnitude, p.z / magnitude }; 
}

Vector unitVector(Vector p){
  float magnitude = sqrt(p.x * p.x + p.y * p.y + p.z * p.z);
  return { p.x / magnitude, p.y / magnitude, p.z / magnitude }; 
}

float dotProduct(Vector v1, Vector v2){
  return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

Vector multByScalar(Vector v, float scalar){
  return { v.x * scalar, v.y * scalar, v.z * scalar};    
}

Color multByScalar(Color c, float scalar){
  return { c.r * scalar, c.g * scalar, c.b * scalar};    
}

Vector add(Vector v1, Vector v2){
  return { v1.x + v2.x, v1.y + v2.y, v1.z + v2.z};
}

float magnitude(Vector v){
  return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

bool equal(Color c1, Color c2){
  return ( c1.r == c2.r && c1.g == c2.g && c1.b == c2.b);
}
Color linearInterpolation(float mid, float begin, float end, Color Ibegin, Color Iend){
  float denom = fabs(end - begin);
//  DPRINT("denom %.2f\n", denom); 
  Color addend1 = multByScalar(Iend,   fabs(mid - begin) /denom );
  Color addend2 = multByScalar(Ibegin, fabs(end - mid) /denom   );
 // printVector("addend1", ctov(Ibegin));
 // printVector("addend2", ctov(Iend));
  Vector result = ( add( ctov(addend1), ctov(addend2) ) );
 // printVector("linear interpolation", result);
  return vtoc(result);
}

void printVector(const char *tag, Vector v){
  DPRINT("%s: (%.2f, %.2f, %.2f)\n", tag, v.x, v.y, v.z);
}
void printColor(const char *tag, Color c){
  DPRINT("%s: (%.2f, %.2f, %.2f)\n", tag, c.r, c.g, c.b);
}
