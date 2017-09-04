#-------------------------------------------------
#
# Project created by QtCreator 2015-06-24 上午11:12
#
#-------------------------------------------------

QT       += network xml

QT       += gui
QT       += widgets


TARGET = USBBBE_PMModuSet
TEMPLATE = lib

DEFINES += DDTBBE_PMMODUSETGUISHARED_LIBRARY

SOURCES += \
    USBBBE_PMModuSetGUI.cpp \
    USBBBE_PMModuSetGUIDlg.cpp \
    USBBBE_PMModuSetGUIExport.cpp \
    ../Common/DeviceMap.cpp \
    ../Common/Log.cpp

HEADERS += \
    USBBBE_PMModuSetGUI.h \
    USBBBE_PMModuSetGUI_global.h \
    USBBBE_PMModuSetGUIDlg.h \
    ../Common/DataStruct2311.h \
    ../Common/Dcu_common_DataStruct.h \
    ../Common/LxTsspCommonDefinition.h \
    ../Common/MCSDataStruct.h \
    ../Common/TypeDefine.h \
    ../Common/DeviceMap.h \
    ../Common/Log.h \
    ../Common/MCSUSBDataStruct.h \
    ../Common/ddtTTCCtrlResultReport.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

FORMS += \
    USBBBE_PMModuSetGUIDlg.ui

MOC_DIR = USBBBE_PMModuSet/moc
RCC_DIR = USBBBE_PMModuSet/rcc
UI_DIR = USBBBE_PMModuSet/ui
OBJECTS_DIR = USBBBE_PMModuSet/obj
