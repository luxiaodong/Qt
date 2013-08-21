#include "qsizeselect.h"
#include "ui_qsizeselect.h"

QSizeSelect::QSizeSelect(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QSizeSelect)
{
    ui->setupUi(this);

    connect(ui->ok, SIGNAL(clicked()), this, SLOT(accept()));
    connect(ui->cancel, SIGNAL(clicked()), this, SLOT(reject()));
    connect(ui->radio1024, SIGNAL(clicked()), this, SLOT(click1024()));
    connect(ui->radio768, SIGNAL(clicked()), this, SLOT(click768()));
    connect(ui->radio480, SIGNAL(clicked()), this, SLOT(click480()));
    connect(ui->radio320, SIGNAL(clicked()), this, SLOT(click320()));
    connect(ui->custom, SIGNAL(clicked()), this, SLOT(clickCustom()));

    ui->radio768->setChecked(true);
    m_size = QSize(768, 1024);
}

QSizeSelect::~QSizeSelect()
{
    delete ui;
}

void QSizeSelect::click1024()
{
    ui->radio1024->setChecked(true);
    ui->radio768->setChecked(false);
    ui->radio480->setChecked(false);
    ui->radio320->setChecked(false);
    ui->custom->setChecked(false);

    m_size = QSize(1024, 768);
}

void QSizeSelect::click768()
{
    ui->radio1024->setChecked(false);
    ui->radio768->setChecked(true);
    ui->radio480->setChecked(false);
    ui->radio320->setChecked(false);
    ui->custom->setChecked(false);

    m_size = QSize(768, 1024);
}

void QSizeSelect::click480()
{
    ui->radio1024->setChecked(false);
    ui->radio768->setChecked(false);
    ui->radio480->setChecked(true);
    ui->radio320->setChecked(false);
    ui->custom->setChecked(false);

    m_size = QSize(480, 320);
}

void QSizeSelect::click320()
{
    ui->radio1024->setChecked(false);
    ui->radio768->setChecked(false);
    ui->radio480->setChecked(false);
    ui->radio320->setChecked(true);
    ui->custom->setChecked(false);

    m_size = QSize(320, 480);
}

void QSizeSelect::clickCustom()
{
    ui->radio1024->setChecked(false);
    ui->radio768->setChecked(false);
    ui->radio480->setChecked(false);
    ui->radio320->setChecked(false);
    ui->custom->setChecked(true);
}

QSize QSizeSelect::selectSize()
{
    if(ui->custom->isChecked() == true)
    {
        m_size = QSize(ui->width->value(), ui->height->value());
    }

    return m_size;
}
