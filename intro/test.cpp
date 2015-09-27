#include <cstdio>

int main(){
  short ar[3] = {0x12345678, 0x22345678, 0x32345678};
  
  short  *ptr_ar = ar;

  for(int i = 0 ; i < sizeof(ar) / sizeof(short); i++){

    printf("%x ", ar[i]);
    printf("%x\n", (ptr_ar + i) );
  }


  return 0;
}
