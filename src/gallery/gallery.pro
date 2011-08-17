load(qt_module)

TARGET = QtAddOnGallery
QPRO_PWD = $PWD

CONFIG += module
MODULE_PRI = ../../modules/qt_gallery.pri

QT = core

DEFINES += QT_MAKEDLL

load(qt_module_config)

PUBLIC_HEADERS += \
    qabstractgallery.h \
    qdocumentgallery.h \
    qgalleryabstractrequest.h \
    qgalleryabstractresponse.h \
    qgalleryfilter.h \
    qgalleryitemrequest.h \
    qgalleryproperty.h \
    qgalleryquerymodel.h \
    qgalleryqueryrequest.h \
    qgalleryresource.h \
    qgalleryresultset.h \
    qgallerytype.h \
    qgallerytyperequest.h

PRIVATE_HEADERS += \
    qgallery_p.h \
    qabstractgallery_p.h \
    qgalleryabstractrequest_p.h \
    qgalleryabstractresponse_p.h \
    qgallerynullresultset_p.h \
    qgalleryresultset_p.h

SOURCES += \
    qabstractgallery.cpp \
    qdocumentgallery.cpp \
    qgalleryabstractrequest.cpp \
    qgalleryabstractresponse.cpp \
    qgalleryfilter.cpp \
    qgalleryitemrequest.cpp \
    qgalleryquerymodel.cpp \
    qgalleryqueryrequest.cpp \
    qgalleryresource.cpp \
    qgalleryresultset.cpp \
    qgallerytyperequest.cpp

OTHER_FILES = \
    qgalleryproperty.qdoc \
    qgallerytype.qdoc

simulator: {
    include (simulator/simulator.pri)
} else: linux-*: contains(QT_CONFIG, dbus): contains(tracker_enabled, yes) {
    include (tracker/tracker.pri)
} else: symbian: contains(mds_enabled, yes) {
     include (symbian/symbian.pri)
} else {
    DEFINES += QT_DOCUMENT_GALLERY_NULL
}

HEADERS = qtaddongalleryversion.h $$PUBLIC_HEADERS $$PRIVATE_HEADERS
