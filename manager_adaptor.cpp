#include "manager_adaptor.h"

ManagerAdaptor::ManagerAdaptor(QObject *parent) :
	QDBusAbstractAdaptor(parent)
{
	setAutoRelaySignals(true);
}

QStringList ManagerAdaptor::GetSensors()
{
	QStringList sensors;
	QMetaObject::invokeMethod(parent(), "getSensors", Q_RETURN_ARG(QStringList, sensors));
	return sensors;
}
