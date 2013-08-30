#ifndef QCYCLICGROUP_H
#define QCYCLICGROUP_H

#include "qpermutegroup.h"

class QCyclicGroup : public QAbstractGroup
{
public:
    QCyclicGroup(){}
    QCyclicGroup(const gint size);
    void createCyclic(const GList e);
private:
    GList m_list;
};

#endif // QCYCLICGROUP_H
