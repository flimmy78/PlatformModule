#ifndef CDevCtlInterfaceGUIDLG_H
#define CDevCtlInterfaceGUIDLG_H

#include <QDialog>
#include "TMRemoteInterfaceGUI.h"
#include <QTimer>
namespace Ui {
class CDevCtlInterfaceGUIDlg;
}

class CDevCtlInterfaceGUIDlg : public QDialog
{
    Q_OBJECT

public:
    explicit CDevCtlInterfaceGUIDlg(QWidget *parent = 0);
    ~CDevCtlInterfaceGUIDlg();
    SearchStatusThread* m_pSearchStatuThread;
protected:

private:
    Ui::CDevCtlInterfaceGUIDlg *ui;
    QString m_strTitle;
    TLxTsspObjectInfo m_ObjectInfo;
    ILxTsspLogManager* m_pILxTsspLogManager;
    ParaStruct m_DlgParaSetStruct[3][60];  //用于存放各个参数参数描述信息
    QString m_DlgMapMean[50][30];       //用于保存map映射类数据项的含义值，往控件中写入
    long DlgMapLineNum[50];             //用于保存各个map映射类数据项的行数（即映射项数）。(数组长度可根据xml文件中map类型的个数自定义)
    ILxTsspHAManager* m_pHAManager;//设备管理对象
    ILxTsspStateManager*  m_pStateMgr;
    ILxTsspMessageManager* m_pILxTsspMessageManager;
    ILxTsspPublicInfoManager* m_pPublicInfo;
    QTimer* m_pTimer;
    QMap<quint64, QString> m_mapi64Str;//控制命令响应,工作模式.

    QString getTaskQStr();
    QString getDataTransFreqRgeTstQStr();
    QString getRealErrCodFrmStatisticsQStr();
    void clear();
    bool exchange(const QString& buf, int& v);
    QString getStrFromDatagram(QByteArray&);

    bool m_showCommDatagram;
    int m_timerComm;
    bool m_showStatuDatagram;
    int m_timerStatu;



public:
    //设置标题 传倒信息
    void setText(const QString& strTitle,ParaStruct xmlsetText[3][60],QString MapMean[50][30],int MapNum[50],uchar Type,ILxTsspParameterMacro* pMacroObj);
    //初始化对话框
    void InitDLG(int Type);
    //设置对象信息
    void SetObjInfo(TLxTsspObjectInfo ObjectInfo);
    //保存参数宏
    int SaveToMacro(ILxTsspParameterMacro* pMacroObj);
    //16进制辅助函数
    DWORDLONG StrToSynCode(const char * pStr);
    void SendMessage(char* pData, unsigned short  usLen, unsigned short usCMD, unsigned short usTID);
protected:
    void closeEvent(QCloseEvent *event);
    void timerEvent(QTimerEvent *event);

    void showDatagram();

private slots:
    void on_pushButton_clicked();
    void on_pushButton_traceChannelSel_clicked();
    void on_pushButton_realErrCodeFrmReport_clicked();
    void on_pushButton_doppTestreport_clicked();
    void on_pushButton_show_clicked();
    void on_timer_show_response();
    void on_pushButton_clear_clicked();
    void on_pushButton_wordModeSet_clicked();
    void on_pushButtonWordWay_clicked();
    void on_pushButton_pauseStatu_clicked();
    void on_pushButton_statuCmd_clicked();
    void on_pushButton_commCmd_clicked();
    void on_pushButton_commClear_clicked();
    void on_pushButton_statuClear_clicked();
    void on_pushButton_flash_clicked();
};

#endif // CDevCtlInterfaceGUIDLG_H

