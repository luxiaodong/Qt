#ifndef QWAVEANALYSE_H
#define QWAVEANALYSE_H

#include <QtGui/QWidget>
#include <QPoint>
#include <QList>
#include <QDataStream>
#include <QString>
#include <QDebug>
#include <QElapsedTimer>
#include <QMouseEvent>
#include <QDataStream>
#include <QTextStream>
#include <QMessageBox>
#include <QFileDialog>
#include <QDir>
#include <QFile>
#include <QTime>
#include <QTimer>
#include <QSound>
#include <QTextEdit>
#include <Phonon/AudioOutput>
#include <Phonon/MediaObject>

typedef int qint;


class QWaveAnalyse : public QWidget
{
    Q_OBJECT

public:
    QWaveAnalyse(QWidget *parent = 0);
    ~QWaveAnalyse();

    void process();
    bool loadWaveData(QString pchPath);
    void calculateSlope(const QList<QPoint> *origin, QList<QPoint> *slope);
    QPoint slopePerInterval(const QList<QPoint> &origin);

    void createPList(QString mp3Path);
    QString indexToTime(const qint index);
    QString addTypeNote(const qint noteId, const qint index, const qint value);
    QString addBackgroundNote(const qint index);

    bool mainDecode(QString inFile);

public slots:
    void timeBreak();

//  void discardPointOnHillside(const QList<QPoint> *origin, QList<QPoint> *peak);

//may useless
//    void discardPointOnHillside();
//    void discardInflectionPoint();
//    void discardInflectionPoint(const QList<QPoint> *peak, QList<QPoint> *analyse);

protected:
    void mousePressEvent(QMouseEvent *e);

private:
    bool m_isDoubleTrack;           //是否双声道
    qint m_quantizationBit;         //量化位数
    qint m_quantizationFrequency;   //采样频率

    QString m_fileName;

//最原始的所有点
    QList<QPoint> m_leftOriginWave;
    QList<QPoint> m_rightOriginWave;

//计算拐点处的斜率,每格一定时间段计算一次,估计0.02之间
//x表示最高点的索引号, y表示斜率值
    QList<QPoint> m_leftSlope;
    QList<QPoint> m_rightSlope;

    QTextEdit *ptextEdit;
};

#endif // QWAVEANALYSE_H
