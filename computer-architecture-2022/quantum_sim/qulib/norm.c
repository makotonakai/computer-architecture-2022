#include <math.h>
#include "norm.h"

int seed = 1, b = 314159221;
double m = 2.147483648e9;

int srand32(int s)
{
   seed = s;
	return seed;
}

/* Generates a pseudo-random integer from -(2^31) to 2^31-1 */
int rand32()
{
   seed = seed*b+1;
   return seed;
}

double uniform()
{
   return ((double)rand32()/m+1)/2;
}

double norm()
{
   int i, j;
   double x1, x2, r2;

   i = rand32();
   j = rand32();
   x1 = (double)i/m;
   x2 = (double)j/m;
   r2 = x1*x1+x2*x2;
   while (r2 >= 1 || r2 == 0) {
      i = rand32();
      j = rand32();
      x1 = (double)i/m;
      x2 = (double)j/m;
      r2 = x1*x1+x2*x2;
   }

   return x1*sqrt(-2*log(r2)/r2);
}
