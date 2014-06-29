#include <ZigbusGateway.h>

ZigbusGateway::ZigbusGateway(QObject* parent)
    : QObject(parent)
{
    xplService = NULL;
    zigbusConnection = NULL;
    translator = new ModuleMessageTranslator(this);
}
//----------------------------------------------------------------
ZigbusGateway::~ZigbusGateway()
{
    xPLThread.quit();
    xPLThread.wait(); //wait until the end of execution
    zigbusThread.quit();
    zigbusThread.wait(); // wait until the end of execution
    if(translator != NULL) {
        delete translator;
    }
}
//----------------------------------------------------------------
void ZigbusGateway::init()
{
    initializexPLService();
    //initializeZigbusConnection();
    emit start_xPLService();
}
//----------------------------------------------------------------
void ZigbusGateway::initializexPLService()
{
    xplService = new Zigbus_xPLService("zigbus-gateway.static0");
    xplService->moveToThread(&xPLThread);
    connect(&xPLThread, SIGNAL(finished()), xplService, SLOT(deleteLater()));
    connect(this, SIGNAL(start_xPLService()), xplService, SLOT(open()));
    connect(this, SIGNAL(stop_xPLService()), xplService, SLOT(close()));
    connect(translator, SIGNAL(xPLMessageToSend(QxPLMessage)), xplService, SLOT(sendMessage(QxPLMessage)));
    connect(xplService, SIGNAL(moduleTargetedMessage(QxPLMessage)), translator, SLOT(processMessage(QxPLMessage)));
    xPLThread.start();
}
//----------------------------------------------------------------
void ZigbusGateway::initializeZigbusConnection()
{
    zigbusConnection = new UsbZigbusConnection("/dev/ttyUSB0");
    zigbusConnection->moveToThread(&zigbusThread);
    connect(&zigbusThread, SIGNAL(finished()), zigbusConnection, SLOT(deleteLater()));
    connect(this, SIGNAL(start_zigbusService()), zigbusConnection, SLOT(open()));
    connect(this, SIGNAL(stop_zigbusService()), zigbusConnection, SLOT(close()));
    connect(zigbusConnection, SIGNAL(messageReceived(ZigbusMessage)), translator, SLOT(processMessage(ZigbusMessage)));
    connect(translator, SIGNAL(zigbusMessageToSend(ZigbusMessage)), zigbusConnection, SLOT(sendMessage(ZigbusMessage)));
    zigbusThread.start();
}

