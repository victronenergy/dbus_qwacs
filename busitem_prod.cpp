#include "busitem_prod.h"
#include "QsLog.h"

QStringList BusItemProd::invalidValue = QStringList();

BusItemProd::BusItemProd(QObject *parent) :
	QObject(parent),
	mBusItem(0)
{
	mBusItem = new BusItemAdaptor(this);
	mValue = invalidValue;
	mBusItem->connect(this, SIGNAL(PropertiesChanged(const QVariantMap &)), SIGNAL(PropertiesChanged(const QVariantMap &)));
}

BusItemProd::~BusItemProd()
{
	delete mBusItem;
}

BusItemProd::BusItemProd(const QVariant &value, QObject *parent) :
	QObject(parent),
	mBusItem(0)
{
	mBusItem = new BusItemAdaptor(this);
	mValue = value;
}

BusItemProd::BusItemProd(const QString &value, QObject *parent) :
	QObject(parent),
	mBusItem(0)
{
	mBusItem = new BusItemAdaptor(this);
	mValue = QVariant(value);
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

void BusItemProd::invalidate()
{
	setValue(invalidValue);
	setText("");
	propertiesUpdated();
}

void BusItemProd::propertiesUpdated()
{
	if (!mChanges.empty()) {
		emit PropertiesChanged(mChanges);
		mChanges.clear();
	}
}
