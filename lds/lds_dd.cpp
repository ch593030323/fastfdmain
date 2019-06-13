#include "lds_dd.h"
#include <QStringList>
#include <QUdpSocket>
#include "cjson_help.h"
#include "cJSON.h"

const int lds_dd::SYS_DOUBLE_SCREEN_PORT = 45454;
const QString lds_dd::udp_insert_request = "insert";
const QString lds_dd::udp_update_request = "update";
const QString lds_dd::udp_delete_request = "delete";
const QString lds_dd::udp_delete_all_request = "delete_all";
const QString lds_dd::udp_change_skin_request = "change_skin";

const QString lds_dd::udp_title_request = "title";
const QString lds_dd::udp_pay_request = "pay";
const QString lds_dd::udp_paying_request = "paying";
const QString lds_dd::udp_rb_code_request_begin = "rb_code_begin";
const QString lds_dd::udp_rb_code_request_end = "rb_code_end";
const QString lds_dd::udp_close_request = "close";
const QString lds_dd::udp_m_hlabels_request = "m_hlabels";
const QString lds_dd::udp_scale_init_request = "scale_init";
const QString lds_dd::udp_scale_update_request = "scale_update";

void lds_dd::DOUBLE_DATA_input(QString &d, const lds_dd::DOUBLE_DATA &data)
{
    QString string;
    d = "";
    d += data.udpKey+"\t";

    d += QString::number(data.row)+"\t";
    d += QString::number(data.count)+"\t";

    d += data.items.vch_dishname+"\t";
    d += data.items.ch_dishno+"\t";
    d += data.items.dish_price+"\t";
    d += data.items.dish_num+"\t";
    d += data.items.dish_total+"\t";
    d += data.items.vch_other+"\t";
    d += data.items.ch_suitflag+"\t";

    d += data.title_num+"\t";
    d += data.title_total+"\t";

    d += data.p_desc+"\t";
    d += data.p_receive_desc+"\t";
    d += data.p_receive+"\t";
    d += data.p_pay_desc+"\t";
    d += data.p_pay+"\t";
    d += data.p_change_desc+"\t";
    d += data.p_change+"\t";

    d += data.rb_title+"\t";
    d += data.rb_title_wechat+"\t";
    d += data.rb_title_alipay+"\t";
    d += data.rb_wechat+"\t";
    d += data.rb_alipay+"\t";
    d += data.rb_ch_paymodeno+"\t";

    d += data.m_hlabels+"\t";
    d += QString::number(data.parentPID)+"\t";

    d += data.scale_keys+"\t";
    d += data.scale_titles+"\t";
    d += data.scale_suffixs+"\t";
    d += data.scale_numbers+"\t";

    d += data.skin_name+"\t";
    //pay_master
    d += data.paying_desc+"\t";
    d += data.paying_master+"\t";
    d += data.paying_detail+"\t";
}

void lds_dd::DOUBLE_DATA_output(const QString &d, lds_dd::DOUBLE_DATA &data)
{
    int index = 0;
    QStringList list = d.split("\t");
    data.udpKey = list[index++];

    data.row = list[index++].toInt();
    data.count = list[index++].toInt();

    data.items.vch_dishname = list[index++];
    data.items.ch_dishno = list[index++];
    data.items.dish_price = list[index++];
    data.items.dish_num = list[index++];
    data.items.dish_total = list[index++];
    data.items.vch_other = list[index++];
    data.items.ch_suitflag = list[index++];

    data.title_num = list[index++];
    data.title_total = list[index++];

    data.p_desc = list[index++];
    data.p_receive_desc = list[index++];
    data.p_receive = list[index++];
    data.p_pay_desc = list[index++];
    data.p_pay = list[index++];
    data.p_change_desc = list[index++];
    data.p_change = list[index++];

    data.rb_title = list[index++];
    data.rb_title_wechat = list[index++];
    data.rb_title_alipay = list[index++];
    data.rb_wechat = list[index++];
    data.rb_alipay = list[index++];
    data.rb_ch_paymodeno = list[index++];

    data.m_hlabels = list[index++];
    data.parentPID = list[index++].toInt();

    data.scale_keys = list[index++];
    data.scale_titles = list[index++];
    data.scale_suffixs = list[index++];
    data.scale_numbers = list[index++];

    data.skin_name = list[index++];
    //pay_master
    data.paying_desc = list[index++];
    data.paying_master = list[index++];
    data.paying_detail = list[index++];
}

void lds_dd::doublescreensenddata(const lds_dd::DOUBLE_DATA &ddata)//往服务端发送信息
{
    if(ddata.udpKey.isEmpty()) return;

    QUdpSocket udpSocket;
    QString datagram;
    lds_dd::DOUBLE_DATA_input(datagram, ddata);
    QByteArray datagram_byte = datagram.toLocal8Bit();

    udpSocket.writeDatagram(datagram_byte.data(),
                            datagram_byte.size(),
                            QHostAddress::LocalHost,
                            SYS_DOUBLE_SCREEN_PORT);
}

QString lds_dd::pair2json(const QList<QPair<QString, QString> > &pair_list)
{
    cJSON_help j(cJSON_Array);
    for(int k = 0; k < pair_list.count(); k ++) {
        j.arrayAppendObject(cJSON_help::mapTocJSON(ldsVariantMap("name", pair_list[k].first, "value", pair_list[k].second)));
    }
    return j.jsonValueUnformatted();
}

QList<QPair<QString, QString> > lds_dd::json2pair(const QString &json)
{
    QList<QPair<QString, QString> > pay_info;
    cJSON_path j(json.toLocal8Bit());
    for(int k = 0, count = j.array_count(""); k < count; k ++) {
        pay_info << QPair<QString, QString>(j.array_value("[%1{name", k).toString(), j.array_value("[%1{value", k).toString());
    }
    return pay_info;
}
