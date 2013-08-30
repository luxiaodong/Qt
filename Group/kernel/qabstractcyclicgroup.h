#ifndef QABSTRACTCYCLICGROUP_H
#define QABSTRACTCYCLICGROUP_H

#include "qabstractgroup.h"
/*
    抽象循环群，同构与Z(n,+)
*/
class QAbstractCyclicGroup : public QAbstractGroup
{
public:
    QAbstractCyclicGroup(){}
    QAbstractCyclicGroup(const gint size);
    void createCyclic(const gint size);
};

#endif // QABSTRACTCYCLICGROUP_H
