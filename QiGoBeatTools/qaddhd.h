#ifndef QADDHD_H
#define QADDHD_H

#include <QStringList>
#include <QString>

class QAddHd
{
public:
    QAddHd();
    bool addHdFile(const QString&);
public:
    QString m_lastError;
};

#endif // QADDHD_H
