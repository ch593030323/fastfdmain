#include "public_dialog.h"
#include <QApplication>
#include <QFontDatabase>
#include <QtDebug>
#include "w_scr_dish_pay_module_select.h"
#include "w_bt_dish.h"
#include "w_i_inventory.h"
#include "public_sql.h"
#include <QApplication>
#include "w_m_member.h"
#include "w_sys_manage_outer_pay_set.h"
#include "restauranttableview.h"
#include "w_rpt_report_frame.h"

pointerAutoDel<QTranslator> public_Dialog::curTranslate;
pointerAutoDel<QTranslator> public_Dialog::printTranslate;

public_Dialog::public_Dialog(QWidget *parent)
    : QDialog(parent)
{
}

void public_Dialog::comIndexSaveClear(QComboBox *com)
{
    int idx = com->currentIndex();
    if(com->isEditable()) {
        if(com->itemText(idx) != com->currentText()) {
            com_index.insert(com, com->currentText());
            return;
        }
    }
    com_index.insert(com, idx);
    com->clear();
}

void public_Dialog::comIndexRestore(QComboBox *com)
{
    QVariant d = com_index.value(com);
    if(d.type() == QVariant::Int) {
        com->setCurrentIndex(d.toInt());
        return;
    }
    if(d.type() == QVariant::String) {
        com->setEditText(d.toString());
        return;
    }
    com->setCurrentIndex(-1);
}

void public_Dialog::comIndexRestoreAll()
{
    for(QMap<QComboBox*, QVariant>::iterator k = com_index.begin();
        k != com_index.end();
        k++) {
        comIndexRestore(k.key());
    }
}

void public_Dialog::clearCache()
{
    w_scr_dish_pay_module_select::clearCache();
    w_bt_dish::clearCache();
    w_i_inventory::clearCache();
    w_m_member::clearCache();
    w_sys_manage_outer_pay_set::clearCache();
    restraurantDelegate::clearCache();
    w_rpt_report_frame::clearCache();
    lds::reportModelMapClear();
    lds::skin_name_map_reset();

    resetPayModeName();
}

void public_Dialog::resetPayModeName()
{
    lds_query_hddpos  query;
    if(!query.db.isOpen()) {
        return;
    }
    lds_query_hddpos::tran_saction();
    query.execUpdate("cey_bt_paymode", "vch_arg4","N", "vch_paymodename", QObject::tr("现金"), "ch_paymodeno = '00'");
    query.execUpdate("cey_bt_paymode", "vch_arg4","N", "vch_paymodename", QObject::tr("充值卡"), "ch_paymodeno = '02'");
    query.execUpdate("cey_bt_paymode", "vch_arg4","N", "vch_paymodename", QObject::tr("银行卡"), "ch_paymodeno = '03'");
    query.execUpdate("cey_bt_paymode", "vch_arg4","N", "vch_paymodename", QObject::tr("代金券"), "ch_paymodeno = '04'");
    query.execUpdate("cey_bt_paymode", "vch_arg4","N", "vch_paymodename", QObject::tr("微信支付"), "ch_paymodeno = '05'");

    query.execUpdate("cey_bt_paymode", "vch_arg4","N", "vch_paymodename", QObject::tr("支付宝支付"), "ch_paymodeno = '06'");
    query.execUpdate("cey_bt_paymode", "vch_arg4","T", "vch_paymodename", QObject::tr("百度糯米"), "ch_paymodeno = '07'");
    query.execUpdate("cey_bt_paymode", "vch_arg4","T", "vch_paymodename", QObject::tr("大众点评"), "ch_paymodeno = '08'");
    query.execUpdate("cey_bt_paymode", "vch_arg4","T", "vch_paymodename", QObject::tr("美团"), "ch_paymodeno = '09'");
    query.execUpdate("cey_bt_paymode", "vch_arg4","N", "vch_paymodename", QObject::tr("云会员卡"), "ch_paymodeno = '10'");

    query.execUpdate("cey_bt_paymode", "vch_arg4","N", "vch_paymodename", QObject::tr("会员挂账"), "ch_paymodeno = '11'");
    query.execUpdate("cey_bt_paymode", "vch_arg4","N", "vch_paymodename", QObject::tr("手工抹零"), "ch_paymodeno = 'XX'");
    query.execUpdate("cey_bt_paymode", "vch_arg4","N", "vch_paymodename", QObject::tr("自动抹零"), "ch_paymodeno = 'YY'");
    query.execUpdate("cey_bt_paymode", "vch_arg4","N", "vch_paymodename", QObject::tr("优惠"), "ch_paymodeno = 'ZZ'");
    query.execUpdate("cey_bt_paymode", "vch_arg4","N", "vch_paymodename", QObject::tr("摇钱树支付"), "ch_paymodeno = 'AA'");
    query.execUpdate("cey_bt_paymode", "vch_arg4","N", "vch_paymodename", QObject::tr("快捷支付"), "ch_paymodeno = 'BB' ");

    query.execUpdate("cey_bt_paymode", "vch_arg4","N", "vch_paymodename", QObject::tr("龙支付"), "ch_paymodeno = 'CC' ");
    query.execUpdate("cey_bt_paymode", "vch_arg4","N", "vch_paymodename", QObject::tr("微支付"), "ch_paymodeno = 'DD' ");

    query.execUpdate("cey_bt_paymode", "vch_arg4","N", "vch_paymodename", QObject::tr("客户"), "ch_paymodeno = 'CL' ");
    lds_query_hddpos::com_mit();
}

QTranslator *public_Dialog::get_printTranslate(const QString &languageKey)
{
    static QString preKey;

    if(preKey != languageKey) {
        preKey = languageKey;
        printTranslate.delPointer();
    }
    if( 0 == printTranslate.getPointer() ) {
        printTranslate.setPointer(new QTranslator);
    }
    if(printTranslate.getPointer()->isEmpty()) {
        printTranslate.getPointer()->load("userdata/settings/language/"+languageKey+"_fastfd.qm");
    }
    return printTranslate.getPointer();
}

void public_Dialog::del_printTranslate()
{
    printTranslate.delPointer();
}

void public_Dialog::retranslateSystem(const QString &key)
{
    if(0 != curTranslate.getPointer()
            &&!curTranslate.getPointer()->isEmpty()) {
        qApp->removeTranslator(curTranslate.getPointer());
        curTranslate.delPointer();
    }
    if( 0 == curTranslate.getPointer() ) {
        curTranslate.setPointer(new QTranslator);
    }

    qDebug() << "FONT_DEFAULT_Family" << lds::FONT_DEFAULT_Family;
    //userdata/settings/language/KO_fastfd.qm
    QString qmpath = QString("userdata/settings/language/") + key + "_fastfd.qm";
    QString font_path_language = "userdata/settings/language/" + key + ".ttf";
    if(QFile::exists(font_path_language)) {
        int fid = QFontDatabase::addApplicationFont(font_path_language);
        if(fid != -1) {
            QString family = QFontDatabase::applicationFontFamilies(fid).value(0);
            qApp->setFont(ldsObjFont(qApp, family));
        }
    } else {
        qApp->setFont(ldsObjFont(qApp, lds::FONT_DEFAULT_Family));
    }
    if(curTranslate.getPointer()->load(qmpath)) {
        qApp->installTranslator(curTranslate.getPointer());
        qDebug() << __FILE__ << __LINE__ << "installTranslator";
    }

    if(lds::sysconf->value("system_setting/SYS_LANGUAGE").toString() != key) {
        lds::sysconf->setValue("system_setting/SYS_LANGUAGE", key);
        //充值后台的中文的title
        QFile::remove("pos/fastfdbacktext.txt");
    }
    public_Dialog::clearCache();
}
