#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <omp.h>
#include "gen2d.h" 

// reads file
int readFile(char* filename, int flag)
{
	if(filename == NULL)
    	return -1;
  
	kvec_t(struct FileLine) lines;
	kv_init(lines);

	int numL = 0;
	FILE *fp = fopen(filename, "r");

	char lineStr[512];
	while(fgets(lineStr, 512, fp))
	{
		FileLine ln;

		sscanf(lineStr, "%d %d %d %d %d\n", &ln.x, &ln.y, &ln.displacement, &ln.count, &ln.class);

		kv_push(FileLine, lines, ln);
	}

	fclose(fp);


	switch(flag)
	{
		case SFLAG:
			numL = readFileSquare(lines.a, kv_size(lines));
		break;

		case CFLAG:
			numL = readFileCircle(lines.a, kv_size(lines));
		break;

		case GFLAG:
			numL = readFileGauss(lines.a, kv_size(lines));
		break;

		default:
			kv_destroy(lines);
			return -1;
	}

	kv_destroy(lines);

	return numL;
}

// generates data using Square option
int readFileSquare(FileLine *lines, int numLines)
{
#pragma omp parallel for
	for(unsigned int i = 0; i < numLines; ++i)
	{
		for(unsigned int j = 0; j < lines[i].count; ++j)
		{
			float x;
			float y;

			if(rand() % 2)
			{
				x = lines[i].x - ((float)rand() / (float)(RAND_MAX) * (float)lines[i].displacement);
			}
			else
			{
				x = lines[i].x + ((float)rand() / (float)(RAND_MAX) * (float)lines[i].displacement);
			}

			if(rand() % 2)
			{
				y = lines[i].y - ((float)rand() / (float)(RAND_MAX) * (float)lines[i].displacement);
			}
			else
			{
				y = lines[i].y + ((float)rand() / (float)(RAND_MAX) * (float)lines[i].displacement);
			}

			printf("%.4f %.4f %d\n", x, y, lines[i].class);
		}
	}

	return numLines;
}

// generates data using circle option
int readFileCircle(FileLine *lines, int numLines)
{
#pragma omp parallel for
	for(unsigned int i = 0; i < numLines; ++i)
	{
		unsigned int j = 0;
		while(j < lines[i].count)
		{
			float x;
			float y;

			if(rand() % 2)
			{
				x = lines[i].x - ((float)rand() / (float)(RAND_MAX) * (float)lines[i].displacement);
			}
			else
			{
				x = lines[i].x + ((float)rand() / (float)(RAND_MAX) * (float)lines[i].displacement);
			}

			if(rand() % 2)
			{
				y = lines[i].y - ((float)rand() / (float)(RAND_MAX) * (float)lines[i].displacement);
			}
			else
			{
				y = lines[i].y + ((float)rand() / (float)(RAND_MAX) * (float)lines[i].displacement);
			}

			if((x - lines[i].x)*(x - lines[i].x) + (y - lines[i].y)*(y - lines[i].y) <= (lines[i].displacement*lines[i].displacement))
			{
				printf("%.4f %.4f %d\n", x, y, lines[i].class);
				++j;
			}
		}			
	}

	return numLines;
}

// generates data using gauss option
int readFileGauss(FileLine *lines, int numLines)
{
#pragma omp parallel for
	for(unsigned int i = 0; i < numLines; ++i)
	{
		unsigned int j = 0;
		while(j < lines[i].count)
		{
			float x;
			float y;

			x = rand_normal(lines[i].x, sqrt(lines[i].displacement));
			y = rand_normal(lines[i].y, sqrt(lines[i].displacement));

			if((x - lines[i].x)*(x - lines[i].x) + (y - lines[i].y)*(y - lines[i].y) <= (lines[i].displacement*lines[i].displacement))
			{
				printf("%.4f %.4f %d\n", x, y, lines[i].class);
				++j;
			}
		}			
	}
 
	return numLines;
}

double rand_normal(double mean, double stddev)
{

//Box muller method

	/*// This Algorithm is from:
	// http://en.literateprograms.org/index.php?title=Special:DownloadCode/Box-Muller_transform_(C)&oldid=7011
    static double n2 = 0.0;
    static int n2_cached = 0;
    if (!n2_cached)
    {
        double x, y, r;
        do
        {
            x = 2.0*rand()/RAND_MAX - 1;
            y = 2.0*rand()/RAND_MAX - 1;

            r = x*x + y*y;
        }
        while (r == 0.0 || r > 1.0);
        {
            double d = sqrt(-2.0*log(r)/r);
            double n1 = x*d;
            n2 = y*d;
            double result = n1*stddev + mean;
            n2_cached = 1;
            return result;
        }
    }
    else
    {
        n2_cached = 0;
        return n2*stddev + mean;
    }*/

    // This algorithm is from:
    // http://phoxis.org/2013/05/04/generating-random-numbers-from-normal-distribution-in-c/
   	double U1, U2, W, mult;
	static double X1, X2;
	static int call = 0;

	if (call == 1)
	{
	  call = !call;
	  return (mean + stddev * (double) X2);
	}

	do
	{
	  U1 = -1 + ((double) rand () / RAND_MAX) * 2;
	  U2 = -1 + ((double) rand () / RAND_MAX) * 2;
	  W = pow (U1, 2) + pow (U2, 2);
	}
	while (W >= 1 || W == 0);

	mult = sqrt ((-2 * log (W)) / W);
	X1 = U1 * mult;
	X2 = U2 * mult;

	call = !call;

	return (mean + stddev * (double) X1);
}
