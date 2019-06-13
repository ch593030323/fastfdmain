#include "lds_roundeddialog.h"
#include <QPainter>

lds_roundeddialog::lds_roundeddialog(QWidget *parent) :
    QDialog(parent)
{
    setAttribute(Qt::WA_TranslucentBackground, true);
    setWindowFlags(Qt::Popup);
    setBgColor(QColor("slategrey"));
}

void lds_roundeddialog::setBgColor(const QColor &color)
{
    bgcolor=color;
}

QColor lds_roundeddialog::getBgColor() const
{
    return bgcolor;
}

void lds_roundeddialog::paintEvent(QPaintEvent *e)
{
    QPainter p(this);
    p.setPen(Qt::NoPen);
    p.fillRect(this->rect(), Qt::transparent);
    p.setBrush(bgcolor);
    p.setRenderHint(QPainter::Antialiasing);
    p.drawRoundedRect(0, 0, width(), height(), 20, 20);
}
