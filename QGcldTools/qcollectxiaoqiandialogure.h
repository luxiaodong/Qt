#ifndef QCOLLECTXIAOQIANDIALOGURE_H
#define QCOLLECTXIAOQIANDIALOGURE_H

#include <QString>
#include <QStringList>

//将小倩的lua文本提取出来,方便多国语言管理
class QCollectXiaoqianDialogure
{
public:
    QCollectXiaoqianDialogure();
    QString convert(QString&);
    void parse(QString&);

public:
    QStringList m_list;
};

#endif // QCOLLECTXIAOQIANDIALOGURE_H
