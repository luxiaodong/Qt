#ifndef QCATCHBIN_H
#define QCATCHBIN_H

#include <QString>
#include <QDataStream>
#include <QFile>
#include <QByteArray>

class QCatchBin
{
public:
    QCatchBin();
    void setStartAndEnd(int start, int end);
    void setOutPutFileName(QString outPutFileName);
    bool catchBin(QString filePath);
    bool catchBin(int start, int end, QString inFilePath, QString outFilePath);

public:
    QString m_lastError;
private:
    QString m_outPutFileName;
    int m_start;
    int m_end;
};

#endif // QCATCHBIN_H
