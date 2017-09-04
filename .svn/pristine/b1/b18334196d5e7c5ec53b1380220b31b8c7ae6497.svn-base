#include "DDTBBE_SysParaSetGUIDlg.h"
#include "ui_DDTBBE_SysParaSetGUIDlg.h"
#include <QCloseEvent>
#include <QMessageBox>
#include <QDateTime>
#include <QDebug>
#include "../../Common/CommonFunction.h"
extern quint64 GetID(ushort usStationID, uchar ucSubSysID, ushort usDeviceID, ushort usType,
                     uchar ucSn);
DDTBBE_SysParaSetGUIDlg::DDTBBE_SysParaSetGUIDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DDTBBE_SysParaSetGUIDlg)
{
    ui->setupUi(this);
    m_pStateMgr = NULL;
    m_pILxTsspMessageManager = NULL;
    m_pILxTsspLogManager = NULL;
    m_pPublicInfo = NULL;
    m_pHAManager = NULL;

    m_MCSSysDataStatus = NULL;

    setAttribute(Qt::WA_DeleteOnClose);
}

DDTBBE_SysParaSetGUIDlg::~DDTBBE_SysParaSetGUIDlg()
{
    killTimer(m_TimerID);
    delete ui;
}

void DDTBBE_SysParaSetGUIDlg::setText(const QString& strTitle,ParaStruct xmlsetText[3][60],QString MapMean[50][30],int MapNum[50],uchar Type,ILxTsspParameterMacro* pMacroObj)
{
    m_strTitle = strTitle;
    setWindowTitle(strTitle);

    memcpy(m_DlgParaSetStruct,xmlsetText,sizeof(ParaStruct)*50*3);

    for(int i=0;i<50;i++)
    {
        for(int j=0;j<30;j++)
        {
            m_DlgMapMean[i][j] = MapMean[i][j];
        }
    }

    memset(DlgMapLineNum,0,50);

    for(int i=0;i<49;i++)
    {
        DlgMapLineNum[i] = MapNum[i];
    }
    //初始化界面
    InitDLG(Type,pMacroObj);

    //设置界面默认选择组选框
    ui->radioButton_IRoadStop->setChecked(true);
    ui->radioButton_QRoadStop->setChecked(true);
}

//初始化对话框
void DDTBBE_SysParaSetGUIDlg::InitDLG(int Type,ILxTsspParameterMacro* pMacroObj)
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
            m_pHAManager->GetParameter(m_MCSSysDataID,byteArray);
            memcpy(&m_MCSSysPara,byteArray.data(),sizeof(MCSSysDataStruct));
        }

        if(m_pStateMgr != NULL)
        {
            DWORD stLen = sizeof(MCSSysDataStatusStruct);
            m_MCSSysDataStatus  =(MCSSysDataStatusStruct*)m_pStateMgr->FindOneItem(m_StatusParaMCSSysDataID, stLen);
            if(m_MCSSysDataStatus == NULL || stLen != sizeof(MCSSysDataStatusStruct))
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
        if(pMacroObj!=NULL)
        {
            QByteArray byteArray;
            //界面上参数的
            pMacroObj->GetParameterBlock(m_MCSSysDataID, byteArray);
            memcpy(&m_MCSSysPara, byteArray.data(),sizeof(MCSSysDataStruct));
        }
        ui->pushButton_Modify->setVisible(false);
        ui->pushButton_Set->setVisible(false);
        ui->pushButton_Set->setText("");
        ui->label_TaskCode->hide();
        ui->label_TaskFlag->hide();
        ui->lineEdit_TaskCode->hide();
        ui->lineEdit_TaskFlag->hide();
        //将设置送数控制设置为不可见
        ui->groupBox_Data->hide();

        //显示参数
        ShowPara(1);
    }
    //    if(!initCmdInfo(MODULENAME))
    //    {
    //        qDebug()<<"获取配置信息失败！";
    //    }
}

//设置输入格式
void DDTBBE_SysParaSetGUIDlg::InitInput()
{
    QRegExp regExp("[A-Fa-f0-9]{1,8}");
    QRegExp regExp1(".{1,6}");
    QRegExp regExp2("[A-Fa-f0-9]{2}");
    //任务代号
    ui->lineEdit_TaskCode->setValidator(new QRegExpValidator(regExp1, this));
    //任务标识
    ui->lineEdit_TaskFlag->setValidator(new QRegExpValidator(regExp, this));
    //数据处理标志码
    ui->lineEdit_DataProcFlag->setValidator(new QRegExpValidator(regExp2, this));
    //浮点数
    QRegExp regExpFre("[0-9]{1,4}(\\.[0-9]{1,6})?");
    //下行频点
    ui->lineEdit_DownRate->setValidator(new QRegExpValidator(regExpFre, this));
}

void DDTBBE_SysParaSetGUIDlg::ShowControls()
{
    //向下拉框控件中写入从xml中读取的内容
    int i;
    //基带送数选择
    for(i=0; i<DlgMapLineNum[0]; i++)
    {
        ui->comboBox_BaseSelect->addItem(m_DlgMapMean[0][i]);
    }
    ui->comboBox_BaseSelect->setCurrentIndex(m_DlgParaSetStruct[0][1].InitVal.lval);
    //数传类别
    for(i=0; i<DlgMapLineNum[1]; i++)
    {
        ui->comboBox_DataType->addItem(m_DlgMapMean[1][i]);
    }
    ui->comboBox_DataType->setCurrentIndex(m_DlgParaSetStruct[0][3].InitVal.lval);
    //RTP负载类型
    for(i=0; i<DlgMapLineNum[2]; i++)
    {
        ui->comboBox_RTPLoadType->addItem(m_DlgMapMean[2][i]);
    }
    ui->comboBox_RTPLoadType->setCurrentIndex(m_DlgParaSetStruct[0][6].InitVal.lval);
    //往静态控件中写入参数缺省值
    //任务代号
    ui->lineEdit_TaskCode->setText(m_DlgParaSetStruct[0][1].CVal.cValue);
    //数据处理标志码
    ui->lineEdit_DataProcFlag->setText(QString::number(m_DlgParaSetStruct[0][2].InitVal.lval));
    //任务标识
    ui->lineEdit_TaskFlag->setText(QString::number(m_DlgParaSetStruct[0][4].InitVal.lval,16).toUpper());
    //下行频率
    ui->lineEdit_DownRate->setText(QString::number(m_DlgParaSetStruct[0][5].InitVal.lval));
    relate();
}

//显示参数
void DDTBBE_SysParaSetGUIDlg::ShowPara(int type)
{
    if(type == 0)       //参数设置
    {
        //副帧同步器
        if(m_MCSSysDataStatus != NULL)
        {
            m_MCSSysPara = m_MCSSysDataStatus->tParams;
        }
        else
        {
            DWORD stLen = sizeof(MCSSysDataStatusStruct);
            if(m_pStateMgr != NULL)
                m_MCSSysDataStatus = (MCSSysDataStatusStruct*)m_pStateMgr->FindOneItem(m_StatusParaMCSSysDataID, stLen);
        }
    }
    else
    {
        //解扰使能或禁止
        //副帧方式控件使能
        if (ui->pushButton_Modify->text() != "更改")
        {
            //            EnabledControlsSubFrmType(ui->comboBox_SubFrmType->currentIndex());
        }
    }
    /********************************分系统公共单元*********************************/
    //基带送数选择
    if (m_MCSSysPara.ucSendMode > 0)
        ui->comboBox_BaseSelect->setCurrentIndex(m_MCSSysPara.ucSendMode - 1);
    //任务代号
    char szVal[7] = { 0 };
    memcpy(szVal, m_MCSSysPara.ucTaskDescribe, sizeof(m_MCSSysPara.ucTaskDescribe));
    ui->lineEdit_TaskCode->setText(QString(szVal));
    //数据处理标识码
    ui->lineEdit_DataProcFlag->setText(QString("%1").arg(m_MCSSysPara.ucDDTCode, 2, 16, QChar::fromLatin1('0')).toUpper());
    ui->lineEdit_DataProcFlag->setEnabled(false);
    //数传类别
    if (m_MCSSysPara.ucDDTOutFormat > 0)
        ui->comboBox_DataType->setCurrentIndex(m_MCSSysPara.ucDDTOutFormat - 1);
    //任务标识
    ui->lineEdit_TaskFlag->setText(QString::number(m_MCSSysPara.uiM, 16).toUpper());
    //下行频率
    ui->lineEdit_DownRate->setText(QString::number(m_MCSSysPara.dwlAnswerRevDownFreq / 1000000.0, 'f'));
    //RTP负载类型
    if (m_MCSSysPara.ucRTPPT > 0)
        ui->comboBox_RTPLoadType->setCurrentIndex(m_MCSSysPara.ucRTPPT - 1);
    /********************************分系统公共单元*********************************/
    relate();
}

//设置帧长范围
void DDTBBE_SysParaSetGUIDlg::SetFrameLenRange(int iWordLen)
{
//    if(iWordLen == 8)
//    {
//        m_DlgParaSetStruct[1][3].MinVal.lmin = 4;
//        m_DlgParaSetStruct[1][3].MaxVal.lmax = 4096;
//    }
//    else
//    {
//        m_DlgParaSetStruct[1][3].MinVal.lmin = 4;
//        m_DlgParaSetStruct[1][3].MaxVal.lmax = 2048;
//    }
//    QString strTemp = "帧长["+QString::number(m_DlgParaSetStruct[1][3].MinVal.lmin)+"~"+
//            QString::number(m_DlgParaSetStruct[1][3].MaxVal.lmax)+"]字";
//        ui->label_FrameLen->setText(strTemp);
}

void DDTBBE_SysParaSetGUIDlg::SetObjInfo(TLxTsspObjectInfo ObjectInfo)
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
        //qDebug() << strInfo;
        CLog::addLog(strInfo);
        return;
    }
    ITEM *pItem = m_DeviceMap.getItem(MODULENAME);
    if (pItem != NULL && pItem->vSubItem.size() > 0)
    {
        int i;
        switch (m_ObjectInfo.usLocalDID) {
        case 0x400:
            i = 0;
            break;
        case 0x401:
            i = 1;
        default:
            break;
        }
        SUBITEM sItem = pItem->vSubItem.at(i);
        m_usC = sItem.usCMD;
        m_usTID = sItem.usTID;
        /*************************框架**********************************/
        m_MCSSysDataID = GetID(sItem.usStationID, sItem.ucSubSysID, sItem.usDeviceID,
                               sItem.usTID, sItem.ucSN);
        /*************************框架**********************************/

        /*************************状态**********************************/
        m_StatusParaMCSSysDataID = GetID(sItem.usStationID, sItem.ucSubSysID, sItem.usDeviceID,
                                         sItem.usStateType, sItem.ucSN);
        /*************************状态**********************************/
    }

    pItem = m_DeviceMap.getItem(SS1_TARGET_NUM);
    if(pItem && pItem->vSubItem.size() > 0)
    {
        SUBITEM sItem = pItem->vSubItem[0];
        m_targetNumID = GetID(sItem.usStationID, sItem.ucSubSysID, sItem.usDeviceID,
                                       sItem.usStateType, sItem.ucSN);
    }

    //获取发送数传控制命令的cmd和type
    ITEM *pItemCom = m_DeviceMap.getItem(CMDNAME);
    if (pItemCom != NULL)
    {
        int index = 0;
        m_sItem = pItemCom->vSubItem.at(index);
        if(m_sItem.usDeviceID != m_ObjectInfo.usLocalDID)
            m_sItem = pItemCom->vSubItem.at(index + 1);
    }

    //日志
    m_pILxTsspLogManager = pObjectManager->GetLogManager();
    m_pStateMgr = pObjectManager->GetStateManager();
    m_pILxTsspMessageManager = pObjectManager->GetMessageManager();
    m_pPublicInfo = pObjectManager->GetPublicInfoManager();
}

//使能或不使能控件
void DDTBBE_SysParaSetGUIDlg::EnabledControls(bool bFlag)
{
    /**************************************************/
    //任务代码
    ui->lineEdit_TaskCode->setEnabled(bFlag);
    //任务标识
    ui->lineEdit_TaskFlag->setEnabled(bFlag);
    //基带送数选择
    ui->comboBox_BaseSelect->setEnabled(bFlag);
    //下行频率
    ui->lineEdit_DownRate->setEnabled(bFlag);
    //数据处理标志码
    ui->lineEdit_DataProcFlag->setEnabled(false);
    //RTP负载类型
    ui->comboBox_RTPLoadType->setEnabled(bFlag);
    //数传类别
    ui->comboBox_DataType->setEnabled(bFlag);
    /**************************************************/

    EnabledDataControls(false);
    ui->pushButton_DataModify->setEnabled(false);
    //
    if (bFlag)
    {
        //副帧方式控件使能
        //        EnabledControlsSubFrmType(ui->comboBox_SubFrmType->currentIndex());
        ui->pushButton_Modify->setText("恢复");
        //ui->comboBox_RTPLoadType->setEnabled(false);
        ui->pushButton_Set->setEnabled(true);
    }
    else
    {
        ui->pushButton_Modify->setText("更改");
        ui->pushButton_Set->setEnabled(false);
    }
}

void DDTBBE_SysParaSetGUIDlg::EnabledDataControls(bool bFlag)
{
    /**********************数传组****************************/
    //数传I路送数组
    //停数
    ui->radioButton_IRoadStart->setEnabled(bFlag);
    //送数
    ui->radioButton_IRoadStop->setEnabled(bFlag);
    //数传Q路送数组
    //停数
    ui->radioButton_QRoadStart->setEnabled(bFlag);
    //送数
    ui->radioButton_QRoadStop->setEnabled(bFlag);
    /************************数传组**************************/
    if (bFlag)
    {
        //副帧方式控件使能
        //        EnabledControlsSubFrmType(ui->comboBox_SubFrmType->currentIndex());
        ui->pushButton_DataModify->setText("恢复");
        ui->pushButton_DataSet->setEnabled(true);
    }
    else
    {
        ui->pushButton_DataModify->setText("更改");
        ui->pushButton_DataSet->setEnabled(false);
    }
}

TLxTsspParamMacroInformation* DDTBBE_SysParaSetGUIDlg::getMacroInfo()
{
    LxTsspObjectManager* pObjectManager = LxTsspObjectManager::Instance();
    if(!pObjectManager)return NULL;
    ILxTsspStateManager* pStateMgr = pObjectManager->GetStateManager();
    if(!pStateMgr)return NULL;

    int sizeOfMacroInfo = sizeof(TLxTsspParamMacroInformation);
    TLxTsspParamMacroInformation* pMacroInfo = (TLxTsspParamMacroInformation*)
            pStateMgr->FindOneItem(m_targetNumID, sizeOfMacroInfo);
    if(!pMacroInfo || sizeOfMacroInfo != sizeof(TLxTsspParamMacroInformation))return NULL;
//    if(pMacroInfo->ucTargetSum < 1 || pMacroInfo->ucTargetSum > 4)return NULL;
    return pMacroInfo;
}

void DDTBBE_SysParaSetGUIDlg::updateParam(MCSSysDataStruct& comPara, ILxTsspParameterMacro* pMacroObj)
{
    TLxTsspParamMacroInformation* pMacroInfo = getMacroInfo();
    if(!pMacroInfo)return;
    //comPara.sysParam.ucTargetNum = pMacroInfo->ucTargetSum;
    if(!pMacroObj)return;
    TLxTsspParamMacroInformation macroInfo;
    if(1 != pMacroObj->GetMacroInformation(macroInfo))return;
    comPara.uiM = macroInfo.aTaskMark[0];
    memcpy(comPara.ucTaskDescribe, macroInfo.aTaskID[0], 6);
}


//保存参数宏

int DDTBBE_SysParaSetGUIDlg::SaveToMacro(ILxTsspParameterMacro* pMacroObj)
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
        updateParam(m_MCSSysPara, pMacroObj);
        QByteArray byteArray;
        //设置当前参数
        byteArray.resize(sizeof(m_MCSSysPara));
        memcpy(byteArray.data(), &m_MCSSysPara, sizeof(m_MCSSysPara));
        pMacroObj->UpdateParameterBlock(m_MCSSysDataID, byteArray);

        //日志信息
        QString strLog;
        strLog += "调制参数设置: 保存宏成功！";
        CLog::addLog(strLog, 1);
    }
}

//从界面取参数
void DDTBBE_SysParaSetGUIDlg::GetPara()
{
    /********************************分系统公共单元*********************************/
    //基带送数选择
    m_MCSSysPara.ucSendMode = ui->comboBox_BaseSelect->currentIndex() + 1;
    //任务代号
    QString strText = ui->lineEdit_TaskCode->text();
    QByteArray bAr = strText.toLatin1();
    memcpy(m_MCSSysPara.ucTaskDescribe, bAr.data(), strText.size());
    //数据处理标识码
    m_MCSSysPara. ucDDTCode = ui->lineEdit_DataProcFlag->text().toInt();
    //数传类别
    m_MCSSysPara.ucDDTOutFormat = ui->comboBox_DataType->currentIndex() + 1;
    //任务标识
    bool ok = false;
    m_MCSSysPara.uiM = ui->lineEdit_TaskFlag->text().toUInt(&ok, 16);
    //下行频率
    m_MCSSysPara.dwlAnswerRevDownFreq = qRound64(ui->lineEdit_DownRate->text().toDouble() * 1000000.0);
    //信息传输格式
    m_MCSSysPara.ucRTPPT = ui->comboBox_RTPLoadType->currentIndex() + 1;
    /********************************分系统公共单元*********************************/
}

//验证参数范围
int DDTBBE_SysParaSetGUIDlg::CheckPara()
{
    QMessageBox msgBox;
    QString strTmp;
    bool ok;
    qint64 downFRate = qRound64(ui->lineEdit_DownRate->text().toDouble(&ok) * 1000000);
    msgBox.setWindowTitle(MODULENAME);
    if (!ui->pushButton_Set->text().isEmpty())
    {
        //任务代号
        if(ui->lineEdit_TaskCode->text().isEmpty())
        {
            strTmp = QString("任务代号为空，请重新输入！");
            goto flag;
        }
        if(ui->lineEdit_TaskCode->text().length() != 6)
        {
            strTmp = QString("任务代号必须为6位，请重新输入!");
            goto flag;
        }
        //任务标识
        if(ui->lineEdit_TaskFlag->text().isEmpty())
        {
            strTmp = QString("任务标识为空，请重新输入！");
            goto flag;
        }
        if(ui->lineEdit_TaskFlag->text().length() > 8)
        {
            strTmp = QString("任务标识为1～8为16进制字符，请重新输入！");
            goto flag;
        }
    }
    //数据处理标志码
    if(ui->lineEdit_DataProcFlag->text().isEmpty())
    {
        strTmp = QString("数据处理标志码为空，请重新输入！");
        goto flag;
    }
    //下行频率
    if(ui->lineEdit_DownRate->text().isEmpty())
    {
        strTmp = QString("下行频率为空, 请重新输入！");
        goto flag;
    }
    if(downFRate < m_DlgParaSetStruct[0][5].MinVal.lmin
            || downFRate > m_DlgParaSetStruct[0][5].MaxVal.lmax || !ok)
    {
        strTmp = QString("下行频率范围是[%1～%2]MHz, 请重新输入！")
                .arg(m_DlgParaSetStruct[0][5].MinVal.lmin / 1000000.0).arg(m_DlgParaSetStruct[0][5].MaxVal.lmax / 1000000.0);

        goto flag;
    }
    return 1;
flag:
    msgBox.setText(strTmp);
    msgBox.exec();
    return -1;
}

//定时器
void DDTBBE_SysParaSetGUIDlg::timerEvent(QTimerEvent *event)
{
    ShowPara(0);
}

void DDTBBE_SysParaSetGUIDlg::on_pushButton_Modify_clicked()
{
    if(ui->pushButton_Modify->text() == "更改")
    {
        EnabledControls(true);
        relate();
        ui->groupBox_Data->setEnabled(false);
        killTimer(m_TimerID);
        return;
    }

    if(ui->pushButton_Modify->text() == "恢复")
    {
        EnabledControls(false);
        relate();
        ui->groupBox_Data->setEnabled(true);
        m_TimerID = startTimer(500);
        return;
    }
}

void DDTBBE_SysParaSetGUIDlg::on_pushButton_Set_clicked()
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
            //码同步参数
            byteArray.resize(sizeof(m_MCSSysPara));
            memcpy(byteArray.data(), &m_MCSSysPara, sizeof(MCSSysDataStruct));
            m_pHAManager->SetParameter(m_MCSSysDataID, byteArray);
            SendMessage((char*)&m_MCSSysPara, sizeof(MCSSysDataStruct), m_usC, m_usTID);

            if(ui->comboBox_BaseSelect->currentIndex() == 0){
                //获取当前的值
                UCHAR cType = ui->comboBox_BaseSelect->currentIndex() + 1;
                QByteArray baI;
                //数传I路送数
                baI.append(52);
                baI.append(0x04);
                baI.append(cType);
                //发送数据内容到接口中
                UCHAR ucIStart = ui->radioButton_IRoadStart->isChecked() ? 0x01 : 0x02;
                baI.append(ucIStart);
                SendMessage(baI.data(), baI.size(), m_sItem.usCMD, m_sItem.usTID, m_sItem.ucSN);
                //数传Q路送数
                QByteArray baQ;
                baQ.append(52);
                baQ.append(0x05);
                baQ.append(cType);
                UCHAR ucQStart = ui->radioButton_QRoadStart->isChecked() ? 0x01 : 0x02;
                baQ.append(ucQStart);
                SendMessage(baQ.data(), baQ.size(), m_sItem.usCMD, m_sItem.usTID, m_sItem.ucSN);
            }
        }

        //日志信息
        QString strLog;
        //tmpLog.ucWarningLevel  = 1;
        strLog += "解调参数设置: 设置参数成功！";
        CLog::addLog(strLog);

        //不使能控件
        EnabledControls(false);
        m_TimerID = startTimer(500);
    }
}

void DDTBBE_SysParaSetGUIDlg::closeEvent(QCloseEvent *event)
{
    event->accept();
}

QSize DDTBBE_SysParaSetGUIDlg::sizeHint() const
{
    return QSize(this->width(), this->height());
}

//副帧方式响应函数
void DDTBBE_SysParaSetGUIDlg::on_comboBox_SubFrmType_currentIndexChanged(int index)
{
    //副帧方式控件使能
    if (ui->pushButton_Modify->text() != "更改" || iParaSetFlag != 0)
    {
        EnabledControlsSubFrmType(index);   //副帧方式控件使能
    }
}

//根据副帧类型，使能副帧控件
void DDTBBE_SysParaSetGUIDlg::EnabledControlsSubFrmType(int iSubFrm)
{
    switch(iSubFrm)
    {
    case 0:     //无副帧
        break;
    case 1:     //ID副帧
        break;
    case 2:     //循环副帧
        break;
    case 3:     //反码副帧
        break;
    default:
        break;
    }
}

//bool DDTBBE_SysParaSetGUIDlg::initCmdInfo(QString strModuleName)
//{
//    bool bRet = m_DeviceMap.loadXML();
//    if (!bRet)
//    {
//        QString strInfo = QString("load device map failed %1");
//        //qDebug() << strInfo;
//        CLog::addLog(strInfo);
//        return false;
//    }
//    ITEM* item = m_DeviceMap.getItem(strModuleName);
//    if (item != NULL)
//    {
//        m_usC = item->vSubItem[0].usCMD;
//        m_usTID = item->vSubItem[0].usTID;
//    }
//    return true;
//}

void DDTBBE_SysParaSetGUIDlg::SendMessage(char* pData, USHORT usLen, USHORT usC, USHORT usTID, USHORT usSN)
{
    static uint seq = 0;
    TLxTsspMessageHeader header = { 0 };
    TLxTsspSubMsgHeader subHeader = { 0 };
    header.uiSequence = seq++;
    header.uiHandle = 1;
    header.S = m_pPublicInfo->GetLocalDeviceID();;
    header.L = sizeof(TLxTsspMessageHeader)  + sizeof(TLxTsspSubMsgHeader) + usLen;
    header.D = m_pPublicInfo->GetLocalDeviceID();//0x09;
    header.T = 1;
    header.M = 1;
    header.P = 1;
    header.A = 0;
    header.R = 0;
    header.C = usC;

    subHeader.Len = usLen;
    subHeader.SID = m_pPublicInfo->GetLocalDeviceID();
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

void DDTBBE_SysParaSetGUIDlg::on_comboBox_WordLen_currentIndexChanged(int index)
{
    //    if (ui->comboBox_WordLen->currentText() == "8")
    //    {
    //        m_DlgParaSetStruct[1][3].MinVal.lmin = 4;
    //        m_DlgParaSetStruct[1][3].MaxVal.lmax = 4096;
    //    }
    //    else
    //    {
    //        m_DlgParaSetStruct[1][3].MinVal.lmin = 4;
    //        m_DlgParaSetStruct[1][3].MaxVal.lmax = 2048;
    //    }
    //    QString ss = "帧长["+QString::number(m_DlgParaSetStruct[1][3].MinVal.lmin)+"～"+
    //            QString::number(m_DlgParaSetStruct[1][3].MaxVal.lmax)+"]字";
    //    ui->label_FrameLen->setText(ss);
}

DWORDLONG DDTBBE_SysParaSetGUIDlg::StrToSynCode(const char * pStr)
{
    char c,C;
    const char*  p;
    DWORDLONG dwlValue = 0;
    char   DigVal = 0;
    USHORT nDigCount=0, MAX_DIGITS=16; //The synchronizing code should be not longer  than 16 digitals

    p=pStr;     //Point to the first char of the string
    c = *p++;   // read the first char
    while ((c != NULL) && (nDigCount <= MAX_DIGITS))//  Not the terminator of a string && total digitals is less than 16
    {
        if(isspace((int)(unsigned char)c))    c = *p++;// skip whitespace
        else if(c >= '0' && c <= '9')                        //the char is a number between 1 and 9
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

void DDTBBE_SysParaSetGUIDlg::on_pushButton_DataModify_clicked()
{
    if(ui->pushButton_DataModify->text() == "更改")
    {
        killTimer(m_TimerID);
        ui->pushButton_Modify->setEnabled(false);
        EnabledDataControls(true);
    }
    else if(ui->pushButton_DataModify->text() == "恢复")
    {
        m_TimerID = startTimer(500);
        ui->pushButton_Modify->setEnabled(true);
        EnabledDataControls(false);
    }
}

void DDTBBE_SysParaSetGUIDlg::on_pushButton_DataSet_clicked()
{
    //获取当前的值
    UCHAR cType = ui->comboBox_BaseSelect->currentIndex() + 1;

    QByteArray baI;
    //数传I路送数
    baI.append(52);
    baI.append(0x04);
    baI.append(cType);
    //发送数据内容到接口中
    UCHAR ucIStart = ui->radioButton_IRoadStart->isChecked() ? 0x01 : 0x02;
    baI.append(ucIStart);
    SendMessage(baI.data(), baI.size(), m_sItem.usCMD, m_sItem.usTID, m_sItem.ucSN);
    //数传Q路送数
    QByteArray baQ;
    baQ.append(52);
    baQ.append(0x05);
    baQ.append(cType);
    UCHAR ucQStart = ui->radioButton_QRoadStart->isChecked() ? 0x01 : 0x02;
    baQ.append(ucQStart);
    SendMessage(baQ.data(), baQ.size(), m_sItem.usCMD, m_sItem.usTID, m_sItem.ucSN);
    on_pushButton_DataModify_clicked();
}

//void DDTBBE_SysParaSetGUIDlg::on_comboBox_BaseSelect_currentIndexChanged(int index)
//{
//    if (index == 1)
//    {
//        ui->pushButton_DataModify->setEnabled(true);
//    }
//    sendDataSel();
//}
void DDTBBE_SysParaSetGUIDlg::sendDataSel()
{
    //数据关联
    if(ui->comboBox_BaseSelect->currentIndex() == 0)//常送
    {
        ui->radioButton_IRoadStart->setChecked(true);
        ui->radioButton_QRoadStart->setChecked(true);
        ui->pushButton_DataModify->setText("更改");
    }

    //控件是否可用的关联
    if(ui->comboBox_BaseSelect->currentIndex() == 0)//常送
    {
        ui->groupBox_Data->setEnabled(false);
    }
    else
    {
        ui->groupBox_Data->setEnabled(true);
        ui->pushButton_DataModify->setEnabled(true);
        if(!m_MCSSysDataStatus)return;
        ui->radioButton_IRoadStart->setChecked(m_MCSSysDataStatus->ucDDTSendSwitchI == 1 ? TRUE : FALSE );
        ui->radioButton_IRoadStop->setChecked(m_MCSSysDataStatus->ucDDTSendSwitchI != 1 ? TRUE : FALSE);
        ui->radioButton_QRoadStart->setChecked(m_MCSSysDataStatus->ucDDTSendSwitchQ == 1 ? TRUE : FALSE);
        ui->radioButton_QRoadStop->setChecked(m_MCSSysDataStatus->ucDDTSendSwitchQ != 1 ? TRUE : FALSE);
        EnabledDataControls(ui->pushButton_DataModify->text() != "更改");
    }
}

void DDTBBE_SysParaSetGUIDlg::dataTransType()
{
    if(ui->pushButton_Modify->text() == "更改")return;
    if(ui->comboBox_DataType->currentIndex() == 0)//921
        ui->comboBox_RTPLoadType->setEnabled(true);
    else
        ui->comboBox_RTPLoadType->setEnabled(false);
}

void DDTBBE_SysParaSetGUIDlg::relate()
{
    sendDataSel();
    dataTransType();
}

void DDTBBE_SysParaSetGUIDlg::on_comboBox_DataType_currentIndexChanged(int /*index*/)
{
    dataTransType();
}
