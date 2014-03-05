#ifndef QWACS_H
#define QWACS_H

#include <QDBusConnection>
#include "manager.h"
#include "gateway.h"
#include "ssdp/brisassdpclient.h"
#include "busitem_interface.h"
#include "busitem_cons.h"
#include "pvinverter.h"
#include "settings.h"
#include "QsLog.h"

#include "json/json.h"


#include "addsetting.h"

class Qwacs : public QObject
{
	Q_OBJECT
public:
	Qwacs(QObject *parent = 0);
	~Qwacs();

signals:

public slots:

private slots:
	void ssdpNewDevice( const QString &usn, const QString &location,
						const QString &st, const QString &ext,
						const QString &server, const QString &cacheControl);
	void gatewayFound(const QString & hostname);
	void gatewayLost();
	void sensorFound(Sensor * const sens);
	void sensorUpdated(Sensor * const sens);
	void addSensorToPVinverter(const QString &id);
	void removeSensorFromPVinverter(const QString &id, const Connections conn);
private:

	void waitForLocalSettings();
	void initLogger(QsLogging::Level logLevel);
	void updatePVinverterConnection(bool connected);

	int mDBusInstance;
	QStringList mArguments;
	BusItemCons mLogLevel;
	QsLogging::Logger& mLogger;
	BrisaUpnp::BrisaSSDPClient mSDDPClient;
	QDBusConnection mDBus;
	Settings mSettings;
	Manager mManager;
	Gateway mGateway;
	QMap<Connections, PVinverter *> mPVinverterMap;
	QStringList mSensorIdList;
	//AddSetting mAddSetting;
	JSON &json;
};

#endif // QWACS_H
