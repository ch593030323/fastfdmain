#ifndef LDS_THREAD_H
#define LDS_THREAD_H

#include <QThread>
#include <QStringList>
#include <QSqlDatabase>

class lds_thread_event : public QObject {
    Q_OBJECT
public:
    lds_thread_event(QObject *parent = 0) : QObject(parent) {}
public slots:
    virtual void toExec() = 0;



    ////////////////////
Q_SIGNALS:
    void signal_busy(bool);
public slots:
    void toExecBusy();
};


//2 能够正常析构
//3 线程不进行统一管理静态函数
class lds_thread : public QThread
{
    Q_OBJECT
public:
    explicit lds_thread(const QString &title, QObject *parent = 0);
    ~lds_thread();
    bool waitForStarted(int msecs = 50*100);
    static QString getThreadDbname();
    static QString getThreadDbnameAndCreat();
    static QSqlDatabase getThreadDbAndCreat();
    static QSqlDatabase getThreadDbAndOpen();
    static const Qt::HANDLE MAIN_THREAD_ID;
    static void msecSleep(unsigned long msec);
    static void secSleep(unsigned long sec);
    ////////////////////////一般run的个数之存在一个
protected:
    int execEvent(lds_thread_event *event);
    int exec();
public:
    bool  isBusy();
signals:
    void signalTrigger();
public slots:
    void emitsignalTrigger();
    void toSetBusy(bool flag);
protected:
    bool busy_state;
    ////////////////////////一般run的个数之存在一个
private slots:
    void updatestate();
protected:
    QString title;
private:
    bool isstarted;
};
#endif // LDS_THREAD_H
