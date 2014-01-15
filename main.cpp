#include <QCoreApplication>
#include <QTimer>

#include <QDBusConnection>
#include <QtDBus/QtDBus>

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
	 *JSON code: https://github.com/da4c30ff/qt-json
	 *UPNP code: https://garage.maemo.org/frs/download.php/8365/libbrisa_0.1.1.tar.gz
	*/

	/*
	 * TODO:
	 *
	 * - Gateway_adaptor members naar Gateway (zie sensor)
	 * - senserset for sum sensors on same position
	 */

	Qwacs qwacs(&app);

	return app.exec();
}
