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
	mState = WAIT_FOR_CONNECTION;
	mAdaptor = new GatewayAdaptor(this);
	mAdaptor->connect(this, SIGNAL(PropertiesChanged(const QVariantMap &)), SIGNAL(PropertiesChanged(const QVariantMap &)));
	connect (&mHTTPConnection, SIGNAL(result(QString)), this, SLOT(httpResult(QString)));
	connect(&mSensTimer, SIGNAL(timeout()), this, SLOT(sensTimer()));
}

void Gateway::gateway(const QString &name, const QString &version)
{
	QLOG_TRACE() << "[Gateway] gateway(QString,QString) " << name << " : " << version;
}

void Gateway::getVersion()
{
	if (mConnected) {
		mState = GET_VERSION;
		mHTTPConnection.getURL("http://"+mHostname+"/API/version/check");
	}
}

void Gateway::getSensorList()
{
	if (mConnected) {
		mState = GET_SENSORLIST;
		mHTTPConnection.getURL("http://"+mHostname+"/API/devices/dect/list");
	}
}

void Gateway::getSensor(const QString & id)
{
	if (mConnected) {
		mState = GET_SENSORDATA;
		mHTTPConnection.getURL("http://"+mHostname+"/API/devices/dect/"+id+"/view");
	}
}

void Gateway::blinkSensor(const QString & id, const int seconds)
{
	QLOG_INFO() << "[Gateway] blinkSensor() id = " << id << " time : " << seconds;
	if (mConnected) {
		//mState = BLINK_SENSOR;
		mHTTPConnection.getURL("http://"+mHostname+"/API/devices/dect/"+id+"/blink/"+QString::number(seconds));
	}
}

void Gateway::registrationMode(const bool on)
{
	QLOG_INFO() << "[Gateway] registrationMode() on = " << on;
	if (mConnected) {
		//mState = BLINK_SENSOR;
		mHTTPConnection.getURL("http://"+mHostname+"/API/devices/dect/regmode/"+QString(on ? "1" : "0"));
	}
}

void Gateway::getUplink()
{
	QLOG_INFO() << "[Gateway] getUplink()";
	if (mConnected) {
		mState = GET_UPLINK;
		mHTTPConnection.getURL("http://"+mHostname+"/API/net/uplink");
	}
}

void Gateway::setUplink(const bool on)
{
	QLOG_INFO() << "[Gateway] setUplink() on = " << on;
	if (mConnected) {
		//mState = BLINK_SENSOR;
		mHTTPConnection.getURL("http://"+mHostname+"/API/net/uplink/"+QString(on ? "1" : "0"));
	}
}

Sensor * Gateway::addSensor(const QString &id, const JsonObject &result)
{
	Sensor * sens = new Sensor(this);

	QLOG_INFO() << "[Gateway] addSensor() " << id;
	mSensorMap.insert(id, sens);
	updateSensor(sens, result);
	emit sensorFound(id);
	return sens;
}

void Gateway::updateSensor(Sensor * const sens, const JsonObject &result)
{
	JsonObject electricity;

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
	QLOG_TRACE() << "[Gateway::httpResult] " << endl << str;
	str.replace("\n","");

	switch (mState) {
	case IDLE:
		QLOG_INFO() << "[Gateway] State idle no http result expected. Ingoring";
		break;
	case WAIT_FOR_CONNECTION:
		break;
	case GET_VERSION:
	{
		bool ok;
		JsonObject result = QtJson::parse(str, ok).toMap();
		if (ok) {
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
				mSensTimer.start(2490);
			}
			getSensorList();
		}
		else {
			QLOG_ERROR() << "QtJson: Cannot parse string to Map: " << str;
			mState = IDLE;
		}
		break;
	}
	case GET_SENSORLIST:
	{
		bool ok;
		QList<QVariant> resultList = QtJson::parse(str, ok).toList();
		if (ok) {
			for (int i = 0; i < resultList.size(); ++i) {
				JsonObject result = resultList[i].toMap();
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
		}
		else
			QLOG_ERROR() << "QtJson: Cannot parse string to List: " << str;
		mState = IDLE;
		break;
	}
	case GET_SENSORDATA:
	{
		QLOG_TRACE() << "[Manager] State GET_SENSORDATA";
		bool ok;
		JsonObject result = QtJson::parse(str, ok).toMap();
		if (ok) {
			QString id = result["id"].toString();
			updateSensor(mSensorMap[id], result);
		}
		else
			QLOG_ERROR() << "QtJson: Cannot parse string to Map: " << str;
		mState = IDLE;
		break;
	}
	case GET_UPLINK:
	{
		QLOG_INFO() << "[Gateway::httpResult] " << endl << str;
		bool ok;
		JsonObject result = QtJson::parse(str, ok).toMap();
		if (ok) {
			mUplinkStatus = result["uplink"].toString();
			emit UplinkStatus(mUplinkStatus);
		}
		mState = IDLE;
		break;
	}
	default:
		QLOG_ERROR() << "[Gateway] mState switch ended up in default. Should not happen!";
		break;
	}
}

void Gateway::sensTimer()
{
	if (mState == IDLE) {
		//getSensorList();
		getVersion();
	}
}
