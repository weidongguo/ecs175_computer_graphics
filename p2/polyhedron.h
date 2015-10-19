#ifndef POLYHEDRON_H
#define POLYHEDRON_H

#include "common_type.h"
#include "graph.h"

class Polyhedron {
  Point_3D* listOfPoints;
  int numberOfPoints;

  Edge *listOfEdges; 
  int numberOfEdges;

  Point_3D centroid;

  Graph **graphs;

  public:
  Polyhedron(Graph **_graphs, Point_3D *_listOfPoints, int _numberOfPoints, Edge *_listOfEdges, int _numberOfEdges);
  void setCentroid();
  void printAttributes();
};

#endif
