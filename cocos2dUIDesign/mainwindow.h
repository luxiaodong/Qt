#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSize>
#include <QAction>
#include <QCloseEvent>
#include <QKeyEvent>

#include <QListWidget>
#include <QListWidgetItem>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsWidget>
#include <QHBoxLayout>
#include <QVariant>

#include "qgraphicslayerscene.h"
#include "variantmanager.h"
#include "variantfactory.h"
#include "qttreepropertybrowser.h"

namespace Ui {
    class MainWindow;
}

enum{
    item_id = 0,
    item_id_sprite,
    item_id_menu,
    item_id_label,
    item_id_list,
    item_id_bar,
    item_id_table,
    item_name,
    item_keepRatio,
    item_ratio,
    item_scaled_width,
    item_scaled_height,
    item_sprite_image,
    item_menu_normalImage,
    item_menu_pressImage,
    item_menu_singleImage,
    item_label_text,
    item_label_font,
    item_label_color,
    item_label_supportEdit,
    item_label_supportClick,
    item_bar_bgImage,
    item_bar_barImage,
    item_table_row,
    item_table_column,
    item_table_offset_x,
    item_table_offset_y,
    item_table_span_width,
    item_table_span_height
};

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    void createPropertyTree();

private:
    void connectMenuSlot();
    void connectSceneEditSlot();
    void connectListSlot();

private slots:
    void clickNew();
    void clickOpen();
    void clickSave();
    void clickSaveAs();
    void clickParset2dFile();
    void clickCode();
    void clickCopy();
    void clickPaste();
    void clickDelete();
    void clickSpite();
    void clickMenuItem();
    void clickLabel();
    void clickBar();
    void clickList();
    void clickTable();
    void clickWorkPath();
    void clickSourcePath();
    void clickAlign();
    void clickGrid();

    void currentItemChanged(QGraphicsItem*);
    void currentItemPropertyChanged(QGraphicsItem*);
    void listWidgetItemClicked(QListWidgetItem*);

private slots:
    void valueChanged(QtProperty*,QVariant);

private:
    void valueNameChanged();
    void valuePointChanged();
    void valueXChanged();
    void valueYChanged();
    void valueZChanged();
    void valueScaleChanged();
    void valueKeepRatioChanged();
    void valueRatioChanged();
    void valueWidthChanged();
    void valueHeightChanged();
    void valueRotateChanged();
    void valueOpacityChanged();
    void valueVisibleChanged();
    void valueEnableChanged();
    void valueSingleChanged();
    void valueLightChanged();
    void valueMenuImageChanged();
    void valueSpriteImageChanged();
    void valueTextChanged();
    void valueEditChanged();
    void valueClickChanged();
    void valueFontChanged();
    void valueColorChanged();
    void valueBarBgImageChanged();
    void valueBarBarImageChanged();
    void valueListWidthChanged();
    void valueListHeightChanged();
    void valueTableWidthChanged();
    void valueTableHeightChanged();
    void valueTableRowChanged();
    void valueTableColumnChanged();
    void valueTableOffsetXChanged();
    void valueTableOffsetYChanged();
    void valueTableSpanWidthChanged();
    void valueTableSpanHeightChanged();

private:
    QHBoxLayout*        m_hBoxLayout;
    QListWidget*        m_listWidget;
    QGraphicsView*      m_graphicsView;
    QtTreePropertyBrowser*  m_browser;

    QtVariantProperty*  m_name;
    QtVariantProperty*  m_visible;
    QtVariantProperty*  m_enable;
    QtVariantProperty*  m_z;
    QtVariantProperty*  m_point;
    QtVariantProperty*  m_x;
    QtVariantProperty*  m_y;
    QtVariantProperty*  m_scale;
    QtVariantProperty*  m_keepRatio;
    QtVariantProperty*  m_ratio;
    QtVariantProperty*  m_width;
    QtVariantProperty*  m_height;
    QtVariantProperty*  m_opacity;
    QtVariantProperty*  m_rotate;
    QtVariantProperty*  m_menuItem;
    QtVariantProperty*  m_menuImage;
    QtVariantProperty*  m_single;
    QtVariantProperty*  m_light;
    QtVariantProperty*  m_sprite;
    QtVariantProperty*  m_spriteImage;
    QtVariantProperty*  m_label;
    QtVariantProperty*  m_text;
    QtVariantProperty*  m_edit;
    QtVariantProperty*  m_click;
    QtVariantProperty*  m_font;
    QtVariantProperty*  m_color;
    QtVariantProperty*  m_list;
    QtVariantProperty*  m_listWidth;
    QtVariantProperty*  m_listHeight;
    QtVariantProperty*  m_bar;
    QtVariantProperty*  m_barBgImage;
    QtVariantProperty*  m_barBarImage;
    QtVariantProperty*  m_table;
    QtVariantProperty*  m_tableWidth;
    QtVariantProperty*  m_tableHeight;
    QtVariantProperty*  m_tableRow;
    QtVariantProperty*  m_tableColumn;
    QtVariantProperty*  m_tableOffsetX;
    QtVariantProperty*  m_tableOffsetY;
    QtVariantProperty*  m_tableSpanWidth;
    QtVariantProperty*  m_tableSpanHeight;

private:
    void clearProject();
    void setWindowTitleShowSaveMark(bool );
    void setStatusMessageFormItem(QGraphicsItem*);
    void initPropertyTree(QGraphicsItem*);
    void updatePropertyTreeBy(QGraphicsItem* );
    bool isValidItem(QGraphicsItem* );
    bool isOverLoadObjectName(QString newName);
    int createZValue();
    QString workingPath();
    QString valueInLine(QString line, QString key);

private:
    void readUI();
    void writeUI();
    QString lineFromItem(QGraphicsItem* item);
    QGraphicsItem* itemFromLine(QString line);
    QListWidgetItem* listWidgetItemFromGraphicsItem(QGraphicsItem* graphicsItem);
    QGraphicsItem* graphicsItemFromListWidgetItem(QListWidgetItem* listWidgetItem);

protected:
//    void keyPressEvent(QKeyEvent *);
//    void keyReleaseEvent(QKeyEvent *);
    void closeEvent(QCloseEvent* );

private:
    QGraphicsLayerScene*    m_scene;
    QGraphicsItem*          m_currentItem;
    QGraphicsRectItem*      m_focusItem;
    bool                    m_isSaved;
    QString                 m_saveFileFullPath;
    QString                 m_copyItem;
    int                     m_aligning;
    QSizeF                  m_sceneSize;
    QFont                   m_defaultFont;

private:
    Ui::MainWindow*     ui;
};

#endif // MAINWINDOW_H
