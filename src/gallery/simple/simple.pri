INCLUDEPATH += $$PWD

QT += gui jsondb jsondb-private

PRIVATE_HEADERS += \
    $$PWD/qsimplegalleryresultset.h \
    $$PWD/qsimplegalleryqueryresultset.h \
    $$PWD/qsimplegalleryitemresultset.h \
    $$PWD/qgallerysimpleutility.h \
    $$PWD/qmdlite.h \
    $$PWD/qmdlitemusic.h \
    $$PWD/qmdlite_p.h \
    $$PWD/qmdlitemusic_p.h

SOURCES += \
    $$PWD/qdocumentgallery_simple.cpp \
    $$PWD/qsimplegalleryqueryresultset.cpp \
    $$PWD/qsimplegalleryitemresultset.cpp \
    $$PWD/qsimplegalleryresultset.cpp \
    $$PWD/qgallerysimpleutility.cpp \
    $$PWD/qmdlite.cpp \
    $$PWD/qmdlitemusic.cpp

QMDLITE_SQLITE=$$(QMDLITE_SQLITE)
!isEmpty(QMDLITE_SQLITE) {
    DEFINES += _SQLITE_QMDBACKEND
    QT += sql
    SOURCES += $$PWD/qmdlitequeryresultsqlite.cpp
    PRIVATE_HEADERS += $$PWD/qmdlitequeryresultsqlite_p.h
    message(Setting SQLITE)
} else {
    SOURCES += $$PWD/qmdlitequeryresultjsondb.cpp
    PRIVATE_HEADERS += $$PWD/qmdlitequeryresultjsondb_p.h
    message(Setting JSONDB)
}
