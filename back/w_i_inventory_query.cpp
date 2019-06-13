#include "w_i_inventory_query.h"
#include "ui_w_i_inventory_query.h"
#include "lds.h"
#include <QTimer>
#include "n_func.h"

w_i_inventory_query::w_i_inventory_query(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::w_i_inventory_query)
{
    ui->setupUi(this);
    resize(lds::MAIN_WINDOW_SIZE);

    querymodel = new report_querymodel(this);
    ui->comboBox_report_type->addItem(QObject::tr("库存查询"), "inventory_look");
    ui->comboBox_report_type->addItem(QObject::tr("库存补仓提醒报表"), "inventory_warn");
    ui->comboBox_dish_type->addItem(QObject::tr("全部"), "");
    ui->comboBox_dish_type->addItemsBySql(" select ch_dish_typeno, vch_dish_typename from cey_bt_dish_type ");
    ui->tableView->setModel(querymodel);
    ui->tableView->setTransferHheader();
    //
    connect(ui->pushButton_exit, SIGNAL(clicked()), this, SLOT(toexit()));
    connect(ui->pushButton_export, SIGNAL(clicked()), this, SLOT(toexport()));
    connect(ui->pushButton_print, SIGNAL(clicked()), this, SLOT(toprint()));
    connect(ui->lineEdit, SIGNAL(textChanged(QString)), this, SLOT(toselect()));
    connect(ui->comboBox_dish_type, SIGNAL(currentIndexChanged(int)), this, SLOT(toselect()));
    connect(ui->comboBox_report_type, SIGNAL(currentIndexChanged(int)), this, SLOT(toselect()));

    QTimer::singleShot(10, this, SLOT(toselect()));
}

w_i_inventory_query::~w_i_inventory_query()
{
    delete ui;
}

void w_i_inventory_query::toselect()
{
    bool inventory_warn = ui->comboBox_report_type->curusrdata().toString() == "inventory_warn";
    //数据库sql
    QString from_sql
            =  " FROM cey_bt_dish a"

            " LEFT OUTER JOIN cey_st_material_num b"

            " ON a.ch_dishno = b.ch_materialno"

            " where IFNULL(a.item_flag, 'N') = 'Y'"

            " and a.ch_sub_typeNo like '%1%' "

            " and (ch_dishno like '%2%' or vch_spell like '%2%' or vch_dishname like '%2%' or vch_barcode like '%2%' ) "

            +QString(inventory_warn ? " and  b.num_num <= a.num_item_warn": "")+

            " ORDER BY num_store desc";
    ;
    from_sql = from_sql
            .arg(ui->comboBox_dish_type->curusrdata().toString())
            .arg(ui->lineEdit->text().toUpper())//
            ;

    querymodel->setQuery(
                "select ch_dishno, "

                " vch_dishname, "

                " (select vch_unitname from cey_bt_unit where cey_bt_unit.ch_unitno = t.ch_unitno) as vch_unitname,"

                " num_store, "

                " num_cost as num_store_price, "

                " num_store * num_cost as num_store_total,"

                " (select vch_dish_typename from cey_bt_dish_type where cey_bt_dish_type.ch_dish_typeno = t.ch_dish_typeno) as vch_dish_typename,"

                " (select vch_sub_typename from cey_bt_dish_subtype where cey_bt_dish_subtype.ch_sub_typeno = t.ch_sub_typeno) as vch_sub_typename"

                +QString(inventory_warn ? ", num_item_warn": "")+

                " from ("

                "SELECT a.ch_dishno, a.vch_dishname, a.int_unit_rate, a.ch_unitno,"

                " a.vch_pur_unitno,"

                " a.ch_dish_typeno,"

                " a.ch_sub_typeno,"

                " IFNULL(b.num_num, 0) as num_store,"

                " IFNULL(b.cost_price, 0) as num_cost,"

                " a.num_item_warn "

                + from_sql
                + ") t"
                );
    //
    lds_query_hddpos  query;
    query.execSelect("select sum(b.num_num) as num_store, sum(b.num_num * b.cost_price) as num_store_total "
                     + from_sql);
    query.next();
    ui->tableView->updateSum(querymodel->fieldIndex("num_store"), query.recordValue("num_store"));
    ui->tableView->updateSum(querymodel->fieldIndex("num_store_total"), query.recordValue("num_store_total"));
    //
    ui->tableView->resizeColumnsToContents();
}

void w_i_inventory_query::toexit()
{
    this->reject();
}

void w_i_inventory_query::toexport()
{
    QString title = ui->comboBox_report_type->currentText();

    w_rpt_report_dataExport dialog(ui->tableView->tableView, ui->tableView->lineView, this);
    dialog.setWindowTitle(title);
    dialog.defFilepath(title);
    transparentCenterDialog(&dialog).exec();
}

void w_i_inventory_query::toprint()
{
    QPushButton *b = qobject_cast<QPushButton *>(this->sender());
    w_rpt_report_printFormat *dialog
            = new w_rpt_report_printFormat(w_rpt_report_printFormat::show_model_quick, this->windowTitle(), lds::gs_operid, n_func::f_get_sysdatetime_str(),
                                           this->metaObject()->className(),
                                           "", "",
                                           ui->tableView,
                                           0
                                           );
    dialog->pop(b, this);
}
