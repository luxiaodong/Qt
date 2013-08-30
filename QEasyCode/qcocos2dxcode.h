#ifndef QCOCOS2DXCODE_H
#define QCOCOS2DXCODE_H

#include "qprotocoldata.h"
#include <QString>
#include <QList>
#include <QSettings>
#include <QFile>
#include <QTextStream>

class QCocos2dxCode
{
public:
    QCocos2dxCode();

public:
    bool createProtocolEnumFile(QList<QProtocolData>&);
    bool createProtocolVariableInServiceDataHFile(QList<QProtocolData>&);
    bool createProtocolVariableInServiceDataCppFile(QList<QProtocolData>&);
    bool createProtocolFunctionInServiceHFile(QList<QProtocolData>&);
    bool createProtocolFunctionInServiceCppFile(QList<QProtocolData>&);

private:
    QString createProtocolEnum(QList<QProtocolData>&);
    QString createSingleEnum(QProtocolData&);

private:
    QString createProtocolVariable(QList<QProtocolData>&);
    QString createSingleProtocolVariable(QProtocolData&);

private:
    QString createProtocolVariableAssign(QList<QProtocolData>&);

private:
    QString createProtocolFunctionInServiceH(QList<QProtocolData>&);
    QString createSingleProtocolFunctionHead(QProtocolData&);
    QString createSingleProtocolFunctionArgs(QStringList);

private:
    QString createProtocolFunctionInServiceCpp(QList<QProtocolData>&);
    QString createSingleProtocolFunctionBody(QProtocolData&);
    QString createSingleProtocolFunctionJsonArgs(QStringList);

private:
    QString createFunctionParseServiceMessage(QList<QProtocolData>&);
    QString createSingleMessageCase(QProtocolData&);
    QString createSingleMessageCaseArgs(QStringList);
    QString createSingleMessageCall(QProtocolData&);
};

#endif // QCOCOS2DXCODE_H
