#include <sys/time.h>

rsl_internal_microclock ()
{
    struct timeval tb ;
    struct timezone tzp ;
    int isec ;  /* seconds */
    int usec ;  /* microseconds */
    int msecs ;
    gettimeofday( &tb, &tzp ) ;
    isec = tb.tv_sec ;
    usec = tb.tv_usec ;
    msecs = 1000000 * isec + usec ;
    return(msecs) ;
}

/*
c_pow_ ( float * a, float * b )
{
    *a = pow( *a , *b ) ;
}*/


