#include "TCBBE_ModuParamSetGUIDlg.h"
#include "ui_TCBBE_ModuParamSetGUIDlg.h"
#include <QCloseEvent>
#include <QMessageBox>
#include <QDateTime>
#include "../../Common/Log.h"
#include "../../Common/CommonFunction.h"
#include <QTimer>
#pragma pack(1)
extern quint64 GetID(ushort usStationID, uchar ucSubSysID, ushort usDeviceID, ushort usType,
                     uchar ucSn);
SS2BBE_ModuParamSetGUIDlg::SS2BBE_ModuParamSetGUIDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SS2BBE_ModuParamSetGUIDlg)
{
    ui->setupUi(this);
    m_pStateMgr = NULL;
    m_pILxTsspMessageManager = NULL;
    m_pILxTsspLogManager = NULL;
    m_pPublicInfo = NULL;
    m_pHAManager = NULL;
    m_MCSSSSUpModulStatus = NULL;
    m_targetNum_usedInMacro = -1;
    m_targetNum_usedInParam = -1;
    m_MCSSysTarget4Status = NULL;
    m_nTargetNum = 0;
    m_timer1 = new QTimer(this);
    connect(m_timer1, SIGNAL(timeout()), this, SLOT(updateRadioButton()));
    setAttribute(Qt::WA_DeleteOnClose);
}
SS2BBE_ModuParamSetGUIDlg::~SS2BBE_ModuParamSetGUIDlg()
{
    killTimer(m_TimerID);
    m_timer1->stop();
    delete m_timer1;
    delete ui;
}

void SS2BBE_ModuParamSetGUIDlg::setText(const QString& strTitle, ParaStruct xmlsetText[5][60], QString MapMean[50][30], int MapNum[50], uchar Type, ILxTsspParameterMacro* pMacroObj)
{
    m_strTitle = strTitle;
    setWindowTitle(strTitle);

    memcpy(m_DlgParaSetStruct, xmlsetText, sizeof(ParaStruct) * 60 * 5);

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
void SS2BBE_ModuParamSetGUIDlg::InitDLG(int Type, ILxTsspParameterMacro* pMacroObj)
{
    iParaSetFlag = Type;
    //设置输入格式
    InitInput();
    //显示变量名字和范围
    ShowControls();

    if(Type == 0)//取当前参数
    {
        //取当前设置的参数
        if(m_pStateMgr != NULL)
        {
            DWORD stLen = sizeof(SMCU2::MCSUpModuleStatusStruct4);
            m_MCSSSSUpModulStatus =(SMCU2::MCSUpModuleStatusStruct4*)m_pStateMgr->FindOneItem(m_StatusParaMCSUpModulID, stLen);
            if(m_MCSSSSUpModulStatus == NULL || stLen != sizeof(SMCU2::MCSUpModuleStatusStruct4))
                return;
        }
        //启动定时器
        m_TimerID = startTimer(500);
        //不使能
        EnabledControls(false);
        EnabledControlsCommon(false);
    }
    else if(Type == 1)//取宏里参数
    {
        ui->pushButton_Modify->setText("");
        ui->pushButton_PlblicUnitModify->setText("");
        ui->pushButton_PlblicUnitModify->setHidden(true);
        ui->lineEdit_DopplerPredFreM->setEnabled(false);
        ui->pushButton_PublicUnitSet->setHidden(true);
        ui->pushButton_Set->setEnabled(false);
        if(pMacroObj!=NULL)
        {
            QByteArray byteArray;
            pMacroObj->GetParameterBlock(m_ParaMCSUpModulID,byteArray);
            memcpy(&m_MCSSSUpModulParam,byteArray.data(),sizeof(m_MCSSSUpModulParam));

        }
        ui->pushButton_Modify->setVisible(false);
        ui->pushButton_Set->setVisible(false);

        on_comboBox_ChannelOutputCtrlT_currentIndexChanged(ui->comboBox_ChannelOutputCtrlT->currentIndex());
        on_comboBox_NoiseSourOutput_currentIndexChanged(ui->comboBox_NoiseSourOutput->currentIndex());
        m_pCommFunc = CCommFunc::getInstance();
        m_timer1->start(500);
        //显示参数
        ShowPara(1);
    }
}

//设置输入格式
void SS2BBE_ModuParamSetGUIDlg::InitInput()
{
    //正整数
    QRegExp regExp("[0-9\\.]{1,16}");
    QRegExp regExp1("[0-9]{1,4}|-[0-9]{1,4}");
    QRegExp regExp0("[0-9]{1,3}|-[0-9]{1,3}");
    //载波输出电平
     QRegExp regExp_OutputElLevel("-[0-9]{1,2}");
    ui->lineEdit_OutputElLevel->setValidator(new QRegExpValidator(regExp_OutputElLevel, this));
    //载波输出载噪比
    QRegExp regExp_WaveNoiseRatio("[0-9]{0,3}");
    ui->lineEdit_WaveNoiseRatio->setValidator(new QRegExpValidator(regExp_WaveNoiseRatio, this));
    //回零速率
    QRegExp regExp_BackZeroRate("[0-9]{0,5}");
    ui->lineEdit_BackZeroRate->setValidator(new QRegExpValidator(regExp_BackZeroRate, this));
    //补偿频率
    ui->lineEdit_CompensateTime->setValidator(new QRegExpValidator(regExp, this));
    QRegExp regExp_DopplerPredFre("(-)?[0-9]{0,3}");
    //遥测支路多普勒预置频率
    ui->lineEdit_DopplerPredFreT->setValidator(new QRegExpValidator(regExp_DopplerPredFre, this));
    QRegExp regExp_OutputPowerDecrease("-[0-9]{0,2}|0");
    //遥测支路输出功率微调
    ui->lineEdit_OutputPowerDecreaseT->setValidator(new QRegExpValidator(regExp_OutputPowerDecrease, this));
    //测量支路多普勒预置频率
    ui->lineEdit_DopplerPredFreM->setValidator(new QRegExpValidator(regExp_DopplerPredFre, this));
    //测量支路输出功率微调
    ui->lineEdit_OutputPowerDecreaseM->setValidator(new QRegExpValidator(regExp_OutputPowerDecrease, this));
    //测量支路上下行载波频差
    ui->lineEdit_UpdownFDiff->setValidator(new QRegExpValidator(regExp_DopplerPredFre, this));
}

void SS2BBE_ModuParamSetGUIDlg::ShowControls()
{
    ui->groupBox_measureBranch->hide();
    ui->groupBox_processCtrl->hide();
    ui->label_DopplerPredFreT->hide();
    ui->label_DopplerPredFreT_Unit->hide();
    ui->lineEdit_DopplerPredFreT->hide();
    //向下拉框控件中写入从xml中读取的内容
    int i;
    //调制方式
    for(i=0; i<DlgMapLineNum[0];i++)
    {
        ui->comboBox_AdjustType->addItem(m_DlgMapMean[0][i]);
    }
    ui->comboBox_AdjustType->setCurrentIndex(m_DlgParaSetStruct[0][1].InitVal.lval);
    //载波控制
    for(i=0; i<DlgMapLineNum[1];i++)
    {
        ui->comboBox_WaveCtrl->addItem(m_DlgMapMean[1][i]);
    }
    ui->comboBox_WaveCtrl->setCurrentIndex(m_DlgParaSetStruct[0][2].InitVal.lval);
    //噪声源输出
    for(i=0; i<DlgMapLineNum[2];i++)
    {
        ui->comboBox_NoiseSourOutput->addItem(m_DlgMapMean[2][i]);
    }
    ui->comboBox_NoiseSourOutput->setCurrentIndex(m_DlgParaSetStruct[0][3].InitVal.lval);
    ui->comboBox_CompensateType->setCurrentIndex(m_DlgParaSetStruct[0][6].InitVal.lval);
    //载波输出电平
    ui->lineEdit_OutputElLevel->setText(QString::number(m_DlgParaSetStruct[0][0].InitVal.lval));
    ui->label_OutputElLevel->setText("载波输出电平\n[" + QString::number(m_DlgParaSetStruct[0][0].MinVal.lmin) +
            "~" + QString::number(m_DlgParaSetStruct[0][0].MaxVal.lmax) + "]dBm");
    //输出载噪比
    ui->lineEdit_WaveNoiseRatio->setText(QString::number(m_DlgParaSetStruct[0][4].InitVal.lval));
    ui->label_WaveNoiseRatio->setText("载波输出载噪比\n[" + QString::number(m_DlgParaSetStruct[0][4].MinVal.lmin) +
            "~" + QString::number(m_DlgParaSetStruct[0][4].MaxVal.lmax) + "]dBHz");
    ui->radioButton_Target1->setChecked(true);
    showTargetParam(0);
}

//显示参数
void SS2BBE_ModuParamSetGUIDlg::ShowPara(int type)
{
    if(type == 0)       //参数设置
    {
        //SMCU2::MCSSSB2TargetTCSSUpModulParamStruct& dbg = m_MCSSSUpModulParam.upModPara[0].m_MCSTargetTCSSUpModulParam;
        //公共参数
        if(m_MCSSSSUpModulStatus != NULL)
        {
            m_MCSSSUpModulParam.m_MCSSSUpModulParam = m_MCSSSSUpModulStatus->m_MCSSSUpModulStatus.tParams;
            for (int i = 0; i < MAX_TARGET_COUNT; i++)
            {
                m_MCSSSUpModulParam.upModPara[i].m_MCSTargetMeasSSUpModulParam = m_MCSSSSUpModulStatus->TcMeas[i].m_MCSTargetMeasSSUpModulStatus.tParams;
                m_MCSSSUpModulParam.upModPara[i].m_MCSTargetTCSSUpModulParam = m_MCSSSSUpModulStatus->TcMeas[i].m_MCSTargetTCSSUpModulStatus.tParams;

            }

        }
        else
        {
            DWORD stLen = sizeof(SMCU2::MCSUpModuleStatusStruct4);
            if(m_pStateMgr != NULL)
                m_MCSSSSUpModulStatus  = (SMCU2::MCSUpModuleStatusStruct4*)m_pStateMgr->FindOneItem(m_StatusParaMCSUpModulID, stLen);
        }
    }
    else
    {
        //解扰使能或禁止
        //副帧方式控件使能
        if (ui->pushButton_Modify->text() != "更改")
        {
            //EnabledControlsSubFrmType(ui->comboBox_SubFrmType->currentIndex());
        }
    }
    //显示目标
    showRadioButton();

    //    if (m_MCSSSSUpModulStatus == NULL)
    //        return;

    /********************************公共参数设置*********************************/

    //调制方式
    if (m_MCSSSUpModulParam.m_MCSSSUpModulParam.ucModulate > 0)
        ui->comboBox_AdjustType->setCurrentIndex(m_MCSSSUpModulParam.m_MCSSSUpModulParam.ucModulate - 1);
    //载波输出电平
    ui->lineEdit_OutputElLevel->setText(QString::number(m_MCSSSUpModulParam.m_MCSSSUpModulParam.cWaveOutLevel));
    //载波控制
    if (m_MCSSSUpModulParam.m_MCSSSUpModulParam.ucWaveOutCtrl > 0)
        ui->comboBox_WaveCtrl->setCurrentIndex(m_MCSSSUpModulParam.m_MCSSSUpModulParam.ucWaveOutCtrl - 1);
    //噪声源输出
    if (m_MCSSSUpModulParam.m_MCSSSUpModulParam.ucNoiseSw > 0)
        ui->comboBox_NoiseSourOutput->setCurrentIndex(m_MCSSSUpModulParam.m_MCSSSUpModulParam.ucNoiseSw - 1);
    //载波输出载噪比
    ui->lineEdit_WaveNoiseRatio->setText(QString::number(m_MCSSSUpModulParam.m_MCSSSUpModulParam.ucCN0));
    /********************************公共参数设置*********************************/

    showTargetParam(m_nTargetNum, 1);

}

//设置帧长范围
void SS2BBE_ModuParamSetGUIDlg::SetFrameLenRange(int iWordLen)
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

void SS2BBE_ModuParamSetGUIDlg::SetObjInfo(TLxTsspObjectInfo ObjectInfo)
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
        //参数
        m_ParaMCSUpModulID = GetID(sItem.usStationID,
                                   sItem.ucSubSysID, sItem.usDeviceID,sItem.usTID,
                                   sItem.ucSN);
        /*************************框架**********************************/

        /*************************状态**********************************/
        //状态
        m_StatusParaMCSUpModulID = GetID(sItem.usStationID,
                                         sItem.ucSubSysID, sItem.usDeviceID,sItem.usStateType,
                                         sItem.ucSN);
        /*************************状态**********************************/
    }

    //获取目标参数的ID
    ITEM *pItemCom = m_DeviceMap.getItem(COMM_TCBBE_NAME);
    if (pItemCom != NULL && pItemCom->vSubItem.size() > 0)
    {
        SUBITEM& sItem = *(pItemCom->getSubItemByDeviceID_SN(m_ObjectInfo.usLocalDID, m_ObjectInfo.ucSN));
        /*************************状态**********************************/
        m_CommTargetID = GetID(sItem.usStationID,
                               sItem.ucSubSysID, sItem.usDeviceID,sItem.usStateType,
                               sItem.ucSN);
        /*************************状态**********************************/
    }

    //获取发送控制命令的cmd和type
    ITEM *pItemModu = m_DeviceMap.getItem(CMDNAME);
    if (pItemModu != NULL)
    {
        //过程控制命令的实现还需要规划
        int index = 6;
        m_sItem = pItemModu->vSubItem.at(index);
        if(m_sItem.usDeviceID != m_ObjectInfo.usLocalDID)
            m_sItem = pItemModu->vSubItem.at(index + 1);
        m_ctlCmd = GetID(m_sItem.usStationID, m_sItem.ucSubSysID, m_sItem.usDeviceID, m_sItem.usStateType,
                         m_sItem.ucSN);
    }
    //日志
    m_pILxTsspLogManager = pObjectManager->GetLogManager();
    m_pStateMgr = pObjectManager->GetStateManager();
    m_pILxTsspMessageManager = pObjectManager->GetMessageManager();
    m_pPublicInfo = pObjectManager->GetPublicInfoManager();
}

void SS2BBE_ModuParamSetGUIDlg::EnabledControlsCommon(bool bFlag){
    //载波输出电平
    ui->lineEdit_OutputElLevel->setEnabled(bFlag);
    //调制方式方法
    ui->comboBox_AdjustType->setEnabled(bFlag);
    //载波控制
    ui->comboBox_WaveCtrl->setEnabled(bFlag);
    //噪声源输出
    ui->comboBox_NoiseSourOutput->setEnabled(bFlag);
    //载波输出载噪比
    ui->lineEdit_WaveNoiseRatio->setEnabled(bFlag);
    //回零速率
    ui->lineEdit_BackZeroRate->setEnabled(bFlag);
    //补偿方式
    ui->comboBox_CompensateType->setEnabled(bFlag);
    //补偿周期
    ui->lineEdit_CompensateTime->setEnabled(bFlag);
    ui->checkBox_dopplerSw->setEnabled(bFlag);
    /*************************帧同步器*************************/
    if (bFlag)
    {
        //副帧方式控件使能
        //EnabledControlsSubFrmType(ui->comboBox_SubFrmType->currentIndex());
        ui->pushButton_PlblicUnitModify->setText("恢复");
        ui->pushButton_PublicUnitSet->setEnabled(true);
    }
    else
    {
        ui->pushButton_PlblicUnitModify->setText("更改");
        ui->pushButton_PublicUnitSet->setEnabled(false);
    }

}
//使能或不使能控件
void SS2BBE_ModuParamSetGUIDlg::EnabledControls(bool bFlag)
{
    /*************************Group1*************************/

    //遥测支路多普勒预置频率
    ui->lineEdit_DopplerPredFreT->setEnabled(bFlag);
    //遥测支路输出功率衰减
    ui->lineEdit_OutputPowerDecreaseT->setEnabled(bFlag);
    //遥测支路扩频码速率
    ui->comboBox_FreExpandRateT->setEnabled(bFlag);
    //遥测支路通道输出控制
    ui->comboBox_ChannelOutputCtrlT->setEnabled(bFlag);
    //遥测支路伪码加调
    ui->comboBox_VirtualAddT->setEnabled(bFlag);
    //遥测支路数据加调
    ui->comboBox_DataAddT->setEnabled(bFlag);

    //测量支路多普勒预置频率
    ui->lineEdit_DopplerPredFreM->setEnabled(false);
    //测量支路输出功率衰减
    ui->lineEdit_OutputPowerDecreaseM->setEnabled(bFlag);
    //测量支路扩频码速率
    ui->comboBox_FreExpandRateM->setEnabled(bFlag);
    //测量支路通道输出控制
    ui->comboBox_ChannelOutputCtrlM->setEnabled(bFlag);
    //测量支路伪码加调
    ui->comboBox_VirtualAddM->setEnabled(bFlag);
    //测量支路数据加调
    ui->comboBox_DataAddM->setEnabled(bFlag);
    //上下行载波频率差
    ui->lineEdit_UpdownFDiff->setEnabled(bFlag);
    /*************************Group1*************************/

    /*************************帧同步器*************************/
    if (bFlag)
    {
        //副帧方式控件使能
        //EnabledControlsSubFrmType(ui->comboBox_SubFrmType->currentIndex());
        ui->pushButton_Modify->setText("恢复");
        ui->pushButton_Set->setEnabled(true);
    }
    else
    {
        ui->pushButton_Modify->setText("更改");
        ui->pushButton_Set->setEnabled(false);
    }
}

//保存参数宏
int SS2BBE_ModuParamSetGUIDlg::SaveToMacro(ILxTsspParameterMacro* pMacroObj)
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
        byteArray.resize(sizeof(m_MCSSSUpModulParam));
        memcpy(byteArray.data(),&m_MCSSSUpModulParam,sizeof(m_MCSSSUpModulParam));
        pMacroObj->UpdateParameterBlock(m_ParaMCSUpModulID,byteArray);

        //日志信息
        TLxTsspLog tmpLog;
        QString strLog;
        memset(&tmpLog, 0, sizeof(TLxTsspLog));
        tmpLog.ucWarningLevel  = 1;
        strLog += QString("模式2上行调制解调参数设置: 保存宏成功！").arg(MODULENAME);
        CLog::addLog(strLog, 1);

    }
}
//从界面取参数
void SS2BBE_ModuParamSetGUIDlg::GetPara()
{
    getParamTarget();
    /***********************************码同步****************************/
    //调制方式
    m_MCSSSUpModulParam.m_MCSSSUpModulParam.ucModulate = ui->comboBox_AdjustType->currentIndex() + 1;
    //载波输出电平
    m_MCSSSUpModulParam.m_MCSSSUpModulParam.cWaveOutLevel = ui->lineEdit_OutputElLevel->text().toInt();
    //载波控制
    m_MCSSSUpModulParam.m_MCSSSUpModulParam.ucWaveOutCtrl = ui->comboBox_WaveCtrl->currentIndex() + 1;
    //噪声源输出
    m_MCSSSUpModulParam.m_MCSSSUpModulParam.ucNoiseSw = ui->comboBox_NoiseSourOutput->currentIndex() + 1;
    //载波输出载噪比
    m_MCSSSUpModulParam.m_MCSSSUpModulParam.ucCN0 = ui->lineEdit_WaveNoiseRatio->text().toInt();
}

//验证参数范围
int SS2BBE_ModuParamSetGUIDlg::CheckPara()
{
    QMessageBox msgBox;
    QString strTmp;
    bool ok = false;
    DOUBLE cFre = ui->lineEdit_CompensateTime->text().toDouble(&ok);
    msgBox.setWindowTitle(MODULENAME);
    /***********************************码同步****************************/
    //载波输出电平
    if(ui->lineEdit_OutputElLevel->text().isEmpty())
    {
        strTmp = QString("载波输出电平不能为空!");
        goto flag;
    }
    //载波输出载噪比
    if(ui->lineEdit_WaveNoiseRatio->text().isEmpty())
    {
        strTmp = QString("载波输出载噪比不能为空!");
        goto flag;
    }
//    //遥测支路多普勒预置频率
//    if(ui->lineEdit_DopplerPredFreT->text().isEmpty())
//    {
//        strTmp = QString("多普勒预置频率不能为空!");
//        goto flag;
//    }
    //遥测支路输出功率微调
    if(ui->lineEdit_OutputPowerDecreaseT->text().isEmpty())
    {
        strTmp = QString("遥测支路输出功率微调不能为空，且其正确范围是：%1～%2dB!");
        goto flag;
    }
    //载波输出电平
    if(ui->lineEdit_OutputElLevel->text().toInt() < m_DlgParaSetStruct[0][0].MinVal.lmin
            || (ui->lineEdit_OutputElLevel->text().toInt()) > m_DlgParaSetStruct[0][0].MaxVal.lmax)
    {
        strTmp = QString("载波输出电平正确范围是：%1～%2dBm!")
                .arg(m_DlgParaSetStruct[0][0].MinVal.lmin).arg(m_DlgParaSetStruct[0][0].MaxVal.lmax);
        goto flag;
    }
    //载波输出载噪比
    if(ui->lineEdit_WaveNoiseRatio->text().toInt() < m_DlgParaSetStruct[0][4].MinVal.lmin
            || ui->lineEdit_WaveNoiseRatio->text().toInt() > m_DlgParaSetStruct[0][4].MaxVal.lmax)
    {
        strTmp = QString("载波输出载噪比正确范围是：%1～%2dBHz!")
                .arg(m_DlgParaSetStruct[0][4].MinVal.lmin).arg(m_DlgParaSetStruct[0][4].MaxVal.lmax);
        goto flag;
    }
    //遥测支路多普勒预置频率
//    if(ui->lineEdit_DopplerPredFreT->text().toInt() < m_DlgParaSetStruct[1][0].MinVal.lmin
//            || ui->lineEdit_DopplerPredFreT->text().toInt() > m_DlgParaSetStruct[1][0].MaxVal.lmax)
//    {
//        strTmp = QString("多普勒预置频率正确范围是：%1～%2kHz!")
//                .arg(m_DlgParaSetStruct[1][0].MinVal.lmin).arg(m_DlgParaSetStruct[1][0].MaxVal.lmax);;
//        goto flag;
//    }
    //遥测支路输出功率微调
    if(ui->lineEdit_OutputPowerDecreaseT->text().toInt() < m_DlgParaSetStruct[1][1].MinVal.lmin
            || ui->lineEdit_OutputPowerDecreaseT->text().toInt() > m_DlgParaSetStruct[1][1].MaxVal.lmax)
    {
        strTmp = QString("遥测支路输出功率微调正确范围是：%1～%2dB!")
                .arg(m_DlgParaSetStruct[1][1].MinVal.lmin).arg(m_DlgParaSetStruct[1][1].MaxVal.lmax);
        goto flag;
    }
    return 1;
flag:
    msgBox.setText(strTmp);
    msgBox.exec();
    return -1;
}

//定时器
void SS2BBE_ModuParamSetGUIDlg::timerEvent(QTimerEvent *event)
{
    ShowPara(0);
}

void SS2BBE_ModuParamSetGUIDlg::closeEvent(QCloseEvent *event)
{
    event->accept();
}

QSize SS2BBE_ModuParamSetGUIDlg::sizeHint() const
{
    return QSize(this->width(), this->height());
}

void SS2BBE_ModuParamSetGUIDlg::SubFrmEn(bool bFlag)
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
void SS2BBE_ModuParamSetGUIDlg::IDSubFrmEn(bool bFlag)
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
void SS2BBE_ModuParamSetGUIDlg::CycSubFrmEn(bool bFlag)
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
void SS2BBE_ModuParamSetGUIDlg::ReserveSubFrmEn(bool bFlag)
{

}

void SS2BBE_ModuParamSetGUIDlg::SendMessage(char* pData, USHORT usLen, USHORT usC, USHORT usTID, USHORT usSN)
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
    if(0x700 == subHeader.DID){
        subHeader.DID = 0x701;
        ITEM *pItem = m_DeviceMap.getItem(MODULENAME);
        if (pItem->vSubItem.size() > 0)
        {
            SUBITEM& sItem = *(pItem->getSubItemByDeviceID_SN(subHeader.DID, m_ObjectInfo.ucSN));
            header.C = sItem.usCMD;
            subHeader.TID = sItem.usTID;
        }
        memcpy(message.pData, &header, sizeof(TLxTsspMessageHeader));
        memcpy(message.pData + sizeof(TLxTsspMessageHeader),
               &subHeader, sizeof(TLxTsspSubMsgHeader));
        m_pILxTsspMessageManager->FireMessage(message);
    }
    else if(0x701 == subHeader.DID){
        subHeader.DID = 0x700;
        ITEM *pItem = m_DeviceMap.getItem(MODULENAME);
        if (pItem->vSubItem.size() > 0)
        {
            SUBITEM& sItem = *(pItem->getSubItemByDeviceID_SN(subHeader.DID, m_ObjectInfo.ucSN));
            header.C = sItem.usCMD;
            subHeader.TID = sItem.usTID;
        }
        memcpy(message.pData, &header, sizeof(TLxTsspMessageHeader));
        memcpy(message.pData + sizeof(TLxTsspMessageHeader),
               &subHeader, sizeof(TLxTsspSubMsgHeader));
        m_pILxTsspMessageManager->FireMessage(message);
    }
    delete[] message.pData;
}

DWORDLONG SS2BBE_ModuParamSetGUIDlg::StrToSynCode(const char * pStr)
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

void SS2BBE_ModuParamSetGUIDlg::showTargetParam(int nIndex, int nType)
{
    if (nType == 0)
    {
        int i;
        //遥测支路扩频码速率
        ui->comboBox_FreExpandRateT->clear();
        for(i=0; i<DlgMapLineNum[nIndex * 8 + 4];i++)
        {
            ui->comboBox_FreExpandRateT->addItem(m_DlgMapMean[nIndex * 8 + 4][i]);
        }
        ui->comboBox_FreExpandRateT->setCurrentIndex(m_DlgParaSetStruct[nIndex + 1][2].InitVal.lval);
        //通道输出控制
        ui->comboBox_ChannelOutputCtrlT->clear();
        for(i=0; i<DlgMapLineNum[nIndex * 8 + 5];i++)
        {
            ui->comboBox_ChannelOutputCtrlT->addItem(m_DlgMapMean[nIndex * 8 + 5][i]);
        }
        ui->comboBox_ChannelOutputCtrlT->setCurrentIndex(m_DlgParaSetStruct[nIndex + 1][3].InitVal.lval);
        //遥测支路伪码加调
        ui->comboBox_VirtualAddT->clear();
        for(i=0; i<DlgMapLineNum[nIndex * 8 + 6];i++)
        {
            ui->comboBox_VirtualAddT->addItem(m_DlgMapMean[nIndex * 8 + 6][i]);
        }
        ui->comboBox_VirtualAddT->setCurrentIndex(m_DlgParaSetStruct[nIndex + 1][4].InitVal.lval);
        //遥测支路数据加调
        ui->comboBox_DataAddT->clear();
        for(i=0; i<DlgMapLineNum[nIndex * 8 + 7];i++)
        {
            ui->comboBox_DataAddT->addItem(m_DlgMapMean[nIndex * 8 + 7][i]);
        }
        ui->comboBox_DataAddT->setCurrentIndex(m_DlgParaSetStruct[nIndex + 1][5].InitVal.lval);
        //往静态控件中写入参数缺省值
        //遥测支路多普勒预置频率
        ui->lineEdit_DopplerPredFreT->setText(QString::number(m_DlgParaSetStruct[nIndex + 1][0].InitVal.lval));
        ui->label_DopplerPredFreT->setText("多普勒预置频率\n[" + QString::number(m_DlgParaSetStruct[nIndex + 1][0].MinVal.lmin) +
                "~" + QString::number(m_DlgParaSetStruct[nIndex + 1][0].MaxVal.lmax) + "]kHz");
        //遥测支路输出功率衰减
        ui->lineEdit_OutputPowerDecreaseT->setText(QString::number(m_DlgParaSetStruct[nIndex + 1][1].InitVal.lval));
        ui->label_OutputPowerDecreaseT->setText("输出功率衰减\n[" + QString::number(m_DlgParaSetStruct[nIndex + 1][1].MinVal.lmin) +
                "~" + QString::number(m_DlgParaSetStruct[nIndex + 1][1].MaxVal.lmax) + "]dBm");

    }
    else if (nType == 1)
    {
        //遥测支路扩频码速率
        if (m_MCSSSUpModulParam.upModPara[nIndex].m_MCSTargetTCSSUpModulParam.ucPNRate > 0)
            ui->comboBox_FreExpandRateT->setCurrentIndex(m_MCSSSUpModulParam.upModPara[nIndex].m_MCSTargetTCSSUpModulParam.ucPNRate - 1);
        //通道输出控制
        if (m_MCSSSUpModulParam.upModPara[nIndex].m_MCSTargetTCSSUpModulParam.ucOutputSwitch > 0)
            ui->comboBox_ChannelOutputCtrlT->setCurrentIndex(m_MCSSSUpModulParam.upModPara[nIndex].m_MCSTargetTCSSUpModulParam.ucOutputSwitch - 1);
        //遥测支路伪码加调
        if (m_MCSSSUpModulParam.upModPara[nIndex].m_MCSTargetTCSSUpModulParam.ucPNIModuSw > 0)
            ui->comboBox_VirtualAddT->setCurrentIndex(m_MCSSSUpModulParam.upModPara[nIndex].m_MCSTargetTCSSUpModulParam.ucPNIModuSw - 1);
        //遥测支路数据加调
        if (m_MCSSSUpModulParam.upModPara[nIndex].m_MCSTargetTCSSUpModulParam.ucDataModuSw > 0)
            ui->comboBox_DataAddT->setCurrentIndex(m_MCSSSUpModulParam.upModPara[nIndex].m_MCSTargetTCSSUpModulParam.ucDataModuSw - 1);
        //遥测支路多普勒预置频率
        ui->lineEdit_DopplerPredFreT->setText(QString::number(m_MCSSSUpModulParam.upModPara[nIndex].m_MCSTargetTCSSUpModulParam.ucPreDplValue));
        //遥测支路输出功率衰减
        ui->lineEdit_OutputPowerDecreaseT->setText(QString::number(m_MCSSSUpModulParam.upModPara[nIndex].m_MCSTargetTCSSUpModulParam.cPowAdjust * (-1)));

    }
}

int SS2BBE_ModuParamSetGUIDlg::getSharedMemoryTargetNum()
{
    return 1;
    DWORD stLen = sizeof(SMCU2::MCSSysTarget4StatusStruct);
    if(m_pStateMgr != NULL)
        m_MCSSysTarget4Status = (SMCU2::MCSSysTarget4StatusStruct*)m_pStateMgr->FindOneItem(m_CommTargetID, stLen);

    if (m_MCSSysTarget4Status != NULL)
        return m_MCSSysTarget4Status->MCSSysStatu.tParams.ucTargetNum;
    return 0;
}

void SS2BBE_ModuParamSetGUIDlg::showRadioButton()
{
    int nNum = getSharedMemoryTargetNum();
    if (m_targetNum_usedInParam == nNum)
        return;
    m_targetNum_usedInParam = nNum;
    m_nTargetNum = 0;
    set_radioButton_Target();
    ui->radioButton_Target1->hide();
    ui->radioButton_Target2->hide();
    ui->radioButton_Target3->hide();
    ui->radioButton_Target4->hide();
    switch(nNum - 1)
    {
    case 0:
        ui->radioButton_Target1->show();
        break;
    case 1:
        ui->radioButton_Target1->show();
        ui->radioButton_Target2->show();
        break;
    case 2:
        ui->radioButton_Target1->show();
        ui->radioButton_Target2->show();
        ui->radioButton_Target3->show();
        break;
    case 3:
        ui->radioButton_Target1->show();
        ui->radioButton_Target2->show();
        ui->radioButton_Target3->show();
        ui->radioButton_Target4->show();
        break;
    default:
        ui->radioButton_Target1->show();
        break;
    }
}
void SS2BBE_ModuParamSetGUIDlg::getParamTarget()
{
    //遥测支路多普勒预置频率
    m_MCSSSUpModulParam.upModPara[m_nTargetNum].m_MCSTargetTCSSUpModulParam.ucPreDplValue
            = ui->lineEdit_DopplerPredFreT->text().toInt();
    //遥测支路输出功率衰减
    m_MCSSSUpModulParam.upModPara[m_nTargetNum].m_MCSTargetTCSSUpModulParam.cPowAdjust
            = ui->lineEdit_OutputPowerDecreaseT->text().toInt() * (-1);
    //遥测支路扩频码速率
    m_MCSSSUpModulParam.upModPara[m_nTargetNum].m_MCSTargetTCSSUpModulParam.ucPNRate
            = ui->comboBox_FreExpandRateT->currentIndex() + 1;
    //遥测支路通道输出控制
    m_MCSSSUpModulParam.upModPara[m_nTargetNum].m_MCSTargetTCSSUpModulParam.ucOutputSwitch
            = ui->comboBox_ChannelOutputCtrlT->currentIndex() + 1;
    //遥测支路伪码加调
    m_MCSSSUpModulParam.upModPara[m_nTargetNum].m_MCSTargetTCSSUpModulParam.ucPNIModuSw
            = ui->comboBox_VirtualAddT->currentIndex() + 1;
    //遥测支路数据加调
    m_MCSSSUpModulParam.upModPara[m_nTargetNum].m_MCSTargetTCSSUpModulParam.ucDataModuSw
            = ui->comboBox_DataAddT->currentIndex() + 1;

}

void SS2BBE_ModuParamSetGUIDlg::on_radioButton_Target1_clicked()
{
    if (CheckPara() != 1)
    {
        set_radioButton_Target();
        return;
    }
    GetPara();
    m_nTargetNum = 0;
    //int nType = ui->pushButton_Set->isEnabled()  ? 0 : 1;
    showTargetParam(0, 1);
}

void SS2BBE_ModuParamSetGUIDlg::on_radioButton_Target2_clicked()
{
    if (CheckPara() != 1)
    {
        set_radioButton_Target();
        return;
    }
    GetPara();
    m_nTargetNum = 1;
    //int nType = ui->pushButton_Set->isEnabled()  ? 0 : 1;
    showTargetParam(1, 1);
}

void SS2BBE_ModuParamSetGUIDlg::on_radioButton_Target3_clicked()
{
    if (CheckPara() != 1)
    {
        set_radioButton_Target();
        return;
    }
    GetPara();
    m_nTargetNum = 2;
    //int nType = ui->pushButton_Set->isEnabled()  ? 0 : 1;
    showTargetParam(2, 1);
}

void SS2BBE_ModuParamSetGUIDlg::on_radioButton_Target4_clicked()
{
    if (CheckPara() != 1)
    {
        set_radioButton_Target();
        return;
    }
    GetPara();
    m_nTargetNum = 3;
    //int nType = ui->pushButton_Set->isEnabled()  ? 0 : 1;
    showTargetParam(3, 1);
}
//更改参数按钮休息处理函数
void SS2BBE_ModuParamSetGUIDlg::on_pushButton_Modify_clicked()
{
    if(ui->pushButton_Modify->text() == "更改")
    {
        EnabledControls(true);
        killTimer(m_TimerID);
        //通道输出控制
        on_comboBox_ChannelOutputCtrlT_currentIndexChanged(ui->comboBox_ChannelOutputCtrlT->currentIndex());
        ui->pushButton_PlblicUnitModify->setEnabled(false);
        ui->pushButton_PublicUnitSet->setEnabled(false);
        return;
    }

    if(ui->pushButton_Modify->text() == "恢复")
    {
        EnabledControls(false);
        m_TimerID = startTimer(500);
        ui->pushButton_PlblicUnitModify->setEnabled(true);
        ui->pushButton_PublicUnitSet->setEnabled(false);
        return;
    }
}

void SS2BBE_ModuParamSetGUIDlg::on_pushButton_Set_clicked()
{
    //两个分路参数
    if(CheckPara() == 1)
    {
        //从界面取参数
        GetPara();

        //设置参数
        //取当前设置的参数
        if(m_pHAManager != NULL)
        {
            struct{
                USHORT target;
                USHORT tmp;
                SMCU2::MCSSSB2SSUpModulParamStruct sys;
                struct{
                    USHORT tmp1;
                    SMCU2::MCSSSB2TargetTCSSUpModulParamStruct tc;
                    USHORT tmp2;
                    SMCU2::MCSSSB2TargetMeasSSUpModulParamStruct meas;
                }param[MAX_TARGET_COUNT];
            }data;
            data.sys = m_MCSSSUpModulParam.m_MCSSSUpModulParam;
            data.target = 1 | 0x02 | 0x04 | 0x08 | 0x10;
            for (int i = 0; i < MAX_TARGET_COUNT; i++)
            {
                data.param[i].tc = m_MCSSSUpModulParam.upModPara[i].m_MCSTargetTCSSUpModulParam;
                data.param[i].meas = m_MCSSSUpModulParam.upModPara[i].m_MCSTargetMeasSSUpModulParam;
            }

            QByteArray byteArray;
            byteArray.resize(sizeof(data));
            memcpy(byteArray.data(),&data,sizeof(data));
            SendMessage((char*)&data, sizeof(data), m_usC, m_usTID);
        }

        //日志信息
        TLxTsspLog tmpLog;
        QString strLog;
        memset(&tmpLog, 0, sizeof(TLxTsspLog));
        tmpLog.ucWarningLevel  = 1;
        strLog += "解调参数设置: 设置参数成功！";
        strcpy(tmpLog.szContent, strLog.toUtf8().data());
        tmpLog.time = QDateTime::currentDateTime().toTime_t();

        m_pILxTsspLogManager->Add(tmpLog);

        //不使能控件
        ui->pushButton_PlblicUnitModify->setEnabled(true);
        EnabledControls(false);
        m_TimerID = startTimer(500);
    }
}

void SS2BBE_ModuParamSetGUIDlg::on_pushButton_PlblicUnitModify_clicked()
{
    if(ui->pushButton_PlblicUnitModify->text() == "更改")
    {
        EnabledControlsCommon(true);
        killTimer(m_TimerID);
        on_comboBox_NoiseSourOutput_currentIndexChanged(ui->comboBox_NoiseSourOutput->currentIndex());
        ui->pushButton_Modify->setEnabled(false);
        ui->pushButton_Set->setEnabled(false);
        return;
    }

    if(ui->pushButton_PlblicUnitModify->text() == "恢复")
    {
        EnabledControlsCommon(false);
        m_TimerID = startTimer(500);
        ui->pushButton_Modify->setEnabled(true);
        ui->pushButton_Set->setEnabled(true);
        return;
    }
}
//噪声源输出
void SS2BBE_ModuParamSetGUIDlg::on_comboBox_NoiseSourOutput_currentIndexChanged(int index)
{
    if(ui->pushButton_PlblicUnitModify->text() == "更改")
        return ;
    if(index ==0){
        ui->lineEdit_WaveNoiseRatio->setEnabled(true);
    }
    else
    {
        ui->lineEdit_WaveNoiseRatio->setEnabled(false);
    }

}
//通道输出控制
void SS2BBE_ModuParamSetGUIDlg::on_comboBox_ChannelOutputCtrlT_currentIndexChanged(int index)
{
    if(ui->pushButton_Modify->text() == "更改")
        return;
    if(index==0){
        //遥测支路多普勒预置频率
        ui->lineEdit_DopplerPredFreT->setEnabled(true);
        //遥测支路输出功率衰减
        ui->lineEdit_OutputPowerDecreaseT->setEnabled(true);
        //遥测支路扩频码速率
        ui->comboBox_FreExpandRateT->setEnabled(true);

        //遥测支路伪码加调
        ui->comboBox_VirtualAddT->setEnabled(true);
        //遥测支路数据加调
        ui->comboBox_DataAddT->setEnabled(true);

    }
    else{
        //遥测支路多普勒预置频率
        ui->lineEdit_DopplerPredFreT->setEnabled(false);
        //遥测支路输出功率衰减
        ui->lineEdit_OutputPowerDecreaseT->setEnabled(false);
        //遥测支路扩频码速率
        ui->comboBox_FreExpandRateT->setEnabled(false);

        //遥测支路伪码加调
        ui->comboBox_VirtualAddT->setEnabled(false);
        //遥测支路数据加调
        ui->comboBox_DataAddT->setEnabled(false);
    }
}

void SS2BBE_ModuParamSetGUIDlg::on_pushButton_PublicUnitSet_clicked()
{
    //公共参数
    if(CheckPara() == 1)
    {
        //从界面取参数
        GetPara();

        //设置参数
        //取当前设置的参数
        if(m_pHAManager != NULL)
        {
            struct{
                USHORT target;
                USHORT tmp;
                SMCU2::MCSSSB2SSUpModulParamStruct comm;
            }data;
            data.target = 1;
            data.comm = m_MCSSSUpModulParam.m_MCSSSUpModulParam;
            QByteArray byteArray;
            byteArray.resize(sizeof(data));
            memcpy(byteArray.data(),&data,sizeof(data));
            SendMessage(byteArray.data(), byteArray.size(), m_usC, m_usTID);

            byteArray.clear();
            //过程控制命令
            struct{
                UCHAR unitNum;
                UCHAR chl;
                UCHAR compensateCtrl;
                UCHAR compensateType;
                UINT  compensatePeriod;
                USHORT resetSpeed;
            }dopplerCom;
            dopplerCom.unitNum = 40;
            switch (m_nTargetNum) {
            case 0:
                dopplerCom.chl = 1;
                break;
            case 1:
                dopplerCom.chl = 2;
                break;
            case 2:
                dopplerCom.chl = 3;
                break;
            case 3:
                dopplerCom.chl = 4;
            default:
                break;
            }
            dopplerCom.compensateCtrl = ui->checkBox_dopplerSw->checkState() ? 1 : 2;//补偿开关
            dopplerCom.compensateType = ui->comboBox_CompensateType->currentIndex() + 2;//补偿方式
            dopplerCom.compensatePeriod = qRound(ui->lineEdit_CompensateTime->text().toDouble());//补偿频率表示补偿周期
            dopplerCom.resetSpeed = ui->lineEdit_BackZeroRate->text().toInt();//回零速率
            //SendMessage((char*)&dopplerCom, sizeof(dopplerCom), m_sItem.usCMD, m_sItem.usTID, m_sItem.ucSN);
        }

        //日志信息
        TLxTsspLog tmpLog;
        QString strLog;
        memset(&tmpLog, 0, sizeof(TLxTsspLog));
        tmpLog.ucWarningLevel  = 1;
        strLog += "解调参数设置: 设置参数成功！";
        strcpy(tmpLog.szContent, strLog.toUtf8().data());
        tmpLog.time = QDateTime::currentDateTime().toTime_t();

        m_pILxTsspLogManager->Add(tmpLog);

        //不使能控件
        ui->pushButton_Modify->setEnabled(true);
        EnabledControlsCommon(false);
        m_TimerID = startTimer(500);
    }
}
#pragma pack()


void SS2BBE_ModuParamSetGUIDlg::updateRadioButton()
{
    if(!m_pCommFunc)return;
    if (m_targetNum_usedInMacro == m_pCommFunc->getTargetNum())
        return;
    m_targetNum_usedInMacro = m_pCommFunc->getTargetNum();
    m_nTargetNum = 0;
    set_radioButton_Target();
    ui->radioButton_Target1->hide();
    ui->radioButton_Target2->hide();
    ui->radioButton_Target3->hide();
    ui->radioButton_Target4->hide();
    switch( m_pCommFunc->getTargetNum() - 1)
    {
    case 0:
        ui->radioButton_Target1->show();
        break;
    case 1:
        ui->radioButton_Target1->show();
        ui->radioButton_Target2->show();
        break;
    case 2:
        ui->radioButton_Target1->show();
        ui->radioButton_Target2->show();
        ui->radioButton_Target3->show();
        break;
    case 3:
        ui->radioButton_Target1->show();
        ui->radioButton_Target2->show();
        ui->radioButton_Target3->show();
        ui->radioButton_Target4->show();
        break;
    default:
        break;
    }
}

void SS2BBE_ModuParamSetGUIDlg::set_radioButton_Target()
{
    switch(m_nTargetNum)
    {
    case 0:
        ui->radioButton_Target1->setChecked(true);
        break;
    case 1:
        ui->radioButton_Target2->setChecked(true);
        break;
    case 2:
        ui->radioButton_Target3->setChecked(true);
        break;
    case 3:
        ui->radioButton_Target4->setChecked(true);
        break;
    default:
        ui->radioButton_Target1->setChecked(true);
    }
}
#pragma pack()

