# dbus_qwacs version
VERSION = 1.0.6

# Add more folders to ship with the application here
unix {
    bindir = $$(bindir)
    DESTDIR = $$(DESTDIR)
    isEmpty(bindir) {
        bindir = /usr/local/bin
    }
    INSTALLS += target
    target.path = $${DESTDIR}$${bindir}
}

machine=$$(MACHINE)
contains(machine,ccgx) {
	DEFINES += TARGET_ccgx
}

#CONFIG(release, debug|release):
DEFINES += QT_NO_DEBUG_OUTPUT
DEFINES += VERSION=\\\"$${VERSION}\\\"

QT       += core
QT       -= gui
QT       += network
QT       += dbus
QT       += script

TARGET    = dbus-qwacs
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

include(ssdp/ssdp.pri)
include(json/json.pri)
include(QsLog/QsLog.pri)

INCLUDEPATH += \
    ext/velib/inc \

SOURCES += main.cpp \
    ext/velib/src/qt/v_busitems.cpp \
	httprequest.cpp \
	sensor.cpp \
	sensor_adaptor.cpp \
	qwacs.cpp \
	manager.cpp \
	manager_adaptor.cpp \
	gateway.cpp \
	gateway_adaptor.cpp \
	busitem_adaptor.cpp \
	busitem_interface.cpp \
	busitem_prod.cpp \
	busitem_cons.cpp \
	pvinverter.cpp \
	settings.cpp \
	sensorset.cpp \
	arguments.cpp

HEADERS += \
    ext/velib/inc/velib/qt/v_busitems.h \
	httprequest.h \
	sensor.h \
	sensor_adaptor.h \
	qwacs.h \
	manager.h \
	manager_adaptor.h \
	gateway.h \
	gateway_adaptor.h \
	busitem_adaptor.h \
	busitem_interface.h \
	busitem_prod.h \
	busitem_cons.h \
	pvinverter.h \
	settings.h \
	types.h \
	sensorset.h \
	arguments.h

# suppress the mangling of va_arg has changed for gcc 4.4
QMAKE_CXXFLAGS += -Wno-psabi
