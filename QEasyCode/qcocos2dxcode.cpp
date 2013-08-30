#include "qcocos2dxcode.h"

QCocos2dxCode::QCocos2dxCode()
{
}

bool QCocos2dxCode::createProtocolEnumFile(QList<QProtocolData>& datas)
{
    QSettings settings("yuguosoft");
    QString path = settings.value("easyCode_codePath", QString()).toString();

    if(path.isEmpty() == true)
    {
        return false;
    }

    QString filePath = path + QString("/protocol.h");
    QFile file(filePath);
    if( file.open(QIODevice::ReadOnly) == false)
    {
        return false;
    }

    QStringList list;
    QTextStream out(&file);
    while(out.atEnd() == false)
    {
        list.append(out.readLine());
    }

    file.close();

    if( file.open(QIODevice::WriteOnly) == false)
    {
        return false;
    }

    int size = list.size();
    bool bRemove = false;
    int i = 0;
    while(i < size)
    {
        QString line = list.at(i);

        if(line.contains(QString("//easyCode_Start")) == true)
        {
            bRemove = true;
            i++;
            line = list.at(i);
        }

        if(bRemove == true)
        {
            if(line.contains("//easyCode_End") == true)
            {
                list.insert(i, this->createProtocolEnum(datas));
                bRemove = false;
                break;
            }
        }

        if(bRemove == true)
        {
            list.removeAt(i);
            size--;
        }
        else
        {
            i++;
        }
    }

    QTextStream in(&file);
    in.setCodec("UTF-8");
    foreach(QString line, list)
    {
        in<<line<<"\n";
    }

    file.close();
    return true;
}

QString QCocos2dxCode::createProtocolEnum(QList<QProtocolData>& datas)
{
    QString str = QString("enum PROTOCOL\n");
    str += QString("{\n");
    foreach (QProtocolData data, datas)
    {
        str += this->createSingleEnum(data);
    }
    str += QString("};");
    return str;
}

QString QCocos2dxCode::createSingleEnum(QProtocolData& data)
{
    QString des = data.des().replace("\n"," ");
    QString str =  QString("    %1 = %2, //%3\n").arg(data.protocolEnumString()).arg(data.protocolId()).arg(des);
    return str;
}


bool QCocos2dxCode::createProtocolVariableInServiceDataHFile(QList<QProtocolData>& datas)
{
    QSettings settings("yuguosoft");
    QString path = settings.value("easyCode_codePath", QString()).toString();

    if(path.isEmpty() == true)
    {
        return false;
    }

    QString filePath = path + QString("/GServiceData.h");
    QFile file(filePath);
    if( file.open(QIODevice::ReadOnly) == false)
    {
        return false;
    }

    QStringList list;
    QTextStream out(&file);
    while(out.atEnd() == false)
    {
        list.append(out.readLine());
    }

    file.close();

    if( file.open(QIODevice::WriteOnly) == false)
    {
        return false;
    }

    int size = list.size();
    bool bRemove = false;
    int i = 0;
    while(i < size)
    {
        QString line = list.at(i);

        if(line.contains(QString("//easyCode_Start")) == true)
        {
            bRemove = true;
            i++;
            line = list.at(i);
        }

        if(bRemove == true)
        {
            if(line.contains("//easyCode_End") == true)
            {
                list.insert(i, this->createProtocolVariable(datas));
                bRemove = false;
                break;
            }
        }

        if(bRemove == true)
        {
            list.removeAt(i);
            size--;
        }
        else
        {
            i++;
        }
    }

    QTextStream in(&file);
    in.setCodec("UTF-8");
    foreach(QString line, list)
    {
        in<<line<<"\n";
    }

    file.close();
    return true;
}

QString QCocos2dxCode::createProtocolVariable(QList<QProtocolData>& datas)
{
    QString str = QString("public:\n");
    foreach (QProtocolData data, datas)
    {
        int protocolId = data.protocolId();
        if(protocolId%2 == 0)
        {
            str += QString("    bool %1;\n").arg(this->createSingleProtocolVariable(data));
        }
    }
    return str;
}

QString QCocos2dxCode::createSingleProtocolVariable(QProtocolData& data)
{
    QString str =  QString("m_is");
    QString variable = data.upperHeadLetter(data.functionName());
    str += QString("%1Changed").arg(variable);
    return str;
}


bool QCocos2dxCode::createProtocolVariableInServiceDataCppFile(QList<QProtocolData>& datas)
{
    QSettings settings("yuguosoft");
    QString path = settings.value("easyCode_codePath", QString()).toString();

    if(path.isEmpty() == true)
    {
        return false;
    }

    QString filePath = path + QString("/GServiceData.cpp");
    QFile file(filePath);
    if( file.open(QIODevice::ReadOnly) == false)
    {
        return false;
    }

    QStringList list;
    QTextStream out(&file);
    while(out.atEnd() == false)
    {
        list.append(out.readLine());
    }

    file.close();

    if( file.open(QIODevice::WriteOnly) == false)
    {
        return false;
    }

    int size = list.size();
    bool bRemove = false;
    int i = 0;
    while(i < size)
    {
        QString line = list.at(i);

        if(line.contains(QString("//easyCode_Start")) == true)
        {
            bRemove = true;
            i++;
            line = list.at(i);
        }

        if(bRemove == true)
        {
            if(line.contains("//easyCode_End") == true)
            {
                list.insert(i, this->createProtocolVariableAssign(datas));
                bRemove = false;
                break;
            }
        }

        if(bRemove == true)
        {
            list.removeAt(i);
            size--;
        }
        else
        {
            i++;
        }
    }

    QTextStream in(&file);
    in.setCodec("UTF-8");
    foreach(QString line, list)
    {
        in<<line<<"\n";
    }

    file.close();
    return true;
}

QString QCocos2dxCode::createProtocolVariableAssign(QList<QProtocolData>& datas)
{
    QString str = QString("");
    foreach (QProtocolData data, datas)
    {
        int protocolId = data.protocolId();
        if(protocolId%2 == 0)
        {
            str += QString("    %1 = false;\n").arg(this->createSingleProtocolVariable(data));
        }
    }
    return str;
}


bool QCocos2dxCode::createProtocolFunctionInServiceHFile(QList<QProtocolData>& datas)
{
    QSettings settings("yuguosoft");
    QString path = settings.value("easyCode_codePath", QString()).toString();

    if(path.isEmpty() == true)
    {
        return false;
    }

    QString filePath = path + QString("/GService.h");
    QFile file(filePath);
    if( file.open(QIODevice::ReadOnly) == false)
    {
        return false;
    }

    QStringList list;
    QTextStream out(&file);
    while(out.atEnd() == false)
    {
        list.append(out.readLine());
    }

    file.close();

    if( file.open(QIODevice::WriteOnly) == false)
    {
        return false;
    }

    int size = list.size();
    bool bRemove = false;
    int i = 0;
    while(i < size)
    {
        QString line = list.at(i);

        if(line.contains(QString("//easyCode_Start")) == true)
        {
            bRemove = true;
            i++;
            line = list.at(i);
        }

        if(bRemove == true)
        {
            if(line.contains("//easyCode_End") == true)
            {
                list.insert(i, this->createProtocolFunctionInServiceH(datas));
                bRemove = false;
                break;
            }
        }

        if(bRemove == true)
        {
            list.removeAt(i);
            size--;
        }
        else
        {
            i++;
        }
    }

    QTextStream in(&file);
    in.setCodec("UTF-8");
    foreach(QString line, list)
    {
        in<<line<<"\n";
    }

    file.close();
    return true;
}

QString QCocos2dxCode::createProtocolFunctionInServiceH(QList<QProtocolData>& datas)
{
    QString str = QString("public:\n");
    foreach(QProtocolData data, datas)
    {
        str +=  QString("   void %1;\n").arg(this->createSingleProtocolFunctionHead(data));
    }
    return str;
}

QString QCocos2dxCode::createSingleProtocolFunctionHead(QProtocolData& data)
{
    int protocolId = data.protocolId();
    QString prefix = QString("request");

    if(protocolId%2 == 0)
    {
        prefix = QString("receive");
    }

    QString str =  QString("%1%2(%3)").arg(prefix, data.upperHeadLetter(data.functionName()), this->createSingleProtocolFunctionArgs(data.args()));

    if(protocolId%2 == 0)
    {
        str.replace("const char*", "std::string");
    }

    return str;
}

QString QCocos2dxCode::createSingleProtocolFunctionArgs(QStringList list)
{
    QString str = QString("");
    foreach(QString single, list)
    {
        QString variableType = single.split(" ").first();
        QString variableName = single.split(" ").last();

        if(variableType == QString("string"))
        {
            variableType = QString("const char*");
        }

        str += QString("%1 %2,").arg(variableType, variableName);
    }

    if(str.isEmpty() == false)
    {
        str = str.left(str.length() - 1);
    }

    return str;
}


bool QCocos2dxCode::createProtocolFunctionInServiceCppFile(QList<QProtocolData>& datas)
{
    QSettings settings("yuguosoft");
    QString path = settings.value("easyCode_codePath", QString()).toString();

    if(path.isEmpty() == true)
    {
        return false;
    }

    QString filePath = path + QString("/GService.cpp");
    QFile file(filePath);
    if( file.open(QIODevice::ReadOnly) == false)
    {
        return false;
    }

    QStringList list;
    QTextStream out(&file);
    while(out.atEnd() == false)
    {
        list.append(out.readLine());
    }

    file.close();

    if( file.open(QIODevice::WriteOnly) == false)
    {
        return false;
    }

    int size = list.size();
    bool bRemove = false;
    int i = 0;
    while(i < size)
    {
        QString line = list.at(i);

        if(line.contains(QString("//easyCode_Start")) == true)
        {
            bRemove = true;
            i++;
            line = list.at(i);
        }

        if(bRemove == true)
        {
            if(line.contains("//easyCode_End") == true)
            {
                list.insert(i, this->createProtocolFunctionInServiceCpp(datas));
                list.insert(i, this->createFunctionParseServiceMessage(datas));
                bRemove = false;
                break;
            }
        }

        if(bRemove == true)
        {
            list.removeAt(i);
            size--;
        }
        else
        {
            i++;
        }
    }

    QTextStream in(&file);
    in.setCodec("UTF-8");
    foreach(QString line, list)
    {
        in<<line<<"\n";
    }

    file.close();
    return true;
}

QString QCocos2dxCode::createProtocolFunctionInServiceCpp(QList<QProtocolData>& datas)
{
    QString str = QString();
    foreach(QProtocolData data, datas)
    {
        str += createSingleProtocolFunctionBody(data);
        str += "\n";
    }
    return str;
}

QString QCocos2dxCode::createSingleProtocolFunctionBody(QProtocolData& data)
{
    int protocolId = data.protocolId();
    QString str = QString("void GService::%1\n").arg(this->createSingleProtocolFunctionHead(data));
    str += QString("{\n");

    if(protocolId%2 == 1)
    {
        str += QString("    json_object* obj = json_object_new_object();\n");
        str += QString("    json_object_object_add(obj, \"protocol\", json_object_new_int((int)%1));\n").arg(data.protocolEnumString());
        str += this->createSingleProtocolFunctionJsonArgs(data.args());
        str += QString("    this->sendMessageToServerBlocking(obj);\n");
        str += QString("    json_object_put(obj);\n");
    }
    else
    {
        str += QString("    m_serviceData->%1 = true;\n").arg(this->createSingleProtocolVariable(data));
    }

    str += QString("}\n");
    return str;
}

QString QCocos2dxCode::createSingleProtocolFunctionJsonArgs(QStringList list)
{
    QString str = QString("");
    foreach(QString single, list)
    {
        QString variableType = single.split(" ").first();
        QString variableName = single.split(" ").last();

        if(variableType == QString("bool"))
        {
            variableType = QString("boolean");
        }

        str += QString("    json_object_object_add(obj, \"%1\", json_object_new_%2(%3));\n").arg(variableName,variableType,variableName);
    }

    return str;
}


QString QCocos2dxCode::createFunctionParseServiceMessage(QList<QProtocolData>& datas)
{
    QString str = QString("bool GService::parseServiceMessage(std::string msg)\n");
    str += QString("{\n");
    str += QString("    int value = GParserJson::getInstance()->intValueFormStringWithKey(msg, \"protocol\");\n");
    str += QString("    CC_ASSERT(value%2 == 0);\n");
    str += QString("    PROTOCOL pId = (PROTOCOL)value;\n");
    str += QString("    switch(pId)\n");
    str += QString("    {\n");

    foreach(QProtocolData data, datas)
    {
        if(data.protocolId()%2 == 0)
        {
            str += this->createSingleMessageCase(data);
        }
    }

    str += QString("        default:\n");
    str += QString("            break;\n");
    str += QString("    }\n");
    str += QString("    return false;\n");
    str += QString("}\n");
    return str;

    return QString();
}

QString QCocos2dxCode::createSingleMessageCase(QProtocolData& data)
{
    QString str = QString();
    str += QString("        case %1:\n").arg(data.protocolEnumString());
    str += QString("            {\n");
    str += this->createSingleMessageCaseArgs(data.args());
    str += this->createSingleMessageCall(data);
    str += QString("                return true;\n");
    str += QString("            }\n");
    str += QString("            break;\n");
    return str;
}

QString QCocos2dxCode::createSingleMessageCaseArgs(QStringList list)
{
    QString str = QString("");
    foreach(QString single, list)
    {
        QString variableType = single.split(" ").first();
        QString variableName = single.split(" ").last();
        QString functionPrefix = variableType;

        if(variableType == QString("string"))
        {
            variableType = QString("std::string");
        }

        str += QString("                %1 %2 = GParserJson::getInstance()->%3ValueFormStringWithKey(msg,\"%4\");\n").arg(variableType,variableName,functionPrefix,variableName);
    }

    return str;
}

QString QCocos2dxCode::createSingleMessageCall(QProtocolData& data)
{
    QStringList list = data.args();
    QString arg = QString("");
    foreach(QString single, list)
    {
        arg += single.split(" ").last();
        arg += QString(",");
    }

    if(arg.isEmpty() == false)
    {
        arg = arg.left(arg.size() - 1);
    }

    QString str = QString("                this->receive%1(%2)\n").arg(data.upperHeadLetter(data.functionName()), arg);
    return str;
}

