#ifndef GNUMBERTHEORY_H
#define GNUMBERTHEORY_H

class GNumberTheory
{
public:
    GNumberTheory();

public:
    int gcd(const int a,const int b);
    int lcm(const int a,const int b);
    bool isPrime(const int n);
};

#endif // GNUMBERTHEORY_H
