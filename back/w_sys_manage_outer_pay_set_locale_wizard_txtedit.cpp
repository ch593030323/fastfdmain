#include "w_sys_manage_outer_pay_set_locale_wizard_txtedit.h"
#include "ui_w_sys_manage_outer_pay_set_locale_wizard_txtedit.h"
#include <QScrollBar>
#include <QTimer>
#include "lds.h"
#include "lds_messagebox.h"
#include <QDir>
#include <QPlainTextEdit>
#include "w_sys_manage_outer_pay_set_locale_wizard.h"

w_sys_manage_outer_pay_set_locale_wizard_txtedit::w_sys_manage_outer_pay_set_locale_wizard_txtedit(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::w_sys_manage_outer_pay_set_locale_wizard_txtedit)
{
    ui->setupUi(this);
    this->setWindowTitle(QObject::tr("编辑"));

    connect(ui->pushButton_cancel, SIGNAL(clicked()), this, SLOT(tocancel()));
    connect(ui->pushButton_save, SIGNAL(clicked()), this, SLOT(took()));
    connect(ui->pushButton_init, SIGNAL(clicked()), this, SLOT(toinit()));
    connect(ui->pushButton_print, SIGNAL(clicked()), this, SLOT(toprint()));
    connect(ui->comboBox_receipt_width, SIGNAL(currentIndexChanged(int)), ui->stackedWidget, SLOT(setCurrentIndex(int)));
}

w_sys_manage_outer_pay_set_locale_wizard_txtedit::~w_sys_manage_outer_pay_set_locale_wizard_txtedit()
{
    delete ui;
}

bool w_sys_manage_outer_pay_set_locale_wizard_txtedit::open(const QString &country, QString &errstring)
{
    this->country =country;
    foreach(const QFileInfo &info, QDir(lds::localdataLocalePath + "/" + country).entryInfoList(QStringList() << "*.txt")) {
        if(info.baseName() == "print_receipt_pay_test_data")
            continue;
        ui->comboBox_receipt_width->addItem(info.fileName(), info.filePath());
        QPlainTextEdit *editor = new QPlainTextEdit(ui->stackedWidget);
        editor->setFont(ldsObjFont<QDialog>(this, lds::FONT_SONGTI_Family));
        editor->setPlainText(lds::fileOpenReadAll(info.filePath()));
        ui->stackedWidget->addWidget(editor);
//        connect(editor->document(), SIGNAL(contentsChanged()), this, SLOT(updateOkState()));
    }
    if(ui->comboBox_receipt_width->count() == 0) {
        errstring = "NO_FILE_BE_FOUND";
        return false;
    }
    ui->comboBox_receipt_width->setcomusrdata(TXT_PRINT_RECEIPT_PAY_FILE_PATH(country));
    return true;
}

void w_sys_manage_outer_pay_set_locale_wizard_txtedit::took()
{
    if(1 == lds_messagebox::question(this, MESSAGE_TITLE_VOID, QObject::tr("确定保存"), QObject::tr("确定"), QObject::tr("取消"))) {
        return;
    }
    if(false == saveFile()) {
        return;
    }
    lds_messagebox::information(this, MESSAGE_TITLE_VOID, QObject::tr("保存成功"));
    this->accept();
}

void w_sys_manage_outer_pay_set_locale_wizard_txtedit::tocancel()
{
    if(modifieddoc_savefile_hasnext()) {
        this->reject();
    }
}

void w_sys_manage_outer_pay_set_locale_wizard_txtedit::toinit()
{
    if(1 == lds_messagebox::question(this, MESSAGE_TITLE_VOID, QObject::tr("初始化") + ":" + ui->comboBox_receipt_width->currentText(), QObject::tr("确定"), QObject::tr("取消"))) {
        return;
    }
    bool ok;
    QString errstring;
    QByteArray byte = lds::fileOpenReadAll(QString("userdata/settings/locale/%1/%2").arg(country).arg(QFileInfo(ui->comboBox_receipt_width->curusrdata().toString()).fileName()), &ok, &errstring);
    if(false == ok) {
        lds_messagebox::warning(this, MESSAGE_TITLE_VOID, errstring);
        return;
    }
    curEdit()->selectAll();
    curEdit()->insertPlainText(byte);
}

void w_sys_manage_outer_pay_set_locale_wizard_txtedit::updateOkState()
{
    ui->pushButton_save->setEnabled(isModified());
}

void w_sys_manage_outer_pay_set_locale_wizard_txtedit::toprint()
{
    emit signal_receipt_print(country, curEdit()->toPlainText().toLocal8Bit());
}

bool w_sys_manage_outer_pay_set_locale_wizard_txtedit::saveFile()
{
    QString errstring;
    for(int k = 0; k < ui->comboBox_receipt_width->count(); k ++) {
        QString filepath = ui->comboBox_receipt_width->comusrdata(k).toString();
        QPlainTextEdit *editor = qobject_cast<QPlainTextEdit *>(ui->stackedWidget->widget(k));
        if(editor->document()->isModified()) {
            if(false == lds::fileWriteByte(filepath, editor->toPlainText().toLocal8Bit(), &errstring)) {
                lds_messagebox::warning(this, MESSAGE_TITLE_VOID, errstring);
                return false;
            }
            lds_messagebox::information(this, MESSAGE_TITLE_VOID, ui->comboBox_receipt_width->itemText(k) + "," + QObject::tr("保存成功"));
            //setPlainText:This property gets and sets the plain text editor's contents. The previous contents are removed and undo/redo history is reset when this property is set.
            editor->setPlainText(editor->toPlainText());
        }
    }
    TXT_PRINT_RECEIPT_PAY_FILE_PATH_SETVALUE(country, ui->comboBox_receipt_width->curusrdata());
    return true;
}

bool w_sys_manage_outer_pay_set_locale_wizard_txtedit::modifieddoc_savefile_hasnext()
{
    if(isModified()) {
        switch(lds_messagebox::question(this, MESSAGE_TITLE_VOID, QObject::tr("有数据未保存,确认保存?"), QObject::tr("保存"),QObject::tr("不保存"),QObject::tr("取消"))) {
        case 0://保存
            if(false == saveFile())
                return false;
            return true;
        case 1://不保存
            return true;
        case 2://取消
            return false;
        }
    }
    return true;
}

QPlainTextEdit *w_sys_manage_outer_pay_set_locale_wizard_txtedit::curEdit()
{
    return qobject_cast<QPlainTextEdit *>(ui->stackedWidget->currentWidget());
}

bool w_sys_manage_outer_pay_set_locale_wizard_txtedit::isModified()
{
    bool isModified = false;
    for(int k = 0; k < ui->comboBox_receipt_width->count(); k ++) {
        QPlainTextEdit *editor = qobject_cast<QPlainTextEdit *>(ui->stackedWidget->widget(k));
        if(editor->document()->isModified()) {
            isModified = true;
            break;
        }
    }
    return isModified;
}
