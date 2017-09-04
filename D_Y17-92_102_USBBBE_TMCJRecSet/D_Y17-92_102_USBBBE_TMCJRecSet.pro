#-------------------------------------------------
#
# Project created by QtCreator 2015-06-24 上午11:12
#
#-------------------------------------------------

QT       += network xml
QT       += gui
QT       += widgets


TARGET = USBBBE_TMCJRecSet
TEMPLATE = lib

DEFINES += USBBBE_TMCJRECSETGUISHARED_LIBRARY

SOURCES += src/USBBBE_TMCJRecSetGUIExport.cpp \
    src/USBBBE_TMCJRecSetGUIDlg.cpp \
    src/USBBBE_TMCJRecSetGUI.cpp \
    ../Common/DeviceMap.cpp \
    ../Common/Log.cpp

HEADERS += \
    src/USBBBE_TMCJRecSetGUI.h \
    src/USBBBE_TMCJRecSetGUI_global.h \
    src/USBBBE_TMCJRecSetGUIDlg.h \
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
    src/USBBBE_TMCJRecSetGUIDlg.ui
	
MOC_DIR = USBBBE_TMCJRecSet/moc
RCC_DIR = USBBBE_TMCJRecSet/rcc
UI_DIR = USBBBE_TMCJRecSet/ui
OBJECTS_DIR = USBBBE_TMCJRecSet/obj
DESTDIR += ../../

