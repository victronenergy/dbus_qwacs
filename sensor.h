#ifndef SENSOR_H
#define SENSOR_H

#include "sensor_adaptor.h"
#include "busitem_adaptor.h"
#include "types.h"

class Sensor : public QObject
{
	Q_OBJECT

public:
	Sensor(QObject *parent = 0);

	void setID(const QString &id) { mID = id; }
	void setType(QString type) { mType = type; }
	void setName(QString name) { mName = name; }
	void setConnected(bool connected) { mConnected = connected; }
	void setVersion(QString version) { mVersion = version; }
	void setVoltage(double value) { mVrms = value; }
	void setCurrent(double value) { mIrms = value; }
	void setPower(int value) { mPower = value; }
	void setEnergyForward(uint value) { mEnergyForward = value; }
	void setEnergyReverse(uint value) { mEnergyReverse = value; }

	inline bool operator==(const QString &id) {
		return mID == id;
	}

public slots:
	const QString getID() const { return mID; }
	const QString getType() const { return mType; }
	const QString getName() const { return mName; }
	bool getConnected() const { return mConnected; }
	const QString getVersion() const { return mVersion; }
	double getVoltage() const { return mVrms; }
	double getCurrent() const { return mIrms; }
	int getPower() const { return mPower; }
	uint getEnergyForward() const { return mEnergyForward; }
	uint  getEnergyReverse() const { return mEnergyReverse; }

	void sensorUpdated() { emit propertiesChanged(); }

signals:
	void propertiesChanged();

private:
	SensorAdaptor* mAdaptor;

	QString mID;
	QString mType;
	QString mName;
	bool mConnected;
	QString mVersion;
	double mVrms; /* V */
	double mIrms; /* A */
	int mPower; /* W */
	uint mEnergyForward; /* Wh */
	uint mEnergyReverse; /* Wh */
};

#endif // SENSOR_H
