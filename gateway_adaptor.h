#ifndef GATEWAYADAPTOR_H
#define GATEWAYADAPTOR_H

#include <QDBusAbstractAdaptor>
#include "sensor.h"

Q_DECLARE_METATYPE(QList<QString>)

class GatewayAdaptor : public QDBusAbstractAdaptor
{
	Q_OBJECT
	Q_CLASSINFO("D-Bus Interface", "com.victronenergy.gateway")
	Q_PROPERTY(QString commonName READ getCommonName)
	Q_PROPERTY(QString firmwareVersion READ getFirmwareVersion)
	Q_PROPERTY(QString archFlavVers READ getArchFlavVers)
	Q_PROPERTY(QString serialNr READ getSerialNr)
	Q_PROPERTY(QString partNr READ getPartNr)
	Q_PROPERTY(uint updays READ getUpdays)
	Q_PROPERTY(uint uphours READ getUphours)

public:
	GatewayAdaptor(QObject *parent = 0);

	QString getCommonName();
	QString getFirmwareVersion();
	QString getArchFlavVers();
	QString getSerialNr();
	QString getPartNr();
	uint getUpdays();
	uint getUphours();

public slots: // DBus methods
	void BlinkSensor(const QString & id, const int seconds);
	void RegistrationMode(bool on);
	void GetUplink();
	void SetUplink(bool enable);

signals: // DBus Signals
	void PropertiesChanged(const QVariantMap &changes);
	void UplinkStatus(const QString & status);

private:

};

#endif // GATEWAYADAPTOR_H
