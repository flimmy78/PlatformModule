#ifndef DDTBBE_MODSOURCESETGUIDLG_H
#define DDTBBE_MODSOURCESETGUIDLG_H

#include <QDialog>
#include <QMessageBox>
#include "DDTBBE_ModSourceSetGUI.h"
#include "../../Common/DeviceMap.h"

namespace Ui {
class DDTBBE_ModSourceSetGUIDlg;
}

class DDTBBE_ModSourceSetGUIDlg : public QDialog
{
    Q_OBJECT

public:
    explicit DDTBBE_ModSourceSetGUIDlg(QWidget *parent = 0);
    QSize sizeHint() const;
    ~DDTBBE_ModSourceSetGUIDlg();
private:
    Ui::DDTBBE_ModSourceSetGUIDlg *ui;
protected:
    void closeEvent(QCloseEvent *event);
    void timerEvent(QTimerEvent *event);

    QString m_strTitle;
    TLxTsspObjectInfo m_ObjectInfo;

    ILxTsspLogManager* m_pILxTsspLogManager;

    MCSDDTTMSimulParamStruct m_MCSDDTTMSimulParam;       //模拟元参数

    MCSDDTTMSimulStatusStruct* m_MCSDDTTMSimulStatus;       //模拟元状态

    MCSDDTRevParamStruct m_MCSDDTRevParam;       //接收机参数

    MCSDDTRevStatusStruct* m_MCSDDTRevStatus;       //接收机状态

    MCSDDTTMIParamStruct m_MCSDDTTMIParam;       //数传解调器单元

    MCSDDTTMIStatusStruct* m_MCSDDTTMIParamStatus;       //数传解调器单元状态

    MCSDDTTMQParamStruct m_MCSDDTTMQParam;       //数传Q解调器单元

    MCSDDTTMQStatusStruct* m_MCSDDTTMQParamStatus;       //数传Q解调器单元状态

    ParaStruct m_DlgParaSetStruct[3][60];  //用于存放各个参数参数描述信息
    QString m_DlgMapMean[50][30];       //用于保存map映射类数据项的含义值，往控件中写入
    long DlgMapLineNum[50];             //用于保存各个map映射类数据项的行数（即映射项数）。(数组长度可根据xml文件中map类型的个数自定义)

    ILxTsspHAManager* m_pHAManager;//设备管理对象

    ILxTsspStateManager*  m_pStateMgr;
    ILxTsspMessageManager* m_pILxTsspMessageManager;
    ILxTsspPublicInfoManager* m_pPublicInfo;

    quint64 m_ParaMCSDDTTMSimulParamID;          //模拟元参数框架ID

    quint64 m_StatusParaMCSDDTTMSimulParamID;    //模拟元参数状态ID

    quint64 m_ParaMCSDDTRevParamID;          //接收机参数框架ID

    quint64 m_StatusParaMCSDDTRevParamID;    //接收机参数状态ID

    quint64 m_ParaMCSDDTTMIParamID;          //数传解调器单元ID

    quint64 m_StatusParaMCSDDTTMIParamID;    //数传解调器单元状态ID

    quint64 m_ParaMCSDDTTMQParamID;          //数传Q解调器单元ID

    quint64 m_StatusParaMCSDDTTMQParamID;    //数传Q解调器单元状态ID

    int m_TimerID;
    int iParaSetFlag;       //参数设置标志，0表示参数设置,1表示宏编辑

    CDeviceMap m_DeviceMap;

    unsigned short m_usC;            //信息代号
    unsigned short m_usTID;         //命令类型
    bool m_bCheckQ;
    bool m_autoMap;
    bool m_IQSwitch;
public:
    void relate();
    void showCurrentDlg();
    void showCurrentDlgSysParam();
    void showCurrentDlgI();
    void showCurrentDlgQ();
    void mudulationSystem();
    void noiseSwitch();
    void grayType();
    void dopplerSwitch();
    void bpsk();
    void qpsk();
    void sqpsk();
    void uqpsk();
    void powerFlag();
    void ioMethod();
    void leftTimeDelay();
    void rightTimeDelay();
    void dataMethod();
    void simulDataI();
    void simulDataQ();
    void codingTypeI();
    void randEnI();
    void codingTypeQ();
    void randEnQ();
    void subFrameSynTypeI();
    void subFrameSynTypeQ();
    void viterbiPhaseI();
    void viterbiPhaseQ();
    void rsCodingI();
    void rsCodingQ();
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
    int CheckParam();
    //根据副帧类型，使能副帧控件
    void EnabledControlsSubFrmType(int iSubFrm);
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
    void SendMessage(char* pData, unsigned short  usLen, unsigned short usC, unsigned short usTID);

    int msgBoxFunction(QMessageBox &msgBox, QString msg, BOOL ret);
    int FspLenIndexToValue(int nIndex);
    //单位字节{{
    bool checkIDSubFrm(int frmLen, int fspLen, int fspPos,int countPos);
    bool checkCycSubFrm(int frmLen, int fspLen, int fspPos, int subFspLen, int subFspPos);
    //}}
private slots:
    void on_pushButton_Setting_clicked();
    void on_pushButton_Modify_clicked();
    void on_comboBox_MudulationSystem_currentIndexChanged(int index);
    void on_comboBox_GRAYType_currentIndexChanged(int index);
    void on_comboBox_PowerFlag_currentIndexChanged(int index);
    void on_comboBox_IOMethod_currentIndexChanged(int index);
    void on_comboBox_DopplerSwitch_currentIndexChanged(int index);
    void on_comboBox_NoiseOutSwitch_currentIndexChanged(int index);
    void on_comboBox_counterClockCtrl_currentIndexChanged(int index);
    void on_comboBox_clockWiseCtrl_currentIndexChanged(int index);
    void on_comboBox_WayOfData_currentIndexChanged(int index);
    void on_comboBox_DataType_currentIndexChanged(int index);
    void on_comboBox_Enctype_currentIndexChanged(int index);
    void on_comboBox_ConcodeType_currentIndexChanged(int index);
    void on_comboBox_AddDisturbCtrl_currentIndexChanged(int index);
    void on_comboBox_SubFrameSyncType_currentIndexChanged(int index);
    void on_comboBox_ODataType_currentIndexChanged(int index);
    void on_comboBox_OEnctype_currentIndexChanged(int index);
    void on_comboBox_OConcodeType_currentIndexChanged(int index);
    void on_comboBox_OAddDisturbCtrl_currentIndexChanged(int index);
    void on_comboBox_OSubFrameSyncType_currentIndexChanged(int index);
    void on_pushButton_AutoMap_clicked();
    UCHAR getucSignalSel();
    int setIOMethod();

};

#endif // DDTBBE_MODSOURCESETGUIDLG_H

