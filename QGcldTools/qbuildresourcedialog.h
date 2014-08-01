#ifndef QBUILDRESOURCEDIALOG_H
#define QBUILDRESOURCEDIALOG_H

#include <QDialog>
#include <QStringList>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMap>
#include <QListWidgetItem>

namespace Ui {
class QBuildResourceDialog;
}

class QBuildResourceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit QBuildResourceDialog(QWidget *parent = 0);
    ~QBuildResourceDialog();

public:
    void createModel();
    void traverseFiles(QString path);

public:
    bool isExistVersion();

protected:
    void dragEnterEvent(QDragEnterEvent* event);
    void dropEvent(QDropEvent* event);

public slots:
    void itemChanged(QListWidgetItem*);
    void click_browse();
    void click_start();
    void click_quit();
    void click_selectAll();
    void click_disableAll();
    void click_filter(QString);
    void click_returnPressed();

private:
    QMap<QString, bool> m_fileListMap;
    QMap<QString, bool> m_filterFileListMap;
    QString m_resourceDir;

private:
    Ui::QBuildResourceDialog *ui;
};

#endif // QBUILDRESOURCEDIALOG_H
