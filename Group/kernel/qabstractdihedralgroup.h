#ifndef QABSTRACTDIHEDRALGROUP_H
#define QABSTRACTDIHEDRALGROUP_H

#include "qabstractgroup.h"
/*
    抽象二面体群D(2*n)  a^n = b^2 = (ab)^2 = 1,
*/
class QAbstractDihedralGroup : public QAbstractGroup
{
public:
    QAbstractDihedralGroup(){};
    QAbstractDihedralGroup(const gint n);
    void createDihedral(const gint n);
};

#endif // QABSTRACTDIHEDRALGROUP_H
