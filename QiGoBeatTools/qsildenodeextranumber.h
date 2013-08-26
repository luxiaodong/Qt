#ifndef QSILDENODEEXTRANUMBER_H
#define QSILDENODEEXTRANUMBER_H

#include <QStringList>
#include <QString>

class QSildeNodeExtraNumber
{
public:
    QSildeNodeExtraNumber();
    bool parseNodes(const QString&);

private:
    void addSlideTag();
    void replaceExtraNumber(int line, int index);

public:
    QString m_lastError;
    QStringList m_lines;
};

#endif // QSILDENODEEXTRANUMBER_H
