include(../auto.pri)

SOURCES += tst_qdocumentgallery.cpp

linux-*: contains(QT_CONFIG, dbus): contains(tracker_enabled, yes): DEFINES += QT_TRACKER_ENABLED

symbian: TARGET.CAPABILITY = ReadDeviceData ReadUserData WriteDeviceData
