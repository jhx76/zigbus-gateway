#include <QCoreApplication>
#include <ZigbusGateway.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    qRegisterMetaType<ZigbusMessage>();
    qRegisterMetaType<QxPLMessage>();
    ZigbusGateway gateway;
    gateway.init();
    return a.exec();
}
