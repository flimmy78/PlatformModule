#-------------------------------------------------
#
# Project created by QtCreator 2017-01-24 上午11:12
#
#-------------------------------------------------

QT       += network xml
QT       += gui
QT       += widgets
TARGET = DevCtlInterface
TEMPLATE = lib

DEFINES += TTCDEVCTLINTERFACEGUISHARED_LIBRARY

SOURCES += \
    Global.cpp \
    XmlReader.cpp \
    DevCtlInterfaceExport.cpp \
    DevCtlInterfaceGUIDlg.cpp \
    DevCtlInterfaceGUI.cpp \
    ProcessModule.cpp\
    ../Common/DeviceMap.cpp \
    ../Common/Log.cpp \
    SearchStatusThread.cpp \
    RecvDataThread.cpp \
    cshowdatagram.cpp \
    util.cpp \
    connectstatus.cpp

HEADERS += \
    LxTsspCommonDefinition.h \
    TypeDefine.h \
    MCSDataStruct.h \
    Global.h \
    XmlReader.h \
    DevCtlInterface_global.h \
    DevCtlInterfaceGUIDlg.h \
    DevCtlInterfaceGUI.h \
    DataStructTTCDevCtlInterface.h \
    ProcessModule.h \
    SMCUHeadFile.h \
    ../Common/DeviceMap.h \
    ../Common/TypeDefine.h \
    ../Common/SMCUHeadFile.h \
    ../Common/MCSDataStruct.h \
    ../Common/LxTsspCommonDefinition.h \
    ../Common/Dcu_common_DataStruct.h \
    ../Common/DataStruct2311.h \
    ../Common/Log.h \
    SearchStatusThread.h \
    ../Common/MCSUSBDataStruct.h \
    RecvDataThread.h \
    cshowdatagram.h \
    ../Common/SMCUSSB2_BBE_CDMAStruct.h \
    util.h \
    connectstatus.h \
    ../Common/ddtTTCCtrlResultReport.h \
    ../Common/ss1trlResultReport.h \
    ../Common/ss2trlResultReport.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

FORMS += \
    DevCtlInterfaceGUIDlg.ui

MOC_DIR = DevCtlInterface/moc
RCC_DIR = DevCtlInterface/rcc
UI_DIR = DevCtlInterface/ui
OBJECTS_DIR = DevCtlInterface/obj
