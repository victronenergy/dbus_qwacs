#ifndef SENSORSET_H
#define SENSORSET_H

#include <QStringList>

class sensorset
{
public:
	sensorset();

	qreal getVoltage();
	qreal getCurrent();
	int getPower();
	uint getEnergyForward();
	uint getEnergyReverse();

private:
	QList<qreal> mVoltageMap;
	QList<qreal> mCurrentMap;
	QList<int> mPowerMap;
	QList<uint> mEnergyForwardMap;
	QList<uint> mEnergyReverseMap;
};

#endif // SENSORSET_H
