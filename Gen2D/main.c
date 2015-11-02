#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <time.h>
#include "gen2d.h"

int main(int argc, char *argv[])
{
  char *fileName = NULL;
  int flag = 0;

  int ch;
  opterr = 0;
  if(argc == 1)
  {
    printf(">>>>>>>>>>>>>>>>Usage: <<<<<<<<<<<<<<<<\n");
    printf("                                       \n");
    printf("-s <filename> for square distribution.\n");
    printf("-c <filename> for circle distribution.\n");
    printf("-g <filename> for gaussian distribution.\n");
    printf("                                       \n");
    printf("Input file format MUST be:             \n");
    printf("x y displacement count class           \n");
    printf("                                       \n");
    printf("Output is print to stdout, to print to \n");
    printf("file use \"> <filename>\" after command. \n");
    printf("^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");
  }

  while ((ch = getopt (argc, argv, "s:c:g:")) != -1)
    switch (ch)
      {
      case 's':
        flag = SFLAG;
        fileName = optarg;
        break;
      case 'c':
        flag = CFLAG;
        fileName = optarg;
        break;
      case 'g':
        flag = GFLAG;
        fileName = optarg;
        break;
      case '?':
        if (optopt == 'c' || optopt == 'g' || optopt == 's')
          fprintf (stderr, "Option -%c requires an argument.\n", optopt);
        else if (isprint (optopt))
          fprintf (stderr, "Unknown option `-%c'.\n", optopt);
        else
          fprintf (stderr,
                   "Unknown option character `\\x%x'.\n",
                   optopt);
        return 1;
      default:
        abort ();
      }
  
  srand(time(0));
  time_t start, end;
  start = clock();
  readFile(fileName, flag);
  end = clock();

  double t = (double)(end - start) / (double)CLOCKS_PER_SEC;
  fprintf(stderr, "time: %.6f\n", t);

  return 0;
}
