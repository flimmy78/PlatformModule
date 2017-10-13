#include "TMServerInterfaceGUI.h"
#include "ui_TMServerInterfaceGUIDlg.h"
#include "TMServerInterfaceGUIDlg.h"
#include <QCloseEvent>
#include <QMessageBox>
#include <QDateTime>

extern quint64 GetID(ushort usStationID, uchar ucSubSysID, ushort usDeviceID, ushort usType,
              uchar ucSn);
CDevCtlInterfaceGUIDlg::CDevCtlInterfaceGUIDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CDevCtlInterfaceGUIDlg)
{
    ui->setupUi(this);
    m_pStateMgr = NULL;
    m_pILxTsspMessageManager = NULL;
    m_pILxTsspLogManager = NULL;
    m_pPublicInfo = NULL;
    m_pHAManager = NULL;
    m_pTimer = NULL;
    m_timerComm = 0;
    m_timerStatu = 0;
    setAttribute(Qt::WA_DeleteOnClose);
}

//定时器
void CDevCtlInterfaceGUIDlg::timerEvent(QTimerEvent *)
{
    showDatagram();
}

void CDevCtlInterfaceGUIDlg::showDatagram()
{
    if(m_showStatuDatagram)//status
    {
        ui->textEdit_statuResponse->setText(getStrFromDatagram(g_showDatagram.m_datagramStatuRev));
        //ui->textEdit_statuCmd->setText(getStrFromDatagram(g_showDatagram.m_datagramStatuSend));
    }
    if(m_showCommDatagram)//comm
    {
        ui->textEdit_commResponse->setText(getStrFromDatagram(g_showDatagram.m_datagramCommRev));
        //ui->textEdit_commCmd->setText(getStrFromDatagram(g_showDatagram.m_datagramCommSend));
    }
}

void CDevCtlInterfaceGUIDlg::closeEvent(QCloseEvent *event)
{
    event->accept();
}

CDevCtlInterfaceGUIDlg::~CDevCtlInterfaceGUIDlg()
{
    killTimer(m_timerStatu);
    killTimer(m_timerComm);
    delete ui;
}

void CDevCtlInterfaceGUIDlg::setText(const QString& strTitle,ParaStruct xmlsetText[3][60],QString MapMean[50][30],int MapNum[50],uchar Type,ILxTsspParameterMacro* )
{
    m_strTitle = strTitle;
    setWindowTitle(strTitle);

    memcpy(m_DlgParaSetStruct, xmlsetText, sizeof(ParaStruct)*50*3);

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
    InitDLG(Type);
}

//初始化对话框
void CDevCtlInterfaceGUIDlg::InitDLG(int )
{
    m_pTimer = new QTimer(this);
    connect( m_pTimer, SIGNAL(timeout()), SLOT(on_timer_show_response()) );
    m_mapi64Str[0x0000000100000001] = "S标准TTC+DDT";
    m_mapi64Str[0x0000000100000021] = "S标准TTC+DDT中的DDT";
    m_mapi64Str[0x0000000000000020] = "S标准TTC+DDT中的TTC";
    m_mapi64Str[0x0000000200000002] = "S扩1";
    m_mapi64Str[0x0000000400000004] = "S扩2";
    m_mapi64Str[0x0000000800000008] = "一体化";
    m_mapi64Str[0x10] = "S频段调频遥测";
    m_mapi64Str[0x0000002000000020] = "S频段数传";
    m_mapi64Str[0x0000004000000040] = "C频段TTC";
    m_mapi64Str[0x0000010000000100] = "C扩2";
    m_mapi64Str[0x0000000000100010] = "UXB";
    QMap<quint64, QString>::iterator mapIter = m_mapi64Str.begin();
    for(; mapIter != m_mapi64Str.end(); ++mapIter)
        ui->comboBox_workModeSet->addItem(mapIter.value());

    ui->comboBox_type->addItem("遥测");
    ui->comboBox_type->addItem("数传I(I+Q)路(低速)");
    ui->comboBox_type->addItem("数传Q路(低速)");
    ui->comboBox_rotation->addItem("左旋");
    ui->comboBox_rotation->addItem("右旋");
    ui->comboBox_statisticCtl->addItem("开始");
    ui->comboBox_statisticCtl->addItem("终止");
    ui->comboBox_doppTestReport->addItem("开始测试");
    ui->comboBox_doppTestReport->addItem("终止测试");
    ui->comboBoxWorkWay->addItem(tr(PCR_WORK_WAY_1));
    ui->comboBoxWorkWay->addItem(tr(PCR_WORK_WAY_2));
    ui->comboBoxWorkWay->addItem(tr(PCR_WORK_WAY_3));
    ui->comboBoxWorkWay->addItem(tr(PCR_WORK_WAY_4));
    ui->comboBoxWorkWay->addItem(tr(PCR_WORK_WAY_5));
    ui->comboBoxWorkWay->addItem(tr(PCR_WORK_WAY_6));
    ui->comboBoxWorkWay->addItem(tr(PCR_WORK_WAY_7));
    ui->comboBoxWorkWay->addItem(tr(PCR_WORK_WAY_8));
    ui->comboBoxWorkWay->addItem(tr(PCR_WORK_WAY_9));
    ui->comboBoxWorkWay->addItem(tr(PCR_WORK_WAY_10));
    ui->comboBoxWorkWay->addItem(tr(PCR_WORK_WAY_11));
    ui->comboBoxWorkWay->addItem(tr(PCR_WORK_WAY_12));
    ui->comboBoxWorkWay->addItem(tr(PCR_WORK_WAY_13));
    ui->comboBoxWorkWay->addItem(tr(PCR_WORK_WAY_14));
    ui->lineEdit_searcgDS->setToolTip("请输入信息元号");
    ui->lineEdit_searcgDS->setText("2311000100901300");
    QRegExp regExp("[A-Fa-f0-9]{1,16}");
    ui->lineEdit_searcgDS->setValidator(new QRegExpValidator(regExp, this));
    g_showDatagram.m_id = 0x2311000100901300;
}

void CDevCtlInterfaceGUIDlg::SetObjInfo(TLxTsspObjectInfo ObjectInfo)
{
    memcpy(&m_ObjectInfo, &ObjectInfo, sizeof(TLxTsspObjectInfo));
    //对象初始化
    //查找对象管理器
    LxTsspObjectManager* pObjectManager = LxTsspObjectManager::Instance();
    if(pObjectManager == NULL)
        return;
    m_pHAManager = pObjectManager->GetHAManager();
    //日志
    //m_pILxTsspLogManager = pObjectManager->GetLogManager();
    m_pStateMgr = pObjectManager->GetStateManager();
    m_pILxTsspMessageManager = pObjectManager->GetMessageManager();
    m_pPublicInfo = pObjectManager->GetPublicInfoManager();
}

//保存参数宏
int CDevCtlInterfaceGUIDlg::SaveToMacro(ILxTsspParameterMacro* )
{
    //判断合法性
    return -1;
    //日志信息
    TLxTsspLog tmpLog;
    QString strLog;
    memset(&tmpLog, 0, sizeof(TLxTsspLog));
    tmpLog.ucWarningLevel  = 1;
    strLog += "调制参数设置: 保存宏成功！";
    strcpy(tmpLog.szContent, strLog.toUtf8().data());
    tmpLog.time = QDateTime::currentDateTime().toTime_t();
    //m_pILxTsspLogManager->Add(tmpLog);
}

void CDevCtlInterfaceGUIDlg::SendMessage(char* pData, unsigned short  usLen, unsigned short usCMD, unsigned short usTID)
{
    static uint seq = 0;
    TLxTsspMessageHeader header;
    memset(&header, 0, sizeof(header));
    TLxTsspSubMsgHeader subHeader;
    memset(&subHeader, 0, sizeof(subHeader));
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
    header.C = usCMD;

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

    m_pILxTsspMessageManager->FireMessage(message);
    delete[] message.pData;
}

DWORDLONG CDevCtlInterfaceGUIDlg::StrToSynCode(const char * pStr)
{
    char c,C;
    const char*  p;
    DWORDLONG dwlValue=0;
    char   DigVal=0;
    USHORT nDigCount=0,MAX_DIGITS=16; //The synchronizing code should be not longer  than 16 digitals

    p=pStr;     //Point to the first char of the string
    c = *p++;   // read the first char
    while ((c!=0) && (nDigCount<=MAX_DIGITS))//  Not the terminator of a string && total digitals is less than 16
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

bool CDevCtlInterfaceGUIDlg::exchange(const QString& buf, int& v)
{
    if(4 != buf.size())return false;
    char tmp[4];
    int pos = 3;
    for(int i = 0; i < 4; ++i)
    {
        tmp[pos--] = buf[i].toLatin1() - 48;
    }
    tmp[0] = tmp[0] |( tmp[1]<<4);
    tmp[1] = tmp[2];
    tmp[2] = tmp[3];
    tmp[1] = tmp[1] |( tmp[2]<<4);
    tmp[2] = tmp[3] = 0;
    memcpy(&v, tmp, 4);
    return true;
}

QString CDevCtlInterfaceGUIDlg::getStrFromDatagram(QByteArray &datagram)
{
    QByteArray tmp = datagram.toHex();
    QString buff;
    for(int i = 0; i < tmp.size(); ++i)
    {
        buff += tmp[i++];
        buff += tmp[i];
        buff += " ";
    }
    return buff;
}
void CDevCtlInterfaceGUIDlg::on_pushButton_clicked()
{
    char buf[10] = {0};
    QString tmp = ui->lineEdit_tID->text();
    int tst = 0;
    exchange(tmp, tst);
    memcpy(buf, &tst, 4);
    tmp = ui->lineEdit_tNum->text();
    for(int i = 0; i < 6; ++i)
    {
        buf[4+i] = tmp[i].toLatin1();
    }
    //SendMessage(buf, 10,  0x200, 0x326);
}

void CDevCtlInterfaceGUIDlg::on_pushButton_traceChannelSel_clicked()
{

}

void CDevCtlInterfaceGUIDlg::on_pushButton_realErrCodeFrmReport_clicked()
{
    char buf[4] = {0};
    int i = 0;
    buf[i++] = ui->comboBox_type->currentIndex() + 1;
    buf[i++] = ui->comboBox_rotation->currentIndex() + 1;
    buf[i++] = ui->comboBox_statisticCtl->currentIndex() + 1;
    buf[i++] = 1;
    //SendMessage(buf, 4, 0x200, 0x320);
}

void CDevCtlInterfaceGUIDlg::on_pushButton_doppTestreport_clicked()
{
    char tmp;
    tmp = ui->comboBox_doppTestReport->currentIndex() + 1;
    //SendMessage(&tmp, 1, 0x200, 0x319);
}

void CDevCtlInterfaceGUIDlg::on_pushButton_show_clicked()
{
    if(!m_pTimer)
        return;
    m_pTimer->start(5000);
}

void CDevCtlInterfaceGUIDlg::on_timer_show_response()
{
    static bool flag = true;
    if(flag)
    {
        ui->textEditTask->setText(getTaskQStr());
        ui->textEditDataTransFreqRgeTst->setText(getDataTransFreqRgeTstQStr());
        ui->textEditRealErrCodFrmStatistics->setText(getRealErrCodFrmStatisticsQStr());
        flag = false;
    }
    else
    {
        clear();
        flag = true;
    }
}

void CDevCtlInterfaceGUIDlg::on_pushButton_clear_clicked()
{
    m_pTimer->stop();
    clear();
}

void CDevCtlInterfaceGUIDlg::clear()
{
    ui->textEditTask->clear();
    ui->textEditDataTransFreqRgeTst->clear();
    ui->textEditRealErrCodFrmStatistics->clear();
}

QString CDevCtlInterfaceGUIDlg::getTaskQStr()
{
    TCISSUE& tmpIssue = g_pPCReport->tcIssue1;
    const int sizeOfId = 4;
    char id[sizeOfId+ 1] = {0};
    memcpy(id, (char*)&tmpIssue.id, sizeOfId);
    for(int i = 0; i < sizeOfId; ++i)
    {
        if(0 != id[i])
            id[i] += 48;
    }
    QString tmp = id;
    tmp += " ";
    const int sizeOfDes = 6;
    char des[sizeOfDes + 1] = {0};
    memcpy(des, tmpIssue.taskDescribe, sizeOfDes);
    tmp += des;
    return tmp;
}

QString CDevCtlInterfaceGUIDlg::getDataTransFreqRgeTstQStr()
{
        DFDRTST& dfdrTst = g_pPCReport->dfdrTst37;
        QMap<int, QString> mapInfo;
        mapInfo[1] = tr(PCR_START);
        mapInfo[2] = tr(PCR_FINISH);
        mapInfo[3] = tr(PCR_STOP);
        QMap<int, QString>::iterator iter = mapInfo.find(dfdrTst.tstStatus);
        if(iter == mapInfo.end())
            return "";
        QString tmp = iter.value();
        tmp += ", ";
        tmp += QString::number(dfdrTst.range);
        tmp += " Hz";
        return tmp;
}

QString CDevCtlInterfaceGUIDlg::getRealErrCodFrmStatisticsQStr()
{
       QMap<int, QString> mapType, mapRotation, mapStatisticsStatus;
       mapType[1] = tr(PCR_TYPE_1);
       mapType[2] = tr(PCR_TYPE_2);
       mapType[3] = tr(PCR_TYPE_3);
       mapRotation[1] = tr(PCR_ROTATION_1);
       mapRotation[2] = tr(PCR_ROTATION_2);
       mapStatisticsStatus[1] = tr(PCR_STATUS_1);
       mapStatisticsStatus[2] = tr(PCR_STATUS_2);
       mapStatisticsStatus[3] = tr(PCR_STATUS_3);
       RTEFSTAT& rtefStat38 = g_pPCReport->rtefStat38;
       QString tmp, comma = ", ";
       QMap<int, QString>::iterator iter = mapType.find(rtefStat38.type);
       if(iter == mapType.end())
           return "";
       tmp += iter.value();
       tmp += comma;
       iter = mapRotation.find(rtefStat38.rotation);
       if(iter == mapRotation.end())
           return "";
       tmp += iter.value();
       tmp += comma;
       iter = mapStatisticsStatus.find(rtefStat38.sstat);
       if(iter == mapStatisticsStatus.end())
           return "";
       tmp += iter.value();
       tmp += comma;
       tmp = tmp + tr(PCR_TOTAL_COD) + QString::number(rtefStat38.totalCod);
       tmp += comma;
       tmp = tmp + tr(PCR_ERR_COD) + QString::number(rtefStat38.errCod);
       tmp += comma;
       tmp = tmp + tr(PCR_TOTAL_FRM) + QString::number(rtefStat38.totalFrm);
       tmp += comma;
       tmp = tmp + tr(PCR_ERR_FRM) + QString::number(rtefStat38.errFrm) + tr(".");
       return tmp;
}

void CDevCtlInterfaceGUIDlg::on_pushButton_wordModeSet_clicked()
{
    int index = ui->comboBox_workModeSet->currentIndex();
    QMap<quint64, QString>::iterator iter = m_mapi64Str.begin();
    iter += index;
    quint64 tmp = iter.key();
    SendMessage((char*)&tmp, 8, 0x200, 0x324);
}

void CDevCtlInterfaceGUIDlg::on_pushButtonWordWay_clicked()
{
    char ch = (char)ui->comboBoxWorkWay->currentIndex()+1;
    SendMessage(&ch, 1, 0x200, 0x325);
}

void CDevCtlInterfaceGUIDlg::on_pushButton_pauseStatu_clicked()
{
//    if(ui->pushButton_pauseStatu->text() == "暂停状态查询")
//    {
//        ui->pushButton_pauseStatu->setText("开始状态查询");
//        if(m_SearchStatusThread)
//            m_SearchStatusThread->pause();
//    }
//    else
//    {
//        ui->pushButton_pauseStatu->setText("暂停状态查询");
//        if(m_SearchStatusThread)
//            m_SearchStatusThread->continu();
//    }
}

void CDevCtlInterfaceGUIDlg::on_pushButton_statuCmd_clicked()
{
    bool ok = false;
    if(m_SearchStatusThread)
        m_SearchStatusThread->continu();
    g_showDatagram.m_id = ui->lineEdit_searcgDS->text().toLongLong(&ok, 16);
    killTimer(m_timerStatu);
    m_timerStatu = startTimer(1000);
    m_showStatuDatagram = true;
}

void CDevCtlInterfaceGUIDlg::on_pushButton_commCmd_clicked()
{
    m_showCommDatagram = true;
    killTimer(m_timerComm);
    m_timerComm = startTimer(1000);

}

void CDevCtlInterfaceGUIDlg::on_pushButton_commClear_clicked()
{
    m_showCommDatagram = false;
    killTimer(m_timerComm);
    ui->textEdit_commResponse->clear();
    //ui->textEdit_commCmd->clear();
}

void CDevCtlInterfaceGUIDlg::on_pushButton_statuClear_clicked()
{
    m_showStatuDatagram = false;
    killTimer(m_timerStatu);
    ui->textEdit_statuResponse->clear();
    //ui->textEdit_statuCmd->clear();
}

void CDevCtlInterfaceGUIDlg::on_pushButton_flash_clicked()
{
    if(ui->pushButton_flash->text() == "停止刷新")
    {
        m_showCommDatagram = false;
        m_showStatuDatagram = false;
        ui->pushButton_flash->setText("开始刷新");
    }
    else
    {
        m_showCommDatagram = true;
        m_showStatuDatagram = true;
        ui->pushButton_flash->setText("停止刷新");
    }
}
void CDevCtlInterfaceGUIDlg::on_radioButton_local_clicked()
{
    static char szVal = 0;
    SendMessage(&szVal,1,7,0x0386);
}

void CDevCtlInterfaceGUIDlg::on_radioButton_remote_clicked()
{
    static char szVal = 1;
    SendMessage(&szVal,1,7,0x0386);
}
