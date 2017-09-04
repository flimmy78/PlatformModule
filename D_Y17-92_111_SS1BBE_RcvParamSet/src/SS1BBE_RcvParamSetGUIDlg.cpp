#include "SS1BBE_RcvParamSetGUIDlg.h"
#include "ui_SS1BBE_RcvParamSetGUIDlg.h"
#include <QCloseEvent>
#include <QMessageBox>
#include <QDateTime>
#include "../../Common/Log.h"
#include "../../Common/CommonFunction.h"
#include <QTimer>
#pragma pack(1)
extern quint64 GetID(ushort usStationID, uchar ucSubSysID, ushort usDeviceID, ushort usType,
                     uchar ucSn);
SS1BBE_RcvParamSetGUIDlg::SS1BBE_RcvParamSetGUIDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SS1BBE_RcvParamSetGUIDlg)
{
    ui->setupUi(this);
    m_pStateMgr = NULL;
    m_pILxTsspMessageManager = NULL;
    m_pILxTsspLogManager = NULL;
    m_pPublicInfo = NULL;
    m_pHAManager = NULL;
    m_targetNum_usedInMacro = -1;
    m_targetNum_usedInParam = -1;
    m_MCSSSRevStatus = NULL;
    m_pMCSTargetSysStatus = NULL;
    m_nTargetNum = 0;
    m_timer1 = new QTimer(this);
    connect(m_timer1, SIGNAL(timeout()), this, SLOT(updateRadioButton()));
    setAttribute(Qt::WA_DeleteOnClose);
}

SS1BBE_RcvParamSetGUIDlg::~SS1BBE_RcvParamSetGUIDlg()
{
    killTimer(m_TimerID);
    m_timer1->stop();
    delete m_timer1;
    delete ui;
}
int SS1BBE_RcvParamSetGUIDlg::msgBoxFunction(QMessageBox &msgBox, QString msg, BOOL ret)
{
    msgBox.setText(msg);
    msgBox.exec();
    return ret;
}
void SS1BBE_RcvParamSetGUIDlg::setText(const QString& strTitle, ParaStruct xmlsetText[5][60], QString MapMean[50][30], int MapNum[50], uchar Type, ILxTsspParameterMacro* pMacroObj)
{
    m_strTitle = strTitle;
    setWindowTitle(strTitle);

    memcpy(m_DlgParaSetStruct, xmlsetText, sizeof(ParaStruct) * 5 * 60);

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
void SS1BBE_RcvParamSetGUIDlg::InitDLG(int Type, ILxTsspParameterMacro* pMacroObj)
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
            DWORD stLen = sizeof(SMCU::MCSSSRevStatusStruct4);
            m_MCSSSRevStatus = (SMCU::MCSSSRevStatusStruct4*)m_pStateMgr->FindOneItem(m_StatusParaMCSSSRevID, stLen);
            if(m_MCSSSRevStatus == NULL || stLen != sizeof(SMCU::MCSSSRevStatusStruct4))
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
            pMacroObj->GetParameterBlock(m_ParaMCSSSRevID, byteArray);
            memcpy(&m_MCSSSRevParam, byteArray.data(), sizeof(SMCU::MCSSSRevParamStruct4));
        }
        ui->pushButton_Modify->setVisible(false);
        ui->pushButton_Set->setVisible(false);
        m_pCommFunc = CCommFunc::getInstance();
        m_timer1->start(500);
        //显示参数
        ShowPara(1);
    }
}

//设置输入格式
void SS1BBE_RcvParamSetGUIDlg::InitInput()
{
    //正整数
    QRegExp regExp_CodeBnDE("[0-9]{0,3}(\\.[0-9])?");
    QRegExp regExp_PowerRatio("(\\-)?[0-9]{0,2}(\\.[0-9])?");
    //功率比
    ui->lineEdit_PowerRatio->setValidator(new QRegExpValidator(regExp_PowerRatio, this));
    //码环带宽
    ui->lineEdit_CodeBnDE->setValidator(new QRegExpValidator(regExp_CodeBnDE, this));
    //捕获门限
    QRegExp regExp_CapThreshold("[0-9]{2,3}");
    ui->lineEdit_CapThreshold->setValidator(new QRegExpValidator(regExp_CapThreshold, this));
}

void SS1BBE_RcvParamSetGUIDlg::ShowControls()
{
    //向下拉框控件中写入从xml中读取的内容
    int i;
    //接收通道
    for(i=0; i<DlgMapLineNum[0];i++)
    {
        ui->comboBox_RecvChannel->addItem(m_DlgMapMean[0][i]);
    }
    ui->comboBox_RecvChannel->setCurrentIndex(m_DlgParaSetStruct[0][0].InitVal.lval);
    //AGC时间常数
    for(i=0; i<DlgMapLineNum[1];i++)
    {
        ui->comboBox_AGCTime->addItem(m_DlgMapMean[1][i]);
    }
    ui->comboBox_AGCTime->setCurrentIndex(m_DlgParaSetStruct[0][1].InitVal.lval);
    //载波环路带宽
    for(i=0; i<DlgMapLineNum[2];i++)
    {
        ui->comboBox_CodeBn->addItem(m_DlgMapMean[2][i]);
    }
    ui->comboBox_CodeBn->setCurrentIndex(m_DlgParaSetStruct[0][2].InitVal.lval);

    showTargetParam(0);
    //默认选择目标1
    ui->radioButton_Target1->setChecked(true);

    //显示静态框控件上的范围
    QString strTmp;
    //码环带宽
    strTmp = "码环带宽[" + QString::number(m_DlgParaSetStruct[1][2].MinVal.fmin, 'f', 1) + "~"
            + QString::number(m_DlgParaSetStruct[1][2].MaxVal.fmax) + "]";
    ui->label_CodeBnDE->setText(strTmp);
    //捕获门限
    strTmp = "捕获门限[" + QString::number(m_DlgParaSetStruct[1][3].MinVal.lmin) + "~"
            + QString::number(m_DlgParaSetStruct[1][3].MaxVal.lmax) + "]";
    ui->label_CapThreshold->setText(strTmp);
}

//显示参数
void SS1BBE_RcvParamSetGUIDlg::ShowPara(int type)
{
    if(type == 0)       //参数设置
    {
        //码同步
        if(m_MCSSSRevStatus != NULL)
        {
            m_MCSSSRevParam.MCSSSRevParam = m_MCSSSRevStatus->SSRevStatu.tParams;
            for(int i = 0; i < MAX_TARGET_COUNT; i++)
            {
                m_MCSSSRevParam.MCSSSRevParamTarget[i] = m_MCSSSRevStatus->targetSSRevStatu[i].tParams;
            }
        }
        else
        {
            DWORD stLen = sizeof(SMCU::MCSSSRevStatusStruct4);
            if(m_pStateMgr != NULL)
                m_MCSSSRevStatus  = (SMCU::MCSSSRevStatusStruct4*)m_pStateMgr->FindOneItem(m_StatusParaMCSSSRevID, stLen);
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

    //    if (m_MCSSSRevStatus == NULL)
    //        return;

    /********************************公共参数设置*********************************/
    //接收通道
    int nCurSel = m_MCSSSRevParam.MCSSSRevParam.ucRevChannel;
    if (nCurSel > 0)
        ui->comboBox_RecvChannel->setCurrentIndex(nCurSel - 1);
    //AGC时间常数
    nCurSel = m_MCSSSRevParam.MCSSSRevParam.ucAgcTime;
    if (nCurSel > 0)
        ui->comboBox_AGCTime->setCurrentIndex(nCurSel - 1);
    //载波环路带宽
    nCurSel = m_MCSSSRevParam.MCSSSRevParam.ucPllBw;
    if (nCurSel > 0)
        ui->comboBox_CodeBn->setCurrentIndex(nCurSel - 1);
    /********************************公共参数设置*********************************/
    showTargetParam(m_nTargetNum, 1);
}

//设置帧长范围
void SS1BBE_RcvParamSetGUIDlg::SetFrameLenRange(int iWordLen)
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

void SS1BBE_RcvParamSetGUIDlg::SetObjInfo(TLxTsspObjectInfo ObjectInfo)
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
        //码同步
        m_ParaMCSSSRevID = GetID(sItem.usStationID,
                                 sItem.ucSubSysID, sItem.usDeviceID,sItem.usTID,
                                 sItem.ucSN);
        /*************************框架**********************************/
        /*************************状态**********************************/
        //码同步
        m_StatusParaMCSSSRevID = GetID(sItem.usStationID,
                                       sItem.ucSubSysID, sItem.usDeviceID,sItem.usStateType,
                                       sItem.ucSN);
        /*************************状态**********************************/
    }

    //获取目标参数的ID
    ITEM *pItemCom = m_DeviceMap.getItem(COMM_SS1BBE_NAME);
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
void SS1BBE_RcvParamSetGUIDlg::EnabledControls(bool bFlag)
{
    /*************************Group1*************************/
    //接受通道
    ui->comboBox_RecvChannel->setEnabled(bFlag);
    //AGC时间常数
    ui->comboBox_AGCTime->setEnabled(bFlag);
    //载波环路带宽
    ui->comboBox_CodeBn->setEnabled(bFlag);
    //扩频码速率
    ui->comboBox_ExpandFrRate->setEnabled(bFlag);
    //功率比
    ui->lineEdit_PowerRatio->setEnabled(bFlag);
    //码环带宽（双边）
    ui->lineEdit_CodeBnDE->setEnabled(bFlag);
    //捕获门限
    ui->lineEdit_CapThreshold->setEnabled(bFlag);
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
int SS1BBE_RcvParamSetGUIDlg::SaveToMacro(ILxTsspParameterMacro* pMacroObj)
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

        //码同步参数
        byteArray.resize(sizeof(m_MCSSSRevParam));
        memcpy(byteArray.data(), &m_MCSSSRevParam, sizeof(m_MCSSSRevParam));
        pMacroObj->UpdateParameterBlock(m_ParaMCSSSRevID, byteArray);

        //日志信息
        QString strLog;
        strLog += QString("%1: 保存宏成功！").arg(MODULENAME);
        CLog::addLog(strLog);
    }
}
//从界面取参数
void SS1BBE_RcvParamSetGUIDlg::GetPara()
{
    //接收通道
    m_MCSSSRevParam.MCSSSRevParam.ucRevChannel = ui->comboBox_RecvChannel->currentIndex() + 1;
    //AGC时间常数
    m_MCSSSRevParam.MCSSSRevParam.ucAgcTime = ui->comboBox_AGCTime->currentIndex() + 1;
    //载波环路带宽
    m_MCSSSRevParam.MCSSSRevParam.ucPllBw = ui->comboBox_CodeBn->currentIndex() + 1;

    getParamTarget();
}

//验证参数范围
int SS1BBE_RcvParamSetGUIDlg::CheckPara()
{
    QMessageBox msgBox;
    QString strTmp;
    msgBox.setWindowTitle(MODULENAME);
    QString powerRatio = ui->lineEdit_PowerRatio->text();
    QString capThreshold = ui->lineEdit_CapThreshold->text();
    /***********************************码同步****************************/
    //码环带宽
    bool suc = false;
    int codeBnDE = (int)(ui->lineEdit_CodeBnDE->text().toDouble(&suc) * 10);
    if(ui->lineEdit_CodeBnDE->text().isEmpty())
    {
        strTmp = QString("码环带宽范围不能为空，请重新输入!");
        goto flag;
    }
    if (!suc)
    {
        strTmp = QString("码环带宽，请输入有效数字!");
        goto flag;
    }
    if((ui->lineEdit_CodeBnDE->text().toDouble()) < m_DlgParaSetStruct[1][2].MinVal.fmin
            || (ui->lineEdit_CodeBnDE->text().toDouble()) > m_DlgParaSetStruct[1][2].MaxVal.fmax)
    {
        strTmp = QString("码环带宽范围[%1～%2]Hz超出范围，请重新输入!")
                .arg(m_DlgParaSetStruct[1][2].MinVal.fmin).arg(m_DlgParaSetStruct[1][2].MaxVal.fmax);
        goto flag;
    }
    if(codeBnDE % 2 != 0)
    {
        strTmp = QString("码环带宽必须是0.2的整数倍!");
        goto flag;
    }

    //捕获门限
    if(capThreshold.isEmpty())
    {
        strTmp = QString("捕获门限不能为空，请重新输入!");
        goto flag;
    }
    if(capThreshold.toInt() < m_DlgParaSetStruct[1][3].MinVal.lmin
            || capThreshold.toInt() > m_DlgParaSetStruct[1][3].MaxVal.lmax)
    {
        strTmp = QString("捕获门限[%1～%2]dBHz超出范围，请重新输入!")
                .arg(m_DlgParaSetStruct[1][3].MinVal.lmin).arg(m_DlgParaSetStruct[1][3].MaxVal.lmax);
        goto flag;
    }
    //功率比
    if(powerRatio.isEmpty())
    {
        strTmp = QString("功率比不能为空，请重新输入!");
        goto flag;
    }
    if(powerRatio.toFloat() < m_DlgParaSetStruct[1][1].MinVal.lmin
            || powerRatio.toFloat() > m_DlgParaSetStruct[1][1].MaxVal.lmax)
    {
        strTmp = QString("功率比[%1～%2]dB超出范围，请重新输入!")
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
void SS1BBE_RcvParamSetGUIDlg::timerEvent(QTimerEvent *event)
{
    ShowPara(0);
}

//更改参数按钮休息处理函数
void SS1BBE_RcvParamSetGUIDlg::on_pushButton_Modify_clicked()
{
    if(ui->pushButton_Modify->text() == "更改")
    {
        EnabledControls(true);
        killTimer(m_TimerID);
        return;
    }

    if(ui->pushButton_Modify->text() == "恢复")
    {
        EnabledControls(false);
        m_TimerID = startTimer(500);
        return;
    }
}

void SS1BBE_RcvParamSetGUIDlg::on_pushButton_Set_clicked()
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
                      SMCU::MCSSSRevParamStruct sys;//公共参数
                         struct{USHORT tmp1;//占位
                                SMCU::MCSTargetTMSSRevParamStruct targetSysParam;//目标参数
                               }param[MAX_TARGET_COUNT];

                  }data;

            QByteArray byteArray;
            byteArray.resize(sizeof(data));

            data.target = 1 | 0x02 | 0x04 | 0x08 | 0x10;

            data.sys = m_MCSSSRevParam.MCSSSRevParam;
            for(int i = 0; i < MAX_TARGET_COUNT; ++i)
            {
                data.param[i].targetSysParam = m_MCSSSRevParam.MCSSSRevParamTarget[i];
            }


            memcpy(byteArray.data(),&data,sizeof(data));
            SendMessage((char*)byteArray.data(), byteArray.size(), m_usC, m_usTID);

            /*byteArray.resize(sizeof(m_MCSSSRevParam));
            memcpy(byteArray.data(),&m_MCSSSRevParam,sizeof(SMCU::MCSSSRevParamStruct4));
            SendMessage((char*)&m_MCSSSRevParam, sizeof(SMCU::MCSSSRevParamStruct4), m_usC, m_usTID);*/
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

void SS1BBE_RcvParamSetGUIDlg::closeEvent(QCloseEvent *event)
{
    event->accept();
}

QSize SS1BBE_RcvParamSetGUIDlg::sizeHint() const
{
    return QSize(this->width(), this->height());
}

/*
//副帧方式响应函数
void SS1BBE_RcvParamSetGUIDlg::on_comboBox_SubFrmType_currentIndexChanged(int index)
{
//    //副帧方式控件使能
//    if (ui->pushButton_Modify->text() != "更改" || iParaSetFlag != 0)
//    {
//        EnabledControlsSubFrmType(index);   //副帧方式控件使能
//    }
}

//根据副帧类型，使能副帧控件
void SS1BBE_RcvParamSetGUIDlg::EnabledControlsSubFrmType(int iSubFrm)
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

void SS1BBE_RcvParamSetGUIDlg::SubFrmEn(bool bFlag)
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
void SS1BBE_RcvParamSetGUIDlg::IDSubFrmEn(bool bFlag)
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
void SS1BBE_RcvParamSetGUIDlg::CycSubFrmEn(bool bFlag)
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
void SS1BBE_RcvParamSetGUIDlg::ReserveSubFrmEn(bool bFlag)
{

}

void SS1BBE_RcvParamSetGUIDlg::SendMessage(char* pData, unsigned short  usLen, unsigned short usC,unsigned short usTID)
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

DWORDLONG SS1BBE_RcvParamSetGUIDlg::StrToSynCode(const char * pStr)
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

void SS1BBE_RcvParamSetGUIDlg::showTargetParam(int nIndex, int nType)
{
    int i;
    //向下拉框控件中写入从xml中读取的内容
    if (nType == 0)
    {
        //扩频码速率
        for(i=0; i<DlgMapLineNum[nIndex * 1 + 3];i++)
        {
            ui->comboBox_ExpandFrRate->addItem(m_DlgMapMean[nIndex * 1 + 3][i]);
        }
        ui->comboBox_ExpandFrRate->setCurrentIndex(m_DlgParaSetStruct[nIndex + 1][0].InitVal.lval);
        //功率比
        ui->lineEdit_PowerRatio->setText(QString::number(m_DlgParaSetStruct[nIndex + 1][1].InitVal.lval));
        //码环带宽
        ui->lineEdit_CodeBnDE->setText(QString::number(m_DlgParaSetStruct[nIndex + 1][2].InitVal.lval));
        ui->label_CodeBnDE->setText("码环带宽[" + QString::number(m_DlgParaSetStruct[nIndex + 1][2].MinVal.fmin) +
                "~" + QString::number(m_DlgParaSetStruct[nIndex + 1][2].MaxVal.fmax) + "]");
        //捕获门限
        ui->lineEdit_CapThreshold->setText(QString::number(m_DlgParaSetStruct[nIndex + 1][3].InitVal.lval));
        ui->label_CapThreshold->setText("捕获门限[" + QString::number(m_DlgParaSetStruct[nIndex + 1][3].MinVal.lmin) +
                "~" + QString::number(m_DlgParaSetStruct[nIndex + 1][3].MaxVal.lmax) + "]");

    }
    else if (nType == 1)
    {
        //扩频码速率
        int nCurSel = m_MCSSSRevParam.MCSSSRevParamTarget[nIndex].ucPNRate;
        if (nCurSel > 0)
            ui->comboBox_ExpandFrRate->setCurrentIndex(nCurSel - 1);
        //功率比
        ui->lineEdit_PowerRatio->setText(QString::number(m_MCSSSRevParam.MCSSSRevParamTarget[nIndex].siRatiodB / 10.0));
        //码环带宽
        ui->lineEdit_CodeBnDE->setText(QString::number(m_MCSSSRevParam.MCSSSRevParamTarget[nIndex].usPNPIIBw / 5.0));
        //捕获门限
        ui->lineEdit_CapThreshold->setText(QString::number(m_MCSSSRevParam.MCSSSRevParamTarget[nIndex].ucCN0));
    }
}

void SS1BBE_RcvParamSetGUIDlg:: getParamTarget()
{
    //扩频码速率
    m_MCSSSRevParam.MCSSSRevParamTarget[m_nTargetNum].ucPNRate =ui->comboBox_ExpandFrRate->currentIndex()+ 1;
    //功率比
    m_MCSSSRevParam.MCSSSRevParamTarget[m_nTargetNum].siRatiodB = ui->lineEdit_PowerRatio->text().toDouble() * 10;
    //码环带宽
    m_MCSSSRevParam.MCSSSRevParamTarget[m_nTargetNum].usPNPIIBw = ui->lineEdit_CodeBnDE->text().toDouble() * 5;
    //捕获门限
    m_MCSSSRevParam.MCSSSRevParamTarget[m_nTargetNum].ucCN0 = ui->lineEdit_CapThreshold->text().toInt();
}

int SS1BBE_RcvParamSetGUIDlg::getSharedMemoryTargetNum()
{
    DWORD stLen = sizeof(SMCU::MCSSysTarget4StatusStruct);
    if(m_pStateMgr != NULL)
        m_pMCSTargetSysStatus  = (SMCU::MCSSysTarget4StatusStruct*)m_pStateMgr->FindOneItem(m_CommTargetID, stLen);

    if (m_pMCSTargetSysStatus != NULL)
        return m_pMCSTargetSysStatus->sysStatu.tParams.ucTargetNum;

    return 0;
}

void SS1BBE_RcvParamSetGUIDlg::showRadioButton()
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
        break;
    }
}

void SS1BBE_RcvParamSetGUIDlg::on_radioButton_Target1_clicked()
{
    if(1 != CheckPara())
    {
        set_radioButton_Target();
        return;
    }
    GetPara();
    m_nTargetNum = 0;
    int nType = ui->pushButton_Set->isEnabled()  ? 0 : 1;
    showTargetParam(0, 1);
}

void SS1BBE_RcvParamSetGUIDlg::on_radioButton_Target2_clicked()
{
    if(1 != CheckPara())
    {
        set_radioButton_Target();
        return;
    }
    GetPara();
    m_nTargetNum = 1;
    int nType = ui->pushButton_Set->isEnabled()  ? 0 : 1;
    showTargetParam(1, 1);
}

void SS1BBE_RcvParamSetGUIDlg::on_radioButton_Target3_clicked()
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

void SS1BBE_RcvParamSetGUIDlg::on_radioButton_Target4_clicked()
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

void SS1BBE_RcvParamSetGUIDlg::updateRadioButton()
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
void SS1BBE_RcvParamSetGUIDlg::set_radioButton_Target()
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
