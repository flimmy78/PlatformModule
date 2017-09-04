#include "USBBBE_PMModuSetGUIDlg.h"
#include "ui_USBBBE_PMModuSetGUIDlg.h"
#include <QCloseEvent>
#include <QMessageBox>
#include <QDateTime>
#include "../../Common/Log.h"

extern quint64 GetID(ushort usStationID, uchar ucSubSysID, ushort usDeviceID, ushort usType,
              uchar ucSn);
USBBBE_PMModuSetGUIDlg::USBBBE_PMModuSetGUIDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::USBBBE_PMModuSetGUIDlg)
{
    ui->setupUi(this);
    m_pStateMgr = NULL;
    m_pILxTsspMessageManager = NULL;
    m_pILxTsspLogManager = NULL;
    m_pPublicInfo = NULL;
    m_pHAManager = NULL;
    m_MCSUSBUpModulStatus = NULL;
    m_doppler = false;
    m_ctlCmd = 0;
    m_pCtlCmd = NULL;
    setAttribute(Qt::WA_DeleteOnClose);
}

USBBBE_PMModuSetGUIDlg::~USBBBE_PMModuSetGUIDlg()
{
    killTimer(m_TimerID);
    delete ui;
}

int USBBBE_PMModuSetGUIDlg::msgBoxFunction(QMessageBox &msgBox, QString msg, BOOL ret)
{
    msgBox.setText(msg);
    msgBox.exec();
    return ret;
}
void USBBBE_PMModuSetGUIDlg::setText(const QString& strTitle, ParaStruct xmlsetText[3][60], QString MapMean[50][30], int MapNum[50], uchar Type, ILxTsspParameterMacro* pMacroObj)
{
    m_strTitle = strTitle;
    setWindowTitle(strTitle);

    memcpy(m_DlgParaSetStruct, xmlsetText, sizeof(ParaStruct) * 50 * 3);

    for(int i=0;i<50;i++)
    {
        for(int j=0; j<30; j++)
        {
            m_DlgMapMean[i][j] = MapMean[i][j];
        }
    }

    memset(DlgMapLineNum, 0, 50);
    for(int i=0; i<49; i++)
    {
        DlgMapLineNum[i] = MapNum[i];
    }
    //初始化界面
    InitDLG(Type, pMacroObj);
}

//初始化对话框
void USBBBE_PMModuSetGUIDlg::InitDLG(int Type, ILxTsspParameterMacro* pMacroObj)
{
    iParaSetFlag = Type;
    //设置输入格式
    InitInput();
    //显示变量名字和范围
    ShowControls();

    if(Type == 0)//取当前参数
    {
        //取当前设置的参数
        QByteArray byteArray;
        if(m_pHAManager!=NULL)
        {
            m_pHAManager->GetParameter(m_ParaMCSUSBUpModulID, byteArray);
            memcpy(&m_MCSUSBUpModulParam, byteArray.data(), sizeof(MCSUSBUpModulParamStruct));
        }
        if(m_pStateMgr != NULL)
        {
            DWORD stLen = sizeof(MCSUSBUpModulStatusStruct);
            m_MCSUSBUpModulStatus = (MCSUSBUpModulStatusStruct*)m_pStateMgr->FindOneItem(m_StatusParaMCSUSBUpModulID, stLen);
            if(m_MCSUSBUpModulStatus == NULL || stLen != sizeof(MCSUSBUpModulStatusStruct))
                return;
        }
        //启动定时器
        m_TimerID = startTimer(500);
        //不使能
        EnabledControls(false);

        EnabledControlsTeleS( false);//遥控
        EnabledControlsTeleCtrl(false);//话音
        EnabledControlsTeleSourceS(false);//遥测模拟元
        EnabledControls12(false);//遥测
        EnabledControlsDoppler(false);//多普勒
    }
    else if(Type == 1)//取宏里参数
    {
        ui->pushButton_Modify->setText("");
        if(pMacroObj!=NULL)
        {
            QByteArray byteArray;
            pMacroObj->GetParameterBlock(m_ParaMCSUSBUpModulID, byteArray);
            memcpy(&m_MCSUSBUpModulParam, byteArray.data(), sizeof(MCSUSBUpModulParamStruct));
        }
//        ui->pushButton_DopplerModify->setVisible(false);
//        ui->pushButton_DopplerSetting->setVisible(false);
        ui->groupBox_SimulSourceExportCtrl->hide();
        ui->groupBox_SingleToneCtrl->hide();
        ui->pushButton_Set->hide();
        ui->pushButton_Modify->hide();

        //显示参数
        ShowPara(1);
        on_comboBox_VoiceSourceOutput_currentIndexChanged(ui->comboBox_VoiceSourceOutput->currentIndex());
    }
    ui->groupBox_SingleToneCtrl->hide();
}

//设置输入格式
void USBBBE_PMModuSetGUIDlg::InitInput()
{
    //正整数
    QRegExp regExp("-?[0-9\\.]{1,16}");
    QRegExp regExp0("-?[0-9]{1,8}");
    //多普勒预置频率
    QRegExp regExpDoppletResFre("-?[0-9]{1,3}");
    ui->lineEdit_DoppletResFre->setValidator(new QRegExpValidator(regExpDoppletResFre, this));
    //载波输出电平
    QRegExp regExpOutputEl("0|-[0-7][0-9]|-80");
    ui->lineEdit_OutputEl->setValidator(new QRegExpValidator(regExpOutputEl, this));
    QRegExp regExpTZD("[01](\\.[0-9])?");
    //测距主音调制度
    ui->lineEdit_MainTone->setValidator(new QRegExpValidator(regExpTZD, this));
    //测距次音调制度
    ui->lineEdit_SubTone->setValidator(new QRegExpValidator(regExpTZD, this));
    //遥测模拟源调制度
    ui->lineEdit_TelemeterSourceSys->setValidator(new QRegExpValidator(regExpTZD, this));
    //遥控调制度
    ui->lineEdit_TelectrlSys->setValidator(new QRegExpValidator(regExpTZD, this));
    //话音调制度
    ui->lineEdit_WordToneSys->setValidator(new QRegExpValidator(regExp, this));
    //扫描速率
    QRegExp regExpSweepSpeed("-?[0-9]{1,3}(\\.[0-9]{3,3})?");
    ui->lineEdit_SweepSpeed->setValidator(new QRegExpValidator(regExpSweepSpeed, this));
    //扫描范围
    QRegExp regExpSweepRange("[0-9]{1,3}");
    ui->lineEdit_SweepRange->setValidator(new QRegExpValidator(regExpSweepRange, this));
    //输出载噪比
    QRegExp regExpOutputRatio("[0-9]{1,3}");
    ui->lineEdit_OutputRatio->setValidator(new QRegExpValidator(regExpOutputRatio, this));
    //模拟多普勒范围
    ui->lineEdit_DopplerRange->setValidator(new QRegExpValidator(regExp0, this));
    //模拟多普勒变化率
    ui->lineEdit_DopplerRate->setValidator(new QRegExpValidator(regExp0, this));

    //字母数字
    QRegExp regExp2("[A-Fa-f0-9]{1,8}");
    /*//帧同步码组[HEX]
    ui->lineEdit_ZTMZ->setValidator(new QRegExpValidator(regExp2, this));
    //副帧同步码组[HEX]
    ui->lineEdit_FZTBMZ->setValidator(new QRegExpValidator(regExp2, this));*/
}

void USBBBE_PMModuSetGUIDlg::ShowControls()
{
    //向下拉框控件中写入从xml中读取的内容
    int i;
    //载波输出
    for(i=0; i<DlgMapLineNum[0];i++)
    {
        ui->comboBox_OutputWave->addItem(m_DlgMapMean[0][i]);
    }
    ui->comboBox_OutputWave->setCurrentIndex(m_DlgParaSetStruct[0][0].InitVal.lval);

    //测距加调
    for(i=0; i<DlgMapLineNum[1];i++)
    {
        ui->comboBox_MeterAdd->addItem(m_DlgMapMean[1][i]);
    }
    ui->comboBox_MeterAdd->setCurrentIndex(m_DlgParaSetStruct[0][3].InitVal.lval);
    //遥控加调
    for(i=0; i<DlgMapLineNum[2];i++)
    {
        ui->comboBox_TelemeterAdd->addItem(m_DlgMapMean[2][i]);
    }
    ui->comboBox_TelemeterAdd->setCurrentIndex(m_DlgParaSetStruct[0][4].InitVal.lval);
    //话音加调
    for(i=0; i<DlgMapLineNum[3];i++)
    {
        ui->comboBox_VoiceAdd->addItem(m_DlgMapMean[3][i]);
    }
    ui->comboBox_VoiceAdd->setCurrentIndex(m_DlgParaSetStruct[0][5].InitVal.lval);
    //盲发开关
    for(i=0; i<DlgMapLineNum[4];i++)
    {
        ui->comboBox_BlindingSw->addItem(m_DlgMapMean[4][i]);
    }
    ui->comboBox_BlindingSw->setCurrentIndex(m_DlgParaSetStruct[0][6].InitVal.lval);
    //噪声源输出
    for(i=0; i<DlgMapLineNum[5];i++)
    {
        ui->comboBox_VoiceSourceOutput->addItem(m_DlgMapMean[5][i]);
    }
    ui->comboBox_VoiceSourceOutput->setCurrentIndex(m_DlgParaSetStruct[0][14].InitVal.lval);
    //遥测模拟源加调
    for(i=0; i<DlgMapLineNum[7];i++)
    {
        ui->comboBox_TelemeterSourceAdd->addItem(m_DlgMapMean[7][i]);
    }
    ui->comboBox_TelemeterSourceAdd->setCurrentIndex(m_DlgParaSetStruct[1][1].InitVal.lval);
    //模拟开关
    for(i=0; i<DlgMapLineNum[8];i++)
    {
        ui->comboBox_AnologSwitch->addItem(m_DlgMapMean[8][i]);
    }
    ui->comboBox_AnologSwitch->setCurrentIndex(m_DlgParaSetStruct[1][2].InitVal.lval);
    //测距发单音
    for(i=0; i<DlgMapLineNum[6];i++)
    {
        ui->comboBox_cj->addItem(m_DlgMapMean[6][i]);
    }
    ui->comboBox_cj->setCurrentIndex(m_DlgParaSetStruct[1][0].InitVal.lval);
    //遥控发单音
    ui->radioButton_tcStop->setChecked(true);
    //遥测模拟源发单音
    ui->radioButton_tmSimuStop->setChecked(true);

    //模拟多普勒范围
    ui->lineEdit_DopplerRange->setText(QString::number(m_DlgParaSetStruct[1][3].InitVal.lval));
    //模拟多普勒变化率
    ui->lineEdit_DopplerRate->setText(QString::number(m_DlgParaSetStruct[1][4].InitVal.lval));



    //往静态控件中写入参数缺省值
    //多普勒预置频率
    ui->lineEdit_DoppletResFre->setText(QString::number(m_DlgParaSetStruct[0][1].InitVal.lval));
    ui->label_DoppletResFre->setText("多普勒预置频率\n[" + QString::number(m_DlgParaSetStruct[0][1].MinVal.lmin) +
            "~" + QString::number(m_DlgParaSetStruct[0][1].MaxVal.lmax) + "]");
    //载波输出电平
    ui->lineEdit_OutputEl->setText(QString::number(m_DlgParaSetStruct[0][2].InitVal.lval));
    ui->label_OutputEl->setText("载波输出电平\n[" + QString::number(m_DlgParaSetStruct[0][2].MinVal.lmin) +
            "~" + QString::number(m_DlgParaSetStruct[0][2].MaxVal.lmax) + "]");
    //测距主音调制度
    ui->lineEdit_MainTone->setText(QString::number(m_DlgParaSetStruct[0][7].InitVal.lval));
    ui->label_MainTone->setText("测距主音调制度\n[" + QString::number(m_DlgParaSetStruct[0][7].MinVal.lmin / 100.0) +
            "~" + QString::number(m_DlgParaSetStruct[0][7].MaxVal.lmax / 100.0) + "]");
    //测距次音调制度
    ui->lineEdit_SubTone->setText(QString::number(m_DlgParaSetStruct[0][8].InitVal.lval));
    ui->label_SubTone->setText("测距次音调制度\n[" + QString::number(m_DlgParaSetStruct[0][8].MinVal.lmin / 100.0) +
            "~" + QString::number(m_DlgParaSetStruct[0][8].MaxVal.lmax / 100.0) + "]");
    //遥测模拟源调制度
    ui->lineEdit_TelemeterSourceSys->setText(QString::number(m_DlgParaSetStruct[0][9].InitVal.lval));
    ui->label_TelemeterSourceSys->setText("遥测模拟源调制度\n[" + QString::number(m_DlgParaSetStruct[0][9].MinVal.lmin / 100.0) +
            "~" + QString::number(m_DlgParaSetStruct[0][9].MaxVal.lmax / 100.0) + "]");
    //遥控调制度
    ui->lineEdit_TelectrlSys->setText(QString::number(m_DlgParaSetStruct[0][10].InitVal.lval));
    ui->label_TelectrlSys->setText("遥控调制度\n[" + QString::number(m_DlgParaSetStruct[0][10].MinVal.lmin / 100.0) +
            "~" + QString::number(m_DlgParaSetStruct[0][10].MaxVal.lmax / 100.0) + "]");
    //话音调制度
    ui->lineEdit_WordToneSys->setText(QString::number(m_DlgParaSetStruct[0][11].InitVal.lval));
    ui->label_WordToneSys->setText("话音调制度\n[" + QString::number(m_DlgParaSetStruct[0][11].MinVal.lmin / 100.0) +
            "~" + QString::number(m_DlgParaSetStruct[0][11].MaxVal.lmax / 100.0) + "]");
    //扫描速率
    ui->lineEdit_SweepSpeed->setText(QString::number(m_DlgParaSetStruct[0][12].InitVal.lval));
    ui->label_SweepSpeed->setText("扫描速率\n[" + QString::number(m_DlgParaSetStruct[0][12].MinVal.lmin) +
            "~" + QString::number(m_DlgParaSetStruct[0][12].MaxVal.lmax) + "]");
    //扫描范围
    ui->lineEdit_SweepRange->setText(QString::number(m_DlgParaSetStruct[0][13].InitVal.lval));
    ui->label_SweepRange->setText("扫描范围\n[" + QString::number(m_DlgParaSetStruct[0][13].MinVal.lmin) +
            "~" + QString::number(m_DlgParaSetStruct[0][13].MaxVal.lmax) + "]");
    //输出载噪比
    ui->lineEdit_OutputRatio->setText(QString::number(m_DlgParaSetStruct[0][15].InitVal.lval));
    ui->label_OutputRatio->setText("输出载噪比\n[" + QString::number(m_DlgParaSetStruct[0][15].MinVal.lmin) +
            "~" + QString::number(m_DlgParaSetStruct[0][15].MaxVal.lmax) + "]");
    //模拟多普勒范围
    ui->lineEdit_DopplerRange->setText(QString::number(m_DlgParaSetStruct[1][3].InitVal.lval));
    ui->label_DopplerRange->setText("模拟多普勒范围\n[" + QString::number(m_DlgParaSetStruct[1][3].MinVal.lmin) +
            "~" + QString::number(m_DlgParaSetStruct[1][3].MaxVal.lmax) + "]kHz");
    //模拟多普勒变化率
    ui->lineEdit_DopplerRate->setText(QString::number(m_DlgParaSetStruct[1][4].InitVal.lval));
    ui->label_DopplerRate->setText("模拟多普勒变化率\n[" + QString::number(m_DlgParaSetStruct[1][4].MinVal.lmin * 1000) +
            "~" + QString::number(m_DlgParaSetStruct[1][4].MaxVal.lmax) + "]Hz/s");
}

//显示参数
void USBBBE_PMModuSetGUIDlg::ShowPara(int type)
{
    if(type == 0)       //参数设置
    {
        if(m_MCSUSBUpModulStatus != NULL)
        {
            m_MCSUSBUpModulParam = m_MCSUSBUpModulStatus->tParams;
        }
        else
        {
            DWORD stLen = sizeof(MCSUSBUpModulStatusStruct);
            if(m_pStateMgr != NULL)
                m_MCSUSBUpModulStatus  = (MCSUSBUpModulStatusStruct*)m_pStateMgr->FindOneItem(m_StatusParaMCSUSBUpModulID, stLen);
        }
    }


    //载波输出
    ui->comboBox_OutputWave->setCurrentIndex(m_MCSUSBUpModulParam.ucWaveOutCtrl - 1);
    //测距加调
    ui->comboBox_MeterAdd->setCurrentIndex(m_MCSUSBUpModulParam.ucToneSw - 1);
    //遥控加调
    ui->comboBox_TelemeterAdd->setCurrentIndex(m_MCSUSBUpModulParam.ucTcSw - 1);
    //话音加调
    ui->comboBox_VoiceAdd->setCurrentIndex(m_MCSUSBUpModulParam.ucAudioSw - 1);
    //盲发开关
    ui->comboBox_BlindingSw->setCurrentIndex(m_MCSUSBUpModulParam.ucTcCntr - 1);
    //噪声源输出
    ui->comboBox_VoiceSourceOutput->setCurrentIndex(m_MCSUSBUpModulParam.ucNoiseSw - 1);
    //遥测模拟源加调
    ui->comboBox_TelemeterSourceAdd->setCurrentIndex(m_MCSUSBUpModulParam.ucTmsimSw - 1);
    //模拟开关
//    ui->comboBox_AnologSwitch->setCurrentIndex(m_MCSUSBUpModulParam.ucSimuSw);

    //多普勒预置频率
    ui->lineEdit_DoppletResFre->setText(QString::number(m_MCSUSBUpModulParam.ucPreDplValue));
    //载波输出电平
    ui->lineEdit_OutputEl->setText(QString::number(m_MCSUSBUpModulParam.cWaveOutLevel));
    //测距主音调制度
    QString tmp = QString("%1").arg(m_MCSUSBUpModulParam.ucT0Index / 100.0);
    ui->lineEdit_MainTone->setText(tmp);
    //测距次音调制度
    QString tmp1 = QString("%1").arg(m_MCSUSBUpModulParam.ucTnIndex / 100.0);
    ui->lineEdit_SubTone->setText(tmp1);
    //遥测模拟源调制度
    QString tmp2 = QString("%1").arg(m_MCSUSBUpModulParam.ucTmsimIndex / 100.0);
    ui->lineEdit_TelemeterSourceSys->setText(tmp2);
    //遥控调制度
    QString tmp3 = QString("%1").arg(m_MCSUSBUpModulParam.ucTcIndex / 100.0);
    ui->lineEdit_TelectrlSys->setText(tmp3);
    //话音调制度
    QString tmp4 = QString("%1").arg(m_MCSUSBUpModulParam.ucAudioIndex / 100.0);
    ui->lineEdit_WordToneSys->setText(tmp4);
    //扫描速率
    QString tmp5 = QString("%1").arg(m_MCSUSBUpModulParam.uiScanRate / 1000.0);
    ui->lineEdit_SweepSpeed->setText(tmp5);
    //扫描范围
    ui->lineEdit_SweepRange->setText(QString::number(m_MCSUSBUpModulParam.uiScanRange));
    //输出载噪比
    ui->lineEdit_OutputRatio->setText(QString::number(m_MCSUSBUpModulParam.ucCN0));
    updateDopplerValue();
}

//设置帧长范围
void USBBBE_PMModuSetGUIDlg::SetFrameLenRange(int iWordLen)
{
   /* if(iWordLen ==8)
    {
        m_DlgParaSetStruct[1][3].MinVal.lmin = 4;
        m_DlgParaSetStruct[1][3].MaxVal.lmax = 4096;
    }
    else
    {
        m_DlgParaSetStruct[1][3].MinVal.lmin = 4;
        m_DlgParaSetStruct[1][3].MaxVal.lmax = 2048;
    }
    QString strTemp = "帧长["+QString::number(m_DlgParaSetStruct[1][3].MinVal.lmin)+"~"+
            QString::number(m_DlgParaSetStruct[1][3].MaxVal.lmax)+"]字";
    ui->label_FrameLen->setText(strTemp);*/
}

void USBBBE_PMModuSetGUIDlg::SetObjInfo(TLxTsspObjectInfo ObjectInfo)
{
    memcpy(&m_ObjectInfo, &ObjectInfo, sizeof(TLxTsspObjectInfo));
    //对象初始化
    //查找对象管理器
    LxTsspObjectManager* pObjectManager = LxTsspObjectManager::Instance();

    if(pObjectManager == NULL){
        return;
    }

    m_pHAManager = pObjectManager->GetHAManager();

    bool bRet = m_DeviceMap.loadXML();
    if (!bRet)
    {
        QString strInfo = QString("load device map failed %1");
        CLog::addLog(strInfo);
        return;
    }

    ITEM *pItem = m_DeviceMap.getItem(MODULENAME);
    if (pItem->vSubItem.size() > 0)
    {
        SUBITEM& sItem = *(pItem->getSubItemByDeviceID_SN(m_ObjectInfo.usLocalDID, m_ObjectInfo.ucSN));
        m_usC = sItem.usCMD;
        m_usTID = sItem.usTID;
        /*************************框架**********************************/
        m_ParaMCSUSBUpModulID = GetID(sItem.usStationID, sItem.ucSubSysID, sItem.usDeviceID,sItem.usTID,
                                          sItem.ucSN);
        /*************************框架**********************************/

        /*************************状态**********************************/
        m_StatusParaMCSUSBUpModulID = GetID(sItem.usStationID, sItem.ucSubSysID, sItem.usDeviceID,sItem.usStateType,
                                          sItem.ucSN);
        /*************************状态**********************************/
    }

    //获取发送数传控制命令的cmd和type
    ITEM *pItemCom = m_DeviceMap.getItem(CMDNAME);
    if (pItemCom != NULL)
    {
        //过程控制命令的实现还需要规划
        int index = 2;
        m_sItem = pItemCom->vSubItem.at(index);
        if(m_sItem.usDeviceID != m_ObjectInfo.usLocalDID)
            m_sItem = pItemCom->vSubItem.at(index+1);
        m_ctlCmd = GetID(m_sItem.usStationID, m_sItem.ucSubSysID, m_sItem.usDeviceID, m_sItem.usStateType,
                         m_sItem.ucSN);
    }
    //日志
    m_pILxTsspLogManager = pObjectManager->GetLogManager();
    m_pStateMgr = pObjectManager->GetStateManager();
    m_pILxTsspMessageManager = pObjectManager->GetMessageManager();
    m_pPublicInfo = pObjectManager->GetPublicInfoManager();
}



//保存参数宏
int USBBBE_PMModuSetGUIDlg::SaveToMacro(ILxTsspParameterMacro* pMacroObj)
{
    //判断合法性
    int iRet = CheckPara();
    if(iRet == -1)
    {
        return -1;
    }
    else
    {
        GetPara();
        //取当前设置的参数
        QByteArray byteArray;

        //副帧同步器参数
        byteArray.resize(sizeof(m_MCSUSBUpModulParam));
        memcpy(byteArray.data(),&m_MCSUSBUpModulParam,sizeof(m_MCSUSBUpModulParam));
        pMacroObj->UpdateParameterBlock(m_ParaMCSUSBUpModulID,byteArray);

        //日志信息
        TLxTsspLog tmpLog;
        QString strLog;
        tmpLog.ucWarningLevel  = 1;
        strLog += "数传解调参数设置: 保存宏成功！";
        CLog::addLog(strLog, 1);
    }
}
//从界面取参数
void USBBBE_PMModuSetGUIDlg::GetPara()
{

    //载波输出
    m_MCSUSBUpModulParam.ucWaveOutCtrl = ui->comboBox_OutputWave->currentIndex() + 1;
    //测距加调
    m_MCSUSBUpModulParam.ucToneSw = ui->comboBox_MeterAdd->currentIndex() + 1;
    //遥控加调
    m_MCSUSBUpModulParam.ucTcSw = ui->comboBox_TelemeterAdd->currentIndex() + 1;
    //话音加调
    m_MCSUSBUpModulParam.ucAudioSw = ui->comboBox_VoiceAdd->currentIndex() + 1;
    //忙伐开关
    m_MCSUSBUpModulParam.ucTcCntr = ui->comboBox_BlindingSw->currentIndex() + 1;
    //噪声源输出
    m_MCSUSBUpModulParam.ucNoiseSw = ui->comboBox_VoiceSourceOutput->currentIndex() + 1;
    //遥测模拟源加调
    m_MCSUSBUpModulParam.ucTmsimSw = ui->comboBox_TelemeterSourceAdd->currentIndex() + 1;
    //模拟开关
//    m_MCSUSBUpModulParam.ucSimuSw = ui->comboBox_AnologSwitch->currentIndex();
    //多普勒预置频率
    m_MCSUSBUpModulParam.ucPreDplValue = ui->lineEdit_DoppletResFre->text().toInt();
    //载波输出电平
    m_MCSUSBUpModulParam.cWaveOutLevel = ui->lineEdit_OutputEl->text().toInt();
    //侧音主音调制度
    m_MCSUSBUpModulParam.ucT0Index = qRound(ui->lineEdit_MainTone->text().toFloat() * 100);
    //侧音次音调制度
    m_MCSUSBUpModulParam.ucTnIndex = qRound(ui->lineEdit_SubTone->text().toFloat() * 100);
    //遥测模拟源调制度
    m_MCSUSBUpModulParam.ucTmsimIndex = qRound(ui->lineEdit_TelemeterSourceSys->text().toFloat() * 100);
    //遥控调制度
    m_MCSUSBUpModulParam.ucTcIndex = qRound(ui->lineEdit_TelectrlSys->text().toFloat() * 100);
    //话音调制度
    m_MCSUSBUpModulParam.ucAudioIndex = qRound(ui->lineEdit_WordToneSys->text().toFloat() * 100);
    //扫描速率
    m_MCSUSBUpModulParam.uiScanRate = qRound(ui->lineEdit_SweepSpeed->text().toFloat() * 1000);
    //扫描范围
    m_MCSUSBUpModulParam.uiScanRange = ui->lineEdit_SweepRange->text().toInt();
    //输出载噪比
    m_MCSUSBUpModulParam.ucCN0 = ui->lineEdit_OutputRatio->text().toInt();
    //多普勒预置范围
//    m_MCSUSBUpModulParam.uiScanRange = ui->lineEdit_DopplerRange->text().toInt();
    //多普勒预置频率
//    m_MCSUSBUpModulParam.uiScanRate = ui->lineEdit_DopplerRate->text().toInt();

}

//验证参数范围
int USBBBE_PMModuSetGUIDlg::CheckPara()
{
    QMessageBox msgBox;
    msgBox.setWindowTitle(MODULENAME);
    GetPara();

    if(ui->lineEdit_OutputEl->text().isEmpty())
    {
        return msgBoxFunction(msgBox,"载波输出电平输入为空，请重新输入!", FALSE);
    }
    if(ui->lineEdit_DoppletResFre->text().isEmpty())
    {
        return msgBoxFunction(msgBox,"多普勒预置频率输入为空，请重新输入!", FALSE);
    }
    if(ui->lineEdit_SweepSpeed->text().isEmpty())
    {
        return msgBoxFunction(msgBox,"扫描速率输入为空，请重新输入!", FALSE);
    }
    if(ui->lineEdit_SweepRange->text().isEmpty())
    {
        return msgBoxFunction(msgBox,"扫描范围输入为空，请重新输入!", FALSE);
    }

    //测距主音调制度
    if(ui->lineEdit_MainTone->text().isEmpty())
    {
        return msgBoxFunction(msgBox,"测距主音调制度输入为空，请重新输入!", FALSE);;
    }
    //测距次音调制度
    if(ui->lineEdit_SubTone->text().isEmpty())
    {
        return msgBoxFunction(msgBox,"测距次音调制度输入为空，请重新输入!", FALSE);;
    }
    //遥测模拟源调制度
    if(ui->lineEdit_TelemeterSourceSys->text().isEmpty())
    {
        return msgBoxFunction(msgBox,"遥测模拟源调制度输入为空，请重新输入!", FALSE);;
    }
    //遥控调制度
    if(ui->lineEdit_TelectrlSys->text().isEmpty())
    {
        return msgBoxFunction(msgBox,"遥控调制度输入为空，请重新输入!", FALSE);;
    }

    if(ui->lineEdit_DopplerRange->text().isEmpty())
    {
        return msgBoxFunction(msgBox,"模拟多普勒范围输入为空，请重新输入!", FALSE);
    }
    if(ui->lineEdit_DopplerRate->text().isEmpty())
    {
        return msgBoxFunction(msgBox,"模拟多普勒变化率输入为空，请重新输入!", FALSE);
    }

    if(ui->lineEdit_OutputRatio->text().isEmpty())
    {
        return msgBoxFunction(msgBox,"输出载噪比输入为空，请重新输入!", FALSE);
    }


    if(m_MCSUSBUpModulParam.cWaveOutLevel  < -80 || m_MCSUSBUpModulParam.cWaveOutLevel > 0)
    {
        msgBox.setText("载波输出电平超出范围为-80～0dBm!");
        msgBox.exec();
        return -1;
    }    
    if(m_MCSUSBUpModulParam.ucPreDplValue  < -180 || m_MCSUSBUpModulParam.ucPreDplValue > 180)
    {
        msgBox.setText("多普勒预置频率超出范围为-180～180KHz!");
        msgBox.exec();
        return -1;
    }
    if(m_MCSUSBUpModulParam.ucToneSw  < 1 || m_MCSUSBUpModulParam.ucTcSw > 2)
    {
        msgBox.setText("遥控加调控制取值为1或2!");
        msgBox.exec();
        return -1;
    }
    if((int)m_MCSUSBUpModulParam.ucT0Index  < 20 || (int)m_MCSUSBUpModulParam.ucT0Index > 150)
    {
        msgBox.setText("主音调制取值范围为0.2～1.5rad!");
        msgBox.exec();
        return -1;
    }
    if((int)m_MCSUSBUpModulParam.ucTnIndex  < 20 || (int)m_MCSUSBUpModulParam.ucTnIndex > 150)
    {
        msgBox.setText("次音调制取值范围为0.2～1.5rad!");
        msgBox.exec();
        return -1;
    }
    if((int)m_MCSUSBUpModulParam.ucTcIndex < 20 || (int)m_MCSUSBUpModulParam.ucTcIndex > 150)
    {
        msgBox.setText("遥控调制取值范围为0.2～1.5rad!");
        msgBox.exec();
        return -1;
    }
    if((int)m_MCSUSBUpModulParam.ucAudioIndex  < 20 || (int)m_MCSUSBUpModulParam.ucAudioIndex > 150)
    {
        msgBox.setText("话音调制取值范围为0.2～1.5rad!");
        msgBox.exec();
        return -1;
    }
    if((int)m_MCSUSBUpModulParam.ucTmsimIndex  < 20 || (int)m_MCSUSBUpModulParam.ucTmsimIndex > 150)
    {
        msgBox.setText("遥测模拟源调制取值范围为0.2～1.5rad!");
        msgBox.exec();
        return -1;
    }
    if((int)m_MCSUSBUpModulParam.uiScanRate  < -180000 || (int)m_MCSUSBUpModulParam.uiScanRate > 180000)
    {
        msgBox.setText("扫描速率取值范围为-180kHz～180kHz/s!");
        msgBox.exec();
        return -1;
    }
    if(m_MCSUSBUpModulParam.uiScanRange  < 0 || m_MCSUSBUpModulParam.uiScanRange > 180)
    {
        msgBox.setText("扫描范围取值范围为0～180KHz!");
        msgBox.exec();
        return -1;
    }
    if(m_MCSUSBUpModulParam.ucCN0  < 0 || m_MCSUSBUpModulParam.ucCN0 > 100)
    {
        msgBox.setText("载波输出载噪比取值范围为0～100dBHz!");
        msgBox.exec();
        return -1;
    }


   return 1;

}

void USBBBE_PMModuSetGUIDlg::EnabledControlsParamSet(bool bFlag)
{
    if (bFlag)
    {
        ui->pushButton_Set->setText("恢复");
    }
    else
    {
        ui->pushButton_Set->setText("更改");
    }
    //参数设置按钮
    ui->pushButton_Set->setEnabled(bFlag);
    //载波输出
    ui->comboBox_OutputWave->setEnabled(bFlag);
    //多普勒预置频率
    ui->lineEdit_DoppletResFre->setEnabled(bFlag);
    //载波输出电平
    ui->lineEdit_OutputEl->setEnabled(bFlag);
    //测距主音调制度
    ui->lineEdit_MainTone->setEnabled(bFlag);
    //测距次音调制度
    ui->lineEdit_SubTone->setEnabled(bFlag);
    //测距模拟源制度
    ui->lineEdit_TelemeterSourceSys->setEnabled(bFlag);
    //遥控调制度
    ui->lineEdit_TelectrlSys->setEnabled(bFlag);
    //话音调制度
    ui->lineEdit_WordToneSys->setEnabled(bFlag);
    //测距加调
    ui->comboBox_MeterAdd->setEnabled(bFlag);
    //遥控加调
    ui->comboBox_TelemeterAdd->setEnabled(bFlag);
    //话音加调
    ui->comboBox_VoiceAdd->setEnabled(bFlag);
    //盲发开关
    ui->comboBox_BlindingSw->setEnabled(bFlag);
    //扫描速率
    ui->lineEdit_SweepSpeed->setEnabled(bFlag);
    //扫描范围
    ui->lineEdit_SweepRange->setEnabled(bFlag);
    //噪声源输出
    ui->comboBox_VoiceSourceOutput->setEnabled(bFlag);
    //输出载噪比
    ui->lineEdit_OutputRatio->setEnabled(bFlag);

    /*************************Group1*************************/
}

//定时器
void USBBBE_PMModuSetGUIDlg::timerEvent(QTimerEvent *event)
{
    ShowPara(0);
}




void USBBBE_PMModuSetGUIDlg::closeEvent(QCloseEvent *event)
{
    event->accept();
}

QSize USBBBE_PMModuSetGUIDlg::sizeHint() const
{
    return QSize(this->width(), this->height());
}

/*
//副帧方式响应函数
void USBBBE_PMModuSetGUIDlg::on_comboBox_SubFrmType_currentIndexChanged(int index)
{
//    //副帧方式控件使能
//    if (ui->pushButton_Modify->text() != "更改" || iParaSetFlag != 0)
//    {
//        EnabledControlsSubFrmType(index);   //副帧方式控件使能
//    }
}

//根据副帧类型，使能副帧控件
void USBBBE_PMModuSetGUIDlg::EnabledControlsSubFrmType(int iSubFrm)
{
//    switch(iSubFrm)
//    {
//        case 0:     //无副帧
//            SubFrmEn(false);            //副帧通用
//            IDSubFrmEn(false);          //ID副帧
//            CycSubFrmEn(false);         //循环副帧
//            ReserveSubFrmEn(false);     //反码副帧
//            break;
//        case 1:     //ID副帧
//            SubFrmEn(true);             //副帧通用
//            IDSubFrmEn(true);           //ID副帧
//            CycSubFrmEn(false);         //循环副帧
//            ReserveSubFrmEn(false);     //反码副帧
//            break;
//        case 2:     //循环副帧
//            SubFrmEn(true);             //副帧通用
//            IDSubFrmEn(false);          //ID副帧
//            CycSubFrmEn(true);          //循环副帧
//            ReserveSubFrmEn(false);     //反码副帧
//            break;
//        case 3:     //反码副帧
//            SubFrmEn(true);         //副帧通用
//            IDSubFrmEn(false);      //ID副帧
//            CycSubFrmEn(false);     //循环副帧
//            ReserveSubFrmEn(true);  //反码副帧
//            break;
//        default:
//            break;
//    }
}*/

void USBBBE_PMModuSetGUIDlg::SubFrmEn(bool bFlag)
{
    //副帧长度
//    ui->label_SubFrmLen->setEnabled(bFlag);
//    ui->lineEdit_SubFrmLen->setEnabled(bFlag);

//    ui->label_CFSearchErrBits->setEnabled(bFlag);
//    ui->comboBox_CFSearchErrBits->setEnabled(bFlag);
//    ui->label_CFVerifyErrBits->setEnabled(bFlag);
//    ui->comboBox_CFVerifyErrBits->setEnabled(bFlag);
//    ui->label_CFLockErrBits->setEnabled(bFlag);
//    ui->comboBox_CFLockErrBits->setEnabled(bFlag);
//    ui->label_CFVerifyCheckFrame->setEnabled(bFlag);
//    ui->comboBox_SubFVerifyFrame->setEnabled(bFlag);
//    ui->label_CFLockCheckFrame->setEnabled(bFlag);
//    ui->comboBox_SubFLockFrame->setEnabled(bFlag);
}

//有无ID副帧
void USBBBE_PMModuSetGUIDlg::IDSubFrmEn(bool bFlag)
{
//    ui->groupBox_IDSubFrm->setEnabled(bFlag);
//    //ID副帧位置
//    ui->label_IDSubFrmPos->setEnabled(bFlag);
//    ui->lineEdit_IDSubFrmPos->setEnabled(bFlag);

//    //ID基值
//    ui->label_IdBase->setEnabled(bFlag);
//    ui->comboBox_IdBase->setEnabled(bFlag);

//    //ID字方向
//    ui->label_IdDirect->setEnabled(bFlag);
//    ui->comboBox_IdDirect->setEnabled(bFlag);
}

//有无循环副帧
void USBBBE_PMModuSetGUIDlg::CycSubFrmEn(bool bFlag)
{
    //循环副帧
//    ui->groupBox_CycSubFrm_->setEnabled(bFlag);
//    ui->label_CycFrmSyncCode->setEnabled(bFlag);
//    ui->lineEdit_CycFrmSyncCode->setEnabled(bFlag);
//    ui->label_CycFrmSynCodePos->setEnabled(bFlag);
//    ui->lineEdit_CycFrmSynCodePos->setEnabled(bFlag);
//    ui->label_CycFrmSynCodeLen->setEnabled(bFlag);
//    ui->lineEdit_CycFrmSynCodeLen->setEnabled(bFlag);
}

//有无反码副帧
void USBBBE_PMModuSetGUIDlg::ReserveSubFrmEn(bool bFlag)
{

}

void USBBBE_PMModuSetGUIDlg::SendMessage(char* pData, USHORT usLen, USHORT usC, USHORT usTID, USHORT usSN)
{
    static uint seq = 0;
    TLxTsspMessageHeader header = {0};
    TLxTsspSubMsgHeader subHeader = {0};
    header.uiSequence = seq++;
    header.uiHandle = 1;
    header.S = m_pPublicInfo->GetLocalDeviceID();
    header.L = sizeof(TLxTsspMessageHeader) +sizeof(TLxTsspSubMsgHeader) + usLen;
    header.D = m_pPublicInfo->GetLocalDeviceID();//0x09;
    header.T = 1;
    header.M = 1;
    header.P = 1;
    header.A = 0;
    header.R = 0;
    header.C = usC;

    subHeader.Len = usLen;
    subHeader.SID = m_pPublicInfo->GetLocalDeviceID();//m_DObjectInfo.usStationID;
    subHeader.DID = m_ObjectInfo.usLocalDID;
    subHeader.TID = usTID;
    subHeader.SN = m_ObjectInfo.ucSubSysID;
    subHeader.O = usSN;
    TLxTsspMessage message;
    message.pData =
            new char[sizeof(TLxTsspMessageHeader) + sizeof(TLxTsspSubMsgHeader) + usLen ];
    memcpy(message.pData, &header, sizeof(TLxTsspMessageHeader));
    memcpy(message.pData + sizeof(TLxTsspMessageHeader),
           &subHeader, sizeof(TLxTsspSubMsgHeader));
    memcpy(message.pData + sizeof(TLxTsspMessageHeader) + sizeof(TLxTsspSubMsgHeader),
           pData, usLen);
    message.iDataLen = sizeof(TLxTsspMessageHeader) + sizeof(TLxTsspSubMsgHeader) + usLen;

    int iRet = m_pILxTsspMessageManager->FireMessage(message);
    delete[] message.pData;
}

DWORDLONG USBBBE_PMModuSetGUIDlg::StrToSynCode(const char * pStr)
{
    char c,C;
    const char*  p;
    DWORDLONG dwlValue=0;
    char   DigVal=0;
    USHORT nDigCount=0,MAX_DIGITS=16; //The synchronizing code should be not longer  than 16 digitals

    p=pStr;     //Point to the first char of the string
    c = *p++;   // read the first char
    while ((c!=NULL) && (nDigCount<=MAX_DIGITS))//  Not the terminator of a string && total digitals is less than 16
    {
         if(isspace((int)(unsigned char)c))    c = *p++;// skip whitespace
         else if(c>='0'&&c<='9')                        //the char is a number between 1 and 9
         {
             DigVal=c-'0';
             dwlValue=dwlValue*16+DigVal;               //Accumulate the value of the hexal number so far to the current digital
             nDigCount++;
             c = *p++;
         }
         else if((C=toupper(c))>='A'&&C<='F')           //the char is a letter could be a digital of a hexal number
         {
             DigVal=C-'A'+10;
             dwlValue=dwlValue*16+DigVal;               //Accumulate the value of the hexal number so far to the current digital
             nDigCount++;
             c = *p++;
         }
         else    break;                                 // An invalid char that is neither whitespace nor a hexal digital is encounted
                                                        // Conclude the scanning process
    }
    return dwlValue;
}

//使能或不使能控件
void USBBBE_PMModuSetGUIDlg::EnabledControls(bool bFlag)
{
    /*************************Group1*************************/
    //载波输出
    ui->comboBox_OutputWave->setEnabled(bFlag);
    //多普勒预置频率
    ui->lineEdit_DoppletResFre->setEnabled(bFlag);
    //载波输出电平
    ui->lineEdit_OutputEl->setEnabled(bFlag);
    //测距主音调制度
    ui->lineEdit_MainTone->setEnabled(bFlag);
    //测距次音调制度
    ui->lineEdit_SubTone->setEnabled(bFlag);
    //测距模拟源制度
    ui->lineEdit_TelemeterSourceSys->setEnabled(bFlag);
    //遥控调制度
    ui->lineEdit_TelectrlSys->setEnabled(bFlag);
    //话音调制度
    ui->lineEdit_WordToneSys->setEnabled(bFlag);
    //测距加调
    ui->comboBox_MeterAdd->setEnabled(bFlag);
    //遥控加调
    ui->comboBox_TelemeterAdd->setEnabled(bFlag);
    //话音加调
    ui->comboBox_VoiceAdd->setEnabled(bFlag);
    //盲发开关
    ui->comboBox_BlindingSw->setEnabled(bFlag);
    //扫描速率
    ui->lineEdit_SweepSpeed->setEnabled(bFlag);
    //扫描范围
    ui->lineEdit_SweepRange->setEnabled(bFlag);
    //噪声源输出
    ui->comboBox_VoiceSourceOutput->setEnabled(bFlag);
    //输出载噪比
    ui->lineEdit_OutputRatio->setEnabled(bFlag);
    //参数设置按钮
    ui->pushButton_Set->setEnabled(bFlag);
    /*************************Group1*************************/

    /*************************Group3*************************/


    /*************************Group3*************************/

    /*************************帧同步器*************************/
    if (bFlag)
    {
        ui->pushButton_Modify->setText("恢复");
        ui->pushButton_Set->setEnabled(true);
    }
    else
    {
        ui->pushButton_Modify->setText("更改");
        ui->pushButton_Set->setEnabled(false);

    }
}
//遥控
void USBBBE_PMModuSetGUIDlg::EnabledControlsTeleS(bool bFlag){

     ui->comboBox_cj->setEnabled(bFlag);
     if (bFlag)
     {
         ui->pushButton_TeleSModify->setText("恢复");
         ui->pushButton_TeleSSetting->setEnabled(true);
     }
     else
     {
         ui->pushButton_TeleSModify->setText("更改");
         ui->pushButton_TeleSSetting->setEnabled(false);
     }
}
//遥测模拟源
void USBBBE_PMModuSetGUIDlg:: EnabledControlsTeleCtrl(bool bFlag){
    ui->radioButton_tcSend->setEnabled(bFlag);
    ui->radioButton_tcStop->setEnabled(bFlag);

    if (bFlag)
    {
        ui->pushButton_TeleCtrlModify->setText("恢复");
        ui->pushButton_TeleCtrlSetting->setEnabled(true);
    }
    else
    {
        ui->pushButton_TeleCtrlModify->setText("更改");
        ui->pushButton_TeleCtrlSetting->setEnabled(false);
    }

}
void USBBBE_PMModuSetGUIDlg:: EnabledControlsTeleSourceS(bool bFlag)//遥测模拟元
{


    ui->radioButton_tmSimuSend->setEnabled(bFlag);
    ui->radioButton_tmSimuStop->setEnabled(bFlag);
    if (bFlag)
    {
        ui->pushButton_TeleSourceSModify->setText("恢复");
        ui->pushButton_TeleSourceSSetting->setEnabled(true);
    }
    else
    {
        ui->pushButton_TeleSourceSModify->setText("更改");
        ui->pushButton_TeleSourceSSetting->setEnabled(false);
    }

}
void USBBBE_PMModuSetGUIDlg:: EnabledControls12(bool bFlag)//遥测
{

    //遥测模拟源加调
    ui->comboBox_TelemeterSourceAdd->setEnabled(bFlag);
    if (bFlag)
    {
        ui->pushButton_12->setText("恢复");
        ui->pushButton_pushButton_TeleSourceASetting->setEnabled(true);
    }
    else
    {
        ui->pushButton_12->setText("更改");
        ui->pushButton_pushButton_TeleSourceASetting->setEnabled(false);
    }
}
void USBBBE_PMModuSetGUIDlg:: EnabledControlsDoppler(bool bFlag)//多普勒
{
    //模拟开关
    ui->comboBox_AnologSwitch->setEnabled(bFlag);
    //模拟多普勒范围
    ui->lineEdit_DopplerRange->setEnabled(bFlag);
    //模拟多普勒变化率
    ui->lineEdit_DopplerRate->setEnabled(bFlag);

    if (bFlag)
    {
        ui->pushButton_DopplerModify->setText("恢复");
        ui->pushButton_DopplerSetting->setEnabled(true);
    }
    else
    {
        ui->pushButton_DopplerModify->setText("更改");
        ui->pushButton_DopplerSetting->setEnabled(false);
    }

}

void USBBBE_PMModuSetGUIDlg::updateDopplerValue()
{
    if(!m_doppler || 0 == m_ctlCmd || m_pStateMgr == NULL)return;//使用过过程控制命令才执行下面
    if(m_pCtlCmd)
    {
        DOPPLERSIMU& doppler = m_pCtlCmd->dSimu49;
        if(doppler.dopplerSwitch != 1 && doppler.dopplerSwitch != 2)return;//2关，1开
        ui->comboBox_AnologSwitch->setCurrentIndex(doppler.dopplerSwitch == 2 ? 0 : 1);
        ui->lineEdit_DopplerRange->setText(QString::number(doppler.range));
        ui->lineEdit_DopplerRate->setText(QString::number(doppler.rateOfChange));
        return;
    }
    DWORD stLen = sizeof(PCRDATATRANS_TTC);
    m_pCtlCmd  = (PCRDATATRANS_TTC*)m_pStateMgr->FindOneItem(m_ctlCmd, stLen);
}
//参数更改
void USBBBE_PMModuSetGUIDlg::on_pushButton_Modify_clicked()
{
    if(ui->pushButton_Modify->text() == "更改")
    {
        EnabledControls(true);
        on_comboBox_VoiceSourceOutput_currentIndexChanged(ui->comboBox_VoiceSourceOutput->currentIndex());
        killTimer(m_TimerID);


        ui->pushButton_TeleSModify->setEnabled(false);
        ui->pushButton_TeleSSetting->setEnabled(false);

        ui->pushButton_TeleCtrlModify->setEnabled(false);
        ui->pushButton_TeleCtrlSetting->setEnabled(false);

        ui->pushButton_TeleSourceSModify->setEnabled(false);
        ui->pushButton_TeleSourceSSetting->setEnabled(false);

        ui->pushButton_12->setEnabled(false);
        ui->pushButton_pushButton_TeleSourceASetting->setEnabled(false);

        ui->pushButton_DopplerModify->setEnabled(false);
        ui->pushButton_DopplerSetting->setEnabled(false);
        return;
    }
    if(ui->pushButton_Modify->text() == "恢复")
    {
        EnabledControls(false);
        m_TimerID = startTimer(500);
        ui->pushButton_TeleSModify->setEnabled(true);
        ui->pushButton_TeleSSetting->setEnabled(false);

        ui->pushButton_TeleCtrlModify->setEnabled(true);
        ui->pushButton_TeleCtrlSetting->setEnabled(false);

        ui->pushButton_TeleSourceSModify->setEnabled(true);
        ui->pushButton_TeleSourceSSetting->setEnabled(false);

        ui->pushButton_12->setEnabled(true);
        ui->pushButton_pushButton_TeleSourceASetting->setEnabled(false);

        ui->pushButton_DopplerModify->setEnabled(true);
        ui->pushButton_DopplerSetting->setEnabled(false);
        return;
    }
}
//参数设置
void USBBBE_PMModuSetGUIDlg::on_pushButton_Set_clicked()
{
    //验证参数范围
    if(CheckPara() == 1)
    {
        //从界面取参数
        GetPara();

        //设置参数
        //取当前设置的参数
        if(m_pHAManager != NULL)
        {
            QByteArray byteArray;
            byteArray.resize(sizeof(m_MCSUSBUpModulParam));
            memcpy(byteArray.data(),&m_MCSUSBUpModulParam,sizeof(MCSUSBUpModulParamStruct));
            SendMessage((char*)&m_MCSUSBUpModulParam, sizeof(MCSUSBUpModulParamStruct), m_usC, m_usTID);

        }

        //日志信息
        TLxTsspLog tmpLog;
        QString strLog;
        tmpLog.ucWarningLevel  = 1;
        strLog += QString("%1: 设置参数成功！").arg(MODULENAME);
        CLog::addLog(strLog, 1);
        //不使能控件
        EnabledControls(false);

        ui->pushButton_TeleSModify->setEnabled(true);

        ui->pushButton_TeleCtrlModify->setEnabled(true);

        ui->pushButton_TeleSourceSModify->setEnabled(true);

        ui->pushButton_12->setEnabled(true);
        ui->pushButton_DopplerModify->setEnabled(true);
        m_TimerID = startTimer(500);
    }
}
//测距单音发送  更改
void USBBBE_PMModuSetGUIDlg::on_pushButton_TeleSModify_clicked()
{
    if(ui->pushButton_TeleSModify->text() == "更改")
    {
        EnabledControlsTeleS(true);
        killTimer(m_TimerID);
        ui->pushButton_Modify->setEnabled(false);
        ui->pushButton_Set->setEnabled(false);


        ui->pushButton_TeleCtrlModify->setEnabled(false);
        ui->pushButton_TeleCtrlSetting->setEnabled(false);

        ui->pushButton_TeleSourceSModify->setEnabled(false);
        ui->pushButton_TeleSourceSSetting->setEnabled(false);

        ui->pushButton_12->setEnabled(false);
        ui->pushButton_pushButton_TeleSourceASetting->setEnabled(false);

        ui->pushButton_DopplerModify->setEnabled(false);
        ui->pushButton_DopplerSetting->setEnabled(false);
        return;
    }
    if(ui->pushButton_TeleSModify->text() == "恢复")
    {
        EnabledControlsTeleS(false);
        m_TimerID = startTimer(500);
        ui->pushButton_Modify->setEnabled(true);
        ui->pushButton_Set->setEnabled(false);

        ui->pushButton_TeleCtrlModify->setEnabled(true);
        ui->pushButton_TeleCtrlSetting->setEnabled(false);

        ui->pushButton_TeleSourceSModify->setEnabled(true);
        ui->pushButton_TeleSourceSSetting->setEnabled(false);

        ui->pushButton_12->setEnabled(true);
        ui->pushButton_pushButton_TeleSourceASetting->setEnabled(false);

        ui->pushButton_DopplerModify->setEnabled(true);
        ui->pushButton_DopplerSetting->setEnabled(false);
        return;
    }
}
//测距单音发送  设置
void USBBBE_PMModuSetGUIDlg::on_pushButton_TeleSSetting_clicked()
{
    //获取当前的值
    UCHAR cType = ui->comboBox_cj->currentIndex();

    QByteArray ba;
    ba.append(17);
    ba.append(cType);

    UCHAR ucIStart = 0 == cType ? 2 : 1;//combox:0停送  协议2停送
    ba.append(ucIStart);
    SendMessage(ba.data(), ba.size(), m_sItem.usCMD, m_sItem.usTID, m_sItem.ucSN);

    //日志信息
    TLxTsspLog tmpLog;
    QString strLog;
    tmpLog.ucWarningLevel  = 1;
    strLog += QString("%1: 控制命令发送成功！").arg(MODULENAME);
    CLog::addLog(strLog, 1);
    //不使能控件
    EnabledControlsTeleS(false);
    ui->pushButton_Modify->setEnabled(true);
    ui->pushButton_TeleCtrlModify->setEnabled(true);
    ui->pushButton_TeleSourceSModify->setEnabled(true);
    ui->pushButton_12->setEnabled(true);
    ui->pushButton_DopplerModify->setEnabled(true);
    m_TimerID = startTimer(500);
}
//遥控
void USBBBE_PMModuSetGUIDlg::on_pushButton_TeleCtrlModify_clicked()
{
    if(ui->pushButton_TeleCtrlModify->text() == "更改")
    {
        EnabledControlsTeleCtrl(true);
        killTimer(m_TimerID);
        ui->pushButton_Modify->setEnabled(false);
        ui->pushButton_Set->setEnabled(false);

        ui->pushButton_TeleSModify->setEnabled(false);
        ui->pushButton_TeleSSetting->setEnabled(false);


        ui->pushButton_TeleSourceSModify->setEnabled(false);
        ui->pushButton_TeleSourceSSetting->setEnabled(false);

        ui->pushButton_12->setEnabled(false);
        ui->pushButton_pushButton_TeleSourceASetting->setEnabled(false);

        ui->pushButton_DopplerModify->setEnabled(false);
        ui->pushButton_DopplerSetting->setEnabled(false);
        return;
    }
    if(ui->pushButton_TeleCtrlModify->text() == "恢复")
    {
        EnabledControlsTeleCtrl(false);
        m_TimerID = startTimer(500);
        ui->pushButton_Modify->setEnabled(true);
        ui->pushButton_Set->setEnabled(false);

        ui->pushButton_TeleSModify->setEnabled(true);
        ui->pushButton_TeleSSetting->setEnabled(false);


        ui->pushButton_TeleSourceSModify->setEnabled(true);
        ui->pushButton_TeleSourceSSetting->setEnabled(false);

        ui->pushButton_12->setEnabled(true);
        ui->pushButton_pushButton_TeleSourceASetting->setEnabled(false);

        ui->pushButton_DopplerModify->setEnabled(true);
        ui->pushButton_DopplerSetting->setEnabled(false);
        return;
    }
}
//话语发单音 设置
void USBBBE_PMModuSetGUIDlg::on_pushButton_TeleCtrlSetting_clicked()
{
    //验证参数范围
    if(CheckPara() == 1)
    {
        //从界面取参数
        GetPara();

        //设置参数
        //取当前设置的参数
        if(m_pHAManager != NULL)
        {
            QByteArray byteArray;
            byteArray.resize(sizeof(m_MCSUSBUpModulParam));
            memcpy(byteArray.data(),&m_MCSUSBUpModulParam,sizeof(MCSUSBUpModulParamStruct));
            SendMessage((char*)&m_MCSUSBUpModulParam, sizeof(MCSUSBUpModulParamStruct), m_usC, m_usTID);

        }

        //日志信息
        TLxTsspLog tmpLog;
        QString strLog;
        tmpLog.ucWarningLevel  = 1;
        strLog += QString("%1: 设置参数成功！").arg(MODULENAME);
        CLog::addLog(strLog, 1);
        //不使能控件
        EnabledControlsTeleCtrl(false);
        ui->pushButton_Modify->setEnabled(true);
        ui->pushButton_TeleSModify->setEnabled(true);

        ui->pushButton_TeleSourceSModify->setEnabled(true);

        ui->pushButton_12->setEnabled(true);
        ui->pushButton_DopplerModify->setEnabled(true);
        m_TimerID = startTimer(500);
    }
}


//模拟源
void USBBBE_PMModuSetGUIDlg::on_pushButton_TeleSourceSModify_clicked()
{
    if(ui->pushButton_TeleSourceSModify->text() == "更改")
    {
        EnabledControlsTeleSourceS(true);
        killTimer(m_TimerID);
        ui->pushButton_Modify->setEnabled(false);
        ui->pushButton_Set->setEnabled(false);

        ui->pushButton_TeleSModify->setEnabled(false);
        ui->pushButton_TeleSSetting->setEnabled(false);

        ui->pushButton_TeleCtrlModify->setEnabled(false);
        ui->pushButton_TeleCtrlSetting->setEnabled(false);


        ui->pushButton_12->setEnabled(false);
        ui->pushButton_pushButton_TeleSourceASetting->setEnabled(false);

        ui->pushButton_DopplerModify->setEnabled(false);
        ui->pushButton_DopplerSetting->setEnabled(false);
        return;
    }
    if(ui->pushButton_TeleSourceSModify->text() == "恢复")
    {
        EnabledControlsTeleSourceS(false);
        m_TimerID = startTimer(500);
        ui->pushButton_Modify->setEnabled(true);
        ui->pushButton_Set->setEnabled(false);

        ui->pushButton_TeleSModify->setEnabled(true);
        ui->pushButton_TeleSSetting->setEnabled(false);

        ui->pushButton_TeleCtrlModify->setEnabled(true);
        ui->pushButton_TeleCtrlSetting->setEnabled(false);


        ui->pushButton_12->setEnabled(true);
        ui->pushButton_pushButton_TeleSourceASetting->setEnabled(false);

        ui->pushButton_DopplerModify->setEnabled(true);
        ui->pushButton_DopplerSetting->setEnabled(false);
        return;
    }
}
//模拟元 设置

void USBBBE_PMModuSetGUIDlg::on_pushButton_TeleSourceSSetting_clicked()
{
    UCHAR cType = ui->radioButton_tmSimuStop->isChecked() ? 2 : 1;//协议:2是停送
    QByteArray ba;
    ba.append(20);
    ba.append(cType);
    UCHAR ucIStart = 0 == cType ? 2 : 1;//combox:0停送  协议2停送
    ba.append(ucIStart);
    SendMessage(ba.data(), ba.size(), m_sItem.usCMD, m_sItem.usTID, m_sItem.ucSN);
    //日志信息
    TLxTsspLog tmpLog;
    QString strLog;
    tmpLog.ucWarningLevel  = 1;
    strLog += QString("%1: 设置参数成功！").arg(MODULENAME);
    CLog::addLog(strLog, 1);
    //不使能控件
    EnabledControlsTeleSourceS(false);
    ui->pushButton_Modify->setEnabled(true);
    ui->pushButton_TeleSModify->setEnabled(true);
    ui->pushButton_TeleCtrlModify->setEnabled(true);
    ui->pushButton_12->setEnabled(true);
    ui->pushButton_DopplerModify->setEnabled(true);
    m_TimerID = startTimer(500);
}
//遥测模拟元 更改
void USBBBE_PMModuSetGUIDlg::on_pushButton_12_clicked()
{
    if(ui->pushButton_12->text() == "更改")
    {
        EnabledControls12(true);
        killTimer(m_TimerID);
        ui->pushButton_Modify->setEnabled(false);
        ui->pushButton_Set->setEnabled(false);

        ui->pushButton_TeleSModify->setEnabled(false);
        ui->pushButton_TeleSSetting->setEnabled(false);

        ui->pushButton_TeleCtrlModify->setEnabled(false);
        ui->pushButton_TeleCtrlSetting->setEnabled(false);

        ui->pushButton_TeleSourceSModify->setEnabled(false);
        ui->pushButton_TeleSourceSSetting->setEnabled(false);



        ui->pushButton_DopplerModify->setEnabled(false);
        ui->pushButton_DopplerSetting->setEnabled(false);
        return;
    }
    if(ui->pushButton_12->text() == "恢复")
    {
        EnabledControls12(false);
        m_TimerID = startTimer(500);

        ui->pushButton_Modify->setEnabled(true);
        ui->pushButton_Set->setEnabled(false);

        ui->pushButton_TeleSModify->setEnabled(true);
        ui->pushButton_TeleSSetting->setEnabled(false);

        ui->pushButton_TeleCtrlModify->setEnabled(true);
        ui->pushButton_TeleCtrlSetting->setEnabled(false);

        ui->pushButton_TeleSourceSModify->setEnabled(true);
        ui->pushButton_TeleSourceSSetting->setEnabled(false);



        ui->pushButton_DopplerModify->setEnabled(true);
        ui->pushButton_DopplerSetting->setEnabled(false);
        return;
    }
}
//遥测模拟元加调 设置
void USBBBE_PMModuSetGUIDlg::on_pushButton_pushButton_TeleSourceASetting_clicked()
{
    const int size = 2;
    char pBuff[size];
    pBuff[0] = char(46);
    pBuff[1] = char(ui->comboBox_TelemeterSourceAdd->currentIndex() + 1);//协议:2是去调
    SendMessage(pBuff, size, m_sItem.usCMD, m_sItem.usTID, m_sItem.ucSN);
    //不使能控件
    EnabledControls12(false);
    ui->pushButton_Modify->setEnabled(true);
    ui->pushButton_TeleSModify->setEnabled(true);
    ui->pushButton_TeleCtrlModify->setEnabled(true);
    ui->pushButton_TeleSourceSModify->setEnabled(true);
    ui->pushButton_DopplerModify->setEnabled(true);
    m_TimerID = startTimer(500);
}
//多普勒 更改
void USBBBE_PMModuSetGUIDlg::on_pushButton_DopplerModify_clicked()
{
    if(ui->pushButton_DopplerModify->text() == "更改")
    {
        EnabledControlsDoppler(true);
        killTimer(m_TimerID);
        on_comboBox_AnologSwitch_currentIndexChanged(ui->comboBox_AnologSwitch->currentIndex());
        ui->pushButton_Modify->setEnabled(false);
        ui->pushButton_Set->setEnabled(false);

        ui->pushButton_TeleSModify->setEnabled(false);
        ui->pushButton_TeleSSetting->setEnabled(false);

        ui->pushButton_TeleCtrlModify->setEnabled(false);
        ui->pushButton_TeleCtrlSetting->setEnabled(false);

        ui->pushButton_TeleSourceSModify->setEnabled(false);
        ui->pushButton_TeleSourceSSetting->setEnabled(false);

        ui->pushButton_12->setEnabled(false);
        ui->pushButton_pushButton_TeleSourceASetting->setEnabled(false);


        return;
    }
    if(ui->pushButton_DopplerModify->text() == "恢复")
    {
        EnabledControlsDoppler(false);
        m_TimerID = startTimer(500);
        ui->pushButton_Modify->setEnabled(true);
        ui->pushButton_Set->setEnabled(false);

        ui->pushButton_TeleSModify->setEnabled(true);
        ui->pushButton_TeleSSetting->setEnabled(false);

        ui->pushButton_TeleCtrlModify->setEnabled(true);
        ui->pushButton_TeleCtrlSetting->setEnabled(false);

        ui->pushButton_TeleSourceSModify->setEnabled(true);
        ui->pushButton_TeleSourceSSetting->setEnabled(false);

        ui->pushButton_12->setEnabled(true);
        ui->pushButton_pushButton_TeleSourceASetting->setEnabled(false);
        return;
    }
}
//多普勒 设置
void USBBBE_PMModuSetGUIDlg::on_pushButton_DopplerSetting_clicked()
{
    if(CheckPara() == 1)
    {
        QMessageBox msgBox;
        msgBox.setWindowTitle(MODULENAME);
        m_doppler = true;
        DOPPLERSIMU doppler;
        memset(&doppler, 0, sizeof(doppler));
        doppler.type = 1;//1遥测 2数传 3同时
        //协议2关，1开；控件0关
        doppler.dopplerSwitch = ui->comboBox_AnologSwitch->currentIndex() == 0 ? 2 : 1;
        doppler.range = ui->lineEdit_DopplerRange->text().toUShort();//0~180
        if(doppler.range < 0 || doppler.range > 180)
        {
            msgBox.setText("模拟多普勒范围0～180kHz!");
            msgBox.exec();
            return;
        }
        doppler.rateOfChange =  ui->lineEdit_DopplerRate->text().toShort();//0~15000
        if(doppler.rateOfChange < 0 || doppler.rateOfChange > 15000)
        {
            msgBox.setText("模拟多普勒变化率0～15000Hz/s!");
            msgBox.exec();
            return;
        }
        QByteArray ba;
        ba.resize(sizeof(doppler) + 1);
        ba[0] = 49;
        memcpy(ba.data()+ 1, &doppler, sizeof(doppler));
        SendMessage(ba.data(), ba.size(), m_sItem.usCMD, m_sItem.usTID, m_sItem.ucSN);
        //日志信息
        TLxTsspLog tmpLog;
        QString strLog;
        tmpLog.ucWarningLevel  = 1;
        strLog += QString("%1: 多普勒程控制命令成功！").arg(MODULENAME);
        CLog::addLog(strLog, 1);
        //不使能控件
        EnabledControlsDoppler(false);
        ui->pushButton_Modify->setEnabled(true);
        ui->pushButton_TeleSModify->setEnabled(true);
        ui->pushButton_TeleCtrlModify->setEnabled(true);
        ui->pushButton_TeleSourceSModify->setEnabled(true);
        ui->pushButton_12->setEnabled(true);
        m_TimerID = startTimer(500);
    }
}
//噪声源输出
void USBBBE_PMModuSetGUIDlg::on_comboBox_VoiceSourceOutput_currentIndexChanged(int index)
{
    if(ui->pushButton_Modify->text() == "更改")
        return;
    if(index==0){
        ui->lineEdit_OutputRatio->setEnabled(true);
    }
    else
    {
        ui->lineEdit_OutputRatio->setEnabled(false);
    }
}
//模拟开关
void USBBBE_PMModuSetGUIDlg::on_comboBox_AnologSwitch_currentIndexChanged(int index)
{
   if(ui->pushButton_DopplerModify->text() == "更改")
       return;
    if(index==0){
        ui->lineEdit_DopplerRange->setEnabled(false);
        ui->lineEdit_DopplerRate->setEnabled(false);

    }
    else
    {
        ui->lineEdit_DopplerRange->setEnabled(true);
        ui->lineEdit_DopplerRate->setEnabled(true);

    }

}

void USBBBE_PMModuSetGUIDlg::on_radioButton_tcStop_clicked()
{

}
