#include "qphpcode.h"

QPHPCode::QPHPCode()
{
}

bool QPHPCode::createProtocolFunctionFile(QList<QProtocolData>& datas)
{
    QSettings settings("yuguosoft");
    QString path = settings.value("easyCode_codePath", QString()).toString();

    if(path.isEmpty() == true)
    {
        return false;
    }

    QString filePath = path + QString("/sendMessageToClient.php");
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
                list.insert(i, this->createProtocolFunction(datas));
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

QString QPHPCode::createProtocolFunction(QList<QProtocolData>& datas)
{
    QString str = QString();
    foreach(QProtocolData data, datas)
    {
        str += this->createSingleProtocolFunction(data);
    }
    return str;
}

QString QPHPCode::createSingleProtocolFunction(QProtocolData& data)
{
    QString str = QString("function Send%1ToClient(%2)\n").arg(data.upperHeadLetter(data.functionName()),this->createSingleProtocolFunctionArgs(data.args()));
    str += QString("{\n");
    str += QString("    $servermessage['protocol'] = %1;\n").arg(data.protocolId());
    str += this->createSingleProtocolMessageArg(data.args());
    str += QString("    $newjson = json_encode($servermessage);\n");
    str += QString("    echo $newjson;\n");
    str += QString("    ob_flush();\n");
    str += QString("    flush();\n");
    str += QString("}\n");
    return str;
}

QString QPHPCode::createSingleProtocolFunctionArgs(QStringList list)
{
    QString str = QString("");
    foreach(QString single, list)
    {
        QString variableName = single.split(" ").last();
        str += QString("$%1,").arg(variableName);
    }

    if(str.isEmpty() == false)
    {
        str = str.left(str.length() - 1);
    }

    return str;
}

QString QPHPCode::createSingleProtocolMessageArg(QStringList list)
{
    QString str = QString("");
    foreach(QString single, list)
    {
        QString variableName = single.split(" ").last();
        str += QString("    servermessage['%1'] = $%2;\n").arg(variableName,variableName);
    }

    return str;
}

