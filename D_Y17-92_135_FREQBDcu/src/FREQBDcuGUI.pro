#-------------------------------------------------
#
# Project created by QtCreator 2017-02-24 上午11:12
#
#-------------------------------------------------

QT       += network xml

QT       += gui
QT       += widgets


TARGET = FREQBDcu
TEMPLATE = lib

DEFINES += FREQBDCUGUISHARED_LIBRARY

SOURCES += FREQBDcuGUI.cpp \
    FREQBDcuGUIDlg.cpp \
    FREQBDcuGUIExport.cpp \
    XmlReader.cpp \
    ../../Common/DeviceMap.cpp \
    ../../Common/Log.cpp \
    ../../Common/GroupSwitch.cpp \
    ../../Common/CommonFunction.cpp

HEADERS +=  \
    TypeDefine.h \
    FREQBDcuGUI.h \
    FREQBDcuGUI_global.h \
    FREQBDcuGUIDlg.h \
    XmlReader.h \
    ../../Common/DeviceMap.h \
    ../../Common/DataStruct2311.h \
    ../../Common/Dcu_common_DataStruct.h \
    ../../Common/LxTsspCommonDefinition.h \
    ../../Common/TypeDefine.h \
    ../../Common/Log.h \
    ../../Common/GroupSwitch.h \
    ../../Common/CommonFunction.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

FORMS += FREQBDcuGUIDlg.ui

MOC_DIR = FREQBDcu/moc
RCC_DIR = FREQBDcu/rcc
UI_DIR = FREQBDcu/ui
OBJECTS_DIR = FREQBDcu/obj
