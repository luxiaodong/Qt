#ifndef QUTF8CONVERT_H
#define QUTF8CONVERT_H

#include <QDataStream>
#include <QDir>
#include <QStringList>

class QUtf8Convert
{
public:
    QUtf8Convert();

public:
    bool convert(QString fileDir);
    void traverseFiles(QString dirPath);
    bool checkBOM(QString filePath);

    bool decode(QString filePath);

private:
    QStringList m_list;
};

#endif // QUTF8CONVERT_H
