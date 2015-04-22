# dbus_qwacs version and revision
VERSION = 1.0.3
REVISION = $$system(git --git-dir $$PWD/.git --work-tree $$PWD describe --always --dirty --tags)

# Create a include file with VERION / REVISION
version_rule.target = $$OUT_PWD/version.h
version_rule.commands = @echo \"updating file $$revtarget.target\"; \
	printf \"/* generated file (do not edit) */\\n \
	$${LITERAL_HASH}ifndef VERSION_H\\n \
	$${LITERAL_HASH}define VERSION_H\\n \
	$${LITERAL_HASH}define VERSION \\\"$${VERSION}\\\"\\n \
	$${LITERAL_HASH}define REVISION \\\"$${REVISION}\\\"\\n \
	$${LITERAL_HASH}endif\" > $$version_rule.target
version_rule.depends = FORCE
QMAKE_DISTCLEAN += $$version_rule.target

QMAKE_EXTRA_TARGETS += version_rule
PRE_TARGETDEPS += $$OUT_PWD/version.h

# Add more folders to ship with the application, here
target.path = /opt/qwacs
INSTALLS += target

machine=$$(MACHINE)
contains(machine,ccgx) {
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
	busitem_adaptor.cpp \
	busitem_interface.cpp \
	busitem_prod.cpp \
	busitem_cons.cpp \
	pvinverter.cpp \
	settings.cpp \
	sensorset.cpp \
	arguments.cpp

HEADERS += \
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
	defines.h \
	pvinverter.h \
	settings.h \
	types.h \
	sensorset.h \
	arguments.h

# suppress the mangling of va_arg has changed for gcc 4.4
QMAKE_CXXFLAGS += -Wno-psabi
