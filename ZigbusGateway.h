#ifndef ZIGBUSGATEWAY_H
#define ZIGBUSGATEWAY_H

#include <QObject>
#include <QThread>
#include <Zigbus_xPLService.h>
#include <ZigbusMessage.h>
#include <UsbZigbusConnection.h>
#include <ModuleMessageTranslator.h>

class ZigbusGateway : public QObject
{
    Q_OBJECT
    QThread xPLThread;
    QThread zigbusThread;

private:
    Zigbus_xPLService* xplService;
    AbstractZigbusConnection* zigbusConnection;
    ModuleMessageTranslator* translator;

public:
    explicit ZigbusGateway(QObject* parent = 0);
    virtual ~ZigbusGateway();
    void initializeZigbusConnection();
    void initializexPLService();
    void init();

signals:
    void start_xPLService();
    void stop_xPLService();

    void start_zigbusService();
    void stop_zigbusService();
};

#endif // ZIGBUSGATEWAY_H
