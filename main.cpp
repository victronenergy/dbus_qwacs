#include <unistd.h>
#include <QCoreApplication>
#include "defines.h"
#include "busitem_cons.h"
#include "qwacs.h"
#include "QsLog.h"
#include "version.h"

QsLogging::Logger& logger = QsLogging::Logger::instance();

void initLogger(QsLogging::Level logLevel)
{
	// init the logging mechanism
	QsLogging::DestinationPtr debugDestination(
			QsLogging::DestinationFactory::MakeDebugOutputDestination() );
	logger.addDestination(debugDestination);

	QLOG_INFO() << "dbus_qwacs" << "v"VERSION << "started" << "("REVISION")";
	QLOG_INFO() << "Built with Qt" << QT_VERSION_STR << "running on" << qVersion();
	QLOG_INFO() << "Built on" << __DATE__ << "at" << __TIME__;
	logger.setLoggingLevel(logLevel);
}

int main(int argc, char *argv[])
{
	QCoreApplication app(argc, argv);

	/*
	 * Description:
	 *
	 *This program tries to find the TIM gateway from Quby makeing use of Upnp.
	 *After this it will fetch the information from all sensors and publish it
	 *on the dbus
	 *
	 *UPNP code: https://garage.maemo.org/frs/download.php/8365/libbrisa_0.1.1.tar.gz
	*/

	initLogger(QsLogging::InfoLevel);

	QDBusConnection dbus = DBUS_CONNECTION;
	if (!dbus.isConnected()) {
		QLOG_ERROR() << "DBus connection failed.";
		exit(EXIT_FAILURE);
	}

#if TARGET_ccgx
	// Wait for local settings to become available on the DBus
	QLOG_INFO() << "Wait for local setting on DBus... ";
	BusItemCons settings("com.victronenergy.settings", "/Settings", DBUS_CONNECTION);
	QVariant reply = settings.getValue();
	while (reply.isValid() == false) {
		reply = settings.getValue();
		usleep(2000000);
		QLOG_INFO() << "Wait...";
	}
	QLOG_INFO() << "Local settings found!";
#endif

	Qwacs qwacs(&app);

	return app.exec();
}
