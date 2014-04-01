#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include "types.h"
#include "busitem_cons.h"

class Settings : public QObject
{
	Q_OBJECT
public:
	Settings(QObject *parent = 0);
	~Settings();

	Positions getPosition(const QString &id);
	Connections getConnection(const QString &id);
	Phases getPhase(const QString &id);
	uint getNumberOfPhases(const Connections conn);

signals:
	void sensorAddedOnPos(const QString & id);
	void sensRemovedFromPos(const QString & id, const Connections conn);

public slots:

private slots:
	void valueChanged(BusItemCons * const busitem);

private:
	void createBusItem(const Positions pos);
	QString getObjectPath(const Positions pos);
	Connections getConnection(const Positions pos);
	Phases getPhase(const Positions pos);

	QMultiMap<BusItemCons *, QString> mBusitemIdMap;
	QMap<BusItemCons *, Positions> mBusitemPositionMap;
	QMap<QString, Positions> mIdPositionMap;
	QMap<Positions, bool> mPositionsTakenMap;
};

#endif // SETTINGS_H
