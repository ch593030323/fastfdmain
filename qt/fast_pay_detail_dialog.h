#ifndef FAST_PAY_DETAIL_DIALOG_H
#define FAST_PAY_DETAIL_DIALOG_H

#include <QDialog>
#include "ui_fast_pay_detail_dialog.h"

namespace Ui {
class fast_pay_detail_Dialog;
}

class fast_pay_detail_Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit fast_pay_detail_Dialog(QWidget *parent);
    ~fast_pay_detail_Dialog();
    void setVisible(const QString &ch_tableno);
    Ui::fast_pay_detail_Dialog *ui;
};

#endif // FAST_PAY_DETAIL_DIALOG_H
