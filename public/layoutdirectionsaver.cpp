#include "layoutdirectionsaver.h"

LayoutDirectionSaver::LayoutDirectionSaver(QPainter *painter, Qt::LayoutDirection will_dir) :
    QObject(0)
{
    keep(painter, will_dir);
}

LayoutDirectionSaver::LayoutDirectionSaver() :
    QObject(0)
{
    painter = 0;
}

LayoutDirectionSaver::~LayoutDirectionSaver()
{
    if(painter != 0)
        painter->setLayoutDirection(old_dir);
}

void LayoutDirectionSaver::keep(QPainter *painter, Qt::LayoutDirection will_dir)
{
    this->painter = painter;
    old_dir = painter->layoutDirection();
    painter->setLayoutDirection(will_dir);
}
