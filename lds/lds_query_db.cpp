#include "lds_query_db.h"
#include <QSqlRecord>
#include <QVariant>
#include <QtDebug>
#include <QFile>
#include <QSqlError>
#include <QElapsedTimer>
#include <QThread>
#include "lds_query_thread.h"
#include <QSqlResult>
#include <QSqlField>
#include <QSqlIndex>
#include <QSqlDatabase>
#include <QStringList>
/*
execUpdate
execInsert
execDelete
-----"[\s\S]{0,}\,[\s\S]{0,}"

匹配execUpdate("dd," 以【execUpdate(】开头， 后面为【"*"】的字符串，【*】中包含，号
execUpdate\("[^"\\S\\s]{0,},[^"\\S\\s]{0,}"

execUpdate("a,"    execUpdate("a "
execInsert("a,"    execInsert("a "
execDelete("a,"    execDelete("a "
*/


static bool emptyDataIsDateTime(const QString &table, const qrtVariantPair &pair);

QMap<QString, int> lds_query_db::dbname_state_map;

QString lds_query_db::insertSql(const QString &table,
                                const qrtVariantPairList &key_value_list) {

    QString prepare_str;
    QString keys_str;
    foreach(const qrtVariantPair &key_value, key_value_list) {
        keys_str += key_value.first + ",";
    }
    keys_str.chop(1);
    prepare_str = "insert into " + table + " (" + keys_str +")values(";
    foreach(const qrtVariantPair &key_value, key_value_list) {
        if(key_value.second.getType() == qrtVariant::FUNC) {
            prepare_str += key_value.second.toString() + ",";
        }
        if(key_value.second.getType() == qrtVariant::VAL) {
            ///sql
            if(emptyDataIsDateTime(table, key_value)) {
                prepare_str += "null,";
            } else {
                prepare_str += "'" + key_value.second.toString() + "',";
            }
        }
    }
    prepare_str.chop(1);
    prepare_str +=  ")";

    return prepare_str;
}

QString lds_query_db::updateSql(const QString &table,
                                const qrtVariantPairList &key_value_list,
                                const QString &where_sql) {

    QString prepare_str = "update " + table +" set ";
    foreach(const qrtVariantPair &key_value, key_value_list) {
        if(key_value.second.getType() == qrtVariant::FUNC) {
            prepare_str += key_value.first + " = "+key_value.second.toString() + ",";
        }
        if(key_value.second.getType() == qrtVariant::VAL) {
            ///sql
            if(emptyDataIsDateTime(table, key_value)) {
                prepare_str += key_value.first + "=null,";
            } else {
                prepare_str += key_value.first  + "= '" + key_value.second.toString() + "',";
            }
        }
    }
    prepare_str.chop(1);
    if(where_sql.length() > 0) {
        prepare_str += " where " + where_sql;
    }

    return prepare_str;
}

int &lds_query_db::get_db_state()
{
    QString dbname = lds_query_thread::getThreadDbname();
    if(dbname_state_map.find(dbname) == dbname_state_map.end()) {
        dbname_state_map.insert(dbname, 0);
    }
    return dbname_state_map[dbname];
}

qrtVariant lds_query_db::pairListValue(const qrtVariantPairList &pairlist, const QString &key)
{
    for(int k = 0; k < pairlist.count(); k++) {
        if(pairlist[k].first == key)
            return pairlist[k].second;
    }
    return "";
}

lds_query_db::lds_query_db(QSqlDatabase db)
    :QSqlQuery(db)
{
    this->db = db;
}

QDateTime lds_query_db::curDateTime()
{
    QDateTime dt;
    //MYSQL
    if(!dt.isValid()) {
        this->exec("select now() ");
        if(this->next()) {
            dt = this->recordValue(0).toDateTime();
        }
    }
    //SQLITE
    if(!dt.isValid()) {
        this->exec(" select datetime(); ");
        if(this->next()) {
            dt = this->recordValue(0).toDateTime();
        }
    }
    return dt;
}

QString lds_query_db::curdatestr()
{
    QDateTime d = curDateTime();
    return "'" + d.toString("yyyy-MM-dd")+"'";
}

bool lds_query_db::exec(const QString &query, QString *errstring)
{
    bool ret = QSqlQuery::exec(query);
    if(!ret) {
        if(errstring) *errstring = recordError();
    }
#ifdef QT_DEBUG
        if(!ret)
            qDebug()  << "cur thread id:" << QThread::currentThreadId() << "main thread id:" << lds_query_thread::MAIN_THREAD_ID<<  query << this->lastError();
        else
            qDebug() << query;
#endif

    return ret;
}

bool lds_query_db::exec(QString *errstring)
{
    bool ret = QSqlQuery::exec();
    if(!ret) {
        if(errstring) *errstring = recordError();
    }
#ifdef QT_DEBUG
    if(!ret)  qDebug()  << this->lastQuery() << this->lastError();
#endif
    return ret;
}

bool lds_query_db::prepare(const QString &query, QString *errstring)
{
    bool ret = QSqlQuery::prepare(query);
    if(!ret) {
        if(errstring) *errstring = recordError();
    }
#ifdef QT_DEBUG
    if(!ret)   qDebug() <<  query << recordError();
#endif
    return ret;
}

QSqlError lds_query_db::lastError() const
{
    return QSqlQuery::lastError();
}

bool lds_query_db::execUpdate(const QString &table, const qrtVariantPairList &list, const QString &where_sql, QString *errstring)
{
    qrtVariantPairList key_value_list = list;
    //
    execUpdateTrigger(table, key_value_list, where_sql);
    //prepare_str
    QString prepare_str = "update " + table +" set ";
    foreach(const qrtVariantPair &key_value, key_value_list) {
        if(key_value.second.getType() == qrtVariant::FUNC) {
            prepare_str += key_value.first + " = "+key_value.second.toString() + ",";
        }
        if(key_value.second.getType() == qrtVariant::VAL) {
            prepare_str += key_value.first + "= ?,";
        }
    }
    //check dot where
    prepare_str.chop(1);
    if(where_sql.length() > 0) {
        prepare_str += " where " + where_sql;
    }
    //addBindValue
    if(!this->prepare(prepare_str,errstring)) {
        return false;
    }
    foreach(const qrtVariantPair &key_value, key_value_list) {
        if(key_value.second.getType() == qrtVariant::VAL) {
            this->addBindValue(key_value.second);
            qDebug() << __LINE__<< key_value.second;
        }
    }

#ifdef QT_DEBUG
    qDebug() << updateSql(table, key_value_list, where_sql);
#endif
    //exec
    return this->exec(errstring);
}

bool lds_query_db::execUpdate(const QString &table, const QString &key1, const qrtVariant &value1, const QString &where_sql, QString *errstring)
{
    return execUpdate(table, qrtVariantPairList()
                      << qrtVariantPair(key1, value1),
                      where_sql,
                      errstring);
}

bool lds_query_db::execUpdate(const QString &table, const QString &key1, const qrtVariant &value1, const QString &key2, const qrtVariant &value2, const QString &where_sql, QString *errstring)
{
    return execUpdate(table, qrtVariantPairList()
                      << qrtVariantPair(key1, value1)
                      << qrtVariantPair(key2, value2),
                      where_sql,
                      errstring);
}

/*
 * QStringList:unique_key_list:a,b
 * selectStr_selectKeyInTable:select a, b, c, d;
 * rect[
 *      a  b
 *      a1  b1 c1 d1   ----------select c1, d1 from a = a1 and b = b1
 *      a2 b2 c2 d2
 *      a3 b3 c3 d3
 *
 * ]
 */
bool lds_query_db::execUpdate(const QString &table, const QStringList &unique_key_list, const QString &selectStr_selectKeyInTable, QString *errstring)
{
    //key_value_rect
    if(!this->execSelect(selectStr_selectKeyInTable, errstring)) {
        return false;
    }
    QList<qrtVariantPairList> key_value_rect;
    QStringList key_list = unique_key_list;
    int qcount = this->recordCount();
    while(this->next()) {
        int end = key_value_rect.count();
        key_value_rect << qrtVariantPairList();
        for(int k = 0; k < qcount; k ++) {
            key_value_rect[end] << qrtVariantPair(this->record().fieldName(k), this->recordValue(k));
            if(key_list.contains(key_value_rect[end][k].first)) {//只会删除一次
                key_value_rect[end][k].second.setType(qrtVariant::NONE);
                key_list.removeOne(key_value_rect[end][k].first);
            }
        }
    }

    //exec
    foreach(const qrtVariantPairList &key_value_list, key_value_rect) {
        if(!execUpdate(table, key_value_list, getWhereSql(unique_key_list, key_value_list), errstring)) {
            return false;
        }
    }
    return true;
}

bool lds_query_db::execInsertDuplicateUpdate(const QString &table, const QStringList &unique_key_list, const qrtVariantPairList &key_value_list, QString *errstring)
{
    QString where_sql = getWhereSql(unique_key_list, key_value_list);
    if(!this->execSelect(" select 1 from " + table + " where " + where_sql, errstring)) {
        return false;
    }
    if(this->next()) {
        foreach(const qrtVariantPair& pair, key_value_list) {
            if(unique_key_list.contains(pair.first)) {
                pair.second.setType(qrtVariant::NONE);
            }
        }
        return execUpdate(table, key_value_list, where_sql, errstring);
    }
    return execInsert(table, key_value_list, errstring);
}

bool lds_query_db::execInsertDuplicateUpdate(const QString &table, const QString &unique_key1, const qrtVariantPairList &key_value_list, QString *errstring)
{
    return execInsertDuplicateUpdate(table, QStringList() << unique_key1, key_value_list, errstring);
}

bool lds_query_db::execInsertDuplicateUpdate(const QString &table, const QString &unique_key1, const QString &unique_key2, const qrtVariantPairList &key_value_list, QString *errstring)
{
    return execInsertDuplicateUpdate(table, QStringList()
                                     << unique_key1
                                     << unique_key2,
                                     key_value_list,
                                     errstring);
}

bool lds_query_db::execInsert(const QString &table, const qrtVariantPairList &list, QString *errstring)
{
    //prepare_str
    //into
    QString prepare_str;
    QString keys_str;
    QString values_str;
    qrtVariantPairList key_value_list = list;

    execInsertTrigger(table, key_value_list);
    //keys_str values_str
    foreach(const qrtVariantPair &key_value, key_value_list) {
        if(key_value.second.getType() == qrtVariant::FUNC) {
            keys_str += key_value.first + ",";
            values_str += key_value.second.toString() + ",";
        }
        if(key_value.second.getType() == qrtVariant::VAL) {
            keys_str += key_value.first + ",";
            values_str += "?,";
        }
    }
    keys_str.chop(1);
    values_str.chop(1);
    //
    prepare_str = "insert into " + table + " (" + keys_str +")values(" + values_str + ")";
    //addBindValue
    if(!this->prepare(prepare_str, errstring)) {
        return false;
    }
    foreach(const qrtVariantPair &key_value, key_value_list) {
        if(key_value.second.getType() == qrtVariant::VAL) {
            if(emptyDataIsDateTime(table, key_value)) {
                this->addBindValue(QVariant());
            } else {
                this->addBindValue(key_value.second);
            }
        }
    }
#ifdef QT_DEBUG
    qDebug() << insertSql(table, key_value_list);
#endif
    return this->exec(errstring);
}

bool lds_query_db::execInsert(const QString &table, const QString &keys_str, const QString &values_str, const QString &split, QString *errstring)
{

    qrtVariantPairList vplist;
    QStringList keylist = keys_str.split(split);
    QStringList valuelist = values_str.split(split);
    //
    for(int k = 0; k < keylist.count(); k++) {
        vplist << qrtVariantPair(keylist[k], valuelist[k]);
    }

    return execInsert(table, vplist, errstring);
}

bool lds_query_db::execInsert(const QString &table, const QString &values_str, const QString &split, QString *errstring)
{

    qrtVariantPairList key_value_list;
    QStringList valuelist = values_str.split(split);
    QSqlRecord db_record = db.record(table);
    int sqlcount = valuelist.count();
    //
    for(int k = 0; k < sqlcount; k++) {
        key_value_list << qrtVariantPair(db_record.fieldName(k), valuelist[k]);
    }
    return this->execInsert(table, key_value_list, errstring);
}

bool lds_query_db::execInsert(const QString &table, const QString &selectStr_selectKeyInTable, QString *errstring)
{

    //key_value_rect
    if(!this->execSelect(selectStr_selectKeyInTable, errstring)) {
        return false;
    }
    QList<qrtVariantPairList> key_value_rect;
    int qcount = this->recordCount();
    while(this->next()) {
        int end = key_value_rect.count();
        key_value_rect << qrtVariantPairList();
        for(int k = 0; k < qcount; k ++) {
            key_value_rect[end] << qrtVariantPair(this->record().fieldName(k), this->recordValue(k));
        }
    }
    //exec
    foreach(const qrtVariantPairList &key_value_list, key_value_rect) {
        if(!execInsert(table, key_value_list, errstring)) {
            return false;
        }
    }
    return true;
}

bool lds_query_db::emptyDataIsDateTime(const QString &table, const qrtVariantPair &pair)
{
    if(pair.second.toString().isEmpty()) {
        QVariant::Type datatype = this->db.record(table).field(pair.first).type();
        if(QVariant::Date == datatype
                || QVariant::Time == datatype
                || QVariant::DateTime == datatype) {
            return true;
        }
    }
    return false;
}

QString lds_query_db::getWhereSql(const QStringList &unique_key_list, const qrtVariantPairList &key_value_list)
{
    QStringList key_list = unique_key_list;
    QString where_sql;
    foreach(const qrtVariantPair &key_value, key_value_list) {
        if(key_list.contains(key_value.first)) {//values包含相同项，只会执行一次
            where_sql += key_value.first + "= '" + key_value.second.toString() + "' and ";
            key_list.removeOne(key_value.first);
        }
        if(0 == unique_key_list.count()) break;
    }
    where_sql.chop(5);//" and ";

    return where_sql;
}

bool lds_query_db::execDelete(const QString &table, QString *errstring)
{
    return execDelete(table, "", errstring);
}

bool lds_query_db::execDelete(const QStringList &table_list, QString *errstring)
{
    foreach(const QString &table, table_list) {
        if(!execDelete(table, errstring)) {
            return false;
        }
    }
    return true;
}

bool lds_query_db::execDelete(const QString &table, const QString &where_sql, QString *errstring)
{
    if(where_sql.length() > 0)
        return this->exec("delete from " + table + " where "+where_sql, errstring);
    else
        return this->exec(" delete from "+table, errstring);
}

bool lds_query_db::execDeleteSelectKey(const QStringList &tableList, const QString &selectStr_tableListKeyForColumnIndex, QString *errstring)
{
    int table_count = tableList.count();
    QList<QStringPairList> table_key_value_rect;
    for(int k = 0; k < table_count; k++) {
        table_key_value_rect << QStringPairList();
    }

    if(!this->execSelect(selectStr_tableListKeyForColumnIndex, errstring)) {
        return false;
    }
    while(this->next()) {
        for(int k = 0; k < table_count; k++) {
            table_key_value_rect[k]  << QStringPair(this->record().fieldName(k), this->recordValue(k).toString());
        }
    }
    for(int k = 0; k < table_count; k++) {
        foreach(const QStringPair &pair, table_key_value_rect[k]) {
            if(!this->execDelete(tableList[k], qrtEqual(pair.first, pair.second), errstring)) {
                return false;
            }
        }
    }
    return true;
}

bool lds_query_db::execDeleteSelectKey(const QString &table, const QString &selectStr_tableKeyForColumn0, QString *errstring)
{
    return execDeleteSelectKey(QStringList() << table, selectStr_tableKeyForColumn0, errstring);
}

bool lds_query_db::execDeleteSelectKey(const QString &table, const QString &table2, const QString &selectStr_tableKeyForColumn0_table2KeyForColumn1, QString *errstring)
{
    return execDeleteSelectKey(QStringList() << table << table2, selectStr_tableKeyForColumn0_table2KeyForColumn1, errstring);
}

bool lds_query_db::execDeleteSelectKey(const QString &table, const QString &table2, const QString &table3, const QString &selectStr_tableKeyForColumn0_table2KeyForColumn1_table3KeyForColumn2, QString *errstring)
{

    return execDeleteSelectKey(QStringList() << table << table2 << table3, selectStr_tableKeyForColumn0_table2KeyForColumn1_table3KeyForColumn2, errstring);
}

bool lds_query_db::execSelect(const QString &sql, QString *errstring)
{
#ifdef QT_DEBUG
    // qDebug() << sql;
#endif
    return this->exec(sql,errstring);
}

bool lds_query_db::execCreate(const QString &sql, QString *errstring)
{
    return this->exec(sql, errstring);
}

bool lds_query_db::execDirect(const QString &sql, QString *errstring)
{

    return this->exec(sql, errstring);
}

bool lds_query_db::execAlter(const QString &sql, QString *errstring)
{

    return this->exec(sql, errstring);
}

bool lds_query_db::execTrigger(const QString &sql, QString *errstring)
{

    return this->exec(sql, errstring);
}

bool lds_query_db::execDrop(const QString &table, QString *errstring)
{

    return this->exec("drop table " + table, errstring);
}

int lds_query_db::recordCount()
{

    return record().count();
}


QVariant lds_query_db::recordValue(const QString &name)
{
#ifdef QT_DEBUG
    if(record().indexOf(name) < 0) {
        qDebug() << this->lastQuery();
        qDebug() << name << ", name is not valid" ;
        return QVariant();
    }
#endif
    return recordValue(record().indexOf(name));
}

QVariant lds_query_db::recordValue(int i)
{
    QVariant d;
#ifdef QT_DEBUG
    if(i >= 0 && i < record().count()) {

    } else {
        qDebug() << this->lastQuery();
        qDebug() << record().fieldName(i) << ", name is not valid" << i;
        QObject *x = 0;
        x->children();
    }
#endif
    d = record().value(i);
    return d;
}

QString lds_query_db::errorString()
{
    return recordError();
}

QString lds_query_db::recordError()
{
    return "ERROR " + QString::number(this->lastError().number()) + ":" + this->lastError().text();
}

void lds_query_db::execUpdateTrigger(const QString &table, qrtVariantPairList &key_value_list, const QString &where_sql)
{

}

void lds_query_db::execInsertTrigger(const QString &table, qrtVariantPairList &key_value_list)
{

}
