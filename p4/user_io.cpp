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
void readCurves(std::ifstream *ifs, Graph *graph, std::list<Curve>*curves, int numberOfCurves){ 

  int sizeOfBuffer = 256, numberOfCtrlPoints, numberOfCurvesAlreadyProcessed = 0;
  float x, y , z;
  char buffer[sizeOfBuffer], *charPtr; 
  Point *ctrlPoints; 
  while(ifs->getline(buffer, sizeOfBuffer)) { // skip a line for before entering the section for describing the next polyhedron
//    ifs->getline(buffer, sizeOfBuffer);
     
     
    ifs->getline(buffer, sizeOfBuffer);//read the number of points for constructing the new polyhedron 
    numberOfCtrlPoints =  atoi(buffer); 
/*     
    listOfPoints = new Point_3D[numberOfPoints];
    for(int i = 0 ; i < numberOfPoints; i++){ // form a listOfPoints;
      ifs->getline(buffer, sizeOfBuffer); //get a 3D point
      charPtr = strtok(buffer, " ");
      x = atof(charPtr); //x value;
      charPtr = strtok(0, " ");
      y = atof(charPtr);//y value
      charPtr = strtok(0, "\0");
      z = atof(charPtr); //z_value
      listOfPoints[i].x = x;
      listOfPoints[i].y = y; 
      listOfPoints[i].z = z;
      DPRINT("(%.2f, %.2f, %.2f)\n", x, y, z);
    }
 */   
    //    polyhedra[numberOfPolyhedraAlreadyProcessed] = new Polyhedron(graphs, listOfPoints, numberOfPoints, listOfEdges, numberOfEdges, listOfSurfaces, numberOfSurfaces);

    DPRINT("\n"); fflush(stdout);
    if( ++numberOfCurvesAlreadyProcessed == numberOfCurves) // already processed the desired number of polyhedra done!!
      break;
    if(ifs->eof()) //end of file - done!!
      break;
  }
}


