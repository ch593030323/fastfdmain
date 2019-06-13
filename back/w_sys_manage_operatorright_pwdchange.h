#ifndef W_SYS_MANAGE_OPERATORRIGHT_PWDCHANGE_H
#define W_SYS_MANAGE_OPERATORRIGHT_PWDCHANGE_H

#include "w_rpt_report_frame.h"
class Ui_w_m_member_pwdchange_Dialog;
class w_sys_manage_operatorright_pwdchange : public QDialog{
    Q_OBJECT
public:
    explicit w_sys_manage_operatorright_pwdchange(QWidget *parent = 0);

    Ui_w_m_member_pwdchange_Dialog *ui;
private slots:
    void took();
    void toexit();
private:
};

#endif // W_SYS_MANAGE_OPERATORRIGHT_PWDCHANGE_H
