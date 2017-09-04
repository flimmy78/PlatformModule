#ifndef SSBBE_SYSPARAMSETGUIDLG_H
#define SSBBE_SYSPARAMSETGUIDLG_H

#include <QDialog>
#include "SSBBE_SysParamSetGUI.h"
#include "../../Common/DeviceMap.h"
#include "../../Common/CommonFunction.h"
#include <QTimer>
#include <QMessageBox>

namespace Ui {
    class SSBBE_SysParamSetGUIDlg;
}

class SSBBE_SysParamSetGUIDlg : public QDialog
{
    Q_OBJECT

public:
    explicit SSBBE_SysParamSetGUIDlg(QWidget *parent = 0);
    QSize sizeHint() const;
    ~SSBBE_SysParamSetGUIDlg();

protected:
    void closeEvent(QCloseEvent *event);
    void timerEvent(QTimerEvent *event);

private:
    Ui::SSBBE_SysParamSetGUIDlg *ui;

    QString m_strTitle;
    TLxTsspObjectInfo m_ObjectInfo;
    ILxTsspLogManager* m_pILxTsspLogManager;

    //参数和状态一样
    SMCU::MCSSysTarget4ParamStruct m_MCSTargetSysParam;       //参数
    SMCU::MCSSysTarget4StatusStruct* m_pMCSTargetSysStatus;       //状态

    ParaStruct m_DlgParaSetStruct[5][60];  //用于存放各个参数参数描述信息
    QString m_DlgMapMean[50][30];       //用于保存map映射类数据项的含义值，往控件中写入
    long DlgMapLineNum[50];             //用于保存各个map映射类数据项的行数（即映射项数）。(数组长度可根据xml文件中map类型的个数自定义)

    ILxTsspHAManager* m_pHAManager;//设备管理对象

    ILxTsspStateManager*  m_pStateMgr;
    ILxTsspMessageManager* m_pILxTsspMessageManager;
    ILxTsspPublicInfoManager* m_pPublicInfo;

    quint64 m_ParaMCSTargetSysID;          //参数ID

    quint64 m_StatusParaMCSTargetSysID;    //状态ID

    int m_TimerID;
    int iParaSetFlag;       //参数设置标志，0表示参数设置,1表示宏编辑
    CDeviceMap m_DeviceMap;
    unsigned short m_usC;
    unsigned short m_usTID;

    int m_nTargetIndex;         //当前目标号
    QTimer* m_timer1;
    CCommFunc* m_pCommFunc;
    quint64 m_targetNumID;
    int m_currentNumber_usedByMacro;
    SUBITEM m_sItem4;//配置文件过程控制命令4 基带校零结果查询
public:
    //设置标题 传倒信息
    void setText(const QString& strTitle,ParaStruct xmlsetText[5][60],QString MapMean[50][30],int MapNum[50],uchar Type,ILxTsspParameterMacro* pMacroObj);
    //初始化对话框
    void InitDLG(int Type, ILxTsspParameterMacro* pMacroObj);
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
    void SendMessage(char* pData, unsigned short  usLen, unsigned short usC, unsigned short usTID);

    //显示公共参数数据内容
    //nType 为0时表是从配置文件读取  1时表示从内存中读取
    void showComParam(int nType = 0);

    //显示不同目标的数据
    void showTargetParam(int nIndex, int nType = 0);

    //获取当前目标的值并赋值到结构体
    void getParamTarget();

    TLxTsspParamMacroInformation *getMacroInfo();
    int msgBoxFunction(QMessageBox &msgBox, QString msg, BOOL ret);
    void set_radioButton_Target();
    void updateParam(SMCU::MCSSysTarget4ParamStruct &comPara, ILxTsspParameterMacro *pMacroObj);
    void targetsCount_Changed(int index);
    void targetsCountChanged(int index);
private slots:
    void updateTarget();
    void on_pushButton_Back_clicked();
    void on_pushButton_Set_clicked();
    void on_comboBox_TargetsCount_currentIndexChanged(int index);
    void on_radioButton_Target1_clicked();
    void on_radioButton_Target2_clicked();
    void on_radioButton_Target3_clicked();
    void on_radioButton_Target4_clicked();
    void on_lineEdit_UpFreDot_textChanged(const QString &arg1);
    void on_lineEdit_DownFreDot_textChanged(const QString &arg1);
};

#endif // SSBBE_SYSPARAMSETGUIDLG_H

