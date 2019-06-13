#include "lds_thread.h"
#include <QtDebug>
#include <QEventLoop>
#include <QTimer>
#include <QSqlDatabase>
#include <QApplication>
#include <QFile>
#include <QSqlQuery>
static int SQL_THREAD_COUNT = 0;

const Qt::HANDLE lds_thread::MAIN_THREAD_ID = QThread::currentThreadId();
lds_thread::lds_thread(const QString &title, QObject *parent) :
    QThread(parent)
{
    this->title = title;
    isstarted = false;
    busy_state = false;
    connect(this,SIGNAL(started()),this,SLOT(updatestate()));
}

lds_thread::~lds_thread()
{
    this->quit();
    this->wait();
    qDebug() << __FUNCTION__ << title;
}

bool lds_thread::waitForStarted(int msecs)
{
    int count = msecs/100;
    QEventLoop l;
    while(count >= 1 && count--&&!isstarted) {
        QTimer::singleShot(100, &l, SLOT(quit()));
        l.exec();
    }
    return isstarted;
}

QString lds_thread::getThreadDbname()
{

    if(MAIN_THREAD_ID == QThread::currentThreadId()) {
        return QSqlDatabase::defaultConnection;
    }
    return QString::number((unsigned long)QThread::currentThreadId());
}

//!约定的主线程的mysql数据库别名为：空
//!约定的子线程的mysql数据库别名为：线程号
//!约定的主线程的其他数据库别名为：非空、非defconnectionname
//!约定的子线程的其他数据库别名为：线程号+"-"+connectionname
//! 本函数值获取mysql数据库的别名
//!若一个项目连接n个的数据库时，则线程最多也是n个数据库别名，命名方式：线程号+数据库别名
//!mysql数据库别名必须为空
QString lds_thread::getThreadDbnameAndCreat()
{
    return getThreadDbAndCreat().databaseName();
}

QSqlDatabase lds_thread::getThreadDbAndCreat()
{
    //子线程
    //1
    QString dbname = getThreadDbname();
    QSqlDatabase db;
    //3
    if(!QSqlDatabase::connectionNames().contains(dbname)) {
        if(QSqlDatabase::defaultConnection == dbname) {
            db = QSqlDatabase::addDatabase("QMYSQL", dbname);
            qDebug() << __LINE__ << __FUNCTION__ << "addDatabase dbname:" << dbname << ++SQL_THREAD_COUNT;
        } else {
            db = QSqlDatabase::cloneDatabase(QSqlDatabase::database(QSqlDatabase::defaultConnection), dbname);//Does nothing if other is an invalid database
            qDebug() << __LINE__ << __FUNCTION__ << "cloneDatabase dbname:" << dbname << ++SQL_THREAD_COUNT;
        }
    } else {
        db = QSqlDatabase::database(dbname, false);
    }

    return db;
}

QSqlDatabase lds_thread::getThreadDbAndOpen()//第一次操作数据库时,是需要false 的
{
    QSqlDatabase db = getThreadDbAndCreat();
    if(false == db.isOpen()) {
        db.open();
        QSqlQuery query(db);
        query.exec(
                    "SET sql_mode = 'STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION';"
                    );
    }
    return db;
}

void lds_thread::msecSleep(unsigned long msec)
{
    QThread::msleep(msec);
}

void lds_thread::secSleep(unsigned long sec)
{
    QThread::sleep(sec);
}
int lds_thread::execEvent(lds_thread_event *event)
{
    connect(this,SIGNAL(signalTrigger()),event, SLOT(toExecBusy()));//若是正常打印，则会跳过
    connect(event, SIGNAL(signal_busy(bool)), this,SLOT(toSetBusy(bool)));

    return lds_thread::exec();
}

int lds_thread::exec()
{
    int r = QThread::exec();

    QString dbname = getThreadDbname();
    if(QSqlDatabase::connectionNames().contains(dbname)) {
        QSqlDatabase::removeDatabase(dbname);
    }
    return r;
}

bool lds_thread::isBusy()
{
    return busy_state;
}

void lds_thread::emitsignalTrigger()
{
    if(isBusy()) {
        qDebug() << title + "------------BUSY";
        return;
    }
    emit signalTrigger();
}

void lds_thread::toSetBusy(bool flag)
{
    busy_state = flag;
}

void lds_thread::updatestate()
{
    isstarted = true;
}


void lds_thread_event::toExecBusy()
{
    emit signal_busy(true);
    toExec();
    emit signal_busy(false);
}
