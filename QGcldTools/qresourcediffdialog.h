#ifndef QRESOURCEDIFFDIALOG_H
#define QRESOURCEDIFFDIALOG_H

#include <QDialog>
#include <QMap>
#include <QString>
#include <QStringList>
#include <QDir>
#include <QFileInfoList>

namespace Ui {
class QResourceDiffDialog;
}

class QResourceDiffDialog : public QDialog
{
    Q_OBJECT

public:
    explicit QResourceDiffDialog(QWidget *parent = 0);
    ~QResourceDiffDialog();

public slots:
    void clickStart();
    void clickQuit();

    void clickBrowse1();
    void clickBrowse2();

public:
    bool loadOneRes();
    bool loadOtherRes();
    bool loadOneRes2();
    bool loadOtherRes2();
    bool compareTwoMap();

private:
    Ui::QResourceDiffDialog *ui;

private:
    QString m_oneDir;
    QString m_otherDir;
    QStringList m_oneList;
    QStringList m_otherList;
    QMap<QString,QString> m_oneMap;
    QMap<QString,QString> m_otherMap;
    QMap<QString,QString> m_diffMap;
};

#endif // QRESOURCEDIFFDIALOG_H
