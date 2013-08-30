#include "qcyclicgroup.h"

QCyclicGroup::QCyclicGroup(const gint size)
{
    assert(size > 0);
    GList e;

    for(int i = 0; i < size; ++i)
    {
        e.append(i);
    }

    this->createCyclic(e);
}

//创建循环群 I(rank)
void QCyclicGroup::createCyclic(const GList e)
{
    assert(e.size() > 0);

    this->m_list = e;
    GMatrix matrix;

    for(gint i = 0; i < e.size(); ++i)
    {
        GList s;

        if(i == 0)
        {
            for(gint j = 0; j < e.size(); ++j)
            {
                s.append(j);
            }
        }
        else
        {
            for(gint j = 0; j < e.size(); ++j)
            {
                if(j == 0)
                {
                    s.append(i);
                }
                else
                {
//怎么办，但是没有规则
                }
            }
        }

        for(gint j = 0; j < rank; j++)
        {
            t.append( (i+j)%rank );
        }

        matrix.append(t);
    }

    QPermuteGroup::createPermute(matrix);
}

