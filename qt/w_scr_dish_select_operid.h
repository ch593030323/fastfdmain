#ifndef W_SCR_DISH_SELECT_OPERID_H
#define W_SCR_DISH_SELECT_OPERID_H

#include <QDialog>
#include "ui_w_scr_dish_special_cook.h"
#include "ftableview_standmodel_sql_none.h"

class w_scr_dish_select_operid : public QDialog
{
    Q_OBJECT
public:
    explicit w_scr_dish_select_operid(const QString &curOperId, QWidget *parent);
    ~w_scr_dish_select_operid();
    QString selectOperid();
private slots:
    void took();
    void toexit();
private:
    QStringList hasSelected();
    QStringList typenolist;
    Ui::w_scr_dish_special_cook *ui;
    ftableview_standmodel_sql_none *cook_model;
};

#endif // W_SCR_DISH_SELECT_OPERID_H
