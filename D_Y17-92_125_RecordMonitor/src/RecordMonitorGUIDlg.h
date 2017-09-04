#ifndef RECORDMONITORGUIDLG_H
#define RECORDMONITORGUIDLG_H

#include <QDialog>
#include <QLabel>
#include <QStandardItemModel>
#include <QTextCodec>
#include "RecordMonitorGUI.h"

enum LightType
{
    Light_empty = 0,    //无状态
    Light_green,            //正常
    Light_red,              //异常
};

namespace Ui {
    class RecordMonitorGUIDlg;
}

class RecordMonitorGUIDlg : public QDialog
{
    Q_OBJECT
public:
    explicit RecordMonitorGUIDlg(QWidget *parent = 0);
    QSize sizeHint() const;
    ~RecordMonitorGUIDlg();

protected:
    void closeEvent(QCloseEvent *event);
    void timerEvent(QTimerEvent *event);

private:
    Ui::RecordMonitorGUIDlg *ui;

    QString m_strTitle;
    //对象信息
    TLxTsspObjectInfo m_ObjectInfo;
    //日志管理接口
    ILxTsspLogManager* m_pILxTsspLogManager;

    ParaStruct m_DlgParaSetStruct[3][60];  //用于存放各个参数参数描述信息
    QString m_DlgMapMean[50][30];       //用于保存map映射类数据项的含义值，往控件中写入
    long DlgMapLineNum[50];             //用于保存各个map映射类数据项的行数（即映射项数）。(数组长度可根据xml文件中map类型的个数自定义)

    ILxTsspHAManager* m_pHAManager;//设备管理对象

    ILxTsspStateManager*  m_pStateMgr;//设备状态管理对象
    ILxTsspMessageManager* m_pILxTsspMessageManager;//消息管理对象
    ILxTsspPublicInfoManager* m_pPublicInfo;//公共信息管理

    int m_TimerID;              //刷新状态使用的定时器
    int m_TimerFileListID;      //刷新查询文件列表使用的定时器
    int m_TimerFileInfoID;      //刷新查询文件信息使用的定时器
    int m_timerStop;//记录停止时，回放停止时刷新文件列表
    int m_timerDel;//删除文件刷新文件列表
    int m_timerWhetherUpdateFileList;//运行修在远控时，是否更新文件列表

    //检前记录工作状态结构
    StatusFindResStruct *m_pStatusWorkRes;
    quint64 m_StatusWorkSynID;

    //文件信息列表查询结构体
    RECORDFILELIST *m_pFileListRes;
    RECORDFILELIST m_fileListRes;
    quint64 m_FileListSynID;

    //查询单个文件信息结构体
    RECORDFILEINFOCOMM *m_pFileInfoRes;
    quint64 m_FileInfoSynID;

    QStandardItemModel * m_pModel;

    //报警参数列表
    //WARN_STATUS m_WStatus;

    CDeviceMap m_DeviceMap;
    QVector<SUBITEM> m_vSubItem;

    QString m_strDeviceName;

    unsigned short m_usC;               //信息代号
    unsigned short m_usTID;             //命令类型

    bool m_bOnLine;                  //设备在线还是离线

    QTextCodec *m_pCodec;
    int m_oldCtrl ;//历史控制方式
public:
    //设置标题 传倒信息
    void setText(const QString& strTitle,ParaStruct xmlsetText[3][60],QString MapMean[50][30],int MapNum[50],uchar Type,ILxTsspParameterMacro* pMacroObj);
    //初始化对话框
    void InitDLG(int Type,ILxTsspParameterMacro* pMacroObj);

    //显示参数
    void ShowPara(int type);
    //设置对象信息
    void SetObjInfo(TLxTsspObjectInfo ObjectInfo);
    //保存参数宏
    int SaveToMacro(ILxTsspParameterMacro* pMacroObj);

    //16进制辅助函数
    DWORDLONG StrToSynCode(const char * pStr);
    void SendMessage(char* pData, unsigned short  usLen, unsigned short usC, unsigned short usTID);

    void SendCmdData(char cmd, char * pData, unsigned int nLen);
    //初始化界面上文件列表控件
    bool initFileTabView();
    //根据数据获取工作状态
    QString GetWorkStatus(char type);
    //根据结果返回故障信息
    void DealWarningStatus(WORD wValue);
    //设置指示灯
    void setLight(QLabel *pLabel, int nState);
    //设置设备离线状态变量状态为--
    void setOfflineStatusValue();
    //刷新查询文件列表显示
    void showFileList();
    //刷新文件信息显示
    void showFileInfo();
    //记录时，查看用户输入文件是否存在
    bool fileExists(QString file);
    bool whetherUpdateFileList();
    bool isRemote();
public slots:
    void keyPressEvent(QKeyEvent *event);

private slots:
    void on_pushButton_FileInfoSearch_clicked();
    void on_pushButton_Record_clicked();
    void on_pushButton_Stop_clicked();
    void on_pushButton_Replay_clicked();
    void on_pushButton_Delete_clicked();
    void on_pushButton_ShutDown_clicked();
    void on_pushButton_FileListSearch_clicked();
};

#endif // RECORDMONITORGUIDLG_H

