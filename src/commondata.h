#ifndef COMMONDATA_H
#define COMMONDATA_H
#include <QVector>
#include <QVariant>

namespace common {

    struct NameDesc {
        QString name;
        QString desc;
    };

    struct NameValue {
        QString name;
        QVariant value;
    };

    typedef QVector<NameDesc> Names;

    typedef QVector<NameValue> Values;

}

#endif // COMMONDATA_H
