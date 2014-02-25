#ifndef SENSORSET_H
#define SENSORSET_H

#include <QStringList>

class sensorset
{
public:
	sensorset();

	double getVoltage();
	double getCurrent();
	int getPower();
	uint getEnergyForward();
	uint getEnergyReverse();

private:
	QList<double> mVoltageMap;
	QList<double> mCurrentMap;
	QList<int> mPowerMap;
	QList<uint> mEnergyForwardMap;
	QList<uint> mEnergyReverseMap;
};

#endif // SENSORSET_H
