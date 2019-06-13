#ifndef W_SYS_MANAGE_operatorright_H
#define W_SYS_MANAGE_operatorright_H

#include <QDialog>
#include <QModelIndex>

namespace Ui {
class w_sys_manage_operatorright;
}

class checkbox_phonestyle;
class ftableview_standmodel_sql_none;
class w_sys_manage_operatorright : public QDialog
{
    Q_OBJECT

public:
    struct OperatorRight{
        QString key;
        QString value;
        QString type;
    };

    static const QList<OperatorRight > &get_code_name_list();
    static QString zh(const QString &key);
    static QString zh_tr(const QString &key);
    static QString code(const QString &value);
    static bool operate_right_check_code(const QString &code, const QString &vch_operID);
    static bool operate_right_check_zh(const QString &zh, const QString &vch_operID);
private:
    static QList<OperatorRight > code_name_list;
    static void code_name_list_append(const OperatorRight &right);
    static int stack_widget_index;
public:
    explicit w_sys_manage_operatorright(QWidget *parent = 0);
    ~w_sys_manage_operatorright();
private slots:
    void toselectall();
    void toclearall();
    void toitemright();
    void took();
    void tocancel();
    void torightcopy();
    void tochangepwd();

    void toupdaterights();
    void toSaveCheckbox();

    void updateSection(const QModelIndex &index);
private:
    void setCheckedAll(bool check);

    void setCheckedSelect(const QString &vch_operID);
    void setCheckedRights(const QString &rights);

    void createItem(QWidget *parent, const OperatorRight &right, int index);
private:
    Ui::w_sys_manage_operatorright *ui;
    QList<checkbox_phonestyle*> checkbox_list;
    ftableview_standmodel_sql_none *backlist_model;
};

#endif // W_SYS_MANAGE_operatorright_H
