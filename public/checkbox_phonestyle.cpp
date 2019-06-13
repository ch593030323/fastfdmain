#include "checkbox_phonestyle.h"
#include <QVariant>
#include <QStylePainter>
#include <QStyleOptionButton>
#include "lds_checkbox.h"

checkbox_phonestyle::checkbox_phonestyle(QWidget *parent) :
    QPushButton(parent)
{
    QString check_png_align = "background-position: right;";
    if(this->layoutDirection() == Qt::RightToLeft) {
        check_png_align = "background-position: left;";
    }
    icon_width = QPixmap(lds_CheckBox::png_h30_path_appcheckon()).width();
    this->setStyleSheet(
                "        QPushButton{text-align : left;min-height:30px;}"
                "        QPushButton:checked{"
                "            background-image: url(" + lds_CheckBox::png_h30_path_appcheckon()+ ");"
                "            background-repeat:no-repeat;" + check_png_align +
                "            border:0px solid black;"
                "        }"
                "        QPushButton:!checked{"
                "            background-image: url(" + lds_CheckBox::png_h30_path_appcheckoff()+ ");"
                "            background-repeat:no-repeat;" + check_png_align +
                "            border:0px solid black;"
                "        }        "
                );
    this->setCheckable(true);
    this->setProperty("outer_stylesheet", QVariant("pushbutton_check"));
}

void checkbox_phonestyle::setData(int role, const QVariant &value)
{
    map.insert(role, value);
}

QVariant checkbox_phonestyle::getData(int role)
{
    return map.value(role);
}

void checkbox_phonestyle::paintEvent(QPaintEvent *)
{
    QStylePainter p(this);
    QStyleOptionButton option;
    initStyleOption(&option);
    option.text = "";
    p.drawControl(QStyle::CE_PushButton, option);
    option.rect.adjust(0, 0, -icon_width, 0);
    option.text = this->text();
    p.drawText(option.rect, Qt::AlignLeft | Qt::AlignVCenter | Qt::TextWordWrap, option.text);
}
