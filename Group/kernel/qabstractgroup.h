#ifndef QABSTRACTGROUP_H
#define QABSTRACTGROUP_H

#include "define.h"

/*
    用表格记录群元素的映射关系
    n元素索引值和元素值取值范围为 (0,n-1)
    0 是单位元素
    参数应该全部用索引值
    第一行和第一列取值一定是(0,n-1)
 */
class QAbstractGroup
{
    enum { GROUP_ZERO = 0 }; //单位元
public:
    QAbstractGroup(){}
    QAbstractGroup(GMatrix matrix){this->setGMatrix(matrix);}
//群
    void setGMatrix(const GMatrix matrix);
    gint rank() const {return this->m_matrix.size();}

//群的元素
    gint rank(const gint e) const;
    gint at(const gint x, const gint y = 0) const {return m_matrix.at(x).at(y);}
    gint inverse(const gint e) const;
    gint power(const gint e, const gint n) const;

//元素的共轭
    gint conjugate(const gint a, const gint g) const;
    GList conjugateSet(const GList &set, const gint g) const;
    GList conjugateClass(const gint e) const;
    GList normalizer(const GList &set) const;
    GList centralizer(const GList &set) const;
    GList center() const;
    GMatrix conjugateClass() const;

//陪集分解,如果不是子群,会怎么样
    GList leftCoset(const GList &set, const gint e);
    GList rightCoset(const GList &set, const gint e);

//商群及正规子集
    GList normalclosure(const GList &set) const;
    GList quotient(const GList &set) const;

//换位子,导出群
    gint commutator(const gint one, const gint other) const;
    GList commutant() const;

//自同构群,目前不知道是不是原来的子群，如果不是，可能要另外定义
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
