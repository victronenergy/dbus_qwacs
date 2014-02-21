#include "manager_adaptor.h"

ManagerAdaptor::ManagerAdaptor(QObject *parent) :
	QDBusAbstractAdaptor(parent)
{
	setAutoRelaySignals(true);
}

bool ManagerAdaptor::getGatewayConnected()
{
	bool value;
	QMetaObject::invokeMethod(parent(), "getGatewayConnected", Q_RETURN_ARG(bool, value));
	return value;
}

QString ManagerAdaptor::getHostname()
{
	QString value;
	QMetaObject::invokeMethod(parent(), "getHostname", Q_RETURN_ARG(QString, value));
	return value;
}

QStringList ManagerAdaptor::getSensors()
{
	QStringList value;
	QMetaObject::invokeMethod(parent(), "getSensors", Q_RETURN_ARG(QStringList, value));
	return value;
}
