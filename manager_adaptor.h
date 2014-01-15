#ifndef MANAGERADAPTOR_H
#define MANAGERADAPTOR_H

#include <QDBusAbstractAdaptor>
#include <QStringList>
#include "version.h"

class ManagerAdaptor : public QDBusAbstractAdaptor
{
	Q_OBJECT
	Q_CLASSINFO("D-Bus Interface", "com.victronenergy.manager")
	Q_PROPERTY(QString Version READ getVersion)
	Q_PROPERTY(bool GatewayConnected READ getGatewayConnected)

public:
	ManagerAdaptor(QObject * parent = 0);

	QString getVersion() { return VERSION; }
	bool getGatewayConnected() { return mGatewayConnected; }
	void setGatewayConnected(bool connected) { mGatewayConnected = connected; }

signals: // DBus Signals
	void gatewayFound(const QString &hostname);
	void sensorAdded(const QString &id);

public slots: // DBus Methods
	QStringList GetSensors();

private slots:

private:
	bool mGatewayConnected;
};

#endif // MANAGERADAPTOR_H
