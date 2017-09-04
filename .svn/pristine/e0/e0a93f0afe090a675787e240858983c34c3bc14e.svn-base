#include "SS1PNCodeSetGUIDlg.h"
#include "ui_SS1PNCodeSetGUIDlg.h"
#include <QCloseEvent>
#include <QMessageBox>
#include <QDateTime>
#include "../../Common/CommonFunction.h"
#include <QTimer>
#pragma pack(1)
extern quint64 GetID(ushort usStationID, uchar ucSubSysID, ushort usDeviceID, ushort usType,
                     uchar ucSn);
SS1PNCodeSetGUIDlg::SS1PNCodeSetGUIDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SS1PNCodeSetGUIDlg)
{
    ui->setupUi(this);
    m_pStateMgr = NULL;
    m_pILxTsspMessageManager = NULL;
    m_pILxTsspLogManager = NULL;
    m_pPublicInfo = NULL;
    m_pHAManager = NULL;
    m_nTargetNum = 0;
    m_MCSSSPNCODEStatus = NULL;
    m_timer1 = new QTimer(this);
    m_targetNum_usedInMacro = -1;
    m_targetNum_usedInParam = -1;
    connect(m_timer1, SIGNAL(timeout()), this, SLOT(updateRadioButton()));

    setAttribute(Qt::WA_DeleteOnClose);
    //gzl 2017/07/24 将为码来源初始化失效
    ui->comboBox_VirtualCodeSource->setEnabled(false);
    //end gzl 2017/07/24
}

SS1PNCodeSetGUIDlg::~SS1PNCodeSetGUIDlg()
{
    killTimer(m_TimerID);
    m_timer1->stop();
    delete m_timer1;
    delete ui;
}
int SS1PNCodeSetGUIDlg::msgBoxFunction(QMessageBox &msgBox, QString msg, BOOL ret)
{
    msgBox.setText(msg);
    msgBox.exec();
    return ret;
}
void SS1PNCodeSetGUIDlg::setText(const QString& strTitle, ParaStruct xmlsetText[4][60], QString MapMean[50][30], int MapNum[50], uchar Type, ILxTsspParameterMacro* pMacroObj)
{
    m_strTitle = strTitle;
    setWindowTitle(strTitle);

    memcpy(m_DlgParaSetStruct, xmlsetText, sizeof(ParaStruct) * 60 * 4);

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
void SS1PNCodeSetGUIDlg::InitDLG(int Type, ILxTsspParameterMacro* pMacroObj)
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
            m_pHAManager->GetParameter(m_ParaComSynID, byteArray);
            memcpy(&m_MCSSSPNCodeParam, byteArray.data(), sizeof(SMCU::MCSSSPNCODEParamStruct4));
        }
        if(m_pStateMgr != NULL)
        {
            DWORD stLen = sizeof(SMCU::MCSSSPNCODEStatusStruct4);
            m_MCSSSPNCODEStatus = (SMCU::MCSSSPNCODEStatusStruct4*)m_pStateMgr->FindOneItem(m_StatusComSynID, stLen);
            if(m_MCSSSPNCODEStatus == NULL || stLen != sizeof(SMCU::MCSSSPNCODEStatusStruct4))
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
            pMacroObj->GetParameterBlock(m_ParaComSynID, byteArray);
            memcpy(&m_MCSSSPNCodeParam, byteArray.data(), sizeof(SMCU::MCSSSPNCODEParamStruct4));
        }
        ui->pushButton_Modify->setVisible(false);
        ui->pushButton_Set->setVisible(false);
        m_timer1->start(500);
        //显示参数
        m_pCommFunc = CCommFunc::getInstance();
        ShowPara(1);
    }
}

//设置输入格式
void SS1PNCodeSetGUIDlg::InitInput()
{
    //正整数


    //字母数字
    QRegExp regExp_Short("[A-Fa-f0-9]{0,3}");
    QRegExp regExp_Long("[A-Fa-f0-9]{0,5}");
    ui->lineEdit_UpShortVirtualPhase1->setValidator(new QRegExpValidator(regExp_Short, this));
    //上行短码伪码初相2
    ui->lineEdit_UpShortVirtualPhase2->setValidator(new QRegExpValidator(regExp_Short, this));
    //上行短码伪码本源多项式1
    ui->lineEdit_UpShortVirtualPoly1->setValidator(new QRegExpValidator(regExp_Short, this));
    //上行短码伪码本源多项式2
    ui->lineEdit_UpShortVirtualPoly2->setValidator(new QRegExpValidator(regExp_Short, this));
    //上行长码伪码初相1
    ui->lineEdit_UpLongVirtualPhase1->setValidator(new QRegExpValidator(regExp_Long, this));
    //上行长码伪码本源多项式1
    ui->lineEdit_UpLongVirtualPoly1->setValidator(new QRegExpValidator(regExp_Long, this));
    //下行短码伪码初相1
    ui->lineEdit_DownShortVirtualPhase1->setValidator(new QRegExpValidator(regExp_Short, this));
    //下行短码伪码初相2
    ui->lineEdit_DownShortVirtualPhase2->setValidator(new QRegExpValidator(regExp_Short, this));
    //下行短码伪码本源多项式1
    ui->lineEdit_DownShortVirtualPoly1->setValidator(new QRegExpValidator(regExp_Short, this));
    //下行短码伪码本源多项式2
    ui->lineEdit_DownShortVirtualPoly2->setValidator(new QRegExpValidator(regExp_Short, this));
    //下行长码伪码初相1
    ui->lineEdit_DownLongVirtualPhase1->setValidator(new QRegExpValidator(regExp_Long, this));
    //下行长码伪码本源多项式1
    ui->lineEdit_DownLongVirtualPoly1->setValidator(new QRegExpValidator(regExp_Long, this));
}

void SS1PNCodeSetGUIDlg::ShowControls()
{
    showTargetParam(0);
    //默认选择目标1
    ui->radioButton_Target1->setChecked(true);
}

//显示参数
void SS1PNCodeSetGUIDlg::ShowPara(int type)
{
    if(type == 0)       //参数设置
    {
        if (m_MCSSSPNCODEStatus != NULL)
        {
            for (int i = 0; i < MAX_TARGET_COUNT; i++)
                m_MCSSSPNCodeParam.SSPNCODEParam[i] = m_MCSSSPNCODEStatus->SSPNCODEStatu[i].tParams;
        }
        else
        {
            DWORD stLen = sizeof(SMCU::MCSSSPNCODEStatusStruct4);
            if (m_pStateMgr != NULL)
                m_MCSSSPNCODEStatus = (SMCU::MCSSSPNCODEStatusStruct4*)m_pStateMgr->FindOneItem(m_StatusComSynID, stLen);
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

    //    if (m_MCSSSPNCODEStatus == NULL)
    //        return;

    showTargetParam(m_nTargetNum, 1);
}

//设置帧长范围
void SS1PNCodeSetGUIDlg::SetFrameLenRange(int iWordLen)
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

void SS1PNCodeSetGUIDlg::SetObjInfo(TLxTsspObjectInfo ObjectInfo)
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
        QString strInfo = QString("load device map failed %1");
        CLog::addLog(strInfo);
        return;
    }

    ITEM *pItem = m_DeviceMap.getItem(MODULENAME);
    if (pItem->vSubItem.size() > 0)
    {
        SUBITEM& sItem = *(pItem->getSubItemByDeviceID_SN(m_ObjectInfo.usLocalDID, m_ObjectInfo.ucSN));
        m_usC=sItem.usCMD;
        m_usTID=sItem.usTID;
        /*************************框架**********************************/
        m_ParaComSynID = GetID(sItem.usStationID,
                               sItem.ucSubSysID, sItem.usDeviceID, sItem.usTID,
                               sItem.ucSN);
        /*************************框架**********************************/
        /*************************状态**********************************/
        m_StatusComSynID = GetID(sItem.usStationID,
                                 sItem.ucSubSysID, sItem.usDeviceID, sItem.usStateType,
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
void SS1PNCodeSetGUIDlg::EnabledControls(bool bFlag)
{
    /*************************Group1*************************/
    //伪码来源
    ui->comboBox_VirtualCodeSource->setEnabled(false);
    //短码伪码初相1
    ui->lineEdit_UpShortVirtualPhase1->setEnabled(bFlag);
    //短码伪码初相2
    ui->lineEdit_UpShortVirtualPhase2->setEnabled(bFlag);
    //短码伪码本原多项式1
    ui->lineEdit_UpShortVirtualPoly1->setEnabled(bFlag);
    //短码伪码本原多项式2
    ui->lineEdit_UpShortVirtualPoly2->setEnabled(bFlag);
    //长码伪码初相1
    ui->lineEdit_UpLongVirtualPhase1->setEnabled(bFlag);
    //长码伪码本原多项式1
    ui->lineEdit_UpLongVirtualPoly1->setEnabled(bFlag);

    //短码伪码初相1
    ui->lineEdit_DownShortVirtualPhase1->setEnabled(bFlag);
    //短码伪码初相2
    ui->lineEdit_DownShortVirtualPhase2->setEnabled(bFlag);
    //短码伪码本原多项式1
    ui->lineEdit_DownShortVirtualPoly1->setEnabled(bFlag);
    //短码伪码本原多项式2
    ui->lineEdit_DownShortVirtualPoly2->setEnabled(bFlag);
    //长码伪码初相1
    ui->lineEdit_DownLongVirtualPhase1->setEnabled(bFlag);
    //长码伪码本原多项式1
    ui->lineEdit_DownLongVirtualPoly1->setEnabled(bFlag);
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
int SS1PNCodeSetGUIDlg::SaveToMacro(ILxTsspParameterMacro* pMacroObj)
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
        byteArray.resize(sizeof(m_MCSSSPNCodeParam));
        memcpy(byteArray.data(), &m_MCSSSPNCodeParam, sizeof(m_MCSSSPNCodeParam));
        pMacroObj->UpdateParameterBlock(m_ParaComSynID, byteArray);

        //日志信息
        QString strLog;
        strLog += QString("%1: 保存宏成功！").arg(MODULENAME);
        CLog::addLog(strLog);
    }
}
//从界面取参数
void SS1PNCodeSetGUIDlg::GetPara()
{
    getParamTarget();
}

//验证参数范围
int SS1PNCodeSetGUIDlg::CheckPara()
{
    QMessageBox msgBox;
    msgBox.setWindowTitle(MODULENAME);
    if(ui->lineEdit_UpShortVirtualPhase1->text().isEmpty())
    {
        return msgBoxFunction(msgBox,"上行伪码,短码伪码初相1输入为空，请重新输入!", FALSE);
    }
    if(ui->lineEdit_UpShortVirtualPhase2->text().isEmpty())
    {
        return msgBoxFunction(msgBox,"上行伪码,短码伪码初相2输入为空，请重新输入!", FALSE);
    }
    if(ui->lineEdit_UpShortVirtualPoly1->text().isEmpty())
    {
        return msgBoxFunction(msgBox,"上行伪码,短码伪码本源多项式1输入为空，请重新输入!", FALSE);
    }
    if(ui->lineEdit_UpShortVirtualPoly2->text().isEmpty())
    {
        return msgBoxFunction(msgBox,"上行伪码,短码伪码本源多项式2输入为空，请重新输入!", FALSE);
    }

    if(ui->lineEdit_UpLongVirtualPhase1->text().isEmpty())
    {
        return msgBoxFunction(msgBox,"上行伪码,长伪码初相1输入为空，请重新输入!", FALSE);
    }
    if(ui->lineEdit_UpLongVirtualPoly1->text().isEmpty())
    {
        return msgBoxFunction(msgBox,"上行伪码,长码伪码本源多项式1输入为空，请重新输入!", FALSE);
    }

    if(ui->lineEdit_DownShortVirtualPhase1->text().isEmpty())
    {
        return msgBoxFunction(msgBox,"下行伪码,短码伪码初相1输入为空，请重新输入!", FALSE);
    }
    if(ui->lineEdit_DownShortVirtualPhase2->text().isEmpty())
    {
        return msgBoxFunction(msgBox,"下行伪码,短码伪码初相2输入为空，请重新输入!", FALSE);
    }

    if(ui->lineEdit_DownShortVirtualPoly1->text().isEmpty())
    {
        return msgBoxFunction(msgBox,"下行伪码,短码伪码本源多项式1输入为空，请重新输入!", FALSE);
    }

    if(ui->lineEdit_DownShortVirtualPoly2->text().isEmpty())
    {
        return msgBoxFunction(msgBox,"下行伪码,短码伪码本源多项式2输入为空，请重新输入!", FALSE);
    }

    if(ui->lineEdit_DownLongVirtualPhase1->text().isEmpty())
    {
        return msgBoxFunction(msgBox,"下行伪码,长伪码初相1输入为空，请重新输入!", FALSE);
    }
    if(ui->lineEdit_DownLongVirtualPoly1->text().isEmpty())
    {
        return msgBoxFunction(msgBox,"下行伪码,长码伪码本源多项式1输入为空，请重新输入!", FALSE);
    }
    bool ok = false;
    if(ui->lineEdit_UpShortVirtualPhase1->text().toInt(&ok, 16) > 0x3ff)
    {
        return msgBoxFunction(msgBox,"上行伪码,短码伪码初相1有效位为10位，所以最多只能输入三个16进制字符，且从低位开始的第三个字符不应大于3!", FALSE);
    }
    if(ui->lineEdit_UpShortVirtualPhase2->text().toInt(&ok, 16) > 0x3ff)
    {
        return msgBoxFunction(msgBox,"上行伪码,短码伪码初相2有效位为10位，所以最多只能输入三个16进制字符，且从低位开始的第三个字符不应大于3!", FALSE);
    }
    if(ui->lineEdit_UpShortVirtualPoly1->text().toInt(&ok, 16) > 0x3ff)
    {
        return msgBoxFunction(msgBox,"上行伪码,短码伪码本源多项式1有效位为10位，所以最多只能输入三个16进制字符，且从低位开始的第三个字符不应大于3!", FALSE);
    }
    if(ui->lineEdit_UpShortVirtualPoly2->text().toInt(&ok, 16) > 0x3ff)
    {
        return msgBoxFunction(msgBox,"上行伪码,短码伪码本源多项式2有效位为10位，所以最多只能输入三个16进制字符，且从低位开始的第三个字符不应大于3!", FALSE);
    }

    if(ui->lineEdit_UpLongVirtualPhase1->text().toInt(&ok, 16) > 0x3ffff)
    {
        return msgBoxFunction(msgBox,"上行伪码,长伪码初相1有效位为10位，所以最多只能输入五个16进制字符，且从低位开始的第五个字符不应大于3!", FALSE);
    }
    if(ui->lineEdit_UpLongVirtualPoly1->text().toInt(&ok, 16) > 0x3ffff)
    {
        return msgBoxFunction(msgBox,"上行伪码,长码伪码本源多项式1有效位为10位，所以最多只能输入五个16进制字符，且从低位开始的第五个字符不应大于3!", FALSE);
    }

    if(ui->lineEdit_DownShortVirtualPhase1->text().toInt(&ok, 16) > 0x3ff)
    {
        return msgBoxFunction(msgBox,"下行伪码,短码伪码初相1有效位为10位，所以最多只能输入三个16进制字符，且从低位开始的第三个字符不应大于3!", FALSE);
    }
    if(ui->lineEdit_DownShortVirtualPhase2->text().toInt(&ok, 16) > 0x3ff)
    {
        return msgBoxFunction(msgBox,"下行伪码,短码伪码初相2有效位为10位，所以最多只能输入三个16进制字符，且从低位开始的第三个字符不应大于3!", FALSE);
    }

    if(ui->lineEdit_DownShortVirtualPoly1->text().toInt(&ok, 16) > 0x3ff)
    {
        return msgBoxFunction(msgBox,"下行伪码,短码伪码本源多项式1有效位为10位，所以最多只能输入三个16进制字符，且从低位开始的第三个字符不应大于3!", FALSE);
    }

    if(ui->lineEdit_DownShortVirtualPoly2->text().toInt(&ok, 16) > 0x3ff)
    {
        return msgBoxFunction(msgBox,"下行伪码,短码伪码本源多项式2有效位为10位，所以最多只能输入三个16进制字符，且从低位开始的第三个字符不应大于3!", FALSE);
    }

    if(ui->lineEdit_DownLongVirtualPhase1->text().toInt(&ok, 16) > 0x3ffff)
    {
        return msgBoxFunction(msgBox,"下行伪码,长伪码初相1有效位为18位，所以最多只能输入五个16进制字符，且从低位开始的第五个字符不应大于3!", FALSE);
    }
    if(ui->lineEdit_DownLongVirtualPoly1->text().toInt(&ok, 16) > 0x3ffff)
    {
        return msgBoxFunction(msgBox,"下行伪码,长码伪码本源多项式1有效位为18位，所以最多只能输入五个16进制字符，且从低位开始的第五个字符不应大于3!", FALSE);
    }

    return 1;
}

//定时器
void SS1PNCodeSetGUIDlg::timerEvent(QTimerEvent *event)
{
    ShowPara(0);
}

//更改参数按钮休息处理函数
void SS1PNCodeSetGUIDlg::on_pushButton_Modify_clicked()
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

void SS1PNCodeSetGUIDlg::on_pushButton_Set_clicked()
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
                   struct{USHORT tmp1;//占位
                          SMCU::MCSSSPNCodeStruct targetSysParam;//目标参数
                         }param[MAX_TARGET_COUNT];

                  }data;
            QByteArray byteArray;
            byteArray.resize(sizeof(data));
            data.target =  0x02 | 0x04 | 0x08 | 0x10;


            for(int i = 0; i < MAX_TARGET_COUNT; ++i)
            {
                data.param[i].targetSysParam = m_MCSSSPNCodeParam.SSPNCODEParam[i];
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

void SS1PNCodeSetGUIDlg::closeEvent(QCloseEvent *event)
{
    event->accept();
}

QSize SS1PNCodeSetGUIDlg::sizeHint() const
{
    return QSize(this->width(), this->height());
}

/*
//副帧方式响应函数
void SS1PNCodeSetGUIDlg::on_comboBox_SubFrmType_currentIndexChanged(int index)
{
//    //副帧方式控件使能
//    if (ui->pushButton_Modify->text() != "更改" || iParaSetFlag != 0)
//    {
//        EnabledControlsSubFrmType(index);   //副帧方式控件使能
//    }
}

//根据副帧类型，使能副帧控件
void SS1PNCodeSetGUIDlg::EnabledControlsSubFrmType(int iSubFrm)
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

void SS1PNCodeSetGUIDlg::SubFrmEn(bool bFlag)
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
void SS1PNCodeSetGUIDlg::IDSubFrmEn(bool bFlag)
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
void SS1PNCodeSetGUIDlg::CycSubFrmEn(bool bFlag)
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
void SS1PNCodeSetGUIDlg::ReserveSubFrmEn(bool bFlag)
{

}

int SS1PNCodeSetGUIDlg::getSharedMemoryTargetNum()
{
    DWORD stLen = sizeof(SMCU::MCSSysTarget4StatusStruct);
    if(m_pStateMgr != NULL)
        m_pMCSTargetSysStatus  = (SMCU::MCSSysTarget4StatusStruct*)m_pStateMgr->FindOneItem(m_CommTargetID, stLen);

    if (m_pMCSTargetSysStatus != NULL)
        return m_pMCSTargetSysStatus->sysStatu.tParams.ucTargetNum;

    return 0;
}

void SS1PNCodeSetGUIDlg::showRadioButton()
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

void SS1PNCodeSetGUIDlg::showTargetParam(int nIndex, int nType)
{
    int i;
    /*******************************目标参数*********************************/
    if (nType == 0)
    {
        //伪码来源
        ui->comboBox_VirtualCodeSource->clear();
        for(i=0; i<DlgMapLineNum[nIndex]; i++)
        {
            ui->comboBox_VirtualCodeSource->addItem(m_DlgMapMean[nIndex][i]);
        }
        ui->comboBox_VirtualCodeSource->setCurrentIndex(m_DlgParaSetStruct[nIndex][0].InitVal.lval);
        //ui->comboBox_VirtualCodeSource->setCurrentIndex(m_MCSSSPNCodeParam.SSPNCODEParam[nIndex].ucPNSource);
        //上行短码伪码初相1
        ui->lineEdit_UpShortVirtualPhase1->setText(QString::number(m_DlgParaSetStruct[nIndex][1].InitVal.lval, 16).toUpper());
        //上行短码伪码初相2
        ui->lineEdit_UpShortVirtualPhase2->setText(QString::number(m_DlgParaSetStruct[nIndex][2].InitVal.lval, 16).toUpper());
        //上行短码伪码本源多项式1
        ui->lineEdit_UpShortVirtualPoly1->setText(QString::number(m_DlgParaSetStruct[nIndex][3].InitVal.lval, 16).toUpper());
        //上行短码伪码本源多项式2
        ui->lineEdit_UpShortVirtualPoly2->setText(QString::number(m_DlgParaSetStruct[nIndex][4].InitVal.lval, 16).toUpper());
        //上行长码伪码初相1
        ui->lineEdit_UpLongVirtualPhase1->setText(QString::number(m_DlgParaSetStruct[nIndex][5].InitVal.lval, 16).toUpper());
        //上行长码伪码本源多项式1
        ui->lineEdit_UpLongVirtualPoly1->setText(QString::number(m_DlgParaSetStruct[nIndex][6].InitVal.lval, 16).toUpper());
        //下行短码伪码初相1
        ui->lineEdit_DownShortVirtualPhase1->setText(QString::number(m_DlgParaSetStruct[nIndex][7].InitVal.lval, 16).toUpper());
        //下行短码伪码初相2
        ui->lineEdit_DownShortVirtualPhase2->setText(QString::number(m_DlgParaSetStruct[nIndex][8].InitVal.lval, 16).toUpper());
        //下行短码伪码本源多项式1
        ui->lineEdit_DownShortVirtualPoly1->setText(QString::number(m_DlgParaSetStruct[nIndex][9].InitVal.lval, 16).toUpper());
        //下行短码伪码本源多项式2
        ui->lineEdit_DownShortVirtualPoly2->setText(QString::number(m_DlgParaSetStruct[nIndex][10].InitVal.lval, 16).toUpper());
        //下行长码伪码初相1
        ui->lineEdit_DownLongVirtualPhase1->setText(QString::number(m_DlgParaSetStruct[nIndex][11].InitVal.lval, 16).toUpper());
        //下行长码伪码本源多项式1
        ui->lineEdit_DownLongVirtualPoly1->setText(QString::number(m_DlgParaSetStruct[nIndex][12].InitVal.lval, 16).toUpper());
        /********************************目标受伪码*********************************/
    }
    else if (nType == 1)
    {
        //伪码来源
        //int nCurSel = m_MCSSSPNCodeParam.SSPNCODEParam[nIndex].ucPNSource;
        //if (nCurSel > 0)
        //    ui->comboBox_VirtualCodeSource->setCurrentIndex(nCurSel - 1);
        ui->comboBox_VirtualCodeSource->setCurrentIndex(0);
        //ui->comboBox_VirtualCodeSource->setCurrentIndex(m_MCSSSPNCodeParam.SSPNCODEParam[nIndex].ucPNSource);
        //上行短码伪码初相1
        ui->lineEdit_UpShortVirtualPhase1->setText(QString::number(m_MCSSSPNCodeParam.SSPNCODEParam[nIndex].usUpTCSPNInitPhaseA, 16).toUpper());
        //上行短码伪码初相2
        ui->lineEdit_UpShortVirtualPhase2->setText(QString::number(m_MCSSSPNCodeParam.SSPNCODEParam[nIndex].usUpTCSPNInitPhaseB, 16).toUpper());
        //上行短码伪码本源多项式1
        ui->lineEdit_UpShortVirtualPoly1->setText(QString::number(m_MCSSSPNCodeParam.SSPNCODEParam[nIndex].usUpTCSPNPolyA, 16).toUpper());
        //上行短码伪码本源多项式2
        ui->lineEdit_UpShortVirtualPoly2->setText(QString::number(m_MCSSSPNCodeParam.SSPNCODEParam[nIndex].usUpTCSPNPolyB, 16).toUpper());
        //上行长码伪码初相1
        ui->lineEdit_UpLongVirtualPhase1->setText(QString::number(m_MCSSSPNCodeParam.SSPNCODEParam[nIndex].usUpMessSPNInitPhaseA, 16).toUpper());
        //上行长码伪码本源多项式1
        ui->lineEdit_UpLongVirtualPoly1->setText(QString::number(m_MCSSSPNCodeParam.SSPNCODEParam[nIndex].usUpMessSPNPolyA, 16).toUpper());
        //下行短码伪码初相1
        ui->lineEdit_DownShortVirtualPhase1->setText(QString::number(m_MCSSSPNCodeParam.SSPNCODEParam[nIndex].usDnTCSPNInitPhaseA, 16).toUpper());
        //下行短码伪码初相2
        ui->lineEdit_DownShortVirtualPhase2->setText(QString::number(m_MCSSSPNCodeParam.SSPNCODEParam[nIndex].usDnTCSPNInitPhaseB, 16).toUpper());
        //下行短码伪码本源多项式1
        ui->lineEdit_DownShortVirtualPoly1->setText(QString::number(m_MCSSSPNCodeParam.SSPNCODEParam[nIndex].usDnTCSPNPolyA, 16).toUpper());
        //下行短码伪码本源多项式2
        ui->lineEdit_DownShortVirtualPoly2->setText(QString::number(m_MCSSSPNCodeParam.SSPNCODEParam[nIndex].usDnTCSPNPolyB, 16).toUpper());
        //下行长码伪码初相1
        ui->lineEdit_DownLongVirtualPhase1->setText(QString::number(m_MCSSSPNCodeParam.SSPNCODEParam[nIndex].usDnMessSPNInitPhaseA, 16).toUpper());
        //下行长码伪码本源多项式1
        ui->lineEdit_DownLongVirtualPoly1->setText(QString::number(m_MCSSSPNCodeParam.SSPNCODEParam[nIndex].usDnMessSPNPolyA, 16).toUpper());
        /********************************目标受伪码*********************************/
    }
    /*******************************目标参数*********************************/
}

void SS1PNCodeSetGUIDlg::getParamTarget()
{
    int nIndex = m_nTargetNum;
    //伪码来源
    //m_MCSSSPNCodeParam.SSPNCODEParam[nIndex].ucPNSource = ui->comboBox_VirtualCodeSource->currentIndex();
    //上行短码伪码初相1
    bool ok = false;
    m_MCSSSPNCodeParam.SSPNCODEParam[nIndex].usUpTCSPNInitPhaseA = ui->lineEdit_UpShortVirtualPhase1->text().toInt(&ok, 16);
    //上行短码伪码初相2
    m_MCSSSPNCodeParam.SSPNCODEParam[nIndex].usUpTCSPNInitPhaseB = ui->lineEdit_UpShortVirtualPhase2->text().toInt(&ok, 16);
    //上行短码伪码本源多项式1
    m_MCSSSPNCodeParam.SSPNCODEParam[nIndex].usUpTCSPNPolyA = ui->lineEdit_UpShortVirtualPoly1->text().toInt(&ok, 16);
    //上行短码伪码本源多项式2
    m_MCSSSPNCodeParam.SSPNCODEParam[nIndex].usUpTCSPNPolyB = ui->lineEdit_UpShortVirtualPoly2->text().toInt(&ok, 16);
    //上行长码伪码初相1
    m_MCSSSPNCodeParam.SSPNCODEParam[nIndex].usUpMessSPNInitPhaseA = ui->lineEdit_UpLongVirtualPhase1->text().toInt(&ok, 16);
    //上行长码伪码本源多项式1
    m_MCSSSPNCodeParam.SSPNCODEParam[nIndex].usUpMessSPNPolyA = ui->lineEdit_UpLongVirtualPoly1->text().toInt(&ok, 16);
    //下行短码伪码初相1
    m_MCSSSPNCodeParam.SSPNCODEParam[nIndex].usDnTCSPNInitPhaseA = ui->lineEdit_DownShortVirtualPhase1->text().toInt(&ok, 16);
    //下行短码伪码初相2
    m_MCSSSPNCodeParam.SSPNCODEParam[nIndex].usDnTCSPNInitPhaseB = ui->lineEdit_DownShortVirtualPhase2->text().toInt(&ok, 16);
    //下行短码伪码本源多项式1
    m_MCSSSPNCodeParam.SSPNCODEParam[nIndex].usDnTCSPNPolyA = ui->lineEdit_DownShortVirtualPoly1->text().toInt(&ok, 16);
    //下行短码伪码本源多项式2
    m_MCSSSPNCodeParam.SSPNCODEParam[nIndex].usDnTCSPNPolyB = ui->lineEdit_DownShortVirtualPoly2->text().toInt(&ok, 16);
    //下行长码伪码初相1
    m_MCSSSPNCodeParam.SSPNCODEParam[nIndex].usDnMessSPNInitPhaseA = ui->lineEdit_DownLongVirtualPhase1->text().toInt(&ok, 16);
    //下行长码伪码本源多项式1
    m_MCSSSPNCodeParam.SSPNCODEParam[nIndex].usDnMessSPNPolyA = ui->lineEdit_DownLongVirtualPoly1->text().toInt(&ok, 16);
}

void SS1PNCodeSetGUIDlg::SendMessage(char* pData, unsigned short  usLen, unsigned short usC, unsigned short usTID)
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

DWORDLONG SS1PNCodeSetGUIDlg::StrToSynCode(const char * pStr)
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
        if(isspace((int)(unsigned char)c))    c = *p++;
        // skip whitespace
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

void SS1PNCodeSetGUIDlg::on_radioButton_Target1_clicked()
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

void SS1PNCodeSetGUIDlg::on_radioButton_Target2_clicked()
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

void SS1PNCodeSetGUIDlg::on_radioButton_Target3_clicked()
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

void SS1PNCodeSetGUIDlg::on_radioButton_Target4_clicked()
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
void SS1PNCodeSetGUIDlg::updateRadioButton()
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
void SS1PNCodeSetGUIDlg::set_radioButton_Target()
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
