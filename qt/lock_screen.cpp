#include "lock_screen.h"
#include "ui_lock_screen.h"
#include <QSqlRecord>
#include "lds_messagebox.h"
#include "n_func.h"
#include <QDebug>
#include "backheader.h"
#include "lds.h"
#include <QSqlQueryModel>

lock_screen::lock_screen(const QString &str,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::lock_screen)
{
    ui->setupUi(this);
    type = str;
    ui->lineEdit_2->setEchoMode(QLineEdit::Password);
    ui->lineEdit->setFocus();
    ui->lineEdit->setText(lds::gs_operid);
    ui->lineEdit_2->installEventFilter(this);
    this->installEventFilter(this);
    ui->lineEdit_2->setFocus();
}

lock_screen::~lock_screen()
{
    delete ui;
}

void lock_screen::on_pushButton_clicked()//1
{
    if(ui->lineEdit->hasFocus())
    {
        ui->lineEdit->insert("1");
    }
    if(ui->lineEdit_2->hasFocus())
    {
        ui->lineEdit_2->insert("1");
    }
}

void lock_screen::on_pushButton_2_clicked()//2
{
    if(ui->lineEdit->hasFocus())
    {
        ui->lineEdit->insert("2");
    }
    if(ui->lineEdit_2->hasFocus())
    {
        ui->lineEdit_2->insert("2");
    }
}

void lock_screen::on_pushButton_3_clicked()//3
{
    if(ui->lineEdit->hasFocus())
    {
        ui->lineEdit->insert("3");
    }
    if(ui->lineEdit_2->hasFocus())
    {
        ui->lineEdit_2->insert("3");
    }
}

void lock_screen::on_pushButton_5_clicked()//4
{
    if(ui->lineEdit->hasFocus())
    {
        ui->lineEdit->insert("4");
    }
    if(ui->lineEdit_2->hasFocus())
    {
        ui->lineEdit_2->insert("4");
    }
}

void lock_screen::on_pushButton_8_clicked()//5
{
    if(ui->lineEdit->hasFocus())
    {
        ui->lineEdit->insert("5");
    }
    if(ui->lineEdit_2->hasFocus())
    {
        ui->lineEdit_2->insert("5");
    }
}

void lock_screen::on_pushButton_7_clicked()//6
{
    if(ui->lineEdit->hasFocus())
    {
        ui->lineEdit->insert("6");
    }
    if(ui->lineEdit_2->hasFocus())
    {
        ui->lineEdit_2->insert("6");
    }
}

void lock_screen::on_pushButton_6_clicked()//7
{
    if(ui->lineEdit->hasFocus())
    {
        ui->lineEdit->insert("7");
    }
    if(ui->lineEdit_2->hasFocus())
    {
        ui->lineEdit_2->insert("7");
    }
}

void lock_screen::on_pushButton_13_clicked()//8
{
    if(ui->lineEdit->hasFocus())
    {
        ui->lineEdit->insert("8");
    }
    if(ui->lineEdit_2->hasFocus())
    {
        ui->lineEdit_2->insert("8");
    }
}

void lock_screen::on_pushButton_12_clicked()//9
{
    if(ui->lineEdit->hasFocus())
    {
        ui->lineEdit->insert("9");
    }
    if(ui->lineEdit_2->hasFocus())
    {
        ui->lineEdit_2->insert("9");
    }
}

void lock_screen::on_pushButton_10_clicked()//0
{
    if(ui->lineEdit->hasFocus())
    {
        ui->lineEdit->insert("0");
    }
    if(ui->lineEdit_2->hasFocus())
    {
        ui->lineEdit_2->insert("0");
    }
}

void lock_screen::on_pushButton_9_clicked()//.
{
    if(ui->lineEdit->hasFocus())
    {
        ui->lineEdit->insert(".");
    }
    if(ui->lineEdit_2->hasFocus())
    {
        ui->lineEdit_2->insert(".");
    }
}

void lock_screen::on_pushButton_11_clicked()//d
{
    if(ui->lineEdit->hasFocus())
    {
        ui->lineEdit->backspace();
    }
    if(ui->lineEdit_2->hasFocus())
    {
        ui->lineEdit_2->backspace();
    }
}

bool lock_screen::eventFilter(QObject *o, QEvent *e)
{
    if(QEvent::KeyPress == e->type()) {
        QKeyEvent *ke = ((QKeyEvent *)e);
        const int kvalue = ke->key();
        if(kvalue == Qt::Key_Escape) {
            e->ignore();
            return true;
        }
    }
    return QDialog::eventFilter(o, e);
}

void lock_screen::on_pushButton_ok_clicked()
{
    QSqlQueryModel model;
    if(ui->lineEdit->text().trimmed().isEmpty() || ui->lineEdit_2->text().trimmed().isEmpty())
    {
        lds_messagebox::warning(this, MESSAGE_TITLE_VOID, QObject::tr("请输入完整的用户名和密码"));

        return;
    }

    QString str = lds::sysconf->value("system_setting/jiesuoyuangong").toString();
    if(str == "1" && type == "1")
    {
        if(ui->lineEdit->text().trimmed() != lds::gs_operid)
        {
            lds_messagebox::warning(this, MESSAGE_TITLE_VOID, QObject::tr("锁屏解锁必须是系统登录员工"));

            return;
        }
    }

    if(ui->lineEdit->text().trimmed() == "0000")
    {
        if(!n_func::f_cmp_syspwd(ui->lineEdit_2->text().trimmed()))//ui->lineEdit_2->text().trimmed() != n_func::f_get_sysparm("system_pass", "0000"))
        {
            lds_messagebox::warning(this, MESSAGE_TITLE_VOID, QObject::tr("密码输入错误"));

            return;
        }
    }
    else
    {
        model.setQuery(QString("select count(0)  from cey_sys_operator  where vch_operID = '%1' and ch_flag ='%2' ").arg(ui->lineEdit->text().trimmed()).arg("Y"));
        if(model.record(0).value(0).toInt() == 0)
        {
            lds_messagebox::warning(this, MESSAGE_TITLE_VOID, QObject::tr("用户名代码不存在"));

            return;
        }

        model.clear();
        model.setQuery(QString("select vch_password  from cey_sys_operator  where vch_operID = '%1' and ch_flag ='%2' ").arg(ui->lineEdit->text().trimmed()).arg("Y"));
        if(model.record(0).value(0).toString().trimmed() != ui->lineEdit_2->text().trimmed())
        {
            lds_messagebox::warning(this, MESSAGE_TITLE_VOID, QObject::tr("密码输入错误"));

            return;
        }
    }

    this->reject();
}
