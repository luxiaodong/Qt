#include "qutf8convert.h"
#include <QDebug>

QUtf8Convert::QUtf8Convert()
{
}

bool QUtf8Convert::convert(QString dirPath)
{
    m_list.clear();
    this->traverseFiles(dirPath);

    foreach (QString single, m_list)
    {
        if(this->checkBOM(single) == true)
        {
            qDebug()<<single;
        }
    }

    return false;
}

bool QUtf8Convert::checkBOM(QString filePath)
{
    if(filePath.isEmpty() == true)
    {
        return false;
    }

    QFile file(filePath);

    if(file.open(QIODevice::ReadOnly) == false)
    {
        return false;
    }

    QDataStream stream(&file);
    //EF BB BF
    unsigned char a1 = 0;
    unsigned char a2 = 0;
    unsigned char a3 = 0;

    unsigned char b1 = 239;
    unsigned char b2 = 187;
    unsigned char b3 = 191;

    stream>>a1;
    stream>>a2;
    stream>>a3;

    //qDebug()<<a1<<a2<<a3;

    bool result = false;
    if( a1 == b1 && a2 == b2 && a3 == b3 )
    {
        result = true;

        QString filePath2 = filePath + QString(".lua");
        QFile file2(filePath2);

        if(file2.open(QIODevice::WriteOnly) == false)
        {
            return false;
        }

        QDataStream stream2(&file2);

        while(stream.atEnd() == false)
        {
            unsigned char t;
            stream>>t;
            stream2<<t;
        }

        file2.close();
    }

    file.close();
    return result;
}

void QUtf8Convert::traverseFiles(QString dirPath)
{
    QDir dir(dirPath);

    QFileInfoList fileInfoList = dir.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot);

    foreach(QFileInfo fileInfo, fileInfoList)
    {
        if(fileInfo.exists())
        {
            if(fileInfo.isDir())
            {
                this->traverseFiles(fileInfo.absoluteFilePath());
            }
            else if(fileInfo.isFile())
            {
                QString filePath = fileInfo.absoluteFilePath();
                if(filePath.right(4) == QString(".lua"))
                {
                    m_list.append(filePath);
                }
            }
        }
    }
}


bool QUtf8Convert::decode(QString filePath)
{
    if(filePath.isEmpty() == true)
    {
        return false;
    }

    QFile file(filePath);

    if(file.open(QIODevice::ReadOnly) == false)
    {
        return false;
    }

    QString filePath2 = filePath.remove(".lua");
    QFile file2(filePath2);

    if(file2.open(QIODevice::WriteOnly) == false)
    {
        return false;
    }

    QDataStream stream(&file);
    QDataStream stream2(&file2);

    while(stream.atEnd() == false)
    {
        unsigned char t;
        stream>>t;
        t = t ^ 0xFF;
        stream2<<t;
    }

    file2.close();
    file.close();
    return true;
}

void QUtf8Convert::rc4Init()
{
    unsigned char key[] = "MOV AL ,DH";
    //unsigned char key[] = {5,6,7,5,6,7,5,6};
    int arraySize = 256;
    int keysize = sizeof(key);

    int count[256];
    for(int i = 0; i < arraySize; ++i)
    {
        s[i] = i;
        count[i] = 0;
    }

    int j = 0;
    for(int i = 0; i < arraySize; ++i)
    {
        j = (j + s[i] + key[ i%keysize] )%arraySize;
        this->swap(i, j);
    }

    m_rc4_index_i = 0;
    m_rc4_index_j = 0;

//    for(int i = 0; i < arraySize; ++i)
//    {
//        //qDebug()<<s[i];
//        count[ s[i] ]++;
//    }

//    for(int i = 0; i < arraySize; ++i)
//    {
//        qDebug()<<i<<count[i];
//    }

//    qDebug()<<t<<k;
}

void QUtf8Convert::swap(int i, int j)
{
    unsigned char temp = s[i];
    s[i] = s[j];
    s[j] = temp;
}

unsigned char QUtf8Convert::getNextKey()
{
    int arraySize = 256;
    m_rc4_index_i = (m_rc4_index_i + 1)%arraySize;
    m_rc4_index_j = (m_rc4_index_j + s[m_rc4_index_i])%arraySize;

    this->swap(m_rc4_index_i, m_rc4_index_j);

    unsigned char t = (s[m_rc4_index_i] + s[m_rc4_index_j])%arraySize;
    unsigned char k = s[t];
    return k;
}

bool QUtf8Convert::rc4(QString filePath)
{
//    this->rc4Init();
//    return false;

    if(filePath.isEmpty() == true)
    {
        return false;
    }

    QFile file(filePath);

    if(file.open(QIODevice::ReadOnly) == false)
    {
        return false;
    }

    QString filePath2 = filePath.remove(".lua");
    QFile file2(filePath2);

    if(file2.open(QIODevice::WriteOnly) == false)
    {
        return false;
    }

    QDataStream stream(&file);
    QDataStream stream2(&file2);

    this->rc4Init();
    int k = 0;
    while(stream.atEnd() == false)
    {
        unsigned char t;
        stream>>t;
        unsigned char g = this->getNextKey();
        qDebug()<<k<<t<<g<<m_rc4_index_i<<m_rc4_index_j;
        k++;
        t = t ^ g;
        stream2<<t;
    }

    qDebug()<<"the end.";

    file2.close();
    file.close();
    return true;
}





