#ifndef W_I_INVENTORY_QUERY_H
#define W_I_INVENTORY_QUERY_H

#include <QDialog>
#include "w_rpt_report.h"
#include "w_rpt_report_frame.h"

namespace Ui {
class w_i_inventory_query;
}

class w_i_inventory_query : public QDialog
{
    Q_OBJECT

public:
    explicit w_i_inventory_query(QWidget *parent = 0);
    ~w_i_inventory_query();

private slots:
    void toselect();
    void toexit();

    void toexport();
    void toprint();
private:
    Ui::w_i_inventory_query *ui;
    report_querymodel *querymodel;
};

#endif // W_I_INVENTORY_QUERY_H
