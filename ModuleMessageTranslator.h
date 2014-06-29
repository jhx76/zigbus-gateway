#ifndef MESSAGETRANSLATOR_H
#define MESSAGETRANSLATOR_H
#include <QxPLMessage.h>
#include <QxPLUtils.h>

#include <ZigbusMessage.h>

#include <QObject>

class ModuleMessageTranslator : public QObject
{
    Q_OBJECT
public:
    ModuleMessageTranslator(QObject* parent = 0);

    void xPL_config_response_cmd(const QxPLMessage &message);
    void xPL_basic_control_cmd(const QxPLMessage &message);


public slots:
    void processMessage(QxPLMessage message);
    void processMessage(ZigbusMessage message);

signals:
    void zigbusMessageToSend(ZigbusMessage message);
    void xPLMessageToSend(QxPLMessage message);
};

#endif // MESSAGETRANSLATOR_H
