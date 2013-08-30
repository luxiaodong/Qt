#include "qmath.h"

QMath::QMath()
{}

bool QMath::isPrime(const int n)
{
    int i = 2;
    while(1)
    {
        if(n%i == 0)
        {
            return false;
        }

        ++i;

        if(i*i > n)
        {
            break;
        }
    }

    return true;
}
