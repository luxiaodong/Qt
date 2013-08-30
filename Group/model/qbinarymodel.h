#ifndef QBINARYMODEL_H
#define QBINARYMODEL_H

#include "../kernel/qabstractgroup.h"
#include "../kernel/depend/gbitarray.h"
/*
    n��C2��ɵ�ֱ��Ⱥ
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
    GMatrix m_elements;     //������
    GMatrix m_inverse;      //�����
    GMatrix m_translate;    //ת������
};

#endif // QBINARYMODEL_H
