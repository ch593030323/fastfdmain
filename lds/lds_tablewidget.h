#ifndef LDS_TABLEWIDGET_H
#define LDS_TABLEWIDGET_H

#include <QTableWidget>

class lds_tablewidget : public QTableWidget
{
    Q_OBJECT
public:
    explicit lds_tablewidget(QWidget *parent = 0);

    void appHorizontalHeader(const QString &key, const QString &text, bool show = true);
    int fieldIndex(const QString &key) const;

    QModelIndex index(int row, const QString &key);
    QModelIndex index(int row, int column);

    void setText(int row, const QString &key, const QString &text);
    void setText(int row, int column, const QString &text);

    QString text(int row, const QString &key) const;
    QString text(int row, int column) const;
    int indexOf(const QString &fieldName, const QString &fieldValue) const;

    void setFlags(int row, const QString &key, Qt::ItemFlags flags);

    QStringList fieldNameList();

    void setContentSize(int rowcount, int columncount);
private:
    QList<QPair<QString, QString> >horizontalHeaderList;
};

#endif // LDS_TABLEWIDGET_H
