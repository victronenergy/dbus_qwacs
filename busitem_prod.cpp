#include "busitem_prod.h"
#include "QsLog.h"

BusItemProd::BusItemProd(QObject *parent) :
	QObject(parent),
	mBusItem(0)
{
	mBusItem = new BusItemAdaptor(this);
	mValue = QVariant();
	mValid = false;
	mBusItem->connect(this, SIGNAL(PropertiesChanged(const QVariantMap &)), SIGNAL(PropertiesChanged(const QVariantMap &)));
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

void BusItemProd::setValue(const QVariant &value)
{
	if (mValue == value)
		return;

	mChanges.insert("Value", value);
	mValue = value;

}

void BusItemProd::setText(const QString &text)
{
	if (mText == text)
		return;

	mChanges.insert("Text", text);
	mText = text;
}

void BusItemProd::propertiesUpdated()
{
	if (!mChanges.empty()) {
		emit PropertiesChanged(mChanges);
		mChanges.clear();
	}
}
