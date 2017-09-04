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
    src/USBBBE_CodeTMParaSetGUI.cpp \
    src/USBBBE_CodeTMParaSetGUIDlg.cpp \
    src/USBBBE_CodeTMParaSetGUIExport.cpp \
    ../Common/DeviceMap.cpp \
    ../Common/Log.cpp

HEADERS += \
    src/USBBBE_CodeTMParaSetGUI.h \
    src/USBBBE_CodeTMParaSetGUI_global.h \
    src/USBBBE_CodeTMParaSetGUIDlg.h \
    ../Common/DataStruct2311.h \
    ../Common/Dcu_common_DataStruct.h \
    ../Common/LxTsspCommonDefinition.h \
    ../Common/MCSDataStruct.h \
    ../Common/TypeDefine.h \
    ../Common/DeviceMap.h \
    ../Common/Log.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

FORMS += \
    src/USBBBE_CodeTMParaSetGUIDlg.ui

MOC_DIR = USBBBE_CodeTMParaSetGUI/moc
RCC_DIR = USBBBE_CodeTMParaSetGUI/rcc
UI_DIR = USBBBE_CodeTMParaSetGUI/ui
OBJECTS_DIR = USBBBE_CodeTMParaSetGUI/obj
DESTDIR += ../
