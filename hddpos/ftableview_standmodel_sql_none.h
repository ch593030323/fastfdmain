#ifndef FTABLEVIEW_STANDMODEL_SQL_NONE_H
#define FTABLEVIEW_STANDMODEL_SQL_NONE_H

#include  "ftableview_standmodel_virtual.h"
#include <QSet>
#include <QMap>
#include "lds_bool.h"


class ftableview_standmodel_sql_none : public ftableview_standmodel_virtual
{
    Q_OBJECT
public:

    class noneData{
    public:
        QMap<int, QVariant> map;
    public:
        //u1 是key
        inline noneData(const QVariant &u, const QVariant &u1) {
            map.insert(Qt::UserRole, u);
            map.insert(Qt::UserRole + 1, u1);
        }
        inline noneData(const QVariant &u, const QVariant &u1, const QVariant &u2) {
            map.insert(Qt::UserRole, u);
            map.insert(Qt::UserRole + 1, u1);
            map.insert(Qt::UserRole + 2, u2);
        }
        noneData &append(int role, const QVariant &value);

        inline QVariant value(int role) const {return map.value(role);}
        inline void setValue(int role, const QVariant &u) {  map.insert(role, u); }
        QVariantList values() const;

    };
    static QVariant list_data(const QList<noneData> &list, const QString &usr_1, int role);
    static void list_remove(QList<noneData> &list, const QString &usr_1);
    static int list_index(const QList<noneData> &list, const QString &usr_1);
public:
    explicit ftableview_standmodel_sql_none();
    //!
    void setPrepareList(const QList<noneData> &list);
    void setPrepareList(const QList<noneData> &list, const QStringList &select_usr_1_list);
    void setModelList(const QList<noneData> &list);
    void select(const QString &usr_1);

    void set_list_data(const QVariant &u1, const QVariant &value, int role);

    const QList<noneData> & data_list();

    void initDelay(int mesc = 100);

    QVariantList get_value_unselect(int role);//u u1 u2 返回u2的拼接

    void enableEndAppendNew(bool enabled);

    QMap<int, QVariant>  getMapByKey(const QString &usr_1);

public slots:
    void toinit();

private:
    lds_bool_default_false b_endAppendNew;//用于最后新增按钮
    QList<noneData> _data_list;
    void generateNext();
    bool atEnd();
    //!~
public:
    virtual void togenerate(int index, int updownum, bool hasnext);
    virtual void togenerateNext();
    virtual void togeneratePre();
};

#endif // FTABLEVIEW_STANDMODEL_SQL_NONE_H
