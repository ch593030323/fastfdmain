#ifndef W_SYS_MANAGE_OUTER_PAY_SET_LOCALE_WIZARD_TXTEDIT_H
#define W_SYS_MANAGE_OUTER_PAY_SET_LOCALE_WIZARD_TXTEDIT_H

#include <QDialog>
class QPlainTextEdit;
namespace Ui {
class w_sys_manage_outer_pay_set_locale_wizard_txtedit;
}

class w_sys_manage_outer_pay_set_locale_wizard_txtedit : public QDialog
{
    Q_OBJECT

public:
    explicit w_sys_manage_outer_pay_set_locale_wizard_txtedit(QWidget *parent = 0);
    ~w_sys_manage_outer_pay_set_locale_wizard_txtedit();

    bool open(const QString &country, QString &errstring);
signals:
    void signal_receipt_print(const QString &country, const QByteArray &byte);
private slots:
    void took();
    void tocancel();
    void toinit();
    void updateOkState();
    void toprint();
private:
    bool saveFile();
    bool modifieddoc_savefile_hasnext();
    QPlainTextEdit *curEdit();
    bool isModified();
private:
    Ui::w_sys_manage_outer_pay_set_locale_wizard_txtedit *ui;
    QString country;
};

#endif // W_SYS_MANAGE_OUTER_PAY_SET_LOCALE_WIZARD_TXTEDIT_H
