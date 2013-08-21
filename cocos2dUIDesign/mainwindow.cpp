#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qentername.h"
#include "qsizeselect.h"
#include "qexportsourcecode.h"

#include <QDebug>
#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QImage>
#include <QFileDialog>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QSettings>
#include <QFontDialog>
#include <QColor>
#include <QLayout>
#include <QRectF>
#include <QMap>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_defaultFont = QFont( "DFPHaiBaoW12-GB", 18);
#ifdef Q_WS_WIN
    m_defaultFont.setPixelSize(18);
#endif

    m_hBoxLayout = new QHBoxLayout();
    m_hBoxLayout->setSizeConstraint(QLayout::SetDefaultConstraint);
    this->centralWidget()->setLayout(m_hBoxLayout);

    m_listWidget = new QListWidget();
    m_listWidget->setMinimumWidth(100);
    m_hBoxLayout->addWidget(m_listWidget,2);

    m_scene = new QGraphicsLayerScene();
    m_graphicsView = new QGraphicsView();
    m_graphicsView->setMinimumWidth(200);
    m_graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_graphicsView->setBackgroundBrush(QBrush(QColor(Qt::black)));
    m_graphicsView->setScene(m_scene);
    m_hBoxLayout->addWidget(m_graphicsView,4);

    createPropertyTree();
    m_hBoxLayout->addWidget(m_browser,3);

    QRect mainR = this->geometry();
    QRect listR = m_listWidget->geometry();
    QRect viewR = m_graphicsView->geometry();
    QRect treeR = m_browser->geometry();

    QSettings settings("yuguosoft.com");
    mainR = settings.value("mainR" , mainR).toRect();
    listR = settings.value("listR" , listR).toRect();
    viewR = settings.value("viewR" , viewR).toRect();
    treeR = settings.value("treeR" , treeR).toRect();

    this->setGeometry(mainR);
    m_listWidget->setGeometry(listR);
    m_graphicsView->setGeometry(viewR);
    m_browser->setGeometry(treeR);

    this->connectMenuSlot();
    this->connectSceneEditSlot();
    this->connectListSlot();

    if(this->workingPath().isEmpty() == false)
    {
        this->statusBar()->showMessage(this->workingPath());
    }

    m_currentItem = 0;
    m_isSaved = true;
    m_saveFileFullPath.clear();
    m_copyItem.clear();

    m_focusItem = new QGraphicsRectItem();
    m_focusItem->setPen(QPen(QColor(Qt::red)));
}

void MainWindow::createPropertyTree()
{
    QtVariantPropertyManager *variantManager = new VariantManager();
    QtVariantEditorFactory *variantFactory = new VariantFactory();
    m_browser = new QtTreePropertyBrowser();
    m_browser->setFactoryForManager(variantManager, variantFactory);

//name
    m_name = variantManager->addProperty(QVariant::String, tr("name"));
    m_name->setValue(tr("none"));
    m_browser->addProperty(m_name);

//visible
    m_visible = variantManager->addProperty(QVariant::Bool, tr("visible"));
    m_visible->setValue(true);
    m_browser->addProperty(m_visible);

//enable
    m_enable = variantManager->addProperty(QVariant::Bool, tr("enable"));
    m_enable->setValue(true);
    m_browser->addProperty(m_enable);

//z
    m_z = variantManager->addProperty(QVariant::Int, "z");
    m_z->setAttribute("minimum",0);
    m_z->setAttribute("maximum",1024);
    m_z->setValue(0);
    m_browser->addProperty(m_z);

//point
    m_x = variantManager->addProperty(QVariant::Int, "x");
    m_x->setAttribute("minimum",0);
    m_x->setAttribute("maximum",1024);
    m_x->setValue(0);

    m_y = variantManager->addProperty(QVariant::Int, "y");
    m_y->setAttribute("minimum",0);
    m_y->setAttribute("maximum",1024);
    m_y->setValue(0);

    m_point = variantManager->addProperty(QVariant::String, tr("center point"));
    m_point->setValue(QString("(0,0)"));
    m_point->addSubProperty(m_x);
    m_point->addSubProperty(m_y);
    m_browser->addProperty(m_point);

//scale
    m_keepRatio = variantManager->addProperty(QVariant::Bool, tr("keepRatio"));
    m_keepRatio->setValue(true);

    m_ratio = variantManager->addProperty(QVariant::Double, tr("ratio"));
    m_ratio->setAttribute("minimum", 0.0f);
    m_ratio->setAttribute("maximum", 1024.0f);
    m_ratio->setAttribute("singleStep", 0.01f);
    m_ratio->setValue(1.0f);

    m_width = variantManager->addProperty(QVariant::Int, tr("width"));
    m_width->setAttribute("minimum",0);
    m_width->setAttribute("maximum",1024);
    m_width->setValue(0);
    m_width->setEnabled(false);

    m_height = variantManager->addProperty(QVariant::Int, tr("height"));
    m_height->setAttribute("minimum",0);
    m_height->setAttribute("maximum",1024);
    m_height->setValue(0);
    m_height->setEnabled(false);

    m_scale = variantManager->addProperty(QVariant::String, tr("scale"));
    m_scale->setValue(QString("(1.00,1.00)"));
    m_scale->addSubProperty(m_keepRatio);
    m_scale->addSubProperty(m_ratio);
    m_scale->addSubProperty(m_width);
    m_scale->addSubProperty(m_height);
    m_browser->addProperty(m_scale);

//opacity
    m_opacity = variantManager->addProperty(QVariant::Double, tr("opacity"));
    m_opacity->setAttribute("minimum", 0.0f);
    m_opacity->setAttribute("maximum", 1.0f);
    m_opacity->setAttribute("singleStep", 0.01f);
    m_opacity->setValue(1.0f);
    m_browser->addProperty(m_opacity);

//rotate
    m_rotate = variantManager->addProperty(QVariant::Int, tr("rotate"));
    m_rotate->setAttribute("minimum", 0);
    m_rotate->setAttribute("maximum", 360);
    m_rotate->setValue(0);
    m_browser->addProperty(m_rotate);

//menuItem
    m_menuImage = variantManager->addProperty(VariantManager::filePathTypeId(), tr("image"));
    m_menuImage->setValue("");
    m_menuImage->setAttribute("filter", "Image files (*.png)");

    m_single = variantManager->addProperty(QVariant::Bool, tr("single"));
    m_single->setValue(false);

    m_light = variantManager->addProperty(QVariant::Bool, tr("light"));
    m_light->setValue(false);

    m_menuItem = variantManager->addProperty(QtVariantPropertyManager::groupTypeId(), tr("menuItem"));
    m_menuItem->addSubProperty(m_menuImage);
    m_menuItem->addSubProperty(m_single);
    m_menuItem->addSubProperty(m_light);
    m_browser->addProperty(m_menuItem);

//sprite
    m_spriteImage = variantManager->addProperty(VariantManager::filePathTypeId(), tr("image"));
    m_spriteImage->setValue("");
    m_spriteImage->setAttribute("filter", "Image files (*.png)");

    m_sprite = variantManager->addProperty(QtVariantPropertyManager::groupTypeId(), tr("sprite"));
    m_sprite->addSubProperty(m_spriteImage);
    m_browser->addProperty(m_sprite);

//label
    m_text = variantManager->addProperty(QVariant::String, tr("text"));
    m_edit = variantManager->addProperty(QVariant::Bool, tr("edit"));
    m_edit->setValue(false);
    m_click = variantManager->addProperty(QVariant::Bool, tr("click"));
    m_click->setValue(false);
    m_font = variantManager->addProperty(QVariant::Font, tr("font"));
    m_font->setValue(m_defaultFont);
    m_color = variantManager->addProperty(QVariant::Color, tr("color"));
    m_color->setValue(QColor(Qt::white));
    m_label = variantManager->addProperty(QtVariantPropertyManager::groupTypeId(), tr("label"));
    m_label->addSubProperty(m_text);
    m_label->addSubProperty(m_edit);
    m_label->addSubProperty(m_click);
    m_label->addSubProperty(m_font);
    m_label->addSubProperty(m_color);
    m_browser->addProperty(m_label);

//bar
    m_barBgImage = variantManager->addProperty(VariantManager::filePathTypeId(), tr("bgImage"));
    m_barBgImage->setValue("");
    m_barBgImage->setAttribute("filter", "Image files (*.png)");

    m_barBarImage = variantManager->addProperty(VariantManager::filePathTypeId(), tr("barImage"));
    m_barBarImage->setValue("");
    m_barBarImage->setAttribute("filter", "Image files (*.png)");

    m_bar = variantManager->addProperty(QtVariantPropertyManager::groupTypeId(), tr("bar"));
    m_bar->addSubProperty(m_barBgImage);
    m_bar->addSubProperty(m_barBarImage);
    m_browser->addProperty(m_bar);

//list
    m_listWidth = variantManager->addProperty(QVariant::Int, tr("width"));
    m_listWidth->setAttribute("minimum", 1);
    m_listWidth->setAttribute("maximum", 1024);
    m_listWidth->setValue(50);

    m_listHeight = variantManager->addProperty(QVariant::Int, tr("height"));
    m_listHeight->setAttribute("minimum", 1);
    m_listHeight->setAttribute("maximum", 1024);
    m_listHeight->setValue(50);

    m_list = variantManager->addProperty(QtVariantPropertyManager::groupTypeId(), tr("list"));
    m_list->addSubProperty(m_listWidth);
    m_list->addSubProperty(m_listHeight);
    m_browser->addProperty(m_list);

 //table
    m_tableWidth = variantManager->addProperty(QVariant::Int, tr("width"));
    m_tableWidth->setAttribute("minimum", 1);
    m_tableWidth->setAttribute("maximum", 1024);
    m_tableWidth->setValue(50);

    m_tableHeight = variantManager->addProperty(QVariant::Int, tr("height"));
    m_tableHeight->setAttribute("minimum", 1);
    m_tableHeight->setAttribute("maximum", 1024);
    m_tableHeight->setValue(50);

    m_tableRow = variantManager->addProperty(QVariant::Int, tr("Row"));
    m_tableRow->setAttribute("minimum", 1);
    m_tableRow->setAttribute("maximum", 10);
    m_tableRow->setValue(1);

    m_tableColumn = variantManager->addProperty(QVariant::Int, tr("Column"));
    m_tableColumn->setAttribute("minimum", 1);
    m_tableColumn->setAttribute("maximum", 10);
    m_tableColumn->setValue(1);

    m_tableOffsetX = variantManager->addProperty(QVariant::Int, tr("OffsetX"));
    m_tableOffsetX->setAttribute("minimum", 0);
    m_tableOffsetX->setAttribute("maximum", 1024);
    m_tableOffsetX->setValue(1);

    m_tableOffsetY = variantManager->addProperty(QVariant::Int, tr("OffsetY"));
    m_tableOffsetY->setAttribute("minimum", 0);
    m_tableOffsetY->setAttribute("maximum", 1024);
    m_tableOffsetY->setValue(1);

    m_tableSpanWidth = variantManager->addProperty(QVariant::Int, tr("SpanWidth"));
    m_tableSpanWidth->setAttribute("minimum", 0);
    m_tableSpanWidth->setAttribute("maximum", 1024);
    m_tableSpanWidth->setValue(1);

    m_tableSpanHeight = variantManager->addProperty(QVariant::Int, tr("SpanHeight"));
    m_tableSpanHeight->setAttribute("minimum", 0);
    m_tableSpanHeight->setAttribute("maximum", 1024);
    m_tableSpanHeight->setValue(1);

    m_table = variantManager->addProperty(QtVariantPropertyManager::groupTypeId(), tr("table"));
    m_table->addSubProperty(m_tableWidth);
    m_table->addSubProperty(m_tableHeight);
    m_table->addSubProperty(m_tableRow);
    m_table->addSubProperty(m_tableColumn);
    m_table->addSubProperty(m_tableOffsetX);
    m_table->addSubProperty(m_tableOffsetY);
    m_table->addSubProperty(m_tableSpanWidth);
    m_table->addSubProperty(m_tableSpanHeight);
    m_browser->addProperty(m_table);

    connect(variantManager, SIGNAL(valueChanged(QtProperty*,QVariant)), this, SLOT(valueChanged(QtProperty*,QVariant)));
}

void MainWindow::connectMenuSlot()
{
    connect(ui->actionNew, SIGNAL(triggered()), this, SLOT(clickNew()));
    connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(clickOpen()));
    connect(ui->actionSave, SIGNAL(triggered()), this, SLOT(clickSave()));
    connect(ui->actionSave_As, SIGNAL(triggered()), this, SLOT(clickSaveAs()));
    connect(ui->actionQuit,SIGNAL(triggered()), this, SLOT(close()));

    connect(ui->actionCopy, SIGNAL(triggered()), this, SLOT(clickCopy()));
    connect(ui->actionPaste, SIGNAL(triggered()), this, SLOT(clickPaste()));
    connect(ui->actionDelete, SIGNAL(triggered()), this, SLOT(clickDelete()));

    connect(ui->actionSprite, SIGNAL(triggered()), this, SLOT(clickSpite()));
    connect(ui->actionLabel, SIGNAL(triggered()), this, SLOT(clickLabel()));
    connect(ui->actionMenuItem, SIGNAL(triggered()), this, SLOT(clickMenuItem()));
    connect(ui->actionBar, SIGNAL(triggered()), this, SLOT(clickBar()));
    connect(ui->actionList, SIGNAL(triggered()), this, SLOT(clickList()));
    connect(ui->actionTable, SIGNAL(triggered()), this, SLOT(clickTable()));

    connect(ui->actionWork_Path, SIGNAL(triggered()), this, SLOT(clickWorkPath()));
    connect(ui->actionSource_Path, SIGNAL(triggered()), this, SLOT(clickSourcePath()));
    connect(ui->actionExport_Code, SIGNAL(triggered()), this, SLOT(clickCode()));
    connect(ui->actionParse_t2d_to_ui, SIGNAL(triggered()), this, SLOT(clickParset2dFile()));

    connect(ui->actionAlign, SIGNAL(triggered()), this, SLOT(clickAlign()));
    connect(ui->actionGrid, SIGNAL(triggered()), this, SLOT(clickGrid()));
}

void MainWindow::connectSceneEditSlot()
{
    connect(m_scene, SIGNAL(currentItemChanged(QGraphicsItem*)), this, SLOT(currentItemChanged(QGraphicsItem*)));
    connect(m_scene, SIGNAL(currentItemPropertyChanged(QGraphicsItem*)), this, SLOT(currentItemPropertyChanged(QGraphicsItem*)));
}

void MainWindow::connectListSlot()
{
    connect(m_listWidget, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(listWidgetItemClicked(QListWidgetItem*)));
}

void MainWindow::clickNew()
{
    if(this->workingPath().isEmpty() == true)
    {
        return ;
    }

    if(m_isSaved == false && m_saveFileFullPath.isEmpty() == false)
    {
        QMessageBox msgBox;
        msgBox.setText(tr("The document has been modified."));
        msgBox.setInformativeText(tr("Do you want to save your changes?"));
        msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::No | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Save);
        int ret = msgBox.exec();

        if(ret == QMessageBox::Cancel)
        {
            return ;
        }

        if(ret == QMessageBox::Save)
        {
            this->clickSave();
        }

        if(ret == QMessageBox::No)
        {
            this->clearProject();
        }
    }

    QString fileName = QFileDialog::getSaveFileName(this, tr("New File"),
                                this->workingPath(),
                                "Xml(*.ui)");

    if(fileName.isEmpty() == true)
    {
        return ;
    }

    QSizeSelect dialog(this);

    if(dialog.exec() == QDialog::Rejected)
    {
        return ;
    }

    m_sceneSize = dialog.selectSize();
    m_scene->setSceneRect(QRectF(QPointF(0,0),m_sceneSize));
    m_graphicsView->setFixedSize(m_sceneSize.width(), m_sceneSize.height());
    m_saveFileFullPath = fileName;
    this->setWindowTitleShowSaveMark(true);

    QFile file(fileName);
    if(file.open(QIODevice::WriteOnly) == true)
    {
        QTextStream in(&file);
        in<<QString("<layer width=\"%1\" height=\"%2\" />\n").arg(m_sceneSize.width()).arg(m_sceneSize.height());
        file.close();
    }
}

void MainWindow::clickOpen()
{
    if(this->workingPath().isEmpty() == true)
    {
        return ;
    }

    if(m_isSaved == false && m_saveFileFullPath.isEmpty() == false)
    {
        QMessageBox msgBox;
        msgBox.setText(tr("The document has been modified."));
        msgBox.setInformativeText(tr("Do you want to save your changes?"));
        msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::No | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Save);
        int ret = msgBox.exec();

        if(ret == QMessageBox::Cancel)
        {
            return ;
        }

        if(ret == QMessageBox::Save)
        {
            this->clickSave();
        }

        if(ret == QMessageBox::No)
        {
            this->clearProject();
        }
    }

    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                this->workingPath(),
                                "Xml(*.ui)");

    if(fileName.isEmpty() == true)
    {
        return;
    }

    m_saveFileFullPath = fileName;
    this->setWindowTitleShowSaveMark(true);
    this->readUI();
    //check if read failed
}

void MainWindow::clickSave()
{
    if(m_saveFileFullPath.isEmpty() == true)
    {
        return ;
    }

    this->writeUI();
    this->setWindowTitleShowSaveMark(true);
    //check write failed
}

void MainWindow::clickSaveAs()
{
    if(m_saveFileFullPath.isEmpty() == true)
    {
        return ;
    }

    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
                                this->workingPath(),
                                "Xml(*.ui)");

    if(fileName.isEmpty() == true)
    {
        return ;
    }

    this->writeUI();
    QFile::copy(m_saveFileFullPath, fileName);
    this->setWindowTitleShowSaveMark(true);
}

void MainWindow::clickParset2dFile()
{
    /*

    QString dir = QFileDialog::getOpenFileName(this, tr("Open File"),
                                               "/home",
                                               tr("level (*.t2d)"));

    if(dir.isEmpty() == true)
    {
        return;
    }

    QFile file(dir);
    if(file.open(QIODevice::ReadOnly) == false)
    {
        return ;
    }

    QStringList lineList;
    QTextStream in(&file);
    while(in.atEnd() == false)
    {
        lineList.append(in.readLine());
    }

    QFile outFile(dir + QString(".ui"));
    if(outFile.open(QIODevice::WriteOnly) == false)
    {
        return ;
    }

    QTextStream out(&outFile);
    out<<QString("<layer width=\"1024\" height=\"768\">\n");

    QString imageName;
    QString imagePath;
    QString text;
    qreal x = 0;
    qreal y = 0;
    int z = 1;
    int data = 0;
    int fontSize = 18;

    for(int i = 0; i < lineList.size(); ++i)
    {
        QString temp = lineList.at(i).trimmed();
        if(temp.contains("new t2dStaticSprite") == true)
        {
            imageName = temp.remove("new t2dStaticSprite").remove("(").remove(")").remove("{").remove(" ").trimmed();
            data = item_id_sprite;
        }
        else if(temp.contains("new t2dTextObject") == true)
        {
            imageName = temp.remove("new t2dTextObject").remove("(").remove(")").remove("{").remove(" ").trimmed();
            data = item_id_label;
        }
        else if(temp.contains("imageMap = ") == true)
        {
            imagePath = temp.remove("imageMap = ").remove("\"").remove(";");
            imagePath = imagePath.replace("Texture_", "iPad/images/") + QString(".png");
        }
        else if(temp.contains("Position = ") == true)
        {
            QString temp2 = temp.remove("Position = ").remove("\"").remove(";");
            QStringList tempList3 = temp2.split(" ");
            x = tempList3.first().toFloat();
            y = tempList3.last().toFloat();
        }
        else if(temp.contains("Layer ="))
        {
            QString temp2 = temp.remove("Layer =").remove("\"").remove(";");
            z = temp2.toInt();
        }
        else if(temp.contains("text = ") == true)
        {
            text = temp.remove("text = ").remove("\"").remove(";").trimmed();
        }
        else if(temp.contains("fontSizes =") == true)
        {
            QString temp2 = temp.remove("fontSizes =").remove("\"").remove(";").trimmed();
            fontSize = temp2.toInt();
        }
        else if(temp.contains("}") == true)
        {
            if(data == item_id_sprite)
            {
                QString checkFile = m_workingPath + "/" + imagePath;

                if(QFile::exists(checkFile) == true)
                {
                    QString sprite = QString("<sprite name=\"%1\" x=\"%2\" y=\"%3\" z=\"%4\" imagePath=\"%5\" />\n").arg(imageName).arg((int)(x + 512)).arg((int)(y + 384)).arg(z).arg(imagePath);
                    out<<sprite;
                }
                else
                {
                    qDebug()<<"miss file "<<checkFile;
                }
            }
            else if(data == item_id_label)
            {
                QString sprite = QString("<label name=\"%1\" x=\"%2\" y=\"%3\" z=\"%4\" text=\"%5\" fontSize=\"%6\" />\n").arg(imageName).arg((int)(x + 512)).arg((int)(y + 384)).arg(z).arg(text).arg(fontSize);
                out<<sprite;
            }

            data = 0;
        }
    }

    out<<QString("</layer>");
    file.close();
    outFile.close();

    qDebug()<<"xxxxxx\n";

    */
}

void MainWindow::clickCode()
{
    if(m_saveFileFullPath.isEmpty() == true)
    {
        return ;
    }

    QSettings settings("yuguosoft.com");
    QString sourceCodePath = settings.value("sourceCodePath").toString();

    if(sourceCodePath.isEmpty() == true)
    {
        this->clickSourcePath();
    }

    sourceCodePath = settings.value("sourceCodePath").toString();

    if(sourceCodePath.isEmpty() == true)
    {
        this->statusBar()->showMessage(tr("please set sourceCode Path first!"));
        return;
    }

    if(QMessageBox::No == QMessageBox::information(this,tr("export sourceCode"), sourceCodePath, QMessageBox::Yes, QMessageBox::No))
    {
        return ;
    }

    QString uiFile = m_saveFileFullPath;
    uiFile = uiFile.split("/").last();
    //uiFile.remove(this->workingPath() + "/");

    QStringList list;

    QFile file(m_saveFileFullPath);
    if(file.open(QIODevice::ReadOnly) == false)
    {
        this->statusBar()->showMessage(tr("uiFile open failed!"));
        return ;
    }

    QTextStream in(&file);
    while(in.atEnd() == false)
    {
        QString line = in.readLine();
        if(line.contains("sprite") ||
           line.contains("label") ||
           line.contains("menu") ||
           line.contains("bar") ||
           line.contains("list") ||
           line.contains("table"))
        {
            QStringList temp = line.remove("<").split(" ");
            list.append(QString("%1 %2").arg(temp.at(0), temp.at(1)));
        }
    }
    file.close();

    QExportSourceCode code;
    code.exportSourceCode(uiFile, list);
    this->statusBar()->showMessage(tr("export code OK!"));
}

void MainWindow::clickCopy()
{
    m_copyItem.clear();
    if(m_currentItem == 0)
    {
        return ;
    }

    m_copyItem = this->lineFromItem(m_currentItem);
}

void MainWindow::clickPaste()
{
    if(m_copyItem.isEmpty() == true)
    {
        return;
    }

    QGraphicsItem* item = this->itemFromLine(m_copyItem);
    QEnterName dialog(this);

    while(1)
    {
        if( dialog.exec() == QDialog::Rejected )
        {
            delete item;
            return ;
        }

        if(dialog.nodeName().isEmpty() == true)
        {
            this->statusBar()->showMessage(tr("enter a name."));
        }

        if( this->isOverLoadObjectName(dialog.nodeName()))
        {
            this->statusBar()->showMessage(tr("The objectName has been exists."));
        }
        else
        {
            if(dialog.nodeName().isEmpty() == false)
            {
                break;
            }
        }
    }

    int x = item->x() + 20;
    int y = item->y() + 20;
    item->setPos(x, y);
    item->setData(item_name, dialog.nodeName());
    m_scene->addItem(item);
    item->setZValue(this->createZValue());
    this->currentItemChanged(item);

    QListWidgetItem* it = new QListWidgetItem(dialog.nodeName());
    it->setCheckState(Qt::Unchecked);
    it->setSelected(true);
    m_listWidget->addItem(it);
}

void MainWindow::clickDelete()
{
    if(m_currentItem == 0)
    {
        return;
    }

    QMessageBox msgBox;
    msgBox.setText("Delete Object.");
    msgBox.setInformativeText(m_currentItem->data(item_name).toString());
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::Yes);

    if(QMessageBox::Yes == msgBox.exec())
    {
        QListWidgetItem* it = this->listWidgetItemFromGraphicsItem(m_currentItem);
        m_listWidget->removeItemWidget(it);
        delete it;
        m_scene->removeItem(m_currentItem);
        m_currentItem = 0;
        this->statusBar()->showMessage(tr("delete succeed"));
    }
}

void MainWindow::clickSpite()
{
    if(m_saveFileFullPath.isEmpty() == true)
    {
        this->statusBar()->showMessage(tr("create ui file first!"));
        return ;
    }

    QStringList fileList = QFileDialog::getOpenFileNames(this, tr("Open File"),
                                                     this->workingPath(),
                                                     tr("Images (*.png)"));
    for(int i =0; i < fileList.size(); ++i)
    {
        QString filePath = fileList.at(i);
        if(filePath.isEmpty() == true)
        {
            qDebug()<<QString(tr("path is empty"));
            return ;
        }

        if(filePath.contains(this->workingPath()) == false)
        {
            qDebug()<<QString(tr("not a valid path"));
            return ;
        }

        QString fileName = filePath;
        fileName.remove(this->workingPath() + "/");

        QString nodeName = fileName;
        nodeName.remove(".png").replace("/", "_");

        QEnterName dialog(this);
        dialog.setWindowModality(Qt::WindowModal);
        //dialog.setNodeName(nodeName);

        while(1)
        {
            if( dialog.exec() == QDialog::Rejected )
            {
                return ;
            }

            if(dialog.nodeName().isEmpty() == true )
            {
                this->statusBar()->showMessage(tr("enter a name."));
            }

            if( this->isOverLoadObjectName(dialog.nodeName()) )
            {
                this->statusBar()->showMessage(tr("The objectName has been exists."));
            }
            else
            {
                if(dialog.nodeName().isEmpty() == false)
                {
                    break;
                }
            }
        }

        QPixmap pixmap(filePath);
        QSize s = pixmap.size();
        QGraphicsPixmapItem* item = m_scene->addPixmap(pixmap);
        int x = int((m_sceneSize.width() + 1)/2) - (int)((s.width() + 1)/2);
        int y = (int)((m_sceneSize.height() + 1)/2) - (int)((s.height() + 1)/2);
        item->setX(x);
        item->setY(y);
        item->setZValue(this->createZValue());
        item->setFlag(QGraphicsItem::ItemIsMovable);
        item->setData(item_id, item_id_sprite);
        item->setData(item_name, dialog.nodeName());
        item->setData(item_keepRatio, true);
        item->setData(item_ratio, 1.0f);
        item->setData(item_sprite_image, filePath);
        this->currentItemChanged(item);
        this->setWindowTitleShowSaveMark(false);

        QListWidgetItem* it = new QListWidgetItem(dialog.nodeName());
        it->setCheckState(Qt::Unchecked);
        m_listWidget->addItem(it);
        it->setSelected(true);
    }
}

void MainWindow::clickMenuItem()
{
    if(m_saveFileFullPath.isEmpty() == true)
    {
        this->statusBar()->showMessage("create ui file first!");
        return ;
    }

    QString filePath = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                     this->workingPath(),
                                                     tr("Images (*.png)"));
    if(filePath.isEmpty() == true)
    {
        return ;
    }

    QString copyPath = filePath;
    copyPath.replace(".png", "_Light.png");

    bool isSingle = false;
    if(QFile::exists(copyPath) == false)
    {
        isSingle = true;
//        QMessageBox::warning(this, "not find file", copyPath);
//        return ;
    }

    QString fileName = filePath;
    fileName.remove(this->workingPath() + "/");

    QEnterName dialog(this);
    //QString nodeName = fileName;
    //nodeName.remove(".png").replace("/", "_");
    //dialog.setNodeName(nodeName);

    while(1)
    {
        if( dialog.exec() == QDialog::Rejected )
        {
            return ;
        }

        if(dialog.nodeName().isEmpty() == true )
        {
            this->statusBar()->showMessage("enter a name.");
        }

        if( this->isOverLoadObjectName(dialog.nodeName()) )
        {
            this->statusBar()->showMessage("The objectName has been exists.");
        }
        else
        {
            if(dialog.nodeName().isEmpty() == false)
            {
                break;
            }
        }
    }

    QPixmap pixmap(filePath);
    QGraphicsPixmapItem* item = m_scene->addPixmap(pixmap);
    QSize s = pixmap.size();
    int x = int((m_sceneSize.width() + 1)/2) - (int)((s.width() + 1)/2);
    int y = (int)((m_sceneSize.height() + 1)/2) - (int)((s.height() + 1)/2);
    item->setX(x);
    item->setY(y);
    item->setZValue(this->createZValue());
    item->setFlag( QGraphicsItem::ItemIsMovable);
    item->setData(item_id, item_id_menu);
    item->setData(item_name, dialog.nodeName());
    item->setData(item_keepRatio, true);
    item->setData(item_ratio, 1.0f);
    item->setData(item_menu_normalImage, filePath);
    item->setData(item_menu_singleImage, isSingle);

    if(isSingle == false)
    {
        item->setData(item_menu_pressImage, copyPath);
    }

    this->currentItemChanged(item);
    this->setWindowTitleShowSaveMark(false);

    QListWidgetItem* it = new QListWidgetItem(dialog.nodeName());
    it->setCheckState(Qt::Unchecked);
    m_listWidget->addItem(it);
    it->setSelected(true);
}

void MainWindow::clickLabel()
{
    //bg is black, text is white
    if(m_saveFileFullPath.isEmpty() == true)
    {
        this->statusBar()->showMessage(tr("create ui file first!"));
        return ;
    }

    QEnterName dialog(this);
    while(1)
    {
        if( dialog.exec() == QDialog::Rejected )
        {
            return ;
        }

        if(dialog.nodeName().isEmpty() == true )
        {
            this->statusBar()->showMessage(tr("enter a name."));
        }

        if( this->isOverLoadObjectName(dialog.nodeName()) )
        {
            this->statusBar()->showMessage(tr("The objectName has been exists."));
        }
        else
        {
            if(dialog.nodeName().isEmpty() == false)
            {
                break;
            }
        }
    }

    QString logo("YUGUOSOFT.COM");
    QGraphicsTextItem* item = m_scene->addText(logo);
    QRectF r = item->boundingRect();
    int x = int((m_sceneSize.width() + 1)/2) - (int)((r.width() + 1)/2);
    int y = (int)((m_sceneSize.height() + 1)/2) - (int)((r.height() + 1)/2);
    item->setX(x);
    item->setY(y);
    item->setZValue(this->createZValue());
    item->setFlag( QGraphicsItem::ItemIsMovable);
    item->setData(item_id, item_id_label);
    item->setData(item_name, dialog.nodeName());
    item->setData(item_keepRatio, true);
    item->setData(item_ratio, 1.0f);
    item->setData(item_label_text, logo);
    item->setData(item_label_supportEdit, false);
    item->setData(item_label_supportClick, false);
    item->setData(item_label_color, QColor(Qt::white));
    item->setData(item_label_font, m_defaultFont);
    item->setDefaultTextColor(QColor(Qt::white));
    item->setFont(m_defaultFont);
    this->currentItemChanged(item);
    this->setWindowTitleShowSaveMark(false);

    QListWidgetItem* it = new QListWidgetItem(dialog.nodeName());
    it->setCheckState(Qt::Unchecked);
    m_listWidget->addItem(it);
    it->setSelected(true);
}

void MainWindow::clickList()
{
    if(m_saveFileFullPath.isEmpty() == true)
    {
        this->statusBar()->showMessage(tr("create ui file first!"));
        return ;
    }

    QEnterName dialog(this);
    while(1)
    {
        if( dialog.exec() == QDialog::Rejected )
        {
            return ;
        }

        if(dialog.nodeName().isEmpty() == true )
        {
            this->statusBar()->showMessage(tr("enter a name."));
        }

        if( this->isOverLoadObjectName(dialog.nodeName()) )
        {
            this->statusBar()->showMessage(tr("The objectName has been exists."));
        }
        else
        {
            if(dialog.nodeName().isEmpty() == false)
            {
                break;
            }
        }
    }

    QRectF r = QRectF(0,0,50,50);
    QGraphicsRectItem* item = m_scene->addRect(r,QPen(QColor(Qt::white)),QBrush(QColor(Qt::white)));
    int x = int((m_sceneSize.width() + 1)/2) - (int)((r.width() + 1)/2);
    int y = (int)((m_sceneSize.height() + 1)/2) - (int)((r.height() + 1)/2);
    item->setX(x);
    item->setY(y);
    item->setZValue(this->createZValue());
    item->setFlag( QGraphicsItem::ItemIsMovable);
    item->setData(item_id, item_id_list);
    item->setData(item_name, dialog.nodeName());
    item->setData(item_keepRatio, true);
    item->setData(item_ratio, 1.0f);
    this->currentItemChanged(item);
    this->setWindowTitleShowSaveMark(false);

    QListWidgetItem* it = new QListWidgetItem(dialog.nodeName());
    it->setCheckState(Qt::Unchecked);
    m_listWidget->addItem(it);
    it->setSelected(true);
}

void MainWindow::clickTable()
{
    if(m_saveFileFullPath.isEmpty() == true)
    {
        this->statusBar()->showMessage(tr("create ui file first!"));
        return ;
    }

    QEnterName dialog(this);
    while(1)
    {
        if( dialog.exec() == QDialog::Rejected )
        {
            return ;
        }

        if(dialog.nodeName().isEmpty() == true )
        {
            this->statusBar()->showMessage(tr("enter a name."));
        }

        if( this->isOverLoadObjectName(dialog.nodeName()) )
        {
            this->statusBar()->showMessage(tr("The objectName has been exists."));
        }
        else
        {
            if(dialog.nodeName().isEmpty() == false)
            {
                break;
            }
        }
    }

    QRectF r = QRectF(0,0,50,50);
    QGraphicsRectItem* item = m_scene->addRect(r,QPen(QColor(Qt::white)),QBrush(QColor(Qt::white)));
    int x = int((m_sceneSize.width() + 1)/2) - (int)((r.width() + 1)/2);
    int y = (int)((m_sceneSize.height() + 1)/2) - (int)((r.height() + 1)/2);
    item->setX(x);
    item->setY(y);
    item->setZValue(this->createZValue());
    item->setFlag( QGraphicsItem::ItemIsMovable);
    item->setData(item_id, item_id_table);
    item->setData(item_table_row, 1);
    item->setData(item_table_column, 1);
    item->setData(item_table_offset_x, 1);
    item->setData(item_table_offset_y, 1);
    item->setData(item_table_span_width, 1);
    item->setData(item_table_span_width, 1);
    item->setData(item_table_span_height, 1);
    item->setData(item_name, dialog.nodeName());
    item->setData(item_keepRatio, true);
    item->setData(item_ratio, 1.0f);
    this->currentItemChanged(item);
    this->setWindowTitleShowSaveMark(false);

    QListWidgetItem* it = new QListWidgetItem(dialog.nodeName());
    it->setCheckState(Qt::Unchecked);
    m_listWidget->addItem(it);
    it->setSelected(true);
}

void MainWindow::clickBar()
{
    if(m_saveFileFullPath.isEmpty() == true)
    {
        this->statusBar()->showMessage(tr("create ui file first!"));
        return ;
    }

    QString filePath = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                     this->workingPath(),
                                                     tr("Images (*.png)"));
    if(filePath.isEmpty() == true)
    {
        qDebug()<<QString(tr("path is empty"));
        return ;
    }

    QString fileName = filePath;
    fileName.remove(this->workingPath() + "/");

    QString nodeName = fileName;
    nodeName.remove(".png").replace("/", "_");

    QEnterName dialog(this);
    dialog.setWindowModality(Qt::WindowModal);

    while(1)
    {
        if( dialog.exec() == QDialog::Rejected )
        {
            return ;
        }

        if(dialog.nodeName().isEmpty() == true )
        {
            this->statusBar()->showMessage(tr("enter a name."));
        }

        if( this->isOverLoadObjectName(dialog.nodeName()) )
        {
            this->statusBar()->showMessage(tr("The objectName has been exists."));
        }
        else
        {
            if(dialog.nodeName().isEmpty() == false)
            {
                break;
            }
        }
    }

    QPixmap pixmap(filePath);
    QSize s = pixmap.size();
    QGraphicsPixmapItem* item = m_scene->addPixmap(pixmap);
    int x = int((m_sceneSize.width() + 1)/2) - (int)((s.width() + 1)/2);
    int y = (int)((m_sceneSize.height() + 1)/2) - (int)((s.height() + 1)/2);
    item->setX(x);
    item->setY(y);
    item->setZValue(this->createZValue());
    item->setFlag(QGraphicsItem::ItemIsMovable);
    item->setData(item_id, item_id_bar);
    item->setData(item_name, dialog.nodeName());
    item->setData(item_keepRatio, true);
    item->setData(item_ratio, 1.0f);
    item->setData(item_bar_bgImage, filePath);
    item->setData(item_bar_barImage, filePath);
    this->currentItemChanged(item);
    this->setWindowTitleShowSaveMark(false);

    QListWidgetItem* it = new QListWidgetItem(dialog.nodeName());
    it->setCheckState(Qt::Unchecked);
    m_listWidget->addItem(it);
    it->setSelected(true);
}

void MainWindow::clickWorkPath()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Working Path"),
                                                              "/home",
                                                               QFileDialog::ShowDirsOnly
                                                             | QFileDialog::DontResolveSymlinks);

    if(dir.isEmpty() == true)
    {
        return;
    }

    QSettings settings("yuguosoft.com");
    settings.setValue("workingPath" , dir);
    this->statusBar()->showMessage(dir);
}

void MainWindow::clickSourcePath()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Source Path"),
                                                              "/home",
                                                               QFileDialog::ShowDirsOnly
                                                             | QFileDialog::DontResolveSymlinks);

    if(dir.isEmpty() == true)
    {
        return;
    }

    QSettings settings("yuguosoft.com");
    settings.setValue("sourceCodePath" , dir);
    this->statusBar()->showMessage(dir);
}

void MainWindow::clickAlign()
{
    if(m_currentItem == 0)
    {
        return ;
    }

    m_aligning++;
    m_aligning = (m_aligning + 9)%9;

    int data = m_currentItem->data(item_id).toInt();

    qreal x[3];
    qreal y[3];

    if(data == item_id_sprite || data == item_id_menu)
    {
        QGraphicsPixmapItem* item = (QGraphicsPixmapItem*)m_currentItem;
        QSize s = item->pixmap().size();

        float scale = item->scale();

        int temp = (int)((s.width()*scale + 1)/2) - (int)((s.width()+ 1)/2);
        x[0] = temp;

        temp = (int)((m_sceneSize.width() + 1)/2) - (int)((s.width() + 1)/2);
        x[1] = temp;

        temp = (int)m_sceneSize.width() - (int)(s.width()*scale) + (int)x[0];
        x[2] = temp;

        temp = (int)((s.height()*scale+ 1)/2) - (int)((s.height()+ 1)/2);
        y[0] = temp;

        temp = (int)((m_sceneSize.height() + 1)/2) - (int)((s.height() + 1)/2);
        y[1] = temp;

        temp = (int)m_sceneSize.height() - (int)s.height()*scale + (int)y[0];
        y[2] = temp;
    }

    m_currentItem->setX(x[m_aligning%3]);
    m_currentItem->setY(y[m_aligning/3]);
    currentItemPropertyChanged(m_currentItem);
}

void MainWindow::clickGrid()
{
    /*

    QList<QGraphicsItem*> graphicsItems;
    int count = m_listWidget->count();
    for(int i = 0; i < count; ++i)
    {
        QListWidgetItem* widgetItem = m_listWidget->item(i);
        if(widgetItem->checkState() == Qt::Checked)
        {
            QGraphicsItem* item = this->graphicsItemFromListWidgetItem(widgetItem);
            graphicsItems.append(item);
        }
    }

    count = graphicsItems.count();

    if(count <= 1)
    {
        return ;
    }

    QList<QPointF> posList;

    for(int i = 0; i < count; ++i)
    {
        QPointF p = graphicsItems.at(i)->pos();
        QRectF r = graphicsItems.at(i)->boundingRect();
        posList.append(QPointF(p.x() + r.width()/2, p.y() + r.height()/2));
    }

    qreal r2 = posList.at(0).x() + posList.at(0).y();
    int topLeftIndex = 0;

    for(int i = 0; i < count; ++i)
    {
        QPointF temp = posList.at(i);
        qDebug()<<temp;
        qreal r1 = temp.x() + temp.y();
        if(r1 < r2)
        {
            r2 = r1;
            topLeftIndex = i;
        }
    }

    QPointF topLeftPt = posList.at(topLeftIndex);

    int xAxisCount = 0;
    int yAxisCount = 0;
    QList<int> xOrderList;
    QList<int> yOrderList;

    for(int i = 0; i < count; ++i)
    {
        QPointF p = graphicsItems.at(i)->pos();
        QRectF r = graphicsItems.at(i)->boundingRect();

        if(topLeftPt.y() > p.y() &&
           topLeftPt.y() < p.y() + r.width())
        {
            xAxisCount++;
            xOrderList.append(i);
        }

        if(topLeftPt.x() > p.x() &&
           topLeftPt.x() < p.x() + r.height())
        {
            yAxisCount++;
            yOrderList.append(i);
        }
    }

    if(xAxisCount == 0 ||
       yAxisCount == 0)
    {
        return ;
    }

    qreal xAxisTotal = 0.0f;
    for(int i = 0; i < xAxisCount; ++i)
    {
        int index = xOrderList.at(i);
        xAxisTotal += posList.at(index).x() - topLeftPt.x();
    }
    qreal xAxisAverage = xAxisTotal/(xAxisCount*(xAxisCount + 1)/2);

    qreal yAxisTotal = 0.0f;
    for(int i = 0; i < yAxisCount; ++i)
    {
        int index = yOrderList.at(i);
        yAxisTotal += posList.at(index).y() - topLeftPt.y();
    }
    qreal yAxisAverage = yAxisTotal/(yAxisCount*(yAxisCount + 1)/2);

    QRectF r = graphicsItems.at(topLeftIndex)->boundingRect();
    xAxisAverage = r.width();
    yAxisAverage = r.height();

    QList<bool> flag;
    for(int i = 0; i < count; ++i)
    {
        flag.append(false);
    }

    for(int j = 0; j < yAxisCount; ++j)
    {
        for(int i = 0; i < xAxisCount; ++i)
        {
            QPointF targetPt = topLeftPt + QPointF(xAxisAverage*i, yAxisAverage*j);

            qreal distance = 0.0f;
            int flagIndex = 0;

            for(int i = 0; i < count; ++i)
            {
                if(flag.at(i) == false)
                {
                    qreal deltX = posList.at(i).x() - targetPt.x();
                    qreal deltY = posList.at(i).y() - targetPt.y();
                    qreal distance2 = 0.0 - (deltX*deltX + deltY*deltY);

                    if(distance2 < distance)
                    {
                        distance = distance2;
                        flagIndex = i;
                    }
                }
            }

            bool isOver = true;
            for(int i = 0; i < count; ++i)
            {
                if(flag.at(i) == false)
                {
                    isOver = false;
                }
            }

            if(isOver == false)
            {
                flag.replace(flagIndex, true);
                QRectF r = graphicsItems.at(flagIndex)->boundingRect();
                graphicsItems.at(flagIndex)->setPos( QPointF(targetPt.x() - r.width()/2, targetPt.y() - r.height()/2) );
            }
            else
            {
                this->currentItemPropertyChanged(m_currentItem);
                return ;
            }
        }
    }

    */
}

void MainWindow::currentItemChanged(QGraphicsItem* item)
{
    if(m_currentItem == item)
    {
        return ;
    }

    if(item->data(item_id).toInt() == item_id)
    {
        return ;
    }

    m_currentItem = item;

    if(item != 0)
    {
        this->initPropertyTree(item);
        m_focusItem->setRect(item->boundingRect());
        m_focusItem->setParentItem(item);
        this->currentItemPropertyChanged(item);
        QListWidgetItem* it = this->listWidgetItemFromGraphicsItem(item);
        if(it != 0)
        {
            it->setSelected(true);
        }
    }
    else
    {
        this->setStatusMessageFormItem(item);
    }
}

void MainWindow::currentItemPropertyChanged(QGraphicsItem* item)
{
    if(item == 0)
    {
        return ;
    }

    if(m_currentItem == item)
    {
        this->updatePropertyTreeBy(item);

//        if(m_isCtrlEnable == true)
//        {
//            qreal deltX = item->x() - m_ctrlOperateOriginX;
//            qreal deltY = item->y() - m_ctrlOperateOriginY;

//            int count = m_listWidget->count();
//            for(int i = 0; i < count; ++i)
//            {
//                QListWidgetItem* it = m_listWidget->item(i);

//                if(it->checkState() == Qt::Checked)
//                {
//                    QGraphicsItem* gIt = this->graphicsItemFromListWidgetItem(it);
//                    if(gIt != item)
//                    {
//                        gIt->setX( gIt->x() + deltX);
//                        gIt->setY( gIt->y() + deltY);
//                    }
//                }
//            }

//            m_ctrlOperateOriginX = item->x();
//            m_ctrlOperateOriginY = item->y();
//        }
    }
}

void MainWindow::listWidgetItemClicked(QListWidgetItem* item)
{
    QGraphicsItem* it = this->graphicsItemFromListWidgetItem(item);

    if(it != 0)
    {
        this->currentItemChanged(it);
    }
}

void MainWindow::valueChanged(QtProperty* property, QVariant )
{
    if(property == 0)
    {
        return;
    }
    else if(property == m_name)
    {
        this->valueNameChanged();
    }
    else if(property == m_x ||
            property == m_y)
    {
        if(property == m_x)
        {
            this->valueXChanged();
        }
        else
        {
            this->valueYChanged();
        }

        m_point->setValue(QString("(%1,%2)").arg(m_x->value().toInt()).arg(m_y->value().toInt()));
    }
    else if(property == m_z)
    {
        this->valueZChanged();
    }
    else if(property == m_keepRatio)
    {
        this->valueKeepRatioChanged();

        bool isKeepRatio = m_keepRatio->value().toBool();
        if(isKeepRatio == true)
        {
            m_keepRatio->setValue(true);
            m_ratio->setEnabled(true);
            m_width->setEnabled(false);
            m_height->setEnabled(false);
        }
        else
        {
            m_keepRatio->setValue(false);
            m_ratio->setEnabled(false);
            m_width->setEnabled(true);
            m_height->setEnabled(true);
        }
    }
    else if(property == m_ratio)
    {
        this->valueRatioChanged();
    }
    else if(property == m_width)
    {
        this->valueWidthChanged();
    }
    else if(property == m_height)
    {
        this->valueHeightChanged();
    }
    else if(property == m_rotate)
    {
        this->valueRotateChanged();
    }
    else if(property == m_opacity)
    {
        this->valueOpacityChanged();
    }
    else if(property == m_visible)
    {
        this->valueVisibleChanged();
    }
    else if(property == m_enable)
    {
        this->valueEnableChanged();
    }
    else if(property == m_single)
    {
        this->valueSingleChanged();
    }
    else if(property == m_light)
    {
        this->valueLightChanged();
    }
    else if(property == m_menuImage)
    {
        this->valueMenuImageChanged();
    }
    else if(property == m_spriteImage)
    {
        this->valueSpriteImageChanged();
    }
    else if(property == m_text)
    {
        this->valueTextChanged();
    }
    else if(property == m_edit)
    {
        this->valueEditChanged();
    }
    else if(property == m_click)
    {
        this->valueClickChanged();
    }
    else if(property == m_font)
    {
        this->valueFontChanged();
    }
    else if(property == m_color)
    {
        this->valueColorChanged();
    }
    else if(property == m_barBgImage)
    {
        this->valueBarBgImageChanged();
    }
    else if(property == m_barBarImage)
    {
        this->valueBarBarImageChanged();
    }
    else if(property == m_listWidth)
    {
        this->valueListWidthChanged();
    }
    else if(property == m_listHeight)
    {
        this->valueListHeightChanged();
    }
    else if(property == m_tableWidth)
    {
        this->valueTableWidthChanged();
    }
    else if(property == m_tableHeight)
    {
        this->valueTableHeightChanged();
    }
    else if(property == m_tableRow)
    {
        this->valueTableRowChanged();
    }
    else if(property == m_tableColumn)
    {
        this->valueTableColumnChanged();
    }
    else if(property == m_tableOffsetX)
    {
        this->valueTableOffsetXChanged();
    }
    else if(property == m_tableOffsetY)
    {
        this->valueTableOffsetYChanged();
    }
    else if(property == m_tableSpanWidth)
    {
        this->valueTableSpanWidthChanged();
    }
    else if(property == m_tableSpanHeight)
    {
        this->valueTableSpanHeightChanged();
    }

    this->setWindowTitleShowSaveMark(false);
}

void MainWindow::valueNameChanged()
{
    if(this->isValidItem(m_currentItem) == false)
    {
        return ;
    }

    QString name = m_name->value().toString();
    m_currentItem->setData(item_name, name);
}

void MainWindow::valueXChanged()
{
    if(this->isValidItem(m_currentItem) == false)
    {
        return ;
    }

    int x = m_x->value().toInt();
    QRectF r = m_currentItem->boundingRect();
    m_currentItem->setX(x - (int)((r.width() + 1)/2));
}

void MainWindow::valueYChanged()
{
    if(this->isValidItem(m_currentItem) == false)
    {
        return ;
    }

    int y = m_y->value().toInt();
    QRectF r = m_currentItem->boundingRect();
    m_currentItem->setY(y - (int)((r.height() + 1)/2));
}

void MainWindow::valueKeepRatioChanged()
{
    if(this->isValidItem(m_currentItem) == false)
    {
        return ;
    }

    bool isKeepRatio = m_keepRatio->value().toBool();
    m_currentItem->setData(item_keepRatio, isKeepRatio);
    int  itemId = m_currentItem->data(item_id).toInt();

    if(itemId == item_id_sprite)
    {
        QString path = m_spriteImage->value().toString();
        QPixmap picture(path);
        QSize newSize;

        if(isKeepRatio == true)
        {
            QSize s = picture.size();
            double ratio = m_ratio->value().toDouble();
            newSize = QSize(s.width()*ratio, s.height()*ratio);
            m_currentItem->setData(item_ratio, ratio);
        }
        else
        {
            int w = m_width->value().toInt();
            int h = m_height->value().toInt();

            if(w == 0 || h == 0)
            {
                return ;
            }

            newSize = QSize(w, h);
            m_currentItem->setData(item_scaled_width, w);
            m_currentItem->setData(item_scaled_height, h);
        }

        QGraphicsPixmapItem* sprite = (QGraphicsPixmapItem*)m_currentItem;
        sprite->setPixmap(picture.scaled(newSize));
        this->valueXChanged();
        this->valueYChanged();
    }
}

void MainWindow::valueRatioChanged()
{
    this->valueKeepRatioChanged();
}

void MainWindow::valueWidthChanged()
{
    this->valueKeepRatioChanged();
}

void MainWindow::valueHeightChanged()
{
    this->valueKeepRatioChanged();
}

void MainWindow::valueZChanged()
{
    if(this->isValidItem(m_currentItem) == false)
    {
        return ;
    }

    int z = m_z->value().toInt();
    m_currentItem->setZValue(z);
}

void MainWindow::valueRotateChanged()
{
    if(this->isValidItem(m_currentItem) == false)
    {
        return ;
    }

    int rotate = m_rotate->value().toInt();
    QRectF r = m_currentItem->boundingRect();
    m_currentItem->setTransformOriginPoint(QPointF(0 - ((r.width() + 1)/2) , 0 - ((r.height() + 1)/2)));
    m_currentItem->setRotation(rotate);
    m_currentItem->setTransformOriginPoint(QPointF(0 + ((r.width() + 1)/2) , 0 + ((r.height() + 1)/2)));
}

void MainWindow::valueOpacityChanged()
{
    if(this->isValidItem(m_currentItem) == false)
    {
        return ;
    }

    double opacity = m_opacity->value().toDouble();
    m_currentItem->setOpacity(opacity);
}

void MainWindow::valueVisibleChanged()
{
    if(this->isValidItem(m_currentItem) == false)
    {
        return ;
    }

    bool visible = m_visible->value().toBool();
    m_currentItem->setVisible(visible);
}

void MainWindow::valueEnableChanged()
{
    if(this->isValidItem(m_currentItem) == false)
    {
        return ;
    }

    bool enable = m_enable->value().toBool();
    m_currentItem->setEnabled(enable);
}

void MainWindow::valueSingleChanged()
{
    if(this->isValidItem(m_currentItem) == false)
    {
        return ;
    }

    bool isSingle = m_single->value().toBool();
    m_currentItem->setData(item_menu_singleImage, m_single->value().toBool());

    if(isSingle == true)
    {
        m_light->setEnabled(false);
    }
    else
    {
        QString path = m_currentItem->data(item_menu_normalImage).toString();
        QString copyPath = path.replace(".png", "_Light.png");
        if(QFile::exists(copyPath) == true)
        {
            m_light->setEnabled(true);
            m_currentItem->setData(item_menu_pressImage, copyPath);
        }
        else
        {
            this->statusBar()->showMessage(tr("no file") + copyPath);
            m_light->setEnabled(false);
            m_single->setValue(true);
            m_currentItem->setData(item_menu_singleImage, true);
        }
    }
}

void MainWindow::valueLightChanged()
{
    if(this->isValidItem(m_currentItem) == false)
    {
        return ;
    }

    bool light = m_light->value().toBool();
    int data = m_currentItem->data(item_id).toInt();

    if(data == item_id_menu)
    {
        QGraphicsPixmapItem* item = (QGraphicsPixmapItem*)m_currentItem;

        QString path;
        if(light == true)
        {
            path = m_currentItem->data(item_menu_pressImage).toString();
        }
        else
        {
            path = m_currentItem->data(item_menu_normalImage).toString();
        }

        QPixmap pix(path);
        item->setPixmap(pix);
    }
}

void MainWindow::valueMenuImageChanged()
{
    if(this->isValidItem(m_currentItem) == false)
    {
        return ;
    }

    QString oldPath = m_currentItem->data(item_menu_normalImage).toString();
    QString newPath = m_menuImage->value().toString();

    if(newPath.contains(this->workingPath() + QString("/")) == false)
    {
        QMessageBox::warning(this, tr("invalid path"), newPath);
        return ;
    }

    if(oldPath == newPath)
    {
        return ;
    }

    QString copyPath = newPath;
    copyPath.replace(".png", "_Light.png");

    bool changePicture = true;
    if(QFile::exists(newPath) == false)
    {
        QMessageBox::warning(this, tr("not find file"), newPath);
        changePicture = false;
    }

    if(changePicture == true && QFile::exists(copyPath) == false)
    {
        QMessageBox::warning(this, tr("not find file"), newPath);
        changePicture = false;
    }

    if(changePicture == false)
    {
        m_menuImage->setValue(oldPath);
    }
    else
    {
        QPixmap picture(newPath);
        QGraphicsPixmapItem* sprite = (QGraphicsPixmapItem*)m_currentItem;
        sprite->setPixmap(picture);
        m_currentItem->setData(item_menu_normalImage, newPath);
        m_currentItem->setData(item_menu_pressImage, copyPath);
        this->valueXChanged();
        this->valueYChanged();
    }
}

void MainWindow::valueSpriteImageChanged()
{
    if(this->isValidItem(m_currentItem) == false)
    {
        return ;
    }

    QString oldPath = m_currentItem->data(item_sprite_image).toString();
    QString newPath = m_spriteImage->value().toString();

    if(newPath.contains(this->workingPath() + QString("/")) == false)
    {
        QMessageBox::warning(this, tr("invalid path"), newPath);
        return ;
    }

    if(oldPath == newPath)
    {
        return ;
    }

    bool changePicture = true;
    if(QFile::exists(newPath) == false)
    {
        QMessageBox::warning(this, tr("not find file"), newPath);
        changePicture = false;
    }

    if(changePicture == false)
    {
        m_spriteImage->setValue(oldPath);
    }
    else
    {
        QPixmap picture(newPath);
        QGraphicsPixmapItem* sprite = (QGraphicsPixmapItem*)m_currentItem;
        sprite->setPixmap(picture);
        m_currentItem->setData(item_sprite_image, newPath);
        this->valueKeepRatioChanged();
        this->valueXChanged();
        this->valueYChanged();
    }
}

void MainWindow::valueTextChanged()
{
    if(this->isValidItem(m_currentItem) == false)
    {
        return ;
    }

    int data = m_currentItem->data(item_id).toInt();
    if(data == item_id_label)
    {
        QGraphicsTextItem* label = (QGraphicsTextItem*)m_currentItem;
        QString str = m_text->value().toString();
        label->setPlainText(str);
        m_currentItem->setData(item_label_text, str);
    }
}

void MainWindow::valueEditChanged()
{
    if(this->isValidItem(m_currentItem) == false)
    {
        return ;
    }

    bool isEdit = m_edit->value().toBool();
    m_currentItem->setData(item_label_supportEdit, isEdit);

    if(isEdit == false)
    {
        m_click->setEnabled(true);
    }
    else
    {
        m_click->setValue(false);
        m_click->setEnabled(false);
    }
}

void MainWindow::valueClickChanged()
{
    if(this->isValidItem(m_currentItem) == false)
    {
        return ;
    }

    bool isClick = m_click->value().toBool();
    m_currentItem->setData(item_label_supportClick, isClick);

    if(isClick == false)
    {
        m_edit->setEnabled(true);
    }
    else
    {
        m_edit->setValue(false);
        m_edit->setEnabled(false);
    }
}

void MainWindow::valueFontChanged()
{
    if(this->isValidItem(m_currentItem) == false)
    {
        return ;
    }

    int data = m_currentItem->data(item_id).toInt();
    if(data == item_id_label)
    {
        QGraphicsTextItem* label = (QGraphicsTextItem*)m_currentItem;
        QFont font = m_font->value().value<QFont>();
        m_currentItem->setData(item_label_font, font);

#ifdef Q_WS_WIN
        int size = font.pointSize();
        font.setPixelSize(size);
#endif
        label->setFont(font);
        this->valueXChanged();
        this->valueYChanged();
    }
}

void MainWindow::valueColorChanged()
{
    if(this->isValidItem(m_currentItem) == false)
    {
        return ;
    }

    int data = m_currentItem->data(item_id).toInt();
    if(data == item_id_label)
    {
        QGraphicsTextItem* label = (QGraphicsTextItem*)m_currentItem;
        QColor color = m_color->value().value<QColor>();
        label->setDefaultTextColor( color );
        m_currentItem->setData(item_label_color, color);
    }
}

void MainWindow::valueBarBgImageChanged()
{
    if(this->isValidItem(m_currentItem) == false)
    {
        return ;
    }

    QString oldPath = m_currentItem->data(item_bar_bgImage).toString();
    QString newPath = m_barBgImage->value().toString();

    if(newPath.contains(this->workingPath() + QString("/")) == false)
    {
        QMessageBox::warning(this, tr("invalid path"), newPath);
        return ;
    }

    if(oldPath == newPath)
    {
        return ;
    }

    bool changePicture = true;
    if(QFile::exists(newPath) == false)
    {
        QMessageBox::warning(this, tr("not find file"), newPath);
        changePicture = false;
    }

    if(changePicture == false)
    {
        m_spriteImage->setValue(oldPath);
    }
    else
    {
        QPixmap picture(newPath);
        QGraphicsPixmapItem* sprite = (QGraphicsPixmapItem*)m_currentItem;
        sprite->setPixmap(picture);
        m_currentItem->setData(item_bar_bgImage, newPath);
        this->valueKeepRatioChanged();
        this->valueXChanged();
        this->valueYChanged();
    }
}

void MainWindow::valueBarBarImageChanged()
{
    if(this->isValidItem(m_currentItem) == false)
    {
        return ;
    }

    QString oldPath = m_currentItem->data(item_bar_barImage).toString();
    QString newPath = m_barBarImage->value().toString();

    if(newPath.contains(this->workingPath() + QString("/")) == false)
    {
        QMessageBox::warning(this, tr("invalid path"), newPath);
        return ;
    }

    if(oldPath == newPath)
    {
        return ;
    }

    bool changePicture = true;
    if(QFile::exists(newPath) == false)
    {
        QMessageBox::warning(this, tr("not find file"), newPath);
        changePicture = false;
    }

    if(changePicture == false)
    {
        m_spriteImage->setValue(oldPath);
    }
    else
    {
        QPixmap picture(newPath);
        QGraphicsPixmapItem* sprite = (QGraphicsPixmapItem*)m_currentItem;
        sprite->setPixmap(picture);
        m_currentItem->setData(item_bar_barImage, newPath);
        this->valueKeepRatioChanged();
        this->valueXChanged();
        this->valueYChanged();
    }
}

void MainWindow::valueListWidthChanged()
{
    if(this->isValidItem(m_currentItem) == false)
    {
        return ;
    }

    QPointF pt = m_currentItem->pos();
    int width = m_listWidth->value().toInt();
    QGraphicsRectItem* item = (QGraphicsRectItem*)m_currentItem;
    QRectF r = item->rect();
    r.setWidth(width);
    item->setRect(r);
    item->setPos(pt);
}

void MainWindow::valueListHeightChanged()
{
    if(this->isValidItem(m_currentItem) == false)
    {
        return ;
    }

    QPointF pt = m_currentItem->pos();
    int height = m_listHeight->value().toInt();
    QGraphicsRectItem* item = (QGraphicsRectItem*)m_currentItem;
    QRectF r = item->rect();
    r.setHeight(height);
    item->setRect(r);
    item->setPos(pt);
}

void MainWindow::valueTableWidthChanged()
{
    if(this->isValidItem(m_currentItem) == false)
    {
        return ;
    }

    QPointF pt = m_currentItem->pos();
    int width = m_tableWidth->value().toInt();
    QGraphicsRectItem* item = (QGraphicsRectItem*)m_currentItem;
    QRectF r = item->rect();
    r.setWidth(width);
    item->setRect(r);
    item->setPos(pt);
}

void MainWindow::valueTableHeightChanged()
{
    if(this->isValidItem(m_currentItem) == false)
    {
        return ;
    }

    QPointF pt = m_currentItem->pos();
    int height = m_tableHeight->value().toInt();
    QGraphicsRectItem* item = (QGraphicsRectItem*)m_currentItem;
    QRectF r = item->rect();
    r.setHeight(height);
    item->setRect(r);
    item->setPos(pt);
}

void MainWindow::valueTableRowChanged()
{
    if(this->isValidItem(m_currentItem) == false)
    {
        return ;
    }

    int row = m_tableRow->value().toInt();
    m_currentItem->setData(item_table_row, row);
}

void MainWindow::valueTableColumnChanged()
{
    if(this->isValidItem(m_currentItem) == false)
    {
        return ;
    }

    int column = m_tableColumn->value().toInt();
    m_currentItem->setData(item_table_column, column);
}

void MainWindow::valueTableOffsetXChanged()
{
    if(this->isValidItem(m_currentItem) == false)
    {
        return ;
    }

    int offsetX = m_tableOffsetX->value().toInt();
    m_currentItem->setData(item_table_offset_x, offsetX);
}

void MainWindow::valueTableOffsetYChanged()
{
    if(this->isValidItem(m_currentItem) == false)
    {
        return ;
    }

    int offsetY = m_tableOffsetY->value().toInt();
    m_currentItem->setData(item_table_offset_y, offsetY);
}

void MainWindow::valueTableSpanWidthChanged()
{
    if(this->isValidItem(m_currentItem) == false)
    {
        return ;
    }

    int spanWidth = m_tableSpanWidth->value().toInt();
    m_currentItem->setData(item_table_span_width, spanWidth);
}

void MainWindow::valueTableSpanHeightChanged()
{
    if(this->isValidItem(m_currentItem) == false)
    {
        return ;
    }

    int spanHeight = m_tableSpanHeight->value().toInt();
    m_currentItem->setData(item_table_span_height, spanHeight);
}

void MainWindow::clearProject()
{
    m_scene->clear();
    m_listWidget->clear();
    m_saveFileFullPath.clear();
    m_currentItem = 0;
    m_isSaved = true;
    this->setWindowTitleShowSaveMark(m_isSaved);
}

void MainWindow::setStatusMessageFormItem(QGraphicsItem* item)
{
    if(item == 0)
    {
        this->statusBar()->showMessage("");
    }
    else
    {
        QString str = this->lineFromItem(item);
        this->statusBar()->showMessage(str);
    }
}

void MainWindow::setWindowTitleShowSaveMark(bool bSave)
{
    if(m_saveFileFullPath.isEmpty() == true)
    {
        this->setWindowTitle("cocos2dUIDesign");
        m_isSaved = true;
        return;
    }

    m_isSaved = bSave;

    QString path = m_saveFileFullPath;
    QString str = QString("cocos2dUIDesign") + QString(" - ") + path.remove(this->workingPath() );

    if(m_isSaved == true)
    {
        this->setWindowTitle(str);
    }
    else
    {
        this->setWindowTitle(str + "*");
    }
}

void MainWindow::initPropertyTree(QGraphicsItem* item)
{
    if(item == 0)
    {
        return ;
    }

    m_name->setValue(item->data(item_name));
    m_visible->setValue(item->isVisible());
    m_enable->setValue(item->isEnabled());
    m_z->setValue(item->zValue());
    QRectF r = item->boundingRect();
    m_x->setValue((int)((int)item->x() + (int)((r.width() + 1)/2)));
    m_y->setValue((int)((int)item->y() + (int)((r.height() + 1)/2)));

    int itemId = item->data(item_id).toInt();
    m_sprite->setEnabled(false);
    m_menuItem->setEnabled(false);
    m_label->setEnabled(false);
    m_bar->setEnabled(false);
    m_list->setEnabled(false);
    m_table->setEnabled(false);
    m_scale->setEnabled(false);

    if(itemId == item_id_sprite)
    {
        QString path = item->data(item_sprite_image).toString();
        m_spriteImage->setValue(path);
        m_sprite->setEnabled(true);
        m_scale->setEnabled(true);
    }
    else if(itemId == item_id_menu)
    {
        QString path = item->data(item_menu_normalImage).toString();
        m_menuImage->setValue(path);
        m_light->setValue(false);
        m_menuItem->setEnabled(true);
        m_single->setValue(item->data(item_menu_singleImage).toBool());
    }
    else if(itemId == item_id_label)
    {
        m_text->setValue(item->data(item_label_text));
        m_edit->setValue(item->data(item_label_supportEdit));
        m_click->setValue(item->data(item_label_supportClick));

        QFont font = item->data(item_label_font).value<QFont>();
        QColor color = item->data(item_label_color).value<QColor>();

#ifdef Q_WS_WIN
        int pixelSize = font.pixelSize();
        font.setPointSize(pixelSize);
#endif
        m_font->setValue(font);
        m_color->setValue(color);
        m_label->setEnabled(true);
    }
    else if(itemId == item_id_list)
    {
        m_listWidth->setValue((int)r.width());
        m_listHeight->setValue((int)r.height());
        m_list->setEnabled(true);
    }
    else if(itemId == item_id_table)
    {
        m_tableWidth->setValue((int)r.width());
        m_tableHeight->setValue((int)r.height());
        m_tableRow->setValue(item->data(item_table_row).toInt());
        m_tableColumn->setValue(item->data(item_table_column).toInt());
        m_tableOffsetX->setValue(item->data(item_table_offset_x).toInt());
        m_tableOffsetY->setValue(item->data(item_table_offset_y).toInt());
        m_tableSpanWidth->setValue(item->data(item_table_span_width).toInt());
        m_tableSpanHeight->setValue(item->data(item_table_span_height).toInt());
        m_table->setEnabled(true);
    }
    else if(itemId == item_id_bar)
    {
        QString path = item->data(item_bar_bgImage).toString();
        m_barBgImage->setValue(path);
        path = item->data(item_bar_barImage).toString();
        m_barBarImage->setValue(path);
        m_bar->setEnabled(true);
    }

    m_opacity->setValue(item->opacity());
    m_rotate->setValue(item->rotation());

    bool isKeepRatio = item->data(item_keepRatio).toBool();
    if(isKeepRatio == true)
    {
        m_ratio->setValue(item->data(item_ratio));
        m_ratio->setEnabled(true);
        m_width->setEnabled(false);
        m_height->setEnabled(false);
    }
    else
    {
        m_ratio->setEnabled(false);
        m_width->setValue(r.width());
        m_width->setEnabled(true);
        m_height->setValue(r.height());
        m_height->setEnabled(true);
    }
    m_keepRatio->setValue(isKeepRatio);
}

void MainWindow::updatePropertyTreeBy(QGraphicsItem* item)
{
    if(item == 0)
    {
        return ;
    }

    QRectF r = item->boundingRect();
    m_x->setValue((int)((int)item->x() + (int)((r.width() + 1)/2)));
    m_y->setValue((int)((int)item->y() + (int)((r.height() + 1)/2)));
    this->setWindowTitleShowSaveMark(false);
}

bool MainWindow::isValidItem(QGraphicsItem* item)
{
    if(item == 0)
    {
        return false;
    }

    if(item->data(item_id).toInt() == item_id)
    {
        return false;
    }

    return true;
}

bool MainWindow::isOverLoadObjectName(QString newName)
{
    QList<QGraphicsItem*> items = m_scene->items();

    foreach (QGraphicsItem* item, items)
    {
        QString name = item->data(item_name).toString();

        if(name == newName)
        {
            return true;
        }
    }

    return false;
}

int MainWindow::createZValue()
{
    int z = 0;
    int count = m_scene->items().count();
    for(int i = 0; i < count; ++i)
    {
        QGraphicsItem* item = m_scene->items().at(i);
        if( item->data(item_id).toInt() != item_id)
        {
            z++;
        }
    }

    return z;
}

QString MainWindow::workingPath()
{
    QSettings settings("yuguosoft.com");
    QString workingPath = settings.value("workingPath").toString();

    if(workingPath.isEmpty() == true)
    {
        this->statusBar()->showMessage(tr("working Path is NULL"));
    }

    return workingPath;
}

QString MainWindow::valueInLine(QString line, QString key)
{
    if(key.isEmpty() == true)
    {
        return QString();
    }

    if(key == "id")
    {
        int index = line.indexOf(" ");
        if(index == -1)
        {
            return QString();
        }

        return line.mid(1, index - 1);
    }

    int index = line.indexOf(key);

    if(index == -1)
    {
        return QString();
    }

    int index1 = line.indexOf("\"", index);
    int index2 = line.indexOf("\"", index1 + 1);
    return line.mid(index1+1, index2 - index1 - 1);
}

MainWindow::~MainWindow()
{

}

//read and write
void MainWindow::readUI()
{
    QFile file(m_saveFileFullPath);
    if(file.open(QIODevice::ReadOnly) == false)
    {
        QMessageBox::warning(this, tr("warning"), tr("open file failed!"));
        return ;
    }

    m_listWidget->clear();

    QTextStream in(&file);
    in.setCodec("UTF-8");

    while(in.atEnd() == false)
    {
        QString line = in.readLine();
        line.remove("<");
        line.remove(">");
        QStringList list = line.trimmed().split(" ");

        if(list.first() == "layer")
        {
            foreach(QString str, list)
            {
                if(str.contains("width") == true)
                {
                    m_sceneSize.setWidth( str.split("=").last().remove("\"").toInt());
                }
                else if(str.contains("height") == true)
                {
                    m_sceneSize.setHeight( str.split("=").last().remove("\"").toInt());
                }
            }

            QRectF r = QRectF(QPointF(0,0),m_sceneSize);
            m_scene->setSceneRect(r);
            m_graphicsView->setFixedSize(m_sceneSize.width(), m_sceneSize.height());
            m_graphicsView->show();
        }
        else if(list.first() == "sprite" ||
                list.first() == "menu" ||
                list.first() == "label" ||
                list.first() == "list" ||
                list.first() == "bar" ||
                list.first() == "table")
        {
            QGraphicsItem* item = this->itemFromLine(line);

            if(item != 0)
            {
                m_scene->addItem(item);
                QListWidgetItem* it = new QListWidgetItem(item->data(item_name).toString());
                it->setCheckState(Qt::Unchecked);
                m_listWidget->addItem(it);
            }
        }
    }
}

void MainWindow::writeUI()
{
    QFile file(m_saveFileFullPath);
    if(file.open(QIODevice::WriteOnly) == false)
    {
        QMessageBox::warning(this, tr("warning"), tr("open file failed!"));
        return ;
    }

    QTextStream in(&file);
    in.setCodec("UTF-8");

    QString str = QString("<layer width=\"%1\" height=\"%2\">").arg(m_sceneSize.width()).arg(m_sceneSize.height());
    in<<str<<"\n";

    QList<QGraphicsItem*> items = m_scene->items();

    foreach (QGraphicsItem* item, items)
    {
        int data = item->data(item_id).toInt();
        if(data == item_id_sprite ||
           data == item_id_menu ||
           data == item_id_label ||
           data == item_id_list ||
           data == item_id_bar ||
           data == item_id_table)
        {
            QString str = this->lineFromItem(item);
            in<<str<<"\n";
        }
    }

    str = QString("</layer>");
    in<<str;

    file.close();
}

//translate withe string sceneItem widgetItem

QString MainWindow::lineFromItem(QGraphicsItem* item)
{
    int data = item->data(item_id).toInt();

    QString str = "";
    if(data == item_id_sprite)
    {
        str.append("<sprite ");
    }
    else if(data == item_id_menu)
    {
        str.append("<menu ");
    }
    else if(data == item_id_label)
    {
        str.append("<label ");
    }
    else if(data == item_id_list)
    {
        str.append("<list ");
    }
    else if(data == item_id_bar)
    {
        str.append("<bar ");
    }
    else if(data == item_id_table)
    {
        str.append("<table ");
    }

    QString name = item->data(item_name).toString();
    str.append( QString("name=\"%1\" ").arg(name) );

    QRectF r = item->boundingRect();
    int x = (int)((int)item->x() + (int)((r.width() + 1)/2));
    int y = (int)((int)item->y() + (int)((r.height() + 1)/2));
    int z = (int)item->zValue();

    str.append( QString("x=\"%1\" y=\"%2\" z=\"%3\" ").arg(x).arg(y).arg(z));

    int rotate = (int)item->rotation();
    if(rotate != 0)
    {
        str.append(QString("rotation=\"%1\" ").arg(rotate));
    }

    float opacity = (float)item->opacity();
    if(opacity < 0.98)
    {
        str.append(QString("opacity=\"%1\" ").arg(opacity));
    }

    if(item->isVisible() == false)
    {
        str.append(QString("visible=\"false\" "));
    }

    if(item->isEnabled() == false)
    {
        str.append(QString("enable=\"false\" "));
    }

    if(data == item_id_sprite)
    {
        QString path = item->data(item_sprite_image).toString();
        path.remove(this->workingPath() + QString("/"));
        str.append( QString("imagePath=\"%1\" ").arg(path) );

        bool isKeepRatio = item->data(item_keepRatio).toBool();
        if(isKeepRatio == true)
        {
            double temp = item->data(item_ratio).toDouble();

            if(temp < 0.99 || temp > 1.01)
            {
                str.append(QString("keepRatio=\"true\" "));
                str.append(QString("ratio=\"%1\" ").arg(temp));
            }
        }
        else
        {
            str.append(QString("keepRatio=\"false\" "));
            str.append(QString("width=\"%1\" height=\"%2\" ").arg(item->data(item_scaled_width).toDouble()).arg(item->data(item_scaled_height).toDouble()));
        }
    }
    else if(data == item_id_menu)
    {
        QString name = item->data(item_menu_normalImage).toString();
        name.remove(this->workingPath() + QString("/"));
        QString normal = name;
        str.append( QString("normalImage=\"%1\" ").arg(normal) );
        bool isSingle = item->data(item_menu_singleImage).toBool();

        if(isSingle == false)
        {
            QString press = name.replace(".png","_Light.png");
            str.append( QString("pressImage=\"%1\" ").arg(press) );
        }
    }
    else if(data == item_id_label)
    {
        bool edit = item->data(item_label_supportEdit).toBool();
        if(edit == true)
        {
            str.append(QString("edit=\"true\" "));
        }

        bool click = item->data(item_label_supportClick).toBool();
        if(click == true)
        {
            str.append(QString("click=\"true\" "));
        }

        QString temp = item->data(item_label_text).toString();
        str.append(QString("text=\"%1\" ").arg(temp));

        QFont font = item->data(item_label_font).value<QFont>();

        if(m_defaultFont.family() != font.family())
        {
            str.append(QString("fontName=\"%1\" ").arg(font.family()));
        }

        int pointSize = font.pointSize();

#ifdef Q_WS_MAC
        if(pointSize != m_defaultFont.pointSize())
        {
            str.append(QString("fontSize=\"%1\" ").arg(pointSize));
        }
#endif

#ifdef Q_WS_WIN
        if(pointSize != m_defaultFont.pixelSize())
        {
            str.append(QString("fontSize=\"%1\" ").arg(pointSize));
        }
#endif
        QColor color = item->data(item_label_color).value<QColor>();

        if(color.red() == 255 && color.green() == 255 && color.blue() == 255)
        {}
        else
        {
            str.append(QString("colorR=\"%1\" ").arg(color.red()));
            str.append(QString("colorG=\"%1\" ").arg(color.green()));
            str.append(QString("colorB=\"%1\" ").arg(color.blue()));
        }
    }
    else if(data == item_id_list)
    {
        int width = r.width();
        int height = r.height();
        str.append( QString("listWidth=\"%1\" listHeight=\"%2\" ").arg(width).arg(height));
    }
    else if(data == item_id_table)
    {
        int width = r.width();
        int height = r.height();
        str.append( QString("tableWidth=\"%1\" tableHeight=\"%2\" ").arg(width).arg(height));
        int row = item->data(item_table_row).toInt();
        int column = item->data(item_table_column).toInt();
        str.append( QString("tableRow=\"%1\" tableColumn=\"%2\" ").arg(row).arg(column));
        int offsetX = item->data(item_table_offset_x).toInt();
        int offsetY = item->data(item_table_offset_y).toInt();
        str.append( QString("tableOffsetX=\"%1\" tableOffsetY=\"%2\" ").arg(offsetX).arg(offsetY));
        int spanWidth = item->data(item_table_span_width).toInt();
        int spanHeight = item->data(item_table_span_height).toInt();
        str.append( QString("tableSpanWidth=\"%1\" tableSpanHeight=\"%2\" ").arg(spanWidth).arg(spanHeight));
    }
    else if(data == item_id_bar)
    {
        QString path = item->data(item_bar_bgImage).toString();
        path.remove(this->workingPath() + QString("/"));
        str.append( QString("bgImage=\"%1\" ").arg(path) );

        path = item->data(item_bar_barImage).toString();
        path.remove(this->workingPath() + QString("/"));
        str.append( QString("barImage=\"%1\" ").arg(path) );
    }

    str.append("/>");

    return str;
}

QGraphicsItem* MainWindow::itemFromLine(QString line)
{
    QGraphicsItem* item = 0;
    line.remove("<");
    line.remove(">");
    QStringList list = line.trimmed().split(" ");

    int data = 0;
    int x = 0;
    int y = 0;
    int z = 0;
    int rotation = 0;
    float opacity = 1.0f;
    bool visible = true;
    bool enable = true;
    bool edit = false;
    bool click = false;
    bool keepRatio = true;
    double ratio = 1.0f;
    int width = 0;
    int height = 0;
    int listWidth = 50;
    int listHeight = 50;
    int tableWidth = 50;
    int tableHeight = 50;
    int tableRow = 1;
    int tableColumn = 1;
    int tableOffsetX = 1;
    int tableOffsetY = 1;
    int tableSpanWidth = 1;
    int tableSpanHeight = 1;
    QString nodeName;
    QString path;
    QFont font = m_defaultFont;
    QColor color = QColor(Qt::white);
    bool isSingle = true;
    QString barBgImage;
    QString barBarImage;

    for(int i = 0; i < list.size(); ++i)
    {
        QString str = list.at(i);

        if(i == 0)
        {
            if(str.contains("sprite") == true)
            {
                data = item_id_sprite;
            }
            else if(str.contains("menu") == true)
            {
                data = item_id_menu;
            }
            else if(str.contains("label") == true)
            {
                data = item_id_label;
            }
            else if(str.contains("list") == true)
            {
                data = item_id_list;
            }
            else if(str.contains("bar") == true)
            {
                data = item_id_bar;
            }
            else if(str.contains("table") == true)
            {
                data = item_id_table;
            }
        }
        else if(i == 1)
        {
            if(str.contains("name=") == true)
            {
                nodeName = str.split("=").last().remove("\"");
            }
        }
        else if(i == 2)
        {
            if(str.contains("x=") == true)
            {
                x = str.split("=").last().remove("\"").toInt();
            }
        }
        else if(i == 3)
        {
            if(str.contains("y=") == true)
            {
                y = str.split("=").last().remove("\"").toInt();
            }
        }
        else if(i == 4)
        {
            if(str.contains("z=") == true)
            {
                z = str.split("=").last().remove("\"").toInt();
            }
        }
        else
        {
            if(str.contains("rotation=") == true)
            {
                rotation = str.split("=").last().remove("\"").toInt();
            }
            else if(str.contains("opacity=") == true)
            {
                opacity = str.split("=").last().remove("\"").toDouble();
            }
            else if(str.contains("visible=") == true)
            {
                visible = false;
            }
            else if(str.contains("enable=") == true)
            {
                enable = false;
            }
            else if(str.contains("edit=") == true)
            {
                edit = true;
            }
            else if(str.contains("click=") == true)
            {
                click = true;
            }
            else if(str.contains("keepRatio=\"true\""))
            {
                keepRatio = true;
            }
            else if(str.contains("keepRatio=\"false\""))
            {
                keepRatio = false;
            }
            else if(str.contains("ratio=") == true)
            {
                ratio = str.split("=").last().remove("\"").toDouble();
            }
            else if(str.contains("width=") == true && str.contains("listWidth=") == false)
            {
                width = str.split("=").last().remove("\"").toInt();
            }
            else if(str.contains("height=") == true && str.contains("listHeight=") == false)
            {
                height = str.split("=").last().remove("\"").toInt();
            }
            else if(str.contains("listWidth=") == true)
            {
                listWidth = str.split("=").last().remove("\"").toInt();
            }
            else if(str.contains("listHeight") == true)
            {
                listHeight = str.split("=").last().remove("\"").toInt();
            }
            else if(str.contains("tableWidth=") == true)
            {
                tableWidth = str.split("=").last().remove("\"").toInt();
            }
            else if(str.contains("tableHeight=") == true)
            {
                tableHeight = str.split("=").last().remove("\"").toInt();
            }
            else if(str.contains("tableRow=") == true)
            {
                tableRow = str.split("=").last().remove("\"").toInt();
            }
            else if(str.contains("tableColumn=") == true)
            {
                tableColumn = str.split("=").last().remove("\"").toInt();
            }
            else if(str.contains("tableOffsetX=") == true)
            {
                tableOffsetX = str.split("=").last().remove("\"").toInt();
            }
            else if(str.contains("tableOffsetY=") == true)
            {
                tableOffsetY = str.split("=").last().remove("\"").toInt();
            }
            else if(str.contains("tableSpanWidth=") == true)
            {
                tableSpanWidth = str.split("=").last().remove("\"").toInt();
            }
            else if(str.contains("tableSpanHeight=") == true)
            {
                tableSpanHeight = str.split("=").last().remove("\"").toInt();
            }
            else if(str.contains("fontName") == true)
            {
                QString family = str.split("=").last().remove("\"");
                font.setFamily(family);
            }
            else if(str.contains("fontSize") == true)
            {
                int pointSize = str.split("=").last().remove("\"").toInt();
                font.setPointSize(pointSize);
#ifdef Q_WS_WIN
                font.setPixelSize(pointSize);
#endif
            }
            else if(str.contains("colorR") == true)
            {
                int red = str.split("=").last().remove("\"").toInt();
                color.setRed(red);
            }
            else if(str.contains("colorG") == true)
            {
                int green = str.split("=").last().remove("\"").toInt();
                color.setGreen(green);
            }
            else if(str.contains("colorB") == true)
            {
                int blue = str.split("=").last().remove("\"").toInt();
                color.setBlue(blue);
            }
            else if(str.contains("imagePath=") ||
                    str.contains("normalImage="))
            {
                path = str.split("=").last().remove("\"");
            }
            else if(str.contains("text=") == true)
            {
                path = this->valueInLine(str, "text");
            }
            else if(str.contains("pressImage="))
            {
                isSingle = false;
            }
            else if(str.contains("bgImage") == true)
            {
                barBgImage = str.split("=").last().remove("\"");
            }
            else if(str.contains("barImage") == true)
            {
                barBarImage = str.split("=").last().remove("\"");
            }
        }
    }

    if(data == item_id_sprite)
    {
        QString filePath = this->workingPath() + "/" + path;
        QPixmap pixmap(filePath);

        if(pixmap.isNull() == true)
        {
            qDebug()<<line;
            QMessageBox::warning(this,"miss file", filePath);
        }

        QSize s = pixmap.size();

        if(keepRatio == true)
        {
            s = QSize(s.width()*ratio, s.height()*ratio);
        }
        else
        {
            s = QSize(width, height);
        }

        item = new QGraphicsPixmapItem(pixmap.scaled(s));
        QRectF r = item->boundingRect();
        item->setX(x - (int)((r.width() + 1)/2));
        item->setY(y - (int)((r.height() + 1)/2));
        item->setZValue(z);
        item->setRotation(rotation);
        item->setOpacity(opacity);
        item->setVisible(visible);
        item->setEnabled(enable);
        item->setData(item_id, data);
        item->setData(item_name, nodeName);
        item->setData(item_keepRatio, keepRatio);
        item->setData(item_ratio, ratio);
        item->setData(item_scaled_width, width);
        item->setData(item_scaled_height, height);
        item->setData(item_sprite_image, filePath);
        item->setFlag(QGraphicsItem::ItemIsMovable);
    }
    else if(data == item_id_menu)
    {
        QString filePath = this->workingPath() + "/" + path;
        QPixmap pixmap(filePath);

        if(pixmap.isNull() == true)
        {
            qDebug()<<line;
            QMessageBox::warning(this,"miss file", filePath);
        }

        item = new QGraphicsPixmapItem(pixmap);
        QRectF r = item->boundingRect();
        item->setX(x - (int)((r.width() + 1)/2));
        item->setY(y - (int)((r.height() + 1)/2));
        item->setZValue(z);
        item->setRotation(rotation);
        item->setOpacity(opacity);
        item->setVisible(visible);
        item->setEnabled(enable);
        item->setData(item_id, data);
        item->setData(item_name, nodeName);
        item->setData(item_menu_normalImage, filePath);
        item->setData(item_menu_singleImage, isSingle);

        if(isSingle == false)
        {
            item->setData(item_menu_pressImage, filePath.replace(".png", "_Light.png"));
        }

        item->setFlag(QGraphicsItem::ItemIsMovable);
    }
    else if(data == item_id_label)
    {
        item = new QGraphicsTextItem(path);
        item->setZValue(z);
        item->setRotation(rotation);
        item->setOpacity(opacity);
        item->setVisible(visible);
        item->setEnabled(enable);
        item->setData(item_id, data);
        item->setData(item_name, nodeName);
        item->setData(item_label_text, path);
        item->setData(item_label_font, font);
        item->setData(item_label_color, color);
        item->setData(item_label_supportEdit, edit);
        item->setData(item_label_supportClick, click);
        item->setFlag(QGraphicsItem::ItemIsMovable);

        QGraphicsTextItem* label = (QGraphicsTextItem*)item;
        label->setPlainText(path);
        label->setFont(font);
        label->setDefaultTextColor(color);

        QRectF r = item->boundingRect();
        item->setX(x - (int)((r.width() + 1)/2));
        item->setY(y - (int)((r.height() + 1)/2));
    }
    else if(data == item_id_list)
    {
        item = new QGraphicsRectItem(QRectF(0,0,listWidth,listHeight));
        item->setPos(x - listWidth/2, y - listHeight/2);
        item->setZValue(z);
        item->setRotation(rotation);
        item->setOpacity(opacity);
        item->setVisible(visible);
        item->setEnabled(enable);
        item->setData(item_id, data);
        item->setData(item_name, nodeName);
        item->setFlag(QGraphicsItem::ItemIsMovable);
        QGraphicsRectItem* rectItem =  (QGraphicsRectItem*)item;
        rectItem->setPen(QPen(QColor(Qt::white)));
        rectItem->setBrush(QBrush(QColor(Qt::white)));
    }
    else if(data == item_id_table)
    {
        item = new QGraphicsRectItem(QRectF(0,0,tableWidth,tableHeight));
        item->setPos(x - tableWidth/2, y - tableHeight/2);
        item->setZValue(z);
        item->setRotation(rotation);
        item->setOpacity(opacity);
        item->setVisible(visible);
        item->setEnabled(enable);
        item->setData(item_id, data);
        item->setData(item_name, nodeName);
        item->setData(item_table_row, tableRow);
        item->setData(item_table_column, tableColumn);
        item->setData(item_table_offset_x, tableOffsetX);
        item->setData(item_table_offset_y, tableOffsetY);
        item->setData(item_table_span_width, tableSpanWidth);
        item->setData(item_table_span_height, tableSpanHeight);
        item->setFlag(QGraphicsItem::ItemIsMovable);
        QGraphicsRectItem* rectItem =  (QGraphicsRectItem*)item;
        rectItem->setPen(QPen(QColor(Qt::white)));
        rectItem->setBrush(QBrush(QColor(Qt::white)));
    }
    else if(data == item_id_bar)
    {
        barBgImage = this->workingPath() + "/" + barBgImage;
        barBarImage = this->workingPath() + "/" + barBarImage;
        QPixmap pixmap(barBgImage);

        if(pixmap.isNull() == true)
        {
            qDebug()<<line;
            QMessageBox::warning(this,"miss file", barBgImage);
        }

        item = new QGraphicsPixmapItem(pixmap);
        QRectF r = item->boundingRect();
        item->setX(x - (int)((r.width() + 1)/2));
        item->setY(y - (int)((r.height() + 1)/2));
        item->setZValue(z);
        item->setRotation(rotation);
        item->setOpacity(opacity);
        item->setVisible(visible);
        item->setEnabled(enable);
        item->setData(item_id, data);
        item->setData(item_name, nodeName);
        item->setData(item_bar_bgImage, barBgImage);
        item->setData(item_bar_barImage, barBarImage);
        item->setFlag(QGraphicsItem::ItemIsMovable);
    }

    return item;
}

QListWidgetItem* MainWindow::listWidgetItemFromGraphicsItem(QGraphicsItem* graphicsItem)
{
    Q_ASSERT(graphicsItem != 0);

    QString nodeName = graphicsItem->data(item_name).toString();
    int count = m_listWidget->count();

    for(int i = 0; i < count; ++i)
    {
        QListWidgetItem* item = m_listWidget->item(i);

        if(item->text() == nodeName)
        {
            return item;
        }
    }

    return 0;
}

QGraphicsItem* MainWindow::graphicsItemFromListWidgetItem(QListWidgetItem* listWidgetItem)
{
    Q_ASSERT(listWidgetItem != 0);

    QString nodeName = listWidgetItem->text();
    QList<QGraphicsItem*> items = m_scene->items();

    foreach (QGraphicsItem* it, items)
    {
        if(nodeName == it->data(item_name).toString())
        {
            return it;
        }
    }

    return 0;
}

//system event.

//void MainWindow::keyPressEvent(QKeyEvent *e)
//{
//    if( e->key() == Qt::Key_Control)
//    {
//        m_isCtrlEnable = true;
//    }

//    QMainWindow::keyPressEvent(e);
//}

//void MainWindow::keyReleaseEvent(QKeyEvent *e)
//{
//    if( e->key() == Qt::Key_Control)
//    {
//        m_isCtrlEnable = false;
//    }

//    QMainWindow::keyReleaseEvent(e);
//}

void MainWindow::closeEvent(QCloseEvent* e)
{
    if(m_isSaved == false && m_saveFileFullPath.isEmpty() == false)
    {
        QMessageBox msgBox;
        msgBox.setText(tr("The document has been modified."));
        msgBox.setInformativeText(tr("Do you want to save your changes?"));
        msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::No | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Save);
        int ret = msgBox.exec();

        if(ret == QMessageBox::Cancel)
        {
            e->ignore();
            return ;
        }

        if(ret == QMessageBox::Save)
        {
            this->clickSave();
        }
    }

    QRect mainR = this->geometry();
    QRect listR = m_listWidget->geometry();
    QRect viewR = m_graphicsView->geometry();
    QRect treeR = m_browser->geometry();

    QSettings settings("yuguosoft.com");
    settings.setValue("mainR" , mainR);
    settings.setValue("listR" , listR);
    settings.setValue("viewR" , viewR);
    settings.setValue("treeR" , treeR);

    return QMainWindow::closeEvent(e);
}
