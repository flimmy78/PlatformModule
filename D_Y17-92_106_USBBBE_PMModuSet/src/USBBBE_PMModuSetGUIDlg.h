#ifndef USBBBE_PMMODUSETGUIDLG_H
#define USBBBE_PMMODUSETGUIDLG_H

#include <QDialog>
#include "USBBBE_PMModuSetGUI.h"
#include "../../Common/DeviceMap.h"
#include "../../Common/ddtTTCCtrlResultReport.h"
#include <QMessageBox>
namespace Ui {
class USBBBE_PMModuSetGUIDlg;
}

class USBBBE_PMModuSetGUIDlg : public QDialog
{
    Q_OBJECT

public:
    explicit USBBBE_PMModuSetGUIDlg(QWidget *parent = 0);
    QSize sizeHint() const;
    ~USBBBE_PMModuSetGUIDlg();

protected:
    void closeEvent(QCloseEvent *event);
    void timerEvent(QTimerEvent *event);

private:
    Ui::USBBBE_PMModuSetGUIDlg *ui;

    QString m_strTitle;
    TLxTsspObjectInfo m_ObjectInfo;
    ILxTsspLogManager* m_pILxTsspLogManager;

    MCSUSBUpModulParamStruct m_MCSUSBUpModulParam;       //参数

    MCSUSBUpModulStatusStruct* m_MCSUSBUpModulStatus;       //状态

    ParaStruct m_DlgParaSetStruct[3][60];  //用于存放各个参数参数描述信息
    QString m_DlgMapMean[50][30];       //用于保存map映射类数据项的含义值，往控件中写入
    long DlgMapLineNum[50];             //用于保存各个map映射类数据项的行数（即映射项数）。(数组长度可根据xml文件中map类型的个数自定义)

    ILxTsspHAManager* m_pHAManager;//设备管理对象

    ILxTsspStateManager*  m_pStateMgr;
    ILxTsspMessageManager* m_pILxTsspMessageManager;
    ILxTsspPublicInfoManager* m_pPublicInfo;

    quint64 m_ParaMCSUSBUpModulID;          //参数ID

    quint64 m_StatusParaMCSUSBUpModulID;    //状态ID
    quint64 m_ctlCmd;
    PCRDATATRANS_TTC* m_pCtlCmd;
    int m_TimerID;
    int iParaSetFlag;       //参数设置标志，0表示参数设置,1表示宏编辑
    CDeviceMap m_DeviceMap;
    unsigned short m_usC;
    unsigned short m_usTID;
    SUBITEM m_sItem;

public:
    //设置标题 传倒信息
    void setText(const QString& strTitle,ParaStruct xmlsetText[3][60],QString MapMean[50][30],int MapNum[50],uchar Type,ILxTsspParameterMacro* pMacroObj);
    //初始化对话框
    void InitDLG(int Type,ILxTsspParameterMacro* pMacroObj);
    //设置输入格式
    void InitInput();
    //显示变量及范围
    void ShowControls();
    //使能或不使能控件
    void EnabledControls(bool bFlag);
    void EnabledControlsTeleS(bool bFlag);//遥控
    void EnabledControlsTeleCtrl(bool bFlag);//话音
    void EnabledControlsTeleSourceS(bool bFlag);//遥测模拟元
    void EnabledControls12(bool bFlag);//遥测
    void EnabledControlsDoppler(bool bFlag);//多普勒
    bool m_doppler;
    void updateDopplerValue();
    //显示参数
    void ShowPara(int type);
    //设置对象信息
    void SetObjInfo(TLxTsspObjectInfo ObjectInfo);
    //保存参数宏
    int SaveToMacro(ILxTsspParameterMacro* pMacroObj);
    //从界面取参数
    void GetPara();
    //验证参数范围
    int CheckPara();
    //参数设置处理
    void EnabledControlsParamSet(bool enable);
    //有无副帧
    void SubFrmEn(bool bFlag);
    //有无ID副帧
    void IDSubFrmEn(bool bFlag);
    //有无循环副帧
    void CycSubFrmEn(bool bFlag);
    //有无反码副帧
    void ReserveSubFrmEn(bool bFlag);
    //设置帧长范围
    void SetFrameLenRange(int iWordLen);
    //16进制辅助函数
    DWORDLONG StrToSynCode(const char * pStr);
    void SendMessage(char* pData, USHORT usLen, USHORT usC, USHORT usTID, USHORT usSN = 0);
    int msgBoxFunction(QMessageBox &msgBox, QString msg, BOOL ret);

private slots:
    void on_pushButton_Modify_clicked();
    void on_pushButton_Set_clicked();
    void on_pushButton_TeleSModify_clicked();
    void on_pushButton_TeleSSetting_clicked();
    void on_pushButton_TeleCtrlModify_clicked();
    void on_pushButton_TeleCtrlSetting_clicked();
    void on_pushButton_TeleSourceSModify_clicked();
    void on_pushButton_TeleSourceSSetting_clicked();
    void on_pushButton_12_clicked();
    void on_pushButton_pushButton_TeleSourceASetting_clicked();
    void on_pushButton_DopplerModify_clicked();
    void on_pushButton_DopplerSetting_clicked();
    void on_comboBox_VoiceSourceOutput_currentIndexChanged(int index);
    void on_comboBox_AnologSwitch_currentIndexChanged(int index);
    void on_radioButton_tcStop_clicked();
};

#endif // USBBBE_PMMODUSETGUIDLG_H

