#-------------------------------------------------
#
# Project created by QtCreator 2015-06-24 上午11:12
#
#-------------------------------------------------

QT       += network xml

QT       += gui
QT       += widgets


TARGET = USBBBE_CodeTMParaSet
TEMPLATE = lib

DEFINES += USBBBE_CODETMPARASETGUISHARED_LIBRARY

SOURCES += \
    USBBBE_CodeTMParaSetGUI.cpp \
    USBBBE_CodeTMParaSetGUIDlg.cpp \
    USBBBE_CodeTMParaSetGUIExport.cpp \
    ../../Common/DeviceMap.cpp \
    ../../Common/Log.cpp

HEADERS += \
    USBBBE_CodeTMParaSetGUI.h \
    USBBBE_CodeTMParaSetGUI_global.h \
    USBBBE_CodeTMParaSetGUIDlg.h \
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
    USBBBE_CodeTMParaSetGUIDlg.ui

MOC_DIR = USBBBE_CodeTMParaSetGUI/moc
RCC_DIR = USBBBE_CodeTMParaSetGUI/rcc
UI_DIR = USBBBE_CodeTMParaSetGUI/ui
OBJECTS_DIR = USBBBE_CodeTMParaSetGUI/obj
