#ifndef QCONGRUENCEGROUP_H
#define QCONGRUENCEGROUP_H

#include "qabstractcyclicgroup.h"

/*
    乘法同余类群Z(p,*). 注：p一定要素数
    加法同余类群可以用 QAbstractCyclicGroup
*/

class QCongruenceGroup : public QAbstractCyclicGroup
{
public:
    QCongruenceGroup(){}
    QCongruenceGroup(const gint prime);
    void createCongruence(const gint a, const gint prime);
private:
    GList m_list;
};

#endif // QCONGRUENCEGROUP_H
