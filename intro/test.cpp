#include <cstdio>
#include <iostream>
#include "../p1/common_type.h"
union {
  struct{
    char w; 
    char x;
    char y;
    char z;
  } st;
  char triPoint[4];
  int triInt;
} un;

class C{
  public :Point *ptr; 
  public:
  C();
  ~C();
};
C::C(){
  ptr = new Point[3]; 
  printf("allocated\n");
}
C::~C(){
  printf("deallocated\n");
  delete[] ptr;
}

int main(){
  char cinBuffer[256];
  std::cin.getline(cinBuffer, 256);
  std::cout << cinBuffer << std::endl;
  
  un.st.w = 2; 
  un.st.x = 3;
  un.st.y = 4;
  un.st.z = 5;
  printf("Values in memory in sequential order begining from addr A: %d %d %d %d\n", un.triPoint[0], un.triPoint[1], un.triPoint[2], un.triPoint[3]);
  printf("The int value at addr A is 0%x\n\n", un.triInt ); 
 
  un.triInt = 0x01020304 ;
  printf("Values in memory in sequential order begining from addr A: %d %d %d %d\n", un.triPoint[0], un.triPoint[1], un.triPoint[2], un.triPoint[3]);
  printf("The int value at addr A is 0%x\n\n", un.triInt ); 
 
  C c;
  c.ptr[0].x = 3;
  c.ptr[0].y = 2;



  return 0;
}
