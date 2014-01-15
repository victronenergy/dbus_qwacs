#ifndef DEFINES_H
#define DEFINES_H

#if TARGET_ccgx
	#define DBUS_CONNECTION QDBusConnection::systemBus()
#else
	#define DBUS_CONNECTION QDBusConnection::sessionBus()
#endif

#endif // DEFINES_H
