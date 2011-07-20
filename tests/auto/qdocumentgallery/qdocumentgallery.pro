include(../auto.pri)

SOURCES += tst_qdocumentgallery.cpp

symbian: TARGET.CAPABILITY = ReadDeviceData ReadUserData WriteDeviceData
