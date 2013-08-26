#ifndef QIGOBEATNODEPOSITION_H
#define QIGOBEATNODEPOSITION_H

#include <QString>
#include <QList>
#include <QStringList>

#include "qbsmnode.h"

//四个或者大于四个同时出现,不要太频繁.
//长音符不要在同一个地方,消失了马上出现.
class QiGoBeatNodePosition
{
public:
    QiGoBeatNodePosition();
    void initPositionList(int count);
    void analysisPosition(const QBsmNode* root, float bpm);

public:
    QList<int>  m_positionList;

private:
    bool analysisSubPosition(const QBsmNode *root, int sequenceCount);
    void updatePositionAt(int nodeIndex);
    QList<int> createRandomSequenceIndex(int togetherCount, int sequenceCount);
    bool isSequenceValid(const QBsmNode* root, QList<int> &seq, int togetherCount, int sequenceCount);
    void createPositionIndexFormSequence(QList<int> &seq, int togetherCount, int sequenceCount);
private:
    //如果是跳跃性的大循环，该如何处理
    //要不生成整条列表，然后一个一个赋值
    //要不二次迭代，恩...
    int     m_currentPositionIndex;
    bool    m_isActive[16];
    int     m_nodeTapEndIndex[16];
    int     m_normalTapNeedIndex;
private:
    void createTwoTapPair();
    void createThreeTapPair();
    void createFourTapPair();
    void createOneTapSeq();
    void createTwoTapSeq();
    void createThreeTapSeq();
    void createFourTapSeq();

    QList<int> inverseList(QList<int> list);

private:
    QList<QList<int> >  m_twoTapPair;
    QList<QList<int> >  m_threeTapPair;
    QList<QList<int> >  m_fourTapPair;

//try create a loop
    QList<QList<int> >  m_oneTapSeq;    //0 - 15
    QList<QList<int> >  m_twoTapSeq;    //m_twoTapPair
    QList<QList<int> >  m_threeTapSeq;  //m_threeTapPair
    QList<QList<int> >  m_fourTapSeq;   //m_fourTapPair
};

#endif // QIGOBEATNODEPOSITION_H
