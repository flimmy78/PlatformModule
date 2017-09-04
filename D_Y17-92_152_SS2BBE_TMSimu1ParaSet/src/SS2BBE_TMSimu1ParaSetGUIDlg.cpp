#include "SS2BBE_TMSimu1ParaSetGUIDlg.h"
#include "ui_SS2BBE_TMSimu1ParaSetGUIDlg.h"
#include <QCloseEvent>
#include <QMessageBox>
#include <QDateTime>
#include "../../Common/Log.h"
#include <QDebug>
#include <QtCore>
#pragma pack(1)
extern quint64 GetID(ushort usStationID, uchar ucSubSysID, ushort usDeviceID, ushort usType,
                     uchar ucSn);
SS2BBE_TMSimu1ParaSetGUIDlg::SS2BBE_TMSimu1ParaSetGUIDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SS2BBE_TMSimu1ParaSetGUIDlg)
{
    ui->setupUi(this);
    m_pStateMgr = NULL;
    m_pILxTsspMessageManager = NULL;
    m_pILxTsspLogManager = NULL;
    m_pPublicInfo = NULL;
    m_pHAManager = NULL;
    m_nTargetNum = 0;
    m_MCSSSTMSimulStatus = NULL;
    m_MCSSSTMStatus = NULL;
    m_timer1 = new QTimer(this);
    m_autoMap = false;
    m_currentNumber_usedByMacro = -1;
    m_currentNumber_usedByParam = -1;
    connect(m_timer1, SIGNAL(timeout()), this, SLOT(updateTarget()));
    setAttribute(Qt::WA_DeleteOnClose);
}

SS2BBE_TMSimu1ParaSetGUIDlg::~SS2BBE_TMSimu1ParaSetGUIDlg()
{
    killTimer(m_TimerID);
    m_timer1->stop();
    delete m_timer1;
    delete ui;
}

void SS2BBE_TMSimu1ParaSetGUIDlg::setText(const QString& strTitle, ParaStruct xmlsetText[4][60], QString MapMean[100][30], int MapNum[100], uchar Type, ILxTsspParameterMacro* pMacroObj)
{
    m_strTitle = strTitle;
    setWindowTitle(strTitle);

    memcpy(m_DlgParaSetStruct, xmlsetText, sizeof(ParaStruct) * 60 * 4);

    for(int i= 0;i<100;i++)
    {
        for(int j=0; j<30; j++)
        {
            m_DlgMapMean[i][j] = MapMean[i][j];
        }
    }

    memset(DlgMapLineNum, 0, 100);
    for(int i = 0; i < 100; i++)
    {
        DlgMapLineNum[i] = MapNum[i];
    }
    //初始化界面
    InitDLG(Type, pMacroObj);
}

//初始化对话框
void SS2BBE_TMSimu1ParaSetGUIDlg::InitDLG(int Type, ILxTsspParameterMacro* pMacroObj)
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
            DWORD stLen = sizeof(SMCU2::MCSSSB2TMSimulStatusStruct4);
            m_MCSSSTMSimulStatus = (SMCU2::MCSSSB2TMSimulStatusStruct4*)m_pStateMgr->FindOneItem(m_StatusParaMCSSSTMSimulID, stLen);
            if(m_MCSSSTMSimulStatus == NULL || stLen != sizeof(SMCU2::MCSSSB2TMSimulStatusStruct4))
                return;
        }
        //启动定时器
        m_TimerID = startTimer(500);
        //不使能
        EnabledControls(false);
    }
    else if(Type == 1)//取宏里参数
    {
        ui->pushButton_Modify->setText("");
        ui->pushButton_Set->setEnabled(false);
        if(pMacroObj!=NULL)
        {
            QByteArray byteArray;
            pMacroObj->GetParameterBlock(m_ParaMCSSSTMSimulID, byteArray);
            memcpy(&m_MCSSSTMSimulParam, byteArray.data(), sizeof(SMCU2::MCSSSB2TMSimulParamStruct4));
        }
        ui->pushButton_Modify->setVisible(false);
        ui->pushButton_Set->setVisible(false);
        ui->pushButton_autoMap->setVisible(false);
         ui->comboBox_VisualFillSw->setDisabled(true);
        m_pCommFunc = CCommFunc::getInstance();
        //显示参数
        ShowPara(1);
        m_timer1->start(500);
        ui->comboBox_VisualFillSw->setEnabled(false);
        on_comboBox_DataSource_currentIndexChanged(ui->comboBox_DataSource->currentIndex());
        on_comboBox_DataSourceType_currentIndexChanged(ui->comboBox_DataSourceType->currentIndex());
        on_comboBox_DecodeType_currentIndexChanged(ui->comboBox_DecodeType->currentIndex());
        on_comboBox_SubFrmSyncType_currentIndexChanged(ui->comboBox_SubFrmSyncType->currentIndex());
        on_comboBox_RecurrentCodeType_currentIndexChanged(ui->comboBox_RecurrentCodeType->currentIndex());
        on_comboBox_RandEn_currentIndexChanged(ui->comboBox_RandEn->currentIndex());

    }
}

//设置输入格式
void SS2BBE_TMSimu1ParaSetGUIDlg::InitInput()
{
    //正整数
    QRegExp regExp("[0-9]{1,16}");
    //码速率
     QRegExp regExp_CodeRate("[0-9]{2,5}");
    ui->lineEdit_CodeRate->setValidator(new QRegExpValidator(regExp_CodeRate, this));
    //交织深度
    QRegExp regExp_RSDecodeDeep("[1-5]");
    ui->lineEdit_RSDecodeDeep->setValidator(new QRegExpValidator(regExp_RSDecodeDeep, this));
    //帧长
    QRegExp regExp_Len("[0-9]{0,4}");
    ui->lineEdit_FrameLen->setValidator(new QRegExpValidator(regExp_Len, this));
    //字长
    QRegExp regExp_WordLen("[0-9]{0,2}");
    ui->lineEdit_WordLen->setValidator(new QRegExpValidator(regExp_WordLen, this));
    //格式长
    QRegExp regExp_FDormatLen("[0-9]{0,3}");
    ui->lineEdit_FDormatLen->setValidator(new QRegExpValidator(regExp_FDormatLen, this));
    //副帧码组位置
    ui->lineEdit_SubFrmSynGroupPos->setValidator(new QRegExpValidator(regExp_Len, this));
    //ID计数位置
    ui->lineEdit_IDCountPos->setValidator(new QRegExpValidator(regExp_Len, this));

    //字母数字
    QRegExp regExp2("[A-Fa-f0-9]{1,8}");
    //固定数设置[HEX]
     QRegExp regExp_ConstSetting("[A-Fa-f0-9]{1,4}");
    ui->lineEdit_ConstSetting->setValidator(new QRegExpValidator(regExp_ConstSetting, this));
    //帧同步码组[HEX]
    ui->lineEdit_FrmSyncGroup->setValidator(new QRegExpValidator(regExp2, this));
    //加扰多项式[HEX]
    ui->lineEdit_RandPoly->setValidator(new QRegExpValidator(regExp2, this));
    //加扰初相[HEX]
    ui->lineEdit_RandPhase->setValidator(new QRegExpValidator(regExp2, this));
    //副帧同步码组[HEX]
    ui->lineEdit_SubFrmGroup->setValidator(new QRegExpValidator(regExp2, this));
}

void SS2BBE_TMSimu1ParaSetGUIDlg::ShowControls()
{
    showTargetParam(0);
    //默认选择目标1
    ui->radioButton_Target1->setChecked(true);
    ui->label_VisualFillSw->hide();
    ui->comboBox_VisualFillSw->hide();
}

//显示参数
void SS2BBE_TMSimu1ParaSetGUIDlg::ShowPara(int type)
{
    if(type == 0)       //参数设置
    {
        //码同步
        if(m_MCSSSTMSimulStatus != NULL)
        {
            //自动映射
            if(m_autoMap)
            {
                for (int i = 0; i < MAX_TARGET_COUNT; i++)
                {
                    //由遥测单元映射来
                    m_MCSSSTMSimulParam.SSTMSimulParam[i].ucFspPlace = m_MCSSSTMParam.SSTMParam[i].ucFspPlace;
                    m_MCSSSTMSimulParam.SSTMSimulParam[i].uiCodeStyle = m_MCSSSTMParam.SSTMParam[i].uiCodeStyle;
                    m_MCSSSTMSimulParam.SSTMSimulParam[i].uiTM_CodeRate = m_MCSSSTMParam.SSTMParam[i].uiTM_CodeRate;
                    m_MCSSSTMSimulParam.SSTMSimulParam[i].ucCodingMd = m_MCSSSTMParam.SSTMParam[i].ucCodingMd;
                    m_MCSSSTMSimulParam.SSTMSimulParam[i].ucViterbiMd = m_MCSSSTMParam.SSTMParam[i].ucViterbiMd;
                    m_MCSSSTMSimulParam.SSTMSimulParam[i].ucG2Inv = m_MCSSSTMParam.SSTMParam[i].ucG2Inv;
                    m_MCSSSTMSimulParam.SSTMSimulParam[i].ucRSMd = m_MCSSSTMParam.SSTMParam[i].ucRSMd;
                    m_MCSSSTMSimulParam.SSTMSimulParam[i].ucBasiSel = m_MCSSSTMParam.SSTMParam[i].ucBasiSel;
                    m_MCSSSTMSimulParam.SSTMSimulParam[i].ucInterleav = m_MCSSSTMParam.SSTMParam[i].ucInterleav;
                    m_MCSSSTMSimulParam.SSTMSimulParam[i].ucWordLen = m_MCSSSTMParam.SSTMParam[i].ucWordLen;
                    m_MCSSSTMSimulParam.SSTMSimulParam[i].usFrameLen = m_MCSSSTMParam.SSTMParam[i].usFrameLen;
                    m_MCSSSTMSimulParam.SSTMSimulParam[i].ucSframeLen = m_MCSSSTMParam.SSTMParam[i].ucSframeLen;
                    m_MCSSSTMSimulParam.SSTMSimulParam[i].ucFrameSynGLen = m_MCSSSTMParam.SSTMParam[i].ucFrameSynGLen;
                    m_MCSSSTMSimulParam.SSTMSimulParam[i].uiFrameSynG = m_MCSSSTMParam.SSTMParam[i].uiFrameSynG;
                    m_MCSSSTMSimulParam.SSTMSimulParam[i].ucturbCtrl = m_MCSSSTMParam.SSTMParam[i].ucUnturbCtrl;
                    m_MCSSSTMSimulParam.SSTMSimulParam[i].ucRandLen = m_MCSSSTMParam.SSTMParam[i].ucRandLen;
                    m_MCSSSTMSimulParam.SSTMSimulParam[i].uiRandPoly = m_MCSSSTMParam.SSTMParam[i].uiRandPoly;
                    m_MCSSSTMSimulParam.SSTMSimulParam[i].uiPhase = m_MCSSSTMParam.SSTMParam[i].uiPhase;
                    m_MCSSSTMSimulParam.SSTMSimulParam[i].ucSFrameSynMd = m_MCSSSTMParam.SSTMParam[i].ucSFrameSynMd;
                    m_MCSSSTMSimulParam.SSTMSimulParam[i].ucCycFrmSynCodeLen = m_MCSSSTMParam.SSTMParam[i].ucCycFrmSynCodeLen;
                    m_MCSSSTMSimulParam.SSTMSimulParam[i].usCycFrmSynCodePos = m_MCSSSTMParam.SSTMParam[i].usCycFrmSynCodePos;
                    m_MCSSSTMSimulParam.SSTMSimulParam[i].uiCycFrmSynCode = m_MCSSSTMParam.SSTMParam[i].uiCycFrmSynCode;
                    m_MCSSSTMSimulParam.SSTMSimulParam[i].usIDSubFrmPos = m_MCSSSTMParam.SSTMParam[i].usIDSubFrmPos;
                    m_MCSSSTMSimulParam.SSTMSimulParam[i].ucIDBase = m_MCSSSTMParam.SSTMParam[i].ucIDBase;
                    m_MCSSSTMSimulParam.SSTMSimulParam[i].ucIDDirect = m_MCSSSTMParam.SSTMParam[i].ucIDDirect;
                }
            }
            else
            {
                for (int i = 0; i < MAX_TARGET_COUNT; i++)
                    m_MCSSSTMSimulParam.SSTMSimulParam[i] = m_MCSSSTMSimulStatus->MCSSSB2TMSimulStatu[i].tParams;
            }
        }
        else
        {
            DWORD stLen = sizeof(SMCU2::MCSSSB2TMSimulStatusStruct4);
            if(m_pStateMgr != NULL)
            {
                m_MCSSSTMSimulStatus = (SMCU2::MCSSSB2TMSimulStatusStruct4*)m_pStateMgr->FindOneItem(m_StatusParaMCSSSTMSimulID, stLen);
            }
        }
        if (m_MCSSSTMStatus != NULL)
            for (int i = 0; i < MAX_TARGET_COUNT; i++)
                m_MCSSSTMParam.SSTMParam[i] = m_MCSSSTMStatus->MCSSSB2TMStatu[i].tParams;
        else
        {
            DWORD stLenTM = sizeof(SMCU2::MCSSSB2TMStatusStruct4);
            if(m_pStateMgr != NULL)
            {
                m_MCSSSTMStatus = (SMCU2::MCSSSB2TMStatusStruct4*)m_pStateMgr->FindOneItem(m_StatusParaMCSSSTMID, stLenTM);
            }
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

    //    if (m_MCSSSTMSimulStatus == NULL)
    //        return;
    //显示目标的数据所
    //    for (int i = 0; i < MAX_TARGET_NUM; i++)
    //        showTargetParam(i, */1);
    showTargetParam(m_nTargetNum, 1);
}

//设置帧长范围
void SS2BBE_TMSimu1ParaSetGUIDlg::SetFrameLenRange(int iWordLen)
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

void SS2BBE_TMSimu1ParaSetGUIDlg::SetObjInfo(TLxTsspObjectInfo ObjectInfo)
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
        SUBITEM& sItem = *(pItem->getSubItemByDeviceID_SN(m_ObjectInfo.usLocalDID, 1));
        m_usC = sItem.usCMD;
        m_usTID = sItem.usTID;
        m_usSN = sItem.ucSN;
        /*************************框架**********************************/
        m_ParaMCSSSTMSimulID = GetID(sItem.usStationID,
                                     sItem.ucSubSysID, sItem.usDeviceID,sItem.usTID,
                                     sItem.ucSN);
        /*************************框架**********************************/

        /*************************状态**********************************/
        m_StatusParaMCSSSTMSimulID = GetID(sItem.usStationID,
                                           sItem.ucSubSysID, sItem.usDeviceID,sItem.usStateType,
                                           sItem.ucSN);
    }

    ITEM *pItemTM = m_DeviceMap.getItem("扩频遥测解调参数设置");
    if (pItemTM->vSubItem.size() > 0)
    {
        SUBITEM& sItem = *(pItemTM->getSubItemByDeviceID_SN(m_ObjectInfo.usLocalDID, 1));
        /*************************框架**********************************/
        m_ParaMCSSSTMID = GetID(sItem.usStationID,
                                sItem.ucSubSysID, sItem.usDeviceID,sItem.usTID,
                                sItem.ucSN);
        /*************************框架**********************************/

        /*************************状态**********************************/
        m_StatusParaMCSSSTMID = GetID(sItem.usStationID,
                                      sItem.ucSubSysID, sItem.usDeviceID,sItem.usStateType,
                                      sItem.ucSN);
    }

    //获取目标参数的ID
    ITEM *pItemCom = m_DeviceMap.getItem(COMM_SS2BBE_NAME);
    if (pItemCom != NULL && pItemCom->vSubItem.size() > 0)
    {
         SUBITEM& sItem = *(pItemCom->getSubItemByDeviceID_SN(m_ObjectInfo.usLocalDID, m_ObjectInfo.ucSN));
        /*************************状态**********************************/
        m_CommTargetID = GetID(sItem.usStationID,
                               sItem.ucSubSysID, sItem.usDeviceID,sItem.usStateType,
                               sItem.ucSN);
        /*************************状态**********************************/
    }

    //日志
    m_pILxTsspLogManager = pObjectManager->GetLogManager();
    m_pStateMgr = pObjectManager->GetStateManager();
    m_pILxTsspMessageManager = pObjectManager->GetMessageManager();
    m_pPublicInfo = pObjectManager->GetPublicInfoManager();
}

//使能或不使能控件
void SS2BBE_TMSimu1ParaSetGUIDlg::EnabledControls(bool bFlag)
{
    /*************************Group1*************************/
    //数据输出方式
    ui->comboBox_OutputPhase->setEnabled(bFlag);
    //码型
    ui->comboBox_CodeType->setEnabled(bFlag);
    //译码方式
    ui->comboBox_DecodeType->setEnabled(bFlag);
    //码速率
    ui->lineEdit_CodeRate->setEnabled(bFlag);
    //帧同步码位置
    ui->comboBox_FrmPlace->setEnabled(bFlag);
    //卷积码类型
    ui->comboBox_RecurrentCodeType->setEnabled(bFlag);
    //RS码类型
    ui->comboBox_RSType->setEnabled(bFlag);
    //RS码坐标
    ui->comboBox_RSEncodePlot->setEnabled(bFlag);
    //RS译码交织深度
    ui->lineEdit_RSDecodeDeep->setEnabled(bFlag);
    //卷积输出相位
    ui->comboBox_OutputPhase->setEnabled(bFlag);
    //副载波频率
    ui->comboBox_DataSource->setEnabled(bFlag);
    //数据源类型
    ui->comboBox_DataSourceType->setEnabled(bFlag);
    //固定数设置
    ui->lineEdit_ConstSetting->setEnabled(bFlag);
    //虚拟填充开关
    ui->comboBox_VisualFillSw->setEnabled(false);
    /*************************Group1*************************/

    /*************************Group2*************************/
    //帧长
    ui->lineEdit_FrameLen->setEnabled(bFlag);
    //帧同步码组长
    ui->comboBox_FrameSyncGroupLen->setEnabled(bFlag);
    //帧同步码组
    ui->lineEdit_FrmSyncGroup->setEnabled(bFlag);
    //字长
    ui->lineEdit_WordLen->setEnabled(bFlag);
    //格式长
    ui->lineEdit_FDormatLen->setEnabled(bFlag);
    //加扰使能
    ui->comboBox_RandEn->setEnabled(bFlag);
    //加扰多项式长
    ui->comboBox_RandPolyLen->setEnabled(bFlag);
    //加扰多项式
    ui->lineEdit_RandPoly->setEnabled(bFlag);
    //加扰初相
    ui->lineEdit_RandPhase->setEnabled(bFlag);
    //副帧码组长
    ui->comboBox_SubFrameGroupLen->setEnabled(bFlag);
    //副帧码组
    ui->lineEdit_SubFrmGroup->setEnabled(bFlag);
    //副帧同步方式
    ui->comboBox_SubFrmSyncType->setEnabled(bFlag);
    //副帧码组位置
    ui->lineEdit_SubFrmSynGroupPos->setEnabled(bFlag);
    //ID字基值
    ui->comboBox_IDBaseValue->setEnabled(bFlag);
    //ID计数方向
    ui->comboBox_idDirect->setEnabled(bFlag);
    //ID计数位置
    ui->lineEdit_IDCountPos->setEnabled(bFlag);
    //自动映射
    ui->pushButton_autoMap->setEnabled(bFlag);
    /*************************Group2*************************/

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
int SS2BBE_TMSimu1ParaSetGUIDlg::SaveToMacro(ILxTsspParameterMacro* pMacroObj)
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
        byteArray.resize(sizeof(m_MCSSSTMSimulParam));
        memcpy(byteArray.data(),&m_MCSSSTMSimulParam,sizeof(m_MCSSSTMSimulParam));
        pMacroObj->UpdateParameterBlock(m_ParaMCSSSTMSimulID,byteArray);

        //日志信息
        TLxTsspLog tmpLog;
        QString strLog;
        tmpLog.ucWarningLevel  = 1;
        strLog += QString("模式1遥测模拟源参数设置: 保存宏成功！").arg(MODULENAME);
        CLog::addLog(strLog, 1);

    }
}
//从界面取参数
void SS2BBE_TMSimu1ParaSetGUIDlg::GetPara()
{
    getParamTarget();
}

//验证参数范围
int SS2BBE_TMSimu1ParaSetGUIDlg::CheckPara()
{
    return CheckParam() ? 1 : -1;
}

////定时器
void SS2BBE_TMSimu1ParaSetGUIDlg::timerEvent(QTimerEvent *event)
{
    ShowPara(0);
}

//更改参数按钮休息处理函数
void SS2BBE_TMSimu1ParaSetGUIDlg::on_pushButton_Modify_clicked()
{
    if(ui->pushButton_Modify->text() == "更改")
    {
        EnabledControls(true);
        on_comboBox_RandEn_currentIndexChanged(ui->comboBox_RandEn->currentIndex());
        on_comboBox_RecurrentCodeType_currentIndexChanged(ui->comboBox_RecurrentCodeType->currentIndex());
        on_comboBox_SubFrmSyncType_currentIndexChanged(ui->comboBox_SubFrmSyncType->currentIndex());
        on_comboBox_DecodeType_currentIndexChanged(ui->comboBox_DecodeType->currentIndex());
        on_comboBox_DataSource_currentIndexChanged(ui->comboBox_DataSource->currentIndex());
        on_comboBox_DataSourceType_currentIndexChanged(ui->comboBox_DataSourceType->currentIndex());
        on_lineEdit_WordLen_textChanged(ui->lineEdit_ConstSetting->text());
        killTimer(m_TimerID);
        return;
    }

    if(ui->pushButton_Modify->text() == "恢复")
    {
        m_autoMap = false;
        EnabledControls(false);
        m_TimerID = startTimer(500);
        return;
    }
}

USHORT SS2BBE_TMSimu1ParaSetGUIDlg::getTarget()
{
    USHORT target = 0;
    switch(m_nTargetNum)
    {
    case 0:
        target = 2;
        break;
    case 1:
        target = 4;
        break;
    case 2:
        target = 8;
        break;
    case 3:
        target = 16;
        break;
    default:
        break;
    }
    return target;
}
void SS2BBE_TMSimu1ParaSetGUIDlg::on_pushButton_Set_clicked()
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
            struct{
                USHORT target;
                struct{
                USHORT tmp;
                SMCU2::MCSSSB2TMSimulParamStruct SSTMSimulParam;
                }param[MAX_TARGET_COUNT];
            }data;
            data.target = 0x02 | 0x04 | 0x08 | 0x10;
            for(int i = 0; i < MAX_TARGET_COUNT; ++i)
            {
            data.param[i].SSTMSimulParam = m_MCSSSTMSimulParam.SSTMSimulParam[i];
            }
            QByteArray byteArray;
            byteArray.resize(sizeof(data));
            memcpy(byteArray.data(),&data,sizeof(data));
            SendMessage(byteArray.data(), byteArray.size(), m_usC, m_usTID, m_usSN);
        }

        //日志信息
        TLxTsspLog tmpLog;
        QString strLog;
        memset(&tmpLog, 0, sizeof(TLxTsspLog));
        tmpLog.ucWarningLevel  = 1;
        strLog += "扩频遥测模拟源: 设置参数成功！";
        strcpy(tmpLog.szContent, strLog.toUtf8().data());
        tmpLog.time = QDateTime::currentDateTime().toTime_t();
        m_pILxTsspLogManager->Add(tmpLog);

        //不使能控件
        m_autoMap = false;
        EnabledControls(false);
        m_TimerID = startTimer(500);
    }
}

void SS2BBE_TMSimu1ParaSetGUIDlg::closeEvent(QCloseEvent *event)
{
    event->accept();
}

QSize SS2BBE_TMSimu1ParaSetGUIDlg::sizeHint() const
{
    return QSize(this->width(), this->height());
}

/*
//副帧方式响应函数
void SS2BBE_TMSimu1ParaSetGUIDlg::on_comboBox_SubFrmType_currentIndexChanged(int index)
{
//    //副帧方式控件使能
//    if (ui->pushButton_Modify->text() != "更改" || iParaSetFlag != 0)
//    {
//        EnabledControlsSubFrmType(index);   //副帧方式控件使能
//    }
}

//根据副帧类型，使能副帧控件
void SS2BBE_TMSimu1ParaSetGUIDlg::EnabledControlsSubFrmType(int iSubFrm)
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

void SS2BBE_TMSimu1ParaSetGUIDlg::SubFrmEn(bool bFlag)
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
void SS2BBE_TMSimu1ParaSetGUIDlg::IDSubFrmEn(bool bFlag)
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
void SS2BBE_TMSimu1ParaSetGUIDlg::CycSubFrmEn(bool bFlag)
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
void SS2BBE_TMSimu1ParaSetGUIDlg::ReserveSubFrmEn(bool bFlag)
{

}

void SS2BBE_TMSimu1ParaSetGUIDlg::SendMessage(char* pData, unsigned short  usLen, unsigned short usC, unsigned short usTID, unsigned short usSN)
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
    header.C = 0x200;

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

DWORDLONG SS2BBE_TMSimu1ParaSetGUIDlg::StrToSynCode(const char * pStr)
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

void SS2BBE_TMSimu1ParaSetGUIDlg::showTargetParam(int nIndex, int nType)
{
    if (nType == 0)
    {
        //向下拉框控件中写入从xml中读取的内容
        int i;
        //帧同步码组位置
        ui->comboBox_FrmPlace->clear();
        for(i=0; i<DlgMapLineNum[nIndex * 17 + 0];i++)
        {
            ui->comboBox_FrmPlace->addItem(m_DlgMapMean[0][i]);
        }
        ui->comboBox_FrmPlace->setCurrentIndex(m_DlgParaSetStruct[nIndex][0].InitVal.lval);
        //码型
        ui->comboBox_CodeType->clear();
        for(i=0; i<DlgMapLineNum[nIndex * 17 + 1];i++)
        {
            ui->comboBox_CodeType->addItem(m_DlgMapMean[nIndex * 17 + 1][i]);
        }
        ui->comboBox_CodeType->setCurrentIndex(m_DlgParaSetStruct[nIndex][2].InitVal.lval);
        //编码方式
        ui->comboBox_DecodeType->clear();
        for(i=0; i<DlgMapLineNum[nIndex * 17 + 2];i++)
        {
            ui->comboBox_DecodeType->addItem(m_DlgMapMean[nIndex * 17 + 2][i]);
        }
        ui->comboBox_DecodeType->setCurrentIndex(m_DlgParaSetStruct[nIndex][4].InitVal.lval);
        //RS码类型
        ui->comboBox_RSType->clear();
        for(i=0; i<DlgMapLineNum[nIndex * 17 + 3];i++)
        {
            ui->comboBox_RSType->addItem(m_DlgMapMean[nIndex * 17 + 3][i]);
        }
        ui->comboBox_RSType->setCurrentIndex(m_DlgParaSetStruct[nIndex][5].InitVal.lval);
        //RS编码坐标基
        ui->comboBox_RSEncodePlot->clear();
        for(i=0; i<DlgMapLineNum[nIndex * 17 + 4];i++)
        {
            ui->comboBox_RSEncodePlot->addItem(m_DlgMapMean[nIndex * 17 + 4][i]);
        }
        ui->comboBox_RSEncodePlot->setCurrentIndex(m_DlgParaSetStruct[nIndex][6].InitVal.lval);
        //数据源选择
        ui->comboBox_DataSource->clear();
        for(i=0; i<DlgMapLineNum[nIndex * 17 + 5];i++)
        {
            ui->comboBox_DataSource->addItem(m_DlgMapMean[nIndex * 17 + 5][i]);
        }
        ui->comboBox_DataSource->setCurrentIndex(m_DlgParaSetStruct[nIndex][7].InitVal.lval);
        //虚拟填充开关
        ui->comboBox_VisualFillSw->clear();
        for(i=0; i<DlgMapLineNum[nIndex * 17 + 6];i++)
        {
            ui->comboBox_VisualFillSw->addItem(m_DlgMapMean[nIndex * 17 + 6][i]);
        }
        ui->comboBox_VisualFillSw->setCurrentIndex(m_DlgParaSetStruct[nIndex][8].InitVal.lval);
        //交织深度
        ui->lineEdit_RSDecodeDeep->setText(QString::number(m_DlgParaSetStruct[nIndex][9].InitVal.lval));
        ui->label_RSDecodeDeep->setText("交织深度\n[" + QString::number(m_DlgParaSetStruct[nIndex][9].MinVal.lmin) + "~" + QString::number(m_DlgParaSetStruct[nIndex][9].MaxVal.lmax) + "]");
        //卷积输出相位
        ui->comboBox_OutputPhase->clear();
        for(i=0; i<DlgMapLineNum[nIndex * 17 + 7];i++)
        {
            ui->comboBox_OutputPhase->addItem(m_DlgMapMean[nIndex * 17 + 7][i]);
        }
        ui->comboBox_OutputPhase->setCurrentIndex(m_DlgParaSetStruct[nIndex][10].InitVal.lval);
        //卷积码类型
        ui->comboBox_RecurrentCodeType->clear();
        for(i=0; i<DlgMapLineNum[nIndex * 17 + 8];i++)
        {
            ui->comboBox_RecurrentCodeType->addItem(m_DlgMapMean[nIndex * 17 + 8][i]);
        }
        ui->comboBox_RecurrentCodeType->setCurrentIndex(m_DlgParaSetStruct[nIndex][11].InitVal.lval);
        //数据源类型
        ui->comboBox_DataSourceType->clear();
        for(i=0; i<DlgMapLineNum[nIndex * 17 + 9];i++)
        {
            ui->comboBox_DataSourceType->addItem(m_DlgMapMean[nIndex * 17 + 9][i]);
        }
        ui->comboBox_DataSourceType->setCurrentIndex(m_DlgParaSetStruct[nIndex][12].InitVal.lval);
        //帧同步码组长
        ui->comboBox_FrameSyncGroupLen->clear();
        for(i=0; i<DlgMapLineNum[nIndex * 17 + 10];i++)
        {
            ui->comboBox_FrameSyncGroupLen->addItem(m_DlgMapMean[nIndex * 17 + 10][i]);
        }
        ui->comboBox_FrameSyncGroupLen->setCurrentIndex(m_DlgParaSetStruct[nIndex][16].InitVal.lval);
        //加扰控制
        ui->comboBox_RandEn->clear();
        for(i=0; i<DlgMapLineNum[nIndex * 17 + 11];i++)
        {
            ui->comboBox_RandEn->addItem(m_DlgMapMean[nIndex * 17 + 11][i]);
        }
        ui->comboBox_RandEn->setCurrentIndex(m_DlgParaSetStruct[nIndex][18].InitVal.lval);
        //加扰多项式长度SS2BBE_TMSimu1ParaSetGUIDlg
        ui->comboBox_RandPolyLen->clear();
        for(i=0; i<DlgMapLineNum[nIndex * 17 + 12];i++)
        {
            ui->comboBox_RandPolyLen->addItem(m_DlgMapMean[nIndex * 17 + 12][i]);
        }
        ui->comboBox_RandPolyLen->setCurrentIndex(m_DlgParaSetStruct[nIndex][20].InitVal.lval);
        //副帧码组长度
        ui->comboBox_SubFrameGroupLen->clear();
        for(i=0; i<DlgMapLineNum[nIndex * 17 + 13];i++)
        {
            ui->comboBox_SubFrameGroupLen->addItem(m_DlgMapMean[nIndex * 17 + 13][i]);
        }
        ui->comboBox_SubFrameGroupLen->setCurrentIndex(m_DlgParaSetStruct[nIndex][22].InitVal.lval);
        //副帧同步方式
        ui->comboBox_SubFrmSyncType->clear();
        for(i=0; i<DlgMapLineNum[nIndex * 17 + 14];i++)
        {
            ui->comboBox_SubFrmSyncType->addItem(m_DlgMapMean[nIndex * 17 + 14][i]);
        }
        ui->comboBox_SubFrmSyncType->setCurrentIndex(m_DlgParaSetStruct[nIndex][23].InitVal.lval);
        //ID字基值
        ui->comboBox_IDBaseValue->clear();
        for(i=0; i<DlgMapLineNum[nIndex * 17 + 15];i++)
        {
            ui->comboBox_IDBaseValue->addItem(m_DlgMapMean[nIndex * 17 + 15][i]);
        }
        ui->comboBox_IDBaseValue->setCurrentIndex(m_DlgParaSetStruct[nIndex][26].InitVal.lval);
        //ID计数方向
        ui->comboBox_idDirect->clear();
        for(i=0; i<DlgMapLineNum[16];i++)
        {
            ui->comboBox_idDirect->addItem(m_DlgMapMean[nIndex * 17 + 16][i]);
        }
        ui->comboBox_idDirect->setCurrentIndex(m_DlgParaSetStruct[nIndex][27].InitVal.lval);

        //往静态控件中写入参数缺省值
        //码速率
        ui->lineEdit_CodeRate->setText(QString::number(m_DlgParaSetStruct[nIndex][1].InitVal.lval));
        //固定数设置
        ui->lineEdit_ConstSetting->setText(QString::number(m_DlgParaSetStruct[nIndex][3].InitVal.lval, 16).toUpper());
        //帧长
        ui->lineEdit_FrameLen->setText(QString::number(m_DlgParaSetStruct[nIndex][13].InitVal.lval));
        //字长
        ui->lineEdit_WordLen->setText(QString::number(m_DlgParaSetStruct[nIndex][14].InitVal.lval));
        //格式长
        ui->lineEdit_FDormatLen->setText(QString::number(m_DlgParaSetStruct[nIndex][15].InitVal.lval));
        //帧同步码组
        ui->lineEdit_FrmSyncGroup->setText(QString::number(m_DlgParaSetStruct[nIndex][17].InitVal.lval, 16).toUpper());
        //加扰多项式
        ui->lineEdit_RandPoly->setText(QString::number(m_DlgParaSetStruct[nIndex][19].InitVal.lval, 16).toUpper());
        //加扰初相
        ui->lineEdit_RandPhase->setText(QString::number(m_DlgParaSetStruct[nIndex][21].InitVal.lval, 16).toUpper());
        //副帧码组位置
        ui->lineEdit_SubFrmSynGroupPos->setText(QString::number(m_DlgParaSetStruct[nIndex][24].InitVal.lval));
        //副帧同步码组
        ui->lineEdit_SubFrmGroup->setText(QString::number(m_DlgParaSetStruct[nIndex][25].InitVal.lval, 16).toUpper());
        //ID计数位置
        ui->lineEdit_IDCountPos->setText(QString::number(m_DlgParaSetStruct[nIndex][28].InitVal.lval));
    }
    else if (nType == 1)
    {
        //帧同步码组位置
        int index = m_MCSSSTMSimulParam.SSTMSimulParam[nIndex].ucFspPlace;
        if(index != 1 && index != 2)index = 0;
        else --index;
        ui->comboBox_FrmPlace->setCurrentIndex(index);
        //码型
        if (m_MCSSSTMSimulParam.SSTMSimulParam[nIndex].uiCodeStyle > 0)
            ui->comboBox_CodeType->setCurrentIndex(m_MCSSSTMSimulParam.SSTMSimulParam[nIndex].uiCodeStyle - 1);
        //编码方式
        if (m_MCSSSTMSimulParam.SSTMSimulParam[nIndex].ucCodingMd > 0)
            ui->comboBox_DecodeType->setCurrentIndex(m_MCSSSTMSimulParam.SSTMSimulParam[nIndex].ucCodingMd - 1);
        //RS码类型
        if (m_MCSSSTMSimulParam.SSTMSimulParam[nIndex].ucRSMd > 0)
            ui->comboBox_RSType->setCurrentIndex(m_MCSSSTMSimulParam.SSTMSimulParam[nIndex].ucRSMd - 1);
        //RS编码坐标基
        if (m_MCSSSTMSimulParam.SSTMSimulParam[nIndex].ucBasiSel > 0)
            ui->comboBox_RSEncodePlot->setCurrentIndex(m_MCSSSTMSimulParam.SSTMSimulParam[nIndex].ucBasiSel - 1);
        //数据源选择
        if (m_MCSSSTMSimulParam.SSTMSimulParam[nIndex].ucDataCheck > 0)
            ui->comboBox_DataSource->setCurrentIndex(m_MCSSSTMSimulParam.SSTMSimulParam[nIndex].ucDataCheck - 1);
        //虚拟填充开关
        //        ui->comboBox_VisualFillSw->setCurrentIndex(m_MCSSSTMSimulParam.SSTMSimulParam[nIndex].u);
        //交织深度
        ui->lineEdit_RSDecodeDeep->setText(QString::number(m_MCSSSTMSimulParam.SSTMSimulParam[nIndex].ucInterleav));
        //卷积输出相位
        if (m_MCSSSTMSimulParam.SSTMSimulParam[nIndex].ucG2Inv > 0)
            ui->comboBox_OutputPhase->setCurrentIndex(m_MCSSSTMSimulParam.SSTMSimulParam[nIndex].ucG2Inv - 1);
        //卷积码类型
        if (m_MCSSSTMSimulParam.SSTMSimulParam[nIndex].ucViterbiMd > 0)
            ui->comboBox_RecurrentCodeType->setCurrentIndex(m_MCSSSTMSimulParam.SSTMSimulParam[nIndex].ucViterbiMd - 1);
        //数据源类型showRadioButton()
        if (m_MCSSSTMSimulParam.SSTMSimulParam[nIndex].ucdatasel > 0)
            ui->comboBox_DataSourceType->setCurrentIndex(m_MCSSSTMSimulParam.SSTMSimulParam[nIndex].ucdatasel - 1);
        //帧同步码组长
        if (m_MCSSSTMSimulParam.SSTMSimulParam[nIndex].ucFrameSynGLen > 0)
            ui->comboBox_FrameSyncGroupLen->setCurrentIndex(m_MCSSSTMSimulParam.SSTMSimulParam[nIndex].ucFrameSynGLen - 1);
        //加扰控制
        ui->comboBox_RandEn->setCurrentIndex(m_MCSSSTMSimulParam.SSTMSimulParam[nIndex].ucturbCtrl - 1);
        //加扰多项式长度
        index = m_MCSSSTMSimulParam.SSTMSimulParam[nIndex].ucRandLen;
        ui->comboBox_RandPolyLen->setCurrentIndex(index == 0 ? 0 : index - 1);
        //副帧码组长度
        if (m_MCSSSTMSimulParam.SSTMSimulParam[nIndex].ucCycFrmSynCodeLen > 0)
            ui->comboBox_SubFrameGroupLen->setCurrentIndex(m_MCSSSTMSimulParam.SSTMSimulParam[nIndex].ucCycFrmSynCodeLen - 1);
        //副帧同步方式
        if (m_MCSSSTMSimulParam.SSTMSimulParam[nIndex].ucSFrameSynMd > 0)
            ui->comboBox_SubFrmSyncType->setCurrentIndex(m_MCSSSTMSimulParam.SSTMSimulParam[nIndex].ucSFrameSynMd - 1);
        //ID字基值
        if (m_MCSSSTMSimulParam.SSTMSimulParam[nIndex].ucIDBase > 0)
            ui->comboBox_IDBaseValue->setCurrentIndex(m_MCSSSTMSimulParam.SSTMSimulParam[nIndex].ucIDBase - 1);
        //ID计数方向
        if (m_MCSSSTMSimulParam.SSTMSimulParam[nIndex].ucIDDirect > 0)
            ui->comboBox_idDirect->setCurrentIndex(m_MCSSSTMSimulParam.SSTMSimulParam[nIndex].ucIDDirect - 1);

        //码速率
        ui->lineEdit_CodeRate->setText(QString::number(m_MCSSSTMSimulParam.SSTMSimulParam[nIndex].uiTM_CodeRate));
        //固定数设置
        ui->lineEdit_ConstSetting->setText(QString::number(m_MCSSSTMSimulParam.SSTMSimulParam[nIndex].uidata, 16).toUpper());
        //帧长
        ui->lineEdit_FrameLen->setText(QString::number(m_MCSSSTMSimulParam.SSTMSimulParam[nIndex].usFrameLen));
        //字长
        ui->lineEdit_WordLen->setText(QString::number(m_MCSSSTMSimulParam.SSTMSimulParam[nIndex].ucWordLen));
        //格式长
        ui->lineEdit_FDormatLen->setText(QString::number(m_MCSSSTMSimulParam.SSTMSimulParam[nIndex].ucSframeLen));
        //帧同步码组
        ui->lineEdit_FrmSyncGroup->setText(QString::number(m_MCSSSTMSimulParam.SSTMSimulParam[nIndex].uiFrameSynG, 16).toUpper());
        //加扰多项式
        ui->lineEdit_RandPoly->setText(QString::number(m_MCSSSTMSimulParam.SSTMSimulParam[nIndex].uiRandPoly, 16).toUpper());
        //加扰初相
        ui->lineEdit_RandPhase->setText(QString::number(m_MCSSSTMSimulParam.SSTMSimulParam[nIndex].uiPhase, 16).toUpper());
        //副帧码组位置
        ui->lineEdit_SubFrmSynGroupPos->setText(QString::number(m_MCSSSTMSimulParam.SSTMSimulParam[nIndex].usCycFrmSynCodePos));
        //副帧同步码组
        ui->lineEdit_SubFrmGroup->setText(QString::number(m_MCSSSTMSimulParam.SSTMSimulParam[nIndex].uiCycFrmSynCode, 16).toUpper());
        //ID计数位置
        ui->lineEdit_IDCountPos->setText(QString::number(m_MCSSSTMSimulParam.SSTMSimulParam[nIndex].usIDSubFrmPos));

    }
}

void SS2BBE_TMSimu1ParaSetGUIDlg::getParamTarget()
{
    bool ok;
    //帧同步码组位置
    m_MCSSSTMSimulParam.SSTMSimulParam[m_nTargetNum].ucFspPlace = ui->comboBox_FrmPlace->currentIndex() + 1;
    //码型
    m_MCSSSTMSimulParam.SSTMSimulParam[m_nTargetNum].uiCodeStyle = ui->comboBox_CodeType->currentIndex() + 1;
    //编码方式
    m_MCSSSTMSimulParam.SSTMSimulParam[m_nTargetNum].ucCodingMd = ui->comboBox_DecodeType->currentIndex() + 1;
    //RS码类型
    m_MCSSSTMSimulParam.SSTMSimulParam[m_nTargetNum].ucRSMd = ui->comboBox_RSType->currentIndex() + 1;
    //RS编码坐标基
    m_MCSSSTMSimulParam.SSTMSimulParam[m_nTargetNum].ucBasiSel = ui->comboBox_RSEncodePlot->currentIndex() + 1;
    //数据源选择
    m_MCSSSTMSimulParam.SSTMSimulParam[m_nTargetNum].ucDataCheck = ui->comboBox_DataSource->currentIndex() + 1;
    //交织深度
    m_MCSSSTMSimulParam.SSTMSimulParam[m_nTargetNum].ucInterleav = ui->lineEdit_RSDecodeDeep->text().toInt();
    //卷积输出相位
    m_MCSSSTMSimulParam.SSTMSimulParam[m_nTargetNum].ucG2Inv = ui->comboBox_OutputPhase->currentIndex() + 1;
    //卷积码类型
    m_MCSSSTMSimulParam.SSTMSimulParam[m_nTargetNum].ucViterbiMd = ui->comboBox_RecurrentCodeType->currentIndex() + 1;
    //数据源类型
    m_MCSSSTMSimulParam.SSTMSimulParam[m_nTargetNum].ucdatasel = ui->comboBox_DataSourceType->currentIndex() + 1;
    //帧同步码组长
    m_MCSSSTMSimulParam.SSTMSimulParam[m_nTargetNum].ucFrameSynGLen = ui->comboBox_FrameSyncGroupLen->currentIndex() + 1;
    //加扰控制
    m_MCSSSTMSimulParam.SSTMSimulParam[m_nTargetNum].ucturbCtrl = ui->comboBox_RandEn->currentIndex() + 1;
    //加扰多项式长度
    m_MCSSSTMSimulParam.SSTMSimulParam[m_nTargetNum].ucRandLen = ui->comboBox_RandPolyLen->currentIndex() + 1;
    //副帧码组长度
    m_MCSSSTMSimulParam.SSTMSimulParam[m_nTargetNum].ucCycFrmSynCodeLen = ui->comboBox_SubFrameGroupLen->currentIndex() + 1;
    //副帧同步方式
    m_MCSSSTMSimulParam.SSTMSimulParam[m_nTargetNum].ucSFrameSynMd = ui->comboBox_SubFrmSyncType->currentIndex() + 1;
    //ID字基值
    m_MCSSSTMSimulParam.SSTMSimulParam[m_nTargetNum].ucIDBase = ui->comboBox_IDBaseValue->currentIndex() + 1;
    //ID计数方向
    m_MCSSSTMSimulParam.SSTMSimulParam[m_nTargetNum].ucIDDirect = ui->comboBox_idDirect->currentIndex() + 1;

    //码速率
    m_MCSSSTMSimulParam.SSTMSimulParam[m_nTargetNum].uiTM_CodeRate = ui->lineEdit_CodeRate->text().toInt();
    //固定数设置
    m_MCSSSTMSimulParam.SSTMSimulParam[m_nTargetNum].uidata = ui->lineEdit_ConstSetting->text().toInt(&ok, 16);
    //帧长
    m_MCSSSTMSimulParam.SSTMSimulParam[m_nTargetNum].usFrameLen = ui->lineEdit_FrameLen->text().toInt();
    //字长
    m_MCSSSTMSimulParam.SSTMSimulParam[m_nTargetNum].ucWordLen = ui->lineEdit_WordLen->text().toInt();
    //格式长
    m_MCSSSTMSimulParam.SSTMSimulParam[m_nTargetNum].ucSframeLen = ui->lineEdit_FDormatLen->text().toInt();
    //帧同步码组
    m_MCSSSTMSimulParam.SSTMSimulParam[m_nTargetNum].uiFrameSynG = ui->lineEdit_FrmSyncGroup->text().toUInt(&ok, 16);
    //加扰多项式
    m_MCSSSTMSimulParam.SSTMSimulParam[m_nTargetNum].uiRandPoly = ui->lineEdit_RandPoly->text().toUInt(&ok, 16);
    //加扰初相
    m_MCSSSTMSimulParam.SSTMSimulParam[m_nTargetNum].uiPhase = ui->lineEdit_RandPhase->text().toUInt(&ok, 16);
    //副帧码组位置
    m_MCSSSTMSimulParam.SSTMSimulParam[m_nTargetNum].usCycFrmSynCodePos = ui->lineEdit_SubFrmSynGroupPos->text().toInt();
    //副帧同步码组
    m_MCSSSTMSimulParam.SSTMSimulParam[m_nTargetNum].uiCycFrmSynCode = ui->lineEdit_SubFrmGroup->text().toUInt(&ok, 16);
    //ID计数位置
    m_MCSSSTMSimulParam.SSTMSimulParam[m_nTargetNum].usIDSubFrmPos = ui->lineEdit_IDCountPos->text().toInt();
}

int SS2BBE_TMSimu1ParaSetGUIDlg::getSharedMemoryTargetNum()
{
    DWORD stLen = sizeof(SMCU2::MCSSysTarget4StatusStruct);
    if(m_pStateMgr != NULL)
        m_pMCSTargetSysStatus  = (SMCU2::MCSSysTarget4StatusStruct*)m_pStateMgr->FindOneItem(m_CommTargetID, stLen);

    if (m_pMCSTargetSysStatus != NULL)
        return m_pMCSTargetSysStatus->MCSSysStatu.tParams.ucTargetNum;

    return 0;
}

void SS2BBE_TMSimu1ParaSetGUIDlg::showRadioButton()
{
    int nNum = getSharedMemoryTargetNum();
    if (m_currentNumber_usedByParam == nNum)
        return;
    m_currentNumber_usedByParam = nNum;
    m_nTargetNum = 0;
    set_radioButton_Target();
    ui->radioButton_Target1->hide();
    ui->radioButton_Target2->hide();
    ui->radioButton_Target3->hide();
    ui->radioButton_Target4->hide();
    switch(nNum -1)
    {
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

void SS2BBE_TMSimu1ParaSetGUIDlg::updateRadioButton(int nNum)
{
    if (m_currentNumber_usedByMacro == nNum)
        return;
    m_currentNumber_usedByMacro = nNum;
    m_nTargetNum = 0;
    set_radioButton_Target();
    ui->radioButton_Target1->hide();
    ui->radioButton_Target2->hide();
    ui->radioButton_Target3->hide();
    ui->radioButton_Target4->hide();
    switch(nNum)
    {
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

void SS2BBE_TMSimu1ParaSetGUIDlg::on_pushButton_autoMap_clicked()
{
    m_autoMap = true;
    ui->pushButton_autoMap->setEnabled(false);
    ShowPara(0);
}
void SS2BBE_TMSimu1ParaSetGUIDlg::set_radioButton_Target()
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
void SS2BBE_TMSimu1ParaSetGUIDlg::on_radioButton_Target1_clicked()
{
    if(1 != CheckPara())
    {
        set_radioButton_Target();
        return;
    }
    GetPara();
    //界面显示对应的参数数据
    m_nTargetNum = 0;
    int nType = ui->pushButton_Set->isEnabled()  ? 0 : 1;
    showTargetParam(0, 1);
}

void SS2BBE_TMSimu1ParaSetGUIDlg::on_radioButton_Target2_clicked()
{
    if(1 != CheckPara())
    {
        set_radioButton_Target();
        return;
    }
    GetPara();
    m_nTargetNum = 1;
    int nType = ui->pushButton_Set->isEnabled() ? 0 : 1;
    showTargetParam(1, 1);
}

void SS2BBE_TMSimu1ParaSetGUIDlg::on_radioButton_Target3_clicked()
{
    if(1 != CheckPara())
    {
        set_radioButton_Target();
        return;
    }
    GetPara();
    m_nTargetNum = 2;
    int nType = ui->pushButton_Set->isEnabled()  ? 0 : 1;
    showTargetParam(2, 1);
}

void SS2BBE_TMSimu1ParaSetGUIDlg::on_radioButton_Target4_clicked()
{
    if(1 != CheckPara())
    {
        set_radioButton_Target();
        return;
    }
    GetPara();
    m_nTargetNum = 3;
    int nType = ui->pushButton_Set->isEnabled()  ? 0 : 1;
    showTargetParam(3, 1);
}
//数据源
void SS2BBE_TMSimu1ParaSetGUIDlg::on_comboBox_DataSource_currentIndexChanged(int index)
{
    if (ui->pushButton_Modify->text() == "更改")
        return;
    if(index==0){
        ui->comboBox_DataSourceType->setEnabled(true);
        on_comboBox_DataSourceType_currentIndexChanged(ui->comboBox_DataSourceType->currentIndex());
    }
    if(index==1){
        ui->comboBox_DataSourceType->setEnabled(false);
        ui->lineEdit_ConstSetting->setEnabled(false);
        on_comboBox_DataSourceType_currentIndexChanged(ui->comboBox_DataSourceType->currentIndex());
    }

}

void SS2BBE_TMSimu1ParaSetGUIDlg::on_comboBox_DataSourceType_currentIndexChanged(int index)
{
    if (ui->pushButton_Modify->text() == "更改")
        return;
    if(index == 0 && ui->comboBox_DataSource->currentIndex() == 0){
        ui->lineEdit_ConstSetting->setEnabled(true);

    }
    else{
        ui->lineEdit_ConstSetting->setEnabled(false);
    }
}
//编码方式
void SS2BBE_TMSimu1ParaSetGUIDlg::on_comboBox_DecodeType_currentIndexChanged(int index)
{
    if (ui->pushButton_Modify->text() == "更改")
        return;
    if (index == 0){//关
        ui->comboBox_FrmPlace->setEnabled(true);
        ui->comboBox_RecurrentCodeType->setEnabled(false);
        ui->comboBox_RSType->setEnabled(false);
        ui->lineEdit_RSDecodeDeep->setEnabled(false);
        ui->comboBox_OutputPhase->setEnabled(false);
        ui->comboBox_RSEncodePlot->setEnabled(false);
        ui->comboBox_SubFrmSyncType->setEnabled(true);
        ui->lineEdit_WordLen->setEnabled(true);
        on_comboBox_SubFrmSyncType_currentIndexChanged(ui->comboBox_SubFrmSyncType->currentIndex());
    }
    if(index == 1){

        ui->comboBox_FrmPlace->setEnabled(true);
        ui->comboBox_RecurrentCodeType->setEnabled(true);
        ui->comboBox_RSType->setEnabled(false);
        ui->lineEdit_RSDecodeDeep->setEnabled(false);
        ui->comboBox_OutputPhase->setEnabled(true);
        ui->comboBox_RSEncodePlot->setEnabled(false);
        ui->comboBox_SubFrmSyncType->setEnabled(true);
        ui->lineEdit_WordLen->setEnabled(true);
        on_comboBox_SubFrmSyncType_currentIndexChanged(ui->comboBox_SubFrmSyncType->currentIndex());
    }
    if(index == 2){
        ui->comboBox_FrmPlace->setEnabled(false);
        ui->comboBox_FrmPlace->setCurrentIndex(0);
        ui->comboBox_RecurrentCodeType->setEnabled(false);
        ui->comboBox_RSType->setEnabled(true);
        ui->lineEdit_RSDecodeDeep->setEnabled(true);
        ui->comboBox_OutputPhase->setEnabled(false);
        ui->comboBox_RSEncodePlot->setEnabled(true);
        ui->comboBox_SubFrmSyncType->setEnabled(false);
        ui->comboBox_SubFrmSyncType->setCurrentIndex(0);
        ui->lineEdit_WordLen->setEnabled(false);
        ui->lineEdit_WordLen->setText("8");
        on_comboBox_SubFrmSyncType_currentIndexChanged(ui->comboBox_SubFrmSyncType->currentIndex());

    }
    if(index == 3){
        ui->comboBox_FrmPlace->setEnabled(false);
        ui->comboBox_FrmPlace->setCurrentIndex(0);
        ui->comboBox_RecurrentCodeType->setEnabled(true);
        ui->comboBox_RSType->setEnabled(true);
        ui->lineEdit_RSDecodeDeep->setEnabled(true);
        ui->comboBox_OutputPhase->setEnabled(true);
        ui->comboBox_RSEncodePlot->setEnabled(true);
        ui->comboBox_SubFrmSyncType->setEnabled(false);
        ui->comboBox_SubFrmSyncType->setCurrentIndex(0);
        ui->lineEdit_WordLen->setEnabled(false);
        ui->lineEdit_WordLen->setText("8");
        on_comboBox_SubFrmSyncType_currentIndexChanged(ui->comboBox_SubFrmSyncType->currentIndex());
    }


}
//副帧同步方式
void SS2BBE_TMSimu1ParaSetGUIDlg::on_comboBox_SubFrmSyncType_currentIndexChanged(int index)
{
    if (ui->pushButton_Modify->text() == "更改")
        return;

    if(index == 0){//无副帧

        ui->comboBox_SubFrameGroupLen->setEnabled(false);
        ui->lineEdit_SubFrmSynGroupPos->setEnabled(false);
        ui->lineEdit_SubFrmGroup->setEnabled(false);
        ui->comboBox_IDBaseValue->setEnabled(false);
        ui->comboBox_idDirect->setEnabled(false);
        ui->lineEdit_IDCountPos->setEnabled(false);
        ui->lineEdit_FDormatLen->setEnabled(false);

    }
    if(index == 2){//循环副帧
        ui->comboBox_SubFrameGroupLen->setEnabled(true);
        ui->lineEdit_SubFrmSynGroupPos->setEnabled(true);
        ui->lineEdit_SubFrmGroup->setEnabled(true);
        ui->comboBox_IDBaseValue->setEnabled(false);
        ui->comboBox_idDirect->setEnabled(false);
        ui->lineEdit_IDCountPos->setEnabled(false);
        ui->lineEdit_FDormatLen->setEnabled(true);


    }
    if(index == 1){//ID副帧

        ui->comboBox_SubFrameGroupLen->setEnabled(false);
        ui->lineEdit_SubFrmSynGroupPos->setEnabled(false);
        ui->lineEdit_SubFrmGroup->setEnabled(false);
        ui->comboBox_IDBaseValue->setEnabled(true);
        ui->comboBox_idDirect->setEnabled(true);
        ui->lineEdit_IDCountPos->setEnabled(true);
        ui->lineEdit_FDormatLen->setEnabled(true);

    }
    if(index == 3){//反码副帧
        ui->comboBox_SubFrameGroupLen->setEnabled(false);
        ui->lineEdit_SubFrmSynGroupPos->setEnabled(false);
        ui->lineEdit_SubFrmGroup->setEnabled(false);
        ui->comboBox_IDBaseValue->setEnabled(false);
        ui->comboBox_idDirect->setEnabled(false);
        ui->lineEdit_IDCountPos->setEnabled(false);
        ui->lineEdit_FDormatLen->setEnabled(true);
    }

}
//卷及码类型
void SS2BBE_TMSimu1ParaSetGUIDlg::on_comboBox_RecurrentCodeType_currentIndexChanged(int index)
{
    if (ui->pushButton_Modify->text() == "更改")
        return;
    // 1/2
    if(index == 0){
        if(ui->comboBox_DecodeType->currentIndex() == 1 ||
                ui->comboBox_DecodeType->currentIndex() == 3)
            ui->comboBox_OutputPhase->setEnabled(true);
    }
    // 3/4
    if(index == 1){
        ui->comboBox_OutputPhase->setEnabled(false);
        ui->comboBox_OutputPhase->setCurrentIndex(0);
    }

}
//解扰控制
void SS2BBE_TMSimu1ParaSetGUIDlg::on_comboBox_RandEn_currentIndexChanged(int index)
{
    if (ui->pushButton_Modify->text() == "更改")
        return;
    ui->comboBox_RandPolyLen->setEnabled(false);
    ui->lineEdit_RandPoly->setEnabled(false);
    ui->lineEdit_RandPhase->setEnabled(false);
    if(index == 2){//人工
        ui->comboBox_RandPolyLen->setEnabled(true);
        ui->lineEdit_RandPoly->setEnabled(true);
        ui->lineEdit_RandPhase->setEnabled(true);
    }
    else{
        ui->comboBox_RandPolyLen->setEnabled(false);
        ui->lineEdit_RandPoly->setEnabled(false);
        ui->lineEdit_RandPhase->setEnabled(false);
    }
}
void SS2BBE_TMSimu1ParaSetGUIDlg::updateTarget()
{
    if(!m_pCommFunc)return;
    updateRadioButton(m_pCommFunc->getTargetNum() - 1);
}
void SS2BBE_TMSimu1ParaSetGUIDlg::on_lineEdit_WordLen_textChanged(const QString &arg1)
{
    int cNum;
    int wordLen = arg1.toInt();
    if (wordLen == 4)
        cNum = 1;
    else if (wordLen > 4 && wordLen < 9)
        cNum = 2;
    else if (wordLen > 8 && wordLen < 13)
        cNum = 3;
    else if (wordLen > 12 && wordLen < 17)
        cNum = 4;
    else
        return;
    QRegExp regExp(QString("[A-Fa-f0-9]{1,%1}").arg(cNum));
    ui->lineEdit_ConstSetting->setValidator(new QRegExpValidator(regExp, this));
}
#pragma pack()

