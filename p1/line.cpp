#include "line.h"

Line::Line(Point p1, Point p2, Graph *g){
  pairOfPointsOriginal[0]=pairOfPointsAfterClipping[0]=p1;
  pairOfPointsOriginal[1]=pairOfPointsAfterClipping[1]=p2;
  pairOfPoints = pairOfPointsOriginal;
  graph = g;
  color.r = 0;//default color is BLACK
  color.g = 0;
  color.b = 0;
}

void Line::setColor(float r, float g, float b){
  color.r = r;
  color.g = g;
  color.b = b;
}

void Line::draw(){
  if( pairOfPoints == 0)
    DPRINT("<>===<> POINTS ARE CLIPPED\n");
  else
    graph->drawLine(pairOfPoints[0], pairOfPoints[1], color.r, color.g, color.b);
}

void Line::clip(int xMin, int xMax, int yMin, int yMax){
  int errorCode =  cohenSutherland(xMin, xMax, yMin, yMax);
  if( errorCode == 0 );//no change to original points
  if( errorCode == -1){ //rejected the lines
    //erase it by fillingthe line with background color
    Color bgColor = graph->background_color;
    graph->drawLine(pairOfPointsOriginal[0], pairOfPointsOriginal[1], bgColor.r, bgColor.g, bgColor.b);
  }
  

  //draw the clipping region
  Point clippingRegion[] = { {xMin, yMin}, {xMin, yMax}, {xMax, yMax}, {xMax, yMin} };
  graph->drawPolygon(clippingRegion, 4, 0.1, 1, 0.1); 
}

//an efficient method to clip a line
int Line::cohenSutherland(int xMin, int xMax, int yMin, int yMax){
  uint8 p1_region_code = encode(pairOfPoints[0], xMin, xMax, yMin, yMax);
  uint8 p2_region_code = encode(pairOfPoints[1], xMin, xMax, yMin, yMax);
  if( accept(p1_region_code, p2_region_code) )
    return 0;
  else if( reject(p1_region_code, p2_region_code) ){
    pairOfPoints = 0; //assign a null pointer
    return -1;
  }
  else { //check for intersection points

  }
}

/*========================================================================*/
/* @fn       : uint8 encode(Point p, int xMin, int xMax, int yMin, int yMax)

 * @brief    : ABRL e.g. code 0000 for being in the clipping region, 
 *              1010 for above and to the right of the clipping region
 *              So, this function is used to encode a point with respect
 *              to the clipping region defined by xMin, xMax, yMin, yMax
 * @param[in]: Point p - the point to be encoded
 *              int xMin, xMax, yMin, yMax - define the clipping region
 *                                           
 * @return   : none 
 */

#define SET_LEFT_CODE   0x01 //0001
#define SET_RIGHT_CODE  0x02 //0010
#define SET_BOTTOM_CODE 0x04 //0100
#define SET_ABOVE_CODE  0x08 //1000

uint8 encode(Point p, int xMin, int xMax, int yMin, int yMax){
  uint8 region_code = 0x00;
  if(p.x < xMin)
    region_code |= SET_LEFT_CODE;
  if(p.x > xMax)
    region_code |= SET_RIGHT_CODE;
  if(p.y < yMin)
    region_code |= SET_BOTTOM_CODE;
  if(p.y > yMax)
    region_code |= SET_ABOVE_CODE;
  return region_code;
}


bool inside(uint8 region_code){ //inside the clipping region
  return !region_code;
}


/* ====================================================================  */
/* cases for accepting or rejecting for sure before going to look
 * for intersection points 
 */
bool reject(uint8 region_code1, uint8 region_code2){ 
  return (bool)(region_code1 & region_code2) ;
}// reject for sure since both points are outside of the clipping region


bool accept(uint8 region_code1, uint8 region_code2){ //accept for sure
  return !(region_code1 | region_code2);
} // i.e. 0000 | 0000 = 0000 -> ! 0 = true
  // accepting for sure cause both points are inside the clipping region
/*=======================================================================*/
