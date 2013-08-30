#ifndef QPERMUTEGROUP_H
#define QPERMUTEGROUP_H

#include "qabstractgroup.h"

typedef GMatrix GPermute;
/*
    �û�Ⱥ
    ֻ�ṩһά����,��Ӧ�û�Ԫ��
*/
class QPermuteGroup : public QAbstractGroup
{
public:
    QPermuteGroup(){}
    QPermuteGroup(const gint size);
//�����û�Ⱥ
    void createPermute(GMatrix &table);

//�������е��������
    void createPermute(const gint size);

//�û�����
    gint metathesis(const gint one, const gint other) const;
    GList metathesis(const GList &one, const GList &other) const;

//�û�Ԫ�صĹ켣
    GMatrix trace(const GList &e) const;
    GMatrix trace(const gint e) const;

//�ж��û�����ż�� 0ż1��
    bool isOddElement(const GList &e) const;
    bool isOddElement(const gint e) const;

//ȡ��ż�û�������Ԫ��
    GList evenPermutions() const;

private:
    // n!*n
    GPermute m_table;
};

#endif // QPERMUTEGROUP_H
