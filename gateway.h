#ifndef GATEWAY_H
#define GATEWAY_H

#include <QTimer>
#include "gateway_adaptor.h"
#include "sensor.h"
#include "httprequest.h"
#include "json/json.h"

using QtJson::JsonObject;
using QtJson::JsonArray;

class Gateway : public QObject
{
	Q_OBJECT

public:
	Gateway(QObject *parent = 0);

	bool getConnected() { return mConnected; }
	void setConnected(bool connected) { mConnected = connected; }
	QString getHostname() { return mHostname; }
	void setHostname(QString hostname) { mHostname = hostname; }
	void getSensorList();
	void getSensor(const QString &id);
	void getVersion();

public slots:
	void propertiesUpdated();

	QString getCommonName() const { return mCommonName; }
	QString getFirmwareVersion() const { return mFirmwareVersion; }
	QString getArchFlavVers() const { return mArchFlavVers; }
	QString getSerialNr() const { return mSerialNr; }
	QString getPartNr() const { return mPartNr; }
	uint getUpdays() const { return mUpdays; }
	uint getUphours() const { return mUphours; }

	void blinkSensor(const QString & id, const int seconds);
	void registrationMode(const bool on);
	void getUplink();
	void setUplink(const bool on);

private slots:
	void httpResult(QString str);
	void sensTimer();
	void gateway(const QString &name, const QString &version);

signals:
	void gatewayFound(const QString &hostname);
	void sensorFound(Sensor * const sens);
	void sensorFound(const QString &id);
	void sensorUpdated(Sensor * const sens);
	void PropertiesChanged(const QVariantMap &changes);
	void UplinkStatus(const QString & status);

private:
	enum State {
		IDLE,
		WAIT_FOR_CONNECTION,
		GET_VERSION,
		GET_CHECK,
		GET_SENSORLIST,
		GET_SENSORDATA,
		BLINK_SENSOR,
		GET_UPLINK
	};

	Sensor * addSensor(const QString &id, const JsonObject &result);
	void updateSensor(Sensor * const sens, const JsonObject &result);

	bool mConnected;
	bool mGotGatewayInfo;
	QString mHostname;
	QString mUplinkStatus;
	State mState;
	GatewayAdaptor* mAdaptor;
	httpRequest mHTTPConnection;
	QMap<QString, Sensor *> mSensorMap;
	QTimer mSensTimer;

	// Dbus properties
	void setCommonName(const QString &value) { mCommonName = value; }
	void setFirmwareVersion(const QString &value) { mFirmwareVersion = value; }
	void setArchFlavVers(const QString &value) { mArchFlavVers = value; }
	void setSerialNr(const QString &value) { mSerialNr = value; }
	void setPartNr(const QString &value) { mPartNr = value; }
	void setUpdays(const uint value);
	void setUphours(const uint value);

	QVariantMap mChanges;
	QString mCommonName;
	QString mFirmwareVersion;
	QString mArchFlavVers;
	QString mSerialNr;
	QString mPartNr;
	uint mUpdays;
	uint mUphours;

};

#endif // GATEWAY_H
