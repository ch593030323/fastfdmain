#include "lds_query_variant.h"

void qrtVariant::setType(qrtVariant::qrtVariantType type) const
{
    _type = type;
}

QString qrtVariant::toString() const
{
    return toString(_value);
}

QString qrtVariant::toString(const QVariant &d)
{
    QVariant::Type datatype = d.type();
    if(QVariant::Date == datatype) {
        return d.toDate().toString("yyyy-MM-dd");
    }
    if(QVariant::Time == datatype) {
        return d.toTime().toString("hh:mm:ss");
    }
    if(QVariant::DateTime == datatype) {
        return d.toDateTime().toString("yyyy-MM-dd hh:mm:ss");
    }
    if(QVariant::Double == datatype) {
        return QString("%1").arg(d.toDouble());
    }
    return d.toString();
}
