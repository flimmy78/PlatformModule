#-------------------------------------------------
#
# Project created by QtCreator 2015-06-24 上午11:12
#
#-------------------------------------------------

QT       += network xml

QT       += gui
QT       += widgets


TARGET = USBBBE_CodeTMEmuSet
TEMPLATE = lib

DEFINES += USBBBE_CODETMEMUSETGUISHARED_LIBRARY

SOURCES += \
    USBBBE_CodeTMEmuSetGUI.cpp \
    USBBBE_CodeTMEmuSetGUIDlg.cpp \
    USBBBE_CodeTMEmuSetGUIExport.cpp \
    ../../Common/DeviceMap.cpp \
    ../../Common/Log.cpp \
    checkParam.cpp

HEADERS += \
    USBBBE_CodeTMEmuSetGUI.h \
    USBBBE_CodeTMEmuSetGUI_global.h \
    USBBBE_CodeTMEmuSetGUIDlg.h \
    ../../Common/DataStruct2311.h \
    ../../Common/Dcu_common_DataStruct.h \
    ../../Common/LxTsspCommonDefinition.h \
    ../../Common/MCSDataStruct.h \
    ../../Common/TypeDefine.h \
    ../../Common/DeviceMap.h \
    ../../Common/Log.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

FORMS += \
    USBBBE_CodeTMEmuSetGUIDlg.ui

MOC_DIR = USBBBE_CodeTMEmuSet/moc
RCC_DIR = USBBBE_CodeTMEmuSet/rcc
UI_DIR = USBBBE_CodeTMEmuSet/ui
OBJECTS_DIR = USBBBE_CodeTMEmuSet/obj
