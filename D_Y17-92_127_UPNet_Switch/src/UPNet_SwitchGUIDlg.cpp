#include "UPNet_SwitchGUIDlg.h"
#include "ui_UPNet_SwitchGUIDlg.h"
#include <QCloseEvent>
#include <QMessageBox>
#include <QDateTime>
#include <QPainter>
#include <QDir>
#include <QDebug>
#include <QHBoxLayout>
#include <QPushButton>
#include <QMenu>
#include <QListWidget>
#include <QListWidgetItem>

#include "../../Common/CommonFunction.h"

//#define SHOW_STATUS     //wl modify 是否在界面上显示状态信息

extern quint64 GetID(ushort usStationID, uchar ucSubSysID, ushort usDeviceID, ushort usType,
              uchar ucSn);

UPNet_SwitchGUIDlg::UPNet_SwitchGUIDlg(QWidget *parent) : QDialog(parent),
    ui(new Ui::UPNet_SwitchGUIDlg)
{
    ui->setupUi(this);
    m_pStateMgr = NULL;
    m_pILxTsspMessageManager = NULL;
    m_pILxTsspLogManager = NULL;
    m_pPublicInfo = NULL;
    m_pHAManager = NULL;

    m_IFSWMatStatuSyn = NULL;
    m_pIFSMatResponse = NULL;

    m_bOnLine = false;

    m_nFontSize = 12;

    m_pTextCodec = NULL;

    setAttribute(Qt::WA_DeleteOnClose);
    //setMinimumSize(800, 550);
    init();
}

UPNet_SwitchGUIDlg::~UPNet_SwitchGUIDlg()
{
    killTimer(m_TimerID);
    delete ui;
}

void UPNet_SwitchGUIDlg::setText(const QString& strTitle,ParaStruct xmlsetText[3][60],QString MapMean[50][30],int MapNum[50],uchar Type,ILxTsspParameterMacro* pMacroObj)
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
}

//初始化对话框
void UPNet_SwitchGUIDlg::InitDLG(int Type, ILxTsspParameterMacro* pMacroObj)
{
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
            DWORD dwLen = sizeof(HK_IFSWMatStatu);
            m_IFSWMatStatuSyn  = (HK_IFSWMatStatu*)m_pStateMgr->FindOneItem(m_StatusCodeSynID, dwLen);
            if(m_IFSWMatStatuSyn == NULL || dwLen != sizeof(HK_IFSWMatStatu))
                return;

#ifdef SHOW_STATUS
            dwLen = sizeof(HK_IFSWMatResponse);
            m_pIFSMatResponse = (HK_IFSWMatResponse*)m_pStateMgr->FindOneItem(m_ParaResponseID, dwLen);
            if (m_pIFSMatResponse == NULL || dwLen != sizeof(HK_IFSWMatResponse))
                return;
#endif // SHOW_STATUS
        }
        //启动定时器 用于显示二维矩阵中的灯光使用
        m_TimerID = startTimer(500);
    }
    else if(Type == 1)//取宏里参数
    {
        if(pMacroObj != NULL)
        {
            QByteArray byteArray;
            pMacroObj->GetParameterBlock(m_ParaCodeSynID, byteArray);
            memcpy(&m_IFSWMatParamSyn, byteArray.data(), sizeof(HK_IFSWMatPara));
        }
        m_pBtnModify->setVisible(false);
        m_pBtnSetting->setVisible(false);

        //显示参数
        showLight();
    }
}

void UPNet_SwitchGUIDlg::SetObjInfo(TLxTsspObjectInfo ObjectInfo)
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
        QString strInfo = QString("load device map failed %1").arg(MODULENAME);
        qDebug() << strInfo;
        CLog::addLog(strInfo);
        return;
    }
    ITEM* pItem = m_DeviceMap.getItem(MODULENAME);
    if (pItem != NULL && pItem->vSubItem.size() > 0)
    {
        SUBITEM sItem = pItem->vSubItem.at(0);
        /**************************获取参数设置的ID************************/
        m_ParaCodeSynID = GetID(sItem.usStationID, sItem.ucSubSysID, sItem.usDeviceID,
                                                                sItem.usTID, sItem.ucSN);
        /*************************获取参数设置的ID*************************/

        /**************************获取状态设置的ID************************/
        m_StatusCodeSynID = GetID(sItem.usStationID, sItem.ucSubSysID, sItem.usDeviceID,
                                                                sItem.usStateType, sItem.ucSN);
        /*************************获取状态设置的ID*************************/
    }
#ifdef SHOW_STATUS
    /*************************获取想响应参数ID*************************/
    m_ParaResponseID = GetID(m_ObjectInfo.usStationID,
                             m_ObjectInfo.ucSubSysID, m_ObjectInfo.usLocalDID,
                             m_ObjectInfo.usObjectID, m_ObjectInfo.ucSN + 1);
    /*************************获取想响应参数ID*************************/
#endif // SHOW_STATUS

    //日志
    m_pILxTsspLogManager = pObjectManager->GetLogManager();
    m_pStateMgr = pObjectManager->GetStateManager();
    m_pILxTsspMessageManager = pObjectManager->GetMessageManager();
    m_pPublicInfo = pObjectManager->GetPublicInfoManager();
}

//保存参数宏
int UPNet_SwitchGUIDlg::SaveToMacro(ILxTsspParameterMacro* pMacroObj)
{
    //获取设置的通道
    QList<QString> channelList = getShowInfo();
    QByteArray ba;
    ba.resize(sizeof(HK_IFSWMatPara));
    for (int i = 0; i < channelList.size(); i++)
    {
        //将数字转化成16进制数字
        QString strChannel = channelList.at(i);
        bool ok;
        int dec = strChannel.toInt(&ok, 10);
        QString hex = QString::number(dec, 16).toUpper();
        UCHAR out = hex.toInt(0, 16);
        ba.append(out);
    }
    pMacroObj->UpdateParameterBlock(m_ParaCodeSynID, ba);

    //日志信息
    QString strLog;
    strLog += QString("%1: 保存宏成功！").arg(MODULENAME);
    CLog::addLog(strLog);
}

//定时器
void UPNet_SwitchGUIDlg::timerEvent(QTimerEvent *event)
{
    if (m_TimerID == event->timerId())
        showLight();
#ifdef SHOW_STATUS
    else if (m_StatusTimerID == event->timerId())
        setStatus();
#endif // SHOW_STATUS
}

void UPNet_SwitchGUIDlg::OnSaveBtn()
{
    //保存配置信息到文件中
    QString strName = m_pComType->currentText();
    if (strName.isEmpty())
    {
        QMessageBox::information(this, tr("信息提示"), tr("请输入要保存的名称!"), QMessageBox::Ok);
        m_pComType->setFocus();
        return;
    }
    //获取界面上的数据内容
    for (int i = 0; i < m_IFMList.size(); i++)
    {
        if (strName == m_IFMList.at(i).strName)
        {
            //当前名称在列表中存在更新列表
            m_IFMList.removeAt(i);
            break;
        }
    }
    //如果不存在则添加到列表中
    IFMSHOWDATA data;
    data.strName = strName;
    QList<QString> strList = getShowInfo();
    data.infoList.swap(strList);
    m_IFMList.push_back(data);

    //获取控件中显示的数据然后写入到文件中
    saveData2File(m_strFilePath, "");

    //更新列表框显示效果
    updateCombo();
}

void UPNet_SwitchGUIDlg::OnBtnDel()
{
    QString strName = m_pComType->currentText();
    if (strName.isEmpty())
    {
        QMessageBox::information(this, tr("信息提示"), tr("请选择要删除的名称!"), QMessageBox::Ok);
        return;
    }
    int nRet = QMessageBox::information(this, tr("信息提示"), tr("是否删除当前配置？"),
                             QMessageBox::Ok | QMessageBox::Cancel);
    if (nRet == QMessageBox::Ok)
    {
        //获取控件中显示的数据然后写入到文件中
        saveData2File(m_strFilePath, strName);

        loadSaveInfo(m_strFilePath);
        //更新列表框显示效果
        updateCombo();
    }
}

void UPNet_SwitchGUIDlg::OnBtnModify()
{
    if (!m_bOnLine)
    {
        QString strMsg = QString(tr("%1处于离线状态，无法进行更改设置！")).arg(MODULENAME);
        QMessageBox::information(this, tr("信息提示"), strMsg);
        return;
    }

    QString strText = m_pBtnModify->text();
    if (strText == tr("更改"))
    {
        m_pBtnModify->setText(tr("恢复"));
        m_pBtnSetting->setEnabled(true);
        m_pComType->setEnabled(true);
        m_pBtnSave->setEnabled(true);
        m_pBtnDel->setEnabled(true);
        killTimer(m_TimerID);
    }
    else
    {
        m_pBtnModify->setText(tr("更改"));
        m_pBtnSetting->setEnabled(false);
        m_pComType->setEnabled(false);
        m_pBtnSave->setEnabled(false);
        m_pBtnDel->setEnabled(false);
        m_TimerID = startTimer(500);
    }
}

void UPNet_SwitchGUIDlg::OnBtnSetting()
{
    //获取设置的通道
    QList<QString> channelList = getShowInfo();
    QByteArray ba;
    for (int i = 0; i < channelList.size(); i++)
    {
        //将数字转化成16进制数字
        QString strChannel = channelList.at(i);
        bool ok;
        int dec = strChannel.toInt(&ok, 10);
        QString hex = QString::number(dec, 16).toUpper();
        UCHAR out = hex.toInt(0,16);
        ba.append(out);
    }

    //取当前设置的参数
    if(m_pHAManager != NULL)
    {
        QByteArray byteArray;
        //分集合成参数
        byteArray.resize(sizeof(HK_IFSWMatPara));
        memcpy(byteArray.data(), &m_IFSWMatParamSyn, sizeof(HK_IFSWMatPara));
        m_pHAManager->SetParameter(m_ParaCodeSynID, byteArray);
        SendMessage(ba.data(), ba.length(), m_usC, m_usTID);
    }

#ifdef SHOW_STATUS
    //不需要在界面上显示状态信息
    m_StatusTimerID = startTimer(500);
#endif // SHOW_STATUS

    //日志信息
    QString strLog;
    strLog = QString("%1: 设置参数成功！").arg(MODULENAME);
    CLog::addLog(strLog);
    OnBtnModify();
}

void UPNet_SwitchGUIDlg::onChooseAccount(const QString &strText)
{
    qDebug() << strText;
    //获取当前选择的列表
    QStringList strList;
    for (int i = 0; i < m_IFMList.size(); i++)
    {
        QString strName = m_IFMList.at(i).strName;
        if (strText == strName)
        {
            strList = m_IFMList.at(i).infoList;
            break;
        }
    }
    if (strList.size() <= 0)
        return;
    //
    setShowInfo(strList);
}

void UPNet_SwitchGUIDlg::setShowInfo(QStringList infoList)
{
    int nInfoCount = infoList.size();
    qDebug() << "list count " << nInfoCount;
    for (int i = 0; i < m_ItemList.size(); i++)
    {
        SignalItem * pItem = m_ItemList.at(i);
        if (pItem != NULL && i <= nInfoCount)
        {
            QString strIndex = infoList.at(i);
            //qDebug() << strIndex;
            //判断当前信息是否在menu中
            pItem->setInfo(strIndex);
        }
    }
}

bool UPNet_SwitchGUIDlg::initCmdInfo(QString strModuleName)
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

QList<QString> UPNet_SwitchGUIDlg::getShowInfo()
{
    QList<QString> infoList;
    for (int i = 0; i < m_ItemList.size(); i++)
    {
        QString str = m_ItemList.at(i)->getInfo();
        infoList.push_back(str);
    }
    return infoList;
}

void UPNet_SwitchGUIDlg::closeEvent(QCloseEvent *event)
{
    event->accept();
}

void UPNet_SwitchGUIDlg::init()
{
    QDir dir;
    QString strPath = dir.currentPath();
    QString strConfigPath = strPath + "/TM/System/ModuleConfig/UPNet_SwitchConfig.xml";
    bool bRet = m_reader.loadXML(strConfigPath);
    if (!bRet)
    {
        QString strInfo = QString("read xml config failed %1").arg(strConfigPath);
        qDebug() << strInfo;
        CLog::addLog(strInfo);
        return;
    }

    m_pTextCodec = QTextCodec::codecForName("utf-8");

    //组选框中的二维矩阵和右侧的信息
    initMatrixAndOther();

    //初始化下面的列表框和按钮
    initCommboAndBtn();

    //加载保存的配置信息
    m_strFilePath = strPath + "/TM/System/ModuleConfig/UPNet_SwitchSave.data";
    loadSaveInfo(m_strFilePath);

    //更新列表框的显示
    updateCombo();

    //初始化指令信息
    initCmdInfo(MODULENAME);

    m_pComType->setEnabled(false);
    m_pBtnSave->setEnabled(false);
    m_pBtnDel->setEnabled(false);
}

QWidget *UPNet_SwitchGUIDlg::getSignalBoard(int boardHeight, int nNum,
                                    NUM_PARAM &Num1, NUM_PARAM &Num2, NUM_PARAM &menu)
{
    //顶部
    QWidget *signalBoard = new QWidget;
    QVBoxLayout *boardLayout = new QVBoxLayout;
    int itemHeight = boardHeight / nNum;
    bool bShowMenu1 = (Num1.bFix == false);
    bool bShowMenu2 = (Num2.bFix == false);
    SignalItem *topItem = new SignalItem(itemHeight, bShowMenu1, Num1.paramList.at(0).strDesc,
                                         Num1.paramList.at(0).strValue,
                                         LineLabel::TOP, bShowMenu2, Num2.paramList.at(0).strValue,
                                         Num2.paramList.at(0).strDesc, menu.paramList);
    topItem->SetLabelFontSize(m_nFontSize);
    boardLayout->addWidget(topItem);

    m_ItemList.push_back(topItem);

    //中间
    for (int var = 1; var < nNum - 1; ++var) {
        SignalItem *cItem = new SignalItem(itemHeight, bShowMenu1, Num1.paramList.at(var).strDesc,
                                           Num1.paramList.at(var).strValue,
                                           LineLabel::CENTER, bShowMenu2, Num2.paramList.at(var).strValue,
                                           Num2.paramList.at(var).strDesc, menu.paramList);
        cItem->SetLabelFontSize(m_nFontSize);
        boardLayout->addWidget(cItem);
        m_ItemList.push_back(cItem);
    }

    //底部
    SignalItem *bottomItem = new SignalItem(itemHeight, bShowMenu1, Num1.paramList.at(nNum - 1).strDesc,
                                            Num1.paramList.at(nNum - 1).strValue,
                                            LineLabel::BOTTOM, bShowMenu2, Num2.paramList.at(nNum - 1).strValue,
                                            Num2.paramList.at(nNum - 1).strDesc, menu.paramList);
    bottomItem->SetLabelFontSize(m_nFontSize);
    boardLayout->addWidget(bottomItem);
    boardLayout->addStretch();
    boardLayout->setSpacing(0);
    signalBoard->setLayout(boardLayout);
    m_ItemList.push_back(bottomItem);
    return signalBoard;
}

void UPNet_SwitchGUIDlg::showLight()
{
    if(m_IFSWMatStatuSyn != NULL)
    {
        m_IFSWMatParamSyn = m_IFSWMatStatuSyn->tParam;
    }
    else
    {
        DWORD stLen = sizeof(HK_IFSWMatStatu);
        if(m_pStateMgr != NULL)
            m_IFSWMatStatuSyn  = (HK_IFSWMatStatu*)m_pStateMgr->FindOneItem(m_StatusCodeSynID, stLen);
    }

    if (m_IFSWMatStatuSyn == NULL)
        return;

    /********************设备是否在线************************/
    m_bOnLine = CCommFunc::getInstance()->getUpnet_switchStatus();
    if (!m_bOnLine)
    {
        setOfflineStatusValue();
        return;
    }
    /********************设备是否在线************************/

    //显示主控还是分控
    bool bCtrlMode = m_IFSWMatStatuSyn->ucConMode == 0x00 ? false : true;
    bool bNormal = m_IFSWMatStatuSyn->ucSelfCheckStatu == 0x00 ? true : false;
    setType(bCtrlMode, bNormal);
    QStringList valList;
    UCHAR * pChar = (UCHAR*)&m_IFSWMatStatuSyn->tParam;
    for (int nRow = 1; nRow <= m_nMatrixNum; nRow++)
    {
        int nCol = static_cast<int>(*(pChar++));
        setLight(nRow, nCol);
        //qDebug() << nRow << "  " << nCol;
        valList.push_back(QString::number(nCol));
    }
    //刷新界面上右边的显示菜单
    setShowInfo(valList);
}

void UPNet_SwitchGUIDlg::setOfflineStatusValue()
{

}

void UPNet_SwitchGUIDlg::initMatrixAndOther()
{
    m_nMatrixNum = m_reader.m_IMFConfig.matrixNum;
    int m_nNum = m_reader.m_IMFConfig.matrixNum;
    static const int s_DIALOG_HEIGHT = 400;//400最佳，TODO:因为Matrix不适应32个情况

    //加入矩阵
    QHBoxLayout *groupHLayout = new QHBoxLayout;
    m_pMatrix = new Matrix(m_nNum);
    m_pMatrix->SetFontSize(m_nFontSize);
    m_pMatrix->setFixedWidth(s_DIALOG_HEIGHT);
    groupHLayout->addWidget(m_pMatrix);

    //加入中间分割线
    groupHLayout->addSpacing(10);
    QWidget *splitLine = new QWidget;
    splitLine->setFixedWidth(2);
    splitLine->setStyleSheet("QWidget{ border: 0px; border-left: 1px solid #D5D5D5; background-color: #FFF; }");
    groupHLayout->addWidget(splitLine);

    //绘制右侧信息
    groupHLayout->addWidget(getSignalBoard(s_DIALOG_HEIGHT, m_nNum,
                                           m_reader.m_IMFConfig.num1,
                                           m_reader.m_IMFConfig.num2,
                                           m_reader.m_IMFConfig.menu));
    ui->groupBox->setLayout(groupHLayout);
    ui->groupBox->setEnabled(true);
}

void UPNet_SwitchGUIDlg::initCommboAndBtn()
{
    QHBoxLayout *bottomLayout = new QHBoxLayout;

#ifdef SHOW_STATUS
    //去掉左下角的响应信息和显示
    QLabel *lblStatus = new QLabel(tr("响应状态:"));
    bottomLayout->addWidget(lblStatus);
    m_pEditStatus = new QLineEdit;
    m_pEditStatus->setReadOnly(true);
    m_pEditStatus->setFixedWidth(200);
    bottomLayout->addWidget(m_pEditStatus);
#endif // SHOW_STATUS

    bottomLayout->addStretch();

    QLabel *lbl = new QLabel(tr("名称:"));
    bottomLayout->addWidget(lbl);

    m_pComType = new QComboBox;
    m_pComType->setFixedWidth(120);
    m_pComType->setEditable(true);  //设置可编辑
    //下拉选项高度
    m_pComType->setStyleSheet("border: 1px solid darkgray;"
                "selection-background-color: lightgray;"
                "QComboBox QAbstractItemView::item{height:30px;}");
    lbl->setBuddy(m_pComType);
    bottomLayout->addWidget(m_pComType);

    m_pBtnSave = new QPushButton(tr("保存"));
    bottomLayout->addWidget(m_pBtnSave);

    m_pBtnDel = new QPushButton(tr("删除"));
    bottomLayout->addWidget(m_pBtnDel);

    m_pBtnModify = new QPushButton(tr("更改"));
    bottomLayout->addWidget(m_pBtnModify);

    m_pBtnSetting = new QPushButton(tr("设置"));
    m_pBtnSetting->setEnabled(false);
    bottomLayout->addWidget(m_pBtnSetting);

    ui->verticalLayout->addLayout(bottomLayout);
    ui->verticalLayout->setEnabled(true);

    QObject::connect(m_pBtnSave, SIGNAL(clicked()), this, SLOT(OnSaveBtn()));
    QObject::connect(m_pBtnDel, SIGNAL(clicked()), this, SLOT(OnBtnDel()));
    QObject::connect(m_pBtnModify, SIGNAL(clicked()), this, SLOT(OnBtnModify()));
    QObject::connect(m_pBtnSetting, SIGNAL(clicked()), this, SLOT(OnBtnSetting()));
    QObject::connect(m_pComType, SIGNAL(currentIndexChanged(QString)), this, SLOT(on_cmb_currentIndexChanged(const QString& str)));
}

void UPNet_SwitchGUIDlg::loadSaveInfo(QString strPath)
{
    m_IFMList.clear();
    //解析文件中的数据
    QFile file(strPath);
    if (!file.open(QIODevice::ReadOnly))
    {
        QString strInfo = QString("load file %1 Failed!").arg(strPath);
        qDebug() << strInfo;
        CLog::addLog(strInfo);
       return;
    }
    //读取文件中的数据然后加载到列表中
    while (!file.atEnd())
    {
        QByteArray arr = file.readLine();
        QString strInfo(arr);
        if (strInfo.isEmpty())
            continue;
        //判断行是否存在回车，存在则去掉
        if (strInfo.right(1) == "\n" || strInfo.right(1) == "\r")
        {
            strInfo = strInfo.left(strInfo.length() - 1);
        }
        QStringList strList = strInfo.split(";");
        if (strList.size() == 2)
        {
            IFMSHOWDATA data;
            data.strName = m_pTextCodec->fromUnicode(strList.at(0));
            QString strTmp = strList.at(1);
            data.infoList = strTmp.split(",");
            m_IFMList.push_back(data);
        }
    }
    file.close();
}

void UPNet_SwitchGUIDlg::saveData2File(QString strPath, const QString &name)
{
    //解析文件中的数据
    QFile file(strPath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QString strInfo = QString("open file %1 Failed!").arg(strPath);
        qDebug() << strInfo;
        CLog::addLog(strInfo);
       return;
    }
    //保存数据内容到文件
    for (int m = 0; m < m_IFMList.size(); m++)
    {
        QString strName = m_IFMList.at(m).strName;
        if (strName == name)
        {
            continue;
        }
        QString strTst;
        int nListCount = m_IFMList.at(m).infoList.size();
        for (int n = 0; n < nListCount; n++)
        {
            strTst += m_IFMList.at(m).infoList.at(n);
            if (n != nListCount - 1)
                strTst += ",";
        }
        //保存名字乱码问题
        QString strName1 = m_pTextCodec->toUnicode(strName.toStdString().c_str(),
                                                   strName.toStdString().length());
        QString strData = QString("%1;%2\n").arg(strName1).arg(strTst);
        //qDebug() << strData;
        QByteArray ba;
        ba.append(strData);
        file.write(ba);
    }
    file.close();
}

void UPNet_SwitchGUIDlg::updateCombo()
{
    //
    m_pComType->clear();
    //将配置信息显示在列表框中
    QListWidget *pLW = new QListWidget();
    pLW->setObjectName("pccombox");
    m_pComType->setModel(pLW->model());
    m_pComType->setView(pLW);
    int nCount = m_IFMList.size();
    QString strName;
    m_pComType->addItem(tr(""));
    for (int i = 0; i < nCount; i++)
    {
        strName = m_IFMList.at(i).strName;
        pLW->addItem(strName);
    }
    //当列表框内容发生改变后出发数据改变
    QObject::connect(m_pComType, SIGNAL(currentTextChanged(QString)), this, SLOT(onChooseAccount(const QString &)));
}

QSize UPNet_SwitchGUIDlg::sizeHint() const
{
    return QSize(this->width(), this->height());
}

void UPNet_SwitchGUIDlg::SendMessage(char* pData, unsigned short  usLen, unsigned short usC, unsigned short usTID)
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

void UPNet_SwitchGUIDlg::setLight(int nRow, int nCol, bool bFlag /* = true */)
{
    if (m_pMatrix != NULL)
    {
        m_pMatrix->SetCellFlag(nRow, nCol, bFlag ? 1 : 0);
    }
}

void UPNet_SwitchGUIDlg::setType(bool bMain, bool bNormal)
{
    QString strType = bMain ? QString(tr(LOCAL_CTRL)) : QString(tr(REMOTE_CTRL));
    QString strStatus = bNormal ? QString(tr(NORMA)) : QString(tr(FAULT));
    if (m_pMatrix != NULL)
    {
        m_pMatrix->SetShowType(strType, strStatus);
    }
}

void UPNet_SwitchGUIDlg::setStatus()
{
#ifdef SHOW_STATUS
    QString strContent(tr("未响应"));
    UCHAR resp;
    if (m_pIFSMatResponse == NULL)
    {
        goto flag;
    }

    resp = m_pIFSMatResponse->ucResponse;
    switch(resp)
    {
    case 0xff:
        strContent = QString(tr("命令被正确执行"));
        break;
    case 0x61:
        strContent = QString(tr("非法码"));
        break;
    case 0x62:
        strContent = QString(tr("帧校验出错"));
        break;
    case 0x63:
        strContent = QString(tr("条件不具备不执行"));
        break;
    case 0x64:
        strContent = QString(tr("超限码"));
        break;
    }
flag:
    m_pEditStatus->setText(strContent);
    killTimer(m_StatusTimerID);
#else

#endif // SHOW_STATUS
}
