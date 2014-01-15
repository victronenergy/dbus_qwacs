#include "busitem_prod.h"
#include "QsLog.h"

BusItemProd::BusItemProd(QObject *parent) :
	QObject(parent),
	mBusItem(0)
{
	mBusItem = new BusItemAdaptor(this);
	mValue = QVariant();
	mValid = false;
}

BusItemProd::BusItemProd(const QVariant &value, QObject *parent) :
	QObject(parent),
	mBusItem(0)
{
	mBusItem = new BusItemAdaptor(this);
	mValue = value;
	mValid = true;
}

BusItemProd::BusItemProd(const QString &value, QObject *parent) :
	QObject(parent),
	mBusItem(0)
{
	mBusItem = new BusItemAdaptor(this);
	mValue = QVariant(value);
	mValid = true;
}
