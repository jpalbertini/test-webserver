#include <QCoreApplication>

#include "CivetServer.h"
#include <cstring>
#include <ctime>

#include "webserver.h"
#include "ressourceaccess.h"

#ifdef _WIN32
    #include <windows.h>
#else
    #include <unistd.h>
#endif

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    const char *options[] = { "listening_ports", "8080"};

    std::vector<std::string> cpp_options;
    for(auto option : options)
        cpp_options.push_back(option);

    CivetServer server(cpp_options); // <-- C++ style start

    WebServer wServer;
    RessourceAccess rAccess;

    server.addHandler("", wServer);
    wServer.registerDataService("data", [&]() {
        return rAccess.getData();
    });

    return a.exec();
}
