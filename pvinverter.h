#ifndef PVINVERTER_H
#define PVINVERTER_H

#include <QObject>
#include <QDBusConnection>
#include "busitem_prod.h"
#include "types.h"

class PVinverter : public QObject
{
	Q_OBJECT
public:
	PVinverter(const QString &service, QObject *parent = 0);

	void registerConnection(const Connections conn);
	void registerPhase(const Phases phase);
	void unregisterConnection(const Connections conn);
	void unregisterPhase(const Phases phase);

	void setVoltage(const Phases phase, const qreal value);
	void setCurrent(const Phases phase, const qreal value);
	void setPower(const Phases phase, const int value);
	void setEnergyForward(const Phases phase, const uint value);
	void setEnergyReverse(const Phases phase, const uint value);
	void setNumberOfPhases(const uint value);
public slots:

signals:

private:
	enum Items {
		L1_Voltage, L1_Current, L1_Power, L1_EnergyForward, L1_EnergyReverse,
		L2_Voltage, L2_Current, L2_Power, L2_EnergyForward, L2_EnergyReverse,
		L3_Voltage, L3_Current, L3_Power, L3_EnergyForward, L3_EnergyReverse,
		Voltage, Current, Power, EnergyForward, EnergyReverse,
		NumberOfPhases, Position, Version, Name
	};

	QDBusConnection mDBus;
	QMap<Items, BusItemProd *> mBusItemMap;

	//QList<qreal> mVoltage;
	QList<qreal> mCurrent;
	QList<int> mPower;
	QList<uint> mEnergyForward;
	QList<uint> mEnergyReverse;
};

#endif // PVINVERTER_H
