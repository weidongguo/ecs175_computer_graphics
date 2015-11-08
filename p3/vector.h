#ifndef VECTOR_H
#define VECTOR_H

#include "common_type.h"
#include <cmath>
//vector operations
Color    vtoc(Vector v);
Vector   ctov(Color c);
Vector   minus(Point_3D p1, Point_3D p2); 
Vector   minus(Vector v1, Vector v2); 
Vector   unitVector(Point_3D p);
Vector   unitVector(Vector v);

float dotProduct(Vector v1, Vector v2);
Vector multByScalar(Vector v, float scalar);
Color  multByScalar(Color c, float scalar); 
Vector add(Vector v1, Vector v2);
float magnitude(Vector v);

void printColor(const char *tag, Color c);
void printVector(const char *tag, Vector v);
Color linearInterpolation(float mid, float begin, float end, Color Ibegin, Color Iend);


#endif
