#include "wtime.h"
#include <time.h>
//#ifndef DOS
//#include <sys/time.h>
//#endif

#include <omp.h>

void wtime(double *t)
{
#if defined(_OPENMP)
	*t = omp_get_wtime();
#else
	* t = 0.0;
#endif
   /* a generic timer */
   //static int sec = -1;
   //struct timeval tv;
   //gettimeofday(&tv, (void *)0);
   //if (sec < 0) sec = tv.tv_sec;
   //*t = (tv.tv_sec - sec) + 1.0e-6*tv.tv_usec;
}

