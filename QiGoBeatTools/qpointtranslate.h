#ifndef QPOINTTRANSLATE_H
#define QPOINTTRANSLATE_H

#include <QStringList>
#include <QString>

class QPointTranslate
{
public:
    QPointTranslate();

    bool translateFiles(QStringList &list);
    bool translateFile(const QString &filePath);
private:
    bool translateXmlFile(const QString &filePath);
    bool translatePlistFile(const QString &filePath);

public:
    QString m_lastError;
};

#endif // QPOINTTRANSLATE_H
