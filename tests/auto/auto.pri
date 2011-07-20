TEMPLATE = app
CONFIG += qt debug warn_on console depend_includepath testcase

qtAddLibrary(QtTest)

QMAKE_LIBS += -Wl,-rpath,$${QT.gallery.libs}

QT += gallery
