#include "qigobeatnodeposition.h"
#include <QDebug>

QiGoBeatNodePosition::QiGoBeatNodePosition()
{
    this->createOneTapSeq();
    this->createTwoTapPair();
    this->createTwoTapSeq();
    this->createThreeTapPair();
    this->createThreeTapSeq();
    this->createFourTapPair();
    this->createFourTapSeq();
}

void QiGoBeatNodePosition::initPositionList(int count)
{
    for(int i = 0; i < count; ++i)
    {
        this->m_positionList.append(-1);
    }

    for(int i = 0; i < 16; ++i)
    {
        m_isActive[i] = false;
        m_nodeTapEndIndex[i] = 0;
    }
}

void QiGoBeatNodePosition::updatePositionAt(int nodeIndex)
{
    for(int i = 0; i < 16; ++i)
    {
        if(m_nodeTapEndIndex[i] < nodeIndex)
        {
            m_isActive[i] = false;
        }
        else
        {
            m_isActive[i] = true;
        }
    }
}

void QiGoBeatNodePosition::analysisPosition(const QBsmNode* root, float bpm)
{
    m_currentPositionIndex = 0;
    m_normalTapNeedIndex = 0.8*bpm + 1;
    const QBsmNode* currentNode = root;

    while(currentNode != 0)
    {
        int continueNumber = currentNode->sideCountContinueNumber();

        int nodeTapEndIndex[16];
        for(int i = 0; i < 16; ++i)
        {
            nodeTapEndIndex[i] = m_nodeTapEndIndex[i];
        }

        while(1)
        {
            if(this->analysisSubPosition(currentNode, continueNumber) == true)
            {
                break;
            }
            else
            {
                if(continueNumber > 1)
                {
                    continueNumber = continueNumber/2;
                }
                else
                {
                    continueNumber = 1;
                    m_currentPositionIndex += currentNode->sideCount(currentNode);
                    break;
                }
            }

            for(int i = 0; i < 16; ++i)
            {
                m_nodeTapEndIndex[i] = nodeTapEndIndex[i];
            }
        }

        for(int i = 0; i < continueNumber; ++i)
        {
            currentNode = currentNode->m_next;
        }
    }
}

bool QiGoBeatNodePosition::analysisSubPosition(const QBsmNode *root, int sequenceCount)
{
    //下面的工作是找到一个合适的解，或者判断出无解
    //验证解的函数比较简单，关键是如果判断有无解
    int togetherCount = root->sideCount(root);
    QList<int> randomSequence = this->createRandomSequenceIndex(togetherCount, sequenceCount);

    //qDebug()<<"xxxxxx"<<randomSequence.size()<<"yyyyyyy"<<randomSequence;

    if(randomSequence.size() == 0)
    {
        //这里面的点全都不要,大于5的情况,暂时这么处理吧
        //m_currentPositionIndex += sequenceCount*togetherCount;
        return false;
    }

    int i = 0;
    while(i < 10000)
    {
        if(this->isSequenceValid(root,randomSequence,togetherCount,sequenceCount) == true)
        {
            this->createPositionIndexFormSequence(randomSequence,togetherCount,sequenceCount);
            return true;
        }

        i++;
    }

    //找不到解,暂时这样,可以考虑用遗传算法优化下
    //m_currentPositionIndex += sequenceCount*togetherCount;
    return false;
}

QList<int> QiGoBeatNodePosition::createRandomSequenceIndex(int togetherCount, int sequenceCount)
{
    QList<int> randomSequence;
    int alreadyAddSequenceIndex = 0;
    if(togetherCount == 1)
    {
        int size = this->m_oneTapSeq.size();
        while(alreadyAddSequenceIndex < sequenceCount)
        {
            int r = rand()%size;
            QList<int> sub = this->m_oneTapSeq.at(r);
            randomSequence.append(sub);
            alreadyAddSequenceIndex += sub.size();
        }
    }
    else if(togetherCount == 2)
    {
        int size = this->m_twoTapSeq.size();
        while(alreadyAddSequenceIndex < sequenceCount)
        {
            int r = rand()%size;
            QList<int> sub = this->m_twoTapSeq.at(r);
            randomSequence.append(sub);
            alreadyAddSequenceIndex += sub.size();
        }
    }
    else if(togetherCount == 3)
    {
        int size = this->m_threeTapSeq.size();
        while(alreadyAddSequenceIndex < sequenceCount)
        {
            int r = rand()%size;
            QList<int> sub = this->m_threeTapSeq.at(r);
            randomSequence.append(sub);
            alreadyAddSequenceIndex += sub.size();
        }
    }
    else if(togetherCount == 4)
    {
        int size = this->m_fourTapSeq.size();
        while(alreadyAddSequenceIndex < sequenceCount)
        {
            int r = rand()%size;
            QList<int> sub = this->m_fourTapSeq.at(r);
            randomSequence.append(sub);
            alreadyAddSequenceIndex += sub.size();
        }
    }
    else
    {
        qDebug()<<"not support for 5 above!";
    }

    //qDebug()<<randomSequence;
    return randomSequence;
}

bool QiGoBeatNodePosition::isSequenceValid(const QBsmNode* root, QList<int> &seq, int togetherCount, int sequenceCount)
{
    const QBsmNode* currentNode = root;
    const QBsmNodeData* currentNodeData = 0;
    int sequenceIndex = 0;

    if(togetherCount == 1)
    {
        while(sequenceIndex < sequenceCount)
        {
            currentNodeData = currentNode->m_data;
            this->updatePositionAt(currentNodeData->m_nodeIndex);
            int positionIndex = seq.at(sequenceIndex);
            if(this->m_isActive[positionIndex] == false)
            {
                if(currentNodeData->m_nodeContinueIndex > 0)
                {
                    m_nodeTapEndIndex[positionIndex] = currentNodeData->m_nodeIndex + currentNodeData->m_nodeContinueIndex;
                }
                else
                {
                    m_nodeTapEndIndex[positionIndex] = currentNodeData->m_nodeIndex + this->m_normalTapNeedIndex;
                }

                currentNode = currentNode->m_next;
                sequenceIndex++;
            }
            else
            {
                return false;
            }
        }

        return true;
    }
    else if(togetherCount == 2)
    {
        while(sequenceIndex < sequenceCount)
        {
            currentNodeData = currentNode->m_data;
            this->updatePositionAt(currentNodeData->m_nodeIndex);
            QList<int> positionPair = this->m_twoTapPair.at(seq.at(sequenceIndex));
            if(this->m_isActive[positionPair.at(0)] == false &&
               this->m_isActive[positionPair.at(1)] == false)
            {
                int temp = currentNodeData->m_nodeIndex + this->m_normalTapNeedIndex;

                if(currentNodeData->m_nodeContinueIndex > 0)
                {
                    temp = currentNodeData->m_nodeIndex + currentNodeData->m_nodeContinueIndex;
                }

                m_nodeTapEndIndex[positionPair.at(0)] = temp;
                m_nodeTapEndIndex[positionPair.at(1)] = temp;

                currentNode = currentNode->m_next;
                sequenceIndex++;
            }
            else
            {
                return false;
            }
        }

        return true;
    }
    else if(togetherCount == 3)
    {
        while(sequenceIndex < sequenceCount)
        {
            currentNodeData = currentNode->m_data;
            this->updatePositionAt(currentNodeData->m_nodeIndex);
            QList<int> positionPair = this->m_threeTapPair.at(seq.at(sequenceIndex));
            if(this->m_isActive[positionPair.at(0)] == false &&
               this->m_isActive[positionPair.at(1)] == false &&
               this->m_isActive[positionPair.at(2)] == false)
            {
                int temp = currentNodeData->m_nodeIndex + this->m_normalTapNeedIndex;

                if(currentNodeData->m_nodeContinueIndex > 0)
                {
                    temp = currentNodeData->m_nodeIndex + currentNodeData->m_nodeContinueIndex;
                }

                m_nodeTapEndIndex[positionPair.at(0)] = temp;
                m_nodeTapEndIndex[positionPair.at(1)] = temp;
                m_nodeTapEndIndex[positionPair.at(2)] = temp;

                currentNode = currentNode->m_next;
                sequenceIndex++;
            }
            else
            {
                return false;
            }
        }

        return true;
    }
    else if(togetherCount == 4)
    {
        while(sequenceIndex < sequenceCount)
        {
            currentNodeData = currentNode->m_data;
            this->updatePositionAt(currentNodeData->m_nodeIndex);
            QList<int> positionPair = this->m_fourTapPair.at(seq.at(sequenceIndex));
            if(this->m_isActive[positionPair.at(0)] == false &&
               this->m_isActive[positionPair.at(1)] == false &&
               this->m_isActive[positionPair.at(2)] == false &&
               this->m_isActive[positionPair.at(3)] == false)
            {
                int temp = currentNodeData->m_nodeIndex + this->m_normalTapNeedIndex;

                if(currentNodeData->m_nodeContinueIndex > 0)
                {
                    temp = currentNodeData->m_nodeIndex + currentNodeData->m_nodeContinueIndex;
                }

                m_nodeTapEndIndex[positionPair.at(0)] = temp;
                m_nodeTapEndIndex[positionPair.at(1)] = temp;
                m_nodeTapEndIndex[positionPair.at(2)] = temp;
                m_nodeTapEndIndex[positionPair.at(3)] = temp;

                currentNode = currentNode->m_next;
                sequenceIndex++;
            }
            else
            {
                return false;
            }
        }

        return true;
    }

    return false;
}

void QiGoBeatNodePosition::createPositionIndexFormSequence(QList<int> &seq, int togetherCount, int sequenceCount)
{
    if(togetherCount == 1)
    {
        for(int i = 0; i < sequenceCount; ++i)
        {
            this->m_positionList.replace(m_currentPositionIndex, seq.at(i));
            m_currentPositionIndex++;
        }
    }
    else if(togetherCount == 2)
    {
        for(int i = 0; i < sequenceCount; ++i)
        {
            QList<int> togetherPair = this->m_twoTapPair.at(seq.at(i));
            this->m_positionList.replace(m_currentPositionIndex, togetherPair.at(0));
            m_currentPositionIndex++;
            this->m_positionList.replace(m_currentPositionIndex, togetherPair.at(1));
            m_currentPositionIndex++;
        }
    }
    else if(togetherCount == 3)
    {
        for(int i = 0; i < sequenceCount; ++i)
        {
            QList<int> togetherPair = this->m_threeTapPair.at(seq.at(i));
            this->m_positionList.replace(m_currentPositionIndex, togetherPair.at(0));
            m_currentPositionIndex++;
            this->m_positionList.replace(m_currentPositionIndex, togetherPair.at(1));
            m_currentPositionIndex++;
            this->m_positionList.replace(m_currentPositionIndex, togetherPair.at(2));
            m_currentPositionIndex++;
        }
    }
    else if(togetherCount == 4)
    {
        for(int i = 0; i < sequenceCount; ++i)
        {
            QList<int> togetherPair = this->m_fourTapPair.at(seq.at(i));
            this->m_positionList.replace(m_currentPositionIndex, togetherPair.at(0));
            m_currentPositionIndex++;
            this->m_positionList.replace(m_currentPositionIndex, togetherPair.at(1));
            m_currentPositionIndex++;
            this->m_positionList.replace(m_currentPositionIndex, togetherPair.at(2));
            m_currentPositionIndex++;
            this->m_positionList.replace(m_currentPositionIndex, togetherPair.at(3));
            m_currentPositionIndex++;
        }
    }
}


/*
  0  1  2  3
  4  5  6  7
  8  9 10 11
 12 13 14 15
*/
void QiGoBeatNodePosition::createOneTapSeq()
{
    QString str = QString();
    str += QString("0,3;4,7;8,11;12,15;0,7;3,4;8,15;11,12;");
    str += QString("0,5;4,9;8,13;1,6;5,10;9,14;2,7;6,11;10,15;");
    str += QString("3,6;2,5;1,4;7,10;6,9;5,8;11,14;10,13;9,12;");
    str += QString("1,2;5,6;9,10;13,14;0,15;3,12;");

    str += QString("4,1,6;5,2,7;8,13,10;9,14,11;");
    str += QString("0,5,2;1,6,3;12,9,14;13,10,15;");
    str += QString("4,9,6;8,5,10;9,6,11;5,10,7;");

    str += QString("0,1,2,3;4,5,6,7;8,9,10,11;12,13,14,15;");
    str += QString("0,4,8,12;1,5,9,13;2,6,10,14;3,7,11,15;");
    str += QString("0,3,4,7,8,11,12,15;1,2,5,6,9,10,13,14;");

    this->m_oneTapSeq.clear();
    QStringList list1 = str.split(";");
    int size1 = list1.size();
    for(int i = 0; i < size1; ++i)
    {
        QList<int> temp;
        QStringList list2 = list1.at(i).split(",");
        int size2 = list2.size();
        for(int j = 0; j < size2; ++j)
        {
            temp.append(list2.at(j).toInt());
        }

        if(temp.size() > 0)
        {
            m_oneTapSeq.append(temp);
            m_oneTapSeq.append(this->inverseList(temp));
        }
    }

    //qDebug()<<"one tap seq is "<<this->m_oneTapSeq;
}

void QiGoBeatNodePosition::createTwoTapPair()
{
    this->m_twoTapPair.clear();
    int array[56][2] =
    {
        //横二
        {0,1},{2,3},{4,5},{6,7},
        {8,9},{10,11},{12,13},{14,15},
        //竖二
        {0,4},{1,5},{2,6},{3,7},
        {8,12},{9,13},{10,14},{11,15},
        //左右对称
        {0,3},{4,7},{8,11},{12,15},
        {1,2},{5,6},{9,10},{13,14},
        //边二
        {1,2},{4,8},{7,11},{13,14},
        //斜二
        {0,5},{3,6},{9,12},{10,15},
        //斜二2
        {1,4},{2,7},{11,14},{8,13},
        //斜二3
        {5,10},{6,9},{0,15},{3,12},
        //跳二
        {0,2},{1,3},{4,6},{5,7},
        {8,10},{9,11},{12,14},{13,15},
        {0,8},{4,12},{1,9},{5,13},
        {2,10},{6,14},{3,11},{7,15},
    };

    for(int i = 0; i < 56; ++i)
    {
        QList<int> list;
        list.append(array[i][0]);
        list.append(array[i][1]);
        this->m_twoTapPair.append(list);
    }

    //qDebug()<<"two tap pair is "<<m_twoTapPair;
}

void QiGoBeatNodePosition::createTwoTapSeq()
{
    QString str = QString();
    str += QString("0,2,4,6;1,3,5,7;0,3,4,7;1,2,5,6;");
    str += QString("8,9,10,11;12,13,14,15;8,13,10,15;12,9,14,11;");
    str += QString("16,17,18,19;20,21,22,23;16,21,18,23;20,17,22,19;");
    str += QString("24,26,25,27;28,29,30,31;32,33,34,35;36,37;38,39;");
    str += QString("40,42,44,46;41,43,45,47;40,43,44,47;41,42,45,46;");
    str += QString("48,49,50,51;52,53,54,55;48,53,50,55;52,49,54,51;");

    this->m_twoTapSeq.clear();
    QStringList list1 = str.split(";");
    int size1 = list1.size();
    for(int i = 0; i < size1; ++i)
    {
        QList<int> temp;
        QStringList list2 = list1.at(i).split(",");
        int size2 = list2.size();
        for(int j = 0; j < size2; ++j)
        {
            temp.append(list2.at(j).toInt());
        }

        if(temp.size() > 0)
        {
            m_twoTapSeq.append(temp);
            m_twoTapSeq.append(this->inverseList(temp));
        }
    }

    //qDebug()<<"two tap seq is "<<this->m_twoTapSeq;
}

void QiGoBeatNodePosition::createThreeTapPair()
{
    this->m_threeTapPair.clear();
    int array[56][3] =
    {
        //横三
        {0,1,2},{4,5,6},{8,9,10},{12,13,14},
        {1,2,3},{5,6,7},{9,10,11},{13,14,15},
        //竖三
        {0,4,8},{1,5,9},{2,6,10},{3,7,11},
        {4,8,12},{5,9,13},{6,10,14},{7,11,15},
        //横眠三
        {0,1,3},{4,5,7},{8,9,11},{12,13,15},
        {0,2,3},{4,6,7},{8,10,11},{12,14,15},
        //竖眠三
        {0,4,12},{1,5,13},{2,6,14},{3,7,15},
        {0,8,12},{1,9,13},{2,10,14},{3,11,15},
        //斜三
        {1,6,11},{4,9,14},{2,5,8},{7,10,13},
        //三角
        {0,1,4},{2,3,7},{8,12,13},{11,14,15},
        {0,5,4},{2,6,7},{8,9,13},{11,14,10},
        {1,4,6},{5,10,7},{9,12,14},
        {2,5,7},{4,9,6},{10,13,15},
        {0,5,2},{9,6,11},{8,13,10},
        {1,6,3},{5,8,10},{9,14,11},
    };

    for(int i = 0; i < 56; ++i)
    {
        QList<int> list;
        list.append(array[i][0]);
        list.append(array[i][1]);
        list.append(array[i][2]);
        this->m_threeTapPair.append(list);
    }

    //qDebug()<<"three tap pair is "<<m_threeTapPair;
}

void QiGoBeatNodePosition::createThreeTapSeq()
{
    QString str = QString();
    str += QString("0,1,2,3;4,5,6,7;8,9,10,11;12,13,14,15;");
    str += QString("0,5,2,7;4,1,6,3;8,13,10,15;12,9,14,11;");
    str += QString("0,1,6,7;4,5,2,3;");
    str += QString("16,17,18,19;20,21,22,23;24,25,26,27;28,29,30,31;");
    str += QString("32,33;34,35;");
    str += QString("36,37,38,39;40,41,42,43;");
    str += QString("44,45,46;47,48,49;50,51,52;53,54,55");

    this->m_threeTapSeq.clear();
    QStringList list1 = str.split(";");
    int size1 = list1.size();
    for(int i = 0; i < size1; ++i)
    {
        QList<int> temp;
        QStringList list2 = list1.at(i).split(",");
        int size2 = list2.size();
        for(int j = 0; j < size2; ++j)
        {
            temp.append(list2.at(j).toInt());
        }

        if(temp.size() > 0)
        {
            m_threeTapSeq.append(temp);
            m_threeTapSeq.append(this->inverseList(temp));
        }
    }

    //qDebug()<<"three tap seq is "<<this->m_threeTapSeq;
}

void QiGoBeatNodePosition::createFourTapPair()
{
    this->m_fourTapPair.clear();
    int array[24][4] =
    {
        //横四
        {0,1,2,3},{4,5,6,7},{8,9,10,11},{12,13,14,15},
        {0,4,8,12},{1,5,9,13},{2,6,10,14},{3,7,11,15},
        {0,1,14,15},{4,5,10,11},{8,9,6,7},{12,13,2,3},
        {0,3,12,15},{5,6,9,10},{0,5,10,15},{3,6,9,12},
        {1,4,6,9},{2,5,7,10},{5,8,10,13},{6,9,11,14},
        {1,4,11,14},{2,7,8,13},
    };

    for(int i = 0; i < 24; ++i)
    {
        QList<int> list;
        list.append(array[i][0]);
        list.append(array[i][1]);
        list.append(array[i][2]);
        list.append(array[i][3]);
        this->m_fourTapPair.append(list);
    }

    //qDebug()<<"four tap pair is "<<m_fourTapPair;
}

void QiGoBeatNodePosition::createFourTapSeq()
{
    QString str = QString();
    str += QString("0,1,2,3;4,5,6,7;8,9,10,11;");
    str += QString("12,13;14,15;16,17;18,19;20,21;");

    this->m_fourTapSeq.clear();
    QStringList list1 = str.split(";");
    int size1 = list1.size();
    for(int i = 0; i < size1; ++i)
    {
        QList<int> temp;
        QStringList list2 = list1.at(i).split(",");
        int size2 = list2.size();
        for(int j = 0; j < size2; ++j)
        {
            temp.append(list2.at(j).toInt());
        }

        if(temp.size() > 0)
        {
            m_fourTapSeq.append(temp);
            m_fourTapSeq.append(this->inverseList(temp));
        }
    }

    //qDebug()<<"four tap seq is "<<this->m_fourTapSeq;
}

QList<int> QiGoBeatNodePosition::inverseList(QList<int> list)
{
    QList<int> inverse;
    int size = list.size();
    for(int i = 0; i < size; ++i)
    {
        inverse.append(list.at(size - 1 - i));
    }

    return inverse;
}
