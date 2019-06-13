#ifndef LAYOUTDIRECTIONSAVER_H
#define LAYOUTDIRECTIONSAVER_H

#include <QObject>
#include <QPainter>

class LayoutDirectionSaver : public QObject
{
    Q_OBJECT
public:
    LayoutDirectionSaver(QPainter *painter, Qt::LayoutDirection will_dir);
    LayoutDirectionSaver();
    ~LayoutDirectionSaver();

    void keep(QPainter *painter, Qt::LayoutDirection will_dir);
private:
    QPainter*painter;
    Qt::LayoutDirection old_dir;
};

#endif // LAYOUTDIRECTIONSAVER_H
