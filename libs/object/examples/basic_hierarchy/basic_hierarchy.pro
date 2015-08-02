TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp

DEFINES += MAKE_HIERARCHY_SNAPSHOT

INCLUDEPATH += $$BOOST_OBJECT_TOP_DIR

include($$BOOST_OBJECT_TOP_DIR/config.pri)


