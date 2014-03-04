#ifndef BUSITEMADAPTOR_H
#define BUSITEMADAPTOR_H

#include <QDBusAbstractAdaptor>
#include <QtDBus/QtDBus>
#include <QString>

class BusItemAdaptor : public QDBusAbstractAdaptor
{
	Q_OBJECT
	Q_CLASSINFO("D-Bus Interface", "com.victronenergy.BusItem")
	Q_PROPERTY(QString Text READ text)
	Q_PROPERTY(QDBusVariant Value READ value WRITE setValue)

public:
	BusItemAdaptor(QObject *parent = 0);

	QString text() const;
	bool valid() const;
	QDBusVariant value() const;
	void setValue(const QDBusVariant &value);

public slots:
	QString GetDescription(const QString &language, int length);
	QDBusVariant GetMax();
	QDBusVariant GetMin();
	QString GetText();
	QDBusVariant GetValue();
	int SetValue(const QDBusVariant &value);

signals:
	void PropertiesChanged(const QVariantMap &changes);

private:

};

#endif // BUSITEMADAPTOR_H
