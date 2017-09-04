#-------------------------------------------------
#
# Project created by QtCreator 2015-06-24 上午11:12
#
#-------------------------------------------------

QT       += network xml
QT       += gui
QT       += widgets


TARGET = SS1PNCodeSet
TEMPLATE = lib

DEFINES += SS1PNCODESETGUISHARED_LIBRARY

SOURCES += \
    SS1PNCodeSetGUI.cpp \
    SS1PNCodeSetGUIDlg.cpp \
    SS1PNCodeSetGUIExport.cpp \
    ../../Common/DeviceMap.cpp \
    ../../Common/Log.cpp \
    ../../Common/CommonFunction.cpp

HEADERS += \
    SS1PNCodeSetGUI.h \
    SS1PNCodeSetGUI_global.h \
    SS1PNCodeSetGUIDlg.h \
    ../../Common/DataStruct2311.h \
    ../../Common/Dcu_common_DataStruct.h \
    ../../Common/LxTsspCommonDefinition.h \
    ../../Common/MCSDataStruct.h \
    ../../Common/TypeDefine.h \
    ../../Common/DeviceMap.h \
    ../../Common/Log.h \
    ../../Common/SMCUHeadFile.h \
    ../../Common/SMCU_BBE_CDMAStruct.h \
    ../../Common/CommonFunction.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

FORMS += \
    SS1PNCodeSetGUIDlg.ui

MOC_DIR = SS1PNCodeSet/moc
RCC_DIR = SS1PNCodeSet/rcc
UI_DIR = SS1PNCodeSet/ui
OBJECTS_DIR = SS1PNCodeSet/obj
