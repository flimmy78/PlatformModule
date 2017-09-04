#ifndef DDTBBE_SYSPARASETGUIDLG_H
#define DDTBBE_SYSPARASETGUIDLG_H

#include <QDialog>
#include "DDTBBE_SysParaSetGUI.h"

namespace Ui {
class DDTBBE_SysParaSetGUIDlg;
}

class DDTBBE_SysParaSetGUIDlg : public QDialog
{
    Q_OBJECT

public:
    explicit DDTBBE_SysParaSetGUIDlg(QWidget *parent = 0);
    QSize sizeHint() const;
    ~DDTBBE_SysParaSetGUIDlg();

protected:
    void closeEvent(QCloseEvent *event);
    void timerEvent(QTimerEvent *event);

private:
    Ui::DDTBBE_SysParaSetGUIDlg *ui;

    QString m_strTitle;
    TLxTsspObjectInfo m_ObjectInfo;
    ILxTsspLogManager* m_pILxTsspLogManager;

    MCSSysDataStruct m_MCSSysPara;         //公共参数单元界面

    MCSSysDataStatusStruct *m_MCSSysDataStatus;     //公共单元状态数据元素

    ParaStruct m_DlgParaSetStruct[3][60];  //用于存放各个参数参数描述信息
    QString m_DlgMapMean[50][30];       //用于保存map映射类数据项的含义值，往控件中写入
    long DlgMapLineNum[50];             //用于保存各个map映射类数据项的行数（即映射项数）。(数组长度可根据xml文件中map类型的个数自定义)

    ILxTsspHAManager* m_pHAManager;//设备管理对象

    ILxTsspStateManager*  m_pStateMgr;
    ILxTsspMessageManager* m_pILxTsspMessageManager;
    ILxTsspPublicInfoManager* m_pPublicInfo;

    quint64 m_MCSSysDataID;      //公共参数单元界面ID

    quint64 m_StatusParaMCSSysDataID;    //公共单元状态数据元素ID

    quint64 m_targetNumID;

    int m_TimerID;
    int iParaSetFlag;       //参数设置标志，0表示参数设置,1表示宏编辑

    CDeviceMap m_DeviceMap;

    unsigned short m_usC;            //信息代号
    unsigned short m_usTID;         //命令类型

    unsigned short m_usCmdCtrlC;            //数传控制信息代号
    unsigned short m_usCmdCtrlTID;         //数传控制命令类型
    void relate();
    void sendDataSel();
    void dataTransType();
    SUBITEM m_sItem;
    void SendMessage(char* pData, USHORT usLen, USHORT usC, USHORT usTID, USHORT usSN = 0);
public:
    //设置标题 传倒信息
    void setText(const QString& strTitle, ParaStruct xmlsetText[3][60], QString MapMean[50][30], int MapNum[50], uchar Type, ILxTsspParameterMacro* pMacroObj);
    //初始化对话框
    void InitDLG(int Type, ILxTsspParameterMacro* pMacroObj);
    //设置输入格式
    void InitInput();
    //显示变量及范围
    void ShowControls();
    //使能或不使能控件
    void EnabledControls(bool bFlag);
    //数传控件组使能或不使能控件
    void EnabledDataControls(bool bFlag);
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
    //设置帧长范围
    void SetFrameLenRange(int iWordLen);
    //16进制辅助函数
    DWORDLONG StrToSynCode(const char * pStr);

    //初始发送的指令信息
    bool initCmdInfo(QString strModuleName);

    //void SendMessage(char* pData, unsigned short usLen, unsigned short usC, unsigned short usTID);

    void updateParam(MCSSysDataStruct &comPara, ILxTsspParameterMacro *pMacroObj);
    TLxTsspParamMacroInformation *getMacroInfo();
private slots:
    void on_pushButton_Modify_clicked();
    void on_pushButton_Set_clicked();
    void on_comboBox_SubFrmType_currentIndexChanged(int index);
    void on_comboBox_WordLen_currentIndexChanged(int index);
    void on_pushButton_DataModify_clicked();
    void on_pushButton_DataSet_clicked();
//    void on_comboBox_BaseSelect_currentIndexChanged(int index);
    void on_comboBox_DataType_currentIndexChanged(int index);
};

#endif // DDTBBE_SYSPARASETGUIDLG_H

