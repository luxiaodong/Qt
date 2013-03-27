#ifndef QPARSECCBFILE_H
#define QPARSECCBFILE_H

#include <QStringList>
#include "qcocos2ddata.h"

class QParseCCBFile
{
public:
    QParseCCBFile();
    bool parseCCBFile(QString filePath);

private:
    bool readCCBFile(QString filePath);
    bool parseCCBFile();
    int  indexOf(QString key, int offset = 0);
    int  indexOfPair(QString key, int startLineIndex);
    QString dataOfLine(QString line);
    QCocos2dCCNode* parseCocos2dClass(int startLineIndex, int endLineIndex);
    QCocos2dCCNode* createCocos2dClass(QString key);
    void parseCocos2dProperties(QCocos2dCCNode* node, int startLineIndex, int endLineIndex);

private:
    void print(QCocos2dCCNode* node);

private:
    QStringList m_lines;
    QCocos2dCCNode* m_root;
};

#endif // QPARSECCBFILE_H
