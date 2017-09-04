#-------------------------------------------------
#
# Project created by QtCreator 2015-06-24 上午11:12
#
#-------------------------------------------------

QT       += network xml

QT       += gui
QT       += widgets


TARGET = SS1BBE_ModuParamSet
TEMPLATE = lib

DEFINES += SS1BBE_MODUPARAMSETGUISHARED_LIBRARY

SOURCES += \
    src/SS1BBE_ModuParamSetGUI.cpp \
    src/SS1BBE_ModuParamSetGUIDlg.cpp \
    src/SS1BBE_ModuParamSetGUIExport.cpp \
    ../Common/DeviceMap.cpp \
    ../Common/Log.cpp \
    ../Common/CommonFunction.cpp

HEADERS += \
    src/SS1BBE_ModuParamSetGUI.h \
    src/SS1BBE_ModuParamSetGUI_global.h \
    src/SS1BBE_ModuParamSetGUIDlg.h \
    ../Common/DataStruct2311.h \
    ../Common/Dcu_common_DataStruct.h \
    ../Common/LxTsspCommonDefinition.h \
    ../Common/TypeDefine.h \
    ../Common/DeviceMap.h \
    ../Common/Log.h \
    ../Common/SMCU_BBE_CDMAStruct.h \
    ../Common/ss1trlResultReport.h \
    ../Common/CommonFunction.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

FORMS += \
    src/SS1BBE_ModuParamSetGUIDlg.ui

MOC_DIR = SS1BBE_ModuParamSet/moc
RCC_DIR = SS1BBE_ModuParamSet/rcc
UI_DIR = SS1BBE_ModuParamSet/ui
OBJECTS_DIR = SS1BBE_ModuParamSet/obj
DESTDIR += ../../
