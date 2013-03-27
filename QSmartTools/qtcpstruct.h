#ifndef QTCPSTRUCT_H
#define QTCPSTRUCT_H

#include <QString>
#include <QList>
#include <QMap>
#include <QFile>
#include <QTextStream>
#include <QStringList>

//一张基础数据类型占用多少字节的map
class QSimpleTypeMap
{
public:
    static QSimpleTypeMap* getInstance();
    QSimpleTypeMap();
    bool isSimple(QString key);
    int length(QString key);

public:
    QMap<QString, int> m_map;
    QMap<QString, int> m_macros;
};

class QSingleStruct;

class QAbstractVariable
{};

class QSingleVariable : public QAbstractVariable
{
public:
    void parse(QString line);
    void reLinkStructIfNeed(); //重新连接数据结构
    void parseLength();
    void catculateLength();
public:
    bool m_isSimple; //是否是原始的变量,否则则是复合变量
    bool m_isArray;  //是否是数组
    QString m_type; //int , char , bool那种, 也可能是数据结构名称
    QString m_name; //变量名称,显示用
    int m_length; //变量占用的字节长度
    int m_arrayLength; //如果是数组, 那么长度是多少
    int m_totalLength;
    QSingleStruct* m_singleStruct;
};

class QSingleStruct : public QAbstractVariable
{
public:
    QSingleStruct();
    void catculateLength();
public:
    QString m_name;
    int m_length; //区别于内部变量的总和,向上4取整数
    QList<QSingleVariable*> m_list;
};

class QTcpStruct
{
public:
    static QTcpStruct* getInstance();
    QTcpStruct();
    QSingleVariable* create(QString line);
    QSingleStruct* create(QString name, QList<QString> &list);

    void readHeaderFile();
    void readHeaderFromPath(QList<QString> allPath);
    void readHeaderFromPath(QString path);
    void add(QString name);
    void add(QSingleStruct* single);
    void linkMissStruct();
    void catculateLength();
public:
    QList<QString> m_list;
    QMap<QString, QSingleStruct*> m_map;
};

#endif // QTCPSTRUCT_H
