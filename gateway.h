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
	void getSensor(QString id);
	void getVersion();

public slots:

private slots:
	void httpResult(QString str);
	void sensTimer();
	void gateway(const QString &name, const QString &version);

signals:
	void gatewayFound(const QString &hostname);
	void sensorFound(Sensor * const sens);
	void sensorFound(const QString &id);
	void sensorUpdated(Sensor * const sens);

private:
	enum State {
		IDLE,
		WAIT_FOR_CONNECTION,
		GET_VERSION,
		GET_CHECK,
		GET_SENSORLIST,
		GET_SENSORDATA
	};

	Sensor * addSensor(const QString &id, const JsonObject &result);
	void updateSensor(Sensor * const sens, const JsonObject &result);

	bool mConnected;
	QString mHostname;
	State mState;
	GatewayAdaptor* mAdaptor;
	httpRequest mHTTPConnection;
	QMap<QString, Sensor *> mSensorMap;
	QTimer mSensTimer;
};

#endif // GATEWAY_H
