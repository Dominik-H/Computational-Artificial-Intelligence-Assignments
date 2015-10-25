#include "../kvec.h"

#define CFLAG 0x00000001
#define SFLAG 0x00000002
#define GFLAG 0x00000003

typedef struct FileLine{
  int x;
  int y;
  int displacement;
  int count;
  int class;
} FileLine; 

int readFile(char* filename, int flag);
int readFileSquare(FileLine *lines, int numLines);
int readFileCircle(FileLine *lines, int numLines);
int readFileGauss(FileLine *lines, int numLines);
double rand_normal(double mean, double stddev);
