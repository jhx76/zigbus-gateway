#ifndef ZIGBUS_XPLSERVICE_H
#define ZIGBUS_XPLSERVICE_H

#include <QxPLService.h>
//#include <ZigbusMessage.h>

class Zigbus_xPLService : public QxPLService
{
    Q_OBJECT
private:
    void xPL_config_response_cmd(const QxPLMessage &message);

public:
    Zigbus_xPLService(const QString& identifier, QObject* parent = 0);
    void initFilters();
    void processMessageForGateway(const QxPLMessage& message);

public slots:
    void xPLMessageProcess(QxPLMessage message);

signals:
    void moduleTargetedMessage(QxPLMessage message);
};

#endif // ZIGBUS_XPLSERVICE_H
