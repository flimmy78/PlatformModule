#ifndef NET_SWITCHGUIDLG_H
#define NET_SWITCHGUIDLG_H

#include <QComboBox>
#include <QDialog>
#include <QLineEdit>
#include "Net_SwitchGUI.h"
#include "XmlReader.h"
#include "shape/Matrix.h"
#include "shape/signalitem.h"
#include <QTextCodec>

//定义最大的开关矩阵的个数
#define MAX_NUM     32

namespace Ui {
    class Net_SwitchGUIDlg;
}

//定义保存界面上数据的结构体
typedef struct _tag_IFMShowData
{
    QString strName;
    QStringList infoList;
    _tag_IFMShowData()
    {
        strName = "";
        infoList.clear();
    }
}IFMSHOWDATA;

typedef QList<IFMSHOWDATA> IFMList;

class Net_SwitchGUIDlg : public QDialog
{
    Q_OBJECT
public:
    explicit Net_SwitchGUIDlg(QWidget *parent = 0);
    QSize sizeHint() const;
    ~Net_SwitchGUIDlg();

protected:
    void closeEvent(QCloseEvent *event);
    void timerEvent(QTimerEvent *event);

private:
    Ui::Net_SwitchGUIDlg *ui;

    QString m_strTitle;
    TLxTsspObjectInfo m_ObjectInfo;
    ILxTsspLogManager* m_pILxTsspLogManager;

    HK_IFSWMatPara m_IFSWMatParamSyn;   //中频开关矩阵参数
    HK_IFSWMatStatu * m_IFSWMatStatuSyn;  //中频开关矩阵状态
    HK_IFSWMatResponse *m_pIFSMatResponse;  //中频开关矩阵显示响应信息

    ParaStruct m_DlgParaSetStruct[3][60];  //用于存放各个参数参数描述信息
    QString m_DlgMapMean[50][30];       //用于保存map映射类数据项的含义值，往控件中写入
    long DlgMapLineNum[50];             //用于保存各个map映射类数据项的行数（即映射项数）。(数组长度可根据xml文件中map类型的个数自定义)

    ILxTsspHAManager* m_pHAManager;//设备管理对象

    ILxTsspStateManager*  m_pStateMgr;
    ILxTsspMessageManager* m_pILxTsspMessageManager;
    ILxTsspPublicInfoManager* m_pPublicInfo;

    quint64 m_ParaCodeSynID;          //参数显示的信息ID
    quint64 m_StatusCodeSynID;          //状态显示的信息ID
    quint64 m_ParaResponseID;                //状态响应参数信息ID

    int m_TimerID;
    int m_StatusTimerID;    //状态响应超时时间
    int iParaSetFlag;       //参数设置标志，0表示参数设置,1表示宏编辑

    bool m_bOnLine;                  //设备在线还是离线

private slots:
    void OnSaveBtn();
    void OnBtnDel();
    void OnBtnModify();
    void OnBtnSetting();
    void onChooseAccount(const QString &strText);

private:
    //初始化相关参数信息
    void init();
    QWidget *getSignalBoard(int boardHeight, int nNum,
                                        NUM_PARAM &Num1, NUM_PARAM &Num2, NUM_PARAM &menu);
    //绘制二维矩阵和右侧显示内容
    void initMatrixAndOther();
    //初始化窗体的列表框和按钮
    void initCommboAndBtn();

    //获取保存后的文件内容
    void loadSaveInfo(const QString &strPath);
    void saveData2File(const QString &strPath, const QString &name);
    //将信息加载到列表框控件中
    void updateCombo();
    //设置显示的信息到界面的右侧显示控件上
    void setShowInfo(const QStringList &infoList);
    //初始发送的指令信息
    bool initCmdInfo(QString strModuleName);

    //获取需要保存控件上的数据
    QList<QString> getShowInfo();

    //显示灯光
    void showLight();

    //设置设备离线状态变量状态为--
    void setOfflineStatusValue();

private:
    CXmlReader m_reader;                //xml配置文件变量
    Matrix * m_pMatrix;                 //左侧二维矩阵变量
    QComboBox * m_pComType;             //将类型数据添加到列表框中
    QPushButton *m_pBtnSave;            //界面上的保存按钮
    QPushButton *m_pBtnDel;             //界面上的删除按钮
    QString m_strFilePath;              //定义保存信息的文件路径
    IFMList     m_IFMList;              //
    QList<SignalItem *> m_ItemList;     //右侧相关信息列表变量
    QLineEdit * m_pEditStatus;          //显示状态信息 20170321
    QPushButton *m_pBtnModify;          //界面上更改按钮
    QPushButton *m_pBtnSetting;         //界面上设置按钮

    unsigned int m_nMatrixNum;          //二维矩阵数组个数

    CDeviceMap m_DeviceMap;

    unsigned short m_usC;               //信息代号
    unsigned short m_usTID;             //命令类型

    QTextCodec *m_pTextCodec;

public:
    //设置标题 传倒信息
    void setText(const QString& strTitle,ParaStruct xmlsetText[3][60],QString MapMean[50][30],int MapNum[50],uchar Type,ILxTsspParameterMacro* pMacroObj);
    //初始化对话框
    void InitDLG(int Type,ILxTsspParameterMacro* pMacroObj);

    //设置对象信息
    void SetObjInfo(TLxTsspObjectInfo ObjectInfo);
    //保存参数宏
    int SaveToMacro(ILxTsspParameterMacro* pMacroObj);

    //设置需要亮的灯光
    void setLight(int nRow, int nCol, bool bFlag = true);
    //设置主站和分站
    void setType(bool bMain = true, bool bNormal = true);
    //设置响应状态信息
    void setStatus();

    //发送指令数据到分机设备接口
    void SendMessage(char* pData, unsigned short  usLen, unsigned short usC, unsigned short usTID);
    //关闭所有的灯
    void closeLight();
};

#endif // NET_SWITCHGUIDLG_H

