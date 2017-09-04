#-------------------------------------------------
#
# Project created by QtCreator 2015-06-24 上午11:12
#
#-------------------------------------------------

QT       += network xml
QT       += gui
QT       += widgets


TARGET = SK_BBEDivCombaraSet
TEMPLATE = lib

DEFINES += BPSK_DEMODUSETGUISHARED_LIBRARY

SOURCES += src/SK_BBEDivCombaraSetGUI.cpp \
    src/SK_BBEDivCombaraSetGUIDlg.cpp \
    src/SK_BBEDivCombaraSetGUIExport.cpp \
    ../Common/Log.cpp \
    ../Common/DeviceMap.cpp

HEADERS += \
    src/SK_BBEDivCombaraSetGUI.h \
    src/SK_BBEDivCombaraSetGUI_global.h \
    src/SK_BBEDivCombaraSetGUIDlg.h \
    ../Common/DataStruct2311.h \
    ../Common/Dcu_common_DataStruct.h \
    ../Common/LxTsspCommonDefinition.h \
    ../Common/MCSDataStruct.h \
    ../Common/TypeDefine.h \
    ../Common/Log.h \
    ../Common/DeviceMap.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

FORMS += src/SK_BBEDivCombaraSetGUIDlg.ui

MOC_DIR = SK_BBEDivCombaraSet/moc
RCC_DIR = SK_BBEDivCombaraSet/rcc
UI_DIR = SK_BBEDivCombaraSet/ui
OBJECTS_DIR = SK_BBEDivCombaraSet/obj
DESTDIR += ../../
