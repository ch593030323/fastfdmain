#ifndef CLIENTDISPLAY_H
#define CLIENTDISPLAY_H

#include <QObject>
#include "lds_const_var.h"

class ClientDisplay_NO;

//客显得波特率已经内置了，不需要添加
//
class ClientDisplay
{
public:
    explicit ClientDisplay(const QString &type);
    ClientDisplay();
    void setType(const QString &type);

    ~ClientDisplay();

    bool writeCom(int sqr, double value, const QString &devpath, const QString &name = QString());
    bool writeCom(const QMap<int, double> &map, const QString &devpath, const QString &name = QString());
    bool cleanCom(const QString &devpath, const QString &/*baudrate*/);
    bool startShowinfo(const QString &text, const QString &devpath);
    QString lastError();

    lds_const_var<QString> _type;
private:
    QString err0;

    static QByteArray DingHeng_flag;
    static QByteArray DingHeng_num;
    static QByteArray DingHeng_price;
    static QByteArray DingHeng_total;
    static QByteArray DinghengDishIns();
///
public:
    static QStringList creat_clientdisplay_ledtypes();
};

#endif // CLIENTDISPLAY_H
