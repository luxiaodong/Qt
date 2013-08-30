#ifndef QPERMUTEGROUP_H
#define QPERMUTEGROUP_H

#include "qabstractgroup.h"

typedef GMatrix GPermute;
/*
    置换群
    只提供一维数组,对应置换元素
*/
class QPermuteGroup : public QAbstractGroup
{
public:
    QPermuteGroup(){}
    QPermuteGroup(const gint size);
//创建置换群
    void createPermute(GMatrix &table);

//产生所有的排列组合
    void createPermute(const gint size);

//置换作用
    gint metathesis(const gint one, const gint other) const;
    GList metathesis(const GList &one, const GList &other) const;

//置换元素的轨迹
    GMatrix trace(const GList &e) const;
    GMatrix trace(const gint e) const;

//判断置换的奇偶性 0偶1奇
    bool isOddElement(const GList &e) const;
    bool isOddElement(const gint e) const;

//取得偶置换的所有元素
    GList evenPermutions() const;

private:
    // n!*n
    GPermute m_table;
};

#endif // QPERMUTEGROUP_H
