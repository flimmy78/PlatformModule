#ifndef TCPNCODESETGUIDLG_H
#define TCPNCODESETGUIDLG_H

#include <QDialog>
#include "TCPNCodeSetGUI.h"
#include "../../Common/DeviceMap.h"
#include "../../Common/SMCUSSB2_BBE_CDMAStruct.h"
#include "../../Common/CommonFunction.h"
#include <QMessageBox>
namespace Ui {
class TCPNCodeSetGUIDlg;
}

class TCPNCodeSetGUIDlg : public QDialog
{
    Q_OBJECT

public:
    explicit TCPNCodeSetGUIDlg(QWidget *parent = 0);
    QSize sizeHint() const;
    ~TCPNCodeSetGUIDlg();

protected:
    void closeEvent(QCloseEvent *event);
    void timerEvent(QTimerEvent *event);

private:
    Ui::TCPNCodeSetGUIDlg* ui;

    QString m_strTitle;
    TLxTsspObjectInfo m_ObjectInfo;
    ILxTsspLogManager* m_pILxTsspLogManager;

    SMCU2::MCSSSB2PNCodeStruct4 m_MCSSSB2PNCode;       //码同步参数

    SMCU2::MCSSSB2PNCODEStatusStruct4* m_MCSSSB2PNCODEStatus;       //码同步参数

    ParaStruct m_DlgParaSetStruct[5][60];  //用于存放各个参数参数描述信息
    QString m_DlgMapMean[50][30];       //用于保存map映射类数据项的含义值，往控件中写入
    long DlgMapLineNum[50];             //用于保存各个map映射类数据项的行数（即映射项数）。(数组长度可根据xml文件中map类型的个数自定义)

    ILxTsspHAManager* m_pHAManager;//设备管理对象

    ILxTsspStateManager*  m_pStateMgr;
    ILxTsspMessageManager* m_pILxTsspMessageManager;
    ILxTsspPublicInfoManager* m_pPublicInfo;

    quint64 m_ParaMCSSSB2PNCodeID;          //参数ID
    quint64 m_StatusParaMCSSSB2PNCodeID;    //状态ID
    quint64 m_CommTargetID;
    int m_TimerID;
    int iParaSetFlag;       //参数设置标志，0表示参数设置,1表示宏编辑
    CDeviceMap m_DeviceMap;
    unsigned short m_usC;
    unsigned short m_usTID;
    unsigned char m_ucSN;
    void showRadioButton();
    int getSharedMemoryTargetNum();
    SMCU2::MCSSysTarget4StatusStruct* m_pMCSTargetSysStatus;
    void showTargetParam(int nIndex,  int nType = 0);
    void getParamTarget();
    int m_nTargetNum;       //当前选择的目标
    int m_currentNumber_usedByMacro;
    int m_currentNumber_usedByParam;
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
    void SendMessage(char* pData, unsigned short  usLen, unsigned short usC, unsigned short usTID, unsigned char ucSN);

    CCommFunc* m_pCommFunc;
    QTimer *m_timer1;
    int msgBoxFunction(QMessageBox &msgBox, QString msg, BOOL ret);
    void set_radioButton_Target();
    bool CheckParaValue(QLineEdit *pLE);
private slots:
    void on_pushButton_Modify_clicked();
    void on_pushButton_Set_clicked();
    void on_radioButton_Target1_clicked();
    void on_radioButton_Target2_clicked();
    void on_radioButton_Target3_clicked();
    void on_radioButton_Target4_clicked();
    void updateRadioButton();
};

#endif // TCPNCODESETGUIDLG_H

