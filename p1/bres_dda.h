// author: Weidong Guo
// header file for both bres.cpp and dda.cpp
// it provides bresenham line drawing algorithm and the DDA(Digital Differentail Analyzer) line drawing algorithm
//
#ifndef BRES_DDA_H
#define BRES_DDA_H

#include "graph.h"

void bresenham(Point p1, Point p2, float r, float g, float b, int (*drawPixelFunc)(int, int, float, float, float));//
//void dda(Point p1, Point p2, float r, float g, float b, int (*drawPixelFunc)(int, int, float, float, float) );//

#endif
