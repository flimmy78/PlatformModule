#-------------------------------------------------
#
# Project created by QtCreator 2015-06-24 上午11:12
#
#-------------------------------------------------

QT       += network xml

QT       += gui
QT       += widgets


TARGET = DDTBBE_ICodFrmSynSet
TEMPLATE = lib

DEFINES += DDTBBE_CODFRMSYNSETGUISHARED_LIBRARY

SOURCES += \
    ../Common/DeviceMap.cpp \
    ../Common/Log.cpp \
    src/DDTBBE_ICodFrmSynSetGUI.cpp \
    src/DDTBBE_ICodFrmSynSetGUIExport.cpp \
    src/DDTBBE_ICodFrmSynSetGUIDlg.cpp

HEADERS += \
    ../Common/DataStruct2311.h \
    ../Common/Dcu_common_DataStruct.h \
    ../Common/LxTsspCommonDefinition.h \
    ../Common/MCSDataStruct.h \
    ../Common/TypeDefine.h \
    ../Common/DeviceMap.h \
    ../Common/Log.h \
    src/DDTBBE_ICodFrmSynSetGUI.h \
    src/DDTBBE_ICodFrmSynSetGUIDlg.h \
    src/DDTBBE_ICodFrmSynSetGUI_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

FORMS += \
    src/DDTBBE_ICodFrmSynSetGUIDlg.ui

MOC_DIR = DDTBBE_ICodFrmSynSet/moc
RCC_DIR = DDTBBE_ICodFrmSynSet/rcc
UI_DIR = DDTBBE_ICodFrmSynSet/ui
OBJECTS_DIR = DDTBBE_ICodFrmSynSet/obj
DESTDIR += ../../
