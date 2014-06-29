#include "Zigbus_xPLService.h"
#include <ZigbusMessage.h>

Zigbus_xPLService::Zigbus_xPLService(const QString& identifier, QObject* parent)
    : QxPLService(identifier, parent)
{
    initFilters();
}

void Zigbus_xPLService::initFilters()
{
    /*
      permet de recevoir les command control.basic à destination des
      modules zigbus
      */
    QxPLFilter filterControlBasicForModules;
    filterControlBasicForModules.setType(QxPLMessage::COMMAND);
    filterControlBasicForModules.setSource(QxPLIdentifier::broadcast());
    filterControlBasicForModules.setTarget(QxPLIdentifier("zigbus", "module", "*"));
    filterControlBasicForModules.setMessageSchema("control");
    filterControlBasicForModules.setMessageClass("basic");
    filters().append(filterControlBasicForModules);

    /*
      permet de recevoir les cmd config.response
      */
    QxPLFilter filterConfigResponseForMe;
    filterConfigResponseForMe.setType(QxPLMessage::COMMAND);
    filterConfigResponseForMe.setSource(QxPLIdentifier::broadcast());
    filterConfigResponseForMe.setTarget(getIdentifier());
    filterConfigResponseForMe.setMessageSchema("config");
    filterConfigResponseForMe.setMessageClass("response");
    filters().append(filterConfigResponseForMe);

    /*
      permet de recevoir les cmd config.list
     */
    QxPLFilter filterConfigListForMe;
    filterConfigListForMe.setType(QxPLMessage::COMMAND);
    filterConfigListForMe.setSource(QxPLIdentifier::broadcast());
    filterConfigListForMe.setTarget(getIdentifier());
    filterConfigListForMe.setMessageSchema("config");
    filterConfigListForMe.setMessageClass("list");
    filters().append(filterConfigListForMe);

    /*
      permet de recevoir les config.current
      */
    QxPLFilter filterConfigCurrentForMe;
    filterConfigCurrentForMe.setType(QxPLMessage::COMMAND);
    filterConfigCurrentForMe.setSource(QxPLIdentifier::broadcast());
    filterConfigCurrentForMe.setTarget(getIdentifier());
    filterConfigCurrentForMe.setMessageSchema("config");
    filterConfigCurrentForMe.setMessageClass("current");
    filters().append(filterConfigCurrentForMe);
}

void Zigbus_xPLService::xPLMessageProcess(QxPLMessage message)
{
    if(message.getTarget().getVendorId() == "zigbus" && message.getTarget().getDeviceId() == "module") {
        emit moduleTargetedMessage(message);
    }
    else {
        processMessageForGateway(message);
    }
}

void Zigbus_xPLService::processMessageForGateway(const QxPLMessage &message)
{
    Q_UNUSED(message)
}


