#-------------------------------------------------
#
# Project created by QtCreator 2015-06-24 上午11:12
#
#-------------------------------------------------

QT       += network xml

QT       += gui
QT       += widgets


TARGET = SS2PNCodeSet
TEMPLATE = lib

DEFINES += SS2PNCODESETGUISHARED_LIBRARY

SOURCES += \
    ../Common/DeviceMap.cpp \
    ../Common/Log.cpp \
    SS2PNCodeSetGUI.cpp \
    SS2PNCodeSetGUIExport.cpp \
    SS2PNCodeSetGUIDlg.cpp \
    ../Common/CommonFunction.cpp

HEADERS += \
    ../Common/DataStruct2311.h \
    ../Common/Dcu_common_DataStruct.h \
    ../Common/LxTsspCommonDefinition.h \
    ../Common/MCSDataStruct.h \
    ../Common/TypeDefine.h \
    ../Common/DeviceMap.h \
    ../Common/Log.h \
    ../Common/SMCUHeadFile.h \
    SS2PNCodeSetGUI.h \
    SS2PNCodeSetGUI_global.h \
    SS2PNCodeSetGUIDlg.h \
    ../Common/SMCUSSB2_BBE_CDMAStruct.h \
    ../Common/CommonFunction.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

FORMS += \
    SS2PNCodeSetGUIDlg.ui

MOC_DIR = SS2PNCodeSet/moc
RCC_DIR = SS2PNCodeSet/rcc
UI_DIR = SS2PNCodeSet/ui
OBJECTS_DIR = SS2PNCodeSet/obj
