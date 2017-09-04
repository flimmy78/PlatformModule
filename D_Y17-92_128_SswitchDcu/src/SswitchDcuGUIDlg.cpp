#include "SswitchDcuGUIDlg.h"
#include "ui_SswitchDcuGUIDlg.h"
#include <QCloseEvent>
#include <QMessageBox>
#include <QDateTime>
#include <QDir>
#include <QDebug>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>

#include "../../Common/CommonFunction.h"

extern quint64 GetID(ushort usStationID, uchar ucSubSysID, ushort usDeviceID, ushort usType,
              uchar ucSn);

SswitchDcuGUIDlg::SswitchDcuGUIDlg(QWidget *parent) : QDialog(parent),
    ui(new Ui::SswitchDcuGUIDlg)
{
    ui->setupUi(this);
    m_pStateMgr = NULL;
    m_pILxTsspMessageManager = NULL;
    m_pILxTsspLogManager = NULL;
    m_pPublicInfo = NULL;
    m_pHAManager = NULL;

    m_pSswitchDcuStatus = NULL;

    m_bOnLine = false;

    setAttribute(Qt::WA_DeleteOnClose);
    //setMinimumSize(800, 550);
    init();

    EnabledControls(false);
}

SswitchDcuGUIDlg::~SswitchDcuGUIDlg()
{
    killTimer(m_TimerID);
    delete ui;
}

void SswitchDcuGUIDlg::setText(const QString& strTitle, ParaStruct xmlsetText[3][60], QString MapMean[50][30], int MapNum[50], uchar Type, ILxTsspParameterMacro* pMacroObj)
{
    m_strTitle = strTitle;
    setWindowTitle(strTitle);

    memcpy(m_DlgParaSetStruct, xmlsetText, sizeof(ParaStruct) * 50 * 3);

    for(int i=0; i<50; i++)
    {
        for(int j=0; j<30; j++)
        {
            m_DlgMapMean[i][j] = MapMean[i][j];
        }
    }
    memset(DlgMapLineNum, 0, 50);

    for(int i=0; i < 49; i++)
    {
        DlgMapLineNum[i] = MapNum[i];
    }
    //初始化界面
    InitDLG(Type, pMacroObj);

    ui->lineEdit_CrrlMode->setReadOnly(true);
    ui->lineEdit_SelCheckStatus->setReadOnly(true);
    ui->lineEdit_CtrlSJ1->setReadOnly(true);
    ui->lineEdit_CtrlSJ2->setReadOnly(true);
}

//初始化对话框
void SswitchDcuGUIDlg::InitDLG(int Type, ILxTsspParameterMacro* pMacroObj)
{
    //设置输入格式
    InitInput();

    if(Type == 0)//取当前参数
    {
        //取当前设置的参数
       /* QByteArray byteArray;
        if(m_pHAManager!=NULL)
        {
            m_pHAManager->GetParameter(m_ParaDDTBBECodeSynID,byteArray);
            memcpy(&m_DDTBBECodeSynPara,byteArray.data(),sizeof(m_DDTBBECodeSynPara));

            m_pHAManager->GetParameter(m_ParaDDTBBEFrameSynID,byteArray);
            memcpy(&m_DDTBBEFrameSynPara,byteArray.data(),sizeof(m_DDTBBEFrameSynPara));

            m_pHAManager->GetParameter(m_ParaDDTBBESubFrameSynID,byteArray);
            memcpy(&m_DDTBBESubFrameSynPara,byteArray.data(),sizeof(m_DDTBBESubFrameSynPara));
        }
      */
        if(m_pStateMgr != NULL)
        {
            DWORD dwLen = sizeof(SSWITCHDCUStatu);
            m_pSswitchDcuStatus  = (SSWITCHDCUStatu*)m_pStateMgr->FindOneItem(m_StatusCodeSynID, dwLen);
            if(m_pSswitchDcuStatus == NULL || dwLen != sizeof(SSWITCHDCUStatu))
                return;
        }
        //启动定时器 用于显示二维矩阵中的灯光使用
        m_TimerID = startTimer(500);

        //不使能
        EnabledControls(false);
    }
    else if(Type == 1)//取宏里参数
    {
        if(pMacroObj != NULL)
        {
            QByteArray byteArray;
            pMacroObj->GetParameterBlock(m_ParaCodeSynID, byteArray);
            memcpy(&m_SswitchDcuPara, byteArray.data(), sizeof(SSWITCHDCUPara));
        }
        ui->pushButton_Modify->setVisible(false);
        ui->pushButton_Set->setVisible(false);

        //显示参数
        show();
    }
}

void SswitchDcuGUIDlg::SetObjInfo(TLxTsspObjectInfo ObjectInfo)
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
        qDebug() << strInfo;
        CLog::addLog(strInfo);
        return;
    }
    ITEM* pItem = m_DeviceMap.getItem(MODULENAME);
    if (pItem != NULL && pItem->vSubItem.size() > 0)
    {
        SUBITEM sItem = pItem->vSubItem.at(0);
        /**************************状态信息设置的ID************************/
        m_ParaCodeSynID = GetID(sItem.usStationID, sItem.ucSubSysID, sItem.usDeviceID,
                                                               sItem.usTID, sItem.ucSN);
        /*************************状态信息设置的ID*************************/

        /**************************状态信息设置的ID************************/
        m_StatusCodeSynID = GetID(sItem.usStationID, sItem.ucSubSysID, sItem.usDeviceID,
                                                               sItem.usStateType, sItem.ucSN);
        /*************************状态信息设置的ID*************************/
    }

    //日志
    m_pILxTsspLogManager = pObjectManager->GetLogManager();
    m_pStateMgr = pObjectManager->GetStateManager();
    m_pILxTsspMessageManager = pObjectManager->GetMessageManager();
    m_pPublicInfo = pObjectManager->GetPublicInfoManager();
}

//保存参数宏
int SswitchDcuGUIDlg::SaveToMacro(ILxTsspParameterMacro* pMacroObj)
{
    //获取设置的通道
    QByteArray ba;
    ba.resize(sizeof(TEST_SWMATPara));
    int nCount = m_GroupSwitchList.size();
    for (int i = 0; i < nCount; i ++)
    {
        GroupSwitch * pSwitch = m_GroupSwitchList.at(i);
        if (pSwitch != NULL)
        {
            //将数字转化成16进制数字
            int index = pSwitch->getCurrentIndex();
            UCHAR ucValue = 0;
            QMap<int, UCHAR>::const_iterator itr = m_reader.m_GroupList.at(i).paramValueMap.find(index);
            if (itr != m_reader.m_GroupList.at(i).paramValueMap.end())
                ucValue = itr.value();
            ba.append(ucValue);
        }
    }
    pMacroObj->UpdateParameterBlock(m_ParaCodeSynID, ba);

    //日志信息
    QString strLog;
    strLog += QString("%1: 保存宏成功！").arg(MODULENAME);
    CLog::addLog(strLog);

    //不使能控件
    EnabledControls(false);
    //m_TimerID = startTimer(500);
}

//定时器
void SswitchDcuGUIDlg::timerEvent(QTimerEvent *event)
{
    show();
}

bool SswitchDcuGUIDlg::initCmdInfo(QString strModuleName)
{
    bool bRet = m_DeviceMap.loadXML();
    if (!bRet)
    {
        QString strInfo = QString("load device map failed %1").arg(MODULENAME);
        qDebug() << strInfo;
        CLog::addLog(strInfo);
        return false;
    }
    ITEM* item = m_DeviceMap.getItem(strModuleName);
    if (item != NULL)
    {
        m_usC = item->vSubItem[0].usCMD;
        m_usTID = item->vSubItem[0].usTID;
    }
    return true;
}

void SswitchDcuGUIDlg::initLabelAndCombox()
{
    QString strOut;
    int nGroupCount = m_reader.m_GroupList.size();
    if (nGroupCount > 0)
    {
        strOut = m_reader.m_GroupList.at(0).strName;
    }
    //信息提示框
    QLabel *pLabel = new QLabel(ui->widget);//绘制输出提示信息
    pLabel->setFixedWidth(120);
    pLabel->setText(strOut + "    ");
    //列表选项
    m_pCommbox = new QComboBox(ui->widget);
    m_pCommbox->setFixedWidth(120);
//    m_pComType->setEditable(true);  //设置可编辑
    //下拉选项高度
    m_pCommbox->setStyleSheet("border: 1px solid darkgray;"
                    "selection-background-color: lightgray;"
                    "QComboBox QAbstractItemView::item{height:30px;}");
    m_pCommbox->clear();
    //将配置信息显示在列表框中
    //m_pCommbox->setModel(&ui->widget);
    //m_pCommbox->setView(&ui->widget);
    if (nGroupCount > 0)
    {
        for (int i = 0; i < m_reader.m_GroupList.at(0).paramNameList.size(); i++)
        {
            m_pCommbox->addItem(m_reader.m_GroupList.at(0).paramNameList.at(i));
        }
    }
    //当列表框内容发生改变后出发数据改变
    QObject::connect(m_pCommbox, SIGNAL(currentIndexChanged(int)), this, SLOT(onChooseAccount(int )));
    //计算移动的位置
    QRect rect = ui->widget->rect();
    int cX = rect.right() - 120 - 10;
    int cY = rect.height() / 2 - 10;
    pLabel->move(cX - pLabel->width() -10, cY);
    m_pCommbox->move(cX, cY);
}

void SswitchDcuGUIDlg::show()
{
    if(m_pSswitchDcuStatus != NULL)
    {
        m_SswitchDcuPara = m_pSswitchDcuStatus->tParam;
    }
    else
    {
        DWORD stLen = sizeof(SSWITCHDCUStatu);
        if(m_pStateMgr != NULL)
            m_pSswitchDcuStatus  = (SSWITCHDCUStatu*)m_pStateMgr->FindOneItem(m_StatusCodeSynID, stLen);
    }

    if (m_pSswitchDcuStatus == NULL)
        return;

    /********************设备是否在线************************/
    m_bOnLine = CCommFunc::getInstance()->getSswitchStatus();
    if (!m_bOnLine)
    {
        setOfflineStatusValue();
        return;
    }
    /********************设备是否在线************************/

    //显示主控还是分控
    bool bCtrlMode = m_pSswitchDcuStatus->ucConMode == 0x00 ? true : false;
    bool bNormal = m_pSswitchDcuStatus->ucSelfCheckStatu == 0x00 ? true : false;
    setType(bCtrlMode, bNormal);
    UCHAR * pChar = (UCHAR*)&m_pSswitchDcuStatus->tParam;
    int nCount = m_GroupSwitchList.size();
    for (int i = 0; i < nCount; i ++)
    {
        UCHAR ucValue = static_cast<UCHAR>(*(pChar++));
        GroupSwitch * pSwitch = m_GroupSwitchList.at(i);
        if (pSwitch != NULL)
        {
            int nCurIndex = 0;
            for (int j = 0; j < m_reader.m_GroupList.at(i).paramValueMap.size(); j++)
            {
                if (ucValue == m_reader.m_GroupList.at(i).paramValueMap.value(j))
                {
                    nCurIndex = j;
                    break;
                }
            }
            pSwitch->setCurrentIndex(nCurIndex);
        }
    }
    //显示数据内容到编辑框中
    short sValue1 = m_pSswitchDcuStatus->tParam.sAttenuation1;
    QString strValue1 = QString("%1").arg(sValue1);
    ui->lineEdit_CtrlSJ1->setText(strValue1);
    ui->lineEdit_SetCtrlSJ1->setText(strValue1);
    short sValue2 = m_pSswitchDcuStatus->tParam.sAttenuation2;
    QString strValue2 = QString("%1").arg(sValue2);
    ui->lineEdit_CtrlSJ2->setText(strValue2);
    ui->lineEdit_SetCtrlSJ2->setText(strValue2);
}

//设置输入格式
void SswitchDcuGUIDlg::InitInput()
{
    QRegExp regExp("[A-Fa-f0-9]{1,8}");
//    ui->lineEdit_FrmSyncCode->setValidator(new QRegExpValidator(regExp, this));

//    //正整数
    QRegExp regExp2("[0-9]{1,16}");
    ui->lineEdit_CtrlSJ1->setValidator(new QRegExpValidator(regExp2, this));
    ui->lineEdit_CtrlSJ2->setValidator(new QRegExpValidator(regExp2, this));

    ui->lineEdit_SetCtrlSJ1->setValidator(new QRegExpValidator(regExp2, this));
    ui->lineEdit_SetCtrlSJ2->setValidator(new QRegExpValidator(regExp2, this));
}

void SswitchDcuGUIDlg::EnabledControls(bool bFlag)
{
    //设置Combox是否可用
    int nCount = m_GroupSwitchList.size();
    for (int i = 0; i < nCount; i ++)
    {
        GroupSwitch * pSwitch = m_GroupSwitchList.at(i);
        if (pSwitch != NULL)
        {
            pSwitch->enabledCtrl(bFlag);
        }
    }
    ui->pushButton_Set->setEnabled(bFlag);
    ui->lineEdit_SetCtrlSJ1->setEnabled(bFlag);
    ui->lineEdit_SetCtrlSJ2->setEnabled(bFlag);
    m_pCommbox->setEnabled(bFlag);
}

void SswitchDcuGUIDlg::setType(bool bMain, bool bNormal)
{
    QString strType = bMain ? QString(tr(LOCAL_CTRL)) : QString(tr(REMOTE_CTRL));
    QString strStatus = bNormal ? QString(tr(NORMA)) : QString(tr(FAULT));

    ui->lineEdit_CrrlMode->setText(strType);
    ui->lineEdit_SelCheckStatus->setText(strStatus);
}

void SswitchDcuGUIDlg::setOfflineStatusValue()
{
    QString strEmpty("--");
    ui->lineEdit_CrrlMode->setText(strEmpty);
    ui->lineEdit_SelCheckStatus->setText(strEmpty);
    ui->lineEdit_CtrlSJ1->setText(strEmpty);
    ui->lineEdit_CtrlSJ2->setText(strEmpty);

    ui->lineEdit_SetCtrlSJ1->setText(strEmpty);
    ui->lineEdit_SetCtrlSJ2->setText(strEmpty);
}

bool SswitchDcuGUIDlg::checkPara()
{
    QMessageBox msgBox;
    QString strVal1 = ui->lineEdit_SetCtrlSJ1->text();
    QString strVal2 = ui->lineEdit_SetCtrlSJ2->text();
    //数控衰减器1
    if (strVal1.isEmpty())
    {
        msgBox.setToolTip(MODULENAME);
        msgBox.setText("请输入数控衰减器1数值！");
        msgBox.exec();
        return false;
    }
    //数控衰减器1：\n0~30dB
    if (strVal1.toShort() < 0 || strVal1.toShort() > 30)
    {
        msgBox.setToolTip(MODULENAME);
        msgBox.setText("数控衰减器1正确范围[0～30]db，请重新输入!");
        msgBox.exec();
        return false;
    }
    //数控衰减器2
    if (strVal2.isEmpty())
    {
        msgBox.setToolTip(MODULENAME);
        msgBox.setText("请输入数控衰减器2数值！");
        msgBox.exec();
        return false;
    }
    //数控衰减器2：\n0~30dB
    if (strVal2.toShort() < 0 || strVal2.toShort() > 30)
    {
        msgBox.setToolTip(MODULENAME);
        msgBox.setText("数控衰减器2正确范围[0～30]db，请重新输入!");
        msgBox.exec();
        return false;
    }
    return true;
}

void SswitchDcuGUIDlg::closeEvent(QCloseEvent *event)
{
    event->accept();
}

void SswitchDcuGUIDlg::init()
{
    QDir dir;
    QString strPath = dir.currentPath();
    QString strConfigPath = strPath + "/TM/System/ModuleConfig/SswitchDcuConfig.xml";
    bool bRet = m_reader.loadXML(strConfigPath);
    if (!bRet)
    {
        QString strInfo = QString("read xml config failed %1").arg(strConfigPath);
        qDebug() << strInfo;
        CLog::addLog(strInfo);
        return;
    }

    //初始化指令信息
    initCmdInfo(MODULENAME);

    //初始化界面上显示的控件
    initCtrl();
}

void SswitchDcuGUIDlg::initCtrl()
{
    //ui->widget->hide();
    QVBoxLayout *hLayout = new QVBoxLayout;
    int nGroupCount = m_reader.m_GroupList.size();
    for (int i = 0; i < nGroupCount;  i++)
    {
        int nParamSize = m_reader.m_GroupList.at(i).paramNameList.size();
        QRectF rect(20, 10, 800, nParamSize * 30);
        QString strOut = m_reader.m_GroupList.at(i).strName;
        GroupSwitch * pSwitch = new GroupSwitch(rect, m_reader.m_GroupList.at(i).paramNameList, strOut, ui->widget);
        pSwitch->showCombox(false);
        pSwitch->setOutContext("");
        m_GroupSwitchList.push_back(pSwitch);
        hLayout->addWidget(pSwitch);
    }
    //右边绘制文字和combox下拉列表
    initLabelAndCombox();

    hLayout->addStretch();
    ui->widget->setLayout(hLayout);
}

QSize SswitchDcuGUIDlg::sizeHint() const
{
    return QSize(this->width(), this->height());
}

void SswitchDcuGUIDlg::SendMessage(char* pData, unsigned short  usLen, unsigned short usC, unsigned short usTID)
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
    memcpy(message.pData + sizeof(TLxTsspMessageHeader), &subHeader, sizeof(TLxTsspSubMsgHeader));
    memcpy(message.pData + sizeof(TLxTsspMessageHeader) + sizeof(TLxTsspSubMsgHeader),
           pData, usLen);
    message.iDataLen = sizeof(TLxTsspMessageHeader) + sizeof(TLxTsspSubMsgHeader) + usLen;

    int iRet = m_pILxTsspMessageManager->FireMessage(message);
    delete[] message.pData;
}

void SswitchDcuGUIDlg::on_pushButton_Modify_clicked()
{
    if (!m_bOnLine)
    {
        QString strMsg = QString(tr("%1处于离线状态，无法进行更改设置！")).arg(MODULENAME);
        QMessageBox::information(this, tr("信息提示"), strMsg);
        return;
    }

    QString strText = ui->pushButton_Modify->text();
    if (strText == tr("更改"))
    {
        ui->pushButton_Modify->setText(tr("恢复"));
        EnabledControls(true);
        killTimer(m_TimerID);
        bool bRet = m_pCommbox->currentIndex() == 0/*A不能编辑*/ ? false : true;
        ui->lineEdit_SetCtrlSJ1->setEnabled(bRet);
        ui->lineEdit_SetCtrlSJ2->setEnabled(bRet);
    }
    else
    {
        ui->pushButton_Modify->setText(tr("更改"));
        EnabledControls(false);
        m_TimerID = startTimer(500);
    }
}

void SswitchDcuGUIDlg::on_pushButton_Set_clicked()
{
    bool bRet = checkPara();
    if (!bRet)
        return;

    //获取设置的通道
    QByteArray ba;
    int nCount = m_GroupSwitchList.size();
    for (int i = 0; i < nCount; i ++)
    {
        //GroupSwitch * pSwitch = m_GroupSwitchList.at(i);
        if (m_pCommbox != NULL)
        {
            //将数字转化成16进制数字
            int index = m_pCommbox->currentIndex();
            UCHAR ucValue = 0;
            QMap<int, UCHAR>::const_iterator itr = m_reader.m_GroupList.at(i).paramValueMap.find(index);
            if (itr != m_reader.m_GroupList.at(i).paramValueMap.end())
                ucValue = itr.value();
            ba.append(ucValue);
        }
    }
    char szTmp[3];
    ushort usVal1 = ui->lineEdit_SetCtrlSJ1->text().toShort();
    memcpy(szTmp, &usVal1, 2);
    ba.append(szTmp, 2);
    ushort usVal2 = ui->lineEdit_SetCtrlSJ2->text().toShort();
    memcpy(szTmp, &usVal2, 2);
    ba.append(szTmp, 2);

    //取当前设置的参数
    if(m_pHAManager != NULL)
    {
        QByteArray byteArray;
        //分集合成参数
        byteArray.resize(sizeof(SSWITCHDCUPara));
        memcpy(byteArray.data(), &m_SswitchDcuPara, sizeof(SSWITCHDCUPara));
        m_pHAManager->SetParameter(m_ParaCodeSynID, byteArray);
        SendMessage(ba.data(), ba.length(), m_usC, m_usTID);
    }
    //日志信息
    QString strLog;
    strLog = QString("%1: 设置参数成功！").arg(MODULENAME);
    CLog::addLog(strLog);
    //m_TimerID = startTimer(500);
    on_pushButton_Modify_clicked();
}

void SswitchDcuGUIDlg::onChooseAccount(int nIndex)
{
    //qDebug() << "Select index :" << nIndex;
    //m_pCommbox->setCurrentIndex(nIndex);
    bool bRet = (nIndex == 0) ? false : true;
//    int nCount = m_GroupSwitchList.size();
//    for (int i = 0; i < nCount; i ++)
//    {
//        GroupSwitch * pSwitch = m_GroupSwitchList.at(i);
//        if (pSwitch != NULL)
//        {
//            pSwitch->setCurrentIndex(nIndex);
//        }
//    }
    ui->lineEdit_SetCtrlSJ1->setEnabled(bRet);
    ui->lineEdit_SetCtrlSJ2->setEnabled(bRet);
}
