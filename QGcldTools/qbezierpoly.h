#ifndef QBEZIERPOLY_H
#define QBEZIERPOLY_H

#include <QString>
#include <QStringList>

//Bezier多项式 从matlab打印出来的格式 转成 lua里的格式
class QBezierPoly
{
public:
    QBezierPoly();
    void parse(QString&);
    QString convert(QString&);
};

#endif // QBEZIERPOLY_H
