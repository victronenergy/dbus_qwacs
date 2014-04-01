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

Manager::~Manager()
{
	delete mAdaptor;
}
