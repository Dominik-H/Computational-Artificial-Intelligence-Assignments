#include "perceptron.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include "../kvec.h"

typedef struct Line{
  float x[NUM_DATA];
  int c; // class
} Line;

// *Training Function.*
//
// [in] trainFileName - 2D vector of training data with classification
// [out] weights - place to save weights
// [out] numWeights - size of weights array
// [in] weightsFile - file where to export weights
// [in] progressFile - file where to export progress
// [in] errorFile - file where to export error data
// [in] flags - flags containing options
// [in] gama - learning rate
void Train(char* trainFileName, float* weights, int* numWeights, char* weightsFile, char* progressFile, char* errFile, int flags, const float gama)
{
  FILE *tFile = NULL;
  FILE *wFile = NULL;
  FILE *pFile = NULL;
  FILE *eFile = NULL;

  if(!(flags & PRC_TRAIN))
    exit(1);

  if((flags & PRC_TRAIN) && (trainFileName == NULL))
    exit(1);

  tFile = fopen(trainFileName, "r");

  if(!tFile)
  {
    exit(1);
  }

  if(flags & PRC_EXPORT_WEIGHTS)
  {
    if(weightsFile != NULL)
    {
      wFile = fopen(weightsFile, "w");
      if(!wFile)
      {
        exit(1);
      }
    }
    else
    {
      fprintf(stderr, "WARN: Weights won't be exported!\n");
    }
  }

  if(flags & PRC_EXPORT_WEIGHTS_PROGRESS)
  {
    if(progressFile != NULL)
    {
      pFile = fopen(progressFile, "w");
      if(!pFile)
      {
        exit(1);
      }
    }
    else
    {
      fprintf(stderr, "WARN: Progress won't be exported!\n");
    }
  }

  if(flags & PRC_EXPORT_ERROR)
  {
    if(errFile != NULL)
    {
      eFile = fopen(errFile, "w");
      if(!eFile)
      {
        exit(1);
      }
    }
    else
    {
      fprintf(stderr, "WARN: Error won't be exported!\n");
    }
  }

  kvec_t(Line) ls;
  kv_init(ls);

  // Load Data
  int load = 1;
  while(load != EOF)
  {
    Line l;
#pragma omp parallel for
    for(unsigned int i = 0; i < NUM_DATA; ++i)
    {
      load = fscanf(tFile, "%f", &l.x[i]);
    }

    load = fscanf(tFile, "%d", &l.c);

    kv_push(Line, ls, l);
  }

  // Pre-train operations
  if((flags & PRC_GEN_IW) || (weights == NULL))
  {
    weights = malloc((NUM_DATA + 1) * sizeof(float));
    //weights[0] = 3000 + rand() % 2000;
    for(unsigned int i = 0; i <= NUM_DATA; ++i)
    {
      weights[i] = rand() % 10000;
    }

    *numWeights = NUM_DATA + 1; // w[0] -> bias
  }

  printf("Begin weights: %f %f %f\n", weights[0], weights[1], weights[2]);

  int error = 0;
  int counter = 0;

  time_t start, end;
  start = clock(); 

  // Train
  do {
    error = 0;
    for(unsigned int i = 0; i < ls.n; ++i)
    {
      int out = Neuron(ls.a[i].x, weights, NUM_DATA);
      if(out != ls.a[i].c)
      {
        ++error;
        if(out)
        {
#pragma omp parallel for
          for(unsigned int j = 0; j <= NUM_DATA; ++j)
          {
            weights[j] -= (j == 0)?(-gama):(gama * ls.a[i].x[j-1]);
          }
        } 
        else 
        {
#pragma omp parallel for
          for(unsigned int j = 1; j <= NUM_DATA; ++j)
          {
            weights[j] += (j == 0)?(-gama):(gama * ls.a[i].x[j-1]);
          }
        }
      }
    }

    // export progress data
    if(pFile)
    {
      fprintf(pFile, "%f %f\n%f %f\n", -500.0f, (-weights[1] / weights[2])*(-500) + (weights[0] / weights[2]), 600.0f, (-weights[1] / weights[2])*(600) + (weights[0] / weights[2]));
    }

    // export error data
    if(eFile)
    {
      fprintf(eFile, "%d\n", error);
    }

    ++counter; 
  } while(error >  0);

  end = clock();

  double t = (double)(end - start) / (double)CLOCKS_PER_SEC;
  fprintf(stderr, "Time: %f \n", t);

  // export weights
  if(wFile)
  {
#pragma omp parallel for
    for(unsigned int i = 0; i <= NUM_DATA; ++i)
    {
      fprintf(wFile, "%f ", weights[i]);
    }
  }

  printf("Done in %d cycles.\n", counter);

  kv_destroy(ls);

  fclose(tFile);
  if(wFile)
    fclose(wFile);
  if(pFile)
    fclose(pFile);
}

// Function that computes output using pre-computed weights
void Compute(char* dataFile, float* weights, int numWeights)
{

}

// *Function that runs inside the neuron.*
// **MAYBE I'LL CHANGE NAME LATER**
//
// [in] data - vector of input data (X-es)
// [in] weights - vector of input weights -> size is size of data vector + 1 (bias)
// [in] numData - size of data vector
// *return -> neuron output (1 or 0)
int Neuron(const float* data, const float* weights, const int numData)
{
  float sum = -weights[0];

#pragma omp parallel for
  for(unsigned int i = 1; i <= numData; ++i)
  {
    sum += data[i - 1] * weights[i];
  }

  if(sum >= 0.0f)
    return 1;
  else 
    return 0;
}
