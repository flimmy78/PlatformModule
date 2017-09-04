#include "SS1BBE_CjCsParamSetGUIDlg.h"
#include "ui_SS1BBE_CjCsParamSetGUIDlg.h"
#include <QCloseEvent>
#include <QMessageBox>
#include <QDateTime>
#include "../../Common/Log.h"
#include "../../Common/CommonFunction.h"
#include <QTimer>
#pragma pack(1)
extern quint64 GetID(ushort usStationID, uchar ucSubSysID, ushort usDeviceID, ushort usType,
                     uchar ucSn);
SS1BBE_CjCsParamSetGUIDlg::SS1BBE_CjCsParamSetGUIDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SS1BBE_CjCsParamSetGUIDlg)
{
    ui->setupUi(this);
    m_pStateMgr = NULL;
    m_pILxTsspMessageManager = NULL;
    m_pILxTsspLogManager = NULL;
    m_pPublicInfo = NULL;
    m_pHAManager = NULL;
    m_totalTargetNum = -1;
    m_totalTargetNum_inParam = -1;
    m_nTargetNum = 0;
    m_MCSSSCjStatus = NULL;

    m_pMCSTargetSysStatus = NULL;
    m_timer1 = new QTimer(this);
    connect(m_timer1, SIGNAL(timeout()), this, SLOT(updateRadioButton()));
    setAttribute(Qt::WA_DeleteOnClose);
}

SS1BBE_CjCsParamSetGUIDlg::~SS1BBE_CjCsParamSetGUIDlg()
{
    killTimer(m_TimerID);
    m_timer1->stop();
    delete m_timer1;
    delete ui;
}

void SS1BBE_CjCsParamSetGUIDlg::setText(const QString& strTitle, ParaStruct xmlsetText[4][60], QString MapMean[50][30], int MapNum[50], uchar Type, ILxTsspParameterMacro* pMacroObj)
{
    m_strTitle = strTitle;
    setWindowTitle(strTitle);

    memcpy(m_DlgParaSetStruct, xmlsetText, sizeof(ParaStruct) * 60 * 4);

    for(int i=0; i < 50; i++)
    {
        for(int j=0; j < 30; j++)
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
void SS1BBE_CjCsParamSetGUIDlg::InitDLG(int Type, ILxTsspParameterMacro* pMacroObj)
{
    iParaSetFlag = Type;
    //设置输入格式
    InitInput();
    //显示变量名字和范围
    ShowControls();

    if(Type == 0)//取当前参数
    {
        if(m_pStateMgr != NULL)
        {
            DWORD stLen = sizeof(SMCU::MCSSSCjStatusStruct4);
            m_MCSSSCjStatus = (SMCU::MCSSSCjStatusStruct4*)m_pStateMgr->FindOneItem(m_StatusParaMCSSSCjID, stLen);
            if(m_MCSSSCjStatus == NULL || stLen != sizeof(SMCU::MCSSSCjStatusStruct4))
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
            pMacroObj->GetParameterBlock(m_ParaMCSSSCjID, byteArray);
            memcpy(&m_MCSSSCjParam, byteArray.data(), sizeof(SMCU::MCSSSCjParamStruct4));
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
void SS1BBE_CjCsParamSetGUIDlg::InitInput()
{

    //距离预报值
    QRegExp regExp_DisFor("[0-9]{0,6}(\\.[0-9]{1,3})");
    ui->lineEdit_DisFor->setValidator(new QRegExpValidator(regExp_DisFor, this));
    //应答机距离零值
    QRegExp regExp_ResponsorZeroPos("[0-9]{0,7}(\\.[0-9]{1,3})");
    ui->lineEdit_ResponsorZeroPos->setValidator(new QRegExpValidator(regExp_ResponsorZeroPos, this));
}

void SS1BBE_CjCsParamSetGUIDlg::ShowControls()
{
    showTargetParam(0);
    //默认选择目标1
    ui->radioButton_Target1->setChecked(true);
}

//显示参数
void SS1BBE_CjCsParamSetGUIDlg::ShowPara(int type)
{
    if(type == 0)       //参数设置
    {
        if(m_MCSSSCjStatus != NULL)
        {
            for (int i = 0; i < MAX_TARGET_COUNT; i++)
                m_MCSSSCjParam.SSCjParam[i] = m_MCSSSCjStatus->SSCjStatu[i].tParams;
        }
        else
        {
            DWORD stLen = sizeof(SMCU::MCSSSCjStatusStruct4);
            if(m_pStateMgr != NULL)
                m_MCSSSCjStatus  = (SMCU::MCSSSCjStatusStruct4*)m_pStateMgr->FindOneItem(m_StatusParaMCSSSCjID, stLen);
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

    //    if (m_MCSSSCjStatus == NULL)
    //        return;
    //显示目标的数据所
    //for (int i = 0; i < MAX_TARGET_NUM; i++)
    //    showTargetParam(i, 1);
    showTargetParam(m_nTargetNum, 1);
}

//设置帧长范围
void SS1BBE_CjCsParamSetGUIDlg::SetFrameLenRange(int iWordLen)
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

void SS1BBE_CjCsParamSetGUIDlg::SetObjInfo(TLxTsspObjectInfo ObjectInfo)
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
    if (pItem != NULL && pItem->vSubItem.size() > 0)
    {
        SUBITEM& sItem = *(pItem->getSubItemByDeviceID_SN(m_ObjectInfo.usLocalDID, m_ObjectInfo.ucSN));
        m_usC = sItem.usCMD;
        m_usTID = sItem.usTID;
        /*************************框架**********************************/
        m_ParaMCSSSCjID = GetID(sItem.usStationID,
                                sItem.ucSubSysID, sItem.usDeviceID,sItem.usTID,
                                sItem.ucSN);
        /*************************框架**********************************/

        /*************************状态**********************************/
        m_StatusParaMCSSSCjID = GetID(sItem.usStationID,
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
void SS1BBE_CjCsParamSetGUIDlg::EnabledControls(bool bFlag)
{
    /*************************Group1*************************/
    //距离解模糊
    ui->comboBox_DisAmbi->setEnabled(bFlag);
    //距离预报值
    ui->lineEdit_DisFor->setEnabled(bFlag);
    //距离预报方式
    ui->comboBox_DisForWay->setEnabled(bFlag);
    //应答机零值距离
    ui->lineEdit_ResponsorZeroPos->setEnabled(bFlag);
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
int SS1BBE_CjCsParamSetGUIDlg::SaveToMacro(ILxTsspParameterMacro* pMacroObj)
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
        byteArray.resize(sizeof(m_MCSSSCjParam));
        memcpy(byteArray.data(), &m_MCSSSCjParam, sizeof(m_MCSSSCjParam));
        pMacroObj->UpdateParameterBlock(m_ParaMCSSSCjID, byteArray);

        //日志信息
        QString strLog;
        strLog += QString("%1: 保存宏成功！").arg(MODULENAME);
        CLog::addLog(strLog);
    }
}

//从界面取参数
void SS1BBE_CjCsParamSetGUIDlg::GetPara()
{
    getParamTarget();
}

//验证参数范围
int SS1BBE_CjCsParamSetGUIDlg::CheckPara()
{
    QMessageBox msgBox;
    msgBox.setWindowTitle(MODULENAME);
    QString strTmp;
    QString disForVal = ui->lineEdit_DisFor->text();
    QString respZero = ui->lineEdit_ResponsorZeroPos->text();
    //距离预报值
    if(disForVal.isEmpty() || (UINT)(disForVal.toDouble() * 1000) < m_DlgParaSetStruct[0][1].MinVal.lmin * 1000
            || (UINT)(disForVal.toDouble() * 1000) > m_DlgParaSetStruct[0][1].MaxVal.lmax * 1000)
    {
        strTmp = QString("距离预报值正确范围[%1～%2]Km，请重新输入!").arg(m_DlgParaSetStruct[0][1].MinVal.lmin)
                .arg(m_DlgParaSetStruct[0][1].MaxVal.lmax);
        goto flag;
    }
    //应答机距离零值
    if(respZero.isEmpty() || (UINT)(respZero.toDouble() * 2000) < m_DlgParaSetStruct[0][3].MinVal.lmin * 2000
            || (UINT)(respZero.toDouble() * 2000) > m_DlgParaSetStruct[0][3].MaxVal.lmax * 2000)
    {
        strTmp = QString("应答机距离零值正确范围[%1～%2]m，请重新输入!").arg(m_DlgParaSetStruct[0][3].MinVal.lmin)
                .arg(m_DlgParaSetStruct[0][3].MaxVal.lmax);
        goto flag;
    }
    return 1;
flag:
    msgBox.setText(strTmp);
    msgBox.exec();
    return -1;

}

//定时器
void SS1BBE_CjCsParamSetGUIDlg::timerEvent(QTimerEvent *event)
{
    ShowPara(0);
}

//更改参数按钮休息处理函数
void SS1BBE_CjCsParamSetGUIDlg::on_pushButton_Modify_clicked()
{
    if(ui->pushButton_Modify->text() == "更改")
    {
        EnabledControls(true);
        on_comboBox_DisAmbi_currentIndexChanged(ui->comboBox_DisAmbi->currentIndex());
        on_comboBox_DisForWay_currentIndexChanged(ui->comboBox_DisForWay->currentIndex());
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

void SS1BBE_CjCsParamSetGUIDlg::on_pushButton_Set_clicked()
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
                   struct{
                       USHORT tmp1;
                       SMCU::MCSSSCjParamStruct targetSysParam;//目标参数
                   }param[MAX_TARGET_COUNT];

                  }data;
            QByteArray byteArray;
            byteArray.resize(sizeof(data));
            data.target =  0x02 | 0x04 | 0x08 | 0x10;


            for(int i = 0; i < MAX_TARGET_COUNT; ++i)
            {
                data.param[i].targetSysParam = m_MCSSSCjParam.SSCjParam[i];
            }

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

void SS1BBE_CjCsParamSetGUIDlg::closeEvent(QCloseEvent *event)
{
    event->accept();
}

QSize SS1BBE_CjCsParamSetGUIDlg::sizeHint() const
{
    return QSize(this->width(), this->height());
}

/*
//副帧方式响应函数
void SS1BBE_CjCsParamSetGUIDlg::on_comboBox_SubFrmType_currentIndexChanged(int index)
{
//    //副帧方式控件使能
//    if (ui->pushButton_Modify->text() != "更改" || iParaSetFlag != 0)
//    {
//        EnabledControlsSubFrmType(index);   //副帧方式控件使能
//    }
}

//根据副帧类型，使能副帧控件
void SS1BBE_CjCsParamSetGUIDlg::EnabledControlsSubFrmType(int iSubFrm)
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

void SS1BBE_CjCsParamSetGUIDlg::SubFrmEn(bool bFlag)
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
void SS1BBE_CjCsParamSetGUIDlg::IDSubFrmEn(bool bFlag)
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
void SS1BBE_CjCsParamSetGUIDlg::CycSubFrmEn(bool bFlag)
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
void SS1BBE_CjCsParamSetGUIDlg::ReserveSubFrmEn(bool bFlag)
{

}

void SS1BBE_CjCsParamSetGUIDlg::SendMessage(char* pData, unsigned short  usLen, unsigned short usC, unsigned short usTID)
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

int SS1BBE_CjCsParamSetGUIDlg::getSharedMemoryTargetNum()
{
    DWORD stLen = sizeof(SMCU::MCSSysTarget4StatusStruct);
    if(m_pStateMgr != NULL)
        m_pMCSTargetSysStatus  = (SMCU::MCSSysTarget4StatusStruct*)m_pStateMgr->FindOneItem(m_CommTargetID, stLen);

    if (m_pMCSTargetSysStatus != NULL)
        return m_pMCSTargetSysStatus->sysStatu.tParams.ucTargetNum;

    return 0;
}

void SS1BBE_CjCsParamSetGUIDlg::showRadioButton()
{
    int nNum = getSharedMemoryTargetNum();
    if (m_totalTargetNum_inParam == nNum)
        return;
    m_totalTargetNum_inParam = nNum;
    m_nTargetNum = 0;
    set_radioButton_Target();
    ui->radioButton_Target1->hide();
    ui->radioButton_Target2->hide();
    ui->radioButton_Target3->hide();
    ui->radioButton_Target4->hide();
    switch (nNum - 1)
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

void SS1BBE_CjCsParamSetGUIDlg::showTargetParam(int nIndex, int nType)
{
    int i;
    /*******************************目标参数*********************************/
    if (nType == 0)
    {
        nIndex += 1;
        //距离解模糊
        for(i=0; i<DlgMapLineNum[nIndex * 2 + 0]; i++)
        {
            ui->comboBox_DisAmbi->addItem(m_DlgMapMean[nIndex * 2 + 0][i]);
        }
        ui->comboBox_DisAmbi->setCurrentIndex(m_DlgParaSetStruct[nIndex][0].InitVal.lval);
        //距离预报值
        ui->lineEdit_DisFor->setText(QString::number((DOUBLE)m_DlgParaSetStruct[nIndex][1].InitVal.lval, 'f', 3));
        //距离预报方式
        for(i=0; i<DlgMapLineNum[nIndex * 2 + 1]; i++)
        {
            ui->comboBox_DisForWay->addItem(m_DlgMapMean[nIndex * 2 + 1][i]);
        }
        ui->comboBox_DisForWay->setCurrentIndex(m_DlgParaSetStruct[nIndex][2].InitVal.lval);
        //应答机零值距离
        ui->lineEdit_ResponsorZeroPos->setText(QString::number(m_DlgParaSetStruct[nIndex][3].InitVal.lval));
    }
    else if (nType == 1)
    {
        //距离解模糊
        int nCurSel = m_MCSSSCjParam.SSCjParam[nIndex].ucRMHCheck;
        if (nCurSel > 0)
            ui->comboBox_DisAmbi->setCurrentIndex(nCurSel - 1);
        //距离预报值
        ui->lineEdit_DisFor->setText(QString::number(m_MCSSSCjParam.SSCjParam[nIndex].ulPridictR / 1000.0, 'f', 3));
        //距离预报方式
        nCurSel = m_MCSSSCjParam.SSCjParam[nIndex].ucPreRangDataMd;
        if (nCurSel > 0)
            ui->comboBox_DisForWay->setCurrentIndex(nCurSel - 1);
        //应答机零值距离
        ui->lineEdit_ResponsorZeroPos->setText(QString::number(m_MCSSSCjParam.SSCjParam[nIndex].uiRSPDisZero/2000.0, 'f', 3));
        on_comboBox_DisForWay_currentIndexChanged(ui->comboBox_DisForWay->currentIndex());
        on_comboBox_DisAmbi_currentIndexChanged(ui->comboBox_DisAmbi->currentIndex());
    }
    /*******************************目标参数*********************************/
}

void SS1BBE_CjCsParamSetGUIDlg::getParamTarget()
{
    //距离解模糊
    m_MCSSSCjParam.SSCjParam[m_nTargetNum].ucRMHCheck = ui->comboBox_DisAmbi->currentIndex() + 1;
    //距离预报值
    m_MCSSSCjParam.SSCjParam[m_nTargetNum].ulPridictR = (UINT)(ui->lineEdit_DisFor->text().toDouble() * 1000);
    //距离预报方式
    m_MCSSSCjParam.SSCjParam[m_nTargetNum].ucPreRangDataMd = ui->comboBox_DisForWay->currentIndex() + 1;
    //应答机零值距离
    m_MCSSSCjParam.SSCjParam[m_nTargetNum].uiRSPDisZero = (UINT)(ui->lineEdit_ResponsorZeroPos->text().toDouble() * 2000);
}

DWORDLONG SS1BBE_CjCsParamSetGUIDlg::StrToSynCode(const char * pStr)
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

void SS1BBE_CjCsParamSetGUIDlg::on_radioButton_Target1_clicked()
{
    if(1 != CheckPara())
    {
        set_radioButton_Target();
        return;
    }
    GetPara();
    m_nTargetNum = 0;
    //int nType = ui->pushButton_Set->isEnabled()  ? 0 : 1;
    showTargetParam(0, 1);
}

void SS1BBE_CjCsParamSetGUIDlg::on_radioButton_Target2_clicked()
{
    if(1 != CheckPara())
    {
        set_radioButton_Target();
        return;
    }
    GetPara();
    m_nTargetNum = 1;
    //int nType = ui->pushButton_Set->isEnabled()  ? 0 : 1;
    showTargetParam(1, 1);
}

void SS1BBE_CjCsParamSetGUIDlg::on_radioButton_Target3_clicked()
{
    if(1 != CheckPara())
    {
        set_radioButton_Target();
        return;
    }
    GetPara();
    m_nTargetNum = 2;
    //int nType = ui->pushButton_Set->isEnabled()  ? 0 : 1;
    showTargetParam(2, 1);
}

void SS1BBE_CjCsParamSetGUIDlg::on_radioButton_Target4_clicked()
{
    if(1 != CheckPara())
    {
        set_radioButton_Target();
        return;
    }
    GetPara();
    m_nTargetNum = 3;
    //int nType = ui->pushButton_Set->isEnabled()  ? 0 : 1;
    showTargetParam(3, 1);
}
//距离解模糊
void SS1BBE_CjCsParamSetGUIDlg::on_comboBox_DisAmbi_currentIndexChanged(int index)
{
    if(ui->pushButton_Modify->text() == "更改")
        return;
    if(index ==0){
        ui->comboBox_DisForWay->setEnabled(true);
    }
    else{
        ui->comboBox_DisForWay->setEnabled(false);
        ui->lineEdit_DisFor->setEnabled(false);
    }
    on_comboBox_DisForWay_currentIndexChanged(ui->comboBox_DisForWay->currentIndex());
}
//距离预报方式

void SS1BBE_CjCsParamSetGUIDlg::on_comboBox_DisForWay_currentIndexChanged(int index)
{
    if(ui->pushButton_Modify->text() == "更改")
        return;
    if(index == 0 && ui->comboBox_DisAmbi->currentIndex()==0){
        ui->lineEdit_DisFor->setEnabled(true);
    }else{
        ui->lineEdit_DisFor->setEnabled(false);
    }
}


void SS1BBE_CjCsParamSetGUIDlg::updateRadioButton()
{
    if(!m_pCommFunc)return;
    if (m_totalTargetNum == m_pCommFunc->getTargetNum())
        return;
    m_totalTargetNum = m_pCommFunc->getTargetNum();
    m_nTargetNum = 0;
    set_radioButton_Target();
    ui->radioButton_Target1->hide();
    ui->radioButton_Target2->hide();
    ui->radioButton_Target3->hide();
    ui->radioButton_Target4->hide();
    switch(m_totalTargetNum - 1)
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
void SS1BBE_CjCsParamSetGUIDlg::set_radioButton_Target()
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

void SS1BBE_CjCsParamSetGUIDlg::on_lineEdit_DisFor_textChanged(const QString &arg1)
{
    CCommFunc::lineEditPrecision(arg1, ui->lineEdit_DisFor, 3);
}

void SS1BBE_CjCsParamSetGUIDlg::on_lineEdit_ResponsorZeroPos_textChanged(const QString &arg1)
{
    CCommFunc::lineEditPrecision(arg1, ui->lineEdit_ResponsorZeroPos, 3);
}
#pragma pack()
