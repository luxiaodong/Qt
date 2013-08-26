#include "qbsmnode.h"

bool QBsmNodeData::isLongNode()
{
    if(m_nodeContinueIndex == 0)
    {
        return false;
    }

    return true;
}

QBsmNode::QBsmNode(const QBsmNodeData* data)
{
    m_next = 0;
    m_side = 0;
    m_data = data;
}

int QBsmNode::equisequenceNumber() const
{
    if(this->m_next == 0)
    {
        return 0;
    }

    const QBsmNode* currentNode = this;
    const QBsmNodeData* currentNodeData = this->m_data;
    const QBsmNode* nextNode = currentNode->m_next;
    const QBsmNodeData* nextNodeData = nextNode->m_data;
    const int delt = nextNodeData->m_nodeIndex - currentNodeData->m_nodeIndex;

    currentNode = nextNode;
    currentNodeData = nextNodeData;

    int number = 1;
    while(currentNode->m_next != 0)
    {
        nextNode = currentNode->m_next;
        nextNodeData = nextNode->m_data;

        if(delt == nextNodeData->m_nodeIndex - currentNodeData->m_nodeIndex)
        {
            number++;
            currentNode = nextNode;
            currentNodeData = nextNodeData;
        }
        else
        {
            break;
        }
    }

    return number;
}

int QBsmNode::sideCount(const QBsmNode* root) const
{
    Q_ASSERT(root != 0);
    int count = 1;
    const QBsmNode* current = root->m_side;
    while(current != 0)
    {
        count++;
        current = current->m_side;
    }

    return count;
}

int QBsmNode::sideCountContinueNumber() const
{
    int number = 1;
    int sideCount = this->sideCount(this);
    const QBsmNode* currentNode = this->m_next;
    while(currentNode != 0)
    {
        if(sideCount == this->sideCount(currentNode))
        {
            number++;
            currentNode = currentNode->m_next;
        }
        else
        {
            break;
        }
    }

    return number;
}
