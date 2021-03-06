#ifndef CURVE_H
#define CURVE_H

#include "common_type.h"
#include "graph.h"
#include <list>
#include <fstream>

class Curve {
  protected: 
  Graph *graph;
  std::list<Point_2D> ctrlPoints;
  std::list<Point_2D> ctrlPointsNDC;
  int numberOfCtrlPoints;
  int selectedCtrlPoint;//index in ctrlPoints or ctrlPointsNDC
  Color curveColor;
  ParamNDC paramNDC;

  public:
  Curve(Graph *_graph, Point_2D *_ctrlPoints, int _numberOfCtrlPoints);
  void print();
  virtual void printAttributes() = 0;
  void drawControlPolygon(Color c, bool isSelected);
  void setCurveColor(Color c);
  virtual void drawCurve(float res) = 0; // ** must ** be overriden by subclass (pure virtual)
  virtual const char* className() = 0;

  static void normalizeCtrlPoints(std::list<Curve*> *curves);
  static void findNDCParam(std::list<Curve*> *curves, float*xMin, float*yMin, float *delta);
  static Point_2D NDCToOriginal(Point_2D pNDC, ParamNDC paramNDC);
  
  int findCtrlPoint(int xPixel, int yPixel);//return an index of the ctrl point on a curve
  bool selectCtrlPoint(int xPixel, int yPixel);//return true if succesfully selected, else point not found
  bool deleteSelectedCtrlPoint();
  void modifySelectedCtrlPoint(int xPixel,int yPixel); //new control point will be of values relative to x and y
  virtual void addCtrlPoint(int xPixel, int yPixel); // can be overriden by subclass
  virtual void insertCtrlPoint(int xPixel, int yPixel); // can be overriden by subclass
  static void saveToFile(std::list<Curve *> *curves, const char *filename);

};
#endif

