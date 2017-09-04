#include "SSBBE_SysParamSetGUIDlg.h"
#include "ui_SSBBE_SysParamSetGUIDlg.h"
#include <QCloseEvent>
#include <QMessageBox>
#include <QDateTime>
#include "../../Common/CommonFunction.h"

#include <QDebug>
#pragma pack(1)
extern quint64 GetID(ushort usStationID, uchar ucSubSysID, ushort usDeviceID, ushort usType,
                     uchar ucSn);
SSBBE_SysParamSetGUIDlg::SSBBE_SysParamSetGUIDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SSBBE_SysParamSetGUIDlg)
{
    ui->setupUi(this);
    m_pStateMgr = NULL;
    m_pILxTsspMessageManager = NULL;
    m_pILxTsspLogManager = NULL;
    m_pPublicInfo = NULL;
    m_pHAManager = NULL;
    m_nTargetIndex = 0;
    m_pCommFunc = NULL;
    m_timer1 = new QTimer(this);
    connect(m_timer1, SIGNAL(timeout()), this, SLOT(updateTarget()));
    m_pMCSTargetSysStatus = NULL;
    m_currentNumber_usedByMacro = -1;

    setAttribute(Qt::WA_DeleteOnClose);
}

TLxTsspParamMacroInformation* SSBBE_SysParamSetGUIDlg::getMacroInfo()
{
    int sizeOfMacroInfo = sizeof(TLxTsspParamMacroInformation);
    TLxTsspParamMacroInformation* pMacroInfo = (TLxTsspParamMacroInformation*)
            m_pStateMgr->FindOneItem(m_targetNumID, sizeOfMacroInfo);
    if(!pMacroInfo || sizeOfMacroInfo != sizeof(TLxTsspParamMacroInformation))return NULL;
    if(pMacroInfo->ucTargetSum < 1 || pMacroInfo->ucTargetSum > 4)return NULL;
    return pMacroInfo;
}
SSBBE_SysParamSetGUIDlg::~SSBBE_SysParamSetGUIDlg()
{
    killTimer(m_TimerID);
    m_timer1->stop();
    delete m_timer1;
    delete ui;
}
int SSBBE_SysParamSetGUIDlg::msgBoxFunction(QMessageBox &msgBox, QString msg, BOOL ret)
{
    msgBox.setText(msg);
    msgBox.exec();
    return ret;
}
void SSBBE_SysParamSetGUIDlg::setText(const QString& strTitle, ParaStruct xmlsetText[5][60], QString MapMean[50][30], int MapNum[50], uchar Type, ILxTsspParameterMacro* pMacroObj)
{
    m_strTitle = strTitle;
    setWindowTitle(strTitle);

    memcpy(m_DlgParaSetStruct, xmlsetText, sizeof(ParaStruct) * 60 * 5);

    for(int i=0; i<50; i++)
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
void SSBBE_SysParamSetGUIDlg::InitDLG(int Type, ILxTsspParameterMacro* pMacroObj)
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
        if(m_pHAManager != NULL)
        {
            m_pHAManager->GetParameter(m_ParaMCSTargetSysID, byteArray);
            memcpy(&m_MCSTargetSysParam, byteArray.data(), sizeof(SMCU::MCSSysTarget4ParamStruct));
        }
        if(m_pStateMgr != NULL)
        {
            DWORD stLen = sizeof(SMCU::MCSSysTarget4StatusStruct);
            m_pMCSTargetSysStatus = (SMCU::MCSSysTarget4StatusStruct*)m_pStateMgr->FindOneItem(m_StatusParaMCSTargetSysID, stLen);
            if(m_pMCSTargetSysStatus == NULL || stLen != sizeof(SMCU::MCSSysTarget4StatusStruct))
                return;
        }
        //启动定时器
        m_TimerID = startTimer(500);
        //不使能
        EnabledControls(false);
    }
    else if(Type == 1)//取宏里参数
    {
        ui->pushButton_Set->setEnabled(false);
        if(pMacroObj != NULL)
        {
            QByteArray byteArray;
            pMacroObj->GetParameterBlock(m_ParaMCSTargetSysID, byteArray);
            memcpy(&m_MCSTargetSysParam, byteArray.data(), sizeof(SMCU::MCSSysTarget4ParamStruct));
        }
        ui->pushButton_Back->setVisible(false);
        ui->pushButton_Set->setVisible(false);
        ui->lineEdit_TaskFlag->hide();
        ui->lineEdit_TaskCode->hide();
        ui->comboBox_TargetsCount->hide();
        ui->label_TargetsCount->hide();
        ui->label_TaskFlag->hide();
        ui->label_TaskCode->hide();
        ui->pushButton_Back->setText("");
        m_pCommFunc = CCommFunc::getInstance();
        //显示参数
        ShowPara(1);
        m_timer1->start(500);
    }
}

//设置输入格式
void SSBBE_SysParamSetGUIDlg::InitInput()
{
    //任务代号
    QRegExp regExp0(".{1,6}");
    ui->lineEdit_TaskCode->setValidator(new QRegExpValidator(regExp0, this));
    //正整数
    QRegExp regExp("[0-9A-Fa-f]{1,8}");
    //任务标识
    ui->lineEdit_TaskFlag->setValidator(new QRegExpValidator(regExp, this));
    QRegExp regExpFre("[0-9]{1,4}(\\.[0-9]{1,6})?");
    //上行频点
    ui->lineEdit_UpFreDot->setValidator(new QRegExpValidator(regExpFre, this));
    //下行频点
    ui->lineEdit_DownFreDot->setValidator(new QRegExpValidator(regExpFre, this));
    //字母数字
    QRegExp regExp2("[A-Fa-f0-9]{2}");
    //测速数据标识码
    ui->lineEdit_SpeedProcessCode->setValidator(new QRegExpValidator(regExp2, this));
    //遥测数据标识码
    ui->lineEdit_TelMeterProcessCode->setValidator(new QRegExpValidator(regExp2, this));
    //测距数据标识码
    ui->lineEdit_MeterProcessCode ->setValidator(new QRegExpValidator(regExp2, this));
}

void SSBBE_SysParamSetGUIDlg::ShowControls()
{
    //向下拉框控件中写入从xml中读取的内容
    showComParam();
    showTargetParam(0);
    //默认选择目标1
    ui->radioButton_Target1->setChecked(true);

    //频段选择默认维不可用
    //ui->comboBox_FreBandType->setEnabled(false);
}

//显示参数
void SSBBE_SysParamSetGUIDlg::ShowPara(int type)
{
    if (type == 0)       //参数设置
    {
        if(m_pMCSTargetSysStatus != NULL)
        {
            m_MCSTargetSysParam.sysParam = m_pMCSTargetSysStatus->sysStatu.tParams;
            for (int i = 0; i < MAX_TARGET_COUNT; i++)
                m_MCSTargetSysParam.targetSysParam[i] = m_pMCSTargetSysStatus->targetSysStatu[i].tParams;
        }
        else
        {
            DWORD stLen = sizeof(SMCU::MCSSysTarget4StatusStruct);
            if(m_pStateMgr != NULL)
                m_pMCSTargetSysStatus  = (SMCU::MCSSysTarget4StatusStruct*)m_pStateMgr->FindOneItem(m_StatusParaMCSTargetSysID, stLen);
        }
    }
    else
    {
        //解扰使能或禁止
        //副帧方式控件使能
        if (ui->pushButton_Back->text() != "更改")
        {
            //EnabledControlsSubFrmType(ui->comboBox_SubFrmType->currentIndex());
        }
    }
    //    if (m_MCSTargetSysParam.sysParam.ucTargetNum == 0)
    //        return;
    //显示参数中的数据内容
    showComParam(1);
    //显示目标的数据所
    //for (int i = 0; i < MAX_TARGET_NUM; i++)
    //    showTargetParam(i, 1);
    showTargetParam(m_nTargetIndex, 1);
}

//设置帧长范围
void SSBBE_SysParamSetGUIDlg::SetFrameLenRange(int iWordLen)
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

void SSBBE_SysParamSetGUIDlg::SetObjInfo(TLxTsspObjectInfo ObjectInfo)
{
    memcpy(&m_ObjectInfo, &ObjectInfo, sizeof(TLxTsspObjectInfo));
    //对象初始化
    //查找对象管理器
    LxTsspObjectManager* pObjectManager = LxTsspObjectManager::Instance();
    if(pObjectManager == NULL)
    {
        return;
    }

    m_pHAManager = pObjectManager->GetHAManager();

    bool bRet = m_DeviceMap.loadXML();
    if (!bRet)
    {
        QString strInfo = QString("load device map failed %1").arg(MODULENAME);
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
        m_ParaMCSTargetSysID = GetID(sItem.usStationID,
                                     sItem.ucSubSysID, sItem.usDeviceID, sItem.usTID,
                                     sItem.ucSN);
        /*************************框架**********************************/

        /*************************状态**********************************/
        m_StatusParaMCSTargetSysID = GetID(sItem.usStationID,
                                           sItem.ucSubSysID, sItem.usDeviceID, sItem.usStateType,
                                           sItem.ucSN);
        /*************************状态**********************************/
    }

    pItem = m_DeviceMap.getItem(SS1_TARGET_NUM);
    if(pItem && pItem->vSubItem.size())
    {
        SUBITEM sItem = pItem->vSubItem.at(0);
        /*************************框架**********************************/
        m_targetNumID = GetID(sItem.usStationID,
                              sItem.ucSubSysID, sItem.usDeviceID, sItem.usStateType,
                              sItem.ucSN);
        /*************************框架**********************************/
    }

    ITEM* pItemCom = m_DeviceMap.getItem(CMDNAME4);
    if (pItemCom != NULL)
    {
        SUBITEM* pSItem = pItemCom->getSubItemBySubSysIDAndDevID(m_ObjectInfo.ucSubSysID, m_ObjectInfo.usLocalDID);
        if(pSItem)
            m_sItem4 = *pSItem;
        else{
            int index = 2;
            m_sItem4 = pItemCom->vSubItem.at(index);
            if(m_sItem4.usDeviceID != m_ObjectInfo.usLocalDID)
                m_sItem4 = pItemCom->vSubItem.at(index + 1);
        }
    }

    //日志
    m_pILxTsspLogManager = pObjectManager->GetLogManager();
    m_pStateMgr = pObjectManager->GetStateManager();
    m_pILxTsspMessageManager = pObjectManager->GetMessageManager();
    m_pPublicInfo = pObjectManager->GetPublicInfoManager();
}

//使能或不使能控件
void SSBBE_SysParamSetGUIDlg::EnabledControls(bool bFlag)
{
    /*************************Group1*************************/
    //目标数
    ui->comboBox_TargetsCount->setEnabled(bFlag);
    //基带传送方式
    ui->comboBox_BaseDeliverType->setEnabled(bFlag);
    /*************************Group1*************************/

    /*************************Group2************************* /
    //设置目标想选卡不可用
    ui->radioButton_Target1->setEnabled(bFlag);
    ui->radioButton_Target2->setEnabled(bFlag);
    ui->radioButton_Target3->setEnabled(bFlag);
    ui->radioButton_Target4->setEnabled(bFlag);
    /*************************Group2*************************/

    /*************************Group3*************************/
    //频段选择
    //ui->comboBox_FreBandType->setEnabled(bFlag);
    //上行频点
    ui->lineEdit_UpFreDot->setEnabled(bFlag);
    //下行频点
    ui->lineEdit_DownFreDot->setEnabled(bFlag);
    //任务标识
    ui->lineEdit_TaskFlag->setEnabled(bFlag);
    //任务代号
    ui->lineEdit_TaskCode->setEnabled(bFlag);
    //测速数据处理标识码
    ui->lineEdit_SpeedProcessCode->setEnabled(bFlag);
    //遥测数据处理标识码
    ui->lineEdit_TelMeterProcessCode->setEnabled(bFlag);
    //测距数据处理标识码
    ui->lineEdit_MeterProcessCode->setEnabled(bFlag);
    //测速采样周期
    ui->comboBox_SpeedCircle->setEnabled(bFlag);
    //测距采样率
    ui->comboBox_SpeedGetRate->setEnabled(bFlag);
    /*************************Group3*************************/

    if (bFlag)
    {
        //副帧方式控件使能
        //EnabledControlsSubFrmType(ui->comboBox_SubFrmType->currentIndex());
        ui->pushButton_Back->setText("恢复");
        ui->pushButton_Set->setEnabled(true);
    }
    else
    {
        ui->pushButton_Back->setText("更改");
        ui->pushButton_Set->setEnabled(false);
    }
}

//保存参数宏
int SSBBE_SysParamSetGUIDlg::SaveToMacro(ILxTsspParameterMacro* pMacroObj)
{
    //判断合法性
    int iRet = CheckPara();
    if(iRet == -1)
    {
        return -1;
    }
    else
    {
        QByteArray byteArray;
        byteArray.resize(sizeof(SMCU::MCSSysTarget4ParamStruct));
        SMCU::MCSSysTarget4ParamStruct tmp;//宏角误差保存
        if(1 == pMacroObj->GetParameterBlock(m_ParaMCSTargetSysID, byteArray)){
            memcpy(&tmp, byteArray.data(), sizeof(m_MCSTargetSysParam));
            m_MCSTargetSysParam.sysParam.ucTrackOutType = tmp.sysParam.ucTrackOutType;
            m_MCSTargetSysParam.sysParam.ucTrackDefCH = tmp.sysParam.ucTrackDefCH;
            m_MCSTargetSysParam.sysParam.ucTrackChnlSel = tmp.sysParam.ucTrackChnlSel;
        }
        GetPara();
        updateParam(m_MCSTargetSysParam, pMacroObj);
        //取当前设置的参数
        memcpy(byteArray.data(), &m_MCSTargetSysParam, sizeof(m_MCSTargetSysParam));
        pMacroObj->UpdateParameterBlock(m_ParaMCSTargetSysID, byteArray);

        //日志信息
        QString strLog;
        strLog += QString("%1: 保存宏成功！").arg(MODULENAME);
        CLog::addLog(strLog);
    }
}
//从界面取参数
void SSBBE_SysParamSetGUIDlg::GetPara()
{
    /***********************************码同步****************************/
    //目标数
    m_MCSTargetSysParam.sysParam.ucTargetNum = ui->comboBox_TargetsCount->currentIndex() + 1;
    //基带送数选择
    m_MCSTargetSysParam.sysParam.ucSendMode = ui->comboBox_BaseDeliverType->currentIndex() + 1;

    getParamTarget();
}

//验证参数范围
int SSBBE_SysParamSetGUIDlg::CheckPara()
{
    QMessageBox msgBox;
    msgBox.setWindowTitle(MODULENAME);
    QString strTmp;
    if(!ui->pushButton_Back->text().isEmpty())
    {
        if (ui->lineEdit_TaskCode->text().isEmpty())
        {
            strTmp = QString("任务代号不能为空，请重新输入！");
            goto flag;
        }
        if (ui->lineEdit_TaskCode->text().length() != 6)
        {
            strTmp = QString("任务代号必须为6位！");
            goto flag;
        }
        if (ui->lineEdit_TaskFlag->text().isEmpty())
        {
            strTmp = QString("任务标识不能为空，请重新输入！");
            goto flag;
        }
        if (ui->lineEdit_TaskFlag->text().length() > 8 || ui->lineEdit_TaskFlag->text().isEmpty())
        {
            strTmp = QString("任务标识必须为1~8位16进制字符！");
            goto flag;
        }
    }
    if (ui->lineEdit_UpFreDot->text().isEmpty())
    {
        strTmp = QString("上行频点不能为空，请重新输入!");
        goto flag;
    }
    if (ui->lineEdit_UpFreDot->text().toDouble() < m_DlgParaSetStruct[1][1].MinVal.lmin / 1000000.0
            || ui->lineEdit_UpFreDot->text().toDouble() > m_DlgParaSetStruct[1][1].MaxVal.lmax / 1000000.0)
    {
        strTmp = QString("上行频点正确范围\n[" + QString::number(m_DlgParaSetStruct[1][1].MinVal.lmin / 1000000.0) +
                "～" + QString::number(m_DlgParaSetStruct[1][1].MaxVal.lmax / 1000000.0) + "]MHz，请重新输入!");
        goto flag;
    }
    if (ui->lineEdit_DownFreDot->text().isEmpty())
    {
        strTmp = QString("下行频点不能为空，请重新输入!");
        goto flag;
    }
    if (ui->lineEdit_DownFreDot->text().toDouble() < m_DlgParaSetStruct[1][2].MinVal.lmin / 1000000.0
            || ui->lineEdit_DownFreDot->text().toDouble() > m_DlgParaSetStruct[1][2].MaxVal.lmax / 1000000.0)
    {
        strTmp = QString("下行频点正确范围\n[" + QString::number(m_DlgParaSetStruct[1][2].MinVal.lmin / 1000000.0) +
                "～" + QString::number(m_DlgParaSetStruct[1][2].MaxVal.lmax / 1000000.0) + "]MHz，请重新输入!");
        goto flag;
    }
    if(ui->lineEdit_MeterProcessCode->text().isEmpty())
    {
        return msgBoxFunction(msgBox,"测速数据处理标识码输入为空，请重新输入!", FALSE);
    }

    if(ui->lineEdit_SpeedProcessCode->text().isEmpty())
    {
        return msgBoxFunction(msgBox,"遥测数据处理标识码输入为空，请重新输入!", FALSE);
    }
    if(ui->lineEdit_TelMeterProcessCode->text().isEmpty())
    {
        return msgBoxFunction(msgBox,"测距数据处理标识码输入为空，请重新输入!", FALSE);
    }
    return 1;
flag:
    msgBox.setText(strTmp);
    msgBox.exec();
    return -1;
}

//定时器
void SSBBE_SysParamSetGUIDlg::timerEvent(QTimerEvent *event)
{
    ShowPara(0);
}

//更改参数按钮休息处理函数
void SSBBE_SysParamSetGUIDlg::on_pushButton_Back_clicked()
{
    if(ui->pushButton_Back->text() == "更改")
    {
        EnabledControls(true);
        killTimer(m_TimerID);
        return;
    }

    if(ui->pushButton_Back->text() == "恢复")
    {
        EnabledControls(false);
        m_TimerID = startTimer(500);
        return;
    }
}

void SSBBE_SysParamSetGUIDlg::on_pushButton_Set_clicked()
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
            struct{USHORT target;//公共和目标  1-5，第一位公共
                   USHORT tmp;//占位
                      SMCU::MCSSysParamStruct sys;//公共参数
                         struct{USHORT tmp1;//占位
                                SMCU::MCSTargetSysParamStruct targetSysParam;//目标参数
                               }param[MAX_TARGET_COUNT];
                  }data;

            QByteArray byteArray;
            byteArray.resize(sizeof(data));
            data.target = 1 | 0x02 | 0x04 | 0x08 | 0x10;

            data.sys = m_MCSTargetSysParam.sysParam;
            for(int i = 0; i < MAX_TARGET_COUNT; ++i)
            {
                data.param[i].targetSysParam = m_MCSTargetSysParam.targetSysParam[i];
            }
            memcpy(byteArray.data(),&data,sizeof(data));
            SendMessage((char*)byteArray.data(), byteArray.size(), m_usC, m_usTID);


//            //基带校零结果查询
//            static USHORT seq = 0;
//            struct{
//                UCHAR unitNo;
//                char m;
//                USHORT seq;
//                UINT   uiM ;		    //任务标识（M字段）
//                DWORDLONG   dwlComboID; //设备组合号
//                DWORDLONG   dwlAnswerRevUpFreq;//上行工作频率，范围20250000~21200000，步进1Hz
//                DWORDLONG   dwlAnswerRevDownFreq;//下行工作频率，范围22000000~23000000，步进1Hz
//                UINT reserve1;
//                UINT reserve2;
//            }adj0;
//            adj0.unitNo = 57;
//            adj0.m = 'M';
//            adj0.seq = seq++;
//            adj0.uiM = m_MCSSysDataParam.uiM;
//            adj0.dwlComboID = m_MCSSysDataParam.dwlComboID;
//            adj0.dwlAnswerRevUpFreq = m_MCSSysDataParam.dwlAnswerRevDownFreq;
//            adj0.dwlAnswerRevDownFreq = m_MCSSysDataParam.dwlAnswerRevDownFreq;
//            SendMessage((char*)&adj0, sizeof(adj0), m_sItem4.usCMD, m_sItem4.usTID);

        }

        //日志信息
        QString strLog;
        strLog += QString("%1: 设置参数成功！").arg(MODULENAME);
        CLog::addLog(strLog);

        //不使能控件
        EnabledControls(false);
        m_TimerID = startTimer(500);
    }
}

void SSBBE_SysParamSetGUIDlg::closeEvent(QCloseEvent *event)
{
    event->accept();
}

QSize SSBBE_SysParamSetGUIDlg::sizeHint() const
{
    return QSize(this->width(), this->height());
}

void SSBBE_SysParamSetGUIDlg::SubFrmEn(bool bFlag)
{

}

//有无ID副帧
void SSBBE_SysParamSetGUIDlg::IDSubFrmEn(bool bFlag)
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
void SSBBE_SysParamSetGUIDlg::CycSubFrmEn(bool bFlag)
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
void SSBBE_SysParamSetGUIDlg::ReserveSubFrmEn(bool bFlag)
{

}

void SSBBE_SysParamSetGUIDlg::SendMessage(char* pData, unsigned short  usLen, unsigned short usC, unsigned short usTID)
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
    subHeader.O = 0;
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

void SSBBE_SysParamSetGUIDlg::showComParam(int nType)
{
    if (nType == 0)
    {
        int i;
        //目标数
        for(i=0; i<DlgMapLineNum[0]; i++)
        {
            ui->comboBox_TargetsCount->addItem(m_DlgMapMean[0][i]);
        }
        ui->comboBox_TargetsCount->setCurrentIndex(m_DlgParaSetStruct[0][0].InitVal.lval);
        //基带送数方式
        for(i=0; i<DlgMapLineNum[1]; i++)
        {
            ui->comboBox_BaseDeliverType->addItem(m_DlgMapMean[1][i]);
        }
        ui->comboBox_BaseDeliverType->setCurrentIndex(m_DlgParaSetStruct[0][1].InitVal.lval);
    }
    else if (nType == 1)
    {
        //目标数
        int nTargetNum = m_MCSTargetSysParam.sysParam.ucTargetNum;
        nTargetNum = (nTargetNum > 0) ? nTargetNum - 1 : 0;
        ui->comboBox_TargetsCount->setCurrentIndex(nTargetNum);
        //基带送数选择
        int nMode = m_MCSTargetSysParam.sysParam.ucSendMode;
        nMode =  (nMode > 0) ? nMode - 1 : 0;
        ui->comboBox_BaseDeliverType->setCurrentIndex(nMode);
    }
}

void SSBBE_SysParamSetGUIDlg::showTargetParam(int nIndex, int nType)
{
    int i;
    /*******************************目标参数*********************************/
    if (nType == 0)
    {
        nIndex += 1;
        //频段选择
        // for(i=0; i<DlgMapLineNum[nIndex * 2 + 0]; i++)
        //{
        //ui->comboBox_FreBandType->addItem(m_DlgMapMean[nIndex * 2 + 0][i]);
        //}
        //ui->comboBox_FreBandType->setCurrentIndex(m_DlgParaSetStruct[nIndex][0].InitVal.lval);
        // ui->comboBox_FreBandType->setCurrentIndex(0);
        //上行频点
        ui->lineEdit_UpFreDot->setText(QString::number(m_DlgParaSetStruct[nIndex][1].InitVal.lval / 1000000.0));
        //下行频点
        ui->lineEdit_DownFreDot->setText(QString::number(m_DlgParaSetStruct[nIndex][2].InitVal.lval / 1000000.0));
        //任务标识
        ui->lineEdit_TaskFlag->setText(QString::number(m_DlgParaSetStruct[nIndex][3].InitVal.lval, 16).toUpper());
        //任务代号
        ui->lineEdit_TaskCode->setText(m_DlgParaSetStruct[nIndex][4].CVal.cValue);

        //测速数据标志码
        ui->lineEdit_SpeedProcessCode->setText(QString::number(m_DlgParaSetStruct[nIndex][5].InitVal.lval, 16).toUpper());
        //遥测数据标志码
        ui->lineEdit_MeterProcessCode->setText(QString::number(m_DlgParaSetStruct[nIndex][6].InitVal.lval, 16).toUpper());
        //测距数据标识码
        ui->lineEdit_TelMeterProcessCode->setText(QString::number(m_DlgParaSetStruct[nIndex][7].InitVal.lval, 16).toUpper());
        //测速采样周期
        for(i=0; i<DlgMapLineNum[nIndex * 2 + 1]; i++)
        {
            ui->comboBox_SpeedCircle->addItem(m_DlgMapMean[nIndex * 2 + 1][i]);
        }
        ui->comboBox_SpeedCircle->setCurrentIndex(m_DlgParaSetStruct[nIndex][8].InitVal.lval);
        //测速采样率
        for(i=0; i<DlgMapLineNum[nIndex * 2 + 2]; i++)
        {
            ui->comboBox_SpeedGetRate->addItem(m_DlgMapMean[nIndex * 2 + 2][i]);
        }
        ui->comboBox_SpeedGetRate->setCurrentIndex(m_DlgParaSetStruct[nIndex][9].InitVal.lval);
    }
    else if (nType == 1)
    {
        //频段选择
        //    ui->comboBox_FreBandType->setCurrentIndex(m_MCSTargetSysParam.ucFreBand);
        //上行频点
        ui->lineEdit_UpFreDot->setText(QString::number(m_MCSTargetSysParam.targetSysParam[nIndex].dwlAnswerRevUpFreq / 1000000.0, 'f', 6));
        //下行频点
        ui->lineEdit_DownFreDot->setText(QString::number(m_MCSTargetSysParam.targetSysParam[nIndex].dwlAnswerRevDownFreq / 1000000.0, 'f', 6));
        //任务标识
        ui->lineEdit_TaskFlag->setText(QString::number(m_MCSTargetSysParam.targetSysParam[nIndex].uiM, 16).toUpper());
        //任务代号
        char szVal[7] = { 0 };
        memcpy(szVal, m_MCSTargetSysParam.targetSysParam[nIndex].ucTaskDescribe,
               sizeof(m_MCSTargetSysParam.targetSysParam[nIndex + 1].ucTaskDescribe));
        ui->lineEdit_TaskCode->setText(szVal);
        //测速数据处理标识码
        ui->lineEdit_SpeedProcessCode->setText(QString::number(m_MCSTargetSysParam.targetSysParam[nIndex].ucCsCode, 16).toUpper());
        //遥测数据处理标识码
        ui->lineEdit_TelMeterProcessCode->setText(QString::number(m_MCSTargetSysParam.targetSysParam[nIndex].ucTmCode, 16).toUpper());
        //测距数据处理标识码
        ui->lineEdit_MeterProcessCode->setText(QString::number(m_MCSTargetSysParam.targetSysParam[nIndex].ucCjCode, 16).toUpper());
        //测速采样周期
        int nCurSel = m_MCSTargetSysParam.targetSysParam[nIndex].ucCSSendRate;
        if (nCurSel > 0){
            if(nCurSel == 3)
                nCurSel=1;
            else if(nCurSel == 1)
                nCurSel = 3;
            else
                nCurSel=nCurSel;
            ui->comboBox_SpeedCircle->setCurrentIndex(nCurSel - 1);
        }

        //测距采样率
        nCurSel = m_MCSTargetSysParam.targetSysParam[nIndex].ucCJSendRate;
        if (nCurSel > 0){
            if(nCurSel>3)
                nCurSel-=1;
            ui->comboBox_SpeedGetRate->setCurrentIndex(nCurSel - 1);
        }
        set_radioButton_Target();
    }
    /*******************************目标参数*********************************/
}

void SSBBE_SysParamSetGUIDlg::getParamTarget()
{
    //获取当前选择的目标几
    //获取当前界面上的数据
    //m_MCSTargetSysParam.targetSysParam[m_nTargetIndex].ucFreBand = ui->comboBox_FreBandType->currentIndex();
    //上行频点
    m_MCSTargetSysParam.targetSysParam[m_nTargetIndex].dwlAnswerRevUpFreq = (DWORDLONG)(ui->lineEdit_UpFreDot->text().toDouble() * 1000000);
    //下行频点
    m_MCSTargetSysParam.targetSysParam[m_nTargetIndex].dwlAnswerRevDownFreq = (DWORDLONG)(ui->lineEdit_DownFreDot->text().toDouble() * 1000000);
    //任务标识
    bool ok = false;
    m_MCSTargetSysParam.targetSysParam[m_nTargetIndex].uiM = ui->lineEdit_TaskFlag->text().toUInt(&ok, 16);
    //任务代号
    QByteArray strTem1 = ui->lineEdit_TaskCode->text().toLatin1();
    memcpy(m_MCSTargetSysParam.targetSysParam[m_nTargetIndex].ucTaskDescribe, strTem1.data(), strTem1.size() > 6 ? 6 : strTem1.size());
    //测速数据标识码
    m_MCSTargetSysParam.targetSysParam[m_nTargetIndex].ucCsCode = ui->lineEdit_SpeedProcessCode->text().toInt(&ok, 16);
    //遥测数据标识码
    m_MCSTargetSysParam.targetSysParam[m_nTargetIndex].ucTmCode = ui->lineEdit_TelMeterProcessCode->text().toInt(&ok, 16);
    //测距数据标识码
    m_MCSTargetSysParam.targetSysParam[m_nTargetIndex].ucCjCode = ui->lineEdit_MeterProcessCode->text().toInt(&ok, 16);
    //采样周期

    int nCurSel= ui->comboBox_SpeedCircle->currentIndex() + 1;
    if (nCurSel > 0){
        if(nCurSel==3)
            nCurSel=1;
        else if(nCurSel==1)
            nCurSel=3;
        else
            nCurSel=nCurSel;
        m_MCSTargetSysParam.targetSysParam[m_nTargetIndex].ucCSSendRate = nCurSel;
    }
    //采样率
    m_MCSTargetSysParam.targetSysParam[m_nTargetIndex].ucCJSendRate;
    nCurSel= ui->comboBox_SpeedGetRate->currentIndex() + 1;
    if (nCurSel > 0){
        if(nCurSel>2)
            nCurSel+=1;
        m_MCSTargetSysParam.targetSysParam[m_nTargetIndex].ucCJSendRate=nCurSel;
    }
}

DWORDLONG SSBBE_SysParamSetGUIDlg::StrToSynCode(const char * pStr)
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

void SSBBE_SysParamSetGUIDlg::on_comboBox_TargetsCount_currentIndexChanged(int index)
{
    int num = m_MCSTargetSysParam.sysParam.ucTargetNum - 1;
    if(!this->isHidden() && num != index && 1 != CheckPara())
    {
        set_radioButton_Target();
        ui->comboBox_TargetsCount->setCurrentIndex(num);
        return;
    }
    m_nTargetIndex = 0;
    set_radioButton_Target();
    ui->radioButton_Target1->hide();
    ui->radioButton_Target2->hide();
    ui->radioButton_Target3->hide();
    ui->radioButton_Target4->hide();
    switch(index)
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

void SSBBE_SysParamSetGUIDlg::targetsCountChanged(int index)
{
    if(!this->isHidden() && 1 != CheckPara())
    {
        set_radioButton_Target();
        return;
    }
    m_nTargetIndex = 0;
    set_radioButton_Target();
    ui->radioButton_Target1->hide();
    ui->radioButton_Target2->hide();
    ui->radioButton_Target3->hide();
    ui->radioButton_Target4->hide();
    switch(index)
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

void SSBBE_SysParamSetGUIDlg::set_radioButton_Target()
{
    switch(m_nTargetIndex)
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
void SSBBE_SysParamSetGUIDlg::on_radioButton_Target1_clicked()
{
    if(1 != CheckPara())
    {
        set_radioButton_Target();
        return;
    }
    GetPara();
    //界面显示对应的参数数据
    m_nTargetIndex = 0;
    //int nType = ui->pushButton_Set->isEnabled()  ? 0 : 1;
    showTargetParam(0, 1);
}

void SSBBE_SysParamSetGUIDlg::on_radioButton_Target2_clicked()
{
    if(1 != CheckPara())
    {
        set_radioButton_Target();
        return;
    }
    GetPara();
    m_nTargetIndex = 1;
    //int nType = ui->pushButton_Set->isEnabled() ? 0 : 1;
    showTargetParam(1, 1);
}

void SSBBE_SysParamSetGUIDlg::on_radioButton_Target3_clicked()
{
    if(1 != CheckPara())
    {
        set_radioButton_Target();
        return;
    }
    GetPara();
    m_nTargetIndex = 2;
    //int nType = ui->pushButton_Set->isEnabled()  ? 0 : 1;
    showTargetParam(2, 1);
}

void SSBBE_SysParamSetGUIDlg::on_radioButton_Target4_clicked()
{
    if(1 != CheckPara())
    {
        set_radioButton_Target();
        return;
    }
    GetPara();
    m_nTargetIndex = 3;
    //int nType = ui->pushButton_Set->isEnabled()  ? 0 : 1;
    showTargetParam(3, 1);
}

void SSBBE_SysParamSetGUIDlg::updateTarget()
{
    if(!m_pCommFunc)return;
    if (m_currentNumber_usedByMacro == m_pCommFunc->getTargetNum())
        return;
    m_currentNumber_usedByMacro = m_pCommFunc->getTargetNum();
    targetsCountChanged(m_currentNumber_usedByMacro - 1);
}

void SSBBE_SysParamSetGUIDlg::updateParam(SMCU::MCSSysTarget4ParamStruct& comPara, ILxTsspParameterMacro* pMacroObj)
{
    TLxTsspParamMacroInformation* pMacroInfo = getMacroInfo();
    if(!pMacroInfo)return;
    comPara.sysParam.ucTargetNum = pMacroInfo->ucTargetSum;
    if(!pMacroObj)return;
    TLxTsspParamMacroInformation macroInfo;
    if(1 != pMacroObj->GetMacroInformation(macroInfo))return;
    for(int i = 0; i < pMacroInfo->ucTargetSum; ++i)
    {
        comPara.targetSysParam[i].uiM = macroInfo.aTaskMark[i];
        memcpy(comPara.targetSysParam[i].ucTaskDescribe, macroInfo.aTaskID[i], 6);
    }
}

#pragma pack()

void SSBBE_SysParamSetGUIDlg::on_lineEdit_UpFreDot_textChanged(const QString &arg1)
{
    CCommFunc::lineEditPrecision(arg1, ui->lineEdit_UpFreDot, 6);
}

void SSBBE_SysParamSetGUIDlg::on_lineEdit_DownFreDot_textChanged(const QString &arg1)
{
    CCommFunc::lineEditPrecision(arg1, ui->lineEdit_DownFreDot, 6);
}
