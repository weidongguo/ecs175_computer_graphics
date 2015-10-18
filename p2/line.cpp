#include "line.h"

#define SET_LEFT_CODE   0x01 //0001
#define SET_RIGHT_CODE  0x02 //0010
#define SET_BOTTOM_CODE 0x04 //0100
#define SET_ABOVE_CODE  0x08 //1000


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
  draw(BRESENHAM);
}

void Line::draw(int method){
  if( pairOfPoints == 0)
    DPRINT("<>===<> POINTS ARE CLIPPED\n");
  else
    graph->drawLine(pairOfPoints[0], pairOfPoints[1], color.r, color.g, color.b, method);
}

void Line::clip(ClipRegion cr){
  clip(cr.xMin, cr.xMax, cr.yMin, cr.yMax);
}
void Line::clip(int xMin, int xMax, int yMin, int yMax){
     //erase original lines by filling the line with background color
  Color bgColor = graph->background_color;
  graph->drawLine(pairOfPointsOriginal[0], pairOfPointsOriginal[1], bgColor.r, bgColor.g, bgColor.b);
 
  int errorCode =  cohenSutherland(xMin, xMax, yMin, yMax);
  if( errorCode == 0 ){//accepted the original line or a new line is generated
    graph->drawLine(pairOfPoints[0], pairOfPoints[1], color.r, color.g, color.b);
  }//else, the entire line is rejected (since it's off the clipping region)

  //draw the clipping region
/*  Point clippingRegion[] = { {xMin, yMin}, {xMin, yMax}, {xMax, yMax}, {xMax, yMin} };
  graph->drawPolygon(clippingRegion, 4, 0.1 , 1, 0.5 ); */
}

//an efficient method to clip a line
int Line::cohenSutherland(int xMin, int xMax, int yMin, int yMax){
  Point p1, p2; float m; 
  uint8  p1_region_code, p2_region_code;
  p1 = pairOfPointsOriginal[0];
  p2 = pairOfPointsOriginal[1];

  while(true){  //will end the loop using "break"
    p1_region_code = encode(p1, xMin, xMax, yMin, yMax);
    p2_region_code = encode(p2, xMin, xMax, yMin, yMax);
    if( accept(p1_region_code, p2_region_code) ){ //accept the line
      pairOfPointsAfterClipping[0] = p1;
      pairOfPointsAfterClipping[1] = p2;
      pairOfPoints = pairOfPointsAfterClipping;
      return 0;
    }
    else if( reject(p1_region_code, p2_region_code) ){ //reject the line
      pairOfPoints = 0; //assign a null pointer
      return -1;
    }
    else { //check for intersection points
       if( inside(p1_region_code) ){ // if this point is inside the region, just find the intersection point of the other point. else deal with this poiint first and will become a point inside the region. At this time, it's the 2nd point's turn to be changed.
        std::swap(p1_region_code, p2_region_code);          
        swapPoints(&p1, &p2);
         
       }
       if( p2.x - p1.x != 0) //not a vertical line
        m = (float)(p2.y-p1.y)/(p2.x-p1.x);
       if( p1_region_code & SET_LEFT_CODE){
        p1.y = m*(xMin - p1.x) + p1.y;//new y value at the xMin edge
        p1.x = xMin;
       }
       else if( p1_region_code & SET_RIGHT_CODE){
        p1.y = m*(xMax - p1.x) + p1.y;//new y-value at the xMax edge
        p1.x = xMax;
       }
       else if( p1_region_code & SET_BOTTOM_CODE){ 
        // y - y0 = m*(x - x0) -> (y - y0)/m = x - x0 -> x = (y-y0)/m + x0
        if(p1.x != p2.x)//if equal, no need to change p1.x cuz straight line
          p1.x = (yMin - p1.y)/m + p1.x;
        p1.y = yMin;
       }
       else if( p1_region_code & SET_ABOVE_CODE){
        if(p1.x !=p2.x) 
          p1.x = (yMax - p1.y)/ m + p1.x;
        p1.y = yMax;
       }
    }
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

void swapPoints(Point *p1, Point *p2){
  Point tmp;
  tmp = *p1;
  *p1 = *p2;
  *p2 = tmp;
}
