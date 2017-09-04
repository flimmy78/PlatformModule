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
    src/SS2PNCodeSetGUI.cpp \
    src/SS2PNCodeSetGUIExport.cpp \
    src/SS2PNCodeSetGUIDlg.cpp \
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
    src/SS2PNCodeSetGUI.h \
    src/SS2PNCodeSetGUI_global.h \
    src/SS2PNCodeSetGUIDlg.h \
    ../Common/SMCUSSB2_BBE_CDMAStruct.h \
    ../Common/CommonFunction.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

FORMS += \
    src/SS2PNCodeSetGUIDlg.ui

MOC_DIR = SS2PNCodeSet/moc
RCC_DIR = SS2PNCodeSet/rcc
UI_DIR = SS2PNCodeSet/ui
OBJECTS_DIR = SS2PNCodeSet/obj
DESTDIR += ../../
