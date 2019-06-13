#ifndef LDS_RECORD_DO_LONGPRESS_H
#define LDS_RECORD_DO_LONGPRESS_H

#include <QObject>
#include <QTimeLine>
#include <QLabel>
#include <QPoint>
#include <QModelIndex>

class lds_record_do_longpress : public QObject
{
    Q_OBJECT
public:
    explicit lds_record_do_longpress(QWidget *parent);

    void setEnable(bool enable);

    void start(const QPoint &pos);
    void stop();

    bool isprepared();
    bool isprepared_stop();
signals:
    void timeout();
    void timeout(const QModelIndex &index);
private slots:
    void framerun(int frame);
private:
    QTimeLine *timeline;
    QLabel *loading_label;
    QPoint press_pos;
    bool enable;
};

#endif // LDS_RECORD_DO_LONGPRESS_H
