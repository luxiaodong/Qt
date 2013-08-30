#ifndef QDIHEDRALCLASS_H
#define QDIHEDRALCLASS_H

#include <qpermutegroup.h>

class QDihedralGroup : public QPermuteGroup
{
public:
    QDihedralGroup(){}
    QDihedralGroup(gint edge);

    void create(const gint edge);
};

#endif // QDIHEDRALCLASS_H
