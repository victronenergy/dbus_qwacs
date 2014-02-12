#include "busitem_adaptor.h"

BusItemAdaptor::BusItemAdaptor(QObject *parent)
	: QDBusAbstractAdaptor(parent)
{
}

QString BusItemAdaptor::text() const
{
	return qvariant_cast< QString >(parent()->property("Text"));
}

bool BusItemAdaptor::valid() const
{
	return qvariant_cast< bool >(parent()->property("Valid"));
}

QDBusVariant BusItemAdaptor::value() const
{
	return qvariant_cast< QDBusVariant >(parent()->property("Value"));
}

void BusItemAdaptor::setValue(const QDBusVariant &value)
{
	parent()->setProperty("Value", QVariant::fromValue(value.variant()));
}

QString BusItemAdaptor::GetDescription(const QString &language, int length)
{
	QString descr;
	QMetaObject::invokeMethod(parent(), "getDescription", Q_RETURN_ARG(QString, descr), Q_ARG(QString, language), Q_ARG(int, length));
	return descr;
}

QDBusVariant BusItemAdaptor::GetMax()
{
	QDBusVariant value;
	QMetaObject::invokeMethod(parent(), "getMax", Q_RETURN_ARG(QDBusVariant, value));
	return value;
}

QDBusVariant BusItemAdaptor::GetMin()
{
	QDBusVariant value;
	QMetaObject::invokeMethod(parent(), "getMin", Q_RETURN_ARG(QDBusVariant, value));
	return value;
}

QString BusItemAdaptor::GetText()
{
	QString value;
	QMetaObject::invokeMethod(parent(), "getText", Q_RETURN_ARG(QString, value));
	return value;
}

bool BusItemAdaptor::GetValid()
{
	bool value;
	QMetaObject::invokeMethod(parent(), "getValid", Q_RETURN_ARG(bool, value));
	return value;
}

QDBusVariant BusItemAdaptor::GetValue()
{
	QDBusVariant value;
	QMetaObject::invokeMethod(parent(), "getValue", Q_RETURN_ARG(QDBusVariant, value));
	return value;
}

int BusItemAdaptor::SetValue(const QDBusVariant &value)
{
	int retval;
	QMetaObject::invokeMethod(parent(), "setValue", Q_RETURN_ARG(int, retval), Q_ARG(QDBusVariant, value));
	return retval;
}
