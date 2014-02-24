#include <QtDBus/QtDBus>

#include "qwacs.h"
#include "defines.h"
#include "types.h"

Qwacs::Qwacs(QObject *parent) :
	QObject(parent),
	mArguments(QCoreApplication::arguments ()),
	mLogLevel("com.victronenergy.settings", "/Settings/System/LogLevel", DBUS_CONNECTION, parent),
	mLogger(QsLogging::Logger::instance()),
	mDBus(DBUS_CONNECTION),
	mSettings(this),
	mManager(this),
	mGateway(this),
	mAddSetting("com.victronenergy.settings", "/Settings", QDBusConnection::sessionBus(), parent),
	json(JSON::instance())
{
	mDBusInstance = 0;
	QVariant reply = mLogLevel.getValue();
	initLogger(reply.isValid() ? (QsLogging::Level)reply.toInt() : QsLogging::TraceLevel);

	mSDDPClient.start();

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
	connect (&mGateway, SIGNAL(sensorFound(Sensor * const)), this, SLOT(sensorFound(Sensor * const)));
	connect (&mGateway, SIGNAL(sensorUpdated(Sensor * const)), this, SLOT(sensorUpdated(Sensor * const)));

	connect (&mSettings, SIGNAL(sensorAddedOnPos(const QString &)), this, SLOT(addSensorToPVinverter(const QString &)));
	connect (&mSettings, SIGNAL(sensRemovedFromPos(const QString &, const Connections)), this, SLOT(removeSensorFromPVinverter(const QString &, const Connections)));

	// Relaying signals to DBus
	mManager.connect (&mGateway, SIGNAL(gatewayFound(const QString &)), SIGNAL(gatewayFound(const QString &)));
	mManager.connect (&mGateway, SIGNAL(sensorFound(const QString &)), SIGNAL(sensorAdded(const QString &)));

	// MMU: testing code
	if (mAddSetting.add("Sensors/OnPosition/ACIn1", "L1", "00.00.00.00.00", "s", 0, 0))
		qDebug() << "ERROR: addSetting";
	//addSetting.add("System/Sensors", "Test2", 10, "i", 3, 20);
}

void Qwacs::initLogger(QsLogging::Level logLevel)
{
	// init the logging mechanism
	QsLogging::DestinationPtr debugDestination(
			QsLogging::DestinationFactory::MakeDebugOutputDestination() );
	mLogger.addDestination(debugDestination);

	// Log header
	QLOG_INFO() << cNAME << " " << cVERSION << " started";
	QLOG_INFO() << "Built with Qt" << QT_VERSION_STR << "running on" << qVersion();
	QLOG_INFO() << cNAME << " built on" << __DATE__ << "at" << __TIME__;
	// Change if necessary
	mLogger.setLoggingLevel(logLevel);
}

void Qwacs::gatewayFound(const QString &hostname)
{
	QLOG_INFO()  << "[Qwacs::gatewayFound()] hostname = " << hostname;
	mManager.setHostname(hostname);
	mDBus.registerObject("/Gateway", &mGateway);
}

void Qwacs::sensorFound(Sensor * const sens)
{
	QString id = sens->getID();
	Connections conn = mSettings.getConnection(id);

	QLOG_INFO()  << "[Qwacs::sensorsFound()] id = " << id;
	mSensorIdList.append(id);
	if (conn != NoConn ) {
		if (!mPVinverterMap.contains(conn)) {
			mPVinverterMap.insert(conn, new PVinverter("com.victronenergy.pvinverter.qwacs_di"+QString::number(mDBusInstance++)));
			mPVinverterMap[conn]->registerConnection(conn);
		}
		addSensorToPVinverter(id);
	}
	id.replace(".","");
	mDBus.registerObject("/Gateway/Sensors/"+id, sens);
}

void Qwacs::addSensorToPVinverter(const QString &id)
{
	Connections conn = mSettings.getConnection(id);

	QLOG_INFO()  << "[Qwacs::addSensorToPVinverter()] id = " << id;
	if (conn != NoConn ) {
		if (mSensorIdList.contains(id)) {
			Phases phase = mSettings.getPhase(id);
			mPVinverterMap[conn]->registerPhase(phase);
		}
		mPVinverterMap[conn]->setNumberOfPhases(mSettings.getNumberOfPhases(conn));
	} else
		QLOG_WARN()  << "[Qwacs::addSensorToPVinverter()] No connection found for sensor with id: " << id;
}

void Qwacs::removeSensorFromPVinverter(const QString &id, const Connections conn)
{
	if (mSensorIdList.contains(id)) {
		QLOG_INFO()  << "[Qwacs::removeSensorFromPVinverter()] id = " << id << " conn: " << conn;
		if (conn != NoConn ) {
			if (mPVinverterMap.contains(conn)) {
				mPVinverterMap[conn]->setNumberOfPhases(mSettings.getNumberOfPhases(conn));
				Phases phase = mSettings.getPhase(id);
				if (phase != NoPhase) {
					mPVinverterMap[conn]->unregisterPhase(phase);
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

	if (mSensorIdList.contains(id)) {
		QLOG_TRACE()  << "[Qwacs::sensorUpdated()] id = " << id;
		Connections conn = mSettings.getConnection(id);
		if (conn != NoConn ) {
			Phases phase = mSettings.getPhase(id);
			//if (phase != NoPhase) {
				//if (mPVinverterMap.contains(conn)) {
					mPVinverterMap[conn]->setVoltage(phase, sens->getVoltage());
					mPVinverterMap[conn]->setCurrent(phase, sens->getCurrent());
					mPVinverterMap[conn]->setPower(phase, sens->getPower());
					mPVinverterMap[conn]->setEnergyForward(phase, sens->getEnergyForward());
					mPVinverterMap[conn]->setEnergyReverse(phase, sens->getEnergyReverse());
				//} else
				//	QLOG_ERROR()  << "[Qwacs::sensorUpdated] Error in position from settings" << conn;
			//}
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
	if (usn == "uuid:tim-001-0122::urn:schemas-quby-nl:service:DectGateway:1") {
		QUrl url(location);
		mGateway.setHostname(url.encodedHost());
		if (!mGateway.getConnected()) {
			QLOG_INFO() << "[ssdpNewDevice] New device found: " << mGateway.getHostname();
			mGateway.setConnected(true);
			mManager.setGatewayConnected(true);
			mGateway.getVersion();
			//mSDDPClient.stop();
		}
	}
}
