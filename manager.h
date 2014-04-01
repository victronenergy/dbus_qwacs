#ifndef MANAGER_H
#define MANAGER_H

#include <QStringList>
#include "manager_adaptor.h"
#include "busitem_prod.h"

class Manager : public QObject
{
	Q_OBJECT

public:
	Manager(QObject * parent = 0);
	~Manager();

	void setGatewayConnected(bool connected) { mGatewayConnected = connected; }
	void setHostname(const QString & hostname) { mHostname = hostname; }

signals:
	void gatewayFound(const QString &hostname);
	void sensorAdded(const QString &id);

public slots:
	bool getGatewayConnected() { return mGatewayConnected; }
	QString getHostname() { return mHostname; }
	QStringList getSensors() { return sensorList; }

private slots:
	void addSensor(const QString &id) { sensorList << id; }

private:
	QString mHostname;
	bool mGatewayConnected;
	QStringList sensorList;
	ManagerAdaptor* mAdaptor;
};

#endif // MANAGER_H
