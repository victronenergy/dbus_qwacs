#include "settings.h"
#include "defines.h"
#include "QsLog.h"

const QString service = "com.victronenergy.settings";
const QString path_prefix = "/Settings/Sensors/OnPosition/";
const QString noSensID =  "00.00.00.00.00";


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

	mPositionsTakenMap.insert(pos, false);
	QVariant reply = busitem->getValue();
	if (reply.isValid()) {
		mBusitemPositionMap.insert(busitem, pos);
		QString replyString = reply.toString();
		QStringList sensors = replyString.split(",");
		for (int i = 0; i < sensors.size(); i++ ) {
			mBusitemIdMap.insert(busitem, sensors[i]);
			mIdPositionMap.insert(sensors[i], pos);
		}
		if (!replyString.isEmpty() && replyString != noSensID)
			mPositionsTakenMap[pos] = true;
	}
}

QString Settings::getObjectPath(const Positions pos)
{
	switch (pos)
	{
	case None:
		return QString();
	case ACIn1_L1:
		return "ACIn1_L1";
	case ACIn1_L2:
		return "ACIn1_L2";
	case ACIn1_L3:
		return "ACIn1_L3";
	case ACIn2_L1:
		return "ACIn2_L1";
	case ACIn2_L2:
		return "ACIn2_L2";
	case ACIn2_L3:
		return "ACIn2_L3";
	case ACOut_L1:
		return "ACOut_L1";
	case ACOut_L2:
		return "ACOut_L2";
	case ACOut_L3:
		return "ACOut_L3";
	}
	return QString();
}

Positions Settings::getPosition(const QString &id)
{
	if (mIdPositionMap.contains(id))
		return mIdPositionMap[id];

	return None;
}

Connections Settings::getConnection(const QString &id)
{
	//QLOG_TRACE() << "[Settings::getConnection()] id: " << id;
	if (mIdPositionMap.contains(id))
		return getConnection(mIdPositionMap[id]);
	return NoConn;
}

Connections Settings::getConnection(const Positions pos)
{
	//QLOG_TRACE() << "[Settings::getConnection()] pos: " << pos;
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

Phases Settings::getPhase(const QString &id)
{
	if (mIdPositionMap.contains(id)) {
		return getPhase(mIdPositionMap[id]);
	}
	return NoPhase;
}

Phases Settings::getPhase(const Positions pos)
{
	switch (pos)
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
	return NoPhase;
}

uint Settings::getNumberOfPhases(Connections conn)
{
	int phases = 0;
	QMap<Positions, bool>::const_iterator i = mPositionsTakenMap.constBegin();
	while (i != mPositionsTakenMap.constEnd()) {
		if (getConnection(i.key()) == conn && i.value() == true)
			phases++;
		++i;
	}
	//QLOG_INFO() << "[Settings::mConnectionNumOfPhasesMap()] conn: " << conn << " phases: " << phases;
	return phases;
}

void Settings::valueChanged(BusItemCons * const busitem)
{
	Positions pos = mBusitemPositionMap[busitem];
	QList<QString> oldSensorsList = mBusitemIdMap.values(busitem);
	QVariant reply = busitem->getValue();

	if (reply.isValid()) {
		QStringList newSensorList = reply.toString().split(",");

		if (reply.toString().isEmpty() || reply.toString() == noSensID)
			mPositionsTakenMap[pos] = false;
		else
			mPositionsTakenMap[pos] = true;

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
