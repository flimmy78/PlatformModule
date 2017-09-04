#include "SS2BBE_TMParaSetGUIDlg.h"
#include "ui_SS2BBE_TMParaSetGUIDlg.h"
#include <QCloseEvent>
#include <QMessageBox>
#include <QDateTime>
#include "../../Common/Log.h"
#include "../../Common/CommonFunction.h"
#include <QTimer>
#pragma pack(1)
extern quint64 GetID(ushort usStationID, uchar ucSubSysID, ushort usDeviceID, ushort usType,
                     uchar ucSn);
SS2BBE_TMParaSetGUIDlg::SS2BBE_TMParaSetGUIDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SS2BBE_TMParaSetGUIDlg)
{
    ui->setupUi(this);
    m_pStateMgr = NULL;
    m_pILxTsspMessageManager = NULL;
    m_pILxTsspLogManager = NULL;
    m_pPublicInfo = NULL;
    m_pHAManager = NULL;
    m_nTargetNum = 0;
    m_MCSSSTMStatus = NULL;
    m_pMCSTargetSysStatus = NULL;
    m_timer1 = new QTimer(this);
    m_currentNumber_usedByMacro = -1;
    m_currentNumber_usedByParam = -1;
    connect(m_timer1, SIGNAL(timeout()), this, SLOT(updateRadioButton()));

    setAttribute(Qt::WA_DeleteOnClose);
}

SS2BBE_TMParaSetGUIDlg::~SS2BBE_TMParaSetGUIDlg()
{
    killTimer(m_TimerID);
    m_timer1->stop();
    delete m_timer1;
    delete ui;
}

void SS2BBE_TMParaSetGUIDlg::setText(const QString& strTitle, ParaStruct xmlsetText[4][60], QString MapMean[100][30], int MapNum[100], uchar Type, ILxTsspParameterMacro* pMacroObj)
{
    m_strTitle = strTitle;
    setWindowTitle(strTitle);

    memcpy(m_DlgParaSetStruct, xmlsetText, sizeof(ParaStruct) * 60 * 4);

    for(int i = 0;i < 100;i++)
    {
        for(int j = 0; j < 30; j++)
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
void SS2BBE_TMParaSetGUIDlg::InitDLG(int Type, ILxTsspParameterMacro* pMacroObj)
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
            DWORD stLen = sizeof(SMCU2::MCSSSB2TMStatusStruct4);
            m_MCSSSTMStatus = (SMCU2::MCSSSB2TMStatusStruct4*)m_pStateMgr->FindOneItem(m_StatusParaMCSSSTMID, stLen);
            if(m_MCSSSTMStatus == NULL || stLen != sizeof(SMCU2::MCSSSB2TMStatusStruct4))
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
        if(pMacroObj != NULL)
        {
            QByteArray byteArray;
            pMacroObj->GetParameterBlock(m_ParaMCSSSTMID, byteArray);
            memcpy(&m_MCSSSTMParam, byteArray.data(), sizeof(SMCU2::MCSSSB2TMParamStruct4));
        }
        ui->pushButton_Modify->setVisible(false);
        ui->pushButton_Set->setVisible(false);
        m_pCommFunc = CCommFunc::getInstance();
        m_timer1->start(500);
        //显示参数
        ShowPara(1);
        on_comboBox_DecodeType_currentIndexChanged(ui->comboBox_DecodeType->currentIndex());
        on_comboBox_SubFrmSyncType_currentIndexChanged(ui->comboBox_SubFrmSyncType->currentIndex());
        on_comboBox_ViterbType_currentIndexChanged(ui->comboBox_ViterbType->currentIndex());
        on_comboBox_RandEn_currentIndexChanged(ui->comboBox_RandEn->currentIndex());
    }
}

//设置输入格式
void SS2BBE_TMParaSetGUIDlg::InitInput()
{
    //正整数
    QRegExp regExp("[0-9]{1,16}");
    QRegExp regExp0("[0-9A-Fa-f]{1,2}");
    //数据帧标志码
    ui->lineEdit_FrameDataFlag->setValidator(new QRegExpValidator(regExp0, this));
    //码率
    QRegExp regExp_CodeRate("[0-9]{2,5}");
    ui->lineEdit_CodeRate->setValidator(new QRegExpValidator(regExp_CodeRate, this));
    //交织深度
    QRegExp regExp_RSDecodeDeep("[1-5]");
    ui->lineEdit_RSDecodeDeep->setValidator(new QRegExpValidator(regExp_RSDecodeDeep, this));
    //帧长
    QRegExp regExp_FrameLen("[0-9]{0,4}");
    ui->lineEdit_FrameLen->setValidator(new QRegExpValidator(regExp_FrameLen, this));
    //字长
     QRegExp regExp_WordLen("[0-9]{0,2}");
    ui->lineEdit_WordLen->setValidator(new QRegExpValidator(regExp_WordLen, this));
    //格式（副帧）长
    QRegExp regExp_SubFrameLen("[0-9]{0,3}");
    ui->lineEdit_SubFrameLen->setValidator(new QRegExpValidator(regExp_SubFrameLen, this));
    //副帧码组位置
    ui->lineEdit_SubFrmSynGroupPos->setValidator(new QRegExpValidator(regExp_FrameLen, this));
    //ID计数位置
    ui->lineEdit_idCountPos->setValidator(new QRegExpValidator(regExp_FrameLen, this));
    //卫星载波锁定波道
    //    ui->lineEdit_SatelliteLock->setValidator(new QRegExpValidator(regExp, this));
    //    //卫星载波锁定位
    //    ui->lineEdit_SatelliteLockWave->setValidator(new QRegExpValidator(regExp, this));
    //    //卫星长码锁定波道
    //    ui->lineEdit_SatelliteLongCode->setValidator(new QRegExpValidator(regExp, this));
    //    //卫星载波锁定位
    //    ui->lineEdit_SatelliteLongCodeWave->setValidator(new QRegExpValidator(regExp, this));
    //    //卫星短码锁定波道
    //    ui->lineEdit_SatelliteShortLock->setValidator(new QRegExpValidator(regExp, this));
    //    //卫星短码锁定位
    //    ui->lineEdit_SatelliteShortLockWave->setValidator(new QRegExpValidator(regExp, this));

    //字母数字
    QRegExp regExp2("[A-Fa-f0-9]{1,8}");
    //帧同步码组[HEX]
    ui->lineEdit_FrmSyncGroup->setValidator(new QRegExpValidator(regExp2, this));
    //解扰多项式[HEX]
    ui->lineEdit_RandPoly->setValidator(new QRegExpValidator(regExp2, this));
    //解扰初相[HEX]
    ui->lineEdit_RandPhase->setValidator(new QRegExpValidator(regExp2, this));
    //副帧同步码组
    ui->lineEdit_SubFrmGroup->setValidator(new QRegExpValidator(regExp2, this));
}

void SS2BBE_TMParaSetGUIDlg::ShowControls()
{
    //向下拉框控件中写入从xml中读取的内容
    showTargetParam(0);
    //默认选择目标1
    ui->radioButton_Target1->setChecked(true);

}

//显示参数
void SS2BBE_TMParaSetGUIDlg::ShowPara(int type)
{
    if(type == 0)       //参数设置
    {
        if(m_MCSSSTMStatus != NULL)
        {
            for (int i = 0; i < MAX_TARGET_COUNT; i++)
                m_MCSSSTMParam.SSTMParam[i] = m_MCSSSTMStatus->MCSSSB2TMStatu[i].tParams;
        }
        else
        {
            DWORD stLen = sizeof(SMCU2::MCSSSB2TMStatusStruct4);
            if(m_pStateMgr != NULL)
                m_MCSSSTMStatus  = (SMCU2::MCSSSB2TMStatusStruct4*)m_pStateMgr->FindOneItem(m_StatusParaMCSSSTMID, stLen);
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

    //    if (m_MCSSSTMStatus == NULL)
    //        return;
    showTargetParam(m_nTargetNum, 1);
}

//设置帧长范围
void SS2BBE_TMParaSetGUIDlg::SetFrameLenRange(int iWordLen)
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

void SS2BBE_TMParaSetGUIDlg::SetObjInfo(TLxTsspObjectInfo ObjectInfo)
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
        /*************************框架**********************************/
        m_ParaMCSSSTMID = GetID(sItem.usStationID,
                                sItem.ucSubSysID, sItem.usDeviceID,sItem.usTID,
                                sItem.ucSN);
        /*************************框架**********************************/

        /*************************状态**********************************/
        m_StatusParaMCSSSTMID = GetID(sItem.usStationID,
                                      sItem.ucSubSysID, sItem.usDeviceID,sItem.usStateType,
                                      sItem.ucSN);
        /*************************状态**********************************/
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
void SS2BBE_TMParaSetGUIDlg::EnabledControls(bool bFlag)
{
    /*************************Group1*************************/
    //帧同步码组位置
    ui->comboBox_FrameSyncCodePos->setEnabled(bFlag);
    //码型
    ui->comboBox_CodeType->setEnabled(bFlag);
    //译码方式
    ui->comboBox_DecodeType->setEnabled(bFlag);
    //码速率
    ui->lineEdit_CodeRate->setEnabled(bFlag);
    //Viterbi编码
    ui->comboBox_ViterbType->setEnabled(bFlag);
    //RS码类型
    ui->comboBox_RSType->setEnabled(bFlag);
    //RS码坐标
    ui->comboBox_RSDecodePlot->setEnabled(bFlag);
    //RS译码交织深度
    ui->lineEdit_RSDecodeDeep->setEnabled(bFlag);
    //维特比译码输出相位
    ui->comboBox_ViterbiPhase->setEnabled(bFlag);
    //帧数据标志
    ui->lineEdit_FrameDataFlag->setEnabled(bFlag);
    //格式长
    ui->lineEdit_SubFrameLen->setEnabled(bFlag);
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
    //解扰使能
    ui->comboBox_RandEn->setEnabled(bFlag);
    //解扰多项式长
    ui->comboBox_RandPolyLen->setEnabled(bFlag);
    //解扰多项式
    ui->lineEdit_RandPoly->setEnabled(bFlag);
    //解扰初相
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
    ui->lineEdit_idCountPos->setEnabled(bFlag);

    /*************************Group2*************************/

    /*************************Group3*************************/
    //帧同步容错搜索容错位数
    ui->comboBox_FrameSyncFaultTolerant->setEnabled(bFlag);
    //帧同步容错校核容错位数
    ui->comboBox_FaltToleranceOfFrameSync->setEnabled(bFlag);
    //帧同步容错锁定容错位数
    ui->comboBox_FrameSyncFaultTolerantLockBit->setEnabled(bFlag);
    //帧同步容错校核检测帧数
    ui->comboBox_CheckFrameOfFaultFrameSync->setEnabled(bFlag);
    //帧同步容错锁定检测帧数
    ui->comboBox_FrameSyncFaultLockDetection->setEnabled(bFlag);
    //副帧同步容错搜索容错位数
    ui->comboBox_SubFrameSyncFaultTolerant->setEnabled(bFlag);
    //副帧同步容错校核容错位数
    ui->comboBox_SubFaltToleranceOfFrameSync->setEnabled(bFlag);
    //副帧同步容错锁定容错位数
    ui->comboBox_SubFrameSyncFaultTolerantLockBit->setEnabled(bFlag);
    //副帧同步容错校核检测帧数
    ui->comboBox_SubCheckFrameOfFaultFrameSync->setEnabled(bFlag);
    //副帧同步容错锁定检测帧数
    ui->comboBox_SubFrameSyncFaultLockDetection->setEnabled(bFlag);
    //    //卫星载波锁定波道
    //    ui->lineEdit_SatelliteLockWave->setEnabled(bFlag);
    //    //卫星载波锁定
    //    ui->lineEdit_SatelliteLock->setEnabled(bFlag);
    //    //卫星长码锁定波道
    //    ui->lineEdit_SatelliteLongCodeWave->setEnabled(bFlag);
    //    //卫星长码锁定
    //    ui->lineEdit_SatelliteLongCode->setEnabled(bFlag);
    //    //卫星短码锁定波道
    //    ui->lineEdit_SatelliteShortLockWave->setEnabled(bFlag);
    //    //卫星短码锁定
    //    ui->lineEdit_SatelliteShortLock->setEnabled(bFlag);
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

//保存参数宏
int SS2BBE_TMParaSetGUIDlg::SaveToMacro(ILxTsspParameterMacro* pMacroObj)
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
        byteArray.resize(sizeof(m_MCSSSTMParam));
        memcpy(byteArray.data(),&m_MCSSSTMParam,sizeof(m_MCSSSTMParam));
        pMacroObj->UpdateParameterBlock(m_ParaMCSSSTMID,byteArray);

        //日志信息
        TLxTsspLog tmpLog;
        QString strLog;
        tmpLog.ucWarningLevel  = 1;
        strLog += QString("%1: 保存宏成功！").arg(MODULENAME);
        CLog::addLog(strLog, 1);
    }
}
//从界面取参数
void SS2BBE_TMParaSetGUIDlg::GetPara()
{
    getParamTarget();
}

//验证参数范围
int SS2BBE_TMParaSetGUIDlg::CheckPara()
{
    return CheckParam() == TRUE ? 1 : -1;
}

////定时器
void SS2BBE_TMParaSetGUIDlg::timerEvent(QTimerEvent *event)
{
    ShowPara(0);
}



void SS2BBE_TMParaSetGUIDlg::on_pushButton_Set_clicked()
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
            struct DATA{
                USHORT target;
                struct{
                USHORT tmp;
                SMCU2::MCSSSB2TMParamStruct SSTMParam;
                }param[MAX_TARGET_COUNT];
            }data;
            data.target = 0x02 | 0x04 | 0x08 | 0x10;
            for(int i = 0; i < MAX_TARGET_COUNT; ++i)
            {
                data.param[i].SSTMParam = m_MCSSSTMParam.SSTMParam[i];
            }
            QByteArray byteArray;
            byteArray.resize(sizeof(data));
            memcpy(byteArray.data(),&data,sizeof(data));
            SendMessage((char*)&data, sizeof(data), m_usC, m_usTID, 1);
        }

        //日志信息
        TLxTsspLog tmpLog;
        QString strLog;
        tmpLog.ucWarningLevel  = 1;
        strLog += "解调参数设置: 设置参数成功！";
        CLog::addLog(strLog, 1);

        //不使能控件
        EnabledControls(false);
        m_TimerID = startTimer(500);
    }
}

void SS2BBE_TMParaSetGUIDlg::closeEvent(QCloseEvent *event)
{
    event->accept();
}

QSize SS2BBE_TMParaSetGUIDlg::sizeHint() const
{
    return QSize(this->width(), this->height());
}

/*
//副帧方式响应函数
void SS2BBE_TMParaSetGUIDlg::on_comboBox_SubFrmType_currentIndexChanged(int index)
{
//    //副帧方式控件使能
//    if (ui->pushButton_Modify->text() != "更改" || iParaSetFlag != 0)
//    {
//        EnabledControlsSubFrmType(index);   //副帧方式控件使能
//    }
}

//根据副帧类型，使能副帧控件
void SS2BBE_TMParaSetGUIDlg::EnabledControlsSubFrmType(int iSubFrm)
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

void SS2BBE_TMParaSetGUIDlg::SubFrmEn(bool bFlag)
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
void SS2BBE_TMParaSetGUIDlg::IDSubFrmEn(bool bFlag)
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
void SS2BBE_TMParaSetGUIDlg::CycSubFrmEn(bool bFlag)
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
void SS2BBE_TMParaSetGUIDlg::ReserveSubFrmEn(bool bFlag)
{

}

void SS2BBE_TMParaSetGUIDlg::SendMessage(char* pData, USHORT usLen, USHORT usC, USHORT usTID, USHORT usSN)
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

DWORDLONG SS2BBE_TMParaSetGUIDlg::StrToSynCode(const char * pStr)
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

void SS2BBE_TMParaSetGUIDlg::showTargetParam(int nIndex, int nType)
{
    if (nType == 0)
    {
        int i;
        //数据帧标识码
        ui->lineEdit_FrameDataFlag->setText(QString::number(m_DlgParaSetStruct[nIndex][0].InitVal.lval));
        //帧同步码组位置
        for(i=0; i<DlgMapLineNum[nIndex * 24 + 0];i++)
        {
            ui->comboBox_FrameSyncCodePos->addItem(m_DlgMapMean[nIndex * 24 + 0][i]);
        }
        ui->comboBox_FrameSyncCodePos->setCurrentIndex(m_DlgParaSetStruct[nIndex][1].InitVal.lval);
        //码型
        for(i=0; i<DlgMapLineNum[nIndex * 24 + 1];i++)
        {
            ui->comboBox_CodeType->addItem(m_DlgMapMean[nIndex * 24 + 1][i]);
        }
        ui->comboBox_CodeType->setCurrentIndex(m_DlgParaSetStruct[nIndex][2].InitVal.lval);
        //码速率
        ui->lineEdit_CodeRate->setText(QString::number(m_DlgParaSetStruct[nIndex][3].InitVal.lval));
        //译码方式
        for(i=0; i<DlgMapLineNum[nIndex * 24 + 2];i++)
        {
            ui->comboBox_DecodeType->addItem(m_DlgMapMean[nIndex * 24 + 2][i]);
        }
        ui->comboBox_DecodeType->setCurrentIndex(m_DlgParaSetStruct[nIndex][4].InitVal.lval);
        //卷积码类型
        for(i=0; i<DlgMapLineNum[nIndex * 24 + 3];i++)
        {
            ui->comboBox_ViterbType->addItem(m_DlgMapMean[nIndex * 24 + 3][i]);
        }
        ui->comboBox_ViterbType->setCurrentIndex(m_DlgParaSetStruct[nIndex][5].InitVal.lval);
        //Viterbi输出相位
        for(i=0; i<DlgMapLineNum[nIndex * 24 + 4];i++)
        {
            ui->comboBox_ViterbiPhase->addItem(m_DlgMapMean[nIndex * 24 + 4][i]);
        }
        ui->comboBox_ViterbiPhase->setCurrentIndex(m_DlgParaSetStruct[nIndex][6].InitVal.lval);
        //RS码类型
        for(i=0; i<DlgMapLineNum[nIndex * 24 + 5];i++)
        {
            ui->comboBox_RSType->addItem(m_DlgMapMean[nIndex * 24 + 5][i]);
        }
        ui->comboBox_RSType->setCurrentIndex(m_DlgParaSetStruct[nIndex][7].InitVal.lval);
        //rs译码坐标基
        for(i=0; i<DlgMapLineNum[nIndex * 24 + 6];i++)
        {
            ui->comboBox_RSDecodePlot->addItem(m_DlgMapMean[nIndex * 24 + 6][i]);
        }
        ui->comboBox_RSDecodePlot->setCurrentIndex(m_DlgParaSetStruct[nIndex][8].InitVal.lval);
        //交织深度
        ui->lineEdit_RSDecodeDeep->setText(QString::number(m_DlgParaSetStruct[nIndex][9].InitVal.lval));
        ui->label_RSDecodeDeep->setText("交织深度\n[" + QString::number(m_DlgParaSetStruct[nIndex][9].MinVal.lmin) +
                "~" + QString::number(m_DlgParaSetStruct[nIndex][9].MaxVal.lmax) + "]");
        //帧长
        ui->lineEdit_FrameLen->setText(QString::number(m_DlgParaSetStruct[nIndex][10].InitVal.lval));
        //字长
        ui->lineEdit_WordLen->setText(QString::number(m_DlgParaSetStruct[nIndex][11].InitVal.lval));
        //格式长
        ui->lineEdit_SubFrameLen->setText(QString::number(m_DlgParaSetStruct[nIndex][12].InitVal.lval));
        //帧同步码组长
        for(i=0; i<DlgMapLineNum[nIndex * 24 + 7];i++)
        {
            ui->comboBox_FrameSyncGroupLen->addItem(m_DlgMapMean[nIndex * 24 + 7][i]);
        }
        ui->comboBox_FrameSyncGroupLen->setCurrentIndex(m_DlgParaSetStruct[nIndex][13].InitVal.lval);
        //帧同步码组
        ui->lineEdit_FrmSyncGroup->setText(QString::number(m_DlgParaSetStruct[nIndex][14].InitVal.lval,16).toUpper());
        //解扰使能
        for(i=0; i<DlgMapLineNum[nIndex * 24 + 8];i++)
        {
            ui->comboBox_RandEn->addItem(m_DlgMapMean[nIndex * 24 + 8][i]);
        }
        ui->comboBox_RandEn->setCurrentIndex(m_DlgParaSetStruct[nIndex][15].InitVal.lval);
        //解扰多项式长度
        for(i=0; i<DlgMapLineNum[nIndex * 24 + 9];i++)
        {
            ui->comboBox_RandPolyLen->addItem(m_DlgMapMean[nIndex * 24 + 9][i]);
        }
        ui->comboBox_RandPolyLen->setCurrentIndex(m_DlgParaSetStruct[nIndex][16].InitVal.lval);
        //解扰多项式
        ui->lineEdit_RandPoly->setText(QString::number(m_DlgParaSetStruct[nIndex][17].InitVal.lval,16).toUpper());
        //解扰初相
        ui->lineEdit_RandPhase->setText(QString::number(m_DlgParaSetStruct[nIndex][18].InitVal.lval,16).toUpper());
        //副帧同步方式
        for(i=0; i<DlgMapLineNum[nIndex * 24 + 10];i++)
        {
            ui->comboBox_SubFrmSyncType->addItem(m_DlgMapMean[nIndex * 24 + 10][i]);
        }
        ui->comboBox_SubFrmSyncType->setCurrentIndex(m_DlgParaSetStruct[nIndex][19].InitVal.lval);
        //副帧码组长度
        for(i=0; i<DlgMapLineNum[nIndex * 24 + 11];i++)
        {
            ui->comboBox_SubFrameGroupLen->addItem(m_DlgMapMean[nIndex * 24 + 11][i]);
        }
        ui->comboBox_SubFrameGroupLen->setCurrentIndex(m_DlgParaSetStruct[nIndex][20].InitVal.lval);
        //副帧码组
        ui->lineEdit_SubFrmGroup->setText(QString::number(m_DlgParaSetStruct[nIndex][22].InitVal.lval,16).toUpper());
        //副帧码组位置
        ui->lineEdit_SubFrmSynGroupPos->setText(QString::number(m_DlgParaSetStruct[nIndex][21].InitVal.lval));
        //ID计数位置
        ui->lineEdit_idCountPos->setText(QString::number(m_DlgParaSetStruct[nIndex][23].InitVal.lval));
        //ID字基值
        for(i=0; i<DlgMapLineNum[nIndex * 24 + 12];i++)
        {
            ui->comboBox_IDBaseValue->addItem(m_DlgMapMean[nIndex * 24 + 12][i]);
        }
        ui->comboBox_IDBaseValue->setCurrentIndex(m_DlgParaSetStruct[nIndex][24].InitVal.lval);
        //ID计数方向
        for(i=0; i<DlgMapLineNum[nIndex * 24 + 13];i++)
        {
            ui->comboBox_idDirect->addItem(m_DlgMapMean[nIndex * 24 + 13][i]);
        }
        ui->comboBox_idDirect->setCurrentIndex(m_DlgParaSetStruct[nIndex][25].InitVal.lval);
        //帧同步容错搜索容错位数
        for(i=0; i<DlgMapLineNum[nIndex * 24 + 14];i++)
        {
            ui->comboBox_FrameSyncFaultTolerant->addItem(m_DlgMapMean[nIndex * 24 + 14][i]);
        }
        ui->comboBox_FrameSyncFaultTolerant->setCurrentIndex(m_DlgParaSetStruct[nIndex][26].InitVal.lval);
        //帧同步容错校核容错位数
        for(i=0; i<DlgMapLineNum[nIndex * 24 + 15];i++)
        {
            ui->comboBox_FaltToleranceOfFrameSync->addItem(m_DlgMapMean[nIndex * 24 + 15][i]);
        }
        ui->comboBox_FaltToleranceOfFrameSync->setCurrentIndex(m_DlgParaSetStruct[nIndex][27].InitVal.lval);
        //帧同步锁定
        for(i=0; i<DlgMapLineNum[nIndex * 24 + 16];i++)
        {
            ui->comboBox_FrameSyncFaultTolerantLockBit->addItem(m_DlgMapMean[nIndex * 24 + 16][i]);
        }
        ui->comboBox_FrameSyncFaultTolerantLockBit->setCurrentIndex(m_DlgParaSetStruct[nIndex][28].InitVal.lval);
        //帧同步容错校核检测帧数
        for(i=0; i<DlgMapLineNum[nIndex * 24 + 17];i++)
        {
            ui->comboBox_CheckFrameOfFaultFrameSync->addItem(m_DlgMapMean[nIndex * 24 + 17][i]);
        }
        ui->comboBox_CheckFrameOfFaultFrameSync->setCurrentIndex(m_DlgParaSetStruct[nIndex][29].InitVal.lval);
        //帧同步容错锁定检测帧数
        for(i=0; i<DlgMapLineNum[nIndex * 24 + 18];i++)
        {
            ui->comboBox_FrameSyncFaultLockDetection->addItem(m_DlgMapMean[nIndex * 24 + 18][i]);
        }
        ui->comboBox_FrameSyncFaultLockDetection->setCurrentIndex(m_DlgParaSetStruct[nIndex][30].InitVal.lval);
        //副帧同步容错校核容错位数
        for(i=0; i<DlgMapLineNum[nIndex * 24 + 19];i++)
        {
            ui->comboBox_SubFrameSyncFaultTolerant->addItem(m_DlgMapMean[nIndex * 24 + 19][i]);
        }
        ui->comboBox_SubFrameSyncFaultTolerant->setCurrentIndex(m_DlgParaSetStruct[nIndex][31].InitVal.lval);
        //副帧同步容错校核容错位数
        for(i=0; i<DlgMapLineNum[nIndex * 24 + 20];i++)
        {
            ui->comboBox_SubFaltToleranceOfFrameSync->addItem(m_DlgMapMean[nIndex * 24 + 20][i]);
        }
        ui->comboBox_SubFaltToleranceOfFrameSync->setCurrentIndex(m_DlgParaSetStruct[nIndex][32].InitVal.lval);
        //副帧同步容错锁定容错位数
        for(i=0; i<DlgMapLineNum[nIndex * 24 + 21];i++)
        {
            ui->comboBox_SubFrameSyncFaultTolerantLockBit->addItem(m_DlgMapMean[nIndex * 24 + 21][i]);
        }
        ui->comboBox_SubFrameSyncFaultTolerantLockBit->setCurrentIndex(m_DlgParaSetStruct[nIndex][33].InitVal.lval);
        //副帧校核帧数
        for(i=0; i<DlgMapLineNum[nIndex * 24 + 22];i++)
        {
            ui->comboBox_SubCheckFrameOfFaultFrameSync->addItem(m_DlgMapMean[nIndex * 24 + 22][i]);
        }
        ui->comboBox_SubCheckFrameOfFaultFrameSync->setCurrentIndex(m_DlgParaSetStruct[nIndex][34].InitVal.lval);
        //副帧锁定帧数
        for(i=0; i<DlgMapLineNum[nIndex * 24 + 23];i++)
        {
            ui->comboBox_SubFrameSyncFaultLockDetection->addItem(m_DlgMapMean[nIndex * 24 + 23][i]);
        }
        ui->comboBox_SubFrameSyncFaultLockDetection->setCurrentIndex(m_DlgParaSetStruct[nIndex][35].InitVal.lval);

        //        //卫星载波锁定波道
        //        ui->lineEdit_SatelliteLockWave->setText(QString::number(m_DlgParaSetStruct[nIndex][36].InitVal.lval));
        //        //卫星载波锁定
        //        ui->lineEdit_SatelliteLock->setText(QString::number(m_DlgParaSetStruct[nIndex][37].InitVal.lval));
        //        //卫星长码锁定波道
        //        ui->lineEdit_SatelliteLongCodeWave->setText(QString::number(m_DlgParaSetStruct[nIndex][38].InitVal.lval));
        //        //卫星长码锁定
        //        ui->lineEdit_SatelliteLongCode->setText(QString::number(m_DlgParaSetStruct[nIndex][39].InitVal.lval));
        //        //卫星短码锁定波道
        //        ui->lineEdit_SatelliteShortLockWave->setText(QString::number(m_DlgParaSetStruct[nIndex][40].InitVal.lval));
        //        //卫星短码锁定
        //        ui->lineEdit_SatelliteShortLock->setText(QString::number(m_DlgParaSetStruct[nIndex][41].InitVal.lval));
    }
    else if (nType == 1)
    {
        //数据帧标志码
        ui->lineEdit_FrameDataFlag->setText(QString::number(m_MCSSSTMParam.SSTMParam[nIndex].ucDataCode, 16));
        //帧同步码组位置
        if (m_MCSSSTMParam.SSTMParam[nIndex].ucFspPlace > 0)
            ui->comboBox_FrameSyncCodePos->setCurrentIndex(m_MCSSSTMParam.SSTMParam[nIndex].ucFspPlace - 1);
        //码型
        if (m_MCSSSTMParam.SSTMParam[nIndex].uiCodeStyle > 0)
            ui->comboBox_CodeType->setCurrentIndex(m_MCSSSTMParam.SSTMParam[nIndex].uiCodeStyle - 1);
        //码速率
        ui->lineEdit_CodeRate->setText(QString::number(m_MCSSSTMParam.SSTMParam[nIndex].uiTM_CodeRate));
        //译码方式
        if (m_MCSSSTMParam.SSTMParam[nIndex].ucCodingMd > 0)
            ui->comboBox_DecodeType->setCurrentIndex(m_MCSSSTMParam.SSTMParam[nIndex].ucCodingMd - 1);
        //卷积码类型
        if (m_MCSSSTMParam.SSTMParam[nIndex].ucViterbiMd > 0)
            ui->comboBox_ViterbType->setCurrentIndex(m_MCSSSTMParam.SSTMParam[nIndex].ucViterbiMd - 1);
        //Viterbi译码输出相位
        if (m_MCSSSTMParam.SSTMParam[nIndex].ucG2Inv > 0)
            ui->comboBox_ViterbiPhase->setCurrentIndex(m_MCSSSTMParam.SSTMParam[nIndex].ucG2Inv - 1);
        //R-S码类型
        if (m_MCSSSTMParam.SSTMParam[nIndex].ucRSMd > 0)
            ui->comboBox_RSType->setCurrentIndex(m_MCSSSTMParam.SSTMParam[nIndex].ucRSMd - 1);
        //R-S译码坐标基
        if (m_MCSSSTMParam.SSTMParam[nIndex].ucBasiSel > 0)
            ui->comboBox_RSDecodePlot->setCurrentIndex(m_MCSSSTMParam.SSTMParam[nIndex].ucBasiSel - 1);
        //交织深度
        if (m_MCSSSTMParam.SSTMParam[nIndex].ucInterleav > 0 && m_MCSSSTMParam.SSTMParam[nIndex].ucInterleav < 6)
            ui->lineEdit_RSDecodeDeep->setText(QString::number(m_MCSSSTMParam.SSTMParam[nIndex].ucInterleav));
        else
            ui->lineEdit_RSDecodeDeep->setText("1");
        //帧长
        ui->lineEdit_FrameLen->setText(QString::number(m_MCSSSTMParam.SSTMParam[nIndex].usFrameLen));
        //帧同步码组长
        if (m_MCSSSTMParam.SSTMParam[nIndex].ucFrameSynGLen > 0)
            ui->comboBox_FrameSyncGroupLen->setCurrentIndex(m_MCSSSTMParam.SSTMParam[nIndex].ucFrameSynGLen - 1);
        //帧同步码组
        ui->lineEdit_FrmSyncGroup->setText(QString::number(m_MCSSSTMParam.SSTMParam[nIndex].uiFrameSynG, 16).toUpper());
        //字长
        ui->lineEdit_WordLen->setText(QString::number(m_MCSSSTMParam.SSTMParam[nIndex].ucWordLen));
        //格式（副帧）长
        ui->lineEdit_SubFrameLen->setText(QString::number(m_MCSSSTMParam.SSTMParam[nIndex].ucSframeLen));
        //解扰控制
        if (m_MCSSSTMParam.SSTMParam[nIndex].ucUnturbCtrl > 0)
            ui->comboBox_RandEn->setCurrentIndex(m_MCSSSTMParam.SSTMParam[nIndex].ucUnturbCtrl - 1);
        //解扰多项式长度
        if (m_MCSSSTMParam.SSTMParam[nIndex].ucRandLen > 0)
            ui->comboBox_RandPolyLen->setCurrentIndex(m_MCSSSTMParam.SSTMParam[nIndex].ucRandLen - 1);
        //解扰多项式
        ui->lineEdit_RandPoly->setText(QString::number(m_MCSSSTMParam.SSTMParam[nIndex].uiRandPoly, 16).toUpper());
        //解扰初相
        ui->lineEdit_RandPhase->setText(QString::number(m_MCSSSTMParam.SSTMParam[nIndex].uiPhase, 16).toUpper());
        //副帧同步方式
        if (m_MCSSSTMParam.SSTMParam[nIndex].ucSFrameSynMd > 0)
            ui->comboBox_SubFrmSyncType->setCurrentIndex(m_MCSSSTMParam.SSTMParam[nIndex].ucSFrameSynMd - 1);
        //副帧码组长度
        if (m_MCSSSTMParam.SSTMParam[nIndex].ucCycFrmSynCodeLen > 0)
            ui->comboBox_SubFrameGroupLen->setCurrentIndex(m_MCSSSTMParam.SSTMParam[nIndex].ucCycFrmSynCodeLen - 1);
        //副帧码组位置
        ui->lineEdit_SubFrmSynGroupPos->setText(QString::number(m_MCSSSTMParam.SSTMParam[nIndex].usCycFrmSynCodePos));
        //副帧同步码组
        ui->lineEdit_SubFrmGroup->setText(QString::number(m_MCSSSTMParam.SSTMParam[nIndex].uiCycFrmSynCode, 16).toUpper());
        //ID计数位置
        ui->lineEdit_idCountPos->setText(QString::number(m_MCSSSTMParam.SSTMParam[nIndex].usIDSubFrmPos));
        //ID字基值
        if (m_MCSSSTMParam.SSTMParam[nIndex].ucIDBase > 0)
            ui->comboBox_IDBaseValue->setCurrentIndex(m_MCSSSTMParam.SSTMParam[nIndex].ucIDBase - 1);
        //ID计数方向
        if (m_MCSSSTMParam.SSTMParam[nIndex].ucIDDirect != 0)
            ui->comboBox_idDirect->setCurrentIndex(m_MCSSSTMParam.SSTMParam[nIndex].ucIDDirect - 1);
        //帧同步容错搜索容错位数
        ui->comboBox_FrameSyncFaultTolerant->setCurrentIndex(m_MCSSSTMParam.SSTMParam[nIndex].ucFrSynSerLiErr);
        //帧同步容错校核容错位数
        ui->comboBox_FaltToleranceOfFrameSync->setCurrentIndex(m_MCSSSTMParam.SSTMParam[nIndex].ucFrSynVerLiErr);
        //帧同步容错锁定容错位数
        ui->comboBox_FrameSyncFaultTolerantLockBit->setCurrentIndex(m_MCSSSTMParam.SSTMParam[nIndex].ucFrSynLoLiErr);
        //帧同步容错校核检测帧数
        ui->comboBox_CheckFrameOfFaultFrameSync->setCurrentIndex(m_MCSSSTMParam.SSTMParam[nIndex].ucFrSynVerCheck - 1);
        //帧同步容错锁定检测帧数
        ui->comboBox_FrameSyncFaultLockDetection->setCurrentIndex(m_MCSSSTMParam.SSTMParam[nIndex].ucFrSynLoCheck - 1);
        //副帧同步容错搜索容错位数
        ui->comboBox_SubFrameSyncFaultTolerant->setCurrentIndex(m_MCSSSTMParam.SSTMParam[nIndex].ucCFSearchErrBits);
        //副帧同步容错校核容错位数
        ui->comboBox_SubFaltToleranceOfFrameSync->setCurrentIndex(m_MCSSSTMParam.SSTMParam[nIndex].ucCFVerifyErrBits);
        //副帧同步容错锁定容错位数
        ui->comboBox_SubFrameSyncFaultTolerantLockBit->setCurrentIndex(m_MCSSSTMParam.SSTMParam[nIndex].ucCFLockErrBits);
        //副帧同步容错校核检测帧数
        ui->comboBox_SubCheckFrameOfFaultFrameSync->setCurrentIndex(m_MCSSSTMParam.SSTMParam[nIndex].ucSubFVerifyFrame -1);
        //副帧同步容错锁定检测帧数
        ui->comboBox_SubFrameSyncFaultLockDetection->setCurrentIndex(m_MCSSSTMParam.SSTMParam[nIndex].ucSubFLockFrame - 1);
        //        //卫星载波锁定波道
        //        ui->lineEdit_SatelliteLockWave->setText(QString::number(m_MCSSSTMParam.SSTMParam[nIndex].usSatCarrLockChnl));
        //        //卫星载波锁定
        //        ui->lineEdit_SatelliteLock->setText(QString::number(m_MCSSSTMParam.SSTMParam[nIndex].ucSatCarrLockBit));
        //        //卫星长码锁定波道
        //        ui->lineEdit_SatelliteLongCodeWave->setText(QString::number(m_MCSSSTMParam.SSTMParam[nIndex].usSatLPNLockChnl));
        //        //卫星长码锁定
        //        ui->lineEdit_SatelliteLongCode->setText(QString::number(m_MCSSSTMParam.SSTMParam[nIndex].ucSatLPNLockBit));
        //        //卫星短码锁定波道
        //        ui->lineEdit_SatelliteShortLockWave->setText(QString::number(m_MCSSSTMParam.SSTMParam[nIndex].usSatSPNLockChnl));
        //        //卫星短码锁定
        //        ui->lineEdit_SatelliteShortLock->setText(QString::number(m_MCSSSTMParam.SSTMParam[nIndex].ucSatSPNLockBit));
    }
}

int SS2BBE_TMParaSetGUIDlg::getSharedMemoryTargetNum()
{
    DWORD stLen = sizeof(SMCU2::MCSSysTarget4StatusStruct);
    if(m_pStateMgr != NULL)
        m_pMCSTargetSysStatus  = (SMCU2::MCSSysTarget4StatusStruct*)m_pStateMgr->
                FindOneItem(m_CommTargetID, stLen);

    if (m_pMCSTargetSysStatus != NULL)
        return m_pMCSTargetSysStatus->MCSSysStatu.tParams.ucTargetNum;

    return 0;
}

void SS2BBE_TMParaSetGUIDlg::showRadioButton()
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
    switch(nNum)
    {
    case 1:
        ui->radioButton_Target1->show();
        break;
    case 2:
        ui->radioButton_Target1->show();
        ui->radioButton_Target2->show();
        break;
    case 3:
        ui->radioButton_Target1->show();
        ui->radioButton_Target2->show();
        ui->radioButton_Target3->show();
        break;
    case 4:
        ui->radioButton_Target1->show();
        ui->radioButton_Target2->show();
        ui->radioButton_Target3->show();
        ui->radioButton_Target4->show();
        break;
    default:
        break;
    }
}

void SS2BBE_TMParaSetGUIDlg::getParamTarget()
{
    bool ok;
    //帧同步码组位置
    m_MCSSSTMParam.SSTMParam[m_nTargetNum].ucFspPlace = ui->comboBox_FrameSyncCodePos->currentIndex() + 1;
    //码型
    m_MCSSSTMParam.SSTMParam[m_nTargetNum].uiCodeStyle = ui->comboBox_CodeType->currentIndex() + 1;
    //译码方式
    m_MCSSSTMParam.SSTMParam[m_nTargetNum].ucCodingMd = ui->comboBox_DecodeType->currentIndex() + 1;
    //码速率
    m_MCSSSTMParam.SSTMParam[m_nTargetNum].uiTM_CodeRate = ui->lineEdit_CodeRate->text().toInt();
    //Viterbi编码
    m_MCSSSTMParam.SSTMParam[m_nTargetNum].ucViterbiMd = ui->comboBox_ViterbType->currentIndex() + 1;
    //RS码类型
    m_MCSSSTMParam.SSTMParam[m_nTargetNum].ucRSMd = ui->comboBox_RSType->currentIndex() + 1;
    //RS码坐标
    m_MCSSSTMParam.SSTMParam[m_nTargetNum].ucBasiSel = ui->comboBox_RSDecodePlot->currentIndex() + 1;
    //RS译码交织深度
    m_MCSSSTMParam.SSTMParam[m_nTargetNum].ucInterleav = ui->lineEdit_RSDecodeDeep->text().toInt();
    //维特比译码输出相位
    m_MCSSSTMParam.SSTMParam[m_nTargetNum].ucG2Inv = ui->comboBox_ViterbiPhase->currentIndex() + 1;
    //  格式长
    m_MCSSSTMParam.SSTMParam[m_nTargetNum].ucSframeLen = ui->lineEdit_SubFrameLen->text().toInt();

    //帧长
    m_MCSSSTMParam.SSTMParam[m_nTargetNum].usFrameLen = ui->lineEdit_FrameLen->text().toInt();
    //帧同步码组长
    m_MCSSSTMParam.SSTMParam[m_nTargetNum].ucFrameSynGLen = ui->comboBox_FrameSyncGroupLen->currentIndex() + 1;
    //帧同步码组
    char* pTem1 = ui->lineEdit_FrmSyncGroup->text().trimmed().toLocal8Bit().data();
    m_MCSSSTMParam.SSTMParam[m_nTargetNum].uiFrameSynG = StrToSynCode(pTem1);
    //字长
    m_MCSSSTMParam.SSTMParam[m_nTargetNum].ucWordLen = ui->lineEdit_WordLen->text().toInt();
    //解扰使能
    m_MCSSSTMParam.SSTMParam[m_nTargetNum].ucUnturbCtrl = ui->comboBox_RandEn->currentIndex() + 1;
    //解扰多项式长
    m_MCSSSTMParam.SSTMParam[m_nTargetNum].ucRandLen = ui->comboBox_RandPolyLen->currentIndex() + 1;
    //解扰多项式
    m_MCSSSTMParam.SSTMParam[m_nTargetNum].uiRandPoly = ui->lineEdit_RandPoly->text().toUInt(&ok, 16);
    //解扰初相
    m_MCSSSTMParam.SSTMParam[m_nTargetNum].uiPhase = ui->lineEdit_RandPhase->text().toUInt(&ok, 16);
    //副帧码组长
    m_MCSSSTMParam.SSTMParam[m_nTargetNum].ucCycFrmSynCodeLen = ui->comboBox_SubFrameGroupLen->currentIndex() + 1;
    //副帧码组
    char* pTem2 = ui->lineEdit_SubFrmGroup->text().trimmed().toLocal8Bit().data();
    m_MCSSSTMParam.SSTMParam[m_nTargetNum].uiCycFrmSynCode = StrToSynCode(pTem2);
    //副帧同步方式
    m_MCSSSTMParam.SSTMParam[m_nTargetNum].ucSFrameSynMd = ui->comboBox_SubFrmSyncType->currentIndex() + 1;
    //副帧码组位置
    m_MCSSSTMParam.SSTMParam[m_nTargetNum].usCycFrmSynCodePos = ui->lineEdit_SubFrmSynGroupPos->text().toInt();
    //ID字基值
    m_MCSSSTMParam.SSTMParam[m_nTargetNum].ucIDBase = ui->comboBox_IDBaseValue->currentIndex() + 1;
    //ID计数方向
    m_MCSSSTMParam.SSTMParam[m_nTargetNum].ucIDDirect = ui->comboBox_idDirect->currentIndex() + 1;
    //ID计数位置
    m_MCSSSTMParam.SSTMParam[m_nTargetNum].usIDSubFrmPos = ui->lineEdit_idCountPos->text().toInt();


    //帧同步容错搜索容错位数
    m_MCSSSTMParam.SSTMParam[m_nTargetNum].ucFrSynSerLiErr = ui->comboBox_FrameSyncFaultTolerant->currentIndex();
    //帧同步容错校核容错位数
    m_MCSSSTMParam.SSTMParam[m_nTargetNum].ucFrSynVerLiErr = ui->comboBox_FaltToleranceOfFrameSync->currentIndex();
    //帧同步容错锁定容错位数
    m_MCSSSTMParam.SSTMParam[m_nTargetNum].ucFrSynLoLiErr = ui->comboBox_FrameSyncFaultTolerantLockBit->currentIndex();
    //帧同步容错校核检测帧数
    m_MCSSSTMParam.SSTMParam[m_nTargetNum].ucFrSynVerCheck = ui->comboBox_CheckFrameOfFaultFrameSync->currentIndex() + 1;
    //帧同步容错锁定检测帧数
    m_MCSSSTMParam.SSTMParam[m_nTargetNum].ucFrSynLoCheck = ui->comboBox_FrameSyncFaultLockDetection->currentIndex() + 1;
    //副帧同步容错搜索容错位数
    m_MCSSSTMParam.SSTMParam[m_nTargetNum].ucCFSearchErrBits = ui->comboBox_SubFrameSyncFaultTolerant->currentIndex();
    //副帧同步容错校核容错位数
    m_MCSSSTMParam.SSTMParam[m_nTargetNum].ucCFVerifyErrBits = ui->comboBox_SubFaltToleranceOfFrameSync->currentIndex();
    //副帧同步容错校核容错位数
    m_MCSSSTMParam.SSTMParam[m_nTargetNum].ucCFLockErrBits = ui->comboBox_SubFrameSyncFaultTolerantLockBit->currentIndex();
    //副帧同步容错校核检测帧数
    m_MCSSSTMParam.SSTMParam[m_nTargetNum].ucSubFVerifyFrame = ui->comboBox_SubCheckFrameOfFaultFrameSync->currentIndex() + 1;
    //副帧同步容错锁定检测帧数
    m_MCSSSTMParam.SSTMParam[m_nTargetNum].ucSubFLockFrame = ui->comboBox_SubFrameSyncFaultLockDetection->currentIndex() + 1;
    //数据帧标志码
     m_MCSSSTMParam.SSTMParam[m_nTargetNum].ucDataCode = ui->lineEdit_FrameDataFlag->text().toInt(&ok, 16);

    //    //卫星载波锁定波道
    //    m_MCSSSTMParam.SSTMParam[m_nTargetNum].usSatCarrLockChnl = ui->lineEdit_SatelliteLockWave->text().toInt();
    //    //卫星载波锁定波道
    //    m_MCSSSTMParam.SSTMParam[m_nTargetNum].ucSatCarrLockBit = ui->lineEdit_SatelliteLock->text().toInt();
    //    //卫星载波锁定波道
    //    m_MCSSSTMParam.SSTMParam[m_nTargetNum].usSatLPNLockChnl = ui->lineEdit_SatelliteLongCodeWave->text().toInt();
    //    //卫星载波锁定波道
    //    m_MCSSSTMParam.SSTMParam[m_nTargetNum].ucSatLPNLockBit = ui->lineEdit_SatelliteLongCode->text().toInt();
    //    //卫星载波锁定波道
    //    m_MCSSSTMParam.SSTMParam[m_nTargetNum].usSatSPNLockChnl = ui->lineEdit_SatelliteShortLockWave->text().toInt();
    //    //卫星载波锁定波道
    //    m_MCSSSTMParam.SSTMParam[m_nTargetNum].ucSatSPNLockBit = ui->lineEdit_SatelliteShortLock->text().toInt();
}

void SS2BBE_TMParaSetGUIDlg::set_radioButton_Target()
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

void SS2BBE_TMParaSetGUIDlg::on_radioButton_Target1_clicked()
{
    if(1 != CheckPara())
    {
        set_radioButton_Target();
        return;
    }
    GetPara();
    m_nTargetNum = 0;
    int nType = ui->pushButton_Set->isEnabled() ? 0 : 1;
    showTargetParam(0, 1);
}

void SS2BBE_TMParaSetGUIDlg::on_radioButton_Target2_clicked()
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

void SS2BBE_TMParaSetGUIDlg::on_radioButton_Target3_clicked()
{
    if(1 != CheckPara())
    {
        set_radioButton_Target();
        return;
    }
    GetPara();
    m_nTargetNum = 2;
    int nType = ui->pushButton_Set->isEnabled() ? 0 : 1;
    showTargetParam(2, 1);
}

void SS2BBE_TMParaSetGUIDlg::on_radioButton_Target4_clicked()
{
    if(1 != CheckPara())
    {
        set_radioButton_Target();
        return;
    }
    GetPara();
    m_nTargetNum = 3;
    int nType = ui->pushButton_Set->isEnabled() ? 0 : 1;
    showTargetParam(3, 1);
}


//更改参数按钮休息处理函数
void SS2BBE_TMParaSetGUIDlg::on_pushButton_Modify_clicked()
{
    if(ui->pushButton_Modify->text() == "更改")
    {
        EnabledControls(true);
        killTimer(m_TimerID);
        on_comboBox_DecodeType_currentIndexChanged(ui->comboBox_DecodeType->currentIndex());
        on_comboBox_SubFrmSyncType_currentIndexChanged(ui->comboBox_SubFrmSyncType->currentIndex());
        on_comboBox_ViterbType_currentIndexChanged(ui->comboBox_ViterbType->currentIndex());
        on_comboBox_RandEn_currentIndexChanged(ui->comboBox_RandEn->currentIndex());
        return;
    }

    if(ui->pushButton_Modify->text() == "恢复")
    {
        EnabledControls(false);
        m_TimerID = startTimer(500);
        return;
    }
}
//译码方式
void SS2BBE_TMParaSetGUIDlg::on_comboBox_DecodeType_currentIndexChanged(int index)
{
    if (ui->pushButton_Modify->text() == "更改")
        return;
    if (index == 0){//关
        ui->comboBox_FrameSyncCodePos->setEnabled(true);
        ui->comboBox_ViterbType->setEnabled(false);
        ui->comboBox_RSType->setEnabled(false);
        ui->lineEdit_RSDecodeDeep->setEnabled(false);
        ui->comboBox_ViterbiPhase->setEnabled(false);
        ui->comboBox_RSDecodePlot->setEnabled(false);
        ui->comboBox_SubFrmSyncType->setEnabled(true);
        ui->lineEdit_WordLen->setEnabled(true);
        on_comboBox_SubFrmSyncType_currentIndexChanged(ui->comboBox_SubFrmSyncType->currentIndex());
    }
    if(index == 1){

        ui->comboBox_FrameSyncCodePos->setEnabled(true);
        ui->comboBox_ViterbType->setEnabled(true);
        ui->comboBox_RSType->setEnabled(false);
        ui->lineEdit_RSDecodeDeep->setEnabled(false);
        ui->comboBox_ViterbiPhase->setEnabled(true);
        ui->comboBox_RSDecodePlot->setEnabled(false);
        ui->comboBox_SubFrmSyncType->setEnabled(true);
        ui->lineEdit_WordLen->setEnabled(true);
        on_comboBox_ViterbType_currentIndexChanged(ui->comboBox_ViterbType->currentIndex());
        on_comboBox_SubFrmSyncType_currentIndexChanged(ui->comboBox_SubFrmSyncType->currentIndex());
    }
    if(index==2){
        ui->comboBox_FrameSyncCodePos->setEnabled(false);
        ui->comboBox_FrameSyncCodePos->setCurrentIndex(0);
        ui->comboBox_ViterbType->setEnabled(false);
        ui->comboBox_RSType->setEnabled(true);
        ui->lineEdit_RSDecodeDeep->setEnabled(true);
        ui->comboBox_ViterbiPhase->setEnabled(false);
        ui->comboBox_RSDecodePlot->setEnabled(true);
        ui->comboBox_SubFrmSyncType->setEnabled(false);
        ui->comboBox_SubFrmSyncType->setCurrentIndex(0);
        ui->lineEdit_WordLen->setEnabled(false);
        ui->lineEdit_WordLen->setText("8");
        on_comboBox_SubFrmSyncType_currentIndexChanged(ui->comboBox_SubFrmSyncType->currentIndex());

    }
    if(index==3){
        ui->comboBox_FrameSyncCodePos->setEnabled(false);
        ui->comboBox_FrameSyncCodePos->setCurrentIndex(0);
        ui->comboBox_ViterbType->setEnabled(true);
        ui->comboBox_RSType->setEnabled(true);
        ui->lineEdit_RSDecodeDeep->setEnabled(true);
        ui->comboBox_ViterbiPhase->setEnabled(true);
        ui->comboBox_RSDecodePlot->setEnabled(true);
        ui->comboBox_SubFrmSyncType->setEnabled(false);
        ui->comboBox_SubFrmSyncType->setCurrentIndex(0);
        ui->lineEdit_WordLen->setEnabled(false);
        ui->lineEdit_WordLen->setText("8");
        on_comboBox_ViterbType_currentIndexChanged(ui->comboBox_ViterbType->currentIndex());
        on_comboBox_SubFrmSyncType_currentIndexChanged(ui->comboBox_SubFrmSyncType->currentIndex());
    }

}
//v-译码方式
void SS2BBE_TMParaSetGUIDlg::on_comboBox_ViterbType_currentIndexChanged(int index)
{
    if (ui->pushButton_Modify->text() == "更改")
        return;
    // 1/2
    if(index== 0){
        if(ui->comboBox_DecodeType->currentIndex() == 1 ||
                ui->comboBox_DecodeType->currentIndex() == 3)
            ui->comboBox_ViterbiPhase->setEnabled(true);
    }
    // 3/4
    if(index== 1){
        ui->comboBox_ViterbiPhase->setEnabled(false);
        ui->comboBox_ViterbiPhase->setCurrentIndex(0);

    }

}
//解扰控制
void SS2BBE_TMParaSetGUIDlg::on_comboBox_RandEn_currentIndexChanged(int index)
{
    if (ui->pushButton_Modify->text() == "更改")
        return;
    ui->comboBox_RandPolyLen->setEnabled(false);
    ui->lineEdit_RandPoly->setEnabled(false);
    ui->lineEdit_RandPhase->setEnabled(false);
    if(index==2){//人工
        ui->comboBox_RandPolyLen->setEnabled(true);
        ui->lineEdit_RandPoly->setEnabled(true);
        ui->lineEdit_RandPhase->setEnabled(true);

    }
    else{
        ui->comboBox_RandPolyLen->setEnabled(false);
        ui->lineEdit_RandPoly->setEnabled(false);
        ui->lineEdit_RandPhase->setEnabled(false);
        if (index == 1){
            ui->comboBox_RandPolyLen->setCurrentIndex(0);
            ui->lineEdit_RandPoly->setText("A9");
            ui->lineEdit_RandPhase->setText("FF");
        }

    }
}
//副帧同步方式
void SS2BBE_TMParaSetGUIDlg::on_comboBox_SubFrmSyncType_currentIndexChanged(int index)
{
    if (ui->pushButton_Modify->text() == "更改")
        return;

    if(index==0){//无副帧

        ui->comboBox_SubFrameGroupLen->setEnabled(false);
        ui->lineEdit_SubFrmSynGroupPos->setEnabled(false);
        ui->lineEdit_SubFrmGroup->setEnabled(false);
        ui->comboBox_IDBaseValue->setEnabled(false);
        ui->comboBox_idDirect->setEnabled(false);
        ui->lineEdit_idCountPos->setEnabled(false);
        ui->lineEdit_SubFrameLen->setEnabled(false);

        ui->comboBox_SubFrameSyncFaultLockDetection->setEnabled(false);
        ui->comboBox_SubCheckFrameOfFaultFrameSync->setEnabled(false);

        ui->comboBox_SubFrameSyncFaultTolerantLockBit->setEnabled(false);
        ui->comboBox_SubFaltToleranceOfFrameSync->setEnabled(false);
        ui->comboBox_SubFrameSyncFaultTolerant->setEnabled(false);





    }
    if(index==2){//循环副帧
        ui->comboBox_SubFrameGroupLen->setEnabled(true);
        ui->lineEdit_SubFrmSynGroupPos->setEnabled(true);
        ui->lineEdit_SubFrmGroup->setEnabled(true);
        ui->comboBox_IDBaseValue->setEnabled(false);
        ui->comboBox_idDirect->setEnabled(false);
        ui->lineEdit_idCountPos->setEnabled(false);
        ui->lineEdit_SubFrameLen->setEnabled(true);

        ui->comboBox_SubFrameSyncFaultLockDetection->setEnabled(true);
        ui->comboBox_SubCheckFrameOfFaultFrameSync->setEnabled(true);

        ui->comboBox_SubFrameSyncFaultTolerantLockBit->setEnabled(true);
        ui->comboBox_SubFaltToleranceOfFrameSync->setEnabled(true);
        ui->comboBox_SubFrameSyncFaultTolerant->setEnabled(true);




    }
    if(index==1){//ID副帧

        ui->comboBox_SubFrameGroupLen->setEnabled(false);
        ui->lineEdit_SubFrmSynGroupPos->setEnabled(false);
        ui->lineEdit_SubFrmGroup->setEnabled(false);
        ui->comboBox_IDBaseValue->setEnabled(true);
        ui->comboBox_idDirect->setEnabled(true);
        ui->lineEdit_idCountPos->setEnabled(true);
        ui->lineEdit_SubFrameLen->setEnabled(true);


        ui->comboBox_SubFrameSyncFaultLockDetection->setEnabled(true);
        ui->comboBox_SubCheckFrameOfFaultFrameSync->setEnabled(true);

        ui->comboBox_SubFrameSyncFaultTolerantLockBit->setEnabled(false);
        ui->comboBox_SubFaltToleranceOfFrameSync->setEnabled(false);
        ui->comboBox_SubFrameSyncFaultTolerant->setEnabled(false);







    }
    if(index==3){//反码副帧
        ui->comboBox_SubFrameGroupLen->setEnabled(false);
        ui->lineEdit_SubFrmSynGroupPos->setEnabled(false);
        ui->lineEdit_SubFrmGroup->setEnabled(false);
        ui->comboBox_IDBaseValue->setEnabled(false);
        ui->comboBox_idDirect->setEnabled(false);
        ui->lineEdit_idCountPos->setEnabled(false);
        ui->lineEdit_SubFrameLen->setEnabled(true);


        ui->comboBox_SubFrameSyncFaultLockDetection->setEnabled(true);
        ui->comboBox_SubCheckFrameOfFaultFrameSync->setEnabled(true);

        ui->comboBox_SubFrameSyncFaultTolerantLockBit->setEnabled(true);
        ui->comboBox_SubFaltToleranceOfFrameSync->setEnabled(true);
        ui->comboBox_SubFrameSyncFaultTolerant->setEnabled(true);

    }

}

void SS2BBE_TMParaSetGUIDlg::updateRadioButton()
{
    if(!m_pCommFunc)return;
    if (m_currentNumber_usedByMacro == m_pCommFunc->getTargetNum())
        return;
    m_currentNumber_usedByMacro = m_pCommFunc->getTargetNum();
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
#pragma pack()
