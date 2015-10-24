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
    printf("Vertex %d:  (%.2f, %.2f, %.2f)\n", i+1, p.x, p.y, p.z);
  }
  for(int i = 0 ; i < numberOfEdges; i++){
    e = listOfEdges[i];
    printf("Edge: %d %d\n", e.p1Index+1, e.p2Index+1);
  }
  printf("Centroid (%.2f, %.2f, %.2f)\n", centroid.x, centroid.y, centroid.z);
  
  printf("----------End of Polyhedron Attributes-----------------\n\n");

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
    
    if( !(isNDC(p1) && isNDC(p2)) ){
      printf("<>=======<> From Polyhedron::draw() : NDC is not valid\n");
      return;
    }
    //xy-plane 
    graphs[1]->drawLine( { (int)round(p1.x * scaleX), (int)round(p1.y * scaleY) }, { (int)round(p2.x*scaleX), (int)round(p2.y*scaleY) } ,r,g,b);
    //xz-plane
    graphs[2]->drawLine( { (int)round(p1.x * scaleX), (int)round(p1.z * scaleY) }, { (int)round(p2.x*scaleX), (int)round(p2.z*scaleY) }, r, g,b);
    //yz-plane
    graphs[3]->drawLine( { (int)round(p1.y * scaleX), (int)round(p1.z * scaleY) }, { (int)round(p2.y*scaleX), (int)round(p2.z*scaleY) }, r, g, b);
  }
}

void Polyhedron::savePolyhedraToFile(Polyhedron **polyhedra, Window *window, const char *filename){
  int numberOfPoints, numberOfEdges, numberOfPolyhedra = window->numberOfPolyhedra-1, p1Index1, p2Index2 ; float x, y, z;

  printf("Saving polyhedra to a file: %s\n...\nDone.\n", filename); fflush(stdout);//
  std::ofstream ofs(filename, std::ofstream::out);
  
  ofs << numberOfPolyhedra << "\n"; // number of polyhedra <numberOfPolyhedra>
  
  for(int i = 0 ; i < numberOfPolyhedra ; i++){
    ofs << "\n" ; 
    numberOfPoints = polyhedra[i]->numberOfPoints;
    ofs << numberOfPoints << "\n";  // <numberOfPoints>
    for(int j = 0 ; j < numberOfPoints; j++){
      x = polyhedra[i]->listOfPoints[j].x;
      y = polyhedra[i]->listOfPoints[j].y;
      z = polyhedra[i]->listOfPoints[j].z;
      ofs << x << " " << y << " " << z << "\n"; // <x> <y> <z>
    }

    numberOfEdges = polyhedra[i]->numberOfEdges;
    ofs << numberOfEdges << "\n";
    for(int j = 0 ; j < numberOfEdges; j++){
      p1Index1 = polyhedra[i]->listOfEdges[j].p1Index + 1; // name edge starting from 1
      p2Index2 = polyhedra[i]->listOfEdges[j].p2Index + 1;
      ofs << p1Index1 << " " << p2Index2 << "\n";
    }
  }

  ofs.close();
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
   
    if( !(isNDC(p1) && isNDC(p2)) ){
      printf("<>=======<> From Polyhedron::erase() : NDC is not valid\n");
      return;
    }
    //xy-plane 
    c = graphs[1]->background_color; 
    graphs[1]->drawLine( { (int)round(p1.x * scaleX), (int)round(p1.y * scaleY) }, { (int)round(p2.x*scaleX), (int)round(p2.y*scaleY) } ,c.r,c.g,c.b);
    
    //xz-plane
    c = graphs[2]->background_color;
    graphs[2]->drawLine( { (int)round(p1.x * scaleX), (int)round(p1.z * scaleY) }, { (int)round(p2.x*scaleX), (int)round(p2.z*scaleY) }, c.r,c.g,c.b);
    
    //yz-plane
    c = graphs[3]->background_color;
    graphs[3]->drawLine( { (int)round(p1.y * scaleX), (int)round(p1.z * scaleY) }, { (int)round(p2.y*scaleX), (int)round(p2.z*scaleY) }, c.r,c.g,c.b);

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
  DPRINT("INITIAL min max POINTS at findNDCParams %.2f, %.2f %.2f\n", xMax, yMax, zMax);
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

void Polyhedron::rotate(Point_3D p1, Point_3D p2, float angle){
  Point_3D dl = minus(p1, p2);  
  dl = unitVector(dl);

  float Tx = p1.x,
        Ty = p1.y,
        Tz = p1.z,
        dx = dl.x,
        dy = dl.y,
        dz = dl.z,
        l = sqrt( dy*dy + dz * dz),
        dy_l = dy/l,
        dz_l = dz/l,
        cosAlpha = cos(angle * PI / 180),
        sinAlpha = sin(angle * PI / 180),
        x, y , z, 
        newX,
        newY,
        newZ;
        
        //special case: rotates about x-axis,  dy = 0 && dz == 0 implies l = 0. If l = 0, dy/l = undefined
        //we can fix this general formula by setting dz_l = 1, dy_l = 0;
        if( l == 0){
          dz_l = 1;
          dy_l = 0;
        } // it means simply skips M21(rotate about x-axis), and go straight to M22(rotate-about y-axis), to get to into the b3(z_axis)

        for(int i = 0 ; i < numberOfPoints; i++){
          x = listOfPoints[i].x;
          y = listOfPoints[i].y;
          z = listOfPoints[i].z;
           
          //formula obtained by doing M = M5 * M42 * M41 * M3 * M22 *M21 * M1  where M41 undo M22, M42 undo M21, M5 undo M1
          //the matrix compuataion was done by hand with Matlab's help
          //in the code, one can just use the final formula
          //newX = M[at row 1];
          //newY = M[at row 2];
          //newZ = M[at row 3];
          newX = Tx + Ty*(dz_l*l*sinAlpha - dx*dy_l*l + cosAlpha*dx*dy_l*l) - Tz*(dx*dz_l*l + dy_l*l*sinAlpha - cosAlpha*dx*dz_l*l) - Tx*(dx*dx + cosAlpha*l*l) - y*(dz_l*l*sinAlpha - dx*dy_l*l + cosAlpha*dx*dy_l*l) + z*(dx*dz_l*l + dy_l*l*sinAlpha - cosAlpha*dx*dz_l*l) + x*(dx*dx + cosAlpha*l*l); 
          
          newY = Ty - Ty*(dz_l*(cosAlpha*dz_l + dx*dy_l*sinAlpha) + dy_l * dy_l * l * l - dx*dy_l*(dz_l*sinAlpha - cosAlpha*dx*dy_l)) + Tz*(- dy_l*dz_l*l*l + dy_l*(cosAlpha*dz_l + dx*dy_l*sinAlpha) + dx*dz_l*(dz_l*sinAlpha - cosAlpha*dx*dy_l)) + y*(dz_l*(cosAlpha*dz_l + dx*dy_l*sinAlpha) + dy_l *dy_l *l*l - dx*dy_l*(dz_l*sinAlpha - cosAlpha*dx*dy_l)) - z*(- dy_l*dz_l*l*l + dy_l*(cosAlpha*dz_l + dx*dy_l*sinAlpha) + dx*dz_l*(dz_l*sinAlpha - cosAlpha*dx*dy_l)) - Tx*(l*(dz_l*sinAlpha - cosAlpha*dx*dy_l) + dx*dy_l*l) + x*(l*(dz_l*sinAlpha - cosAlpha*dx*dy_l) + dx*dy_l*l); 
        
          newZ = Tz - Tz*(dy_l*(cosAlpha*dy_l - dx*dz_l*sinAlpha) + dz_l*dz_l*l*l + dx*dz_l*(dy_l*sinAlpha + cosAlpha*dx*dz_l)) - Ty*(dy_l*dz_l*l*l - dz_l*(cosAlpha*dy_l - dx*dz_l*sinAlpha) + dx*dy_l*(dy_l*sinAlpha + cosAlpha*dx*dz_l)) + z*(dy_l*(cosAlpha*dy_l - dx*dz_l*sinAlpha) + dz_l*dz_l*l*l + dx*dz_l*(dy_l*sinAlpha + cosAlpha*dx*dz_l)) + y*(dy_l*dz_l*l*l - dz_l*(cosAlpha*dy_l - dx*dz_l*sinAlpha) + dx*dy_l*(dy_l*sinAlpha + cosAlpha*dx*dz_l)) + Tx*(l*(dy_l*sinAlpha + cosAlpha*dx*dz_l) - dx*dz_l*l) - x*(l*(dy_l*sinAlpha + cosAlpha*dx*dz_l) - dx*dz_l*l);
          
          listOfPoints[i].x = newX;
          listOfPoints[i].y = newY;
          listOfPoints[i].z = newZ;
        }

        setCentroid();
}

Point_3D Polyhedron:: minus(Point_3D p1, Point_3D p2){
  return { p2.x - p1.x, p2.y-p1.y, p2.z - p1.z };
}

Point_3D Polyhedron:: unitVector(Point_3D p){
  float magnitude = sqrt(p.x * p.x + p.y * p.y + p.z * p.z);
  return { p.x / magnitude, p.y / magnitude, p.z / magnitude }; 
}

bool Polyhedron::isNDC(Point_3D p){
  return !( p.x < 0 || p.x > 1 || p.y <0 || p.y > 1 || p.z < 0 || p.y >1);
}

void Polyhedron::updateRotationAxis(Polyhedron **polyhedra, int numberOfPolyhedra, Point_3D *pairOfPoints){
  polyhedra[numberOfPolyhedra-1]->listOfPoints[0] = pairOfPoints[0];
  polyhedra[numberOfPolyhedra-1]->listOfPoints[1] = pairOfPoints[1];
}

