#-------------------------------------------------
#
# Project created by QtCreator 2017-01-24 上午11:12
#
#-------------------------------------------------

QT       += network xml
QT       += gui
QT       += widgets
TARGET =TMRemoteInterface
TEMPLATE = lib

DEFINES += CDevCtlInterfaceGUISHARED_LIBRARY

SOURCES += \
    Global.cpp \
    XmlReader.cpp \
    ProcessModule.cpp\
    ../../Common/DeviceMap.cpp \
    ../../Common/Log.cpp \
    cshowdatagram.cpp \
    util.cpp \
    connectstatus.cpp \
    platformresource.cpp \
    cbaseband.cpp \
    cddt.cpp \
    CSS1.cpp \
    CSS2.cpp \
    CTTC.cpp \
    devices.cpp \
    CTC.cpp \
    alter.cpp \
    adjustalter.cpp \
    TMRemoteInterfaceExport.cpp \
    TMRemoteInterfaceGUI.cpp \
    TMRemoteInterfaceGUIDlg.cpp \
    ../../Common/GroupSwitch.cpp \
    cdatasource.cpp \
    SearchStatusThread_tmremote.cpp \
    RecvDataThread_tmremote.cpp \
    searchmarc.cpp \
    CRM.cpp

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
    ../../Common/DeviceMap.h \
    ../../Common/TypeDefine.h \
    ../../Common/SMCUHeadFile.h \
    ../../Common/MCSDataStruct.h \
    ../../Common/LxTsspCommonDefinition.h \
    ../../Common/Dcu_common_DataStruct.h \
    ../../Common/DataStruct2311.h \
    ../../Common/Log.h \
    ../../Common/MCSUSBDataStruct.h \
    cshowdatagram.h \
    ../../Common/SMCUSSB2_BBE_CDMAStruct.h \
    util.h \
    connectstatus.h \
    ../../Common/ddtTTCCtrlResultReport.h \
    ../../Common/ss1trlResultReport.h \
    ../../Common/ss2trlResultReport.h \
    platformresource.h \
    cbaseband.h \
    cddt.h \
    CSS1.h \
    CSS2.h \
    CTTC.h \
    devices.h \
    CTC.h \
    alter.h \
    adjustalter.h \
    DataStructTTCRemoteInterface.h \
    TMRemoteInterface_global.h \
    TMRemoteInterfaceeGUI.h \
    TMRemoteInterfaceGUIDlg.h \
    ../../Common/DataStruct2311.h \
    ../../Common/Dcu_common_DataStruct.h \
    ../../Common/ddtTTCCtrlResultReport.h \
    ../../Common/DeviceMap.h \
    ../../Common/GroupSwitch.h \
    ../../Common/Log.h \
    ../../Common/LxTsspCommonDefinition.h \
    ../../Common/MCSDataStruct.h \
    ../../Common/MCSUSBDataStruct.h \
    ../../Common/SMCU_BBE_CDMAStruct.h \
    ../../Common/SMCUHeadFile.h \
    ../../Common/SMCUSSB2_BBE_CDMAStruct.h \
    ../../Common/ss1trlResultReport.h \
    ../../Common/ss2trlResultReport.h \
    ../../Common/TypeDefine.h \
    TMRemoteInterfaceGUI.h \
    ../../Common/interfaces/cdatasource.h \
    ../../Common/const.h \
    SearchStatusThread_tmremote.h \
    RecvDataThread_tmremote.h \
    searchmarc.h \
    CRM.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

FORMS += \
    TMRemoteInterfaceGUIDlg.ui

MOC_DIR = TMRemoteInterface/moc
RCC_DIR = TMRemoteInterface/rcc
UI_DIR = TMRemoteInterface/ui
OBJECTS_DIR = TMRemoteInterface/obj

DISTFILES += \
    TMRemoteInterfaceGUI.png \
    TMRemoteInterface.pro.user
