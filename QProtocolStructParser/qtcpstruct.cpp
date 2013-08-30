#include "qtcpstruct.h"
#include <QDebug>

QSimpleTypeMap* QSimpleTypeMap::getInstance()
{
    static QSimpleTypeMap* _gSimpleTypeMap = 0;
    if(_gSimpleTypeMap == 0)
    {
        _gSimpleTypeMap = new QSimpleTypeMap();
    }

    return _gSimpleTypeMap;
}

QTcpStruct* QTcpStruct::getInstance()
{
    static QTcpStruct* _gTcpStruct = 0;
    if(_gTcpStruct == 0)
    {
        _gTcpStruct = new QTcpStruct();
        //_gTcpStruct->readHeaderFile();
    }

    return _gTcpStruct;
}

QSimpleTypeMap::QSimpleTypeMap()
{
    m_map.clear();
    m_map.insert(QString("BYTE"), 1);
    m_map.insert(QString("WORD"), 2);
    m_map.insert(QString("DWORD"), 4);
    m_map.insert(QString("LONG"), 4);
    m_map.insert(QString("LONGLONG"), 8);
    m_map.insert(QString("BOOL"), 4);
    m_map.insert(QString("TCHAR"), 1);
    m_map.insert(QString("int"), 4);
    m_map.insert(QString("char"), 1);
    m_map.insert(QString("bool"), 1);
    m_map.insert(QString("long"), 4);
    m_map.insert(QString("short"), 2);
    m_map.insert(QString("__int64"), 8);
}

int QSimpleTypeMap::length(QString key)
{
    int value = m_map.value(key, 0);
    return value;
}

bool QSimpleTypeMap::isSimple(QString key)
{
    int value = m_map.value(key, 0);

    if( value == 0 )
    {
        return false;
    }

    return true;
}

void QSingleVariable::parse(QString line)
{
    QStringList list = line.split(" ", QString::SkipEmptyParts);
    this->m_type = list.at(0).trimmed();

    if(this->m_type.isEmpty() == true)
    {
        foreach(QString s, list)
        {
            qDebug()<<s;
        }

        Q_ASSERT(false);
    }

    this->m_name = line;
    this->parseLength();
    this->m_isArray = m_arrayLength > 1 ? true : false;
    this->m_isSimple = QSimpleTypeMap::getInstance()->isSimple(m_type);
    this->m_length = QSimpleTypeMap::getInstance()->length(m_type);  //类型的长度
    this->m_singleStruct = 0;
    this->m_totalLength = 0;
}

void QSingleVariable::parseLength()
{
    QString line = this->m_name;
    qDebug()<<line;
    int index1 = line.indexOf(";");
    Q_ASSERT(index1 != -1);
    line = line.left(index1);
    m_arrayLength = 1;
    int count = line.count("[");
    int leftIndex = 0;
    int rightIndex = 0;
    for(int i = 0; i < count; ++i)
    {
        leftIndex = line.indexOf("[", rightIndex);
        rightIndex = line.indexOf("]", leftIndex);
        QString sub = line.mid(leftIndex + 1, rightIndex - leftIndex - 1);
        bool ok;
        int factor = sub.toInt(&ok);
        if(ok == false)
        {
            //尝试用宏替换试试,如果不行就报错
            if(QSimpleTypeMap::getInstance()->m_macros.contains(sub) == false)
            {
                qDebug()<<sub;
                Q_ASSERT(0);
            }

            factor = QSimpleTypeMap::getInstance()->m_macros.value(sub);
        }

        m_arrayLength *= factor;
    }
}

void QSingleVariable::reLinkStructIfNeed()
{
    if(this->m_isSimple == false)
    {
        QSingleStruct* singleStruct = QTcpStruct::getInstance()->m_map.value(this->m_type, 0);
        this->m_singleStruct = singleStruct;
    }
}

void QSingleVariable::catculateLength()
{
    if(this->m_isSimple == false)
    {
        qDebug()<<this->m_name<<this->m_type;
        Q_ASSERT(this->m_singleStruct != 0);
        this->m_singleStruct->catculateLength();
        this->m_length = m_singleStruct->m_length;
    }

    m_totalLength = m_length*m_arrayLength;
}

QSingleStruct::QSingleStruct()
{
    m_list.clear();
}

void QSingleStruct::catculateLength()
{
    m_length = 0;
    foreach(QSingleVariable *v, m_list)
    {
        v->catculateLength();
        m_length += v->m_totalLength;
    }

    if(m_length%4 != 0)
    {
        int len = m_length/4;
        m_length = (len + 1)*4;
    }
}

QTcpStruct::QTcpStruct()
{
    m_list.clear();
    m_map.clear();
}

void QTcpStruct::readHeaderFromPath(QList<QString> allPath)
{
    foreach(QString path, allPath)
    {
        this->readHeaderFromPath(path);
    }

    this->linkMissStruct();
    this->catculateLength();
}

void QTcpStruct::readHeaderFromPath(QString path)
{
    if(path.isEmpty() == true)
    {
        return ;
    }

    QFile file(path);
    if(file.open(QIODevice::ReadOnly) == false)
    {
        return ;
    }

    QTextStream stream(&file);
    stream.setCodec("gb2312");

    //create(QString name, QList<QString> &list)
    QString name;
    QStringList list;
    bool checkAppend = false;

    while(stream.atEnd() == false)
    {
        QString line = stream.readLine();
        line.replace(QString("\t"), QString(" "));
qDebug()<<line;
        if(line.isEmpty() == false)
        {
            QStringList sl = line.split(" ", QString::SkipEmptyParts);

            if(sl.first() != QString("//"))
            {
                if(line.contains("#define") == true)
                {
                    if(sl.size() >= 3)
                    {
                        QString s2 = sl.at(1);
                        QString s3 = sl.at(2);

                        bool ok = false;
                        int value = s3.toInt(&ok);
                        if(ok == true)
                        {
                            QSimpleTypeMap::getInstance()->m_macros.insert(s2, value);
                        }
                    }
                }
                else if(line.contains("struct") == true)
                {
                    if(sl.size() >= 2)
                    {
                        name = sl.at(1);
                        list.clear();
                        checkAppend = true;
                    }
                }
                else
                {
                    if(checkAppend == true)
                    {
                        if(line.contains("};") == true)
                        {
                            checkAppend = false;

                            QSingleStruct* s = this->create(name, list);
                            this->add(s);
                            this->add(s->m_name);
                        }
                        else
                        {
                            if(line.contains(";") == true)
                            {
                                list.append(line);
                            }
                        }
                    }
                }
            }
        }
    }

    qDebug()<<"ok";
}

void QTcpStruct::readHeaderFile()
{
    /*
        strcut testB
        {
            testA c[2];
            int b;
        };
    */


//    struct testA
//    {
//    char b[3];
//    int a;
//      };

//    struct testC
//    {
//      char b;
//      char c[4];
//      testA a;
//      };

//    testC c;
//    memset(&c, 0 , 12);
//    c.b = 'a';
//    c.a.a = 1;
//    c.a.b[0] = 'x';
//    c.a.b[1] = 'y';
//    c.a.b[2] = 'z';
//    c.c[0] = '1';
//    c.c[1] = '2';
//    c.c[2] = '3';

//    qint8* p = (qint8*)&c;
//    for(int i = 0; i < 12; ++i)
//    {
//        qDebug()<<p[i];
//    }

//    qDebug()<<sizeof(testA)<<sizeof(testC);

    QList<QString> list1;
    QList<QString> list2;
    QList<QString> list3;

    list1.append("char b;");
    list1.append("char c;");
    list1.append("testA a;");

    list2.append("char b[3];");
    list2.append("int a;");

    list3.append("testA c[2];");
    list3.append("int b;");

    QSingleStruct* s1 = this->create("testC", list1);
    QSingleStruct* s2 = this->create("testA", list2);
    QSingleStruct* s3 = this->create("testB", list3);

    this->add(s1);
    this->add(s2);
    this->add(s3);

    this->linkMissStruct();
    this->catculateLength();
}

QSingleStruct* QTcpStruct::create(QString name, QList<QString> &list)
{
    QSingleStruct* singleStruct = new QSingleStruct();
    singleStruct->m_name = name;
    singleStruct->m_list.clear();

    foreach(QString key, list)
    {
        QSingleVariable* variable = this->create(key);
        singleStruct->m_list.append(variable);
    }

    singleStruct->m_length = 0;

    return singleStruct;
}

QSingleVariable* QTcpStruct::create(QString line)
{
    QSingleVariable* variable = new QSingleVariable();
    variable->parse(line);
    return variable;
}

void QTcpStruct::add(QString name)
{
    m_list.append(name);
}

void QTcpStruct::add(QSingleStruct* single)
{
    m_map.insert(single->m_name, single);
}

void QTcpStruct::linkMissStruct()
{
    QList<QString> keys = m_map.keys();
    foreach(QString key, keys)
    {
        QSingleStruct* singleStruct = m_map.value(key);
        foreach(QSingleVariable* v, singleStruct->m_list)
        {
            v->reLinkStructIfNeed();
        }
    }
}

void QTcpStruct::catculateLength()
{
    QList<QString> keys = m_map.keys();
    foreach(QString key, keys)
    {
        QSingleStruct* singleStruct = m_map.value(key);
        singleStruct->catculateLength();
    }
}

