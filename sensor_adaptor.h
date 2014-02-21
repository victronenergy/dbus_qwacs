#ifndef SENSORADAPTOR_H
#define SENSORADAPTOR_H

#include <QDBusAbstractAdaptor>
#include <QString>

class SensorAdaptor : public QDBusAbstractAdaptor
{
	Q_OBJECT
	Q_CLASSINFO("D-Bus Interface", "com.victronenergy.sensor")
	Q_PROPERTY(QString id READ getID)
	Q_PROPERTY(QString type READ getType)
	Q_PROPERTY(QString name READ getName)
	Q_PROPERTY(bool connected READ getConnected)
	Q_PROPERTY(QString version READ getVersion)
	Q_PROPERTY(qreal voltage READ getVoltage)
	Q_PROPERTY(qreal current READ getCurrent)
	Q_PROPERTY(int power READ getPower)
	Q_PROPERTY(uint energyForward READ getEnergyForward)
	Q_PROPERTY(uint energyReverse READ getEnergyReverse)

public:
	SensorAdaptor(QObject *parent = 0);

	QString getID();
	QString getType();
	QString getName();
	bool getConnected();
	QString getVersion();
	qreal getVoltage();
	qreal getCurrent();
	int getPower();
	uint getEnergyForward();
	uint getEnergyReverse();

public slots:

signals:
	void PropertiesChanged();

private:

};

#endif // SENSORADAPTOR_H
