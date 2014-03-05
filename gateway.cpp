#include "gateway.h"
#include "QsLog.h"

Gateway::Gateway(QObject *parent) :
	QObject(parent),
	mAdaptor()
{
	mUpdays = 0;
	mUphours = 0;
	mConnected = false;
	mGotGatewayInfo = false;
	mAdaptor = new GatewayAdaptor(this);
	mAdaptor->connect(this, SIGNAL(PropertiesChanged(const QVariantMap &)), SIGNAL(PropertiesChanged(const QVariantMap &)));
	connect (&mHTTPConnection, SIGNAL(result(QString)), this, SLOT(httpResult(QString)));
	connect(&mBlinkTimer, SIGNAL(timeout()), this, SLOT(blinkTimer()));
	connect(&mSensTimer, SIGNAL(timeout()), this, SLOT(sensTimer()));
}

void Gateway::gateway(const QString &name, const QString &version)
{
	QLOG_TRACE() << "[Gateway] gateway(QString,QString) " << name << " : " << version;
}

void Gateway::getVersion()
{
	if (mConnected) {
		mHTTPConnection.getURL("http://"+mHostname+"/API/version/check");
	}
}

void Gateway::getSensorList()
{
	if (mConnected) {
		mHTTPConnection.getURL("http://"+mHostname+"/API/devices/dect/list");
	}
}

void Gateway::getSensor(const QString & id)
{
	if (mConnected) {
		mHTTPConnection.getURL("http://"+mHostname+"/API/devices/dect/"+id+"/view");
	}
}

void Gateway::blinkOn(const QString & id)
{
	QLOG_INFO() << "[Gateway] Turn blinking on for sensor " << id;
	if (mConnected) {
		/*
		* Blink for 2 seconds.
		* The sensor blinks for 2 second because it cannot be turned off immediately
		* There for we wait 2 second to turn it off. A longer time in not convenient
		*/
		mHTTPConnection.postURL("http://"+mHostname+"/API/devices/dect/"+id+"/blink/2");
		if (!mBlinkTimer.isActive())
			mBlinkTimer.start(2000);
		/*
		 * Senser ID is copied. Because the gui can only be in one menu item it
		 * is sufficient to remember only the latest
		 */
		mBlinkSensorID = id;
	}
}

void Gateway::blinkOff(const QString & id)
{
	QLOG_INFO() << "[Gateway] Turn blinking off for sensor " << id;
	mBlinkTimer.stop();
}


void Gateway::registrationMode(const bool on)
{
	QLOG_INFO() << "[Gateway] registrationMode() on = " << on;
	if (mConnected) {
		mHTTPConnection.getURL("http://"+mHostname+"/API/devices/dect/regmode/"+QString(on ? "1" : "0"));
	}
}

void Gateway::getUplink()
{
	QLOG_INFO() << "[Gateway] getUplink()";
	if (mConnected) {
		mHTTPConnection.getURL("http://"+mHostname+"/API/net/uplink");
	}
}

void Gateway::setUplink(const bool on)
{
	QLOG_INFO() << "[Gateway] setUplink() on = " << on;
	if (mConnected) {
		mHTTPConnection.postURL("http://"+mHostname+"/API/net/uplink/"+QString(on ? "1" : "0"));
	}
}

Sensor * Gateway::addSensor(const QString &id, const QVariantMap &result)
{
	Sensor * sens = new Sensor(this);

	QLOG_TRACE() << "[Gateway] Add sensor: " << id;
	mSensorMap.insert(id, sens);
	updateSensor(sens, result);
	emit sensorFound(id);
	return sens;
}

void Gateway::updateSensor(Sensor * const sens, const QVariantMap &result)
{
	QVariantMap electricity;

	if (sens) {
		bool connected = result["connected"].toBool();
		sens->setConnected(connected);
		if (connected) {
			sens->setID(result["id"].toString());
			sens->setType(result["type"].toString());
			sens->setName(result["name"].toString());
			sens->setVersion(result["version"].toString());
			electricity = result["electricity"].toMap();
			sens->setVoltage(electricity["Vrms"].toDouble());
			sens->setCurrent(electricity["Irms"].toDouble());
			sens->setPower(electricity["power"].toInt());
			sens->setEnergyForward(electricity["energyForward"].toUInt());
			sens->setEnergyReverse(electricity["energyReverse"].toUInt());
		}
	}
}

void Gateway::setUpdays(const uint value)
{
	if (mUpdays == value)
		return;

	mChanges.insert("updays", value);
	mUpdays = value;
}

void Gateway::setUphours(const uint value)
{
	if (mUphours == value)
		return;

	mChanges.insert("uphours", value);
	mUphours = value;
}

void Gateway::propertiesUpdated()
{
	if (!mChanges.empty()) {
		emit PropertiesChanged(mChanges);
		mChanges.clear();
	}
}

void Gateway::httpResult(QString str)
{
	if (str.startsWith("NetworkError")) {
		QLOG_ERROR() << "[Gateway] " << str;
		if (mConnected) {
			mSensTimer.stop();
			mGotGatewayInfo = false;
			emit gatewayLost();
		}
		return;
	}

	QVariant reply = JSON::instance().parse(str);
	if (reply.type() == QVariant::Map) {
		QVariantMap result = JSON::instance().parse(str).toMap();
		if (result["result"].isValid()) {
			// Simply reply, check of ok else error message
			if (result["result"].toString() != "ok") {
				QLOG_ERROR() << "[Gateway] Reply not ok. Message: " << result["comment"].toString();
			}
		} else if (result["uplink"].isValid()) {
			// Uplink status reply
			QLOG_TRACE() << "[Gateway] Uplink status reply" << endl << str;
			mUplinkStatus = result["uplink"].toString();
			emit UplinkStatus(mUplinkStatus);
		} else if (result["commonName"].isValid()) {
			// Version reply
			setCommonName(result["commonName"].toString());
			setFirmwareVersion(result["firmwareVersion"].toString());
			setArchFlavVers(result["archFlavVers"].toString());
			setSerialNr(result["serialNr"].toString());
			setPartNr(result["partNr"].toString());
			setUpdays(result["updays"].toUInt());
			setUphours(result["uphours"].toUInt());
			propertiesUpdated();
			if (!mGotGatewayInfo) {
				emit gatewayFound(mHostname);
				mGotGatewayInfo = true;
				mSensTimer.start(2500);
			}
		}
	} else if (reply.type() == QVariant::List) {
		// Because the reply type is list the result string is the sensor list
		QList<QVariant> resultList = JSON::instance().parse(str).toList();
		for (int i = 0; i < resultList.size(); ++i) {
			QVariantMap result = resultList[i].toMap();
			bool connected = result["connected"].toBool();
			if (connected) {
				QString id = result["id"].toString();
				if (!mSensorMap.contains(id))
					emit sensorFound(addSensor(id, result));
				else
					updateSensor(mSensorMap[id], result);
				emit sensorUpdated(mSensorMap[id]);
			}
		}
	} else {
		QLOG_ERROR() << "[Gateway] JSON string of unkown type " << reply.type();
	}
}

void Gateway::blinkTimer()
{
	blinkOn(mBlinkSensorID);
}

void Gateway::sensTimer()
{
	getVersion();
	getSensorList();
}
