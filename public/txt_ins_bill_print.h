#ifndef TXT_INS_BILL_PRINT_H
#define TXT_INS_BILL_PRINT_H

#include <QObject>
#include <QtGui>
#include <QtNetwork>
#include <cJSON.h>
#include "printer_pos_serial.h"
#include "lds_model_sqltablemodel_d.h"


class txt_ins_bill_print : public QObject
{
    Q_OBJECT
public:

    struct resloveOne{
        resloveOne() {init();}
        QString str;
        bool flag;

        void init() {
            str.clear();
            flag = false;
        }
        void clear() {init();}
    };

    struct resloveData{
        resloveData() {init();}
        int begin;
        int k_off;

        resloveOne txtQuestion;//%?p
        resloveOne txtAlign;//%+p
        resloveOne param;//%p
        resloveOne hex;//%3
        resloveOne txt;//%你好

        bool tableFlag;//%[
        bool cellFlag;//%(
        bool percentFlag;//%%

        QString picPath;
        QString qrcTxt;
        QString barTxt;

        void init() {
            begin = -1;
            k_off = 0;

            txtQuestion.clear();
            txtAlign.clear();
            param.clear();
            hex.clear();
            txt.clear();

            tableFlag = false;
            cellFlag = false;
            percentFlag = false;

            picPath = "";
            qrcTxt = "";
            barTxt = "";
        }
        void print() {
            qDebug() << "begin" << begin;
            qDebug() << "k_off" << k_off;
            qDebug() << "txtAlign" << txtAlign.str;
        }
        int rpLength(int k);
    };

    struct lineData{
        lineData() {init();}
        lineData(int begin, int end, const QString&replaceValue)
            : begin(begin)
            , end(end)
            , replaceValue(replaceValue) {}

        int begin;
        int end;
        QString replaceValue;

        void init() {begin = end = -1;}
        void replace(QString &str);
    };

    struct resloveTable{
        resloveTable() {init();}
        bool prepare_begin;
        int prepare_begin_column_count;
        bool prepare_end;
        bool begin;
        int index;
        int index_count;

        void init() {
            prepare_begin = false;//%[
            prepare_begin_column_count = 1;
            prepare_end = false;//%]
            index = -1;//reslove 时复制, -1表示没有运行
            index_count = -1;// mapValue 时赋值

            begin = false;// mapValue 时赋值
        }
    };

    struct resloveCell{
        resloveCell() {init();}
        bool prepare_begin;
        bool prepare_end;
        bool begin;
        QString param;

        void init() {begin = prepare_begin = prepare_end = false; param = "";}
    };

    static const QByteArray HEX_FONT_BOLD0;
    static const QByteArray HEX_FONT_BOLD1;
    static const QByteArray HEX_FONT_SCALE00;
    static const QByteArray HEX_FONT_SCALE01;
    static const QByteArray HEX_FONT_SCALE10;
    static const QByteArray HEX_FONT_SCALE11;
    static const QByteArray HEX_ALIGN_CENTER;
    static const QByteArray HEX_ALIGN_LEFT;
public:
    txt_ins_bill_print(const lds_model_sqltablemodel_d *model_d, const QString &format_data, QObject *parent = 0);

    blockIns bill_before_reslove();
    blockIns bill_atfter_reslove();
private:
    blockIns print_for_image();
private:
    blockIns reslove(const QString &str);

    blockIns resloveLine(QString line);
    blockIns resloveLine(QString line, bool with_control);//3
    QString resloveText(QString line);

    blockIns resloveResult(const QString &str);
    blockIns resloveLineIns(const QString &line, bool contentKip = false);

    int resloveQuestion(const QString &line, int k, QString &ret);
    int resloveAlign(const QString &line, int k, QString &ret, bool &hasParam);
    resloveData resloveAlignFlag(const QString &line, int k, resloveData d);

    QString mapValue(const QString &key, bool *ok = 0);
    bool isInt(const QString &str);
    int getInt(const QString &str, int begin, int &intValue);
    int txtWidth(const QString &txt);
    QString hex2_Xstr(const QString &hex);
    QString flagFill(const QString &flag/*+-*/, int width, const QString &str);
    QString getWord(const QString &text, int k, QStringList splitValues = QStringList());
    QString mid(const QString &line, int begin, int end);

private:
    resloveTable ins_table;
    resloveCell ins_cell;
    const lds_model_sqltablemodel_d *model_d;
    QString format_data;

    int map_valid_count;
};

#endif // TXT_INS_BILL_PRINT_H
