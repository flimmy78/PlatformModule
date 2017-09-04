#-------------------------------------------------
#
# Project created by QtCreator 2015-06-24 上午11:12
#
#-------------------------------------------------

QT       += network xml
QT       += gui
QT       += widgets


TARGET = USBBBE_SysParaSet
TEMPLATE = lib

DEFINES += USBBBE_SYSPARASETGUISHARED_LIBRARY

SOURCES += \
    src/USBBBE_SysParaSetGUI.cpp \
    src/USBBBE_SysParaSetGUIDlg.cpp \
    src/USBBBE_SysParaSetGUIExport.cpp \
    ../Common/DeviceMap.cpp \
    ../Common/Log.cpp \
    ../Common/CommonFunction.cpp

HEADERS += \
    src/USBBBE_SysParaSetGUI.h \
    src/USBBBE_SysParaSetGUI_global.h \
    src/USBBBE_SysParaSetGUIDlg.h \
    ../Common/DataStruct2311.h \
    ../Common/LxTsspCommonDefinition.h \
    ../Common/MCSDataStruct.h \
    ../Common/TypeDefine.h \
    ../Common/Dcu_common_DataStruct.h \
    ../Common/DeviceMap.h \
    ../Common/Log.h \
    ../Common/MCSUSBDataStruct.h \
    ../Common/CommonFunction.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

FORMS += \
    src/USBBBE_SysParaSetGUIDlg.ui

MOC_DIR = USBBBE_SysParaSet/moc
RCC_DIR = USBBBE_SysParaSet/rcc
UI_DIR = USBBBE_SysParaSet/ui
OBJECTS_DIR = USBBBE_SysParaSet/obj
DESTDIR += ../../
