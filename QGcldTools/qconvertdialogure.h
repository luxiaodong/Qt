#ifndef QCONVERTDIALOGURE_H
#define QCONVERTDIALOGURE_H

#include <QString>
#include <QStringList>
#include <QList>

class QSingleSpeak
{
public:
    QString print(int index, bool isLast);
public:
    QString m_type;
    QString m_pic;
    QString m_open;
    QString m_desc;
};

class QSingleDialogue
{
public:
    void parseSingleSpeak(QString);
    QString print();
public:
    QString m_itemID;
    QList<QSingleSpeak*> m_speaks;
    QString m_note;
};

class QConvertDialogure
{
public:
    QConvertDialogure();

public:
    void parse(QString filePath);

public:
    QList<QSingleDialogue*> m_dialogues;
};

#endif // QCONVERTDIALOGURE_H