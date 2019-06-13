#include "lds_query_hddpos.h"
#include "lds_thread.h"
#include "lds.h"
#include <QtDebug>
#include <QApplication>
#include <QSqlError>
#include <QMap>
#include <QSqlRecord>
#include <QSqlField>

QVariant lds_query_hddpos::selectValue(lds_query_hddpos &query, const QString &sql, int i, const QVariant &defValue)
{
    query.execSelect(sql);
    if(query.next())
        return query.recordValue(i);

    return defValue;
}

QVariant lds_query_hddpos::selectValue(const QString &sql, int i, const QVariant &defValue)
{
    lds_query_hddpos q;
    return selectValue(q, sql, i, defValue);
}

QVariant lds_query_hddpos::selectValue(const QString &sql, const QString &arg0, int i, const QVariant &defValue)
{
    lds_query_hddpos q;
    return selectValue(q, sql.arg(arg0), i, defValue);
}

QDateTime lds_query_hddpos::selectNow()
{
    lds_query_hddpos q;
    return selectValue(q, "select now() ", 0).toDateTime();
}

bool lds_query_hddpos::selectHasNext(const QString &sql)
{
    lds_query_hddpos q;
    q.execSelect(sql);
    return q.next();
}

bool lds_query_hddpos::updateTable(const QString &tablename, const qrtVariantPairList &key_value_list, const QString &where_sql, QString *errstring)
{
    lds_query_hddpos q;
    return q.execUpdate(tablename, key_value_list, where_sql, errstring);
}

bool lds_query_hddpos::insertTable(const QString &tablename, const qrtVariantPairList &key_value_list, QString *errstring)
{
    lds_query_hddpos q;
    return q.execInsert(tablename, key_value_list, errstring);
}

bool lds_query_hddpos::deleteTable(const QString &tablename, const QString &where_sql, QString *errstring)
{
    lds_query_hddpos q;
    return q.execDelete(tablename, where_sql, errstring);
}

lds_query_hddpos::lds_query_hddpos()
    :lds_query_db(lds_thread::getThreadDbAndOpen())
{
}

void lds_query_hddpos::execUpdateTrigger(const QString &table, qrtVariantPairList &key_value_list, const QString &where_sql)
{
    bool flag = true;
    //ch_yun_already_keep 若是主动更新该字段，则不进行update
    if(db.record(table).field("ch_yun_already_keep").isValid()) {
        flag = true;
        for(int k = 0; k < key_value_list.count(); k++) {
            if(key_value_list[k].first == "ch_yun_already_keep")  {
                flag = false;
                break;
            }
        }
        if(flag)
            key_value_list << qrtVariantPair("ch_yun_already_keep", "N");
    }
}

void lds_query_hddpos::execInsertTrigger(const QString &table, qrtVariantPairList &key_value_list)
{
    bool flag = true;

    //ch_yun_already_keep 插入包含时，一律都是N
    if(db.record(table).field("ch_yun_already_keep").isValid()) {
        flag = true;
        for(int k = 0; k < key_value_list.count(); k++) {
            if(key_value_list[k].first == "ch_yun_already_keep")  {
                flag = false;
                break;
            }
        }
        if(flag)
            key_value_list << qrtVariantPair("ch_yun_already_keep", "N");
    }
    //terminal_code 插入时，一律都是本机
    if(db.record(table).field("terminal_code").isValid()) {
        flag = true;
        int k = 0;
        for(; k < key_value_list.count(); k++) {
            if(key_value_list[k].first == "terminal_code")  {
                flag = false;
                break;
            }
        }
        if(flag)
            key_value_list << qrtVariantPair("terminal_code", lds::terminalCode);
        else
           key_value_list[k].second = lds::terminalCode;
    }
}

bool lds_query_hddpos::tran_saction()
{
#ifdef QT_DEBUG
    qDebug() << __FUNCTION__ << get_db_state();
    if(get_db_state() != 0) {
        qDebug("transaction,  state != 0 ");
        qDebug() << get_db_state();
        QObject *o = 0;
        o->children();
    }
#endif
    get_db_state()++;
    return lds_thread::getThreadDbAndOpen().transaction();
}

bool lds_query_hddpos::com_mit()
{
#ifdef QT_DEBUG
    qDebug() << __FUNCTION__ << get_db_state();
    if(get_db_state() != 1) {
        qDebug("commit,  state != 1 ");
        qDebug() << get_db_state();
        QObject *o = 0;
        o->children();
    }
#endif
    get_db_state()--;
    return lds_thread::getThreadDbAndOpen().commit();
}

bool lds_query_hddpos::roll_back()
{
#ifdef QT_DEBUG
    qDebug() << __FUNCTION__ << get_db_state();
    if(get_db_state() != 1) {
        qDebug("rollback,  state != 1 ");
        qDebug() << get_db_state();
        QObject *o = 0;
        o->children();
    }
#endif
    get_db_state()--;
    return lds_thread::getThreadDbAndOpen().rollback();
}

void lds_query_hddpos::recordFill(qrtVariantPairList &d)
{
    for(int k = 0, count = this->recordCount(); k < count; k++) {
        d.append(qrtVariantPair(this->record().fieldName(k), this->recordValue(k)));
    }
}

void lds_query_hddpos::replace(qrtVariantPairList &d, const QString &field_name, const QVariant &value)
{
    for(int k = 0; k < d.count(); k++) {
        if(d[k].first == field_name) {
            d[k].second = value;
            break;
        }
    }
}

void lds_query_hddpos::remove(qrtVariantPairList &d, const QString &field_name)
{
    for(int k = 0; k < d.count(); k++) {
        if(d[k].first == field_name) {
            d.removeAt(k);
            break;
        }
    }
}
