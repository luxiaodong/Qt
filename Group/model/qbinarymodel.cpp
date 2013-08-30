#include "qbinarymodel.h"

QBinaryModel::QBinaryModel()
{
    this->m_elements.clear();
    this->m_inverse.clear();
    this->m_translate.clear();
}


void QBinaryModel::addElement(GList e)
{
    this->m_elements.append(e);
    GList value;
    value.append(e.first());
    this->m_inverse.append(value);
}

void QBinaryModel::solveInverse()
{
    GMatrix translate = this->m_elements;
    GMatrix inverse = this->m_inverse;
    int rowCount = translate.size(); //行数

    GList ones;
    for(int i = 0; i < rowCount; ++i)
    {
        ones.append(1);
    }

//先把它化成上三角矩阵
    for(int j = 0; j < rowCount; j++)
    {
        if(translate.at(j).size() == 0)
        {
            continue;
        }
//这里的first会导致元素并非顺序下来,所以到最后还要进行一次排序
        gint value = translate.at(j).first();
        gint i = j + 1;

        while(i < rowCount)
        {
            int index = translate.at(i).indexOf(value);

            if(index != -1)
            {
                for(int k = 0; k < translate.at(j).size(); ++k)
                {
                    gint value1 = translate.at(j).at(k);
                    int find = translate.at(i).indexOf(value1);

                    if( find == -1)
                    {
                        translate[i].append(value1);
                    }
                    else
                    {
                        translate[i].removeAt(find);
                    }
                }

                qSort(translate[i].begin(), translate[i].end());

                for(int k = 0; k < inverse.at(j).size(); ++k)
                {
                    gint value1 = inverse.at(j).at(k);
                    int find = inverse.at(i).indexOf(value1);

                    if( find == -1)
                    {
                        inverse[i].append(value1);
                    }
                    else
                    {
                        inverse[i].removeAt(find);
                    }
                }

                qSort(inverse[i].begin(), inverse[i].end());

                if(ones.at(i) == 0)
                {
                    ones.replace(i,1);
                }
                else
                {
                    ones.replace(i,0);
                }
            }

            i++;
        }
    }

//转为对角矩阵
    for(int j = rowCount - 1; j >= 0; --j)
    {
        if(translate.at(j).size() == 0)
        {
            continue;
        }

        int value = translate.at(j).first();
        int i = 0;

        while(i < j)
        {
            int index = translate.at(i).indexOf(value);

            if(index != -1)
            {
                //不一定满秩
                for(int k = 0; k < translate.at(j).size(); ++k)
                {
                    gint value1 = translate.at(j).at(k);
                    int find = translate.at(i).indexOf(value1);
                    if( find == -1)
                    {
                        translate[i].append(value1);
                    }
                    else
                    {
                        translate[i].removeAt(find);
                    }
                }

                qSort(translate[i].begin(), translate[i].end());

                for(int k = 0; k < inverse.at(j).size(); ++k)
                {
                    gint value1 = inverse.at(j).at(k);
                    int find = inverse.at(i).indexOf(value1);

                    if( find == -1)
                    {
                        inverse[i].append(value1);
                    }
                    else
                    {
                        inverse[i].removeAt(find);
                    }
                }

                qSort(inverse[i].begin(), inverse[i].end());

                if(ones.at(i) == 0)
                {
                    ones.replace(i,1);
                }
                else
                {
                    ones.replace(i,0);
                }
            }

            i++;
        }
    }

    for(int i = 0; i < inverse.size(); ++i)
    {
        qSort(inverse[i].begin(),inverse[i].end());
    }

    this->m_inverse = inverse;
    this->m_translate = translate;
}

GMatrix QBinaryModel::solveRoot()
{
//如果translate是满秩,就很容易了,全加在一起就可以了
//如果不是满秩,是否存在这样的变换,应该怎么求
//其实相当于求一个方程组ax=b,其中a = m_elements, b = 1,求x
    int count = this->m_elements.size();
    GList ones;
    this->m_inverse.append(ones);
    for(int i =0; i < count; ++i)
    {
        ones.append(i);
    }
    this->m_elements.append(ones);
    this->solveInverse();
    return this->m_inverse;
}

//new api
bool QBinaryModel::solve(GList &list)
{
	list.clear();
	return false;
}
