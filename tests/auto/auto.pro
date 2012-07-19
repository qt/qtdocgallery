TEMPLATE = subdirs
SUBDIRS += \
    cmake
    qdocumentgallery \
    qgalleryabstractrequest \
    qgalleryabstractresponse \
    qgalleryfilter \
    qgalleryitemrequest \
    qgalleryquerymodel \
    qgalleryqueryrequest \
    qgalleryresource \
    qgallerytyperequest \
#    qdeclarativedocumentgalleryitem \
#    qdeclarativedocumentgallerymodel \
#    qdeclarativedocumentgallerytype

linux-*: contains(QT_CONFIG, dbus): contains(tracker_enabled, yes) {
    SUBDIRS += \
#        qgallerytrackerresultset_tracker \
#        qgallerytrackerschema_tracker
}
