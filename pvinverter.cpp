#include "pvinverter.h"
#include "defines.h"
#include "QsLog.h"

PVinverter::PVinverter(const QString &service, QObject *parent) :
	QObject(parent),
	mDBus(service)
{
	QLOG_INFO()  << "[PVinverter::PVinverter()] service = " << service;
	// This looks a bit odd (with the above init constructor) but I cannot see
	// another way to get a new connection which is not related to the applciation
	mDBus = mDBus.connectToBus(QDBusConnection::SessionBus, service);
	mDBus.registerService(service);

	mBusItemMap.insert(L1_Voltage, new BusItemProd);
	mBusItemMap.insert(L1_Current, new BusItemProd);
	mBusItemMap.insert(L1_Power, new BusItemProd);
	mBusItemMap.insert(L1_EnergyForward, new BusItemProd);
	mBusItemMap.insert(L1_EnergyReverse, new BusItemProd);
	mBusItemMap.insert(L2_Voltage, new BusItemProd);
	mBusItemMap.insert(L2_Current, new BusItemProd);
	mBusItemMap.insert(L2_Power, new BusItemProd);
	mBusItemMap.insert(L2_EnergyForward, new BusItemProd);
	mBusItemMap.insert(L2_EnergyReverse, new BusItemProd);
	mBusItemMap.insert(L3_Voltage, new BusItemProd);
	mBusItemMap.insert(L3_Current, new BusItemProd);
	mBusItemMap.insert(L3_Power, new BusItemProd);
	mBusItemMap.insert(L3_EnergyForward, new BusItemProd);
	mBusItemMap.insert(L3_EnergyReverse, new BusItemProd);
	mBusItemMap.insert(NumberOfPhases, new BusItemProd);
	mBusItemMap.insert(Position, new BusItemProd);
}

void PVinverter::registerConnection(const Connections pos)
{
	QLOG_INFO()  << "[PVinverter::registerPosition()] position = " << pos;
	switch (pos)
	{
	case ACIn1:
		mBusItemMap[Position]->setValue(0);
		mBusItemMap[Position]->setText("AC Input 1");
		break;
	case ACOut:
		mBusItemMap[Position]->setValue(1);
		mBusItemMap[Position]->setText("AC Output");
		break;
	case ACIn2:
		mBusItemMap[Position]->setValue(2);
		mBusItemMap[Position]->setText("AC Input 2");
		break;
	default:
		QLOG_ERROR() << "[PVinverter::registerPosition] Unknown position: " << pos;
		break;
	}
	mDBus.registerObject("/Position", mBusItemMap[Position]);
}

void PVinverter::unregisterConnection(const Connections conn)
{
	Q_UNUSED(conn);

	mDBus.unregisterObject("/Position");
}

void PVinverter::registerPhase(const Phases phase)
{
	QLOG_INFO()  << "[PVinverter::registerPhase()] phase = " << phase;
	switch (phase)
	{
	case L1:
		mDBus.registerObject("/AC/L1/Voltage", mBusItemMap[L1_Voltage]);
		mDBus.registerObject("/AC/L1/Current", mBusItemMap[L1_Current]);
		mDBus.registerObject("/AC/L1/Power", mBusItemMap[L1_Power]);
		mDBus.registerObject("/AC/L1/Energy/Forward", mBusItemMap[L1_EnergyForward]);
		mDBus.registerObject("/AC/L1/Energy/Reverse", mBusItemMap[L1_EnergyReverse]);
		break;
	case L2:
		mDBus.registerObject("/AC/L2/Voltage", mBusItemMap[L2_Voltage]);
		mDBus.registerObject("/AC/L2/Current", mBusItemMap[L2_Current]);
		mDBus.registerObject("/AC/L2/Power", mBusItemMap[L2_Power]);
		mDBus.registerObject("/AC/L2/Energy/Forward", mBusItemMap[L2_EnergyForward]);
		mDBus.registerObject("/AC/L2/Energy/Reverse", mBusItemMap[L2_EnergyReverse]);
		break;
	case L3:
		mDBus.registerObject("/AC/L3/Voltage", mBusItemMap[L3_Voltage]);
		mDBus.registerObject("/AC/L3/Current", mBusItemMap[L3_Current]);
		mDBus.registerObject("/AC/L3/Power", mBusItemMap[L3_Power]);
		mDBus.registerObject("/AC/L3/Energy/Forward", mBusItemMap[L3_EnergyForward]);
		mDBus.registerObject("/AC/L3/Energy/Reverse", mBusItemMap[L3_EnergyReverse]);
		break;
	default:
		QLOG_ERROR() << "[PVinverter::registerPhase] Unknown phase: " << phase;
		break;
	}
}

void PVinverter::unregisterPhase(const Phases phase)
{
	QLOG_INFO()  << "[PVinverter::unregisterPhase()] phase = " << phase;
	switch (phase)
	{
	case L1:
		mDBus.unregisterObject("/AC/L1", QDBusConnection::UnregisterTree);
		break;
	case L2:
		mDBus.unregisterObject("/AC/L2", QDBusConnection::UnregisterTree);
		break;
	case L3:
		mDBus.unregisterObject("/AC/L3", QDBusConnection::UnregisterTree);
		break;
	default:
		QLOG_ERROR() << "[PVinverter::unregisterPhase] Unknown phase: " << phase;
		break;
	}
}

void PVinverter::setVoltage(const Phases phase, const qreal value)
{
	QString text(QString::number(value,'f',0)+" V");

	switch (phase)
	{
	case L1:
		mBusItemMap[L1_Voltage]->setValue(value);
		mBusItemMap[L1_Voltage]->setText(text);
		mBusItemMap[L1_Voltage]->propertiesUpdated();
		break;
	case L2:
		mBusItemMap[L2_Voltage]->setValue(value);
		mBusItemMap[L2_Voltage]->setText(text);
		mBusItemMap[L2_Voltage]->propertiesUpdated();
		break;
	case L3:
		mBusItemMap[L3_Voltage]->setValue(value);
		mBusItemMap[L3_Voltage]->setText(text);
		mBusItemMap[L3_Voltage]->propertiesUpdated();
		break;
	default:
		QLOG_ERROR() << "[PVinverter::setVoltage] Unknown phase: " << phase;
		return;
	}
}

void PVinverter::setCurrent(const Phases phase, const qreal value)
{
	QString text(QString::number(value,'f',1)+" A");

	switch (phase)
	{
	case L1:
		mBusItemMap[L1_Current]->setValue(value);
		mBusItemMap[L1_Current]->setText(text);
		mBusItemMap[L1_Current]->propertiesUpdated();
		break;
	case L2:
		mBusItemMap[L2_Current]->setValue(value);
		mBusItemMap[L2_Current]->setText(text);
		mBusItemMap[L2_Current]->propertiesUpdated();
		break;
	case L3:
		mBusItemMap[L3_Current]->setValue(value);
		mBusItemMap[L3_Current]->setText(text);
		mBusItemMap[L3_Current]->propertiesUpdated();
		break;
	default:
		QLOG_ERROR() << "[PVinverter::setCurrent] Unknown phase: " << phase;
		return;
	}
}

void PVinverter::setPower(const Phases phase, const uint value)
{
	QString text(QString::number(value,'f',0)+" W");

	switch (phase)
	{
	case L1:
		mBusItemMap[L1_Power]->setValue(value);
		mBusItemMap[L1_Power]->setText(text);
		mBusItemMap[L1_Power]->propertiesUpdated();
		break;
	case L2:
		mBusItemMap[L2_Power]->setValue(value);
		mBusItemMap[L2_Power]->setText(text);
		mBusItemMap[L2_Power]->propertiesUpdated();
		break;
	case L3:
		mBusItemMap[L3_Power]->setValue(value);
		mBusItemMap[L3_Power]->setText(text);
		mBusItemMap[L3_Power]->propertiesUpdated();
		break;
	default:
		QLOG_ERROR() << "[PVinverter::setPower] Unknown phase: " << phase;
		return;
	}
}

void PVinverter::setEnergyForward(const Phases phase, const uint value)
{
	switch (phase)
	{
	case L1:
		mBusItemMap[L1_EnergyForward]->setValue(value);
		break;
	case L2:
		mBusItemMap[L2_EnergyForward]->setValue(value);
		break;
	case L3:
		mBusItemMap[L3_EnergyForward]->setValue(value);
		break;
	default:
		QLOG_ERROR() << "[PVinverter::setEnergyForward] Unknown phase: " << phase;
		break;
	}
}

void PVinverter::setEnergyReverse(const Phases phase, const uint value)
{
	switch (phase)
	{
	case L1:
		mBusItemMap[L1_EnergyReverse]->setValue(value);
		break;
	case L2:
		mBusItemMap[L2_EnergyReverse]->setValue(value);
		break;
	case L3:
		mBusItemMap[L3_EnergyReverse]->setValue(value);
		break;
	default:
		QLOG_ERROR() << "[PVinverter::setEnergyReverse] Unknown phase: " << phase;
		break;
	}
}
