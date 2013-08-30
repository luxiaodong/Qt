#ifndef QBINARYMODEL_H
#define QBINARYMODEL_H

#include "../kernel/qabstractgroup.h"
#include "../kernel/depend/gbitarray.h"
/*
    n个C2组成的直积群
*/
class QBinaryModel : public QAbstractGroup
{
public:
    QBinaryModel();
    bool solve(GList &list);


    void addElement(GList e);
    void solveInverse();
    GMatrix solveRoot();
private:
    GMatrix m_elements;     //基矩阵
    GMatrix m_inverse;      //解矩阵
    GMatrix m_translate;    //转换矩阵
};

#endif // QBINARYMODEL_H
