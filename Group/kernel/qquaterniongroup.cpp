#include "qquaterniongroup.h"

QQuaternionGroup::QQuaternionGroup()
{
    GMatrix matrix;
    gint n = 4;

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
            s.append((n/2 + 2*n + i - j)%n);
        }

        matrix.append(s);
    }

    this->setGMatrix(matrix);
}
