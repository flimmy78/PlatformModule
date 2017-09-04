#-------------------------------------------------
#
# Project created by QtCreator 2015-06-24 上午11:12
#
#-------------------------------------------------

QT       += network xml
QT       += gui
QT       += widgets


TARGET = SS2BBE_ModuParamSet
TEMPLATE = lib

DEFINES += SS2BBE_MODUPARAMSETGUISHARED_LIBRARY

SOURCES += \
    ../Common/DeviceMap.cpp \
    ../Common/Log.cpp \
    src/SS2BBE_ModuParamSetGUI.cpp \
    src/SS2BBE_ModuParamSetGUIDlg.cpp \
    src/SS2BBE_ModuParamSetGUIExport.cpp \
    ../Common/CommonFunction.cpp

HEADERS += \
    ../Common/DataStruct2311.h \
    ../Common/Dcu_common_DataStruct.h \
    ../Common/LxTsspCommonDefinition.h \
    ../Common/TypeDefine.h \
    ../Common/DeviceMap.h \
    ../Common/Log.h \
    ../Common/SMCUHeadFile.h \
    src/SS2BBE_ModuParamSetGUI.h \
    src/SS2BBE_ModuParamSetGUI_global.h \
    src/SS2BBE_ModuParamSetGUIDlg.h \
    ../Common/SMCUSSB2_BBE_CDMAStruct.h \
    ../Common/CommonFunction.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

FORMS += \
    src/SS2BBE_ModuParamSetGUIDlg.ui

MOC_DIR = SS2BBE_ModuParamSet/moc
RCC_DIR = SS2BBE_ModuParamSet/rcc
UI_DIR = SS2BBE_ModuParamSet/ui
OBJECTS_DIR = SS2BBE_ModuParamSet/obj
DESTDIR += ../../
