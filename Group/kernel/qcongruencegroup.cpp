#include "qcongruencegroup.h"

QCongruenceGroup::QCongruenceGroup(const gint prime):QAbstractCyclicGroup(prime - 1)
{
    assert(QMath::isPrime(prime) == true);
    this->createCongruence(2,prime);
}

void QCongruenceGroup::createCongruence(const gint e, const gint prime)
{
    this->m_list.clear();
    this->m_list.append(1);
    gint next = e;

    while(1)
    {
        this->m_list.append(next);
        next = (next*e)%prime;

        if(next == 1)
        {
            break;
        }

    }

    return ;
}
