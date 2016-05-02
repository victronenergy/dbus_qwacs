#ifndef MANAGERADAPTOR_H
#define MANAGERADAPTOR_H

#include <QDBusAbstractAdaptor>
#include <QStringList>

class ManagerAdaptor : public QDBusAbstractAdaptor
{
	Q_OBJECT
	Q_CLASSINFO("D-Bus Interface", "com.victronenergy.manager")
	Q_PROPERTY(QString Version READ getVersion)
	Q_PROPERTY(bool GatewayConnected READ getGatewayConnected)
	Q_PROPERTY(QString Hostname READ getHostname)
	Q_PROPERTY(QStringList SensorList READ getSensors)

public:
	ManagerAdaptor(QObject * parent = 0);

	QString getVersion() { return VERSION; }
	bool getGatewayConnected();
	QString getHostname();
	QStringList getSensors();

signals: // DBus Signals
	void gatewayFound(const QString &hostname);
	void sensorAdded(const QString &id);

public slots: // DBus Methods

private slots:

private:

};

#endif // MANAGERADAPTOR_H
