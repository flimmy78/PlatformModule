#-------------------------------------------------
#
# Project created by QtCreator 2015-06-24 上午11:12
#
#-------------------------------------------------

QT       += network xml
QT       += gui
QT       += widgets


TARGET = USBBBE_CJParaSet
TEMPLATE = lib

DEFINES += USBBBE_CJPARASETGUISHARED_LIBRARY

SOURCES += \
    USBBBE_CJParaSetGUI.cpp \
    USBBBE_CJParaSetGUIDlg.cpp \
    USBBBE_CJParaSetGUIExport.cpp \
    ../../Common/DeviceMap.cpp \
    ../../Common/Log.cpp

HEADERS += \
    USBBBE_CJParaSetGUI.h \
    USBBBE_CJParaSetGUI_global.h \
    USBBBE_CJParaSetGUIDlg.h \
    ../../Common/DataStruct2311.h \
    ../../Common/Dcu_common_DataStruct.h \
    ../../Common/LxTsspCommonDefinition.h \
    ../../Common/MCSDataStruct.h \
    .../../Common/TypeDefine.h \
    ../../Common/DeviceMap.h \
    ../../Common/Log.h \
    ../../Common/MCSUSBDataStruct.h \
    ../../Common/SMCU_BBE_CDMAStruct.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

FORMS += \
    USBBBE_CJParaSetGUIDlg.ui

MOC_DIR = USBBBE_CJParaSet/moc
RCC_DIR = USBBBE_CJParaSet/rcc
UI_DIR = USBBBE_CJParaSet/ui
OBJECTS_DIR = USBBBE_CJParaSet/obj