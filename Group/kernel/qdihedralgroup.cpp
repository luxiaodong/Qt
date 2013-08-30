#include "QDihedralGroup.h"

QDihedralGroup::QDihedralGroup(gint edge)
{
    this->create(edge);
}

void QDihedralGroup::create(const gint edge)
{
    GPermute table;

    for(gint i = 0; i < edge; i++)
    {
        GList t1;
        GList t2;

        for(gint j = 0; j < edge; j++)
        {
            t1.append((i+j)%edge);
            t2.append(((i-j-1+edge)%edge));
        }

        table.append(t1);
        table.append(t2);
    }

    QPermuteGroup::createPermute(table);
}
