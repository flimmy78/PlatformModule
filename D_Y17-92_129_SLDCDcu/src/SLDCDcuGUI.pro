#-------------------------------------------------
#
# Project created by QtCreator 2017-02-24 上午11:12
#
#-------------------------------------------------

QT       += network xml

QT       += gui
QT       += widgets


TARGET = SLDCDcu
TEMPLATE = lib

DEFINES += SLDCDCUGUISHARED_LIBRARY

SOURCES += SLDCDcuGUI.cpp \
    SLDCDcuGUIDlg.cpp \
    SLDCDcuGUIExport.cpp \
    XmlReader.cpp \
    ../../Common/DeviceMap.cpp \
    ../../Common/Log.cpp \
    ../../Common/GroupSwitch.cpp \
    ../../Common/CommonFunction.cpp

HEADERS +=  \
    TypeDefine.h \
    SLDCDcuGUI.h \
    SLDCDcuGUI_global.h \
    SLDCDcuGUIDlg.h \
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

FORMS += SLDCDcuGUIDlg.ui

MOC_DIR = SLDCDcu/moc
RCC_DIR = SLDCDcu/rcc
UI_DIR = SLDCDcu/ui
OBJECTS_DIR = SLDCDcu/obj
