#include "qabstractdihedralgroup.h"

QAbstractDihedralGroup::QAbstractDihedralGroup(const gint n)
{
    assert(n > 0);
    this->createDihedral(n);
}

void QAbstractDihedralGroup::createDihedral(const gint n)
{
    GMatrix matrix;

    for(gint i = 0; i < n; ++i)
    {
        GList s;
        for(gint j = 0; j < n; ++j)
        {
            s.append((i+j)%n);
        }

        for(gint j = 0; j < n; ++j)
        {
            s.append(n + (i+j)%n);
        }

        matrix.append(s);
    }

    for(gint i = 0; i < n; ++i)
    {
        GList s;
        for(gint j = 0; j < n; ++j)
        {
            s.append((n + i - j)%n + n);
        }

        for(gint j = 0; j < n; ++j)
        {
            s.append((n + i - j)%n);
        }

        matrix.append(s);
    }

    this->setGMatrix(matrix);
}

