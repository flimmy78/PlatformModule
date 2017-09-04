#include "SS2BBE_RcvParamSetGUIDlg.h"
#include "ui_SS2BBE_RcvParamSetGUIDlg.h"
#include <QCloseEvent>
#include <QMessageBox>
#include <QDateTime>
#include "../../Common/CommonFunction.h"
#include <QTimer>
#pragma pack(1)
extern quint64 GetID(ushort usStationID, uchar ucSubSysID, ushort usDeviceID, ushort usType,
                     uchar ucSn);
SS2BBE_RcvParamSetGUIDlg::SS2BBE_RcvParamSetGUIDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SS2BBE_RcvParamSetGUIDlg)
{
    ui->setupUi(this);
    m_pStateMgr = NULL;
    m_pILxTsspMessageManager = NULL;
    m_pILxTsspLogManager = NULL;
    m_pPublicInfo = NULL;
    m_pHAManager = NULL;

    m_MCSSSRevStatus = NULL;
    m_pMCSTargetSysStatus = NULL;
    m_currentNumber_usedByParam = m_currentNumber_usedByMacro = -1;
    m_nTargetNum = 0;
    m_timer1 = new QTimer(this);
    connect(m_timer1, SIGNAL(timeout()), this, SLOT(updateRadioButton()));
    setAttribute(Qt::WA_DeleteOnClose);
}

SS2BBE_RcvParamSetGUIDlg::~SS2BBE_RcvParamSetGUIDlg()
{
    killTimer(m_TimerID);
    m_timer1->stop();
    delete m_timer1;
    delete ui;
}

void SS2BBE_RcvParamSetGUIDlg::setText(const QString& strTitle, ParaStruct xmlsetText[5][60], QString MapMean[50][30], int MapNum[50], uchar Type, ILxTsspParameterMacro* pMacroObj)
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
    for(int i = 0; i < 49; i++)
    {
        DlgMapLineNum[i] = MapNum[i];
    }
    //初始化界面
    InitDLG(Type, pMacroObj);
}

//初始化对话框
void SS2BBE_RcvParamSetGUIDlg::InitDLG(int Type, ILxTsspParameterMacro* pMacroObj)
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
            DWORD stLen = sizeof(SMCU2::MCSRevStatusStruct4);
            m_MCSSSRevStatus  = (SMCU2::MCSRevStatusStruct4*)m_pStateMgr->FindOneItem(m_StatusParaMCSSSRevID, stLen);
            if(m_MCSSSRevStatus == NULL || stLen != sizeof(SMCU2::MCSRevStatusStruct4))
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
            //参数
            pMacroObj->GetParameterBlock(m_ParaMCSSSRevID, byteArray);
            memcpy(&m_MCSSSRevParam, byteArray.data(), sizeof(m_MCSSSRevParam));
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
void SS2BBE_RcvParamSetGUIDlg::InitInput()
{

    QRegExp regExp_CapThreshold("[0-9]{2,3}");
    QRegExp regExp_CodeBnDE("[0-9]{0,3}(\\.[0-9])?");
    ui->lineEdit_CapThresholdM->setValidator(new QRegExpValidator(regExp_CapThreshold, this));
    ui->lineEdit_CapThresholdT->setValidator(new QRegExpValidator(regExp_CapThreshold, this));
    ui->lineEdit_CodeBnDEM->setValidator(new QRegExpValidator(regExp_CodeBnDE, this));
    ui->lineEdit_CodeBnDET->setValidator(new QRegExpValidator(regExp_CodeBnDE, this));
}

void SS2BBE_RcvParamSetGUIDlg::ShowControls()
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
    strTmp = "码环带宽\n[" + QString::number(m_DlgParaSetStruct[1][1].MinVal.fmin, 'f', 1) + "~"
            + QString::number(m_DlgParaSetStruct[1][1].MaxVal.fmax, 'f', 1) + "]";
    ui->label_CodeBnDEM->setText(strTmp);
    //捕获门限
    strTmp = "捕获门限\n[" + QString::number(m_DlgParaSetStruct[1][2].MinVal.lmin) + "~"
            + QString::number(m_DlgParaSetStruct[1][2].MaxVal.lmax) + "]";
    ui->label_CapThresholdM->setText(strTmp);

    //码环带宽
    strTmp = "码环带宽\n[" + QString::number(m_DlgParaSetStruct[1][1].MinVal.fmin, 'f', 1) + "~"
            + QString::number(m_DlgParaSetStruct[1][4].MaxVal.fmax, 'f', 1) + "]";
    ui->label_CodeBnDET->setText(strTmp);
    //捕获门限
    strTmp = "捕获门限\n[" + QString::number(m_DlgParaSetStruct[1][2].MinVal.lmin) + "~"
            + QString::number(m_DlgParaSetStruct[1][5].MaxVal.lmax) + "]";
    ui->label_CapThresholdT->setText(strTmp);
}

//显示参数
void SS2BBE_RcvParamSetGUIDlg::ShowPara(int type)
{
    if(type == 0)       //参数设置
    {
        //公共参数
        if(m_MCSSSRevStatus != NULL)
        {
            m_MCSSSRevParam.sysPara = m_MCSSSRevStatus->sysStatus.tParams;
            for (int i = 0; i < MAX_TARGET_COUNT; i++)
            {
                m_MCSSSRevParam.revParam[i].recvTMPara = m_MCSSSRevStatus->revStatu[i].recvTMStatus.tParams;
                m_MCSSSRevParam.revParam[i].recvMeasPara = m_MCSSSRevStatus->revStatu[i].recvMeasStatus.tParams;
            }
        }
        else
        {
            DWORD stLen = sizeof(SMCU2::MCSRevStatusStruct4);
            if(m_pStateMgr != NULL)
                m_MCSSSRevStatus  = (SMCU2::MCSRevStatusStruct4*)m_pStateMgr->FindOneItem(m_StatusParaMCSSSRevID, stLen);
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

    int nCurSel = m_MCSSSRevParam.sysPara.ucRevChannel;
    if (nCurSel > 0)
        ui->comboBox_RecvChannel->setCurrentIndex(nCurSel - 1);
    /********************************公共参数设置*********************************/
    showTargetParam(m_nTargetNum, 1);
    /********************************参数设置*********************************/
}



//设置帧长范围
void SS2BBE_RcvParamSetGUIDlg::SetFrameLenRange(int iWordLen)
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

void SS2BBE_RcvParamSetGUIDlg::SetObjInfo(TLxTsspObjectInfo ObjectInfo)
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
        m_ParaMCSSSRevID = GetID(sItem.usStationID,
                                 sItem.ucSubSysID, sItem.usDeviceID,sItem.usTID,
                                 sItem.ucSN);
        /*************************框架**********************************/

        /*************************状态**********************************/
        //公共参数
        m_StatusParaMCSSSRevID = GetID(sItem.usStationID,
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
void SS2BBE_RcvParamSetGUIDlg::EnabledControls(bool bFlag)
{
    /*************************Group1*************************/
    //接受通道
    ui->comboBox_RecvChannel->setEnabled(bFlag);
    //AGC时间常数
    ui->comboBox_AGCTime->setEnabled(bFlag);
    //载波环路带宽
    ui->comboBox_CodeBn->setEnabled(bFlag);
    //测量支路扩频码速率
    ui->comboBox_ExpandFrRateM->setEnabled(bFlag);
    //测量支路码环带宽
    ui->lineEdit_CodeBnDEM->setEnabled(bFlag);
    //测量支路捕获门限
    ui->lineEdit_CapThresholdM->setEnabled(bFlag);
    //遥测支路扩频码速率
    ui->comboBox_ExpandFrRateT->setEnabled(bFlag);
    //遥测支路码环带宽
    ui->lineEdit_CodeBnDET->setEnabled(bFlag);
    //遥测支路捕获门限
    ui->lineEdit_CapThresholdT->setEnabled(bFlag);
    /*************************Group1*************************/

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
int SS2BBE_RcvParamSetGUIDlg::SaveToMacro(ILxTsspParameterMacro* pMacroObj)
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

        //参数
        byteArray.resize(sizeof(m_MCSSSRevParam));
        memcpy(byteArray.data(),&m_MCSSSRevParam,sizeof(m_MCSSSRevParam));
        pMacroObj->UpdateParameterBlock(m_ParaMCSSSRevID, byteArray);

        //日志信息
        QString strLog;
        strLog += "数传解调参数设置: 保存宏成功！";
        CLog::addLog(strLog);
    }
}
//从界面取参数
void SS2BBE_RcvParamSetGUIDlg::GetPara()
{
    m_MCSSSRevParam.sysPara.ucRevChannel=ui->comboBox_RecvChannel->currentIndex() +1;
    getParamTarget();
}

//验证参数范围
int SS2BBE_RcvParamSetGUIDlg::CheckPara()
{
    QMessageBox msgBox;
    msgBox.setWindowTitle(MODULENAME);
    QString strTmp;
    bool okM = false, okT = false;
    QString codeBnMText = ui->lineEdit_CodeBnDEM->text();
    QString codeBnTText = ui->lineEdit_CodeBnDET->text();
    QString capThroldMText = ui->lineEdit_CapThresholdM->text();
    QString capThroldTText = ui->lineEdit_CapThresholdT->text();
    USHORT codeBnM = qRound(codeBnMText.toDouble(&okM) * 10.0);
    USHORT codeBnT = qRound(codeBnTText.toDouble(&okT) * 10.0);
    USHORT capThroldM = capThroldMText.toInt();
    USHORT capThroldT = capThroldTText.toInt();
    if (codeBnMText.isEmpty())
    {
        strTmp = QString("测量支路码环带宽不能为空！");
        goto flag;
    }
    if (codeBnTText.isEmpty())
    {
        strTmp = QString("遥测支路码环带宽不能为空！");
        goto flag;
    }
    if (capThroldMText.isEmpty())
    {
        strTmp = QString("测量支路捕获门限不能为空!");
        goto flag;
    }
    if (capThroldTText.isEmpty())
    {
        strTmp = QString("遥测支路捕获门限不能为空!");
        goto flag;
    }
    if (!okM)
    {
        strTmp = QString("测量支路码环带,请输入有效数字！");
        goto flag;
    }
    if (codeBnM < (USHORT)(m_DlgParaSetStruct[1][1].MinVal.fmin * 10)
            || codeBnM > (USHORT)(m_DlgParaSetStruct[1][1].MaxVal.fmax * 10))
    {
        strTmp = QString("测量支路码环带宽正确范围是%1～%2Hz！")
                .arg(m_DlgParaSetStruct[1][1].MinVal.fmin).arg(m_DlgParaSetStruct[1][1].MaxVal.fmax);
        goto flag;
    }
    if (!okT)
    {
        strTmp = QString("遥测支路码环带,请输入有效数字！");
        goto flag;
    }
    if (codeBnT < (USHORT)(m_DlgParaSetStruct[1][4].MinVal.fmin * 10)
            || codeBnT > (USHORT)(m_DlgParaSetStruct[1][4].MaxVal.fmax * 10))
    {
        strTmp = QString("遥测支路码环带宽正确范围是%1～%2Hz！")
                .arg(m_DlgParaSetStruct[1][4].MinVal.fmin).arg(m_DlgParaSetStruct[1][4].MaxVal.fmax);
        goto flag;
    }

    if (capThroldM < m_DlgParaSetStruct[1][2].MinVal.lmin
            || capThroldM > m_DlgParaSetStruct[1][2].MaxVal.lmax)
    {
        strTmp = QString("测量支路捕获门限正确范围是%1～%2dBHz")
                .arg(m_DlgParaSetStruct[1][2].MinVal.lmin).arg(m_DlgParaSetStruct[1][2].MaxVal.lmax);
        goto flag;
    }

    if (capThroldTText.isEmpty() || capThroldT < m_DlgParaSetStruct[1][5].MinVal.lmin
            || capThroldT > m_DlgParaSetStruct[1][5].MaxVal.lmax)
    {
        strTmp = QString("遥测支路捕获门限正确范围是%1～%2dBHz")
                .arg(m_DlgParaSetStruct[1][5].MinVal.lmin).arg(m_DlgParaSetStruct[1][5].MaxVal.lmax);
        goto flag;
    }
    if (qRound(codeBnMText.toDouble() * 10) % 2 != 0)
    {
        strTmp = ("测量支路码环带宽必须是0.2的整数倍！");
        goto flag;
    }
    if (qRound(codeBnTText.toDouble() * 10) % 2 != 0)
    {
        strTmp = ("遥测支路码环带宽必须是0.2的整数倍！");
        goto flag;
    }
    return 1;
flag:
    msgBox.setText(strTmp);
    msgBox.exec();
    return -1;
}

//定时器
void SS2BBE_RcvParamSetGUIDlg::timerEvent(QTimerEvent *event)
{
    ShowPara(0);
}

//更改参数按钮休息处理函数
void SS2BBE_RcvParamSetGUIDlg::on_pushButton_Modify_clicked()
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

void SS2BBE_RcvParamSetGUIDlg::on_pushButton_Set_clicked()
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
                USHORT t;
                SMCU2::MCSSSRevParamStruct ucRevChannel;
                struct{
                    USHORT tmp;
                    SMCU2::MCSTargetTMSSRevParamStruct recvTMPara;
                    USHORT tmp1;
                    SMCU2::MCSTargetMeasSSRevParamStruct recvMeasPara;
                }param[MAX_TARGET_COUNT];

            }data;
            data.target = 1 | 2 | 4 | 8 | 16;
            data.ucRevChannel = m_MCSSSRevParam.sysPara;
            for ( int i = 0; i < MAX_TARGET_COUNT; i++)
            {
                data.param[i].recvTMPara = m_MCSSSRevParam.revParam[i].recvTMPara;
                data.param[i].recvMeasPara = m_MCSSSRevParam.revParam[i].recvMeasPara;
            }
            QByteArray byteArray;
            byteArray.resize(sizeof(data));
            memcpy(byteArray.data(),&data,sizeof(data));
            SendMessage((char*)byteArray.data(), byteArray.size(), m_usC, m_usTID);
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

void SS2BBE_RcvParamSetGUIDlg::closeEvent(QCloseEvent *event)
{
    event->accept();
}

QSize SS2BBE_RcvParamSetGUIDlg::sizeHint() const
{
    return QSize(this->width(), this->height());
}

/*
//副帧方式响应函数
void SS2BBE_RcvParamSetGUIDlg::on_comboBox_SubFrmType_currentIndexChanged(int index)
{
//    //副帧方式控件使能
//    if (ui->pushButton_Modify->text() != "更改" || iParaSetFlag != 0)
//    {
//        EnabledControlsSubFrmType(index);   //副帧方式控件使能
//    }
}

//根据副帧类型，使能副帧控件
void SS2BBE_RcvParamSetGUIDlg::EnabledControlsSubFrmType(int iSubFrm)
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

void SS2BBE_RcvParamSetGUIDlg::SubFrmEn(bool bFlag)
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
void SS2BBE_RcvParamSetGUIDlg::IDSubFrmEn(bool bFlag)
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
void SS2BBE_RcvParamSetGUIDlg::CycSubFrmEn(bool bFlag)
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
void SS2BBE_RcvParamSetGUIDlg::ReserveSubFrmEn(bool bFlag)
{

}

void SS2BBE_RcvParamSetGUIDlg::SendMessage(char* pData, unsigned short  usLen, unsigned short usC, unsigned short usTID)
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

void SS2BBE_RcvParamSetGUIDlg::showTargetParam(int nIndex, int nType)
{
    int i;
    //向下拉框控件中写入从xml中读取的内容
    if (nType == 0)
    {
        //扩频码速率
        ui->comboBox_ExpandFrRateM->clear();
        for(i=0; i<DlgMapLineNum[nIndex * 1 + 3];i++)
        {

            ui->comboBox_ExpandFrRateM->addItem(m_DlgMapMean[nIndex * 1 + 3][i]);
        }
        ui->comboBox_ExpandFrRateM->setCurrentIndex(m_DlgParaSetStruct[nIndex + 1][0].InitVal.lval);
        //码环带宽
        ui->lineEdit_CodeBnDEM->setText(QString::number(m_DlgParaSetStruct[nIndex + 1][1].InitVal.lval));
        //捕获门限
        ui->lineEdit_CapThresholdM->setText(QString::number(m_DlgParaSetStruct[nIndex + 1][2].InitVal.lval));

        ui->comboBox_ExpandFrRateT->clear();
        for(i = 0; i<DlgMapLineNum[nIndex * 1 + 4];i++)
        {
            ui->comboBox_ExpandFrRateT->addItem(m_DlgMapMean[nIndex * 1 + 4][i]);
        }
        ui->comboBox_ExpandFrRateT->setCurrentIndex(m_DlgParaSetStruct[nIndex + 1][3].InitVal.lval);
        //码环带宽
        ui->lineEdit_CodeBnDET->setText(QString::number(m_DlgParaSetStruct[nIndex + 1][4].InitVal.lval));
        //捕获门限
        ui->lineEdit_CapThresholdT->setText(QString::number(m_DlgParaSetStruct[nIndex + 1][5].InitVal.lval));
    }
    else if (nType == 1)
    {
        //测量支路
        //伪码速率
        int nCurSel = m_MCSSSRevParam.revParam[nIndex].recvMeasPara.ucPNRate ;
        if (nCurSel > 0)
            ui->comboBox_ExpandFrRateM->setCurrentIndex(nCurSel - 1);
        //码环带宽
        ui->lineEdit_CodeBnDEM->setText(QString::number(m_MCSSSRevParam.revParam[nIndex].recvMeasPara.usPNPIIBw / 10.0, 'f', 1));
        //捕获门限
        ui->lineEdit_CapThresholdM->setText(QString::number(m_MCSSSRevParam.revParam[nIndex].recvMeasPara.ucCN0));

        nCurSel = m_MCSSSRevParam.revParam[nIndex].recvMeasPara.ucAgcTime ;
        if (nCurSel > 0)
            ui->comboBox_AGCTime->setCurrentIndex(nCurSel - 1);

        nCurSel = m_MCSSSRevParam.revParam[nIndex].recvMeasPara.ucPllBw ;
        if (nCurSel > 0)
            ui->comboBox_CodeBn->setCurrentIndex(nCurSel - 1);
        //遥测支路
        //伪码速率
        nCurSel = m_MCSSSRevParam.revParam[nIndex].recvTMPara.ucPNRate ;
        if (nCurSel > 0)
            ui->comboBox_ExpandFrRateT->setCurrentIndex(nCurSel - 1);
        //码环带宽
        ui->lineEdit_CodeBnDET->setText(QString::number(m_MCSSSRevParam.revParam[nIndex].recvTMPara.usPNPIIBw / 10.0, 'f', 1));
        //捕获门限
        ui->lineEdit_CapThresholdT->setText(QString::number(m_MCSSSRevParam.revParam[nIndex].recvTMPara.ucCN0));

        nCurSel = m_MCSSSRevParam.revParam[nIndex].recvTMPara.ucAgcTime ;
        if (nCurSel > 0)
            ui->comboBox_AGCTime->setCurrentIndex(nCurSel - 1);

        nCurSel = m_MCSSSRevParam.revParam[nIndex].recvTMPara.ucPllBw ;
        if (nCurSel > 0)
            ui->comboBox_CodeBn->setCurrentIndex(nCurSel - 1);

    }
}

void SS2BBE_RcvParamSetGUIDlg::getParamTarget()
{
    int nIndex = m_nTargetNum;

    //测量支路
    //伪码速率
    m_MCSSSRevParam.revParam[nIndex].recvMeasPara.ucPNRate = ui->comboBox_ExpandFrRateM->currentIndex() + 1;

    //码环带宽
    m_MCSSSRevParam.revParam[nIndex].recvMeasPara.usPNPIIBw = qRound(ui->lineEdit_CodeBnDEM->text().toDouble() * 10);

    //捕获门限
    m_MCSSSRevParam.revParam[nIndex].recvMeasPara.ucCN0 = ui->lineEdit_CapThresholdM->text().toInt();
    //AGC事件常数
    m_MCSSSRevParam.revParam[nIndex].recvMeasPara.ucAgcTime = ui->comboBox_AGCTime->currentIndex() + 1;
    //载波环路带宽
    m_MCSSSRevParam.revParam[nIndex].recvMeasPara.ucPllBw = ui->comboBox_CodeBn->currentIndex() + 1;

    //遥测支路
    //伪码速率
    m_MCSSSRevParam.revParam[nIndex].recvTMPara.ucPNRate = ui->comboBox_ExpandFrRateT->currentIndex() + 1;

    //码环带宽
    m_MCSSSRevParam.revParam[nIndex].recvTMPara.usPNPIIBw= qRound(ui->lineEdit_CodeBnDET->text().toDouble() * 10);

    //捕获门限
    m_MCSSSRevParam.revParam[nIndex].recvTMPara.ucCN0 = ui->lineEdit_CapThresholdT->text().toInt();
    for(int i = 0 ;i<4;i++)
    {
        //AGC事件常数
        m_MCSSSRevParam.revParam[i].recvTMPara.ucAgcTime = ui->comboBox_AGCTime->currentIndex() + 1;
        //载波环路带宽
        m_MCSSSRevParam.revParam[i].recvTMPara.ucPllBw = ui->comboBox_CodeBn->currentIndex() + 1;
    }

}

int SS2BBE_RcvParamSetGUIDlg::getSharedMemoryTargetNum()
{
    DWORD stLen = sizeof(SMCU2::MCSSysTarget4StatusStruct);
    if(m_pStateMgr != NULL)
        m_pMCSTargetSysStatus  = (SMCU2::MCSSysTarget4StatusStruct*)m_pStateMgr->FindOneItem(m_CommTargetID, stLen);

    if (m_pMCSTargetSysStatus != NULL)
        return m_pMCSTargetSysStatus->MCSSysStatu.tParams.ucTargetNum;

    return 0;
}

void SS2BBE_RcvParamSetGUIDlg::showRadioButton()
{
    int nNum = getSharedMemoryTargetNum();
    if(m_currentNumber_usedByParam == nNum)return;
    m_currentNumber_usedByParam = nNum;
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

DWORDLONG SS2BBE_RcvParamSetGUIDlg::StrToSynCode(const char * pStr)
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

void SS2BBE_RcvParamSetGUIDlg::on_radioButton_Target1_clicked()
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

void SS2BBE_RcvParamSetGUIDlg::on_radioButton_Target2_clicked()
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
void SS2BBE_RcvParamSetGUIDlg::on_radioButton_Target3_clicked()
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
void SS2BBE_RcvParamSetGUIDlg::on_radioButton_Target4_clicked()
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

void SS2BBE_RcvParamSetGUIDlg::updateRadioButton()
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
    switch( m_currentNumber_usedByMacro - 1)
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

void SS2BBE_RcvParamSetGUIDlg::set_radioButton_Target()
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
