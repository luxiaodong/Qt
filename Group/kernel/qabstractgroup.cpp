#include "qabstractgroup.h"

//构造群
//1.封闭性
//2.结合率
//3.单位性
//4.逆元素
void QAbstractGroup::setGMatrix(const GMatrix matrix)
{
    int size = matrix.size();
    assert(size > 0);

    for(int i = 0; i < size; ++i)
    {
        GList l = matrix.at(i);
        int count = l.size();
        assert(count > 0);
    }

    this->m_matrix.clear();
    this->m_matrix = matrix;
}

//元素的阶
gint QAbstractGroup::rank(const gint e) const
{
    gint rank = 0;
    gint start = e;
    gint next = start;

    while(1)
    {
        rank++;

        if( next == GROUP_ZERO )
        {
            break;
        }
        else
        {
            next = this->at(next,start);
        }
    }

    return rank;
}

//逆元素
gint QAbstractGroup::inverse(const gint e) const
{
    gint r = GROUP_ZERO;
    while(1)
    {
        if( this->at(e,r) == GROUP_ZERO )
        {
            break;
        }
        r++;
    }

    return r;
}

//元素的幂
gint QAbstractGroup::power(const gint e, const gint n) const
{
    gint rank = this->rank(e);
    gint times = n%rank;

    if(times == 0)
    {
        return GROUP_ZERO;
    }

    gint r = e;

    for(int i = 0; i < times; ++i)
    {
        r = this->at(r,e);
    }

    return r;
}

//共轭元素
gint QAbstractGroup::conjugate(const gint a, const gint g) const
{
    //g~ag
    gint rg = this->inverse(g);
    gint rga = this->at(rg,a);
    gint rgag = this->at(rga,g);
    return rgag;
}

//共轭子集
GList QAbstractGroup::conjugateSet(const GList &set, const gint g) const
{
    assert(set.size() > 0);
    GList r;

    for(int i = 0; i < set.size(); ++i)
    {
        gint e = this->conjugate(i, g);
        gint find = r.indexOf(e);

        if(find == -1)
        {
            r.append(e);
        }
    }

    return r;
}

//元素的共轭类
GList QAbstractGroup::conjugateClass(const gint e) const
{
    GList r;
    gint rank = this->rank();

    for(int i = 0; i < rank; ++i)
    {
        gint other = this->conjugate(e,i);
        int find = r.indexOf(other);

        if(find == -1)
        {
            r.append(other);
        }
    }

    return r;
}

//元素的正规化子
GList QAbstractGroup::normalizer(const GList &set) const
{
    assert(set.size() > 0);
    GList r;
    r.append(GROUP_ZERO);
    gint rank = this->rank();

    for(int i = 1; i < rank; ++i)
    {
        GList temp = this->conjugateSet(set,i);

        if(this->isSameSet(set,temp) == true)
        {
            r.append(i);
        }
    }

    return r;
}

//元素的中心化子
GList QAbstractGroup::centralizer(const GList &set) const
{
    assert(set.size() > 0);
    GList r;
    r.append(GROUP_ZERO);
    gint rank = this->rank();

    for(int i = 1; i < rank; ++i)
    {
        bool canSwap = true;
        for(int j = 0; j < set.size(); ++j)
        {
            if(this->canSwap(i, set.at(j)) == false)
            {
                canSwap = false;
                break;
            }
        }

        if(canSwap == true)
        {
            r.append(i);
        }
    }

    return r;
}

//群的中心
GList QAbstractGroup::center() const
{
    GList set = this->m_matrix.at(0);
    return this->centralizer(set);
}

//群的共轭分类
GMatrix QAbstractGroup::conjugateClass() const
{
    GMatrix r;
    gint rank = this->rank();

    for(int i = 0; i < rank; ++i)
    {
        GList t = this->conjugateClass(i);

        bool isAlreadyAdd = false;
        for(int j = 0; j < r.size(); ++j)
        {
            if(this->isSameSet(t, r.at(j)) == true)
            {
                isAlreadyAdd = true;
                break;
            }
        }

        if(isAlreadyAdd == false)
        {
            r.append(t);
        }
    }

//to check
    gint count = 0;
    for(int i = 0; i < r.size(); ++i)
    {
        count += r.at(i).count();
    }

    assert(count == rank);

    return r;
}

//陪集分解
GList QAbstractGroup::leftCoset(const GList &set, const gint e)
{
    GList r;
    for(int i = 0; i < set.size(); ++i)
    {
        r.append(this->at(e,set.at(i)));
    }

    return r;
}

GList QAbstractGroup::rightCoset(const GList &set, const gint e)
{
    GList r;
    for(int i = 0; i < set.size(); ++i)
    {
        r.append(this->at(set.at(i),e));
    }

    return r;
}

//正规闭包
GList QAbstractGroup::normalclosure(const GList &set) const
{
    assert(set.size() > 0);
    GList r;
    gint rank = this->rank();

    for(int i = 0; i < rank; ++i)
    {
        for(int j = 0; j < set.size(); ++j)
        {
            gint e = this->at(i,set.at(j));
            gint find = r.indexOf(e);

            if(find == -1)
            {
                r.append(e);
            }
        }
    }

    return r;
}

//商群
GList QAbstractGroup::quotient(const GList &set) const
{
    assert(this->isNormalSubGroup(set) == true);

    GList r;
    gint rank = this->rank();
    gint size = set.size();

    if(size == 1)
    {
        r = this->m_matrix.at(0);
    }
    else if(size == rank)
    {
        r.append(GROUP_ZERO);
    }
    else
    {
        assert(rank%size == 0);
    }

    return r;
}

//换位子
gint QAbstractGroup::commutator(const gint one, const gint other) const
{
    gint inverseOne = this->inverse(one);
    gint inverseOther = this->inverse(other);
    return this->at(this->at(inverseOne,inverseOther),this->at(one,other));
}

//换位子的导出群
GList QAbstractGroup::commutant() const
{
    GList s;
    s.append(GROUP_ZERO);
    gint rank = this->rank();

    for(gint i = 1; i < rank; i++)
    {
        for(gint j = 1; j < rank; j++)
        {
            gint e = this->commutator(i,j);
            gint find = s.indexOf(e);

            if(find == -1)
            {
                s.append(e);
            }
        }
    }

    return s;
}

//内自同构群
GList QAbstractGroup::inner() const
{
    return this->quotient(this->center());
}

//两个元素乘积是否可以交换
bool QAbstractGroup::canSwap(const gint one, const gint other) const
{
    return this->at(one,other) == this->at(other,one);
}

//判断两个集合是否相等
bool QAbstractGroup::isSameSet(const GList &one, const GList &other) const
{
    assert(one.size() > 0);
    assert(other.size() > 0);

    if (one.size() != other.size())
    {
        return false;
    }

    for(int i = 0; i < one.size(); ++i)
    {
        gint e = one.at(i);
        gint find = other.indexOf(e);

        if(find == -1)
        {
            return false;
        }
    }

    return true;
}

//是否是交换群
bool QAbstractGroup::isAbel() const
{
    gint rank = this->rank();

    for(int i = 0; i < rank; ++i)
    {
        for(int j = 0; j < i; ++j)
        {
            if(this->canSwap(i,j) == false)
            {
                return false;
            }
        }
    }

    return true;
}

//是否是正规子群
bool QAbstractGroup::isNormalSubGroup(const GList &set) const
{
    GList t = this->normalclosure(set);
    return this->isSameSet(t,set);
}


