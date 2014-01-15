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

	Positions getPosition(QString id);
	Connections getConnection(QString id);
	Phases getPhase(QString id);

signals:
	void sensorAddedOnPos(const QString & id);
	void sensRemovedFromPos(const QString & id, const Connections conn);

public slots:

private slots:
	void valueChanged(BusItemCons * const busitem);

private:
	void createBusItem(const Positions pos);
	QString getObjectPath(const Positions pos);
	Connections getConnection(Positions pos);

	QMultiMap<BusItemCons *, QString> mBusitemIdMap;
	QMap<BusItemCons *, Positions> mBusitemPositionMap;
	QMap<QString, Positions> mIdPositionMap;
};

#endif // SETTINGS_H
