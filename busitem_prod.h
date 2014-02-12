#ifndef BUSITEMPROD_H
#define BUSITEMPROD_H

#include "busitem_adaptor.h"

class BusItemProd : public QObject
{
	Q_OBJECT
	Q_PROPERTY(QDBusVariant Value READ getValue)

public:
	BusItemProd(QObject *parent = 0);
	BusItemProd(const QVariant &value, QObject *parent = 0);
	BusItemProd(const QString &value, QObject *parent = 0);

	void setValue(const QVariant &value);
	void setValid(const bool valid) { mValid = valid; }
	void setText(const QString &text);

public slots:
	QDBusVariant getValue() {return QDBusVariant(mValue); }
	bool getValid() { return mValid; }
	QString getText() { return mText; }

	void propertiesUpdated();

signals:
	void PropertiesChanged(const QVariantMap &changes);

private:
	BusItemAdaptor* mBusItem;
	QVariantMap mChanges;

	bool mValid;
	QVariant mValue;
	QString mText;
};

#endif // BUSITEMPROD_H
