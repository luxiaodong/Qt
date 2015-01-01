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

    void rc4Init();

    bool rc4(QString filePath);

    void swap(int i, int j);
    unsigned char getNextKey();


private:
    QStringList m_list;
    unsigned char s[256];
    unsigned char m_rc4_index_i;
    unsigned char m_rc4_index_j;
};

#endif // QUTF8CONVERT_H
