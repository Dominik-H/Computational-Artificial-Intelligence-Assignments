#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <ctype.h>
#include "perceptron.h"

int main(int argc, char *argv[])
{
  char *trainFileName = NULL;
  char *weightsFileName = NULL;
  char *weightsProgressionFileName = NULL;
  char *computationDataFileName = NULL;
  char *errorFileName = NULL;
  int flags = 0;

  int ch;
  opterr = 0;
  
  if(argc == 1)
  {
    printf(">>>>>>>>>>>>>>>>>>>>>>>>>Usage: <<<<<<<<<<<<<<<<<<<<<<<<<\n");
    printf("                                       \n");
    printf("-w <filename> to export weights after trained,\n");
    printf("              or to provide weights for computation.\n");
    printf("-t <filename> to provide file with train data.\n");
    printf("-p <filename> if you want progression data exported.\n");
    printf("-c <filename> if you want to compute results using,\n");
    printf("              precomputed weights or computed weights\n"); 
    printf("              from train data.\n");
    printf("-e <filename> to export error data.\n");
    printf("^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");

    return 1;
  }

  while ((ch = getopt (argc, argv, "w:t:p:c:e:")) != -1)
    switch (ch)
      {
      case 'w':
        flags |= PRC_EXPORT_WEIGHTS;
        weightsFileName = optarg;
        break;
      case 't':
        flags |= PRC_TRAIN;
        trainFileName = optarg;
        break;
      case 'p':
        flags |= PRC_EXPORT_WEIGHTS_PROGRESS;
        weightsProgressionFileName = optarg;
        break;
      case 'c':
        flags |= PRC_COMPUTE;
        computationDataFileName = optarg;
        break;
      case 'e':
        flags |= PRC_EXPORT_ERROR;
        errorFileName = optarg;
        break;
      case '?':
        if (optopt == 'c' || optopt == 'w' || optopt == 't' || optopt == 'p')
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

  if(((flags & PRC_COMPUTE) && !(flags & PRC_EXPORT_WEIGHTS)) && ((flags & PRC_COMPUTE) && !(flags & PRC_TRAIN)))
  {
    fprintf(stderr, "You need to provide file with weights or train data in order to get results!\n");
    return 1;
  }
  
  float *weights;
  flags |= PRC_GEN_IW;
  int numWeights;

  // Train
  if(flags & PRC_TRAIN)
  {
    if((flags & PRC_EXPORT_WEIGHTS) || (flags & PRC_EXPORT_WEIGHTS_PROGRESS) || (flags & PRC_COMPUTE))
    {
      Train(trainFileName, weights, &numWeights, weightsFileName, weightsProgressionFileName, errorFileName, flags, 0.2f);
      return 0;
    }
  }

  // Compute
  if(flags & PRC_COMPUTE)
  {
    Compute(computationDataFileName, weights, numWeights);
    free(weights);
    return 0;
  }

  printf("Your Call Did Nothing. Try Using Different Combination Of Arguments.\n");

  return 1;
}