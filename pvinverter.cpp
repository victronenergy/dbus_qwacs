#include "pvinverter.h"
#include "defines.h"
#include "version.h"
#include "QsLog.h"

const QString productNameText = "PV Inverter";
const quint32 productIdValue = 0xA140;
const QString productIdText = "0x"+QString::number(productIdValue,16);
const QString connectionText =" Wireless AC sensors";
const QString processName = "dbus_qwacs";
const QString processVersion = QString(VERSION);

PVinverter::PVinverter(const QString &service, QObject *parent) :
	QObject(parent),
	mDBus(service)
{
	QLOG_INFO()  << "[PVinverter] Add service: " << service;

	//mNumberOfPhases = 0;
	// This looks a bit odd (with the above init constructor) but I cannot see
	// another way to get a new connection which is not related to the applciation
	mDBus = mDBus.connectToBus(DBUS_TYPE, service);
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
	//mBusItemMap.insert(Voltage, new BusItemProd);
	mBusItemMap.insert(Current, new BusItemProd);
	mBusItemMap.insert(Power, new BusItemProd);
	mBusItemMap.insert(EnergyForward, new BusItemProd);
	mBusItemMap.insert(EnergyReverse, new BusItemProd);
	mBusItemMap.insert(NumberOfPhases, new BusItemProd);
	mBusItemMap.insert(Position, new BusItemProd);
	mBusItemMap.insert(Version, new BusItemProd);
	mBusItemMap.insert(Name, new BusItemProd);
	mBusItemMap.insert(Connection, new BusItemProd);
	mBusItemMap.insert(ProductName, new BusItemProd);
	mBusItemMap.insert(ProductId, new BusItemProd);
	mBusItemMap.insert(Connected, new BusItemProd);
	mBusItemMap.insert(FirmwareVersion, new BusItemProd);


	for (int i = 0; i < L3; i++) {
		//mVoltage.append(0);
		mCurrent.append(0.0d);
		mPower.append(0.0d);
		mEnergyForward.append(0.0d);
		mEnergyReverse.append(0.0d);
	}
}

PVinverter::~PVinverter()
{
	QMap<Items, BusItemProd *>::const_iterator i = mBusItemMap.constBegin();
	while (i != mBusItemMap.constEnd()) {
		delete i.value();
		++i;
	}
}

void PVinverter::registerConnection(const Connections pos, const QString version)
{
	switch (pos)
	{
	case ACIn1:
		mBusItemMap[Position]->setValue(0);
		mBusItemMap[Position]->setText("AC Input 1");
		mBusItemMap[ProductName]->setValue(productNameText+" on AC In1");
		mBusItemMap[ProductName]->setText(productNameText+" on AC In1");
		break;
	case ACOut:
		mBusItemMap[Position]->setValue(1);
		mBusItemMap[Position]->setText("AC Output");
		mBusItemMap[ProductName]->setValue(productNameText+" on AC Out");
		mBusItemMap[ProductName]->setText(productNameText+" on AC Out");
		break;
	case ACIn2:
		mBusItemMap[Position]->setValue(2);
		mBusItemMap[Position]->setText("AC Input 2");
		mBusItemMap[ProductName]->setValue(productNameText+" on AC In2");
		mBusItemMap[ProductName]->setText(productNameText+" on AC In2");
		break;
	default:
		QLOG_ERROR() << "[PVinverter::registerPosition] Unknown position: " << pos;
		break;
	}
	mDBus.registerObject("/Position", mBusItemMap[Position]);
	mDBus.registerObject("/DeviceInstance", mBusItemMap[Position]);
	mDBus.registerObject("/ProductName", mBusItemMap[ProductName]);
	QLOG_INFO()  << "[PVinverter] Register position: " << mBusItemMap[Position]->getText();

	mBusItemMap[Version]->setValue(QString(VERSION));
	mBusItemMap[Version]->setText(QString(VERSION));
	mDBus.registerObject("/Mgmt/ProcessVersion", mBusItemMap[Version]);
	mBusItemMap[Name]->setValue(processName);
	mBusItemMap[Name]->setText(processName);
	mDBus.registerObject("/Mgmt/ProcessName", mBusItemMap[Name]);
	mBusItemMap[Connection]->setValue(connectionText);
	mBusItemMap[Connection]->setText(connectionText);
	mDBus.registerObject("/Mgmt/Connection", mBusItemMap[Connection]);

	mBusItemMap[ProductId]->setValue(productIdValue);
	mBusItemMap[ProductId]->setText(productIdText);
	mDBus.registerObject("/ProductId", mBusItemMap[ProductId]);
	setConnected(true);
	mDBus.registerObject("/Connected", mBusItemMap[Connected]);
	setFirmwareVersion(version);
	mDBus.registerObject("/FirmwareVersion", mBusItemMap[FirmwareVersion]);

	//mBusItemMap[Name]->setValue(0);
	//mBusItemMap[Name]->setText("0");
	//mDBus.registerObject("/NumberOfPhases", mBusItemMap[NumberOfPhases]);

	// Only if NumberOfPhases > 0, so replace tis part
	mDBus.registerObject("/Ac/Current", mBusItemMap[Current]);
	mDBus.registerObject("/Ac/Power", mBusItemMap[Power]);
	mDBus.registerObject("/Ac/Energy/Forward", mBusItemMap[EnergyForward]);
	mDBus.registerObject("/Ac/Energy/Reverse", mBusItemMap[EnergyReverse]);

	registerPhases();
}

void PVinverter::unregisterConnection(const Connections conn)
{
	Q_UNUSED(conn);

	mDBus.unregisterObject("/Position");
	mDBus.unregisterObject("/NumberOfPhases");
	mDBus.unregisterObject("/Ac", QDBusConnection::UnregisterTree);
}

void PVinverter::registerPhase(const Phases phase)
{
	if (phase == NoPhase) {
		QLOG_ERROR() << "[PVinverter] Unknown phase: " << phase;
		return;
	}

	switch (phase)
	{
	case L1:
		QLOG_INFO()  << "[PVinverter] Register phase L1";
		mDBus.registerObject("/Ac/L1/Voltage", mBusItemMap[L1_Voltage]);
		mDBus.registerObject("/Ac/L1/Current", mBusItemMap[L1_Current]);
		mDBus.registerObject("/Ac/L1/Power", mBusItemMap[L1_Power]);
		mDBus.registerObject("/Ac/L1/Energy/Forward", mBusItemMap[L1_EnergyForward]);
		mDBus.registerObject("/Ac/L1/Energy/Reverse", mBusItemMap[L1_EnergyReverse]);
		break;
	case L2:
		QLOG_INFO()  << "[PVinverter] Register phase L2";
		mDBus.registerObject("/Ac/L2/Voltage", mBusItemMap[L2_Voltage]);
		mDBus.registerObject("/Ac/L2/Current", mBusItemMap[L2_Current]);
		mDBus.registerObject("/Ac/L2/Power", mBusItemMap[L2_Power]);
		mDBus.registerObject("/Ac/L2/Energy/Forward", mBusItemMap[L2_EnergyForward]);
		mDBus.registerObject("/Ac/L2/Energy/Reverse", mBusItemMap[L2_EnergyReverse]);
		break;
	case L3:
		QLOG_INFO()  << "[PVinverter] Register phase L3";
		mDBus.registerObject("/Ac/L3/Voltage", mBusItemMap[L3_Voltage]);
		mDBus.registerObject("/Ac/L3/Current", mBusItemMap[L3_Current]);
		mDBus.registerObject("/Ac/L3/Power", mBusItemMap[L3_Power]);
		mDBus.registerObject("/Ac/L3/Energy/Forward", mBusItemMap[L3_EnergyForward]);
		mDBus.registerObject("/Ac/L3/Energy/Reverse", mBusItemMap[L3_EnergyReverse]);
		break;
	default:
		return;
	}
}

void PVinverter::registerPhases()
{
	registerPhase(L1);
	registerPhase(L2);
	registerPhase(L3);
}

void PVinverter::unregisterPhase(const Phases phase)
{
	if (phase == NoPhase) {
		QLOG_ERROR() << "[PVinverter::unregisterPhase] Unknown phase: " << phase;
		return;
	} else
		QLOG_INFO()  << "[PVinverter::unregisterPhase()] phase = " << phase;

	switch (phase)
	{
	case L1:
		mDBus.unregisterObject("/Ac/L1", QDBusConnection::UnregisterTree);
		break;
	case L2:
		mDBus.unregisterObject("/Ac/L2", QDBusConnection::UnregisterTree);
		break;
	case L3:
		mDBus.unregisterObject("/Ac/L3", QDBusConnection::UnregisterTree);
		break;
	default:
		return;
	}
	const int index = phase-1;
	mCurrent[index] = 0;
	mPower[index] = 0;
	mEnergyForward[index] = 0;
	mEnergyReverse[index] = 0;
}

void PVinverter::invalidatePhase(const Phases phase)
{
	if (phase == NoPhase) {
		QLOG_ERROR() << "[PVinverter::invalidatePhase] Unknown phase: " << phase;
		return;
	} else
		QLOG_INFO()  << "[PVinverter::invalidatePhase()] phase = " << phase;

	switch (phase)
	{
	case L1:
		mBusItemMap[L1_Voltage]->invalidate();
		mBusItemMap[L1_Current]->invalidate();
		mBusItemMap[L1_Power]->invalidate();
		mBusItemMap[L1_EnergyForward]->invalidate();
		mBusItemMap[L1_EnergyReverse]->invalidate();
		break;
	case L2:
		mBusItemMap[L2_Voltage]->invalidate();
		mBusItemMap[L2_Current]->invalidate();
		mBusItemMap[L2_Power]->invalidate();
		mBusItemMap[L2_EnergyForward]->invalidate();
		mBusItemMap[L2_EnergyReverse]->invalidate();
		break;
	case L3:
		mBusItemMap[L3_Voltage]->invalidate();
		mBusItemMap[L3_Current]->invalidate();
		mBusItemMap[L3_Power]->invalidate();
		mBusItemMap[L3_EnergyForward]->invalidate();
		mBusItemMap[L3_EnergyReverse]->invalidate();
		break;
	default:
		return;
	}
	/* Remove from total calculations */
	const int index = phase-1;
	mCurrent[index] = 0;
	mPower[index] = 0;
	mEnergyForward[index] = 0;
	mEnergyReverse[index] = 0;
}

void PVinverter::invalidateTotals()
{
	mBusItemMap[Current]->invalidate();
	mBusItemMap[Power]->invalidate();
	mBusItemMap[EnergyForward]->invalidate();
	mBusItemMap[EnergyReverse]->invalidate();
}

void PVinverter::setConnected(const bool connected)
{
	if (!connected) {
		invalidatePhase(L1);
		invalidatePhase(L2);
		invalidatePhase(L3);
		invalidateTotals();
	}
	mBusItemMap[Connected]->setValue((int)connected);
	mBusItemMap[Connected]->setText(QString((int)connected));
	mBusItemMap[Connected]->propertiesUpdated();
}

void PVinverter::setFirmwareVersion(const QString version)
{
	mBusItemMap[FirmwareVersion]->setValue("Gateway v"+version);
	mBusItemMap[FirmwareVersion]->setText("Gateway v"+version);
	mBusItemMap[FirmwareVersion]->propertiesUpdated();
}

void PVinverter::setVoltage(const Phases phase, const double value)
{
	if (phase == NoPhase) {
		QLOG_ERROR() << "[PVinverter::setVoltage] Unknown phase: " << phase;
		return;
	}

	QString text(QString::number(value,'f',0)+"V");
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
		return;
	}
}

void PVinverter::setCurrent(const Phases phase, const double value)
{
	if (phase == NoPhase) {
		QLOG_ERROR() << "[PVinverter::setCurrent] Unknown phase: " << phase;
		return;
	}

	QString text(QString::number(value,'f',1)+"A");
	mCurrent[phase-1] = value;
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
		return;
	}
	double sum = 0;
	const int listSize = mCurrent.size();
	for (int i = 0; i < listSize; ++i)
		sum += mCurrent.at(i);
	mBusItemMap[Current]->setValue(sum);
	text = QString::number(sum,'f',1)+"A";
	mBusItemMap[Current]->setText(text);
	mBusItemMap[Current]->propertiesUpdated();
}

void PVinverter::setPower(const Phases phase, int value)
{
	if (phase == NoPhase) {
		QLOG_ERROR() << "[PVinverter::setPower] Unknown phase: " << phase;
		return;
	}

	if (value < 0) // Set power to 0 if negative
		value = 0;
	mPower[phase-1] = value;
	QString text(QString::number(value,'f',0)+"W");
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
		return;
	}
	int sum = 0;
	const int listSize = mPower.size();
	for (int i = 0; i < listSize; ++i)
		sum += mPower.at(i);
	mBusItemMap[Power]->setValue(sum);
	text = QString::number(sum,'f',0)+"W";
	mBusItemMap[Power]->setText(text);
	mBusItemMap[Power]->propertiesUpdated();
}

void PVinverter::setEnergyForward(const Phases phase, const uint value)
{
	if (phase == NoPhase) {
		QLOG_ERROR() << "[PVinverter::setEnergyForward] Unknown phase: " << phase;
		return;
	}

	/* Convert to kWh */
	double kwhValue = value/1000.0;
	QString text(QString::number(kwhValue,'f',2)+"kWh");
	mEnergyForward[phase-1] = kwhValue;
	switch (phase)
	{
	case L1:
		mBusItemMap[L1_EnergyForward]->setValue(kwhValue);
		mBusItemMap[L1_EnergyForward]->setText(text);
		mBusItemMap[L1_EnergyForward]->propertiesUpdated();
		break;
	case L2:
		mBusItemMap[L2_EnergyForward]->setValue(kwhValue);
		mBusItemMap[L2_EnergyForward]->setText(text);
		mBusItemMap[L2_EnergyForward]->propertiesUpdated();
		break;
	case L3:
		mBusItemMap[L3_EnergyForward]->setValue(kwhValue);
		mBusItemMap[L3_EnergyForward]->setText(text);
		mBusItemMap[L3_EnergyForward]->propertiesUpdated();
		break;
	default:
		return;
	}
	double sum = 0.0d;
	const int listSize = mEnergyForward.size();
	for (int i = 0; i < listSize; ++i)
		sum += mEnergyForward.at(i);
	mBusItemMap[EnergyForward]->setValue(sum);
	text = QString::number(sum,'f',2)+"kWh";
	mBusItemMap[EnergyForward]->setText(text);
	mBusItemMap[EnergyForward]->propertiesUpdated();
}

void PVinverter::setEnergyReverse(const Phases phase, const uint value)
{
	if (phase == NoPhase) {
		QLOG_ERROR() << "[PVinverter::setEnergyForward] Unknown phase: " << phase;
		return;
	}

	/* Convert to kWh */
	double kwhValue = value/1000.0;
	QString text(QString::number(kwhValue,'f',2)+"kWh");
	mEnergyReverse[phase-1] = kwhValue;
	switch (phase)
	{
	case L1:
		mBusItemMap[L1_EnergyReverse]->setValue(kwhValue);
		mBusItemMap[L1_EnergyReverse]->setText(text);
		mBusItemMap[L1_EnergyReverse]->propertiesUpdated();
		break;
	case L2:
		mBusItemMap[L2_EnergyReverse]->setValue(kwhValue);
		mBusItemMap[L2_EnergyReverse]->setText(text);
		mBusItemMap[L2_EnergyReverse]->propertiesUpdated();
		break;
	case L3:
		mBusItemMap[L3_EnergyReverse]->setValue(kwhValue);
		mBusItemMap[L3_EnergyReverse]->setText(text);
		mBusItemMap[L3_EnergyReverse]->propertiesUpdated();
		break;
	default:
		return;
	}
	double sum = 0.0d;
	const int listSize = mEnergyReverse.size();
	for (int i = 0; i < listSize; ++i)
		sum += mEnergyReverse.at(i);
	mBusItemMap[EnergyReverse]->setValue(sum);
	text = QString::number(sum,'f',2)+"kWh";
	mBusItemMap[EnergyReverse]->setText(text);
	mBusItemMap[EnergyReverse]->propertiesUpdated();
}

void PVinverter::setNumberOfPhases(const uint value)
{
	mBusItemMap[NumberOfPhases]->setValue(value);
	mBusItemMap[NumberOfPhases]->propertiesUpdated();
}
