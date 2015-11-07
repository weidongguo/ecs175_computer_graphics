 class Vector{ 
  public:
    float x;
    float y;
    float z;
  
  static Vector minus(Point_3D p1, Point_3D p2); 
  static Vector minus(Vector v1, Vector v2); 
  static Vector   unitVector(Vector v);

  static float dotProduct(Vector v1, Vector v2);
  static Vector multByScalar(Vector v, float scalar);
  static Vector add(Vector v1, Vector v2);
  static float magnitude(Vector v);
  static void printVector(const char *tag, Vector v);
  //end of vector operations
}
