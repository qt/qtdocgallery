QT.gallery.VERSION = 5.0.0
QT.gallery.MAJOR_VERSION = 5
QT.gallery.MINOR_VERSION = 0
QT.gallery.PATCH_VERSION = 0

QT.gallery.name = QtGallery
QT.gallery.bins = $$QT_MODULE_BIN_BASE
QT.gallery.includes = $$QT_MODULE_INCLUDE_BASE $$QT_MODULE_INCLUDE_BASE/QtGallery
QT.gallery.private_includes = $$QT_MODULE_INCLUDE_BASE/QtGallery/$$QT.gallery.VERSION
QT.gallery.sources = $$QT_MODULE_BASE/src/gallery
QT.gallery.libs = $$QT_MODULE_LIB_BASE
QT.gallery.plugins = $$QT_MODULE_PLUGIN_BASE
QT.gallery.imports = $$QT_MODULE_IMPORT_BASE
QT.gallery.depends = core
QT.gallery.DEFINES = QT_GALLERY_LIB

QT_CONFIG += gallery
