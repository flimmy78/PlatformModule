#-------------------------------------------------
#
# Project created by QtCreator 2015-06-24 上午11:12
#
#-------------------------------------------------

QT       += network xml

QT       += gui
QT       += widgets


TARGET = USBBBE_ZKset
TEMPLATE = lib

DEFINES += USBBBE_ZKsetGUISHARED_LIBRARY

SOURCES += \
    src/USBBBE_ZKsetGUI.cpp \
    src/USBBBE_ZKsetGUIDlg.cpp \
    src/USBBBE_ZKsetGUIExport.cpp \
    ../Common/DeviceMap.cpp \
    ../Common/Log.cpp \
    src/checkparam.cpp

HEADERS += \
    src/USBBBE_ZKsetGUI.h \
    src/USBBBE_ZKsetGUI_global.h \
    src/USBBBE_ZKsetGUIDlg.h \
    ../Common/DataStruct2311.h \
    ../Common/Dcu_common_DataStruct.h \
    ../Common/LxTsspCommonDefinition.h \
    ../Common/MCSDataStruct.h \
    ../Common/TypeDefine.h \
    ../Common/DeviceMap.h \
    ../Common/Log.h \
    ../Common/MCSUSBDataStruct.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

FORMS += \
    src/USBBBE_ZKsetGUIDlg.ui

MOC_DIR = USBBBE_ZKset/moc
RCC_DIR = USBBBE_ZKset/rcc
UI_DIR = USBBBE_ZKset/ui
OBJECTS_DIR = USBBBE_ZKset/obj
DESTDIR += ../../
