#include "curve.h"
#include <cstring>
#include "bspline.h"
Curve::Curve(Graph *_graph, Point_2D *_ctrlPoints, int _numberOfCtrlPoints){
  graph = _graph; //for making pixels to screen (line drawing, etc)
  for(int i = 0 ; i < _numberOfCtrlPoints; i++){
    ctrlPoints.push_back(_ctrlPoints[i]);
    ctrlPointsNDC.push_back(_ctrlPoints[i]);//init first, later normalize it
  }
  
  numberOfCtrlPoints = _numberOfCtrlPoints;
  selectedCtrlPoint = 0 ; // init to select the first control point
}

bool Curve::deleteSelectedCtrlPoint(){ //if there are no more control points, return true;
  if(!ctrlPoints.empty()){
    std::list<Point_2D>::iterator it = ctrlPoints.begin();
    std::advance(it, selectedCtrlPoint); //go to that postion;
    ctrlPoints.erase(it);//delete it
    numberOfCtrlPoints--;
    selectedCtrlPoint = 0;//reset selectedCtrlPoint
  }
  return ctrlPoints.empty();
}

void Curve::print(){
  printf("Curve::Print()\n");
}

void Curve::setCurveColor(Color c){
  curveColor = c;
}

void Curve::drawControlPolygon(Color c, bool isSelected){
  Point_2D p1, p2;
  for(std::list<Point_2D>::iterator it = ctrlPointsNDC.begin(); it != ctrlPointsNDC.end(); ){
    //first control point 
    p1 = (*it);
    p1.x = p1.x * (graph->window_width - 1);
    p1.y = p1.y * (graph->window_height -1);
    if( std::distance(ctrlPointsNDC.begin(), it) == selectedCtrlPoint && isSelected)
      graph->drawBigDot(p1, {0,1,0}, 15);
    else
      graph->drawBigDot(p1, {0.5,0.5,0.5}, 15);

    //possible next control point 
    it++;
    if(it == ctrlPointsNDC.end())
            break;
    p2 = (*it);          
    p2.x = p2.x * (graph->window_width - 1);
    p2.y = p2.y * (graph->window_height -1);
    
    graph->drawBigDot(p2, {0.5,0.5,0.5}, 15);

    graph->drawLine(p1, p2, c);
    
  }
}



void Curve::normalizeCtrlPoints(std::list<Curve*> *curves){ //static method
  float xMin, yMin, delta;
  std::list<Point_2D>::iterator itp, itpNDC;
  Point_2D p;

  findNDCParam(curves, &xMin, &yMin, &delta);
  //DPRINT("xMin: %.2f, yMin: %.2f, delta: %.2f\n", xMin, yMin, delta);
  for(std::list<Curve*>::iterator itc = curves->begin(); itc!=curves->end(); itc++){
    (*itc)->ctrlPointsNDC.clear(); //clear the old NDC points
    for(itp = (*itc)->ctrlPoints.begin() ; itp != (*itc)->ctrlPoints.end(); itp++){
      p.x = ( (*itp).x - xMin ) / delta; //normalize the x value
      p.y = ( (*itp).y - yMin ) / delta; //normalize the y value
      (*itc)->ctrlPointsNDC.push_back(p);
      
      (*itc)->paramNDC.xMin = xMin;
      (*itc)->paramNDC.yMin = yMin;
      (*itc)->paramNDC.delta = delta;
    }
  }
}

void Curve::findNDCParam(std::list<Curve*> *curves, float*_xMin, float*_yMin, float *delta){ //static method
  bool isInit = true; 
  std::list<Point_2D> ctrlPoints;
  std::list<Point_2D>::iterator itp;
  float xMin, yMin, xMax, yMax; 

  for(std::list<Curve*>::iterator itc = curves->begin(); itc!=curves->end(); itc++){
    ctrlPoints = (*itc)->ctrlPoints;
    for(itp = ctrlPoints.begin() ; itp != ctrlPoints.end(); itp++){
      if(isInit){
        xMin = xMax = (*itp).x;
        yMin = yMax = (*itp).y; 
        isInit = false;        
      }
      else{
        if( (*itp).x < xMin )
                xMin = (*itp).x;
        else if( (*itp).x > xMax )
                xMax = (*itp).x;
        if( (*itp).y < yMin)
                yMin = (*itp).y;
        else if( (*itp).y > yMax)
                yMax = (*itp).y;
      }
    }
  }
  *_xMin = xMin;
  *_yMin = yMin;
  *delta = std::max(xMax-xMin, yMax-yMin);
}


/* @fn    :   findCtrlPoint(int xPixel, int yPixel);
 * @brief :   find if the pixelPoint is one of the control points on curve
 * @return:   positive number - control point is found, return its index in the list<Point_2D>
 *            -1              - if not found
 */
int Curve::findCtrlPoint(int xPixel, int yPixel){
 Point pixelPoint;
 int index;
 for(std::list<Point_2D>::iterator itp = ctrlPointsNDC.begin(); itp != ctrlPointsNDC.end(); itp++){
          pixelPoint = graph->NDCToPixel(*itp); 
          if( graph->isWithinDot(xPixel,yPixel,pixelPoint, 15)) {
            index = std::distance(ctrlPointsNDC.begin(), itp);
            DPRINT("Hit control point %d\n", index);
            return index; 
          }
 }       
 return -1;
}
bool Curve::selectCtrlPoint(int xPixel, int yPixel){//return true if succesfully selected, else point not found
  int index = findCtrlPoint(xPixel,yPixel); 
  if( index != -1){
    selectedCtrlPoint = index;             
    return true;
  }
  return false;
}
void Curve::modifySelectedCtrlPoint(int xPixel, int yPixel){
  Point_2D pNDC = graph->pixelToNDC({xPixel,yPixel});
  Point_2D p = NDCToOriginal(pNDC, paramNDC);    
  DPRINT("Original (%.2f, %.2f)\n",p.x, p.y); 
  //modify point right here:
  std::list<Point_2D>::iterator it = ctrlPoints.begin();
  std::advance(it, selectedCtrlPoint);
  (*it) = p;
}

Point_2D Curve::NDCToOriginal(Point_2D pNDC, ParamNDC paramNDC){
  return { paramNDC.delta *  pNDC.x + paramNDC.xMin, paramNDC.delta * pNDC.y + paramNDC.yMin }; 
}

void Curve::insertCtrlPoint(int xPixel, int yPixel){
  Point_2D pNDC = graph->pixelToNDC({xPixel,yPixel});
  Point_2D p = NDCToOriginal(pNDC, paramNDC);    
  std::list<Point_2D>::iterator it = ctrlPoints.begin();
  std::advance(it, selectedCtrlPoint);
  ctrlPoints.insert(it, p);
  numberOfCtrlPoints++;
}

void Curve::addCtrlPoint(int xPixel, int yPixel){
  Point_2D pNDC = graph->pixelToNDC({xPixel,yPixel});
  Point_2D p = NDCToOriginal(pNDC, paramNDC);    
  std::list<Point_2D>::iterator it = ctrlPoints.begin();
  std::advance(it, selectedCtrlPoint+1);
  ctrlPoints.insert(it, p);
  numberOfCtrlPoints++;
  selectedCtrlPoint++;// to make the newly added ctrl point the selected control point
}


void Curve::saveToFile(std::list<Curve *> *curves, const char*filename){
  printf("Saving polyhedra to a file: %s\n...\nDone.\n", filename); fflush(stdout);//
  std::ofstream ofs(filename, std::ofstream::out);
  ofs << curves->size() << "\n"; // number of curves
  
  for(std::list<Curve *>::iterator itc = curves->begin(); itc!=curves->end(); itc++){ // for each curve
    ofs << "\n";  
    if( strcmp( (*itc)->className(), "Bezier" ) == 0 ){
      ofs << "bz\n";
      ofs << (*itc)->numberOfCtrlPoints << "\n"; 
      for(std::list<Point_2D>::iterator itp = (*itc)->ctrlPoints.begin(); itp!=(*itc)->ctrlPoints.end(); itp++){ // for each control point
        ofs << (*itp).x <<  " " << (*itp).y << "\n";     
      }
    }
    else{
      ofs << "bs\n";
      ofs << (*itc)->numberOfCtrlPoints << "\n"; 
      ofs << ((Bspline*)(*itc))-> k <<"\n";
      ofs << "T\n"; 

      for(std::list<Point_2D>::iterator itp = (*itc)->ctrlPoints.begin(); itp!=(*itc)->ctrlPoints.end(); itp++){ // for each control point
        ofs << (*itp).x <<  " " << (*itp).y << "\n";     
      }
      for(int i = 0 ; i < (*itc)->numberOfCtrlPoints + ((Bspline*)(*itc))->k - 1; i++){
        ofs << ((Bspline*)(*itc))->knotValues[i] << " ";
      }
      ofs << ((Bspline*)(*itc))->knotValues[(*itc)->numberOfCtrlPoints + ((Bspline*)(*itc))->k - 1] ;
      ofs << "\n";
    }
  }
}

