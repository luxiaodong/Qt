#ifndef QENCRYPTION_H
#define QENCRYPTION_H

#include <QString>

class QEncryption
{
public:
    QEncryption();

    bool codeFile(const QString &filePath);

    QString m_lastError;
};

#endif // QENCRYPTION_H
