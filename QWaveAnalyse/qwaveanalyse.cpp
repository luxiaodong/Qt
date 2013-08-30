#include "qwaveanalyse.h"

#include "decmpa/decodefile.cpp"

QWaveAnalyse::QWaveAnalyse(QWidget *parent)
    : QWidget(parent)
{
    m_isDoubleTrack = true;
    m_quantizationBit = 16;
    m_quantizationFrequency = 44100;
    m_fileName = QString();

    this->m_leftOriginWave.clear();
    this->m_rightOriginWave.clear();
    this->m_leftSlope.clear();
    this->m_rightSlope.clear();

    ptextEdit = new QTextEdit(this);
    ptextEdit->setGeometry(this->geometry());

//    this->m_leftOriginWave.clear();
//    this->m_rightOriginWave.clear();
//    this->m_leftPeakWave.clear();
//    this->m_rightPeakWave.clear();
//    this->m_leftAnalyseWave.clear();
//    this->m_rightAnalyseWave.clear();
}

QWaveAnalyse::~QWaveAnalyse()
{

}

void QWaveAnalyse::mousePressEvent(QMouseEvent *e)
{
    //ptextEdit->append("ssdfasdfasdf");
    this->process();
    ptextEdit->append("start play");
    //QMessageBox::information(this,"analyse","analyse Over!");
}

void QWaveAnalyse::process()
{
//    QElapsedTimer timer;
//    timer.start();
//    qDebug()<<"the elapsed time is "<<timer.elapsed()<<"ms";

    this->m_leftOriginWave.clear();
    this->m_rightOriginWave.clear();

    QString mp3Path = QFileDialog::getOpenFileName(this,
                                                     tr("Open File"),
                                                     QDir::currentPath(),
                                                     tr("*.mp3"));
    if(mp3Path.isEmpty())
    {
        ptextEdit->append("mp3 file path is empty!");
        return ;
    }

    if(this->mainDecode(mp3Path) == false)
    {
        ptextEdit->append("decode failed!");
        return ;
    }

//    QString pcmPath = mp3Path + QString(".pcm");
//    if( this->loadWaveData(pcmPath) == false )
//    {
//        qDebug()<<"load data failed!";
//        return ;
//    }

    qDebug()<<this->m_leftOriginWave.size();

    this->m_leftSlope.clear();
    this->m_rightSlope.clear();
    this->calculateSlope(&this->m_leftOriginWave, &this->m_leftSlope);

    if(this->m_isDoubleTrack == false)
    {
        this->calculateSlope(&this->m_rightOriginWave, &this->m_rightSlope);
    }

    //this->createPList(mp3Path);
    qDebug()<<this->m_leftSlope.size();

    Phonon::MediaObject *mediaObject = new Phonon::MediaObject(this);
    mediaObject->setCurrentSource(Phonon::MediaSource(mp3Path));
    Phonon::AudioOutput *audioOutput =
        new Phonon::AudioOutput(Phonon::MusicCategory, this);
    Phonon::Path path = Phonon::createPath(mediaObject, audioOutput);
    QTimer::singleShot(10*(this->m_leftSlope.at(0).x())/441, this, SLOT(timeBreak()));
    mediaObject->play();
    return ;
}

bool QWaveAnalyse::loadWaveData(QString pchPath)
{

//8位单声道   0声道   0声道
//8位立体声   0声道（左）   1声道（右）   0声道（左）   1声道（右）
//16位单声道   0声道低字节   0声道高字节   0声道低字节   0声道高字节
//16位立体声   0声道（左）低字节   0声道（左）高字节   1声道（右）低字节   1声道（右）高字节

//    QString pchPath = QFileDialog::getOpenFileName(this,
//                                                     tr("Open File"),
//                                                     QDir::currentPath(),
//                                                     tr("*.pcm"));

    this->m_leftOriginWave.clear();
    this->m_rightOriginWave.clear();

    QFile pchFile(pchPath);
    if( !pchFile.open(QIODevice::ReadOnly) )
    {
        QMessageBox::warning(this,"wave","open file failed!");
        return false;
    }

    m_fileName = pchPath;
    QDataStream pchStream(&pchFile);

    if(m_isDoubleTrack == true)
    {
        if(m_quantizationBit == 16)
        {
            qint i = 0;
            while(pchStream.atEnd() == false )//&& i < 15)
            {
                quint8 value[4] = {0,0,0,0};
                pchStream>>value[0];
                pchStream>>value[1];
                pchStream>>value[2];
                pchStream>>value[3];

                qint16 left = (value[1]<<8) + value[0];
                qint16 right = (value[3]<<8) + value[2];
                this->m_leftOriginWave.append(QPoint(i,left));
                this->m_rightOriginWave.append(QPoint(i,right));
                i++;
            }
        }
        else
        {
            qDebug()<<"sorry,m_quantizationBit != 16, unsupport now!";
            pchFile.close();
            return false;
        }
    }
    else
    {
        qDebug()<<"sorry, unsupport single Track, now";
        pchFile.close();
        return false;
    }

    pchFile.close();
    ptextEdit->append(QString("the wave size is %1 per track!").arg(this->m_leftOriginWave.size()));

    return true;
}

void QWaveAnalyse::calculateSlope(const QList<QPoint> *origin, QList<QPoint> *slope)
{
    const qint unitInterval = m_quantizationFrequency*0.02;
    const qint size = origin->size()/unitInterval;

    QPoint prev = QPoint(0,0);
    qint i = 0;
    while(i + 1 < size)
    {
        QPoint next = this->slopePerInterval(origin->mid(i*unitInterval, unitInterval));

        if(next != QPoint(0,0))
        {
//人耳辨别两次声音的最短时间约0.1~0.15秒
            if((next.x() + next.y() - prev.x() - prev.y()) > m_quantizationFrequency*0.2)
            {
                if(next.y() > m_quantizationFrequency*0.6)
                {
                    slope->append(next);
                    //continue;
                }
            }
            prev = next;
        }
        i++;
    }
}

QPoint QWaveAnalyse::slopePerInterval(const QList<QPoint> &origin)
{
#if 1
    QPoint higher = (origin.at(0).y() >  origin.at(1).y()) ? origin.at(0) : origin.at(1);
    QPoint lower = (origin.at(0).y() <  origin.at(1).y()) ? origin.at(0) : origin.at(1);
    qint size = origin.size();

    for(qint i = 2; i < size; i += 2)
    {
        QPoint first = origin.at(i);
        QPoint second = origin.at(i + 1);

        if(first.y() < second.y())
        {
            if(first.y() < lower.y())
            {
                lower = first;
            }

            if(second.y() > higher.y())
            {
                higher = second;
            }
        }
        else
        {
            if(second.y() < lower.y())
            {
                lower = second;
            }

            if(first.y() > higher.y())
            {
                higher = first;
            }
        }
    }

    if(higher.x() == lower.x())
    {
        return QPoint(0,0);
    }

    if(higher.y() < 5000 || lower.y() > -5000)
    {
        return QPoint(0,0);
    }

    if(higher.y() - lower.y() < 80*qAbs(higher.x() - lower.x()))
    {
        return QPoint(0,0);
    }

    return QPoint(lower.x() , higher.x());

#else
    QPoint first = QPoint(0,0);
    QPoint second = QPoint(0,0);
    QPoint slope = QPoint(first.x(),0);

    qint i = 0;
    qint size = origin.size();
    bool flag = false;

    while(i < size)
    {
        QPoint pt = origin.at(i);
        if(flag == false)
        {
            i++;
            if( pt.y() < 0 )
            {
                continue;
            }

            first = pt;
            second = QPoint(0,0);
            flag = true;
        }
        else
        {
            if( pt.y() < 0 )
            {
                first = QPoint(0,0);
                i++;
                flag = false;
                continue;
            }
            else
            {
                second = pt;

                while(i + 1 < size)
                {
                    i++;
                    QPoint temp = origin.at(i);

                    if( temp.y() < 0)
                    {
                        break;
                    }
                    else
                    {
                        if(second.y() < temp.y())
                        {
                            second = temp;
                        }
                    }
                }

                if(i + 1 < size)
                {
                    QPoint temp = QPoint(second.x(), 0);
                    temp.setY((second.y() - first.y())/(second.x() - first.x()) );

                    if(temp.y() > slope.y())
                    {
                        slope = temp;
                    }
                }

                flag = false;
                i++;
            }
        }
    }

    return slope;
#endif
}

void QWaveAnalyse::createPList(QString mp3Path)
{
    QString head = QString("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<!DOCTYPE plist PUBLIC \"-//Apple//DTD PLIST 1.0//EN\" \"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">\n");
    head += QString("<plist version=\"1.0\">\n\n<dict>\n");
    QString body = QString();

    qint size = this->m_leftSlope.size();
    qint prev = qrand()%16;
    for(qint i = 0; i < size; ++i)
    {
        qint index = this->m_leftSlope.at(i).y();
        qint next = qrand()%16;

        if(prev == next)
        {
            next = (prev + 1 + qrand()%8 )%16;
        }

        body += this->addTypeNote(i,index,next);

        prev = next;
    }

    QString nil = ("</dict>\n</plist>");

    QString str = head + body + nil;

    QString outPath = mp3Path + ".plist";

    QFile outFile(outPath);
    if( !outFile.open(QIODevice::WriteOnly) )
    {
        QMessageBox::warning(this,"wave","open outFile failed!");
        return ;
    }

    QTextStream out(&outFile);
    out<<str;
    outFile.close();
}

QString QWaveAnalyse::addTypeNote(const qint noteId, const qint index, const qint value)
{
    QString temp[16] = {"A1","A2","A3","A4","B1","B2","B3","B4","C1","C2","C3","C4","D1","D2","D3","D4"};

    QString str[11];
    str[0] = QString("  <key>Note-%1</key>").arg(noteId);
    str[1] = QString("  <dict>");
    str[2] = QString("    <key>ExtraNumber</key>");
    str[3] = QString("    <integer>0</integer>");
    str[4] = QString("    <key>ExtraString</key>");
    str[5] = QString("    <string>0</string>");
    str[6] = QString("    <key>StartPosition</key>");
    str[7] = QString("    <string>%1</string>").arg(this->indexToTime(index - 0.6*m_quantizationFrequency));
    str[8] = QString("    <key>Type</key>");
    str[9] = QString("    <string>%1</string>").arg(temp[value]);
    str[10] = QString("  </dict>");

    QString rtn = QString();

    for(int i = 0; i < 11; i++)
    {
        rtn += str[i] + "\n";
    }

    return rtn;
}

QString QWaveAnalyse::addBackgroundNote(const qint index)
{
    return QString();
}

QString QWaveAnalyse::indexToTime(const qint index)
{
    qint mm = index/m_quantizationFrequency/60;
    qint ss = index/m_quantizationFrequency - 60*mm;
    qint zz = (index%m_quantizationFrequency)*1000/m_quantizationFrequency;
    QTime time = QTime(0,mm,ss,zz);
    return time.toString(QString("mm:ss:zzz"));
}

bool QWaveAnalyse::mainDecode(QString inFile)
//bool MainDecode(const char* sInFile,FILE* pOutFile)
{
    char *sInFile = inFile.toAscii().data();
qDebug()<<sInFile;


//    FILE* pOutFile;
//    //open the output file
//    pOutFile=fopen(QString( inFile + QString(".pcm") ).toAscii().data(),"wb");

//    if(pOutFile==NULL)
//    {
//        qDebug()<<("Error opening output file.\n");
//        return false;
//    }

    void* pDecoder;

    if(DecMPA_CreateUsingFile(&pDecoder,sInFile,DECMPA_VERSION)!=DECMPA_OK)
    {
        ptextEdit->append("Error creating decoder.\n");
        return false;
    }

    bool bResult = false;
    char* pOutBuffer;
    bool bFirstDecode=true;
    long offset = 0;

    //allocate an output buffer to store the decoded data.
    //the size can be chosen freely, DecMPA will work with what you provide
    long nOutBufferBytes=16384;
    pOutBuffer=new char[nOutBufferBytes];

    //now just decode the data until the end of the input file is reached
    while(true)
    {
        long nDecodedBytes;
        int DecodeResult = DecMPA_Decode(pDecoder,pOutBuffer,nOutBufferBytes,&nDecodedBytes);
        if(DecodeResult==DECMPA_OK)
        {
            if(bFirstDecode)
            {
                long Duration=-1;
                DecMPA_MPEGHeader Header;
                memset(&Header,0,sizeof(Header));
                DecMPA_GetDuration(pDecoder,&Duration);
                DecMPA_GetMPEGHeader(pDecoder,&Header);

                qDebug("duration:   %ld ms\n"
                        "layer:      %d\n"
                        "bitrate:    %d kbps\n"
                        "frequency:  %d Hz\n"
                        "mode:       %d \n",
                        Duration,Header.nLayer,Header.nBitRateKbps,Header.nFrequency,Header.nMode);

                bFirstDecode=false;

ptextEdit->append(QString("bitrate: %1kbps").arg(Header.nBitRateKbps));
ptextEdit->append(QString("frequency: %1Hz").arg(Header.nFrequency));
ptextEdit->append(QString("mode: %1").arg(Header.nMode));

                this->m_quantizationFrequency = Header.nFrequency;
                this->m_quantizationBit = 16;  //这个量化位数 不知道怎么提取
                if(Header.nMode == 0x11)
                {
                    this->m_isDoubleTrack = false;
                }
                else
                {
                    this->m_isDoubleTrack = true;
                }

                this->m_leftOriginWave.clear();
                this->m_rightOriginWave.clear();
            }

            //some data was decoded - write it to the output file
            //the format of the decoded data (i.e. the frequency and
            //wether it is stereo) could be obtained by calling
            //DecMPA_GetOutputFormat. We just write the data to a
            //file here, though.
//            if(fwrite(pOutBuffer,1,nDecodedBytes,pOutFile)!=(size_t)nDecodedBytes)
//            {
//                printf("Error writing to output file");
//                break;
//            }

            //将解码完的数据写入,算法很危险
            if(m_isDoubleTrack == true)
            {
                if(m_quantizationBit == 16)
                {
                    for(int i = 0; i < (nDecodedBytes>>2); i++)
                    {
                        quint8 value1 = pOutBuffer[4*i];
                        quint8 value2 = pOutBuffer[4*i + 1];
                        quint8 value3 = pOutBuffer[4*i + 2];
                        quint8 value4 = pOutBuffer[4*i + 3];

                        qint16 left = (value2<<8) + value1;
                        qint16 right = (value4<<8) + value3;
                        this->m_leftOriginWave.append(QPoint(offset + i,left));
                        this->m_rightOriginWave.append(QPoint(offset + i,right));
                    }

                    offset += (nDecodedBytes>>2);
                }
                else
                {
                    ptextEdit->append("sorry,m_quantizationBit != 16, unsupport now!");
                    return false;
                }
            }
            else
            {
                ptextEdit->append("sorry, unsupport single Track, now");
                return false;
            }
        }
        else
        {
            //the decoder was unable to decode more data
            if(DecodeResult==DECMPA_END)
            {
                //everything is ok, the end of the file was reached
                delete[] pOutBuffer;
                bResult = true;
            }
            else
            {
                //some error occurred
                qDebug()<<("Error during decoding.\n");
            }

            //always stop the decoding loop here, no matter wether an
            //error occurred or not,
            break;
        }
    }

    DecMPA_Destroy(pDecoder);
    //fclose(pOutFile);
    return bResult;
}

void QWaveAnalyse::timeBreak()
{
    static qint i = 0;
    ptextEdit->append(QString("%1").arg(this->m_leftSlope.at(i).x()));
    i++;

    if( i < this->m_leftSlope.size() )
    {
        qint delt = this->m_leftSlope.at(i).x() - this->m_leftSlope.at(i-1).x();
        QTimer::singleShot(10*delt/441, this, SLOT(timeBreak()));
    }
}

