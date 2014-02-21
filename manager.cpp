#include "manager.h"
#include "defines.h"
#include "QsLog.h"

Manager::Manager(QObject *parent) :
	QObject(parent),
	mAdaptor(0)
{
	mGatewayConnected = false;
	mAdaptor = new ManagerAdaptor(this);
	connect (this, SIGNAL(sensorAdded(const QString &)), this, SLOT(addSensor(const QString &)));
}

QStringList Manager::getSensors()
{
	QLOG_TRACE() << "[Manager::getSensors] " << sensorList;
	return sensorList;
}

void Manager::addSensor(const QString &id)
{
	QLOG_TRACE() << "[Manager::addSensor] " << id;
	sensorList << id;
}
