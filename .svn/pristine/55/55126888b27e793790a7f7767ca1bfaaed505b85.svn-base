#ifndef USBRSPDCUGUIDLG_H
#define USBRSPDCUGUIDLG_H

#include <QDialog>
#include <QLabel>
#include "USBRSPDcuGUI.h"
#include "../../Common/DeviceMap.h"

#include <QStandardItemModel>
#include "../../Common/CommonFunction.h"

enum LightType
{
    Light_empty = 0,    //无状态
    Light_green,            //正常
    Light_red,              //异常
};

namespace Ui {
    class USBRSPDcuGUIDlg;
}

class USBRSPDcuGUIDlg : public QDialog
{
    Q_OBJECT
public:
    explicit USBRSPDcuGUIDlg(QWidget *parent = 0);
    QSize sizeHint() const;
    ~USBRSPDcuGUIDlg();

protected:
    void closeEvent(QCloseEvent *event);
    void timerEvent(QTimerEvent *event);

private:
    Ui::USBRSPDcuGUIDlg *ui;

    QString m_strTitle;
    TLxTsspObjectInfo m_ObjectInfo;
    ILxTsspLogManager* m_pILxTsspLogManager;

    //COMBTRANSPara  m_ComPara;          //此参数用于宏参数显示使用
    COMBTRANSPara *m_pComPara;          //用于显示参数宏应答信息
    COMBTRANSStatu *m_pComStatus;      //用于状态更新使用

    ParaStruct m_DlgParaSetStruct[3][60];  //用于存放各个参数参数描述信息
    QString m_DlgMapMean[50][30];       //用于保存map映射类数据项的含义值，往控件中写入
    long DlgMapLineNum[50];             //用于保存各个map映射类数据项的行数（即映射项数）。(数组长度可根据xml文件中map类型的个数自定义)

    ILxTsspHAManager* m_pHAManager;//设备管理对象

    ILxTsspStateManager*  m_pStateMgr;
    ILxTsspMessageManager* m_pILxTsspMessageManager;
    ILxTsspPublicInfoManager* m_pPublicInfo;

    quint64 m_ParaComSynID;      //参数框架ID
    quint64 m_ParaMarcoComSynID;      //参数宏ID
    quint64 m_StatusComSynID;    //参数状态ID

    int m_TimerID;

    int m_MarcoSearchTimerID;      //参数宏查询指令

    int iParaSetFlag;       //参数设置标志，0表示参数设置,1表示宏编辑

    CDeviceMap m_DeviceMap;

    unsigned short m_usC;            //信息代号
    unsigned short m_usTID;         //命令类型


    unsigned short m_usCMarcoSearch;        //宏查询信息代号
    unsigned short m_usTIDMarcoSearch;      //宏查询命令类型

    unsigned short m_usCMarcoCall;          //宏调用信息代号
    unsigned short m_usTIDMarcoCall;        //宏调用命令类型

    QVector<SUBITEM> m_vSubItem;

    QStandardItemModel *m_pModel;

    bool m_bOnLine;                  //设备在线还是离线

public:
    //设置标题 传倒信息
    void setText(const QString& strTitle, ParaStruct xmlsetText[3][60], QString MapMean[50][30], int MapNum[50], uchar Type, ILxTsspParameterMacro* pMacroObj);
    //初始化对话框
    void InitDLG(int Type, ILxTsspParameterMacro* pMacroObj);
    //初始化TabWidget表格控件
    void initTableView();
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
    //16进制辅助函数
    DWORDLONG StrToSynCode(const char * pStr);
    void SendMessage(char* pData, unsigned short  usLen, unsigned short usC, unsigned short usTID);
    //初始发送的指令信息
    bool initCmdInfo(QString strModuleName);
    //发送命令数据帧
    void sendCMD(QByteArray ba, QString strInfo);
    //设置灯的颜色
    void setLight(QLabel *pLabel, int nState);
    //添加数据内容到表格数据中
    void addData2TabView();

    //设置设备离线状态变量状态为--
    void setOfflineStatusValue();

private slots:
    void on_pushButton_Modify_clicked();
    void on_pushButton_Set_clicked();
    void on_pushButton_MacroSearch_clicked();
    void on_pushButton_MacroCall_clicked();
    void on_pushButton_CtrlCMD_clicked();
    void on_pushButton_ModuleCMD_clicked();
    void on_pushButton_SaveCtrlCMD_clicked();
    void on_pushButton_FZCtrlCMD_clicked();
};

#endif // USBRSPDcuGUIDLG_H

