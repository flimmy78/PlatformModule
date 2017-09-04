#-------------------------------------------------
#
# Project created by QtCreator 2015-06-24 上午11:12
#
#-------------------------------------------------

QT       += network xml
QT       += gui
QT       += widgets


TARGET = DDTBBE_ModSourceSet
TEMPLATE = lib
DEFINES += DDTBBE_MODSOURCESETGUI_LIBRARY

SOURCES += src/DDTBBE_ModSourceSetGUIDlg.cpp \
    src/DDTBBE_ModSourceSetGUIExport.cpp \
    src/DDTBBE_ModSourceSetGUI.cpp \
    ../Common/DeviceMap.cpp \
    ../Common/Log.cpp \
    src/checkPara.cpp

HEADERS += \
    src/DDTBBE_ModSourceSetGUIDlg.h \
    src/DDTBBE_ModSourceSetGUI.h \
    src/DDTBBE_ModSourceSetGUI_global.h \
    ../Common/DataStruct2311.h \
    ../Common/DeviceMap.h \
    ../Common/Log.h \
    ../Common/LxTsspCommonDefinition.h \
    ../Common/SMCUHeadFile.h \
    ../Common/TypeDefine.h \
    ../Common/Log.h \
    ../Common/MCSDataStruct.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

FORMS += \
    src/DDTBBE_ModSourceSetGUIDlg.ui

MOC_DIR = DDTBBE_ModSourceSet/moc
RCC_DIR = DDTBBE_ModSourceSet/rcc
UI_DIR = DDTBBE_ModSourceSet/ui
OBJECTS_DIR = DDTBBE_ModSourceSet/obj
DESTDIR += ../../