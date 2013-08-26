#include "qparsebsm.h"
#include <QDebug>

bool bsmNodeDataIndexLessThan(const QBsmNodeData* s1, const QBsmNodeData* s2)
{
    return s1->m_nodeIndex < s2->m_nodeIndex;
}

QParseBsm::QParseBsm()
{
    m_bpm = 0;
    m_nodeOffset = 5000;
    m_nodeStartIndex = 0;
    m_allBsmNodeData.clear();
    m_root = 0;

    for(int i = 0; i < 10; ++i)
    {
        m_isLongTag[i] = false;
    }
}

void QParseBsm::debugTest()
{
    qDebug()<<"the total size is "<<m_allBsmNodeData.size();
    foreach(const QBsmNodeData* data ,m_allBsmNodeData)
    {
        qDebug()<< "(" << data->m_nodeIndex << ", " << data->m_nodeContinueIndex << ")";
    }

    qDebug()<<"----------root Node test-----------";
    QBsmNode* currentNode = m_root;
    while(currentNode != 0)
    {
        const QBsmNodeData* data = currentNode->m_data;
        qDebug()<< "(" << data->m_nodeIndex << ", " << data->m_nodeContinueIndex << ")";

        if(currentNode->m_side != 0)
        {
            QBsmNode* saveCurrentNode = currentNode;
            while(currentNode->m_side != 0)
            {
                currentNode = currentNode->m_side;
                data = currentNode->m_data;
                qDebug()<< "        (" << data->m_nodeIndex << ", " << data->m_nodeContinueIndex << ")";

            }
            currentNode = saveCurrentNode;
        }
        currentNode = currentNode->m_next;
    }
}

void QParseBsm::parseLine(QString line)
{
    QStringList list = line.split(":");

    if(list.size() != 2)
    {
        return ;
    }

    int index = list.first().remove("#").toInt();
    QString stream = list.last();

    if(index == 0)
    {
        return ;
    }

    int section = index/100;
    int tag = (index/10)%10;
    int position = index%10;
    int count = stream.size()/2;

    if(section == 0)
    {
        for(int i = 0; i < count; ++i)
        {
            QString temp = stream.mid(2*i, 2);
            if(temp != QString("00"))
            {
                m_nodeStartIndex = i*192/count;
                qDebug()<<"the index offset is "<<m_nodeStartIndex;
                return;
            }
        }
    }

    for(int i = 0; i < count; ++i)
    {
        QString temp = stream.mid(2*i, 2);
        if(temp != QString("00") && (position != 0))
        {
            if(tag == 5)
            {
                if(m_isLongTag[position] == true)
                {
                    m_isLongTag[position] = false;
                    QBsmNodeData* data = new QBsmNodeData();
                    data->m_nodeIndex = m_longStartIndex[position];
                    data->m_nodeTag = tag;
                    data->m_nodePosition = position;
                    data->m_nodeContinueIndex = section*192 + i*192/count - m_longStartIndex[position];
                    if(data->m_nodeContinueIndex < 0.8f*m_bpm)
                    {
                        data->m_nodeContinueIndex = 0 - data->m_nodeContinueIndex;
                    }
                    data->m_nodeValue = temp.toInt();
                    m_allBsmNodeData.append(data);
                }
                else
                {
                    m_isLongTag[position] = true;
                    m_longStartIndex[position] = section*192 + i*192/count;
                }
            }
            else
            {
                QBsmNodeData* data = new QBsmNodeData();
                data->m_nodeIndex = section*192 + i*192/count;
                data->m_nodeTag = tag;
                data->m_nodePosition = position;
                data->m_nodeContinueIndex = 0;
                data->m_nodeValue = temp.toInt();
                m_allBsmNodeData.append(data);
            }
        }
    }
}

void QParseBsm::praseBsmFile(QString filePath)
{
    QFile file(filePath);
    if(file.open(QIODevice::ReadOnly) == false)
    {
        return ;
    }

    QTextStream in(&file);
    in.setCodec("UTF-8");
    bool bStart = false;
    m_allBsmNodeData.clear();

    while(in.atEnd() == false)
    {
        QString line = in.readLine().trimmed();

        if(line.left(4) == QString("#BPM"))
        {
            m_bpm = line.remove("#BPM").trimmed().toFloat();
        }

        if(bStart == true)
        {
            this->parseLine(line);
        }

        if(line.contains("MAIN DATA FIELD") == true)
        {
            bStart = true;
        }
    }

    file.close();

    qSort(m_allBsmNodeData.begin(), m_allBsmNodeData.end(), bsmNodeDataIndexLessThan);

    m_filePath = filePath.replace(".bms",".plist");
}

void QParseBsm::createBsmNodeTree()
{
    const QBsmNodeData* nodeData = m_allBsmNodeData.first();
    m_root = new QBsmNode(nodeData);
    QBsmNode* currentNode = m_root;
    int size = m_allBsmNodeData.size();
    int nodeIndex = nodeData->m_nodeIndex;

    int i = 1;
    while(i < size)
    {
        nodeData = m_allBsmNodeData.at(i);
        QBsmNode* temp = new QBsmNode(nodeData);

        if(nodeData->m_nodeIndex == nodeIndex)
        {
            QBsmNode* saveCurrentNode = currentNode;
            while(nodeData->m_nodeIndex == nodeIndex)
            {
                currentNode->m_side = temp;
                currentNode = temp;
                i++;
                if(i < size)
                {
                    nodeData = m_allBsmNodeData.at(i);
                    temp = new QBsmNode(nodeData);
                }
                else
                {
                    return;
                }
            }
            currentNode = saveCurrentNode;
        }

        nodeIndex = nodeData->m_nodeIndex;
        currentNode->m_next = temp;
        currentNode = temp;
        i++;
    }
}

void QParseBsm::createNodeFile(int timeOffset)
{
    int size = this->m_allBsmNodeData.size();
    QiGoBeatNodePosition nodePosition;
    nodePosition.initPositionList(size);
    nodePosition.analysisPosition(this->m_root, m_bpm);

//    qDebug()<<"-----------node position-------------";
//    for(int i = 0; i < size; ++i)
//    {
//        qDebug()<<i<<nodePosition.m_positionList.at(i);
//    }
//    return ;

    QList<QiGoBeatNodeData> nodeDatas;
    for(int i = 0; i < size; ++i)
    {
        int position = nodePosition.m_positionList.at(i);

        if(position != -1) //-1表示找不到位置，或者太多过滤掉
        {
            QiGoBeatNodeData data1;
            const QBsmNodeData* data2 = this->m_allBsmNodeData.at(i);
            data1.m_nodeType = IGOBEATNODETYPE_NORMAL;
            if(data2->m_nodeContinueIndex > 0)
            {
                data1.m_nodeType = IGOBEATNODETYPE_LONG;
            }
            else if(data2->m_nodeContinueIndex < 0)
            {
                data1.m_nodeType = IGOBEATNODETYPE_REWARD;
            }

            data1.m_isIGoBeatPosition = true;
            data1.m_nodeIndex = this->m_nodeOffset + 1 + i;
            data1.m_nodePosition = position;
            data1.m_nodeStartTime = timeOffset + 1250*(data2->m_nodeIndex - m_nodeStartIndex)/m_bpm;
            data1.m_nodeEndTime = timeOffset + 1250*(data2->m_nodeIndex + data2->m_nodeContinueIndex - m_nodeStartIndex)/m_bpm;

            qDebug()<<data2->m_nodeIndex<<data1.m_nodeStartTime;

            nodeDatas.append(data1);
        }
    }

    QiGoBeatNodeFile nodeFile;
    nodeFile.createPListFile(this->m_filePath, nodeDatas);
}

void QParseBsm::createNodeFile2(int timeOffset)
{
    int size = this->m_allBsmNodeData.size();
    //QiGoBeatNodePosition nodePosition;
    //nodePosition.initPositionList(size);
    //nodePosition.analysisPosition(this->m_root, m_bpm);

//    qDebug()<<"-----------node position-------------";
//    for(int i = 0; i < size; ++i)
//    {
//        qDebug()<<i<<nodePosition.m_positionList.at(i);
//    }
//    return ;

    QList<QiGoBeatNodeData> nodeDatas;
    for(int i = 0; i < size; ++i)
    {
        //int position = nodePosition.m_positionList.at(i);

        //if(position != -1) //-1表示找不到位置，或者太多过滤掉
        {
            QiGoBeatNodeData data1;
            const QBsmNodeData* data2 = this->m_allBsmNodeData.at(i);
            data1.m_nodeType = IGOBEATNODETYPE_NORMAL;
            if(data2->m_nodeContinueIndex > 0)
            {
                data1.m_nodeType = IGOBEATNODETYPE_LONG;
            }
            else if(data2->m_nodeContinueIndex < 0)
            {
                data1.m_nodeType = IGOBEATNODETYPE_REWARD;
            }

            data1.m_isIGoBeatPosition = false;
            data1.m_nodeIndex = this->m_nodeOffset + 1 + i;
            data1.m_nodePosition = data2->m_nodePosition;
            data1.m_nodeExtraNumber = data2->m_nodeValue;
            data1.m_nodeStartTime = timeOffset + 1250*(data2->m_nodeIndex - m_nodeStartIndex)/m_bpm;
            data1.m_nodeEndTime = timeOffset + 1250*(data2->m_nodeIndex + data2->m_nodeContinueIndex - m_nodeStartIndex)/m_bpm;

            qDebug()<<data2->m_nodeIndex<<data1.m_nodeStartTime;

            nodeDatas.append(data1);
        }
    }

    QiGoBeatNodeFile nodeFile;
    nodeFile.createPListFile(this->m_filePath, nodeDatas);
}

