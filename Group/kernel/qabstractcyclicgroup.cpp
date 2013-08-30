#include "qabstractcyclicgroup.h"

QAbstractCyclicGroup::QAbstractCyclicGroup(const gint size)
{
    assert(size > 0);
    this->createCyclic(size);
}

void QAbstractCyclicGroup::createCyclic(const gint size)
{
    GMatrix matrix;

    for(gint i = 0; i < size; ++i)
    {
        GList s;
        for(gint j = 0; j < size; ++j)
        {
           s.append((i+j)%size);
        }

        matrix.append(s);
    }

    this->setGMatrix(matrix);
}
