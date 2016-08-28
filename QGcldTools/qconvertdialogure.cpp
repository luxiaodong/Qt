#include "qconvertdialogure.h"
#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QDebug>

QString QSingleSpeak::print(int index, bool isLast)
{
    QString str;
    str += QString("    data.step[%1] = {};\n").arg(index);
    str += QString("    data.step[%1].type = %2;\n").arg(index).arg(m_type);

    if(m_pic.isEmpty() == true)
    {
        m_pic = "xiaoqian";
    }
    else if(m_pic == QString("kidnapper"))
    {
        m_pic = "res/ui/guide/huanying.png";
    }
    else if(m_pic == QString("player"))
    {
        m_pic = "player";
    }
    else
    {
        m_pic = QString("res/ui/guide/wujiang/%1.png").arg(m_pic);
    }

    str += QString("    data.step[%1].pic = \"%2\";\n").arg(index).arg(m_pic);

    //m_desc.remove("{0}");
    m_desc.replace("《br》", "\\n");

    while(1)
    {
        int index1 = m_desc.indexOf("《");

        if(index1 == -1)
        {
            break;
        }

        int index2 = m_desc.indexOf("》", index1 + 1);
        m_desc.remove(index1, index2 - index1 + 1);

    }
qDebug()<<m_desc;
    str += QString("    data.step[%1].desc = \"%2\";\n").arg(index).arg(m_desc);

    if(m_open.isEmpty() == false)
    {
        if(isLast == true)
        {
            str += QString("    data.open = \"%1\";\n").arg(m_open);
        }
        else
        {
            qDebug()<<m_open;
            Q_ASSERT(0);
        }
    }

    return str;
}

void QSingleDialogue::parseSingleSpeak(QString line)
{
    if(line.contains("type") && line.contains("pic") && line.contains("desc"))
    {
        QSingleSpeak* speak = new QSingleSpeak();

        int index = line.indexOf("type");
        int index1 = line.indexOf("\"", index);
        int index2 = line.indexOf("\"", index1 + 1);
        speak->m_type = line.mid(index1+1, index2 - index1 - 1);

        index = line.indexOf("pic");
        index1 = line.indexOf("\"", index);
        index2 = line.indexOf("\"", index1 + 1);
        speak->m_pic = line.mid(index1+1, index2 - index1 - 1);

        index = line.indexOf("desc");
        index1 = line.indexOf("\"", index);
        index2 = line.indexOf("\"", index1 + 1);
        speak->m_desc = line.mid(index1+1, index2 - index1 - 1);

        index = line.indexOf("open");
        if(index != -1)
        {
            index1 = line.indexOf("\"", index);
            index2 = line.indexOf("\"", index1 + 1);
            speak->m_open = line.mid(index1+1, index2 - index1 - 1);
        }

        this->m_speaks.append(speak);

        //qDebug()<<speak->m_type<<speak->m_pic<<speak->m_open<<speak->m_desc;
    }
    else
    {
        qDebug()<<"parseSingleSpeak:"<<line;
    }
}

QString QSingleDialogue::print()
{
    QString str;
    if(m_note.isEmpty() == false)
    {
        str += QString("--%1\n").arg(m_note);
    }

    str += QString("function roleDialogue.constant.dialogueFrame_%1()\n").arg(m_itemID);
    str += QString("    local data = {};\n");
    str += QString("    data.totalStep = %1;\n").arg(m_speaks.size());
    str += QString("    data.step = {};\n");

    for(int i=0; i < m_speaks.size(); ++i)
    {
        QSingleSpeak* speak = m_speaks.at(i);
        bool isLast = false;
        if(i == m_speaks.size() - 1)
        {
            isLast = true;
        }
        str += speak->print(i + 1, isLast);
    }

    str += QString("    return data;\n");
    str += QString("end\n");
    return str;
}

QConvertDialogure::QConvertDialogure()
{
}

void QConvertDialogure::parse(QString filePath)
{
    if(filePath.isEmpty() == true)
    {
        return ;
    }

    QFile file(filePath);

    if(file.open(QIODevice::ReadOnly) == false)
    {
        return ;
    }

    bool parse = false;
    m_dialogues.clear();
    QSingleDialogue* pSingle;
    QString note;
    QTextStream stream(&file);
    stream.setCodec("UTF-8");
    while(stream.atEnd() == false)
    {
        QString line = stream.readLine().trimmed();

        if( line.contains("<item")  == true)
        {
            int index = line.indexOf("id");
            int index1 = line.indexOf("\"", index);
            int index2 = line.indexOf("\"", index1 + 1);

            pSingle = new QSingleDialogue();
            QString item = line.mid(index1+1, index2 - index1 - 1);
            pSingle->m_itemID = item.replace("#","_");
            pSingle->m_note = note;
            parse = true;
            m_dialogues.append(pSingle);
            //qDebug()<<pSingle->m_itemID;
        }
        else if( line.contains("</item>")  == true)
        {
            parse = false;
            //qDebug()<<pSingle->print();
        }
        else if( line.contains("<speak") == true)
        {
            if(parse == true)
            {
                pSingle->parseSingleSpeak(line);
            }
        }
        else if( line.contains("<!--") == true)
        {
            note = line;
        }
    }

    file.close();

    QString newfilePath = filePath;
    newfilePath.replace("dialogue.xml", "constant2.lua");
    QFile file2(newfilePath);

    if(file2.open(QIODevice::WriteOnly) == false)
    {
        qDebug()<<"open failed";
        return ;
    }

    QTextStream stream2(&file2);
    stream2.setCodec("UTF-8");
    foreach(QSingleDialogue* dialogue, m_dialogues)
    {
        stream2<<dialogue->print()<<"\n";
    }
    file2.close();
    qDebug()<<"ok";
}

QString QConvertDialogure::parse_file(QString filePath)
{
    if(filePath.isEmpty() == true)
    {
        return "";
    }

    QFile file(filePath);

    if(file.open(QIODevice::ReadOnly) == false)
    {
        return "";
    }

    int index = 0;
    QString dialogString = "";
    QTextStream stream(&file);
    while(stream.atEnd() == false)
    {
        QString line = stream.readLine().trimmed();

        if( line.contains("<talk")  == true)
        {
            if(index == 0)
            {
                QString name = filePath.split('/').last();
                name = name.left(name.size() - 4);
                dialogString += QString("function dialogue.get_%1()\n").arg(name);
                dialogString += QString("    local data = {}\n");
                index++;
            }

            QStringList list = line.split('"');
            QString pos = list.at(1);
            QString pic = list.at(3);
            QString name = list.at(5);
            QString last = list.last();

            if( pic==QString("{0}") )
            {
                pic = QString("zhugong");
            }

            int i = last.indexOf('>');
            int j = last.indexOf('<');
            QString des = last.mid(i+1, j-i-1);
//qDebug()<<des;
            if(des.contains("style") == true)
            {
                des.remove("[/style]");
                des.remove(QRegExp("style\\d"));
                des.remove("[]");
            }
//qDebug()<<des;
            m_list.append(des);
            dialogString += QString("    data[%1] = {pos=%2,pic=\"%3\",name=\"%4\",w=language.get(%5)},\n").arg(index).arg(pos,pic,name).arg(m_list.count());
            index++;
        }
    }

    file.close();

    if(index != 0)
    {
        dialogString += QString("    return data\n");
        dialogString += QString("end\n");
    }

    //qDebug()<<dialogString;
    return dialogString;
}

void QConvertDialogure::parse_dir(QString filePath)
{
    QDir dir(filePath);
    QFileInfoList list = dir.entryInfoList(QDir::NoDotAndDotDot | QDir::Files);

    QString str = "";
    m_list.clear();
    foreach(QFileInfo fileInfo, list)
    {
        QString filePath = fileInfo.absoluteFilePath();
        QString temp = this->parse_file(filePath);
        if(temp.isEmpty() == false)
        {
           str += temp;
           str += "\n";
        }
    }

    qDebug()<<str;

    str = "";
    int i = 1;
    foreach(QString single, m_list)
    {
        str += QString(" [\"dialog_%1\"] = \"%2\",\n").arg(i).arg(single);
        i++;
    }

    qDebug()<<str;
}



