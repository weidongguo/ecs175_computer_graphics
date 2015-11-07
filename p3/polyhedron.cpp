#include "polyhedron.h"

Polyhedron::Polyhedron(Graph **_graphs, Point_3D *_listOfPoints, int _numberOfPoints, Edge *_listOfEdges, int _numberOfEdges, Surface *_listOfSurfaces, int _numberOfSurfaces){ 
  listOfSurfaces = new Surface[_numberOfSurfaces];
  listOfEdges = new Edge[_numberOfEdges];
  listOfPoints = new Point_3D[_numberOfPoints]; 
  listOfPointsNDC = new Point_3D[_numberOfPoints];
  for(int i = 0 ; i < _numberOfEdges; i++)
    listOfEdges[i] = _listOfEdges[i]; // make a local copy of edges
  
  for(int i = 0 ; i < _numberOfPoints; i++)
    listOfPoints[i] = _listOfPoints[i]; // make a local copy of points
  
  for(int i = 0 ; i < _numberOfSurfaces; i++) // make a local copy of surfaces
    listOfSurfaces[i] = _listOfSurfaces[i]; 

  numberOfSurfaces  = _numberOfSurfaces;
  numberOfEdges     = _numberOfEdges;
  numberOfPoints    = _numberOfPoints;
  graphs = _graphs;
  
  for(int i = 0 ; i< 3; i++) // 3 planes, xy, xz, yz
    listOfContourPoints[i] = new std::list<Point>[graphs[i]->window_height]; // indexing from 0 to window_height, watch out for overflow

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
  scaleX--;
  scaleY--;
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

Point_3D Polyhedron::obliqueProj(Point_3D dirVector, Point_3D p){ //oblique projection on  xy-plane
  float dx = dirVector.x,
      dy = dirVector.y,
      dz = dirVector.z,
      dx_dz = dx/dz,
      dy_dz = dy/dz,
      x = p.x,
      y = p.y,
      z = p.z,
      newX, newY, newZ;
   
   newX = x - dx_dz*z ;
   newY = y - dy_dz*z;
   newZ = 0.0;
   
   Point_3D newP = { newX, newY, newZ };

   return newP;
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
  scaleX--;
  scaleY--;
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
  Vector dl = minus(p2, p1);  
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

Vector   Polyhedron:: minus(Point_3D p2, Point_3D p1){
  return { p2.x - p1.x, p2.y-p1.y, p2.z - p1.z };
}
Vector   Polyhedron:: minus(Vector v1, Vector v2) {
  return { v1.x - v2.x, v1.y-v2.y, v1.z - v2.z };
}

Vector   Polyhedron:: unitVector(Point_3D p){
  float magnitude = sqrt(p.x * p.x + p.y * p.y + p.z * p.z);
  return { p.x / magnitude, p.y / magnitude, p.z / magnitude }; 
}

Vector   Polyhedron:: unitVector(Vector p){
  float magnitude = sqrt(p.x * p.x + p.y * p.y + p.z * p.z);
  return { p.x / magnitude, p.y / magnitude, p.z / magnitude }; 
}

float Polyhedron::dotProduct(Vector v1, Vector v2){
  return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

Vector Polyhedron::multByScalar(Vector v, float scalar){
  return { v.x * scalar, v.y * scalar, v.z * scalar};    
}

Vector Polyhedron::add(Vector v1, Vector v2){
  return { v1.x + v2.x, v1.y + v2.y, v1.z + v2.z};
}

float Polyhedron::magnitude(Vector v){
  return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

bool Polyhedron::isNDC(Point_3D p){
  return !( p.x < 0 || p.x > 1 || p.y <0 || p.y > 1 || p.z < 0 || p.y >1);
}

void Polyhedron::updateRotationAxis(Polyhedron **polyhedra, int numberOfPolyhedra, Point_3D *pairOfPoints){
  polyhedra[numberOfPolyhedra-1]->listOfPoints[0] = pairOfPoints[0];
  polyhedra[numberOfPolyhedra-1]->listOfPoints[1] = pairOfPoints[1];
}

void Polyhedron::printVector(const char *tag, Vector v){
  DPRINT("%s: (%.2f, %.2f, %.2f)\n", tag, v.x, v.y, v.z);
}

Vector Polyhedron::phong(Point_3D p, Vector ka, Vector kd, Vector ks, float Ia, float Il, Vector nn, Point_3D ff, int n, Point_3D xx ){
  Vector ll = unitVector( minus(xx,p) ); printVector("ll", ll);
  Vector vv = unitVector( minus(ff,p) ); printVector("vv", vv);
  Vector rr = minus( multByScalar(nn, 2*dotProduct(nn,ll)), ll ); printVector("rr", rr);
  double C =  magnitude( minus(xx, p) );
  return phong(p, ka, kd, ks, Ia, Il, nn, ff, n, C, ll, vv, rr);
}

Vector Polyhedron::phong(Point_3D p, Vector ka, Vector kd, Vector ks, float Ia, float Il, Vector nn, Point_3D ff, int n, double C, Vector ll, Vector rr, Vector vv){
  Vector Iamb = multByScalar(ka, Ia);
  float scalarForDiffSpec = Il / ( magnitude( minus(ff, p) ) + C ); 
  
  Vector diff = multByScalar( kd, dotProduct(ll,nn) );
  Vector spec = multByScalar( ks, pow( dotProduct(rr,vv), n ) );
  
  Vector Idiff_spec = multByScalar( add(diff, spec), scalarForDiffSpec);
  
  printVector( "Ip Phong", add(Iamb, Idiff_spec) );
  return add(Iamb, Idiff_spec);

}

/* Gourauld Shading */


bool compareXValue(Point p1, Point p2){
  return p1.x <= p2.x;
}

void Polyhedron:: _storeContourPoint(Point p, int planeIndex){
  if(! graphs[planeIndex]->outOfBound(p.x,p.y) )
    listOfContourPoints[planeIndex][p.y].push_back( p ); 
  else
    printf("out of bound\n");
}

//end points are NOT stored!!
int Polyhedron::_bresenham(Point pt1, Point pt2, int planeIndex){ //for storing points, not drawing pixel
  Point p1 = pt1;
  Point p2 = pt2;
  int x, y, x_end, y_end, p; 
  int dx = (p2.x - p1.x), dy = (p2.y - p1.y); //for determining sign of slope
  bool steep = false;
  float m = (float)dy/(float)dx ; //find the slope first
  bool positive_slope;
  if( m >= 0 )  // positive slope
    positive_slope = true;
  else
    positive_slope = false;
  
  if( fabs(m) <= 1 ){ //shallow
    steep = false; 
  }
  else{ //steep
   steep = true;
   swapXY(&p1);
   swapXY(&p2);
  }
  determineStartAndEndPoints(p1, p2, &x, &y, &x_end, &y_end);
  //DPRINT("x: %d,\ty: %d,\tx_end: %d,\ty_end:%d\n", x, y, x_end, y_end);
  dx = abs(x_end - x);
  dy = abs(y_end - y);
/*  //draw first point  
  if(steep)
    _storeContourPoint(y,x);//x and y was swapped before
  else 
    gstoreContourPoint(x,y); */  //no more first point
  p = 2 * dy - dx;

//  
  x_end--; // do  not store the last point
//
  for( ; x < x_end; ){
    x++;
    if( p >= 0){ // if d1 - d2  >= 0, means d2 is shorter, so advance y one level up
        positive_slope? y++:y--; 
        p = p + 2*dy - 2*dx;
    }
    else // if d1 - d2 < 0; means d1 is shorter, so no change of y;
      p = p + 2*dy;
    
    if(steep)
      _storeContourPoint( {y,x}, planeIndex);
    else 
      _storeContourPoint( {x,y}, planeIndex);
  }
  
  return 0;
}

//NOTE: End points are NOT stored, meaning the original two points
int Polyhedron::_storeLinePoints(Point p1, Point p2, int planeIndex){
  if(p1.x == p2.x){ //vertical line
    int y,y_end; 
    if(p1.y <= p2.y){
      y = p1.y;
      y_end = p2.y;
    }else{
      y = p2.y;
      y_end = p1.y;
    }
    
    y++; // do not store the first point
    y_end--; // do  not store the last point

    for(; y<=y_end; y++)
      _storeContourPoint( {p1.x, y}, planeIndex);
    return 0;
  }
  else if(p1.y == p2.y){ // horizontal line
    int x, x_end;
    if(p1.x <= p2.x){
      x = p1.x;
      x_end =  p2.x;
    }
    else{
      x = p2.x;
      x_end = p1.x; 
    }

    x++; // do not store the first point
    x_end--; // do  not store the last point

    for(; x <= x_end; x++)
      _storeContourPoint( {x, p1.y}, planeIndex);
    return 0;
  }

  // all other cases are taken care below
  _bresenham(p1, p2, planeIndex);
  
  return 0;
}



void Polyhedron::clearContourPoints(int planeIndex){
  for(int i = 0; i< graphs[planeIndex]->window_height; i++){
    listOfContourPoints[planeIndex][i].clear();
  }
}

void Polyhedron::clearContourPointsForEachPlane(int numberOfPlanes){
  for(int i = 0; i < numberOfPlanes; i++)
    clearContourPoints(i);
}

void Polyhedron::storeOriginalPointsToContourPointsForEachPlane(){
  Point_3D point3d; Point point2d;
  int scale = MIN( graphs[1]->window_width, graphs[1]->window_height);
  scale--; //from 0 to scale - 1
  
  //for xy-plane
  for(int i = 0 ; i < numberOfPoints; i++){
    point3d = listOfPointsNDC[i] ;
    point2d.x  =  (int)round( point3d.x * scale); 
    point2d.y  =  (int)round( point3d.y * scale);
    _storeContourPoint(point2d, 0); 
  }
  //for xz-plane
   for(int i = 0 ; i < numberOfPoints; i++){
    point3d = listOfPointsNDC[i] ;
    point2d.x  =  (int)round( point3d.x * scale); 
    point2d.y  =  (int)round( point3d.z * scale);
    _storeContourPoint(point2d, 1); 
  }
  //for yz-plane
   for(int i = 0 ; i < numberOfPoints; i++){
    point3d = listOfPointsNDC[i] ;
    point2d.x  =  (int)round( point3d.y * scale); 
    point2d.y  =  (int)round( point3d.z * scale);
    _storeContourPoint(point2d, 2); 
  }

}

void Polyhedron::setupContourPoints(){
  int p1Index, p2Index; Point_3D p1, p2; int scaleX = graphs[1]->window_width, scaleY = graphs[1]->window_height;
  scaleX = scaleY = MIN(scaleX,scaleY);
  scaleX--;  scaleY--;
  
  clearContourPointsForEachPlane(3);//reset all listOfCountourPoints
   
  storeOriginalPointsToContourPointsForEachPlane();//do not want duplicates of the original points, so store it once here and ignore all other instances when encountering the same points

  for(int i = 0 ; i < numberOfEdges; i++){
    p1Index = listOfEdges[i].p1Index;
    p2Index = listOfEdges[i].p2Index;
    p1 = listOfPointsNDC[p1Index];
    p2 = listOfPointsNDC[p2Index];
    
    if( !(isNDC(p1) && isNDC(p2)) ){
      printf("<>=======<> From Polyhedron::draw() : NDC is not valid\n");
      return;
    }
    /* */

    //xy-plane 
    _storeLinePoints( { (int)round(p1.x * scaleX), (int)round(p1.y * scaleY) }, { (int)round(p2.x*scaleX), (int)round(p2.y*scaleY) } ,   0);
    //xz-plane
    _storeLinePoints( { (int)round(p1.x * scaleX), (int)round(p1.z * scaleY) }, { (int)round(p2.x*scaleX), (int)round(p2.z*scaleY) },    1);
    //yz-plane
    _storeLinePoints( { (int)round(p1.y * scaleX), (int)round(p1.z * scaleY) }, { (int)round(p2.y*scaleX), (int)round(p2.z*scaleY) },    2);
  }
  
  //for each plane
  for(int planeIndex = 0; planeIndex < 3; planeIndex++){ 
    ///sort each scanline points by it's x-value 
    for(int i = 0 ; i < graphs[planeIndex]->window_height; i++){
      if(!listOfContourPoints[planeIndex][i].empty())
        listOfContourPoints[planeIndex][i].sort(compareXValue);
    }
  }

}


void Polyhedron::printContourPoints(){
 for(int planeIndex = 0 ; planeIndex < 3; planeIndex++){
  DPRINT("\n=================START OF CONTOUR POINTS for %d plane=======================\n", planeIndex);
  for(int i = 0; i< graphs[planeIndex]->window_height; i++){
    for(std::list<Point>::iterator it = listOfContourPoints[planeIndex][i].begin(); it != listOfContourPoints[planeIndex][i].end(); it++)
      DPRINT("(%d, %d)  ", (*it).x, (*it).y);
    if(!listOfContourPoints[planeIndex][i].empty()) 
      DPRINT("\n"); 
  }
  DPRINT("=================END OF CONTOUR POINTS=========================\n\n");
 }
}
/*
void Polyhedron::rasterize(float r, float g, float b){
  storeContourPoints();// set up all the points for the contour first, so they can be used for rasterizing
  for(int i = 0; i < graph->window_height; i++){ //for each scanline
    if(listOfContourPoints[i].size() > 1){ // if more than 1 point on a scanline
      for(std::list<Point>::iterator it = listOfContourPoints[i].begin(); it != listOfContourPoints[i].end();){
        Point p1 = *it;
        std::advance(it, 1);
        if(it == listOfContourPoints[i].end())
          break;
        Point p2 = *it;
        //DPRINT("(%d, %d) ; (%d, %d) \n", p1.x, p1.y, p2.x, p2.y);
        if( abs(p2.x - p1.x) >= 1 ){
          graph->drawLine(p1,p2, r, g, b); 
         
        }
      }
    }
  }
}
*/
