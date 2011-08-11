TARGET  = declarative_gallery
TARGETPATH = Qt/gallery

include(qgalleryimport.pri)
target.path = $$[QT_INSTALL_IMPORTS]/$$TARGETPATH
DESTDIR = $$QT.gallery.imports/$$TARGETPATH
INSTALLS += target

qmldir.files += $$PWD/qmldir
qmldir.path +=  $$[QT_INSTALL_IMPORTS]/$$TARGETPATH
INSTALLS += qmldir

QT += declarative gallery script

HEADERS += \
    qdeclarativedocumentgallery.h \
    qdeclarativegalleryfilter.h \
    qdeclarativegalleryitem.h \
    qdeclarativegalleryquerymodel.h \
    qdeclarativegallerytype.h

SOURCES += \
    qdeclarativedocumentgallery.cpp \
    qdeclarativegallery.cpp \
    qdeclarativegalleryfilter.cpp \
    qdeclarativegalleryitem.cpp \
    qdeclarativegalleryquerymodel.cpp \
    qdeclarativegallerytype.cpp

symbian {
    # In Symbian, a library should enjoy _largest_ possible capability set.
    TARGET.CAPABILITY = ALL -TCB
    # Allow writable DLL data
    TARGET.EPOCALLOWDLLDATA = 1
    # Target UID, makes every Symbian app unique
    TARGET.UID3 = 0x2002BFCC
    # Specifies what files shall be deployed: the plugin itself and the qmldir file.
    importFiles.sources = $$DESTDIR/declarative_gallery$${QT_LIBINFIX}.dll qmldir
    importFiles.path = $$QT_IMPORTS_BASE_DIR/$$TARGETPATH
    DEPLOYMENT = importFiles
 }
