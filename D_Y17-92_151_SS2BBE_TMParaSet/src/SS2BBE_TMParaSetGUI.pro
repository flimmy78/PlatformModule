#-------------------------------------------------
#
# Project created by QtCreator 2015-06-24 上午11:12
#
#-------------------------------------------------

QT       += network xml
QT       += gui
QT       += widgets


TARGET = SS2BBE_TMParaSet
TEMPLATE = lib

DEFINES += SS2BBE_TMPARASETGUI_LIBRARY

SOURCES += \
    ../Common/DeviceMap.cpp \
    ../Common/Log.cpp \
    SS2BBE_TMParaSetGUI.cpp \
    SS2BBE_TMParaSetGUIDlg.cpp \
    SS2BBE_TMParaSetGUIExport.cpp \
    checkParam.cpp \
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
    ../Common/SMCU_BBE_CDMAStruct.h \
    SS2BBE_TMParaSetGUI.h \
    SS2BBE_TMParaSetGUIDlg.h \
    SS2BBE_TMParaSetGUI_global.h \
    ../Common/CommonFunction.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

FORMS += \
    SS2BBE_TMParaSetGUIDlg.ui

MOC_DIR = SS2BBE_TMParaSet/moc
RCC_DIR = SS2BBE_TMParaSet/rcc
UI_DIR = SS2BBE_TMParaSet/ui
OBJECTS_DIR = SS2BBE_TMParaSet/obj
