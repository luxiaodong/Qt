#include "GNumberTheory.h"

GNumberTheory::GNumberTheory()
{
}

int GNumberTheory::gcd(const int a, const int b)
{
    if(b == 0)
    {
        return a;
    }

    return gcd(b, a%b);
}

int GNumberTheory::lcm(const int a, const int b)
{
    return a*b/gcd(a,b);
}

bool GNumberTheory::isPrime(const int n)
{
    for(int i = 2; i < n; ++i)
    {
        if(i*i >= n)
        {
            break;
        }

        if(n%i == 0)
        {
            return false;
        }
    }

    return true;
}

