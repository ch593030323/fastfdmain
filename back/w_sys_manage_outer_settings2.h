#ifndef W_SYS_MANAGE_OUTER_SETTINGS2_H
#define W_SYS_MANAGE_OUTER_SETTINGS2_H

#include <QDialog>

namespace Ui {
class w_sys_manage_outer_settings2;
}

class w_sys_manage_outer_settings2 : public QDialog
{
    Q_OBJECT

public:
    explicit w_sys_manage_outer_settings2(QWidget *parent = 0);
    ~w_sys_manage_outer_settings2();

private slots:
    void took();
    void tocancel();

    void toPageDown();
    void toPageUp();
private:
    Ui::w_sys_manage_outer_settings2 *ui;
};

#endif // W_SYS_MANAGE_OUTER_SETTINGS2_H
