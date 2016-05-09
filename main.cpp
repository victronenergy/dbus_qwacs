#include <unistd.h>
#include <QCoreApplication>
#include <velib/qt/v_busitems.h>
#include "busitem_cons.h"
#include "qwacs.h"
#include "QsLog.h"
#include "arguments.h"

QsLogging::Logger& logger = QsLogging::Logger::instance();

void initLogger(QsLogging::Level logLevel)
{
	// init the logging mechanism
	QsLogging::DestinationPtr debugDestination(
			QsLogging::DestinationFactory::MakeDebugOutputDestination() );
	logger.addDestination(debugDestination);

	QLOG_INFO() << "dbus_qwacs" << "v" VERSION << "started";
	QLOG_INFO() << "Built with Qt" << QT_VERSION_STR << "running on" << qVersion();
	QLOG_INFO() << "Built on" << __DATE__ << "at" << __TIME__;
	logger.setLoggingLevel(logLevel);
}

void usage(Arguments & arg)
{
	arg.addArg("-h", "Print this help");
	arg.addArg("-d level", "Debug level: 0=TRACE, 1=DEBUG, 2=INFO...");
	arg.addArg("-g ip address", "IP address of gateway");
	arg.addArg("--dbus address", "dbus address or 'session' or 'system'");
}

int main(int argc, char *argv[])
{
	/*
	 * Description:
	 *
	 *This program tries to find the TIM gateway from Quby makeing use of Upnp.
	 *After this it will fetch the information from all sensors and publish it
	 *on the dbus
	 *
	 *UPNP code: https://garage.maemo.org/frs/download.php/8365/libbrisa_0.1.1.tar.gz
	*/

	QCoreApplication app(argc, argv);
	Arguments arg;

	usage(arg);
	if (arg.contains("h")) {
		arg.help();
		exit(0);
	}

	initLogger(QsLogging::InfoLevel);
	if (arg.contains("d"))
		logger.setLoggingLevel((QsLogging::Level)arg.value("d").toInt());

	if (arg.contains("dbus"))
		VBusItems::setDBusAddress(arg.value("dbus"));
	else
		VBusItems::setConnectionType(QDBusConnection::SystemBus);

	QDBusConnection dbus = VBusItems::getConnection("settings");
	if (!dbus.isConnected()) {
		QLOG_ERROR() << "DBus connection failed.";
		exit(EXIT_FAILURE);
	}

	// Wait for local settings to become available on the DBus
	QLOG_INFO() << "Wait for local settings on DBus... ";
	BusItemCons settings("com.victronenergy.settings", "/Settings", dbus);
	QVariant reply = settings.getValue();
	while (reply.isValid() == false) {
		reply = settings.getValue();
		usleep(500000);
		QLOG_INFO() << "Wait...";
	}
	QLOG_INFO() << "Local settings found!";

	Qwacs qwacs(&app, arg.value("g"));

	return app.exec();
}
