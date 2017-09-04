#-------------------------------------------------
#
# Project created by QtCreator 2015-06-24 上午11:12
#
#-------------------------------------------------

QT       += network xml
QT       += gui
QT       += widgets


TARGET = SS2BBE_OutPutSelSet
TEMPLATE = lib

DEFINES += SS2BBE_OUTPUTSELSETGUISHARED_LIBRARY

SOURCES += \
    ../Common/DeviceMap.cpp \
    ../Common/Log.cpp \
    src/SS2BBE_OutPutSelSetGUI.cpp \
    src/SS2BBE_OutPutSelSetGUIDlg.cpp \
    src/SS2BBE_OutPutSelSetGUIExport.cpp \
    ../Common/CommonFunction.cpp

HEADERS += \
    ../Common/DataStruct2311.h \
    ../Common/Dcu_common_DataStruct.h \
    ../Common/LxTsspCommonDefinition.h \
    ../Common/MCSDataStruct.h \
    ../Common/TypeDefine.h \
    ../Common/DeviceMap.h \
    ../Common/Log.h \
    ../Common/SMCU_BBE_CDMAStruct.h \
    src/SS2BBE_OutPutSelSetGUI.h \
    src/SS2BBE_OutPutSelSetGUI_global.h \
    src/SS2BBE_OutPutSelSetGUIDlg.h \
    ../Common/CommonFunction.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

FORMS += \
    src/SS2BBE_OutPutSelSetGUIDlg.ui

MOC_DIR = SS2BBE_OutPutSelSet/moc
RCC_DIR = SS2BBE_OutPutSelSet/rcc
UI_DIR = SS2BBE_OutPutSelSet/ui
OBJECTS_DIR = SS2BBE_OutPutSelSet/obj
DESTDIR += ../../
