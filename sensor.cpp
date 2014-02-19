#include "sensor.h"
#include "QsLog.h"

Sensor::Sensor(QObject *parent) :
	QObject(parent),
	mAdaptor()
{
	mConnected = false;
	mVrms = 0.0;
	mIrms = 0.0;
	mPower = 0;
	mEnergyForward = 0;
	mEnergyReverse = 0;

	mAdaptor = new SensorAdaptor(this);
	mAdaptor->connect(this, SIGNAL(propertiesChanged()), SIGNAL(PropertiesChanged()));
}
