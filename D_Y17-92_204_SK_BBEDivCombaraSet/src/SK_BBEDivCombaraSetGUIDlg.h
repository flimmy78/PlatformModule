#ifndef SK_BBEDivCombaraSetGUIDlg_H
#define SK_BBEDivCombaraSetGUIDlg_H

#include <QDialog>
#include "SK_BBEDivCombaraSetGUI.h"
#include "../../Common/DeviceMap.h"

namespace Ui {
class SK_BBEDivCombaraSetGUIDlg;
}

class SK_BBEDivCombaraSetGUIDlg : public QDialog
{
    Q_OBJECT

public:
    explicit SK_BBEDivCombaraSetGUIDlg(QWidget *parent = 0);
    QSize sizeHint() const;
    ~SK_BBEDivCombaraSetGUIDlg();
protected:
    void closeEvent(QCloseEvent *event);
    void timerEvent(QTimerEvent *event);
private:
    Ui::SK_BBEDivCombaraSetGUIDlg *ui;

    QString m_strTitle;
    TLxTsspObjectInfo m_ObjectInfo;
    ILxTsspLogManager* m_pILxTsspLogManager;

    MCSDivComParamStruct  m_MCSDivComPara;          //分集合成参数

    MCSDivComStatusStruct *m_pMCSDivComStatus;      //分集合成状态参数

    ParaStruct m_DlgParaSetStruct[3][60];  //用于存放各个参数参数描述信息
    QString m_DlgMapMean[50][30];       //用于保存map映射类数据项的含义值，往控件中写入
    long DlgMapLineNum[50];             //用于保存各个map映射类数据项的行数（即映射项数）。(数组长度可根据xml文件中map类型的个数自定义)

    ILxTsspHAManager* m_pHAManager;//设备管理对象

    ILxTsspStateManager*  m_pStateMgr;
    ILxTsspMessageManager* m_pILxTsspMessageManager;
    ILxTsspPublicInfoManager* m_pPublicInfo;

    quint64 m_ParaMCSDivComSynID;      //分集合成参数框架ID

    quint64 m_StatusMCSDivComSynID;    //分集合成参数状态ID

    int m_TimerID;
    int iParaSetFlag;       //参数设置标志，0表示参数设置,1表示宏编辑

    CDeviceMap m_DeviceMap;

    unsigned short m_usC;            //信息代号
    unsigned short m_usTID;         //命令类型
    unsigned char m_ucSN;

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
    //设置帧长
    void SetFrameLenRange(int iWordLen);
    //验证参数范围
    int CheckPara();
    //16进制辅助函数
    DWORDLONG StrToSynCode(const char * pStr);
    void SendMessage(char* pData, unsigned short  usLen, unsigned short usC, unsigned short usTID, unsigned char ucSN);

    //初始发送的指令信息

private slots:
    void on_pushButton_Modify_clicked();
    void on_pushButton_Set_clicked();
    void on_comboBox_codeType_currentIndexChanged(int index);
};

#endif // DIALOG_H

