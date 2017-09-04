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

SOURCES += src/SswitchDcuGUI.cpp \
    src/SswitchDcuGUIDlg.cpp \
    src/SswitchDcuGUIExport.cpp \
    src/XmlReader.cpp \
    ../Common/DeviceMap.cpp \
    ../Common/Log.cpp \
    ../Common/GroupSwitch.cpp \
    ../Common/CommonFunction.cpp

HEADERS +=  \
    src/TypeDefine.h \
    src/SswitchDcuGUI.h \
    src/SswitchDcuGUI_global.h \
    src/SswitchDcuGUIDlg.h \
    src/XmlReader.h \
    ../Common/DeviceMap.h \
    ../Common/DataStruct2311.h \
    ../Common/Dcu_common_DataStruct.h \
    ../Common/LxTsspCommonDefinition.h \
    ../Common/TypeDefine.h \
    ../Common/Log.h \
    ../Common/GroupSwitch.h \
    ../Common/CommonFunction.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

FORMS += src/SswitchDcuGUIDlg.ui

MOC_DIR = SswitchDcu/moc
RCC_DIR = SswitchDcu/rcc
UI_DIR = SswitchDcu/ui
OBJECTS_DIR = SswitchDcu/obj

DESTDIR = ../../
