#include "GSpecialSeriesOfNumbers.h"

GSpecialSeriesOfNumbers::GSpecialSeriesOfNumbers()
{
}

int GSpecialSeriesOfNumbers::catalan(int index)
{
    int n = index;
    // C(2*n,n)/(n+1);
    return n;
}

int GSpecialSeriesOfNumbers::fabonacci(int index)
{
    if(index <= 1)
    {
        return 1;
    }


}
