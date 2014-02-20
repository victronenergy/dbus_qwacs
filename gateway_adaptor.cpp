#include "gateway.h"

GatewayAdaptor::GatewayAdaptor(QObject *parent)
	: QDBusAbstractAdaptor(parent)
{
	//setAutoRelaySignals(true);
}

QString GatewayAdaptor::getCommonName()
{
	QString value;
	QMetaObject::invokeMethod(parent(), "getCommonName", Q_RETURN_ARG(QString, value));
	return value;
}

QString GatewayAdaptor::getFirmwareVersion()
{
	QString value;
	QMetaObject::invokeMethod(parent(), "getFirmwareVersion", Q_RETURN_ARG(QString, value));
	return value;
}

QString GatewayAdaptor::getArchFlavVers()
{
	QString value;
	QMetaObject::invokeMethod(parent(), "getArchFlavVers", Q_RETURN_ARG(QString, value));
	return value;
}

QString GatewayAdaptor::getSerialNr()
{
	QString value;
	QMetaObject::invokeMethod(parent(), "getSerialNr", Q_RETURN_ARG(QString, value));
	return value;
}

QString GatewayAdaptor::getPartNr()
{
	QString value;
	QMetaObject::invokeMethod(parent(), "getPartNr", Q_RETURN_ARG(QString, value));
	return value;
}

uint GatewayAdaptor::getUpdays()
{
	uint value;
	QMetaObject::invokeMethod(parent(), "getUpdays", Q_RETURN_ARG(uint, value));
	return value;
}

uint GatewayAdaptor::getUphours()
{
	uint value;
	QMetaObject::invokeMethod(parent(), "getUphours", Q_RETURN_ARG(uint, value));
	return value;
}
