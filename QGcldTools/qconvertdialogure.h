#ifndef QCONVERTDIALOGURE_H
#define QCONVERTDIALOGURE_H

#include <QString>
#include <QStringList>
#include <QList>

//转小倩的文本,从xml到lua
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
    QString parse_file(QString filePath);
    void parse_dir(QString filePath);

    QStringList m_list;

public:
    QList<QSingleDialogue*> m_dialogues;
};

#endif // QCONVERTDIALOGURE_H
