#-------------------------------------------------
#
# Project created by QtCreator 2017-03-24 上午15:17
#
#-------------------------------------------------

QT       += network xml

QT       += gui
QT       += widgets


TARGET = SswitchDcu
TEMPLATE = lib

DEFINES += SSWITCHDCUGUI_LIBRARY

SOURCES += SswitchDcuGUI.cpp \
    SswitchDcuGUIDlg.cpp \
    SswitchDcuGUIExport.cpp \
    XmlReader.cpp \
    ../../Common/DeviceMap.cpp \
    ../../Common/Log.cpp \
    ../../Common/GroupSwitch.cpp \
    ../../Common/CommonFunction.cpp

HEADERS +=  \
    TypeDefine.h \
    SswitchDcuGUI.h \
    SswitchDcuGUI_global.h \
    SswitchDcuGUIDlg.h \
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

FORMS += SswitchDcuGUIDlg.ui

MOC_DIR = SswitchDcu/moc
RCC_DIR = SswitchDcu/rcc
UI_DIR = SswitchDcu/ui
OBJECTS_DIR = SswitchDcu/obj
