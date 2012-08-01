include(../auto.pri)

SOURCES += tst_qdocumentgallery.cpp

linux-*: {
    contains(QT_CONFIG, dbus): contains(tracker_enabled, yes): DEFINES += QT_TRACKER_ENABLED

    contains(QT_CONFIG, jsondb): DEFINES += QT_SIMPLE_ENABLED
}
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0
