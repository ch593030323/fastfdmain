#ifndef W_RPT_REPORT_FRAME_TABLEBUSINESS_H
#define W_RPT_REPORT_FRAME_TABLEBUSINESS_H

#include <QWidget>
#include "w_rpt_report_frame.h"

namespace Ui {
class w_rpt_report_frame_tablebusiness;
}

class w_rpt_report_frame_tablebusiness : public w_rpt_report_frame
{
    Q_OBJECT

public:
    explicit w_rpt_report_frame_tablebusiness(QWidget *parent = 0);
    ~w_rpt_report_frame_tablebusiness();
private slots:
    void toselect();

private:
    Ui::w_rpt_report_frame_tablebusiness *ui_help;
};

#endif // W_RPT_REPORT_FRAME_TABLEBUSINESS_H
