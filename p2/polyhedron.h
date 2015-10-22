#ifndef POLYHEDRON_H
#define POLYHEDRON_H

#include "common_type.h"
#include "graph.h"

class Polyhedron {
  Point_3D* listOfPoints;
  Point_3D* listOfPointsNDC;
  int numberOfPoints;

  Edge *listOfEdges; 
  int numberOfEdges;

  Point_3D centroid;

  Graph **graphs;

  public:
  Polyhedron(Graph **_graphs, Point_3D *_listOfPoints, int _numberOfPoints, Edge *_listOfEdges, int _numberOfEdges);
  void setCentroid();
  void printAttributes();
  void draw();
  void draw(Color c);
  void draw(float r, float g, float b);
  void erase();
  void setNDC(float delta, float xMin, float yMin, float zMin);
  static void findNDCParams(Polyhedron **polyhedra, int numberOfPolyhedra, float *delta, float *xMin, float *yMin, float *zMin);      

  void scale(float alpha, float beta, float gamma);
  void scale(float alpha);
  void translate(float x_offset, float y_offset, float z_offset);


};

#endif
