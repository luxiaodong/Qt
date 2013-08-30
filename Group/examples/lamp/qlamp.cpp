#include "qlamp.h"
#include "ui_qlamp.h"

Qlamp::Qlamp(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Qlamp)
{
    ui->setupUi(this);

    //QPermuteGroup group(3);
    //this->printTrace(group.conjugateClass());
    this->printGroupMatrix();
    this->ui->table->setVisible(false);

    int horz = 3;
    int vert = 3;
    m_logic.setGildNumber(horz,vert);

//type 0,nill, 1,O  2,X
    for(int i = 0; i < horz; i++)
    {
        for(int j = 0; j < vert; j++)
        {
            m_logic.setGild(1,2);
        }
    }
}

Qlamp::~Qlamp()
{
    delete ui;
}

void Qlamp::paintEvent(QPaintEvent *e)
{
    (void)e;
    QRect rect = this->geometry();
    QPainter p;
    p.setViewport(rect);
    p.setWindow(QRect(0,0,rect.width(), rect.height()));
    p.begin(this);
    p.setPen(QColor(255,0,0));

    for(int i = 0; i < m_logic.horzGildNumber() - 1; i++)
    {
        int xPos = rect.width()*(i+1)/m_logic.horzGildNumber();
        p.drawLine(xPos, 0, xPos, rect.height());
    }

    for(int i = 0; i < m_logic.vertGildNumber() - 1; i++)
    {
        int yPos = rect.height()*(i+1)/m_logic.vertGildNumber();
        p.drawLine(0, yPos, rect.width(), yPos);
    }

    for(int i = 0; i < m_logic.horzGildNumber(); i++)
    {
        for(int j = 0; j < m_logic.vertGildNumber(); j++)
        {
            int drawType = m_logic.drawType(i,j);

            if(drawType == (int)DRAW_NONE)
            {
            }
            else if(drawType == (int)DRAW_O)
            {
                int xPos = rect.width()*(i)/m_logic.horzGildNumber();
                int yPos = rect.height()*(j)/m_logic.vertGildNumber();
                p.drawEllipse(xPos,yPos,rect.width()/m_logic.horzGildNumber(),rect.height()/m_logic.vertGildNumber());
            }
            else if(drawType == (int)DRAW_X)
            {
                int xPos = rect.width()*(i)/m_logic.horzGildNumber();
                int yPos = rect.height()*(j)/m_logic.vertGildNumber();
                p.drawLine(xPos, yPos, xPos + rect.width()/m_logic.horzGildNumber(), yPos + rect.height()/m_logic.vertGildNumber());
                p.drawLine(xPos + rect.width()/m_logic.horzGildNumber(), yPos, xPos, yPos + rect.height()/m_logic.vertGildNumber());
            }
        }
    }

    p.end();
}

void Qlamp::mousePressEvent( QMouseEvent * e )
{
    const QPoint pt = e->pos();
    QRect rect = this->geometry();

    int x = (pt.x() - 0)*m_logic.horzGildNumber()/rect.width();
    int y = (pt.y() - 0)*m_logic.vertGildNumber()/rect.height();

    m_logic.mainFlip(x,y);
    this->update();
}

// for test code
QString Qlamp::printTrace(GMatrix m)
{
    QString str = QString("");
    gint count = m.size();

    for(gint i = 0; i < count; i++)
    {
        QString subStr = QString("(");
        gint size = m.at(i).size();
        for(gint j = 0; j < size; j++)
        {
            subStr += QString("%1").arg(m.at(i).at(j) + 1);
            if(j != size - 1)
            {
                subStr += ",";
            }
        }

        subStr += ")";
        str += subStr;
    }

    return str;
}

void Qlamp::printPermuteGroup()
{
    QPermuteGroup group;
    group.createPermute(4);
    gint rank = group.rank();
    ui->table->setColumnCount(rank);
    ui->table->setRowCount(rank);

    for(int j = 0; j < rank; ++j)
    {
        for(int i = 0; i < rank; ++i)
        {
            int e = group.at(j,i);
            QString str = this->printTrace(group.trace(e));
            QTableWidgetItem* item = new QTableWidgetItem(str);
            ui->table->setItem(j,i,item);
        }
    }

    QTableWidgetItem* item = ui->table->item(0,0);
    item->setText(this->printTrace(group.conjugateClass()));
}

void Qlamp::printGroupMatrix()
{
    QCongruenceGroup group(7);
    //group.createDihedral(4);
    gint rank = group.rank();
    ui->table->setColumnCount(rank);
    ui->table->setRowCount(rank);

    for(int j = 0; j < rank; ++j)
    {
        for(int i = 0; i < rank; ++i)
        {
            gint e = group.at(j,i);
            QString str = QString("%1").arg(e+1);
            //QString str = this->printTrace(group.conjugateClass());
            QTableWidgetItem* item = new QTableWidgetItem(str);
            ui->table->setItem(j,i,item);
        }
    }

    QTableWidgetItem* item = ui->table->item(0,0);
    item->setText(this->printTrace(group.conjugateClass()));
}
