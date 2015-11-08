#ifndef POLYHEDRON_H
#define POLYHEDRON_H

#include "common_type.h"
#include "graph.h"
#include "vector.h"
#include <fstream>
#include <list>

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
  
  std::list<Point> *listOfContourPoints[3]; //index 0 for xy, 1 for xz, 2 for yz
  //the original points + the points along the lines between the original points
  int numberOfPlanes;

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
 /* 
  //vector operations
  static Color    vtoc(Vector v);
  static Vector   ctov(Color c);
  static Vector   minus(Point_3D p1, Point_3D p2); 
  static Vector   minus(Vector v1, Vector v2); 
  static Vector   unitVector(Point_3D p);
  static Vector   unitVector(Vector v);

  static float dotProduct(Vector v1, Vector v2);
  static Vector multByScalar(Vector v, float scalar);
  static Color  multByScalar(Color c, float scalar); 
  static Vector add(Vector v1, Vector v2);
  static float magnitude(Vector v);
  
  static void printVector(const char *tag, Vector v);
  Color linearInterpolation(float mid, float begin, float end, Color Ibegin, Color Iend);

  //end of vector operations
*/
  static void updateRotationAxis(Polyhedron **polyhedra, int numberOfPolyhedra, Point_3D *pairOfPoints);//rot axis is the last element of polyhedra
  static void savePolyhedraToFile(Polyhedron **polyhedra, Window *window, const char *filename);
  
  Point_3D obliqueProj(Point_3D dirVector, Point_3D p);
  static Color phong(Point_3D p,  float Ia, float Il, Point_3D ff, Point_3D xx, int n);
  static Vector phong(Point_3D p, Vector ka, Vector kd, Vector ks, Vector nn, float Ia, float Il, Point_3D ff, int n, double C, Vector ll, Vector rr, Vector vv);
  void  setPhongKCoeff(Color ka, Color kd, Color ks); 
  static void setPhongKCoeffForEachPolyhedron(Polyhedron **polyhedra, int numberOfPolyhedra, Color ka, Color kd, Color ks);
  void setNormalVector();
  static void setNormalVectors(Polyhedron **polyhedra, int numberOfPolyhedra);
  static void setPhongParams(Polyhedron**polyhedra, int numberOfPolyhedra, Color ka, Color kd, Color ks);
  static void applyPhong(Polyhedron **polyhedra, int numberOfPolyhedra, float Ia, float Il, Point_3D ff, Point_3D xx, int n);
  
  static float findMaxIntensity(Polyhedron **polyhedra, int numberOfPolyhedra);
  static void normalizeIntensities(Polyhedron **polyhedra, int numberOfPolyhedra);

  void clearContourPoints(int planeIndex);
  void clearContourPointsForEachPlane(int numberOfPlanes);
  void storeOriginalPointsToContourPointsForEachPlane();
  void setupContourPoints();
  int  _bresenham(Point pt1, Point pt2, int planeIndex); //for storing points, not drawing pixel
  int  _storeLinePoints(Point p1, Point p2, int planeIndex);
  void _storeContourPoint(Point p, int planeIndex);
  void printContourPoints();

  void rasterize(float r, float g, float b);

};

#endif
