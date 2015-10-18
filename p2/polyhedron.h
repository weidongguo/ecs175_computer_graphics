#ifndef POLYHEDRON_H
#define POLYHEDRON_H

#include "common_type.h"
#include "polygon.h"

class Polyhedron {
  Point_3D* listOfPoints;
  int numberOfPoints;

  Edge *listOfEdges; 
  int numberOfEdges;

  Point_3D centroid;

  public:
  Polyhedron(Graph *_graph, Point_3D *_listOfPoints, int _numberOfPoints, Edge *_listOfEdges, int _numberOfEdges);
  void setCentroid();
};

#endif
