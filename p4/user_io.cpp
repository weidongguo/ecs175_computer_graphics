#include "user_io.h"
void readFile(std::ifstream &ifs){
  char cinBuffer[256];
  while(1){ 
    std::cout<<"Please enter name of the input file: "; std::cout.flush();
    std::cin.getline(cinBuffer, 256);
    ifs.open (cinBuffer, std::ifstream::in); 
    if( ifs.good() ){ //opened the file succesfully
      std::cout <<"Successfully loaded file\n";
      break;
    }
    else
      std::cout <<"Error: file not found, please try again\n";
  }

}

/*=====================================================================*/
/* @fn        :     void readHeaders(std::ifstream *ifs, int*numberOfObjects)
 * @brief     :     prime the pump - reads the header info from  file
 *                   - the numeber of objects there are
 * @param[in] :     std::ifstream *ifs    - pointer to the input file stream
 * @param[out]:     int *numberOfObjects  - for storing the number of objects
 * @return    :     none
 */
void readHeaders(std::ifstream *ifs, int*numberOfObjects){
  int sizeOfBuffer = 256;
  char buffer[sizeOfBuffer], *charPtr; 
  //prime the pump
  ifs->getline(buffer, sizeOfBuffer); // contains number of polygon
  *numberOfObjects = atoi(buffer);
  DPRINT("numberOfObjects: %d\n", *numberOfObjects);
}

/*========================================================================*/
/* @fn        :   void readPolhedra(std::ifstream *ifs, Graph **graphs, Polyhedron **polyhedra, int numberOfPolyhedra);
 * @brief     :   read data from an input file stream  and build polyhedra using the data 
 * @param[in] :   std::ifstream *ifs   - pointer to the input file stream
 * @param[in] :   Graph **graphs         - list of graphs for drawing pixel and related drawing methods, each graph takes up 1 quadrant of screen
 * @param[in] :   int numberOfPolyhedra - number of polyhedra
 * @param[out]:   Polyhedron **polyhedra   - the output: polyhdera created by the info specified in the file
 * @return    :   none
 */
void readCurves(std::ifstream *ifs, Graph *graph, std::list<Curve*> *curves, int numberOfCurves){ 

  int sizeOfBuffer = 256, numberOfCtrlPoints, numberOfCurvesAlreadyProcessed = 0, k ;
  float x,y, *knotValues;
  char buffer[sizeOfBuffer], *charPtr; 
  Point_2D *ctrlPoints; 
  Curve *curve;
    
  bool isKnotsSpecified = false;
  printf("reading Curves...\n"); 
  while(ifs->getline(buffer, sizeOfBuffer)) { // skip a line for before entering the section for describing the next polyhedron
    ifs->getline(buffer, sizeOfBuffer);
    if(strstr(buffer, "bz")){
      ifs->getline(buffer, sizeOfBuffer);
      numberOfCtrlPoints = atoi(buffer);          
      ctrlPoints = new Point_2D[numberOfCtrlPoints];
      for(int i = 0 ; i < numberOfCtrlPoints; i++){
        ifs->getline(buffer, sizeOfBuffer);
        charPtr = strtok(buffer, ", ");
        x = atoi(charPtr); 
        charPtr = strtok(0, "\0");
        y = atoi(charPtr);
        ctrlPoints[i].x = x;
        ctrlPoints[i].y = y;
      }
      curve = (Curve*)( new Bezier(graph, ctrlPoints, numberOfCtrlPoints) ); 
      //curve->printAttributes();
      delete [] ctrlPoints;
    }
    else{ // bs - bspline curves
      ifs->getline(buffer, sizeOfBuffer); // # of control points
      numberOfCtrlPoints = atoi(buffer);
      ifs->getline(buffer, sizeOfBuffer); // k value
      k = atoi(buffer);
      ifs->getline(buffer, sizeOfBuffer); // are knots specified?
      if(strchr(buffer, 'T')) 
        isKnotsSpecified = true; 
      else
        isKnotsSpecified = false;
      
      ctrlPoints = new Point_2D[numberOfCtrlPoints];
      for(int i = 0 ; i < numberOfCtrlPoints; i++){
        ifs->getline(buffer, sizeOfBuffer);
        charPtr = strtok(buffer, ", ");
        x = atoi(charPtr); 
        charPtr = strtok(0, "\0");
        y = atoi(charPtr);
        ctrlPoints[i].x = x;
        ctrlPoints[i].y = y;
      }
      
      knotValues = new float[numberOfCtrlPoints+k];
      if(isKnotsSpecified){
        ifs->getline(buffer, sizeOfBuffer);
        charPtr = strtok(buffer, ", ");
        knotValues[0] = atof(charPtr);
        for(int i = 1 ; i < numberOfCtrlPoints+k-1; i++){
          charPtr = strtok(0, ", ");
          knotValues[i] = atof(charPtr);
        }
        charPtr = strtok(0, "\0");
        knotValues[numberOfCtrlPoints+k-1] = atof(charPtr);
      }
      else{ // if not specified, then generate a sequence;
        for(int i = 0 ; i < numberOfCtrlPoints+k; i++){
          knotValues[i] = i; 
        }
      } 

      curve = (Curve*)( new Bspline(graph, ctrlPoints, numberOfCtrlPoints, k, knotValues) );
      //curve->printAttributes();
      delete[] knotValues;
      delete[] ctrlPoints;
    }
    
    curves->push_back(curve); 

     
    if( ++numberOfCurvesAlreadyProcessed == numberOfCurves) 
      break;
    if(ifs->eof()) //end of file - done!!
      break;
  }
}


