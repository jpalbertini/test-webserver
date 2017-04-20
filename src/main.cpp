#include <QCoreApplication>

#include "webserver.h"
#include "ressourceaccess.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    WebServer wServer;
    RessourceAccess rAccess;

    wServer.registerDataService("data", [&]() {
        return rAccess.getData();
    });

    return a.exec();
}
