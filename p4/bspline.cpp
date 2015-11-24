#include "bspline.h"

Bspline::Bspline(Graph *_graph, Point_2D *_ctrlPoints, int _numberOfCtrlPoints, int _k, float *_knotValues)
:Curve(_graph, _ctrlPoints, _numberOfCtrlPoints)
{
  k = _k;
  knotValues = new float[_numberOfCtrlPoints + _k];// n+1+k
  memcpy(knotValues, _knotValues, sizeof(float)*(_numberOfCtrlPoints+_k));
  setCurveColor(COLOR_CRIMSON);
  knotValuesCap = _numberOfCtrlPoints +_k;
}

void Bspline::printAttributes(){
  printf("\n==B-Spline curve==\n");
  printf("degree n: %d\n", numberOfCtrlPoints - 1);
  printf("K value : %d\n", k);

  for(std::list<Point_2D>::iterator it = ctrlPoints.begin(); it!=ctrlPoints.end(); ++it){
    printf("(%.2f, %.2f)\n", (*it).x, (*it).y) ;
  }
  
  printf("Knot Values: "); 
  for(int i = 0 ; i < numberOfCtrlPoints + k; i++){
    printf("%.2f ", knotValues[i]); 
  }
  printf("\n");
}

void Bspline::drawCurve(float res ){
  if(numberOfCtrlPoints > 1){
    float inc = 1.0/res;
    Point_2D p1, p2;
    for(float u = knotValues[k-1] ; u <= knotValues[numberOfCtrlPoints]-inc; u=u+inc){
      p1 = cc(u);
      p2 = cc(u+inc);
      DPRINT("Bspline: (%.2f, %.2f) @ u: %.2f\n", p1.x, p1.y, u);
      graph->drawLine(p1, p2, curveColor);
    }
  }
}

Point_2D Bspline::cc(float u){
    std::list<Point_2D>::iterator it = ctrlPointsNDC.begin();
    float leftRatio, rightRatio; 
    int I;

    //build a table 
    Point_2D lb[numberOfCtrlPoints][numberOfCtrlPoints], c_t;
    for(int c = 0; c < numberOfCtrlPoints; c++, it++){
      lb[0][c] = (*it);        
    } //init the 0th generation(j=0) control points

    //determine which segment I does the u lies in
    for(int i = 0 ; i < k+numberOfCtrlPoints; i++){
      if( u < knotValues[i]){
              I = i-1;
              break;
      }
    }
    //DPRINT("I: %d\n",I); 
    for(int j = 1 ; j <= k-1; j++){ //starting from the first generation
      for(int i = (I - (k-1)) ; i <= I-j; i++){
        rightRatio = (knotValues[i+k] - u) / (knotValues[i+k] - knotValues[i+j]) ;
        leftRatio  = (u - knotValues[i+j]) / (knotValues[i+k] - knotValues[i+j]) ;
        lb[j][i] = add( multByScalar( lb[j-1][i], rightRatio ) ,  multByScalar( lb[j-1][i+1], leftRatio ) );            
      }
    }
    
    c_t.x = lb[k-1][I-(k-1)].x * (graph->window_width - 1); 
    c_t.y = lb[k-1][I-(k-1)].y * (graph->window_height -1);
    return c_t;
}

const char* Bspline::className(){
  return "Bspline";
}

void Bspline::setParam(){
  float u;
  printf("For the selected B-Spline curve\nPlease enter its K value:");
  scanf("%d", &k);
  printf("K: %d recorded!\n", k);
  if(knotValues != 0)
    delete [] knotValues; 
  knotValues = new float[numberOfCtrlPoints + k];// n+1+k
  for(int i = 0 ; i < k + numberOfCtrlPoints; i++){
    printf("u%d: ", i); 
    scanf("%f", &u);
    knotValues[i] = u;
  }
  printf("Done Recording B-Spline param\n");
}

void Bspline::insertCtrlPoint(int xPixel, int yPixel){
  DPRINT("BSpline overriding\n");
  if( k+numberOfCtrlPoints >= knotValuesCap) // when space is not enough
    expandKnotValues();
  knotValues[k+numberOfCtrlPoints] = knotValues[k+numberOfCtrlPoints-1] + 1; 
  Curve::insertCtrlPoint(xPixel, yPixel);
}

void Bspline::addCtrlPoint(int xPixel, int yPixel){
  DPRINT("BSpline overriding\n");
  if( k+numberOfCtrlPoints >= knotValuesCap) // when space is not enough
    expandKnotValues();
  knotValues[k+numberOfCtrlPoints] = knotValues[k+numberOfCtrlPoints-1] + 1; 
  Curve::addCtrlPoint(xPixel, yPixel);
}

void Bspline::expandKnotValues() {
  float *tmp = new float[k+numberOfCtrlPoints];   
  for(int i= 0 ; i < k+numberOfCtrlPoints; i++){// copy everything to temp first
    tmp[i] = knotValues[i];
  }
  delete [] knotValues;  //deallocate memory for the old knotValues
  knotValues = new float[knotValuesCap*=2] ;// allocate new memory with double the size of the old
  
  for(int i = 0 ; i < k+numberOfCtrlPoints; i++){ //copy the content back
    knotValues[i] = tmp[i];  
  }
  //now knotValues has the same content as before, but the capacity is doubled 
  delete[] tmp;
}

