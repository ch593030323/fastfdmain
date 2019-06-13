#include "lds_tableview_delegate_time.h"
#include <QDateTimeEdit>
#include <QTimeEdit>
#include "lds_datetimeedit.h"

lds_tableview_delegate_time::lds_tableview_delegate_time(const QString &format, QObject *parent)
    :lds_tableview_delegate_virtual(parent)
    , _format(format)
{

}

QWidget *lds_tableview_delegate_time::createEditor(QWidget *aParent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option)
    QDateTime t = QDateTime::fromString(index.data().toString(), _format);
    lds_datetimeEdit *editor=new lds_datetimeEdit(aParent);
    editor->setDisplayFormat(_format);
    editor->enablePopAdjustPostion();
    editor->setDateTime(t);
    return editor;
}

void lds_tableview_delegate_time::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    lds_datetimeEdit *editor0 = qobject_cast<lds_datetimeEdit *>(editor);
    model->setData(index, editor0->dateTime());
}

void lds_tableview_delegate_time::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    lds_datetimeEdit *editor0=qobject_cast<lds_datetimeEdit *>(editor);
    editor0->setDateTime(index.data().toDateTime());
}
