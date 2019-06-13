#include "w_sys_manage_outer_settings2.h"
#include "ui_w_sys_manage_outer_settings2.h"
#include <QScrollBar>

w_sys_manage_outer_settings2::w_sys_manage_outer_settings2(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::w_sys_manage_outer_settings2)
{
    ui->setupUi(this);
    ui->pushButton->setText(QObject::tr("结账打印机") + "\n" + "更多");
    ui->pushButton_2->setText(QObject::tr("开钱箱") + "\n" + "更多");
    ui->pushButton_3->setText(QObject::tr("厨房打印机") + "\n" + "更多");
    ui->pushButton_4->setText(QObject::tr("标签打印机") + "\n" + "更多");
    ui->pushButton_5->setText(QObject::tr("客显") + "\n" + "更多");
    ui->pushButton_6->setText(QObject::tr("副屏") + "\n" + "更多");
    ui->pushButton_7->setText(QObject::tr("前台计价秤") + "\n" + "更多");
    ui->pushButton_8->setText(QObject::tr("后台条码秤") + "\n" + "更多");
    ui->pushButton_9->setText(QObject::tr("无线点菜机") + "\n" + "更多");
    ui->pushButton_10->setText(QObject::tr("平板") + "\n" + "更多");
    ui->pushButton_11->setText(QObject::tr("厨显") + "\n" + "更多");
    ui->pushButton_12->setText(QObject::tr("电话盒") + "\n" + "更多");

    connect(ui->pushButton_ok, SIGNAL(clicked()),this,SLOT(took()));
    connect(ui->pushButton_cancel, SIGNAL(clicked()),this,SLOT(tocancel()));

    connect(ui->pushButton_PageDown, SIGNAL(clicked()),this,SLOT(toPageDown()));
    connect(ui->pushButton_PageUp, SIGNAL(clicked()),this,SLOT(toPageUp()));
}

w_sys_manage_outer_settings2::~w_sys_manage_outer_settings2()
{
    delete ui;
}

void w_sys_manage_outer_settings2::took()
{

}

void w_sys_manage_outer_settings2::tocancel()
{
    this->reject();
}

void w_sys_manage_outer_settings2::toPageDown()
{
    QScrollBar *scrollBar = ui->scrollArea->verticalScrollBar();
    scrollBar->setValue(scrollBar->value()+scrollBar->pageStep());
}

void w_sys_manage_outer_settings2::toPageUp()
{
    QScrollBar *scrollBar = ui->scrollArea->verticalScrollBar();
    scrollBar->setValue(scrollBar->value()-scrollBar->pageStep());
}
