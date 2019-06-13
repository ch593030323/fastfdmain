#ifndef LDS_ROUNDEDDIALOG_H
#define LDS_ROUNDEDDIALOG_H

#include <QDialog>

class lds_roundeddialog : public QDialog
{
    Q_OBJECT
    Q_PROPERTY(QColor bgcolor READ getBgColor WRITE setBgColor DESIGNABLE true)

public:
    explicit lds_roundeddialog(QWidget *parent = 0);


protected:
    void setBgColor(const QColor &color);
    QColor getBgColor() const;
    void paintEvent(QPaintEvent *e);
    QColor bgcolor;

};

#endif // LDS_ROUNDEDDIALOG_H
