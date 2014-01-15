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

	bool getGatewayConnected() { return mAdaptor->getGatewayConnected(); }
	void setGatewayConnected(bool connected) { mAdaptor->setGatewayConnected(connected); }

signals:
	void gatewayFound(const QString &hostname);
	void sensorAdded(const QString &id);

public slots:
	QStringList getSensors();

private slots:
	void addSensor(const QString &id);

private:
	QStringList sensorList;
	ManagerAdaptor* mAdaptor;
	BusItemProd mVersionBusItem;
};

#endif // MANAGER_H
