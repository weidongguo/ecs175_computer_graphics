#include "polyhedron.h"

Polyhedron::Polyhedron(Graph **_graphs, Point_3D *_listOfPoints, int _numberOfPoints, Edge *_listOfEdges, int _numberOfEdges){ 
  listOfEdges = new Edge[_numberOfEdges];
  listOfPoints = new Point_3D[_numberOfPoints]; 
  listOfPointsNDC = new Point_3D[_numberOfPoints];
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
  printf("------------Polyhedron Attributes----------------------\n");
  for(int i = 0 ; i < numberOfPoints; i++){
    p = listOfPoints[i]; 
    printf("(%.2f, %.2f, %.2f)\n", p.x, p.y, p.z);
  }
  for(int i = 0 ; i < numberOfEdges; i++){
    e = listOfEdges[i];
    printf("Edge %d %d\n", e.p1Index+1, e.p2Index+1);
  }
  printf("Centroid (%.2f, %.2f, %.2f)\n", centroid.x, centroid.y, centroid.z);
  
  printf("----------End of Polyhedron Attributes-----------------\n");

}
void Polyhedron::draw(){
  draw(0.0, 0.0, 1.0);
}
void Polyhedron::draw(Color c){
  draw(c.r, c.g, c.b);
}

void Polyhedron::draw(float r, float g, float b){
  int p1Index, p2Index; Point_3D p1, p2; int scaleX = graphs[1]->window_width, scaleY = graphs[1]->window_height;
  scaleX = scaleY = MIN(scaleX,scaleY);
  //scaleX = scaleY = 10; 
  for(int i = 0 ; i < numberOfEdges; i++){
    p1Index = listOfEdges[i].p1Index;
    p2Index = listOfEdges[i].p2Index;
    p1 = listOfPointsNDC[p1Index];
    p2 = listOfPointsNDC[p2Index];
    //xy-plane 
    graphs[1]->drawLine( {p1.x * scaleX, p1.y * scaleY}, { p2.x*scaleX, p2.y*scaleY} ,r,g,b);
    //xz-plane
    graphs[2]->drawLine( {p1.x * scaleX, p1.z * scaleY}, { p2.x*scaleX, p2.z*scaleY}, r, g,b);
    //yz-plane
    graphs[3]->drawLine( {p1.y * scaleX, p1.z * scaleY}, { p2.y*scaleX, p2.z *scaleY}, r, g, b);
  }
}

void Polyhedron::erase(){
  int p1Index, p2Index; Point_3D p1, p2; int scaleX = graphs[1]->window_width, scaleY = graphs[1]->window_height;
  scaleX = scaleY = MIN(scaleX,scaleY);
  Color c; 
  for(int i = 0 ; i < numberOfEdges; i++){
    p1Index = listOfEdges[i].p1Index;
    p2Index = listOfEdges[i].p2Index;
    p1 = listOfPointsNDC[p1Index];
    p2 = listOfPointsNDC[p2Index];
    //xy-plane 
    c = graphs[1]->background_color; 
    graphs[1]->drawLine( {p1.x * scaleX, p1.y * scaleY}, { p2.x*scaleX, p2.y*scaleY} ,c.r, c.g, c.b);
    //xz-plane
    c = graphs[2]->background_color;
    graphs[2]->drawLine( {p1.x * scaleX, p1.z * scaleY}, { p2.x*scaleX, p2.z*scaleY}, c.r, c.g, c.b);
    //yz-plane
    c = graphs[3]->background_color;
    graphs[3]->drawLine( {p1.y * scaleX, p1.z * scaleY}, { p2.y*scaleX, p2.z *scaleY}, c.r, c.g, c.b);
  }
}


void Polyhedron::setNDC(float delta, float xMin, float yMin, float zMin){
  if(listOfPointsNDC != 0 && listOfPoints!=0){
    //set normalized device coordinates
    for(int i = 0 ; i < numberOfPoints; i++){
      listOfPointsNDC[i].x = (listOfPoints[i].x - xMin)/delta;
      listOfPointsNDC[i].y = (listOfPoints[i].y - yMin)/delta;
      listOfPointsNDC[i].z = (listOfPoints[i].z - zMin)/delta;
      DPRINT("NDC: (%.2f, %.2f, %.2f)\n", listOfPointsNDC[i].x, listOfPointsNDC[i].y, listOfPointsNDC[i].z);
    }

  }
  else{
    DPRINT("ERROR: setNDC() encounters invalid class data member\n");
  }
}

/*==================================================================*/
/* @fn          :     void findNDCParams(Polyhedron **polyhedra, int numberOfPolyhedra, float *delta, float *_xMin float *_yMin, float *_zMin);      
 * @brief       :     find the parameters required for calculating NDC
 * @params[in]  :     Polyhedron ** polyhedra - the list of polyhedra
 * @params[in]  :     int numberOfPolyhedra   - the number of polyhedra
 * @params[out] :     float *delta            - master max of ( master delta X, master delta Y, and  master delta Y)
 * @params[out] :     float *_xMin             - master x min
 * @params[out] :     float *_yMin             - master y min
 * @params[out] :     float *_zMin             - master z min
 */

void Polyhedron::findNDCParams(Polyhedron **polyhedra, int numberOfPolyhedra, float *delta, float *_xMin, float *_yMin, float *_zMin){
  float xMax, xMin, yMax, yMin, zMax, zMin;
  Point_3D p, *listOfPoints; 
  int numberOfPoints;
  //preset min and max
  listOfPoints = polyhedra[0]->listOfPoints; 
  xMax = xMin = listOfPoints[0].x;
  yMax = yMin = listOfPoints[0].y;
  zMax = zMin = listOfPoints[0].z;

 //find master  max and min
  for(int  j = 0 ; j < numberOfPolyhedra; j++){
    listOfPoints = polyhedra[j]->listOfPoints; 
    numberOfPoints = polyhedra[j]->numberOfPoints;
    if(listOfPoints!=0){
      for(int i = 0 ; i < numberOfPoints; i++){
        p = listOfPoints[i];
        if( p.x < xMin )
          xMin = p.x;
        else if( p.x > xMax)
          xMax = p.x;
        
        if( p.y < yMin)
          yMin = p.y;
        else if( p.y > yMax)
          yMax = p.y;
        
        if( p.z < zMin)
          zMin = p.z;
        else if( p.z > zMax)
          zMax = p.z;
      }
    }
    else
      printf("ERROR: null pointers encountered while executing findNDCParams()\n");
  }
  *delta = MAX( MAX(xMax-xMin, yMax-yMin), zMax-zMin);
  *_xMin = xMin;
  *_yMin = yMin;
  *_zMin = zMin;
  DPRINT("DELTA : %.2f\n xMin : %.2f\n yMin : %.2f\n zMin : %.2f\n", *delta, xMin, yMin, zMin);

}

void Polyhedron::scale(float alpha){
  scale(alpha, alpha, alpha); //scale by the same factor for all 3 dimensions 
}

void Polyhedron::scale(float alpha, float beta, float gamma){
  float Cx = centroid.x,
        Cy = centroid.y,
        Cz = centroid.z,
        newX, newY, newZ, x, y, z;
  for(int i = 0 ; i < numberOfPoints; i ++){
    x = listOfPoints[i].x;
    y = listOfPoints[i].y;
    z = listOfPoints[i].z;

    newX = Cx - Cx*alpha + alpha*x;
    newY = Cy - Cy*beta + beta*y; 
    newZ = Cz - Cz*gamma + gamma*z;

    listOfPoints[i].x = newX;
    listOfPoints[i].y = newY;
    listOfPoints[i].z = newZ;
  }
  
  setCentroid(); //  after transformation, there will be a new centroid for later use
}
void Polyhedron::translate(float x_offset, float y_offset, float z_offset){
  float  new_x, new_y, new_z;
  for(int i = 0 ; i< numberOfPoints; i++){
    new_x = listOfPoints[i].x + x_offset;
    new_y = listOfPoints[i].y + y_offset;
    new_z = listOfPoints[i].z + z_offset;
    listOfPoints[i].x = new_x; 
    listOfPoints[i].y = new_y;
    listOfPoints[i].z = new_z;
  }

  setCentroid(); // set the enw centroid after translating
}

