#-------------------------------------------------
#
# Project created by QtCreator 2017-02-24 上午11:12
#
#-------------------------------------------------

QT       += network xml

QT       += gui
QT       += widgets

TARGET = UPNet_Switch
TEMPLATE = lib

DEFINES += UPNET_SWITCHGUISHARED_LIBRARY

SOURCES += src/UPNet_SwitchGUI.cpp \
    src/UPNet_SwitchGUIDlg.cpp \
    src/UPNet_SwitchGUIExport.cpp \
    src/XmlReader.cpp \
    src/shape/arrowlabel.cpp \
    src/shape/dotlabel.cpp \
    src/shape/drawlabel.cpp \
    src/shape/linelabel.cpp \
    src/shape/Matrix.cpp \
    src/shape/signalitem.cpp \
    src/shape/menubutton.cpp \
    ../Common/DeviceMap.cpp \
    ../Common/Log.cpp \
    ../Common/CommonFunction.cpp

HEADERS +=  \
    src/UPNet_SwitchGUI.h \
    src/UPNet_SwitchGUI_global.h \
    src/UPNet_SwitchGUIDlg.h \
    src/XmlReader.h \
    src/shape/arrowlabel.h \
    src/shape/dotlabel.h \
    src/shape/drawlabel.h \
    src/shape/linelabel.h \
    src/shape/Matrix.h \
    src/shape/signalitem.h \
    src/shape/menubutton.h \
    ../Common/DeviceMap.h \
    ../Common/DataStruct2311.h \
    ../Common/Dcu_common_DataStruct.h \
    ../Common/LxTsspCommonDefinition.h \
    ../Common/TypeDefine.h \
    ../Common/Log.h \
    ../Common/CommonFunction.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

FORMS += src/UPNet_SwitchGUIDlg.ui

MOC_DIR = UPNet_Switch/moc
RCC_DIR = UPNet_Switch/rcc
UI_DIR = UPNet_Switch/ui
OBJECTS_DIR = UPNet_Switch/obj

DESTDIR = ../../
