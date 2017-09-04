#-------------------------------------------------
#
# Project created by QtCreator 2015-06-24 上午11:12
#
#-------------------------------------------------

QT       += network xml
QT       += gui
QT       += widgets


TARGET = DDTBBE_RecSet
TEMPLATE = lib

DEFINES += DDTBBE_RECSETGUISHARED_LIBRARY

SOURCES += src/DDTBBE_RecSetGUI.cpp \
    src/DDTBBE_RecSetGUIExport.cpp \
    src/DDTBBE_RecSetGUIDlg.cpp \
    ../Common/DeviceMap.cpp \
    ../Common/Log.cpp

HEADERS +=  src/DDTBBE_RecSetGUI.h\
         src/DDTBBE_RecSet_global.h \
    src/DDTBBE_RecSetGUIDlg.h \
    ../Common/DataStruct2311.h \
    ../Common/Dcu_common_DataStruct.h \
    ../Common/LxTsspCommonDefinition.h \
    ../Common/MCSDataStruct.h \
    ../Common/TypeDefine.h \
    ../Common/DeviceMap.h \
    ../Common/Log.h \
    ../Common/SMCUHeadFile.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

FORMS += \
    src/DDTBBE_RecSetGUIDlg.ui

MOC_DIR = DDTBBE_RecSet/moc
RCC_DIR = DDTBBE_RecSet/rcc
UI_DIR = DDTBBE_RecSet/ui
OBJECTS_DIR = DDTBBE_RecSet/obj
DESTDIR += ../../
