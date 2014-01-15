#ifndef ADDSETTING_H
#define ADDSETTING_H

#include <QObject>
#include <QtDBus/QtDBus>

class AddSetting : public QDBusAbstractInterface
{
	Q_OBJECT
public:
	static inline const char *staticInterfaceName()
	{ return "com.victronenergy.Settings"; }

	AddSetting(const QString &service, const QString &path, const QDBusConnection &connection, QObject *parent = 0);

signals:

public slots:
	QDBusReply<int> add(const QString &group, const QString &name, const QVariant &defaultValue, const QString &itemType, const QVariant &minimum, const QVariant &maximum)
	{
		return call("AddSetting", group, name, defaultValue, itemType, minimum, maximum);
	}

private:

};

#endif // ADDSETTING_H
