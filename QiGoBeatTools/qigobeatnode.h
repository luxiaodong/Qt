#ifndef QIGOBEATNODE_H
#define QIGOBEATNODE_H

#include <QString>
#include <QStringList>
#include <QList>
#include <QFile>
#include <QTextStream>

enum IGOBEATNODETYPE
{
    IGOBEATNODETYPE_NORMAL = 0,
    IGOBEATNODETYPE_LONG,
    IGOBEATNODETYPE_REWARD
};

class QiGoBeatNodeData
{
public:
    QString iGoBeatNodeTemplate();
private:
    QString convertTimeFromIntToString(int);
    QString convertPositionFromIntToString(int);
    int     convertPositionFromIntToInt(int);
public:
    IGOBEATNODETYPE m_nodeType;
    int m_nodeIndex;
    int m_nodePosition;
    int m_nodeStartTime;
    int m_nodeEndTime;
    int m_nodeExtraNumber;

    bool m_isIGoBeatPosition;
};

class QiGoBeatNodeFile
{
public:
    QString plistHeader();
    void createPListFile(QString filePath, QList<QiGoBeatNodeData> &);
};

#endif // QIGOBEATNODEDATA_H
