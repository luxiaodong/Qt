#ifndef QPHPCODE_H
#define QPHPCODE_H

#include "qprotocoldata.h"
#include <QString>
#include <QList>
#include <QSettings>
#include <QFile>
#include <QTextStream>

class QPHPCode
{
public:
    QPHPCode();

public:
    bool createProtocolFunctionFile(QList<QProtocolData>&);

private:
    QString createProtocolFunction(QList<QProtocolData>&);
    QString createSingleProtocolFunction(QProtocolData&);
    QString createSingleProtocolFunctionArgs(QStringList);
    QString createSingleProtocolMessageArg(QStringList);
};

#endif // QPHPCODE_H
