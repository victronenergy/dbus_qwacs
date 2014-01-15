#ifndef GATEWAYADAPTOR_H
#define GATEWAYADAPTOR_H

#include <QDBusAbstractAdaptor>
#include "sensor.h"

Q_DECLARE_METATYPE(QList<QString>)

class GatewayAdaptor : public QDBusAbstractAdaptor
{
	Q_OBJECT
	Q_CLASSINFO("D-Bus Interface", "com.victronenergy.gateway")
	Q_PROPERTY(QString commonName READ getCommonName)
	Q_PROPERTY(QString firmwareVersion READ getFirmwareVersion)
	Q_PROPERTY(QString archFlavVers READ getArchFlavVers)
	Q_PROPERTY(QString serialNr READ getSerialNr)
	Q_PROPERTY(QString partNr READ getPartNr)
	Q_PROPERTY(uint updays READ getUpdays)
	Q_PROPERTY(uint uphours READ getUphours)

public:
	GatewayAdaptor(QObject *parent = 0);

	QString getCommonName() const { return mCommonName; }
	QString getFirmwareVersion() const { return mFirmwareVersion; }
	QString getArchFlavVers() const { return mArchFlavVers; }
	QString getSerialNr() const { return mCommonName; }
	QString getPartNr() const { return mCommonName; }
	uint getUpdays() const { return mUpdays; }
	uint getUphours() const { return mUphours; }

	void setCommonName(const QString &value) { mCommonName = value; }
	void setFirmwareVersion(const QString &value) { mFirmwareVersion = value; }
	void setArchFlavVers(const QString &value) { mArchFlavVers = value; }
	void setSerialNr(const QString &value) { mSerialNr = value; }
	void setPartNr(const QString &value) { mPartNr = value; }
	void setUpdays(const uint value) { mUpdays = value; }
	void setUphours(const uint value) { mUphours = value; }

signals:
	void gateway(const QString &name, const QString &version);

public slots:

private:
	QString mCommonName;
	QString mFirmwareVersion;
	QString mArchFlavVers;
	QString mSerialNr;
	QString mPartNr;
	uint mUpdays;
	uint mUphours;
};

#endif // GATEWAYADAPTOR_H
