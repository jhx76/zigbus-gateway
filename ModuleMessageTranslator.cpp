#include "ModuleMessageTranslator.h"

ModuleMessageTranslator::ModuleMessageTranslator(QObject *parent)
    : QObject(parent)
{

}

void ModuleMessageTranslator::processMessage(QxPLMessage message)
{
    if(message.getMessageSchema() == "config" && message.getMessageClass() == "response"
            && message.getType() == QxPLMessage::COMMAND)
    {
        xPL_config_response_cmd(message);
    }
    else if(message.getMessageSchema() == "control" && message.getMessageClass() == "basic"
            && message.getType() == QxPLMessage::COMMAND)
    {
        xPL_basic_control_cmd(message);
    }
}

void ModuleMessageTranslator::processMessage(ZigbusMessage message)
{
    Q_UNUSED(message)
}

void ModuleMessageTranslator::xPL_config_response_cmd(const QxPLMessage &message)
{
    /*
        config.response
        {
        newconf=[new instance id]
        [interval=[value]]
        [group=[value]]
        [filter=[value]]
        [developer defined parameters...=[value]]
        ...
        }
      */
    bool conversionOK = false;
    int hbInterval = message.getNamedValue("interval").toInt(&conversionOK);
    if(!conversionOK) {
        qDebug() << "[SKIP] unreconized interval";
        return;
    }

    QString reconf = message.getNamedValue("newconf");
    if(!conversionOK) {
        qDebug() << "[SKIP] no newconf value in the config msg";
        return;
    }
    QStringList groups;
    for(int i = 0; i < message.count("group"); i++) {
        groups << message.getNamedValue("group", i);
    }
    QStringList filters;
    for(int i = 0; i < message.count("filter"); i++) {
        filters << message.getNamedValue("filter", i);
    }
}

void ModuleMessageTranslator::xPL_basic_control_cmd(const QxPLMessage &message)
{
    if(message.getTarget().getVendorId() != "zigbus" || message.getTarget().getDeviceId() != "module") {
        qDebug() << "[SKIP] control.basic cmd not for zigbus module: " << message.getTarget().toString();
        return;
    }

    bool conversionOK = false;
    QString device = message.getNamedValue("device");
    QString function = xPL4Qt::function_name(device);
    QStringList args = xPL4Qt::function_arg_list(device);
    if(function.isEmpty()) {
        qDebug() << "[SKIP] unreconized function : " << device;
        return;
    }
    QString type = message.getNamedValue("type");
    QString current = message.getNamedValue("current");
    if(function == "numeric_output" && type == "output") {
        if(current != "on" && current != "off" && current != "toggle" && current != "enable" && current != "disable") {
            qDebug() << "[SKIP] bad current value "<< current << " for function " << device;
            return;
        }
        if(args.count() < 1) {
            qDebug() << "[SKIP] bad arguments for function " << device;
            return;
        }
        const QString& instanceId = message.getTarget().getInstanceId();
        if(instanceId.contains(":")) {
            qDebug() << "[SKIP] Cannot parse instanceId " << message.getTarget().toString();
            return;
        }
        QString strNetwork = instanceId.left(instanceId.length() - instanceId.indexOf(":") - 1);
        int inetwork = strNetwork.toInt(&conversionOK);
        if(!conversionOK) {
            qDebug() << "[SKIP] Cannot parse instanceId " << message.getTarget().toString();
            return;
        }
        ZigbusNetworkId networkId(inetwork);

        QString strModule = instanceId.left(instanceId.indexOf(":"));
        int imodule = strModule.toInt(&conversionOK, 8);
        if(!conversionOK) {
            qDebug() << "[SKIP] Cannot parse instanceId " << message.getTarget().toString();
            return;
        }
        ZigbusModuleId moduleId(imodule);

        ZigbusMessage zb_message(networkId, moduleId, ZigbusOrder(current));
        zb_message.addAddressingType(ZigbusAddressingType::ADDR_DIRECT());

        ZigbusIO io(args.at(0).trimmed());
        zb_message.addIO(io);
        QString strtime = message.getNamedValue("time");
        QString strunit = message.getNamedValue("unit");
        if(!strtime.isEmpty() && !strunit.isEmpty()) {
            int itime = strtime.toInt(&conversionOK);
            if(!conversionOK) {
                qDebug() << "[SKIP] bad time value "+strtime;
                return;
            }
            int iunit;
            if(strunit == "sec") {
                iunit = UNIT_SEC;
            }
            else if(strunit == "dsec") {
                iunit = UNIT_DSEC;
            }
            else {
                qDebug() << "[SKIP] bad time unit " << strunit;
                return;
            }
            zb_message.addTimeItem(ZigbusTimeItem(itime, iunit));
        }
        emit zigbusMessageToSend(zb_message);
    }
}
