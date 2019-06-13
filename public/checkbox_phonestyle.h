#ifndef CHECKBOX_PHONESTYLE_H
#define CHECKBOX_PHONESTYLE_H

#include <QPushButton>
#include <QMap>
#include <QVariant>

class checkbox_phonestyle : public QPushButton
{
    Q_OBJECT
public:
    explicit checkbox_phonestyle(QWidget *parent = 0);

    void setData(int role, const QVariant &value);
    QVariant getData(int role);
protected:
    void paintEvent(QPaintEvent *);
private:
    QMap<int, QVariant> map;
    int icon_width;

};

#endif // CHECKBOX_PHONESTYLE_H
