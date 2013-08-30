#ifndef QCONGRUENCEGROUP_H
#define QCONGRUENCEGROUP_H

#include "qabstractcyclicgroup.h"

/*
    �˷�ͬ����ȺZ(p,*). ע��pһ��Ҫ����
    �ӷ�ͬ����Ⱥ������ QAbstractCyclicGroup
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
