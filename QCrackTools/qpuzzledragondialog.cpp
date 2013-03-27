#include "qpuzzledragondialog.h"
#include "ui_qpuzzledragondialog.h"
#include "qcatchbin.h"

#include <QFileDialog>
#include <QString>
#include <QFile>
#include <QDataStream>
#include <QImage>
#include <QDebug>

QPuzzleDragonDialog::QPuzzleDragonDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QPuzzleDragonDialog)
{
    ui->setupUi(this);
    connect(ui->split, SIGNAL(clicked()), this, SLOT(clickSplit()));
    connect(ui->createPng, SIGNAL(clicked()), this, SLOT(clickCatchPng()));

    m_data1_offset.clear();
    m_data1_length.clear();
    m_data2_offset.clear();
    m_data2_length.clear();
    m_data3_offset.clear();
    m_data3_length.clear();
}

QPuzzleDragonDialog::~QPuzzleDragonDialog()
{
    delete ui;
}

void QPuzzleDragonDialog::clickSplit()
{
    this->splitMonster();
}

void QPuzzleDragonDialog::splitMonster()
{
    QString filePath = QFileDialog::getOpenFileName(this);

    if(filePath.isEmpty() == true)
    {
        return ;
    }

    if(filePath.contains("DATA000.BIN") == true)
    {
        return this->parseBin000(filePath);
    }
    else if(filePath.contains("DATA001.BIN") == true)
    {
        return this->parseBin001(filePath);
    }
    else if(filePath.contains("DATA002.BIN") == true)
    {
        return this->parseBin002(filePath);
    }
    else if(filePath.contains("DATA003.BIN") == true)
    {
        return this->parseBin003(filePath);
    }
}

void QPuzzleDragonDialog::parseBin000(QString filePath)
{
    //bin1. 0040 - 1840.
    //bin2. 1840 - 7840.
    //bin3. 7840 - end.
    QFile file(filePath);
    if(file.open(QIODevice::ReadOnly) == false)
    {
        return ;
    }

    QDataStream stream(&file);
    stream.setByteOrder(QDataStream::LittleEndian);

    int bin1_offset = 0x40;
    int bin2_offset = 0x1840;
    int bin3_offset = 0x7840;

    int i = 0;

    int temp0 = 0;
    int temp1 = 0;
    int temp2 = 0;
    int temp3 = 0;
    while( stream.atEnd() == false)
    {
        stream>>temp0;
        stream>>temp1;
        stream>>temp2;
        stream>>temp3;
        if( i >= 0 && i < bin1_offset)
        {}
        else if(i >= bin1_offset && i < bin2_offset)
        {
            if(temp2 != 0 || temp3 != 0)
            {
                m_data1_offset.append(temp2);
                m_data1_length.append(temp3);
            }
        }
        else if( i >= bin2_offset && i < bin3_offset)
        {
            if(temp2 != 0 || temp3 != 0)
            {
                m_data2_offset.append(temp2);
                m_data2_length.append(temp3);
            }
        }
        else if( i >= bin3_offset)
        {
            if(temp2 != 0 || temp3 != 0)
            {
                m_data3_offset.append(temp2);
                m_data3_length.append(temp3);
            }
        }

        i += 16;
    }

    file.close();

    qDebug()<<m_data1_offset;
    qDebug()<<m_data1_length;
    qDebug()<<m_data2_offset;
    qDebug()<<m_data2_length;
    qDebug()<<m_data3_offset;
    qDebug()<<m_data3_length;
}

void QPuzzleDragonDialog::parseBin001(QString filePath)
{
    if(m_data1_offset.isEmpty() == true)
    {
        return ;
    }

    int size = m_data1_offset.size();
    qDebug()<<size;
    for(int i = 0; i < size; ++i)
    {
        int start = m_data1_offset.at(i);
        int end = start + m_data1_length.at(i);
        QString inFilePath = filePath;
        QString outFilePath = filePath;
        outFilePath.remove("DATA001.BIN");

        char buf[16];
        ::snprintf(buf, 16, "%03d.bc", i);
        QString tempStr = QString::fromUtf8(buf);

        outFilePath += tempStr;
        QCatchBin catchBin;
        catchBin.catchBin(start, end, inFilePath, outFilePath);
    }
}

void QPuzzleDragonDialog::parseBin002(QString filePath)
{
    if(m_data2_offset.isEmpty() == true)
    {
        return ;
    }

    //mons_%03d.bc
    int size = m_data2_offset.size();
    for(int i = 0; i < size; ++i)
    {
        int start = m_data2_offset.at(i);
        int end = start + m_data2_length.at(i);
        QString inFilePath = filePath;
        QString outFilePath = filePath;
        outFilePath.remove("DATA002.BIN");

        char buf[16];
        ::snprintf(buf, 16, "mons_%03d.bc", i - 1);
        QString tempStr = QString::fromUtf8(buf);

        outFilePath += tempStr;
        QCatchBin catchBin;
        catchBin.catchBin(start, end, inFilePath, outFilePath);
    }
}

void QPuzzleDragonDialog::parseBin003(QString filePath)
{
    if(m_data3_offset.isEmpty() == true)
    {
        return ;
    }

    int size = m_data3_offset.size();
    qDebug()<<size;
    for(int i = 0; i < size; ++i)
    {
        int start = m_data3_offset.at(i);
        int end = start + m_data3_length.at(i);
        QString inFilePath = filePath;
        QString outFilePath = filePath;
        outFilePath.remove("DATA003.BIN");

        char buf[16];
        ::snprintf(buf, 16, "%03d.wav", i);
        QString tempStr = QString::fromUtf8(buf);

        outFilePath += tempStr;
        QCatchBin catchBin;
        catchBin.catchBin(start, end, inFilePath, outFilePath);
    }
}

void QPuzzleDragonDialog::clickCatchPng()
{
    QStringList filePaths = QFileDialog::getOpenFileNames(this);
    if(filePaths.isEmpty() == true)
    {
        return ;
    }

    foreach(QString single, filePaths)
    {
        this->catchSinglePng(single);
    }
}

void QPuzzleDragonDialog::catchSinglePng(QString filePath)
{
    qDebug()<<filePath;
    QFile file(filePath);
    if(file.open(QIODevice::ReadOnly) == false)
    {
        qDebug()<<"read failed!";
        return ;
    }

    QDataStream stream(&file);
    stream.setByteOrder(QDataStream::LittleEndian);

    qint32 sign = 0;
    int count = 0;
    int temp = 0;

    stream>>sign;

    if(sign != 0x31584554)
    {
        qDebug()<<"unknow format!";
        return ;
    }

    stream>>count;
    stream>>temp;
    stream>>temp;
    int offset[count];
    qint16 width[count];
    qint16 height[count];
    qint16 type[count];
    char name[count][24];

    for(int i = 0; i < count; ++i)
    {
        stream>>offset[i];
        stream>>width[i];
        stream>>height[i];

        for(int j = 0; j < 24; ++j)
        {
            qint8 value;
            stream>>value;
            name[i][j] = (char)value;
        }

        type[i] = (width[i] & 0xf000)>>12;
        width[i] &= 0x0fff;

        qDebug()<<offset[i]<<type[i]<<width[i]<<height[i]<<name[i];
    }

    int index = 0x10 + count*0x20;

    int k = 0;
    while( k < 1)
    {
        if(index < offset[k])
        {
            qint8 value;
            stream>>value;
            index++;
            continue;
        }

        //int t = 0;
//        QImage images[24];
//        for(int t = 0; t < 24; ++t)
//        {
//            images[t] = QImage(width[k], height[k], QImage::Format_ARGB32);
//        }

        QImage image(width[k], height[k], QImage::Format_ARGB32);

        for(int j = 0; j < height[k]; ++j)
        {
            for(int i = 0; i < width[k]; ++i)
            {
                unsigned char v1 = 0;
                unsigned char v2 = 0;

                stream>>v1;
                stream>>v2;
                index += 2;

                if(type[k] == 4)
                {
                    v1 -= 0x0e;
                }

                unsigned char r = ((v1 & 0xF0)>>4)*0x11;
                unsigned char g = (v1 & 0x0F)*0x11;
                unsigned char b = ((v2 & 0xF0)>>4)*0x11;
                unsigned char a = (v2 & 0x0F)*0x11;

                QColor color[24];
                color[0] = QColor(r,g,b,a);
                color[1] = QColor(r,g,a,b);
                color[2] = QColor(r,b,g,a);
                color[3] = QColor(r,b,a,g);
                color[4] = QColor(r,a,b,g);
                color[5] = QColor(r,a,g,b);
                color[6] = QColor(g,r,b,a);
                color[7] = QColor(g,r,a,b);
                color[8] = QColor(g,b,r,a);
                color[9] = QColor(g,b,r,g);
                color[10] = QColor(g,a,b,r);
                color[11] = QColor(g,a,r,b);
                color[12] = QColor(b,g,r,a);
                color[13] = QColor(b,g,a,r);
                color[14] = QColor(b,r,g,a);
                color[15] = QColor(b,r,a,g);
                color[16] = QColor(b,a,r,g);
                color[17] = QColor(b,a,g,r);
                color[18] = QColor(a,g,b,r);
                color[19] = QColor(a,g,r,b);
                color[20] = QColor(a,b,g,r);
                color[21] = QColor(a,b,r,g);
                color[22] = QColor(a,r,b,g);
                color[23] = QColor(a,r,g,b);

                if(type[k] == 3)
                {
                    //QColor color(b,a,r,g);
                    image.setPixel(i,j, color[16].rgba());
                }
                else if(type[k] == 4)
                {
//                    for(int t = 0; t < 24; ++t)
//                    {
//                        images[t].setPixel(i,j,color[t].rgba());
//                    }

                    //QColor color(r,g,b,a);
                    //int row = j*4/512;
                    //int col = i*6/512;
                    //int colorIndex = row*6 + col;
                    //qDebug()<<row<<col<<colorIndex;
                    image.setPixel(i,j,color[16].rgba());
                }
                else if(type[k] == 2)
                {
                    //QColor color(r,g,a,b);
                    image.setPixel(i,j,color[16].rgba());
                }
            }
        }

//        for(int t = 0; t < 24; ++t)
//        {
//            QString newFilePath = filePath;
//            int tempIndex = filePath.lastIndexOf("/");
//            newFilePath = filePath.left(tempIndex) + QString("/%1_%2").arg(t).arg(name[k]);

//            qDebug()<<newFilePath;
//            if( images[t].save(newFilePath,"png") == false)
//            {
//                qDebug()<<"save failed!";
//            }
//        }

        QString newFilePath = filePath;
        int tempIndex = filePath.lastIndexOf("/");
        newFilePath = filePath.left(tempIndex) + QString("/%1").arg(name[k]);

        qDebug()<<newFilePath;
        if( image.save(newFilePath,"png") == false)
        {
            qDebug()<<"save failed!";
        }

        k++;
    }

    return ;
}

