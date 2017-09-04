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
    src/USBBBE_PMModuSetGUI.cpp \
    src/USBBBE_PMModuSetGUIDlg.cpp \
    src/USBBBE_PMModuSetGUIExport.cpp \
    ../Common/DeviceMap.cpp \
    ../Common/Log.cpp

HEADERS += \
    src/USBBBE_PMModuSetGUI.h \
    src/USBBBE_PMModuSetGUI_global.h \
    src/USBBBE_PMModuSetGUIDlg.h \
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
    src/USBBBE_PMModuSetGUIDlg.ui

MOC_DIR = USBBBE_PMModuSet/moc
RCC_DIR = USBBBE_PMModuSet/rcc
UI_DIR = USBBBE_PMModuSet/ui
OBJECTS_DIR = USBBBE_PMModuSet/obj
DESTDIR += ../../
