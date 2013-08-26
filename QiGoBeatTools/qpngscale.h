#ifndef QPNGSCALE_H
#define QPNGSCALE_H

#include <QStringList>
#include <QString>

class QPngScale
{
public:
    QPngScale();
    bool scale(const QString&);

public:
    QString m_lastError;
    double m_scale;
};

#endif // QPNGSCALE_H
