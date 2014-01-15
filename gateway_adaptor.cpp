#include "gateway.h"

GatewayAdaptor::GatewayAdaptor(QObject *parent)
	: QDBusAbstractAdaptor(parent)
{
	mUpdays = 0;
	mUphours = 0;

	setAutoRelaySignals(true);
}
