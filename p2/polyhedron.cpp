#include "polyhedron.h"

Polyhedron::Polyhedron(Graph **_graphs, Point_3D *_listOfPoints, int _numberOfPoints, Edge *_listOfEdges, int _numberOfEdges){ 
  listOfEdges = new Edge[_numberOfEdges];
  listOfPoints = new Point_3D[_numberOfPoints]; 
  for(int i = 0 ; i < _numberOfEdges; i++)
    listOfEdges[i] = _listOfEdges[i]; // make a local copy of edges
  
  for(int i = 0 ; i < _numberOfPoints; i++)
    listOfPoints[i] = _listOfPoints[i]; // make a local copy of points
 
  numberOfEdges = _numberOfEdges;
  numberOfPoints = _numberOfPoints;
  graphs = _graphs;

  setCentroid();
}

void Polyhedron::setCentroid(){
  float xSum = 0 , ySum = 0 , zSum = 0; Point_3D point;
  for(int i = 0 ; i< numberOfPoints; i++){
    point = listOfPoints[i];
    xSum += point.x;
    ySum += point.y;
    zSum += point.z;
  }
  centroid.x = xSum / numberOfPoints;
  centroid.y = ySum / numberOfPoints;
  centroid.z = zSum / numberOfPoints; 
}

void Polyhedron::printAttributes(){
  Point_3D p; Edge e;
  printf("---------Polyhedron Attributes----------------------\n");
  for(int i = 0 ; i < numberOfPoints; i++){
    p = listOfPoints[i]; 
    printf("(%.2f, %.2f, %.2f)\n", p.x, p.y, p.z);
  }
  for(int i = 0 ; i < numberOfEdges; i++){
    e = listOfEdges[i];
    printf("Edge %d %d\n", e.p1Index+1, e.p2Index+1);
  }
  printf("Centroid (%.2f, %.2f, %.2f)\n", centroid.x, centroid.y, centroid.z);

}
