#ifndef POLYHEDRON_H
#define POLYHEDRON_H

#include "common_type.h"
#include "graph.h"
#include <fstream>

class Polyhedron {
  Point_3D* listOfPoints;
  Point_3D* listOfPointsNDC;
  int numberOfPoints;

  Edge *listOfEdges; 
  int numberOfEdges;
  
  Surface *listOfSurfaces;
  int numberOfSurfaces;

  Point_3D centroid;

  Graph **graphs;
  public:
  Polyhedron(Graph **_graphs, Point_3D *_listOfPoints, int _numberOfPoints, Edge *_listOfEdges, int _numberOfEdges, Surface *s, int numberOfSurface);
  void setCentroid();
  void printAttributes();
  void draw();
  void draw(Color c);
  void draw(float r, float g, float b);
  void erase();
  void setNDC(float delta, float xMin, float yMin, float zMin);
  static void findNDCParams(Polyhedron **polyhedra, int numberOfPolyhedra, float *delta, float *xMin, float *yMin, float *zMin);      
  static bool isNDC(Point_3D p);

  void scale(float alpha, float beta, float gamma);
  void scale(float alpha);
  void translate(float x_offset, float y_offset, float z_offset);
  void rotate(Point_3D p1, Point_3D p2, float angle);
  static Point_3D minus(Point_3D p1, Point_3D p2); 
  static Point_3D unitVector(Point_3D p);
  static float dotProduct(Vector v1, Vector v2);
  static Vector multByScalar(Vector v, float scalar);
  static Vector add(Vector v1, Vector v2);
  static float magnitude(Vector v);

  static void printVector(const char *tag, Vector v);


  static void updateRotationAxis(Polyhedron **polyhedra, int numberOfPolyhedra, Point_3D *pairOfPoints);//rot axis is the last element of polyhedra
  static void savePolyhedraToFile(Polyhedron **polyhedra, Window *window, const char *filename);
  
  Point_3D obliqueProj(Point_3D dirVector, Point_3D p);
  static Vector phong(Point_3D p, Vector ka, Vector kd, Vector ks, float Ia, float Il, Vector nn, Point_3D ff, int n, Point_3D xx );
  static Vector phong(Point_3D p, Vector ka, Vector kd, Vector ks, float Ia, float Il, Vector nn, Point_3D ff, int n, double C, Vector ll, Vector rr, Vector vv);

};

#endif
