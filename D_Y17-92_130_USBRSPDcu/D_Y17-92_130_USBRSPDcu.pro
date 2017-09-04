#-------------------------------------------------
#
# Project created by QtCreator 2017-04-07 上午09:30
#
#-------------------------------------------------

QT       += network xml

QT       += gui
QT       += widgets


TARGET = USBRSPDcu
TEMPLATE = lib

DEFINES += USBRSPDCUGUI_LIBRARY

SOURCES += src/USBRSPDcuGUI.cpp \
    src/USBRSPDcuGUIDlg.cpp \
    src/USBRSPDcuGUIExport.cpp \
    ../Common/Log.cpp \
    ../Common/DeviceMap.cpp \
    ../Common/CommonFunction.cpp

HEADERS += \
    src/USBRSPDcuGUI.h \
    src/USBRSPDcuGUI_global.h \
    src/USBRSPDcuGUIDlg.h \
    ../Common/DataStruct2311.h \
    ../Common/Dcu_common_DataStruct.h \
    ../Common/LxTsspCommonDefinition.h \
    ../Common/MCSDataStruct.h \
    ../Common/TypeDefine.h \
    ../Common/Log.h \
    ../Common/DeviceMap.h \
    src/USBRSPDcuGUIDlg.h \
    ../Common/CommonFunction.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

FORMS += src/USBRSPDcuGUIDlg.ui

MOC_DIR = USBRSPDcu/moc
RCC_DIR = USBRSPDcu/rcc
UI_DIR = USBRSPDcu/ui
OBJECTS_DIR = USBRSPDcu/obj

RESOURCES += \
    src/image.qrc
DESTDIR = ../../
