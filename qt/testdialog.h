#ifndef TESTDIALOG_H
#define TESTDIALOG_H

#include <QDialog>
#include <QKeyEvent>

class testDialog : public QDialog
{
    Q_OBJECT
public:
    explicit testDialog(QWidget *parent = 0);

    void keyPressEvent(QKeyEvent *e);
signals:

public slots:

};

#endif // TESTDIALOG_H
