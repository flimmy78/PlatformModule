#ifndef SSWITCHDCUGUIDLG_H
#define SSWITCHDCUGUIDLG_H

#include <QDialog>
#include "SswitchDcuGUI.h"
#include "XmlReader.h"
#include "../../Common/DeviceMap.h"
#include "../../Common/GroupSwitch.h"

namespace Ui {
    class SswitchDcuGUIDlg;
}

class SswitchDcuGUIDlg : public QDialog
{
    Q_OBJECT
public:
    explicit SswitchDcuGUIDlg(QWidget *parent = 0);
    QSize sizeHint() const;
    ~SswitchDcuGUIDlg();

protected:
    void closeEvent(QCloseEvent *event);
    void timerEvent(QTimerEvent *event);

private:
    Ui::SswitchDcuGUIDlg *ui;

    QString m_strTitle;
    TLxTsspObjectInfo m_ObjectInfo;
    ILxTsspLogManager* m_pILxTsspLogManager;

    SSWITCHDCUPara m_SswitchDcuPara;  //状态参数
    SSWITCHDCUStatu *m_pSswitchDcuStatus;  //

    ParaStruct m_DlgParaSetStruct[3][60];  //用于存放各个参数参数描述信息
    QString m_DlgMapMean[50][30];       //用于保存map映射类数据项的含义值，往控件中写入
    long DlgMapLineNum[50];             //用于保存各个map映射类数据项的行数（即映射项数）。(数组长度可根据xml文件中map类型的个数自定义)

    ILxTsspHAManager* m_pHAManager;//设备管理对象

    ILxTsspStateManager*  m_pStateMgr;
    ILxTsspMessageManager* m_pILxTsspMessageManager;
    ILxTsspPublicInfoManager* m_pPublicInfo;

    quint64 m_ParaCodeSynID;           //参数设置信息ID
    quint64 m_StatusCodeSynID;        //状态响应信息ID

    int m_TimerID;
    int m_StatusTimerID;    //状态响应超时时间
    int iParaSetFlag;       //参数设置标志，0表示参数设置,1表示宏编辑

    bool m_bOnLine;                  //设备在线还是离线

private slots:
    void on_pushButton_Modify_clicked();

    void on_pushButton_Set_clicked();

    void onChooseAccount(int nIndex);

private:
    //初始化相关参数信息
    void init();
    //初始化界面上的控件
    void initCtrl();
    //初始发送的指令信息
    bool initCmdInfo(QString strModuleName);
    //初始化界面上的列表
    void initLabelAndCombox();

    //显示需要画的线的颜色
    void show();

    //设置输入格式
    void InitInput();
    //使能或不使能控件
    void EnabledControls(bool bFlag);

    //设置主站和分站
    void setType(bool bMain = true, bool bNormal = true);

    //设置设备离线状态变量状态为--
    void setOfflineStatusValue();

    //检测参数设置
    bool checkPara();

private:
    CXmlReader m_reader;             //xml配置文件变量
    CDeviceMap m_DeviceMap;

    unsigned short m_usC;            //信息代号
    unsigned short m_usTID;         //命令类型

    QList<GroupSwitch*> m_GroupSwitchList;  //保存new出来的控件信息
    QComboBox *m_pCommbox;

public:
    //设置标题 传倒信息
    void setText(const QString& strTitle,ParaStruct xmlsetText[3][60],QString MapMean[50][30],int MapNum[50],uchar Type,ILxTsspParameterMacro* pMacroObj);
    //初始化对话框
    void InitDLG(int Type,ILxTsspParameterMacro* pMacroObj);

    //设置对象信息
    void SetObjInfo(TLxTsspObjectInfo ObjectInfo);
    //保存参数宏
    int SaveToMacro(ILxTsspParameterMacro* pMacroObj);

    //发送指令数据到分机设备接口
    void SendMessage(char* pData, unsigned short  usLen, unsigned short usC, unsigned short usTID);
};

#endif // SSWITCHDCUGUIDLG_H

