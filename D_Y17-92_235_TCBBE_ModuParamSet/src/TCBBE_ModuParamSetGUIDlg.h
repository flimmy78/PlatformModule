#ifndef SS2BBE_MODUPARAMSETGUIDLG_H
#define SS2BBE_MODUPARAMSETGUIDLG_H

#include <QDialog>
#include "TCBBE_ModuParamSetGUI.h"
#include "../../Common/DeviceMap.h"
#include "../../Common/CommonFunction.h"

namespace Ui {
class SS2BBE_ModuParamSetGUIDlg;
}

class SS2BBE_ModuParamSetGUIDlg : public QDialog
{
    Q_OBJECT

public:
    explicit SS2BBE_ModuParamSetGUIDlg(QWidget *parent = 0);
    QSize sizeHint() const;
    ~SS2BBE_ModuParamSetGUIDlg();

protected:
    void closeEvent(QCloseEvent *event);
    void timerEvent(QTimerEvent *event);

private:
    Ui::SS2BBE_ModuParamSetGUIDlg *ui;

    QString m_strTitle;
    TLxTsspObjectInfo m_ObjectInfo;
    ILxTsspLogManager* m_pILxTsspLogManager;

    SMCU2::MCSUpModulParamStruct4 m_MCSSSUpModulParam;       //参数

    SMCU2::MCSUpModuleStatusStruct4* m_MCSSSSUpModulStatus;       //码同步参数

    ParaStruct m_DlgParaSetStruct[5][60];  //用于存放各个参数参数描述信息
    QString m_DlgMapMean[50][30];       //用于保存map映射类数据项的含义值，往控件中写入
    long DlgMapLineNum[50];             //用于保存各个map映射类数据项的行数（即映射项数）。(数组长度可根据xml文件中map类型的个数自定义)

    ILxTsspHAManager* m_pHAManager;//设备管理对象

    ILxTsspStateManager*  m_pStateMgr;
    ILxTsspMessageManager* m_pILxTsspMessageManager;
    ILxTsspPublicInfoManager* m_pPublicInfo;

    quint64 m_ParaMCSUpModulID;          //参数框架ID

    quint64 m_StatusParaMCSUpModulID;    //参数状态ID

    SMCU2::MCSSysTarget4StatusStruct* m_MCSSysTarget4Status;
    quint64 m_CommTargetID;

    int m_TimerID;
    int iParaSetFlag;       //参数设置标志，0表示参数设置,1表示宏编辑
    CDeviceMap m_DeviceMap;
    unsigned short m_usC;
    unsigned short m_usTID;

    int m_nTargetNum;       //当前选择的目标
    SUBITEM m_sItem;
    quint64 m_ctlCmd;
    int m_targetNum_usedInMacro;
    int m_targetNum_usedInParam;
public:
    //设置标题 传倒信息
    void setText(const QString& strTitle,ParaStruct xmlsetText[5][60],QString MapMean[50][30],int MapNum[50],uchar Type,ILxTsspParameterMacro* pMacroObj);
    //初始化对话框
    void InitDLG(int Type,ILxTsspParameterMacro* pMacroObj);
    //设置输入格式
    void InitInput();
    //显示变量及范围
    void ShowControls();
    //使能或不使能控件
    void EnabledControls(bool bFlag);
    void EnabledControlsCommon(bool bFlag);
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
    void SendMessage(char* pData, USHORT usLen, USHORT usC, USHORT usTID, USHORT usSN = 0);

    //获取当前目标的值并赋值到结构体
    void getParamTarget();

    //获取共享内存中目标的个数
    int getSharedMemoryTargetNum();

    //目标 显示
    void showRadioButton();

    //显示不同目标的数据
    void showTargetParam(int nIndex, int nType = 0);
    CCommFunc* m_pCommFunc;

    QTimer *m_timer1;

    void set_radioButton_Target();
private slots:
    void updateRadioButton();
    void on_pushButton_Modify_clicked();
    void on_pushButton_Set_clicked();
    void on_radioButton_Target1_clicked();
    void on_radioButton_Target2_clicked();
    void on_radioButton_Target3_clicked();
    void on_radioButton_Target4_clicked();
    void on_pushButton_PlblicUnitModify_clicked();
    void on_comboBox_NoiseSourOutput_currentIndexChanged(int index);
    void on_comboBox_ChannelOutputCtrlT_currentIndexChanged(int index);
    void on_pushButton_PublicUnitSet_clicked();
};

#endif // SS2BBE_MODUPARAMSETGUIDLG_H

