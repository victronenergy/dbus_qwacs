#include "sensor_adaptor.h"

SensorAdaptor::SensorAdaptor(QObject *parent)
	: QDBusAbstractAdaptor(parent)
{
}

QString SensorAdaptor::getID()
{
	QString value;
	QMetaObject::invokeMethod(parent(), "getID", Q_RETURN_ARG(QString, value));
	return value;
}

QString SensorAdaptor::getType()
{
	QString value;
	QMetaObject::invokeMethod(parent(), "getType", Q_RETURN_ARG(QString, value));
	return value;
}

QString SensorAdaptor::getName()
{
	QString value;
	QMetaObject::invokeMethod(parent(), "getName", Q_RETURN_ARG(QString, value));
	return value;
}

bool SensorAdaptor::getConnected()
{
	bool value;
	QMetaObject::invokeMethod(parent(), "getConnected", Q_RETURN_ARG(bool, value));
	return (bool) value;
}

QString SensorAdaptor::getVersion()
{
	QString value;
	QMetaObject::invokeMethod(parent(), "getVersion", Q_RETURN_ARG(QString, value));
	return value;
}

qreal SensorAdaptor::getVoltage()
{
	qreal value;
	QMetaObject::invokeMethod(parent(), "getVoltage", Q_RETURN_ARG(qreal, value));
	return value;
}

qreal SensorAdaptor::getCurrent()
{
	qreal value;
	QMetaObject::invokeMethod(parent(), "getCurrent", Q_RETURN_ARG(qreal, value));
	return value;
}

int SensorAdaptor::getPower()
{
	int value;
	QMetaObject::invokeMethod(parent(), "getPower", Q_RETURN_ARG(int, value));
	return value;
}

uint SensorAdaptor::getEnergyForward()
{
	uint value;
	QMetaObject::invokeMethod(parent(), "getEnergyForward", Q_RETURN_ARG(uint, value));
	return value;
}

uint SensorAdaptor::getEnergyReverse()
{
	uint value;
	QMetaObject::invokeMethod(parent(), "getEnergyReverse", Q_RETURN_ARG(uint, value));
	return value;
}

uint SensorAdaptor::getPhase()
{
	uint value;
	QMetaObject::invokeMethod(parent(), "getPhase", Q_RETURN_ARG(uint, value));
	return value;
}
