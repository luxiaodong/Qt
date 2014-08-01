#ifndef QCUBEPOLY_H
#define QCUBEPOLY_H

#include <QString>

//将matlab生成的多项式,转成lua格式,该多项式主要计算f(x)
class QCubePoly
{
public:
    QCubePoly();
    QString convert(QString&,QString&);
    void parse(QString&);   //for world.
    QString convert2(QString&,QString&);
    void parse2(QString&);  //for juben.

private:
    QString m_name;
};

#endif // QCUBEPOLY_H
