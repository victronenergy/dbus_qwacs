#include "busitem_cons.h"
#include "QsLog.h"

BusItemCons::BusItemCons(const QString &service, const QString &path, const QDBusConnection &connection, QObject *parent) :
	QObject(parent),
	mBusItem(0)
{
	mValueValid = false;

	mBusItem = new BusItemInterface(service, path, connection, this);
	connect(mBusItem, SIGNAL(PropertiesChanged(const QVariantMap &)), this, SLOT(PropertiesChanged(const QVariantMap &)));
}

BusItemCons::~BusItemCons()
{
	delete mBusItem;
}

QVariant BusItemCons::getValue()
{
	if (!mValueValid)
		mValue = mBusItem->getValue();
	return mValue;
}

void BusItemCons::PropertiesChanged(const QVariantMap &changes)
{
	QMapIterator<QString, QVariant> i(changes);
	while (i.hasNext()) {
		i.next();
		QLOG_TRACE() << "[BusItemCons] Properties changed: " << i.key() << "=" << i.value().toString();
		if (i.key() == "Value") {
			if (i.value().isValid()) {
				mValueValid = true;
			}
			if (i.value() != mValue) {
				mValue = i.value();
				emit valueChanged(this);
			}
		}
		//else if (i.key() == "Text") {
		//}
	}
}
