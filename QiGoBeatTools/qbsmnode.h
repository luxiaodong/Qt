#ifndef QBSMNODE_H
#define QBSMNODE_H

#include <QString>
#include <QStringList>
#include <QList>
#include <QDebug>

class QBsmNodeData
{
public:
    bool isLongNode();
public:
    int  m_nodeTag;
    int  m_nodeIndex; //全局的
    int  m_nodePosition;// 1 - 10;
    int  m_nodeContinueIndex;//长音符持续时间
    int  m_nodeValue;
};

/*
  #                 #
  |                 |
  #---#             #---#
  |                 |   |
  #---#    ---->    #   #
  |                 |   |
  #---#             #   #
  |                 |

  (1)               (2)
*/

//先生成(1)号数据结构，再转成(2)，需要吗?
class QBsmNode
{
    //二叉树结构
public:
    QBsmNode(const QBsmNodeData* data);
    int equisequenceNumber()const;//从该节点出发，时间上是等差数列的个数
    int sideCount(const QBsmNode* node)const;
    int sideCountContinueNumber()const;//从该节点出发，右边节点个数相同的连续的个数

    QBsmNode*   m_side;//右边
    QBsmNode*   m_next;//下面
    const QBsmNodeData* m_data;
};

#endif // QBSMNODE_H
