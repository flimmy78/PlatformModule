#ifndef USBBBE_CODETMPARASETGUIDLG_H
#define USBBBE_CODETMPARASETGUIDLG_H

#include <QDialog>
#include "USBBBE_CodeTMParaSetGUI.h"
#include "../../Common/DeviceMap.h"

namespace Ui {
class USBBBE_CodeTMParaSetGUIDlg;
}

class USBBBE_CodeTMParaSetGUIDlg : public QDialog
{
    Q_OBJECT

public:
    explicit USBBBE_CodeTMParaSetGUIDlg(QWidget *parent = 0);
    QSize sizeHint() const;
    ~USBBBE_CodeTMParaSetGUIDlg();

protected:
    void closeEvent(QCloseEvent *event);
    void timerEvent(QTimerEvent *event);

private:
    Ui::USBBBE_CodeTMParaSetGUIDlg *ui;

    QString m_strTitle;
    TLxTsspObjectInfo m_ObjectInfo;
    ILxTsspLogManager* m_pILxTsspLogManager;

    MCSUSBTMParamStruct m_MCSUSBTMParam;       //参数

    MCSUSBTMStatusStruct* m_MCSUSBTMStatus;       //状态

    ParaStruct m_DlgParaSetStruct[3][60];  //用于存放各个参数参数描述信息
    QString m_DlgMapMean[50][30];       //用于保存map映射类数据项的含义值，往控件中写入
    long DlgMapLineNum[50];             //用于保存各个map映射类数据项的行数（即映射项数）。(数组长度可根据xml文件中map类型的个数自定义)

    ILxTsspHAManager* m_pHAManager;//设备管理对象

    ILxTsspStateManager*  m_pStateMgr;
    ILxTsspMessageManager* m_pILxTsspMessageManager;
    ILxTsspPublicInfoManager* m_pPublicInfo;

    quint64 m_ParaComSynID;          //参数ID

    quint64 m_StatusComSynID;    //状态ID

    int m_TimerID;
    int iParaSetFlag;       //参数设置标志，0表示参数设置,1表示宏编辑

    CDeviceMap m_DeviceMap;

    unsigned short m_usC;
    unsigned short m_usTID;

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

private slots:
    void on_pushButton_Modify_clicked();
    void on_pushButton_Set_clicked();
    void on_comboBox_DecodeType_currentIndexChanged(int index);
    void on_comboBox_RecurrentCodeType_currentIndexChanged(int index);

    void on_comboBox_SubFrmSyncType_currentIndexChanged(int index);
    void on_comboBox_RandEn_currentIndexChanged(int index);
};

#endif // USBBBE_CODETMPARASETGUIDLG_H

