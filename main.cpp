#include <iostream>
#include <unistd.h>
#include <QCoreApplication>
#include "defines.h"
#include "busitem_cons.h"
#include "qwacs.h"

int main(int argc, char *argv[])
{
	QCoreApplication app(argc, argv);

	/*
	 * Description:
	 *
	 *This program tries to find the TIM gateway from Quby makeing use of Upnp.
	 *After this it will fetch the information from all sensors and publish it
	 *on the dbus under com.victronenergy.quby.sensors
	 *
	 *UPNP code: https://garage.maemo.org/frs/download.php/8365/libbrisa_0.1.1.tar.gz
	*/

	/*
	 * TODO:
	 *
	 * - Sensorsets for sum sensors on same position
	 * - Check unregister and delete when quiting
	 */

	QDBusConnection dbus = DBUS_CONNECTION;
	if (!dbus.isConnected()) {
		std::cerr << "DBus connection failed.";
		exit(EXIT_FAILURE);
	}

	// Wait for local settings to become available on the DBus
	std::cerr << "Wait for local setting on DBus... ";
	BusItemCons settings("com.victronenergy.settings", "/Settings/Sensors/OnPosition/ACIn1/L1", DBUS_CONNECTION);
	QVariant reply = settings.getValue();
	while (reply.isValid() == false) {
		reply = settings.getValue();
		usleep(500000);
	}
	std::cerr << "Found!" << std::endl;

	Qwacs qwacs(&app);

	return app.exec();
}
