#ifndef QBEZIERPOLY_H
#define QBEZIERPOLY_H

#include <QString>
#include <QStringList>

class QBezierPoly
{
public:
    QBezierPoly();
    void parse(QString&);
    QString convert(QString&);
};

#endif // QBEZIERPOLY_H
