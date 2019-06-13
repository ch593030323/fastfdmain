#include "testdialog.h"

testDialog::testDialog(QWidget *parent) :
    QDialog(parent)
{
}

void testDialog::keyPressEvent(QKeyEvent *e)
{
    if(e->key() == Qt::Key_Return) {
        this->accept();
    }
    QDialog::keyPressEvent(e);
}
