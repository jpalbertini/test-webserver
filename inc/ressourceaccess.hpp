#ifndef RESSOURCEACCESS_H
#define RESSOURCEACCESS_H

#include <QObject>
#include <QVariantMap>

#define VALUES "values"

class RessourceAccess : public QObject
{
    Q_OBJECT
public:
    RessourceAccess(QObject* parent = Q_NULLPTR) : QObject(parent)
    {

    }

    const QVariantMap& getData()
    {
        data[VALUES] = data.value(VALUES, 0).toInt() + 1;
        return data;
    }

private:
    QVariantMap data;
};

#endif // RESSOURCEACCESS_H
