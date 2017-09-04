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

SOURCES += USBRSPDcuGUI.cpp \
    USBRSPDcuGUIDlg.cpp \
    USBRSPDcuGUIExport.cpp \
    ../../Common/Log.cpp \
    ../../Common/DeviceMap.cpp \
    ../../Common/CommonFunction.cpp

HEADERS += \
    USBRSPDcuGUI.h \
    USBRSPDcuGUI_global.h \
    USBRSPDcuGUIDlg.h \
    ../../Common/DataStruct2311.h \
    ../../Common/Dcu_common_DataStruct.h \
    ../../Common/LxTsspCommonDefinition.h \
    ../../Common/MCSDataStruct.h \
    ../../Common/TypeDefine.h \
    ../../Common/Log.h \
    ../../Common/DeviceMap.h \
    USBRSPDcuGUIDlg.h \
    ../../Common/CommonFunction.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

FORMS += USBRSPDcuGUIDlg.ui

MOC_DIR = USBRSPDcu/moc
RCC_DIR = USBRSPDcu/rcc
UI_DIR = USBRSPDcu/ui
OBJECTS_DIR = USBRSPDcu/obj

RESOURCES += \
    image.qrc
