#-------------------------------------------------
#
# Project created by QtCreator 2014-01-06T13:29:51
#
#-------------------------------------------------

# Add more folders to ship with the application, here
target.path = /opt/qwacs

machine=$$(MACHINE)
contains(machine,ccgx) {
	message($$(MACHINE))
	DEFINES += TARGET_ccgx
}

#CONFIG(release, debug|release):
DEFINES += QT_NO_DEBUG_OUTPUT

QT       += core
QT       -= gui
QT       += network
QT       += dbus
QT       += script

TARGET    = dbus_qwacs
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

include(ssdp/ssdp.pri)
include(json/json.pri)
include(QsLog/QsLog.pri)

SOURCES += main.cpp \
	httprequest.cpp \
	sensor.cpp \
	sensor_adaptor.cpp \
	qwacs.cpp \
	manager.cpp \
	manager_adaptor.cpp \
	gateway.cpp \
	gateway_adaptor.cpp \
	addsetting.cpp \
	busitem_adaptor.cpp \
	busitem_interface.cpp \
	busitem_prod.cpp \
	busitem_cons.cpp \
	pvinverter.cpp \
	settings.cpp \
	sensorset.cpp \

HEADERS += \
	httprequest.h \
	sensor.h \
	sensor_adaptor.h \
	qwacs.h \
	manager.h \
	manager_adaptor.h \
	gateway.h \
	gateway_adaptor.h \
	addsetting.h \
	busitem_adaptor.h \
	version.h \
	busitem_interface.h \
	busitem_prod.h \
	busitem_cons.h \
	defines.h \
	pvinverter.h \
	settings.h \
	types.h \
	sensorset.h \
