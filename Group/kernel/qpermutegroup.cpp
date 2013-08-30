#include "qpermutegroup.h"
#include <qdebug.h>

QPermuteGroup::QPermuteGroup(const gint size)
{
    assert(size > 0);
    this->m_table.clear();
    this->createPermute(size);
}

//创建置换群
void QPermuteGroup::createPermute(GMatrix &table)
{
    assert(table.size() > 0);

    this->m_table = table;
    gint rank = table.size();
    GMatrix  matrix;
    for(gint i = 0; i < rank; i++)
    {
        GList s;

        if(i == 0)
        {
            for(gint j = 0; j < rank; j++)
            {
                s.append(j);
            }
        }
        else
        {
            for(gint j = 0; j < rank; j++)
            {
                if(j == 0)
                {
                    s.append(i);
                }
                else
                {
                    GList one = table.at(i);
                    GList other = table.at(j);
                    GList e = this->metathesis(one,other);
                    gint find = table.indexOf(e);
                    assert(find != -1);
                    s.append(find);
                }
            }
        }

        matrix.append(s);
    }

    this->setGMatrix(matrix);
}

//产生所有的排列组合
void QPermuteGroup::createPermute(const gint size)
{
    GMatrix table;
    GList board;

    for(gint i = 0; i < size; ++i)
    {
        board.append(0);
    }

    gint row = 0;

    while( row != -1 )
    {
        board[row]++;

        while(board[row] <= size )
        {
            bool conflict = false;
            gint i = 0;

            for(; i < row; i++)
            {
                if(board[i] == board[row])
                {
                    conflict = true;
                    break;
                }
            }

            if(conflict == true)
            {
                board[row]++;
            }
            else
            {
                break;
            }
        }

        if(board[row] <= size)
        {
            if(row == size - 1)
            {
                GList temp;

                for(gint j = 0; j < size; j++)
                {
                    temp.append(board.at(j) - 1);
                }

                table.append(temp);
            }
            else
            {
                row++;
                board[row] = 0;
            }
        }
        else
        {
            row--;
        }
    }

    this->createPermute(table);
}

//置换作用
GList QPermuteGroup::metathesis(const GList &one, const GList &other) const
{
    assert(one.size() == other.size());

    GList r;

    for(gint i = 0; i < one.size(); ++i)
    {
        r.append(one.at(other.at(i)));
    }

    return r;
}

gint QPermuteGroup::metathesis(const gint one, const gint other) const
{
    return this->at(one,other);
}

//置换元素的轨迹
GMatrix QPermuteGroup::trace(const GList &e) const
{
    GMatrix r;
    GList copy;

    for(gint i = 0; i < e.size(); i++)
    {
        copy.append(i);
    }

    while(1)
    {
        if(copy.isEmpty())
        {
            break;
        }

        GList t;
        gint target = copy.at(0);
        gint current = target;
        t.append(current);
        copy.removeAll(current);

        while(1)
        {
            if(target == e.at(current))
            {
                break;
            }
            else
            {
                current = e.at(current);
                t.append(current);
                copy.removeAll(current);
            }
        }

        r.append(t);
    }

    return r;
}

GMatrix QPermuteGroup::trace(const gint e) const
{
    GList r = this->m_table.at(e);
    return this->trace(r);
}

//置换奇偶性
bool QPermuteGroup::isOddElement(const GList &e) const
{
    gint size = e.size();
    gint count = this->trace(e).count();
    return ((size - count)%2) ? true : false;
}

bool QPermuteGroup::isOddElement(const gint e) const
{
    GList r = this->m_table.at(e);
    return this->isOddElement(r);
}

//偶置换索引表
GList QPermuteGroup::evenPermutions() const
{
    GList s;
    gint size = this->m_table.size();

    for(gint i = 0; i < size; i++)
    {
        if(this->isOddElement(i) == false)
        {
            s.append(i);
        }
    }

    return s;
}

