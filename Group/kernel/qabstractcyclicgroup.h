#ifndef QABSTRACTCYCLICGROUP_H
#define QABSTRACTCYCLICGROUP_H

#include "qabstractgroup.h"
/*
    ����ѭ��Ⱥ��ͬ����Z(n,+)
*/
class QAbstractCyclicGroup : public QAbstractGroup
{
public:
    QAbstractCyclicGroup(){}
    QAbstractCyclicGroup(const gint size);
    void createCyclic(const gint size);
};

#endif // QABSTRACTCYCLICGROUP_H
