#ifndef LDS_H
#define LDS_H

#include <QWidget>
#include <QVariant>
#include <QDesktopWidget>
#include <QStandardItemModel>
#include <QSettings>
#include <QHeaderView>
#ifdef QT_NO_DEBUG
#include <QNoDebug>
#endif
#include <QElapsedTimer>
#include <QMutex>
#include <QNetworkAccessManager>
#ifdef QT_DEBUG
#include <QtDebug>
#endif
#include <QPushButton>
#include "lds_dd.h"
#include "lds_const_var.h"

#define MAX_DOUBLE_VALUE  1000000000
#define MIN_DOUBLE_VALUE  -1000000000
#define DATETIME_VOID QVariant(QVariant::DateTime)
#define _CN_ "zh"
#define _EN_ "en"
#define _AR_ "ar"

//加密狗
#ifdef Q_OS_WIN
#include<QLibrary>
//typedef long ( *NoxGetLastError)();
//typedef long ( *NoxClose)(long keyHandle);
//typedef long ( *NoxFind)(long ,long * , long *); //定义函数指针
//typedef long ( *NoxOpen)(long keyHandle, char * userPin);
//typedef long ( *NoxReadStorage)(long keyHandle, unsigned char * pBuffer);

typedef long (__stdcall *NoxGetLastError)();
typedef long (__stdcall *NoxClose)(long keyHandle);
typedef long (__stdcall *NoxFind)(long ,long * , long *); //定义函数指针
typedef long (__stdcall *NoxOpen)(long keyHandle, char * userPin);
typedef long (__stdcall *NoxReadStorage)(long keyHandle,   unsigned char * pBuffer);
#endif

class lds : public lds_dd {
public:
    lds() {}
public:
    enum SIZETYPE{
        SIZEHNIT,
        SIZE
    };
    enum resultCode{
        resultOk = 0,
        resultError,
        resultInfo
    };

    static void setSystemValue(const QString &key, const QVariant &value);
    static QVariant systemValue(const QString &key, const QVariant &defValue = QVariant());

    //基础参数的设定，然而并不会对所有的参数进行赋值
    static resultCode LDS_INIT(QString *errstring = 0);
    static void systemTextCodec();

    static QPoint getPopPoint(const QPoint &gpos, const QPoint &gpos2, const QSize &popsize, Qt::LayoutDirection dir);
    static QPoint getPopPoint(QWidget *parent, QWidget *popwidget);
    static QPoint getPopPoint(QWidget *parent, const QRect &section, QWidget *popwidget);
    static QPoint getPopPoint_size(QWidget *parent, QWidget *popwidget);
    static QPoint getPopPoint0(QWidget *parent, QWidget *popwidget, SIZETYPE type);
    static QPoint getGlobalCenterPopup(QWidget *parent, QWidget *pop);
    static QPoint getParentCenterPopup(QWidget *parent, QWidget *pop);

    static QString get_sys_language();
    static QString get_soft_curent_language_first();
    static void set_soft_curent_language(const QString &key);
    static QString get_soft_curent_language_second();
    static void set_soft_curent_language_second(const QString &key);

    static  lds_const_var<QString> terminalCode;

    static QVariant dtifnull(const QString &dtstring = "");

    static void setHeaderResizeMode(QHeaderView *h, QHeaderView::ResizeMode mode);
    static void setHeaderResizeMode(QHeaderView *h, int logicalIndex, QHeaderView::ResizeMode mode);

    static  lds_const_int QT_FIXED_HEIGHT_BOTTOM;
    static  lds_const_int QT_FIXED_BETWEEN;
    static  lds_const_int QT_FIXED_EDIT_HEIGHT;
    static  lds_const_int BACK_LOGO_HEIGHT;
    static  lds_const_int BORDER_DEFAULT_HEIGHT;
    static  lds_const_int PAY_LEFT_SPACE;

    static  lds_const_var<QPoint> MAIN_WINDOW_TOPLEFT;
    static  lds_const_var<QSize> MAIN_WINDOW_SIZE;//主窗口的大小，底部高40px
    static  lds_const_var<QSize> FULL_WINDOW_SIZE;//前台,后台,中餐点菜,中餐前台
    static  lds_const_var<QString> FONT_NUM_Family;//外部数字字体
    static  lds_const_var<QString> FONT_SONGTI_Family;//外部宋体字体
    static  lds_const_var<QString> FONT_DEFAULT_Family;
    static  lds_const_var<QString> FONT_DIGIT_Family;

    static  lds_const_var<QString> LOCAL_TERMINAL_INDEX_FF;

    static void setwflagFrameless(QWidget *widget);

    static QColor static_color_light(const QString &c, double light_0_3, int a = 255);//0 1 2 3 原 亮 很亮 最亮
    static QColor static_color_light(const QColor &color, double light_0_3, int a = 255);//0 1 2 3 原 亮 很亮 最亮
    static QString appName();
    static void showWidget(QWidget *w);
    static void showWidget(QWidget *w, bool show);
    static void hideWidget(QWidget *w);
    static void hideWidget(QWidget *w, bool hide);
    static bool widgetIsShow(QWidget *w);
    static bool widgetIsHide(QWidget *w);

    static QSize dsize();

    static QString groupSeparatorV2str(double value1, int decimal, bool clearDotZero = false);
    static double groupSeparatorStr2v(const QString & text);
    static QString septNumber(int value, int decimal = 0, bool clearDotZero = false);
    static QString septNumber(double value, int decimal = 2, bool clearDotZero = false);
    static bool isSeptNumber(const QString &value);

    static int size_g_of_u(const QString &in);                                     //获取utf8-》gbk的一个中文的大小
    static QString size_g_of_u_left_justified(const QString &in, int width, QChar fill = QLatin1Char(' '), bool trunc = false);

    static QString _fromlast_cutbetween(const QString &content, const QString &cmpvalue);
    static QString strcutBygbnum(const QString &data, int width, bool isfill = false, char fill = ' ', Qt::AlignmentFlag f = Qt::AlignLeft);
    static int min_max_value_add1_loop(int min, int max, int value);

    static QStringList getUsbPathList();

    static QString of_str_decrypt(QString as_source);
    static QString of_str_encrypt(QString as_source);

    static double doubleRound(double d, int prec);

    static QString cur_machine_dt_str(const QString &displayFormat = "yyyy-MM-dd hh:mm:ss");
    static QString dt_str(const QDateTime &dt, const QString &displayFormat = "yyyy-MM-dd hh:mm:ss");

    static QString reportModelMapValue(const QVariant &dd, const QString &defaultValue = "");
    static void reportModelMapClear();

    static QByteArray fileOpenReadAll(const QString &filepath, bool *ok = 0, QString *errstring = 0);
    static bool fileWriteByte(const QString &filepath, const QByteArray &byte, QString *errstring = 0);
    static QPixmap pixmapIfNull(const QString &filepath, const QString &defaultFileValue);

    static void paintPolygon(QPainter *painter, const QRect &rect, const QColor &color);

    static QStringList removeStrList(QStringList origin, const QStringList &take);


    static QPixmap get_fastfdbacklogo();
    static QStringList get_fastfdbacktext();
    static bool resCopyFile(const QString &resFile, const QString &toFile, QString &errstring);

    static lds_const_var<QString> gs_operid;
    static lds_const_var<QString> gs_opername;
    static lds_const_var<QString> gs_password;

    static QSize pushButtonSizeHint(const QPushButton *b, const QSize &sizeHint);

    static QByteArray u2local(const QString &str);
    static QByteArray u2local(const QString &str, const QString &languageKey);

    static  QVariantMap skin_name_map;
    static  void skin_name_map_reset();

    static  lds_const_var<QString> localdataPath;
    static  lds_const_var<QString> localdataLocalePath;
    static  QSettings *sysconf;

    /////arg
    static void arg_forbid_debug(int argc, char *argv[]);                          //软件debug模式
#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
    static void arg_message_handler(QtMsgType type, const char *msg);
#else
    static void arg_message_handler(QtMsgType type, const QMessageLogContext &msg, const QString &text);
#endif
    static lds_const_var<QString> arg_d_value;

    ///linux cmd
    static void cmd_ps_pid(QStringList &pids, const QString &psFilter, const QString &exactFilter);


    ///加密狗
    enum ERROR_CODE {
        ERROR_CODE_WRONGLIB = 0,
        ERROR_CODE_NODOG,
        ERROR_CODE_WRONGDOG,
        ERROR_CODE_NOLIB,
        ERROR_CODE_NOERR
    };
    static bool dogLoad(QString &dogID, QString *errstring = 0);
    static QString error_name(ERROR_CODE code);
    static ERROR_CODE conductlock(QString &dogID);
    ///~加密狗
    ///

    static  lds_const_int SYS_QRCODE_ORDER_PORT;

    static bool isNumber(const QVariant &d);

    static void paintDense7Pattern(QPainter *painter);

    static QPixmap baseNamePixmap(const QString &filePath, const QString &baseName);
};
///~lds


class lds_widget_auto_enable{
public:
    lds_widget_auto_enable(QWidget *b) : _b(b) {b->setEnabled(false);b->repaint();if(b->parentWidget())b->parentWidget()->repaint();}
    ~lds_widget_auto_enable() {_b->setEnabled(true);}
private:
    QWidget *_b;
};

class lds_record_do_double_clicked{
public:
    lds_record_do_double_clicked()
        : press_event_do_press_mesc_off(0) {}
    bool press_event_do_double_clicked(const QString &index);
    bool press_event_do_double_clicked(const QModelIndex &index);
private:
    QString press_event_do_press_index;
    int press_event_do_press_mesc_off;
};


template <class T>
static  void ldsUpdateProgress(QObject *obj, qint64 bytesStep, qint64 bytesTotal)
{
    static_cast<T *>(obj)->updateProgress(bytesStep, bytesTotal);
}

template<class T>
static QFont ldsObjFont(T *obj, const QString &font_family)
{
    QFont font = obj->font();
    font.setFamily(font_family);
    return font;
}

#endif // LDS_H
