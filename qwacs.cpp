#include <QtDBus/QtDBus>

#include "qwacs.h"
#include "defines.h"
#include "types.h"
#include "QsLog.h"

Qwacs::Qwacs(QObject *parent, const QString &gatewayAddress) :
	QObject(parent),
	mGatewayAddress(gatewayAddress),
	mArguments(QCoreApplication::arguments ()),
	mDBus(DBUS_CONNECTION),
	mSettings(this),
	mManager(this),
	mGateway(this),
	json(JSON::instance())
{
	connect (&mSDDPClient, SIGNAL(newDeviceEvent(const QString &, const QString &,
												const QString &, const QString &,
												const QString &, const QString &)),
			 this, SLOT(ssdpNewDevice(const QString &, const QString &,
									  const QString &, const QString &,
									  const QString &, const QString &))
			 );

	mDBus.registerService("com.victronenergy.qwacs");
	mDBus.registerObject("/Manager", &mManager);

	connect (&mGateway, SIGNAL(gatewayFound(const QString &)), this, SLOT(gatewayFound(const QString &)));
	connect (&mGateway, SIGNAL(gatewayLost()), this, SLOT(gatewayLost()));
	connect (&mGateway, SIGNAL(sensorFound(Sensor * const)), this, SLOT(sensorFound(Sensor * const)));
	connect (&mGateway, SIGNAL(sensorUpdated(Sensor * const)), this, SLOT(sensorUpdated(Sensor * const)));

	connect (&mSettings, SIGNAL(sensorAddedOnPos(const QString &)), this, SLOT(addSensorToPVinverter(const QString &)));
	connect (&mSettings, SIGNAL(sensRemovedFromPos(const QString &, const Connections)), this, SLOT(removeSensorFromPVinverter(const QString &, const Connections)));

	// Relaying signals to DBus
	mManager.connect (&mGateway, SIGNAL(gatewayFound(const QString &)), SIGNAL(gatewayFound(const QString &)));
	mManager.connect (&mGateway, SIGNAL(sensorFound(const QString &)), SIGNAL(sensorAdded(const QString &)));

	if (gatewayAddress.isEmpty()) {
		mSDDPClient.start();
	} else
		connectGaterway("http://"+gatewayAddress);
}

Qwacs::~Qwacs()
{
	QMap<Connections, PVinverter *>::const_iterator i = mPVinverterMap.constBegin();
	while (i != mPVinverterMap.constEnd()) {
		delete i.value();
		++i;
	}
}

void Qwacs::gatewayFound(const QString &hostname)
{
	QLOG_INFO()  << "[Qwacs] Gateway found: " << hostname;
	mManager.setHostname(hostname);
	mDBus.registerObject("/Gateway", &mGateway);
}

void Qwacs::gatewayLost()
{
	mGateway.setConnected(false);
	mManager.setGatewayConnected(false);
	updatePVinverterConnection(false);
}

void Qwacs::updatePVinverterConnection(bool connected)
{
	QMap<Connections, PVinverter *>::const_iterator i = mPVinverterMap.constBegin();
	while (i != mPVinverterMap.constEnd()) {
		i.value()->setConnected(connected);
		++i;
	}
}


void Qwacs::sensorFound(Sensor * const sens)
{
	QString id = sens->getID();

	QLOG_INFO()  << "[Qwacs] Sensor found: " << id;
	mSensorIdList.append(id);
	addSensorToPVinverter(id);
	id.replace(".","");
	mDBus.registerObject("/Gateway/Sensors/"+id, sens);
}

void Qwacs::addSensorToPVinverter(const QString &id)
{
	Connections conn = mSettings.getConnection(id);

	if (conn != NoConn ) {
		QLOG_INFO()  << "[Qwacs] Add sensor to pv-inverter: " << id;
		if (!mPVinverterMap.contains(conn)) {
			mPVinverterMap.insert(conn, new PVinverter("com.victronenergy.pvinverter.qwacs_di"+QString::number(conn)));
			mPVinverterMap[conn]->registerConnection(conn, mGateway.getFirmwareVersion());
		}
	}
}

void Qwacs::removeSensorFromPVinverter(const QString &id, const Connections conn)
{
	if (mSensorIdList.contains(id)) {
		QLOG_INFO()  << "[Qwacs] Remove sensor from pv-inverter: " << id << " conn: " << conn;
		if (conn != NoConn ) {
			if (mPVinverterMap.contains(conn)) {
				Phases phase = mSettings.getPhase(id);
				if (phase != NoPhase) {
					mPVinverterMap[conn]->invalidatePhase(phase);
					if (mSettings.getNumberOfPhases(conn) == 0) {
						mPVinverterMap[conn]->invalidateTotals();
					}
				} else
					QLOG_WARN()  << "[Qwacs::removeSensorFromPVinverter()] No phase found for sensor with id: " << id;
			} else
				QLOG_WARN()  << "[Qwacs::removeSensorFromPVinverter()] No PV inverter found for sensor with id: " << id;
		} else
			QLOG_WARN()  << "[Qwacs::removeSensorFromPVinverter()] No connection found for sensor with id: " << id;
	}
}

void Qwacs::sensorUpdated(Sensor * const sens)
{
	QString id = sens->getID();

	//QLOG_INFO()  << "[Qwacs::sensorUpdated()] id = " << id;
	if (mSensorIdList.contains(id)) {
		Connections conn = mSettings.getConnection(id);
		//QLOG_INFO()  << "[Qwacs] Sensor connected to " << conn;
		if (conn != NoConn ) {
			Phases phase = mSettings.getPhase(id);
			mPVinverterMap[conn]->setVoltage(phase, sens->getVoltage());
			mPVinverterMap[conn]->setCurrent(phase, sens->getCurrent());
			mPVinverterMap[conn]->setPower(phase, sens->getPower());
			mPVinverterMap[conn]->setEnergyForward(phase, sens->getEnergyForward());
			mPVinverterMap[conn]->setEnergyReverse(phase, sens->getEnergyReverse());
		}
	}
}

void Qwacs::ssdpNewDevice(const QString &usn, const QString &location,
							const QString &st, const QString &ext,
							const QString &server, const QString &cacheControl)
{
	Q_UNUSED(st);
	Q_UNUSED(ext);
	Q_UNUSED(server);
	Q_UNUSED(cacheControl);
	if (!mGateway.getConnected()) {
		QLOG_INFO() << "SSDP Device: " << usn << ", " << location << ", " << st << ", " << ext << ", " << server;
		if (usn.contains("urn:schemas-quby-nl:service:DectGateway:1")) {
			connectGaterway(location);
		}
	}
}

void Qwacs::connectGaterway(const QString &gatewayAddress)
{
	QLOG_INFO() << "[Qwacs] Connect gateway" << gatewayAddress;
	QUrl url(gatewayAddress);
	mGateway.setHostname(url.encodedHost());
	QLOG_INFO() << "[Qwacs] New SSDP device found: " << mGateway.getHostname();
	mGateway.setConnected(true);
	mManager.setGatewayConnected(true);
	updatePVinverterConnection(true);
	mGateway.getVersion();
	//mSDDPClient.stop();
}
