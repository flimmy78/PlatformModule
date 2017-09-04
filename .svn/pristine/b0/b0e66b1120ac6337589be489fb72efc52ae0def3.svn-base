#include "USBBBE_SysParaSetGUIDlg.h"
#include "ui_USBBBE_SysParaSetGUIDlg.h"
#include <QCloseEvent>
#include <QMessageBox>
#include <QDateTime>
#include "../../Common/Log.h"
#include "../../Common/CommonFunction.h"

extern quint64 GetID(ushort usStationID, uchar ucSubSysID, ushort usDeviceID, ushort usType,
                     uchar ucSn);
USBBBE_SysParaSetGUIDlg::USBBBE_SysParaSetGUIDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::USBBBE_SysParaSetGUIDlg)
{
    ui->setupUi(this);
    m_pStateMgr = NULL;
    m_pILxTsspMessageManager = NULL;
    m_pILxTsspLogManager = NULL;
    m_pPublicInfo = NULL;
    m_pHAManager = NULL;
    flash = true;

    m_MCSSysDataStatus = NULL;
    setAttribute(Qt::WA_DeleteOnClose);
}

USBBBE_SysParaSetGUIDlg::~USBBBE_SysParaSetGUIDlg()
{
    killTimer(m_TimerID);
    delete ui;
}

TLxTsspParamMacroInformation* USBBBE_SysParaSetGUIDlg::getMacroInfo()
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
void USBBBE_SysParaSetGUIDlg::updateParam(MCSSysDataStruct& comPara, ILxTsspParameterMacro* pMacroObj)
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


int USBBBE_SysParaSetGUIDlg::msgBoxFunction(QMessageBox &msgBox, QString msg, BOOL ret)
{
    msgBox.setText(msg);
    msgBox.exec();
    return ret;
}
void USBBBE_SysParaSetGUIDlg::setText(const QString& strTitle, ParaStruct xmlsetText[3][60], QString MapMean[50][30], int MapNum[50], uchar Type, ILxTsspParameterMacro* pMacroObj)
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
void USBBBE_SysParaSetGUIDlg::InitDLG(int Type, ILxTsspParameterMacro* pMacroObj)
{
    iParaSetFlag = Type;
    //设置输入格式
    ui->comboBox_DataTransFormat->setEnabled(false);
    InitInput();
    //显示变量名字和范围
    ShowControls();

    if(Type == 0)//取当前参数
    {
        //取当前设置的参数
        if(m_pStateMgr != NULL)
        {
            DWORD stLen = sizeof(MCSSysDataStatusStruct);
            m_MCSSysDataStatus = (MCSSysDataStatusStruct*)m_pStateMgr->FindOneItem(m_StatusParaMCSSysDataID, stLen);
            if(m_MCSSysDataStatus == NULL || stLen != sizeof(MCSSysDataStatusStruct))
                return;
        }
        //启动定时器
        m_TimerID = startTimer(500);
        //不使能
        EnabledDataControls(false);
        EnabledControls(false);
    }
    else if(Type == 1)//取宏里参数
    {
        if(pMacroObj != NULL)
        {
            QByteArray byteArray;

            pMacroObj->GetParameterBlock(m_ParaMCSSysDataID, byteArray);
            memcpy(&m_MCSSysDataParam, byteArray.data(), sizeof(MCSSysDataStruct));
        }
        ui->pushButton_Modify->setVisible(false);
        ui->pushButton_Set->setVisible(false);
        ui->pushButton_Set->setText("");
        ui->label_TaskCode->hide();
        ui->label_TaskFlag->hide();
        ui->lineEdit_TaskCode->hide();
        ui->lineEdit_TaskFlag->hide();
        //宏参数时将送数控制进行隐藏
        ui->groupBox_DataCtrl->hide();
        //显示参数
        ShowPara(1);
    }
}

//设置输入格式
void USBBBE_SysParaSetGUIDlg::InitInput()
{
    //任务代号
    QRegExp regExp0("[A-Za-z0-9]{1,6}");
    ui->lineEdit_TaskCode->setValidator(new QRegExpValidator(regExp0, this));
    //正整数
    QRegExp regExpFre("[0-9]{1,4}(\\.[0-9]{1,6})?");
    //任务标识
    QRegExp regExpTask("[A-Fa-f0-9]{1,8}");
    ui->lineEdit_TaskFlag->setValidator(new QRegExpValidator(regExpTask, this));
    //上行频率
    ui->lineEdit_UptoFr->setValidator(new QRegExpValidator(regExpFre, this));
    //下行频率
    ui->lineEdit_DowntoFre->setValidator(new QRegExpValidator(regExpFre, this));

    //字母数字
    QRegExp regExp2("[A-Fa-f0-9]{2}");
    //测距数据标识码
    ui->lineEdit_MeterDataCode->setValidator(new QRegExpValidator(regExp2, this));
    //测速数据标识码
    ui->lineEdit_SpeedDataCode->setValidator(new QRegExpValidator(regExp2, this));
    //遥测数据标识码
    ui->lineEdit_TeleDataCode->setValidator(new QRegExpValidator(regExp2, this));
}

void USBBBE_SysParaSetGUIDlg::ShowControls()
{
    int i;
    //任务代号
    ui->lineEdit_TaskCode->setText(m_DlgParaSetStruct[0][0].CVal.cValue);
    //任务标识
    ui->lineEdit_TaskFlag->setText(QString::number(m_DlgParaSetStruct[0][1].InitVal.lval));
    //上行频率
    ui->lineEdit_UptoFr->setText(QString::number(m_DlgParaSetStruct[0][2].InitVal.lval));
    //下行频率
    ui->lineEdit_DowntoFre->setText(QString::number(m_DlgParaSetStruct[0][3].InitVal.lval));
    //基带送数选择
    for(i=0; i<DlgMapLineNum[4];i++)
    {
        ui->comboBox_BaseDataTransType->addItem(m_DlgMapMean[4][i]);
    }
    ui->comboBox_BaseDataTransType->setCurrentIndex(m_DlgParaSetStruct[0][8].InitVal.lval);
    //信息传输格式
    for(i=0; i<DlgMapLineNum[1];i++)
    {
        ui->comboBox_DataTransFormat->addItem(m_DlgMapMean[1][i]);
    }
    ui->comboBox_DataTransFormat->setCurrentIndex(m_DlgParaSetStruct[0][5].InitVal.lval);
    //测距采样率
    for(i=0; i<DlgMapLineNum[2];i++)
    {
        ui->comboBox_MeterGetRate->addItem(m_DlgMapMean[2][i]);
    }
    ui->comboBox_MeterGetRate->setCurrentIndex(m_DlgParaSetStruct[0][6].InitVal.lval);
    //测速采样率
    for(i=0; i<DlgMapLineNum[3];i++)
    {
        ui->comboBox_SpeedGetRate->addItem(m_DlgMapMean[3][i]);
    }
    ui->comboBox_SpeedGetRate->setCurrentIndex(m_DlgParaSetStruct[0][7].InitVal.lval);
    //系统输出
    for(i=0; i<DlgMapLineNum[6];i++)
    {
        ui->comboBox_systemOut->addItem(m_DlgMapMean[6][i]);
    }
    ui->comboBox_systemOut->setCurrentIndex(m_DlgParaSetStruct[0][14].InitVal.lval);
    //测距数据标志码
    ui->lineEdit_MeterDataCode->setText(QString::number(m_DlgParaSetStruct[0][10].InitVal.lval,16).toUpper());
    //测速数据标识码
    ui->lineEdit_SpeedDataCode->setText(QString::number(m_DlgParaSetStruct[0][11].InitVal.lval,16).toUpper());
    //遥测数据标识码
    ui->lineEdit_TeleDataCode->setText(QString::number(m_DlgParaSetStruct[0][12].InitVal.lval,16).toUpper());

    //测距送数
    ui->radioButton_MeterStop->setChecked(true);
    //测速送数
    ui->radioButton_SpeedStop->setChecked(true);
    //遥测送数
    ui->radioButton_TeleStop->setChecked(true);

    //静态控件显示内容
    QString strTmp;
    //上行频率
    strTmp = QString("上行频率\n[%1~%2]MHz").arg(m_DlgParaSetStruct[0][2].MinVal.lmin / 1000 / 1000)
            .arg(m_DlgParaSetStruct[0][2].MaxVal.lmax / 1000 / 1000);
    ui->label_UptoFr->setText(strTmp);
    //下行频率
    strTmp = QString("下行频率\n[%1~%2]MHz").arg(m_DlgParaSetStruct[0][3].MinVal.lmin / 1000 / 1000)
            .arg(m_DlgParaSetStruct[0][3].MaxVal.lmax / 1000 / 1000);
    ui->label_DowntoFre->setText(strTmp);

    ui->label_systemOut->hide();
    ui->comboBox_systemOut->hide();
}

//显示参数
void USBBBE_SysParaSetGUIDlg::ShowPara(int type)
{
    if(type == 0)       //参数设置
    {
        //码同步
        if(m_MCSSysDataStatus != NULL)
        {
            m_MCSSysDataParam = m_MCSSysDataStatus->tParams;
            if (flash)
            {
                if (m_MCSSysDataStatus->ucCJSendSwitch == 1)
                {
                    ui->radioButton_MeterGo->setChecked(true);
                }
                else if (m_MCSSysDataStatus->ucCJSendSwitch == 2)
                {
                    ui->radioButton_MeterStop->setChecked(true);
                }

                if (m_MCSSysDataStatus->ucCSSendSwitch == 1)
                {
                    ui->radioButton_SpeedGo->setChecked(true);
                }
                else if (m_MCSSysDataStatus->ucCJSendSwitch == 2)
                {
                    ui->radioButton_SpeedStop->setChecked(true);
                }

                if (m_MCSSysDataStatus->ucTMSendSwitch == 1)
                {
                    ui->radioButton_TeleGo->setChecked(true);
                }
                else if (m_MCSSysDataStatus->ucTMSendSwitch == 2)
                {
                    ui->radioButton_TeleStop->setChecked(true);
                }
            }
        }
        else
        {
            DWORD stLen = sizeof(MCSSysDataStatusStruct);
            if(m_pStateMgr != NULL)
                m_MCSSysDataStatus  = (MCSSysDataStatusStruct*)m_pStateMgr->FindOneItem(m_StatusParaMCSSysDataID, stLen);
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
    int nCurSel = 0;
    //信息传输格式
    nCurSel = m_MCSSysDataParam.ucDataMode;
    if (nCurSel > 0)
        ui->comboBox_DataTransFormat->setCurrentIndex(nCurSel - 1);
    //测距采样率
    nCurSel = m_MCSSysDataParam.ucCJSendRate;
    if (nCurSel > 0)
        ui->comboBox_MeterGetRate->setCurrentIndex(nCurSel - 1);
    //测速采样率
    nCurSel = m_MCSSysDataParam.ucCSSendRate;
    if (nCurSel > 0)
        ui->comboBox_SpeedGetRate->setCurrentIndex(nCurSel - 1);
    //基带送数选择
    nCurSel = m_MCSSysDataParam.ucSendMode;
    if (nCurSel > 0)
        ui->comboBox_BaseDataTransType->setCurrentIndex(nCurSel - 1);
    if (ui->comboBox_BaseDataTransType->currentText() == "人工")
    {
        ui->pushButton_ModifyDTCtrl->setEnabled(true);
    }
    else
    {
        ui->pushButton_ModifyDTCtrl->setEnabled(false);
    }

    //任务代号
    char szBuf[7] = { 0 };
    memcpy(szBuf, m_MCSSysDataParam.ucTaskDescribe, 6);
    ui->lineEdit_TaskCode->setText(QString(szBuf));
    //任务标识
    ui->lineEdit_TaskFlag->setText(QString::number(m_MCSSysDataParam.uiM, 16).toUpper());
    //上行频率
    QString strVal = QString::number(m_MCSSysDataParam.dwlAnswerRevUpFreq / 1000000.0, 'f');
    ui->lineEdit_UptoFr->setText(strVal);
    //下行频率
    ui->lineEdit_DowntoFre->setText(QString::number(m_MCSSysDataParam.dwlAnswerRevDownFreq / 1000000.0, 'f'));
    //测距数据标志码
    ui->lineEdit_MeterDataCode->setText(QString::number(m_MCSSysDataParam.ucCjCode, 16).toUpper());
    //测速数据标志码
    ui->lineEdit_SpeedDataCode->setText(QString::number(m_MCSSysDataParam.ucCsCode, 16).toUpper());
    //遥测数据标志码
    ui->lineEdit_TeleDataCode->setText(QString::number(m_MCSSysDataParam.ucTmCode, 16).toUpper());
    //系统输出
    if(m_MCSSysDataParam.ucSystemOut > 0)
    {
        ui->comboBox_systemOut->setCurrentIndex(m_MCSSysDataParam.ucSystemOut - 1);
    }
}

//设置帧长范围
void USBBBE_SysParaSetGUIDlg::SetFrameLenRange(int iWordLen)
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

void USBBBE_SysParaSetGUIDlg::SetObjInfo(TLxTsspObjectInfo ObjectInfo)
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
        m_ParaMCSSysDataID = GetID(sItem.usStationID,
                                   sItem.ucSubSysID, sItem.usDeviceID, sItem.usTID,
                                   sItem.ucSN);
        /*************************框架**********************************/

        /*************************状态**********************************/
        m_StatusParaMCSSysDataID = GetID(sItem.usStationID,
                                         sItem.ucSubSysID, sItem.usDeviceID, sItem.usStateType,
                                         sItem.ucSN);
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
    if (pItemCom != NULL/*pItemCom->vSubItem.size() > 10*/)
    {
        //过程控制命令的实现还需要规划
        int index = 2;
        m_sItem = pItemCom->vSubItem.at(index);
        if(m_sItem.usDeviceID != m_ObjectInfo.usLocalDID)
            m_sItem = pItemCom->vSubItem.at(index + 1);
    }

    pItemCom = m_DeviceMap.getItem(CMDNAME4);
    if (pItemCom != NULL)
    {
        SUBITEM* pSItem = pItemCom->getSubItemBySubSysIDAndDevID(m_ObjectInfo.ucSubSysID, m_ObjectInfo.usLocalDID);
        if(pSItem)
            m_sItem4 = *pSItem;
        else{
            int index = 0;
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
void USBBBE_SysParaSetGUIDlg::EnabledControls(bool bFlag)
{
    //任务代号
    ui->lineEdit_TaskCode->setEnabled(bFlag);
    //任务标识
    ui->lineEdit_TaskFlag->setEnabled(bFlag);
    //上行频率
    ui->lineEdit_UptoFr->setEnabled(bFlag);
    //下行频率
    ui->lineEdit_DowntoFre->setEnabled(bFlag);
    //信息传输格式
    ui->comboBox_DataTransFormat->setEnabled(false);
    //测距采样率
    ui->comboBox_MeterGetRate->setEnabled(bFlag);
    //测速采样率
    ui->comboBox_SpeedGetRate->setEnabled(bFlag);
    //基带送数选择
    ui->comboBox_BaseDataTransType->setEnabled(bFlag);
    //测量数据标志码
    ui->lineEdit_MeterDataCode->setEnabled(bFlag);
    //测速数据标志码
    ui->lineEdit_SpeedDataCode->setEnabled(bFlag);
    //遥测数据标志码
    ui->lineEdit_TeleDataCode->setEnabled(bFlag);
    //系统输出
    ui->comboBox_systemOut->setEnabled(bFlag);

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
int USBBBE_SysParaSetGUIDlg::SaveToMacro(ILxTsspParameterMacro* pMacroObj)
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
        updateParam(m_MCSSysDataParam, pMacroObj);
        QByteArray byteArray;
        byteArray.resize(sizeof(m_MCSSysDataParam));
        memcpy(byteArray.data(), &m_MCSSysDataParam, sizeof(m_MCSSysDataParam));
        pMacroObj->UpdateParameterBlock(m_ParaMCSSysDataID, byteArray);

        //日志信息
        QString strLog;
        strLog += QString("%1: 保存宏成功！").arg(MODULENAME);
        CLog::addLog(strLog);
    }
}
//从界面取参数
void USBBBE_SysParaSetGUIDlg::GetPara()
{
    bool ok;
    //基带送数选择
    m_MCSSysDataParam.ucSendMode = ui->comboBox_BaseDataTransType->currentIndex() + 1;
    //信息传输格式
    m_MCSSysDataParam.ucDataMode = ui->comboBox_DataTransFormat->currentIndex() + 1;
    //测距采样率
    m_MCSSysDataParam.ucCJSendRate = ui->comboBox_MeterGetRate->currentIndex() + 1;
    //测速采样率
    m_MCSSysDataParam.ucCSSendRate = ui->comboBox_SpeedGetRate->currentIndex() + 1;
    //任务代号
    const char* tmp = ui->lineEdit_TaskCode->text().toStdString().c_str();
    memcpy(m_MCSSysDataParam.ucTaskDescribe, tmp, sizeof(tmp));
    //任务标识
    m_MCSSysDataParam.uiM = ui->lineEdit_TaskFlag->text().toUInt(&ok, 16);
    //上行频率
    m_MCSSysDataParam.dwlAnswerRevUpFreq = (DWORDLONG)(ui->lineEdit_UptoFr->text().toDouble() * 1000000);
    //下行频率
    m_MCSSysDataParam.dwlAnswerRevDownFreq = (DWORDLONG)(ui->lineEdit_DowntoFre->text().toDouble() * 1000000);
    //测距数据标志码
    m_MCSSysDataParam.ucCjCode = ui->lineEdit_MeterDataCode->text().toInt(&ok, 16);
    //测速数据标志码
    m_MCSSysDataParam.ucCsCode = ui->lineEdit_SpeedDataCode->text().toInt(&ok, 16);
    //遥测数据标志码
    m_MCSSysDataParam.ucTmCode = ui->lineEdit_TeleDataCode->text().toInt(&ok, 16);
    //系统输出
    //m_MCSSysDataParam.ucSystemOut = ui->comboBox_systemOut->currentIndex() + 1;
}

//验证参数范围
int USBBBE_SysParaSetGUIDlg::CheckPara()
{
    QMessageBox msgBox;
    msgBox.setWindowTitle(MODULENAME);
    QString strTmp;
    if (!ui->pushButton_Set->text().isEmpty())
    {
        //任务代号
        if(ui->lineEdit_TaskCode->text().isEmpty())
        {
            msgBox.setText("任务代号不能为空，请重新输入！");
            msgBox.exec();
            return -1;
        }
        if(ui->lineEdit_TaskCode->text().length() != 6)
        {
            msgBox.setText("任务代号必须为6位，请重新输入！");
            msgBox.exec();
            return -1;
        }
        //任务标识
        if(ui->lineEdit_TaskFlag->text().isEmpty())
        {
            msgBox.setText("任务标识不能为空，请重新输入！");
            msgBox.exec();
            return -1;
        }
        if(ui->lineEdit_TaskFlag->text().length() < 1 || ui->lineEdit_TaskFlag->text().length() > 8)
        {
            msgBox.setText("任务标识为1～8为16进制数，请重新输入！");
            msgBox.exec();
            return -1;
        }
    }
    //上行频率
    strTmp = QString("上行频率[%1~%2]MHz超出范围, 请重新输入！")
            .arg(m_DlgParaSetStruct[0][2].MinVal.lmin / 1000000.0).arg(m_DlgParaSetStruct[0][2].MaxVal.lmax / 1000000.0);
    if(ui->lineEdit_UptoFr->text().isEmpty())
    {
        msgBox.setText("上行频率不能为空，请重新输入！");
        msgBox.exec();
        return -1;
    }
    if((DWORDLONG)(ui->lineEdit_UptoFr->text().toDouble() * 1000000) < m_DlgParaSetStruct[0][2].MinVal.lmin
            || (DWORDLONG)(ui->lineEdit_UptoFr->text().toDouble() * 1000000) > m_DlgParaSetStruct[0][2].MaxVal.lmax)
    {
        msgBox.setText(strTmp);
        msgBox.exec();
        return -1;
    }
    //下行频率
    strTmp = QString("下行频率[%1~%2]MHz超出范围, 请重新输入！")
            .arg(m_DlgParaSetStruct[0][3].MinVal.lmin / 1000000.0).arg(m_DlgParaSetStruct[0][3].MaxVal.lmax / 1000000.0);
    if(ui->lineEdit_UptoFr->text().isEmpty())
    {
        msgBox.setText("下行频率不能为空，请重新输入！");
        msgBox.exec();
        return -1;
    }
    if((DWORDLONG)(ui->lineEdit_DowntoFre->text().toDouble() * 1000000) < m_DlgParaSetStruct[0][3].MinVal.lmin
            || (DWORDLONG)(ui->lineEdit_DowntoFre->text().toDouble() * 1000000) > m_DlgParaSetStruct[0][3].MaxVal.lmax)
    {
        msgBox.setText(strTmp);
        msgBox.exec();
        return -1;
    }
    if(ui->lineEdit_MeterDataCode->text().isEmpty())
    {
        return msgBoxFunction(msgBox,"测距数据标志码输入为空，请重新输入!", FALSE);
    }
    if(ui->lineEdit_SpeedDataCode->text().isEmpty())
    {
        return msgBoxFunction(msgBox,"测速数据标识码输入为空，请重新输入!", FALSE);
    }
    if(ui->lineEdit_TeleDataCode->text().isEmpty())
    {
        return msgBoxFunction(msgBox,"遥测数据标志码输入为空，请重新输入!", FALSE);
    }
    return 1;
}

//定时器
void USBBBE_SysParaSetGUIDlg::timerEvent(QTimerEvent *event)
{
    ShowPara(0);
}

//更改参数按钮休息处理函数
void USBBBE_SysParaSetGUIDlg::on_pushButton_Modify_clicked()
{
    if(ui->pushButton_Modify->text() == "更改")
    {
        EnabledControls(true);
        ui->pushButton_ModifyDTCtrl->setEnabled(false);
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

void USBBBE_SysParaSetGUIDlg::on_pushButton_Set_clicked()
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
            byteArray.resize(sizeof(m_MCSSysDataParam));
            memcpy(byteArray.data(), &m_MCSSysDataParam, sizeof(MCSSysDataStruct));
            SendMessage((char*)&m_MCSSysDataParam, sizeof(MCSSysDataStruct), m_usC, m_usTID);
//            static USHORT seq = 0;
//            //基带校零结果查询
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

void USBBBE_SysParaSetGUIDlg::closeEvent(QCloseEvent *event)
{
    event->accept();
}

QSize USBBBE_SysParaSetGUIDlg::sizeHint() const
{
    return QSize(this->width(), this->height());
}

void USBBBE_SysParaSetGUIDlg::SubFrmEn(bool bFlag)
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
void USBBBE_SysParaSetGUIDlg::IDSubFrmEn(bool bFlag)
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
void USBBBE_SysParaSetGUIDlg::CycSubFrmEn(bool bFlag)
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
void USBBBE_SysParaSetGUIDlg::ReserveSubFrmEn(bool bFlag)
{

}

void USBBBE_SysParaSetGUIDlg::SendMessage(char* pData, USHORT usLen, USHORT usC, USHORT usTID, USHORT usSN)
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

DWORDLONG USBBBE_SysParaSetGUIDlg::StrToSynCode(const char * pStr)
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

void USBBBE_SysParaSetGUIDlg::on_pushButton_ModifyDTCtrl_clicked()
{
    if(ui->pushButton_ModifyDTCtrl->text() == "更改")
    {
        flash = false;
        EnabledDataControls(true);
        ui->pushButton_Modify->setEnabled(false);
    }
    else if(ui->pushButton_ModifyDTCtrl->text() == "恢复")
    {
        flash = true;
        EnabledDataControls(false);
        ui->pushButton_Modify->setEnabled(true);
    }
}

void USBBBE_SysParaSetGUIDlg::on_pushButton_SetDTCtrl_clicked()
{
    UCHAR sendDataWay = ui->comboBox_BaseDataTransType->currentIndex() + 1;
    //获取当前的值
    QByteArray baCj;
    //测据
    baCj.append(52);//单元编号UCHAR
    baCj.append(0x02);//类别
    baCj.append(sendDataWay);//送数方式
    //发送数据内容到接口中
    UCHAR ucCjStart = ui->radioButton_MeterGo->isChecked() ? 0x01 : 0x02;
    baCj.append(ucCjStart);//送数控制
    SendMessage(baCj.data(), baCj.size(), m_sItem.usCMD, m_sItem.usTID, m_sItem.ucSN);
    //测速
    QByteArray baCs;
    baCs.append(52);
    baCs.append(0x03);
    baCs.append(sendDataWay);
    UCHAR ucCsStart = ui->radioButton_SpeedGo->isChecked() ? 0x01 : 0x02;
    baCs.append(ucCsStart);
    SendMessage(baCs.data(), baCs.size(), m_sItem.usCMD, m_sItem.usTID, m_sItem.ucSN);

    //遥测
    QByteArray baTM;
    baTM.append(52);
    baTM.append(0x01);
    baTM.append(sendDataWay);
    UCHAR ucTMStart = ui->radioButton_TeleGo->isChecked() ? 0x01 : 0x02;
    baTM.append(ucTMStart);
    SendMessage(baTM.data(), baTM.size(), m_sItem.usCMD, m_sItem.usTID, m_sItem.ucSN);
    flash = true;
    EnabledDataControls(false);
    ui->pushButton_Modify->setEnabled(true);
}

//基带送数选择
void USBBBE_SysParaSetGUIDlg::on_comboBox_BaseDataTransType_currentIndexChanged(int index)
{
    if(ui->pushButton_Modify->text() == "更改")
        return;
    if(index == 0)//常送
    {
        ui->radioButton_MeterGo->setChecked(true);
        ui->radioButton_SpeedGo->setChecked(true);
        ui->radioButton_TeleGo->setChecked(true);
    }
}

void USBBBE_SysParaSetGUIDlg::EnabledDataControls(bool bFlag)
{
    //测距送数
    ui->groupBox_Cj->setEnabled(bFlag);
    //测速送数
    ui->groupBox_Cs->setEnabled(bFlag);
    //遥测送数
    ui->groupBox_TM->setEnabled(bFlag);

    if (bFlag)
    {
        ui->pushButton_ModifyDTCtrl->setText("恢复");
        ui->pushButton_SetDTCtrl->setEnabled(true);
    }
    else
    {
        ui->pushButton_ModifyDTCtrl->setText("更改");
        ui->pushButton_SetDTCtrl->setEnabled(false);
    }

}
