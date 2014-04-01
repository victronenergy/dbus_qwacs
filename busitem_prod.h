#ifndef BUSITEMPROD_H
#define BUSITEMPROD_H

#include "busitem_adaptor.h"

class BusItemProd : public QObject
{
	Q_OBJECT
	Q_PROPERTY(QDBusVariant value READ getValue)

public:
	BusItemProd(QObject *parent = 0);
	BusItemProd(const QVariant &value, QObject *parent = 0);
	BusItemProd(const QString &value, QObject *parent = 0);
	~BusItemProd();

	void setValue(const QVariant &value);
	void setText(const QString &text);
	void invalidate();

public slots:
	QDBusVariant getValue() { return QDBusVariant(mValue); }
	QString getText() { return mText; }
	void propertiesUpdated();

signals:
	void PropertiesChanged(const QVariantMap &changes);

private:
	static QStringList invalidValue;

	BusItemAdaptor* mBusItem;
	QVariantMap mChanges;

	//bool mInitialized;
	QVariant mValue;
	QString mText;
};

#endif // BUSITEMPROD_H
