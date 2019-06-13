#include <QApplication>
#include <QLabel>
#include <QtDebug>
#include "public_logindialog.h"
#include "backheader.h"
#include "w_i_inventory.h"
#include "w_sys_manage.h"
#include "w_bt_dish.h"
#include "w_bt_dish_kitchenprinter.h"
#include "lds.h"
#include "alipay_help.h"
#include "mainwindow.h"
#include "s610pos.h"
#include "lds_calendarwidget.h"
#include "w_sys_manage_wirelessordermonitoring.h"
#include "lds.h"
#include "lds_messagebox.h"
#include <QProcess>
#include "barscale_main_dialog.h"
#include "printer_pos_serial.h"
#include "w_bt_dish_base.h"
#include "lds_roundeddialog_rect_align.h"
#include "public_pushbutton_numkey.h"
#include "electronicscale_rongta.h"
#include "system_setting_mobile_max_connect.h"
#include "printer_pos_net.h"
#include "fexpandmain_2.h"
#include "back_view_dialog.h"
#include "w_scr_dish_restaurant_dialog.h"
#include "back_view_dialog.h"
#include <QStringList>
#include "w_scr_dish_pay_widget.h"
#include "w_sys_manage_cloudsync_with_time.h"
#include "lds_toolbar_button.h"
#include "w_scr_dish_restaurant_table_select.h"
#include "w_rpt_report_frame_dishsale.h"
#include "w_rpt_report_frame_cashflow.h"
#include "w_rpt_report_frame_cash.h"
#include "w_bt_dish_kitchen_dish_state.h"
#include "w_sys_manage_outer_settings.h"
#include "w_rpt_report_frame_dishsalerank.h"
#include "colordialog.h"
#include "saas_pay_detail.h"
#include "w_sys_manage_cloudsync_cloud2.h"
#include "colordialog.h"
#include "saas_guide.h"
#include "w_scr_dish_pay_widget.h"
#include "w_scr_dish_pay_dialog.h"
#include "saas_guide_hard_test.h"
#include "w_inventory_input.h"
#include "saas_guide_hard_test.h"
#include "w_sys_manage_cloudsync_transfer.h"
#include "saas_guide_direct.h"
#include "saas_guide2.h"
#include "saas_guide_pay.h"
#include "saas_guide_num_key.h"
#include "printer_pos_vidpid.h"
#include "saas_guide_init_soft.h"
#include "fastfd_update.h"
#include "public_showimages.h"
#include "w_scr_dish_main_bar_cancel_win.h"
#ifdef QT_NEW_SYSTEM
#include <QDBusInterface>
#include "frminputnew.h"
#endif
#include "w_inventory_draw.h"
#include "saas_guide_pay_rbcode.h"
#include "commonheader.h"
#include "w_bt_dish_cookset.h"
#include "w_scr_dish_main_table_800x600.h"
#include "w_scr_dish_phone_pay.h"
#include "transparentcenterdialog.h"
#include "w_scr_dish_pay_kitchen_quit_request.h"
#include "w_scr_dish_main_view_scale.h"
#include "w_sys_manage_cloudsync_with.h"
#include "fupay_help.h"
#include "w_scr_dish_main_qr_code_over_view_cash_pay.h"
#include "txt_ins_bill_print.h"
#include "public_printby_ch_billno.h"
#include "w_sys_manage_outer_pay_set_mt_smart_cate_up.h"
#include "vch_print_memodata.h"
#include "w_bt_dish_kitchenprinter_printerset_port_dialog.h"
#include "bmp_info.h"
#include "public_printby_ch_billno_over_view.h"
#include <QtSql>
#include "labelprint_main_dialog.h"
#include "w_sys_manage_sendemail.h"
#include "w_sys_manage_sqlcopy_soft_limit2.h"
#include "w_sys_manage_cloudsync.h"
#include "w_sys_manage_sqlcopy_soft_limit2_for_language.h"
#include "cjson_help.h"
#include "lds_encrypt.h"

QString textMid(const QString &text, int pos, int length)
{
    int c = 0;
    int begin = -1;
    int  len = -1;
    for(int k = 0; k < text.count(); k ++) {
        //
        if(begin == -1 && c >= pos) {
            begin = k;
        }
        if(len == -1 && c >= pos + length) {
            len = k - begin;
            break;
        }
        //
        if(QByteArray().append(text.mid(k, 1)).length() == 3) {
            c += 2;
        } else {
            c += 1;
        }
    }
    return text.mid(begin, len);
}

static void bill_ins_table(lds_model_sqltablemodel *m, const QString &key, const QString &table_name, const QString &key_name, const QString &explain, const QVariant &value)
{
    m->tablename_kvmap_insert_field("bill_ins_table", key, "value", value);
    m->tablename_kvmap_insert_field("bill_ins_table", key, "table_name", table_name);
    m->tablename_kvmap_insert_field("bill_ins_table", key, "key_name", key_name);
    m->tablename_kvmap_insert_field("bill_ins_table", key, "explain", explain);
}

static void print(const QChar &c) {

    qDebug()<< c << "Category"<<c.category();  //Letter_Uppercase \ Letter_Other
    qDebug()<< c << "Direction"<<c.direction();
    qDebug()<< c << "Joining"<<c.joining();
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QString errstring;
    //新系统 且 unix 且 发布
#if defined(QT_NEW_SYSTEM) && defined(Q_OS_UNIX) && defined(QT_NO_DEBUG)
    frmInputNew::Instance();
#endif
    public_sql::    syssetcode();                           //设置编号
    //配置文件
    QSettings conf(lds::localdataPath +"/fastfd.ini", QSettings::IniFormat);
    lds::sysconf = &conf;
    public_sql::    syswindowicon();
    public_sql::    locale_print_file_init();
    app.setStyleSheet(public_sql::stylesheetData(lds::sysconf->value("system_setting/skin_name", "obsidian").toString()));
    if(QString("ar,uy").contains(lds::get_soft_curent_language_first()))
    {
        app.setLayoutDirection(Qt::RightToLeft);
    }
    //    app.setLayoutDirection(Qt::RightToLeft);

    switch(lds::LDS_INIT(&errstring)) {
    case lds::resultError:
        lds_messagebox::warning(0, MESSAGE_TITLE_VOID, errstring);
        return 0;
    case lds::resultInfo:
        lds_messagebox::information(0, MESSAGE_TITLE_VOID, errstring);
        break;
    default:
        break;
    }
    lds::reportModelMapValue("");
    public_sql::    systranslate();                         //翻译
    //~配置文件
    //! 0-----------------判断软件是否正常运行
#ifdef Q_OS_UNIX

    //fastfddlist fastfdlist
    QStringList fastfddlist;
    QStringList fastfdlist;
    //相比SoftPos少一个s，以免后面匹配Pos时，多一个
    public_sql::cmd_ps_pid(fastfdlist, "pos/SoftPo", "pos/SoftPos");
    //相比fastfdd少一个d，以免后面匹配Pos时，多一个
    public_sql::cmd_ps_pid(fastfddlist, "pos/fastfd", "pos/fastfdd");

    if(fastfdlist.count() > 1 || fastfddlist.count() >= 1) {//软件软已经启动，并且本次启动fastfdlist>1；副屏启动时
        if(1 == QMessageBox::critical(0, MESSAGE_TITLE_VOID, QObject::tr("软件已启动") +":"+"self"+QString::number(app.applicationPid())+"fastfdlist:"+fastfdlist.join("|")+"fastfddlist:"+fastfddlist.join("|"), QObject::tr("关闭软件"),QObject::tr("退出"))) {
            return 0;
        }

        QProcess p;
        //fastfdd异常的话，全部杀死
        foreach(const QString &pid, fastfddlist) {
            p.start("kill -9 "+pid);
            p.waitForReadyRead(5*1000);
            p.close();
        }
        //fastfd异常的话，全部杀死
        foreach(const QString &pid, fastfdlist) {
            if(pid == QString::number(app.applicationPid())) {//本身最后杀死
                continue;
            }
            p.start("kill -9 "+pid);
            p.waitForReadyRead(5*1000);
            p.close();
        }
        //杀死本身
        p.start("kill -9 "+QString::number(app.applicationPid()));
        p.waitForReadyRead(5*1000);
        p.close();
        return 0;
    }
#endif

    //! 2-----------------icon设置

//    public_sql::    arg_forbid_debug(argc, argv);               //debug模式，获取usr password debug等信息
    //! 4-----------------软件 初始化
    public_sql::    PUBLIC_SQL_INIT();                      //数据库的初始化
    public_sql::    MySQL_PARAM_INIT();                     //mysql的一些初始化
    //! 4-----------------软件到期自检

    while(false == w_sys_manage_sqlcopy_soft_limit2_for_language::checkLimit(lds::get_soft_curent_language_first(), lds::get_soft_curent_language_second(), 0)) {
        if(0 == lds_messagebox::warning(0, MESSAGE_TITLE_VOID, QObject::tr("恢复默认"), QObject::tr("确定"), QObject::tr("取消"))) {
            lds::set_soft_curent_language("zh");
            lds::set_soft_curent_language_second("en");
            public_sql::    systranslate();
            break;
        }
    }

    public_loginDialog login;
    //! 5-----------------登录界面
    public_sql::save_login = &login;
    {
        lds_messagebox_loading_show loading(0, "");
        loading.show_delay();
        //#ifdef QT_DEBUG
        //#else
        if(false == saas_pay_detail::isSaaS()) {//非saas
            loading.setText("软件到期自检");
            if(false == public_sql::syslimitcheck(0))
                return 0;               //软件到期自检
        }
        //#endif

        //新系统
        //! 5 软件版本号
        fastfd_update::setSoftVersiontCyPath(
            #if defined(QT_SCPOS_SUPPORT)
                    "SC"
            #else
                    "V"
            #endif
                    + public_sql::get_MySQL_sqlversion()/*数据库版本*/+"14"/*软件版本*/+"."+"190612"/*修改日期*/
                    );
        //登陆界面ddd
        //! 6-----------------启动
        if(false == public_sql::    sysstart(&login, &loading)) {                       //软件启动
            return 0;
        }
        //saas
        {
            //! 6 SaaS
            if(saas_pay_detail::isSaaS())
            {
#ifdef QT_DEBUG
#else
                //                saaslimitcheck
                loading.setText(QObject::tr("软件到期自检"), 100);
                if(false == saas_pay_detail::saaslimitcheck(0))
                {
                    saas_guide2 d(new saas_guide_hard_test, public_sql::save_login);
                    if(QDialog::Rejected == transparentCenterDialog(&d).exec()) {
                        return 0;
                    }
                    return 0;               //软件到期自检
                }
#endif
            }

            //    软件激活
            if(saas_pay_detail::isSaaS()) {//第二次判断是否为saas
                if(saas_pay_detail::saas_type() == "")
                {
                    saas_guide2 d(new saas_guide_direct(0, public_sql::save_login));
                    if(QDialog::Rejected == transparentCenterDialog(&d).exec()) {
                        return 0;
                    }
                }
                if(saas_pay_detail::saas_type() == "") {
                    lds_messagebox::warning(0, MESSAGE_TITLE_VOID, QObject::tr("软件未激活"));
                    return 0;
                }
                //广告
#ifdef QT_NO_DEBUG
                if(lds::sysconf->value("w_sys_manage_cloudsync_with_time/cloud_yun_is_run", false).toBool()) {
                    loading.setText(QObject::tr("云端广告下载"), 100);
                    saas_pay_detail::saas_update_adv(lds::localdataPath +"/image_double_screen_dir",  &loading);
                }
#endif
                //检测软件版本号
                //        loading.setText(QObject::tr("检测软件更新"), 100);
                //        QString version;
                //        bool isdownload;
                //        if(fastfd_update::getNewVersion(version, &isdownload, &errstring)) {
                //            loading.setText("检测软件更新");
                //            if(version <= fastfd_update::gs_soft_version) {
                //                loading.setText("已经是最新版本");
                //            } else {
                //                public_sql::softUpdate();
                //                return 0;
                //            }
                //        }
            }
        }
    }

    //! 7-----------------若是启动双屏
    qDebug() << __LINE__  << lds::sysconf->value("system_setting/fuping_mode", -1).toInt();
    if(lds::sysconf->value("system_setting/fuping_mode", -1).toInt() != -1) {
        //开始
        QString cmd_exec =app.applicationDirPath()+"/fastfdd";
        QStringList cmd_arg;
        cmd_arg << "-dp" + lds::sysconf->value("system_setting/bofanglujing").toString();
        cmd_arg << "-ap" + lds::localdataPath + "/image_double_screen_dir";
        cmd_arg << "-sd" + QString(lds::sysconf->value("system_setting/dd_show_dish_list", true).toBool() ? "1" : "0");
        cmd_arg << "-sn" + lds::sysconf->value("system_setting/skin_name", "obsidian").toString();
        cmd_arg << "-ps" + lds::sysconf->value("system_setting/screen_primary_size").toString();
        cmd_arg << "-ss" + lds::sysconf->value("system_setting/bofangg_rate").toString();

        cmd_arg << "-ty" + lds::sysconf->value("system_setting/fuping_mode", -1).toString();
        cmd_arg << "-la" + lds::sysconf->value("system_setting/bofang_layout_dir", 0).toString();
        cmd_arg << "-sx" + QString::number(lds::FULL_WINDOW_SIZE.width());

#if defined (Q_OS_UNIX)
#ifdef QT_NO_DEBUG
        QString param = QString("-display Transformed:Rot%1:LinuxFb:/dev/fb1:1 -nomouse -nokeyboard -qws")
                .arg(lds::sysconf->value("system_setting/bofangg_rote", 0).toInt());//为逆时针旋转
        cmd_exec += " " + param;
#endif
        system(QString(cmd_exec + " " + cmd_arg.join(" ")).toLocal8Bit() + "&");
#else
        QProcess::startDetached(cmd_exec, cmd_arg);
#endif
    }
    qDebug() << "currentThreadId" << QThread::currentThreadId();

    return public_sql::save_login->exec();
    bool ret = app.exec();
    return ret;
}

//db_startup_base.sql
