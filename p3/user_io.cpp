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
/* @fn        :     void readHeaders(std::ifstream *ifs, int*window_width, int*window_height, int*numberOfObjects)
 * @brief     :     prime the pump - reads the header info from  file
 *                  the header contains info about the dimension of the window and the numeber of objects ther are
 * @param[in] :     std::ifstream *ifs    - pointer to the input file stream
 * @param[out]:     int *window_width     - for storing window_width
 * @param[out]:     int *window_height    - for storing window_height
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
void readPolyhedra(std::ifstream *ifs, Graph **graphs, Polyhedron **polyhedra, int numberOfPolyhedra){ 

  int sizeOfBuffer = 256, numberOfPoints, numberOfEdges, numberOfSurfaces, numberOfPolyhedraAlreadyProcessed = 0;
  float x, y , z;
  int p1Index, p2Index; 
  char buffer[sizeOfBuffer], *charPtr; 
  Point_3D *listOfPoints; Edge *listOfEdges; Surface *listOfSurfaces;
  while(ifs->getline(buffer, sizeOfBuffer)) { // skip a line for before entering the section for describing the next polyhedron
    ifs->getline(buffer, sizeOfBuffer);//read the number of points for constructing the new polyhedron 
    numberOfPoints =  atoi(buffer); 
     
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
    
    //read edges
    ifs->getline(buffer, sizeOfBuffer); // read the number of edges
    numberOfEdges = atoi(buffer);
    listOfEdges = new Edge[numberOfEdges];
    for(int i = 0, offset = -1 ; i < numberOfEdges; i++){
      ifs->getline(buffer, sizeOfBuffer);
      charPtr = strtok(buffer, " ");
      p1Index = atoi(charPtr);
      charPtr = strtok(0, "\0");
      p2Index = atoi(charPtr);
      listOfEdges[i].p1Index = p1Index + offset;  // make point 1 to have index (1 - 1) = 0
      listOfEdges[i].p2Index = p2Index + offset;
      DPRINT("Edge %d %d\n", p1Index, p2Index);
    }

    //read surfaces;
    ifs->getline(buffer, sizeOfBuffer); // read number of surfaces
    numberOfSurfaces = atoi(buffer);
    listOfSurfaces = new Surface[numberOfSurfaces];
    for(int i = 0 , offset = -1; i < numberOfSurfaces; i++){
      ifs->getline(buffer, sizeOfBuffer);
      charPtr = strtok(buffer, " ");
      listOfSurfaces[i].p1Index = atof(charPtr) + offset; // +offset to make index starting from 0
      charPtr = strtok(0, " ");
      listOfSurfaces[i].p2Index = atof(charPtr) + offset;
      charPtr = strtok(0, "\0");
      listOfSurfaces[i].p3Index = atof(charPtr) + offset; 
      DPRINT("Surface %d : %d %d %d\n", i, listOfSurfaces[i].p1Index - offset, listOfSurfaces[i].p2Index - offset, listOfSurfaces[i].p3Index - offset);
    }
    
    //read normals for each surface 
    for(int i = 0 ; i < numberOfSurfaces; i++){
      ifs->getline(buffer, sizeOfBuffer);
      charPtr = strtok(buffer, " ");
      x = atof(charPtr);
      charPtr = strtok(0, " ");
      y = atof(charPtr);
      charPtr = strtok(0, "\0");
      z = atof(charPtr);
      listOfSurfaces[i].normalVector.x = x;
      listOfSurfaces[i].normalVector.y = y;
      listOfSurfaces[i].normalVector.z = z;
      DPRINT("Normal Vector %d : %.2f %.2f %.2f\n",i, x,y,z); 
    }

    polyhedra[numberOfPolyhedraAlreadyProcessed] = new Polyhedron(graphs, listOfPoints, numberOfPoints, listOfEdges, numberOfEdges, listOfSurfaces, numberOfSurfaces);
    delete [] listOfPoints;
    delete [] listOfEdges;
    delete [] listOfSurfaces; 

    DPRINT("\n"); fflush(stdout);
    if( ++numberOfPolyhedraAlreadyProcessed == numberOfPolyhedra) // already processed the desired number of polyhedra done!!
      break;
    if(ifs->eof()) //end of file - done!!
      break;
  }
}


