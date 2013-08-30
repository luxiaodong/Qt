#ifndef QABSTRACTGROUP_H
#define QABSTRACTGROUP_H

#include "define.h"

/*
    �ñ���¼ȺԪ�ص�ӳ���ϵ
    nԪ������ֵ��Ԫ��ֵȡֵ��ΧΪ (0,n-1)
    0 �ǵ�λԪ��
    ����Ӧ��ȫ��������ֵ
    ��һ�к͵�һ��ȡֵһ����(0,n-1)
 */
class QAbstractGroup
{
    enum { GROUP_ZERO = 0 }; //��λԪ
public:
    QAbstractGroup(){}
    QAbstractGroup(GMatrix matrix){this->setGMatrix(matrix);}
//Ⱥ
    void setGMatrix(const GMatrix matrix);
    gint rank() const {return this->m_matrix.size();}

//Ⱥ��Ԫ��
    gint rank(const gint e) const;
    gint at(const gint x, const gint y = 0) const {return m_matrix.at(x).at(y);}
    gint inverse(const gint e) const;
    gint power(const gint e, const gint n) const;

//Ԫ�صĹ���
    gint conjugate(const gint a, const gint g) const;
    GList conjugateSet(const GList &set, const gint g) const;
    GList conjugateClass(const gint e) const;
    GList normalizer(const GList &set) const;
    GList centralizer(const GList &set) const;
    GList center() const;
    GMatrix conjugateClass() const;

//�㼯�ֽ�,���������Ⱥ,����ô��
    GList leftCoset(const GList &set, const gint e);
    GList rightCoset(const GList &set, const gint e);

//��Ⱥ�������Ӽ�
    GList normalclosure(const GList &set) const;
    GList quotient(const GList &set) const;

//��λ��,����Ⱥ
    gint commutator(const gint one, const gint other) const;
    GList commutant() const;

//��ͬ��Ⱥ,Ŀǰ��֪���ǲ���ԭ������Ⱥ��������ǣ�����Ҫ���ⶨ��
    GList inner() const;

//check
    bool canSwap(const gint one, const gint other) const;
    bool isSameSet(const GList &one, const GList &other) const;
    bool isAbel() const;
    bool isNormalSubGroup(const GList &set) const;

private:
    GMatrix m_matrix;
};

#endif // QABSTRACTGROUP_H
