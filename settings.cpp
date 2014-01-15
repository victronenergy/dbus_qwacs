#include "settings.h"
#include "defines.h"
#include "QsLog.h"

const QString service = "com.victronenergy.settings";
const QString path_prefix = "/Settings/Sensors/OnPosition/";

Settings::Settings(QObject *parent) :
	QObject(parent)
{
	createBusItem(ACIn1_L1);
	createBusItem(ACIn1_L2);
	createBusItem(ACIn1_L3);
	createBusItem(ACIn2_L1);
	createBusItem(ACIn2_L2);
	createBusItem(ACIn2_L3);
	createBusItem(ACOut_L1);
	createBusItem(ACOut_L2);
	createBusItem(ACOut_L3);
}

void Settings::createBusItem(const Positions pos)
{
	const QString objPath= path_prefix+getObjectPath(pos);

	BusItemCons * busitem = new BusItemCons(service, objPath, DBUS_CONNECTION);

	connect(busitem, SIGNAL(valueChanged(BusItemCons * const )), this, SLOT(valueChanged(BusItemCons * const)));

	QVariant reply = busitem->getValue();
	if (reply.isValid()) {
		mBusitemPositionMap.insert(busitem, pos);
		QStringList sensors = reply.toString().split(",");
		for (int i = 0; i < sensors.size(); i++ ) {
			mBusitemIdMap.insert(busitem, sensors[i]);
			mIdPositionMap.insert(sensors[i], pos);
		}
	}
}

QString Settings::getObjectPath(const Positions pos)
{
	switch (pos)
	{
	case None:
		return QString();
	case ACIn1_L1:
		return "ACIn1/L1";
	case ACIn1_L2:
		return "ACIn1/L2";
	case ACIn1_L3:
		return "ACIn1/L3";
	case ACIn2_L1:
		return "ACIn2/L1";
	case ACIn2_L2:
		return "ACIn2/L2";
	case ACIn2_L3:
		return "ACIn2/L3";
	case ACOut_L1:
		return "ACOut/L1";
	case ACOut_L2:
		return "ACOut/L2";
	case ACOut_L3:
		return "ACOut/L3";
	}
	return QString();
}

Positions Settings::getPosition(QString id)
{
	if (mIdPositionMap.contains(id))
		return mIdPositionMap[id];

	return None;
}

Connections Settings::getConnection(QString id)
{
	QLOG_TRACE() << "[Settings::getConnection()] id: " << id;
	if (mIdPositionMap.contains(id))
		return getConnection(mIdPositionMap[id]);
	return NoConn;
}

Connections Settings::getConnection(Positions pos)
{
	QLOG_TRACE() << "[Settings::getConnection()] pos: " << pos;
	switch (pos)
	{
	case None:
		return NoConn;
	case ACIn1_L1:
	case ACIn1_L2:
	case ACIn1_L3:
		return ACIn1;
	case ACIn2_L1:
	case ACIn2_L3:
	case ACIn2_L2:
		return ACIn2;
	case ACOut_L1:
	case ACOut_L3:
	case ACOut_L2:
		return ACOut;
	}
	return NoConn;
}

Phases Settings::getPhase(QString id)
{
	if (mIdPositionMap.contains(id)) {
		switch (mIdPositionMap[id])
		{
		case None:
			return NoPhase;
		case ACIn1_L1:
		case ACIn2_L1:
		case ACOut_L1:
			return L1;
		case ACIn1_L2:
		case ACIn2_L2:
		case ACOut_L2:
			return L2;
		case ACIn1_L3:
		case ACIn2_L3:
		case ACOut_L3:
			return L3;
		}

	}
	return NoPhase;
}

void Settings::valueChanged(BusItemCons * const busitem)
{
	Positions pos = mBusitemPositionMap[busitem];
	QList<QString> oldSensorsList = mBusitemIdMap.values(busitem);
	QVariant reply = busitem->getValue();

	if (reply.isValid()) {
		QStringList newSensorList = reply.toString().split(",");

		QLOG_INFO() << "[Settings::valueChanged()] " << getObjectPath(pos) << " = " << newSensorList;
		// Check for removed sensors
		for (int i = 0; i < oldSensorsList.size(); i++) {
			if (!newSensorList.contains(oldSensorsList[i])) {
				mBusitemIdMap.remove(busitem, oldSensorsList[i]);
				emit sensRemovedFromPos(oldSensorsList[i], getConnection(pos));
				mIdPositionMap.remove(oldSensorsList[i]);
			}
		}
		// Check for new sensors
		for (int i = 0; i < newSensorList.size(); i++) {
			if (!oldSensorsList.contains(newSensorList[i])) {
				mBusitemIdMap.insert(busitem, newSensorList[i]);
				mIdPositionMap.insert(newSensorList[i], pos);
				emit sensorAddedOnPos(newSensorList[i]);
			}
		}

	} else
		QLOG_WARN() << "[Settings::valueChanged] reply is not valid ";
}
