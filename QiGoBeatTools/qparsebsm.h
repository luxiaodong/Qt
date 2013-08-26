#ifndef QPARSEBSM_H
#define QPARSEBSM_H

#include <QString>
#include <QStringList>
#include <QList>

#include "qbsmnode.h"
#include "qigobeatnode.h"
#include "qigobeatnodeposition.h"

class QParseBsm
{
public:
    QParseBsm();
    void debugTest();
    void praseBsmFile(QString filePath);
    void parseLine(QString line);
    void createBsmNodeTree();
    void createNodeFile(int timeOffset);
    void createNodeFile2(int timeOffset);

public:
    QList<const QBsmNodeData*> m_allBsmNodeData;
    QBsmNode* m_root;

    float   m_bpm;
    int     m_nodeOffset;//5000的那个数值
    int     m_nodeStartIndex;//歌曲的总体偏移量
    QString m_filePath;
    bool    m_isLongTag[10];
    int     m_longStartIndex[10];
};

#endif // QPARSEBSM_H
