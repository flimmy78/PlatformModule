#ifndef USBBBE_CJPARASETGUIDLG_H
#define USBBBE_CJPARASETGUIDLG_H

#include <QDialog>
#include "USBBBE_CJParaSetGUI.h"
#include "../../Common/DeviceMap.h"
#include "../../Common/MCSUSBDataStruct.h"
#include <QMessageBox>

namespace Ui {
class USBBBE_CJParaSetGUIDlg;
}

class USBBBE_CJParaSetGUIDlg : public QDialog
{
    Q_OBJECT

public:
    explicit USBBBE_CJParaSetGUIDlg(QWidget *parent = 0);
    QSize sizeHint() const;
    ~USBBBE_CJParaSetGUIDlg();

protected:
    void closeEvent(QCloseEvent *event);
    void timerEvent(QTimerEvent *event);

private:
    Ui::USBBBE_CJParaSetGUIDlg *ui;

    QString m_strTitle;
    TLxTsspObjectInfo m_ObjectInfo;
    ILxTsspLogManager* m_pILxTsspLogManager;

    MCSUSBCjParamStruct m_MCSUSBCjParam;       //参数

    MCSUSBCjStatusStruct* m_MCSUSBCjStatus;       //状态

    ParaStruct m_DlgParaSetStruct[3][60];  //用于存放各个参数参数描述信息
    QString m_DlgMapMean[50][30];       //用于保存map映射类数据项的含义值，往控件中写入
    long DlgMapLineNum[50];             //用于保存各个map映射类数据项的行数（即映射项数）。(数组长度可根据xml文件中map类型的个数自定义)

    ILxTsspHAManager* m_pHAManager;//设备管理对象

    ILxTsspStateManager*  m_pStateMgr;
    ILxTsspMessageManager* m_pILxTsspMessageManager;
    ILxTsspPublicInfoManager* m_pPublicInfo;

    quint64 m_ParaMCSUSBCjParamID;          //参数ID

    quint64 m_StatusParaMCSUSBCjParamID;    //状态ID

    CDeviceMap m_DeviceMap;

    int m_TimerID;
    int iParaSetFlag;       //参数设置标志，0表示参数设置,1表示宏编辑

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
    void EnabledControlsDown(bool bFlag);
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
    void on_pushButton_Setting_clicked();
    //16进制辅助函数
    void on_pushButton_Change_clicked();
    DWORDLONG StrToSynCode(const char * pStr);
    void SendMessage(char* pData, unsigned short  usLen, unsigned short usC, unsigned short usTID);

    int msgBoxFunction(QMessageBox &msgBox, QString msg, BOOL ret);
private slots:
    void on_pushButton_Modify_clicked();
    void on_pushButton_Set_clicked();
    void on_Setting_1_clicked();
    void on_pushButton_Change_1_clicked();
    void on_comboBox_DisForType_currentIndexChanged(int index);
    void on_Set_clicked();
    void on_comboBox_FuzDis_currentIndexChanged(int index);
    void on_pushButton_Chang_clicked();
    void on_pushButton_Set_down_clicked();
    void on_pushButton_Modify_down_clicked();
    void on_groupBox_6_clicked();
};

#endif // USBBBE_CJPARASETGUIDLG_H

