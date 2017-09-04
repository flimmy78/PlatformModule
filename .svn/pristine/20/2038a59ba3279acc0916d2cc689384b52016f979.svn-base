#include "USBBBE_CodeTMParaSetGUIDlg.h"
#include "ui_USBBBE_CodeTMParaSetGUIDlg.h"
#include <QCloseEvent>
#include <QMessageBox>
#include <QDateTime>

extern quint64 GetID(ushort usStationID, uchar ucSubSysID, ushort usDeviceID, ushort usType,
                     uchar ucSn);
USBBBE_CodeTMParaSetGUIDlg::USBBBE_CodeTMParaSetGUIDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::USBBBE_CodeTMParaSetGUIDlg)
{
    ui->setupUi(this);
    m_pStateMgr = NULL;
    m_pILxTsspMessageManager = NULL;
    m_pILxTsspLogManager = NULL;
    m_pPublicInfo = NULL;
    m_pHAManager = NULL;
    m_MCSUSBTMStatus = NULL;
    setAttribute(Qt::WA_DeleteOnClose);
}

USBBBE_CodeTMParaSetGUIDlg::~USBBBE_CodeTMParaSetGUIDlg()
{
    killTimer(m_TimerID);
    delete ui;
}

void USBBBE_CodeTMParaSetGUIDlg::setText(const QString& strTitle, ParaStruct xmlsetText[3][60], QString MapMean[50][30], int MapNum[50], uchar Type, ILxTsspParameterMacro* pMacroObj)
{
    m_strTitle = strTitle;
    setWindowTitle(strTitle);

    memcpy(m_DlgParaSetStruct, xmlsetText, sizeof(ParaStruct) * 50 * 3);

    for(int i = 0; i < 50; i++)
    {
        for(int j = 0; j < 30; j++)
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
void USBBBE_CodeTMParaSetGUIDlg::InitDLG(int Type, ILxTsspParameterMacro* pMacroObj)
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
            memcpy(&m_MCSUSBTMParam, byteArray.data(), sizeof(MCSUSBTMParamStruct));
        }
        if(m_pStateMgr != NULL)
        {
            DWORD stLen = sizeof(MCSUSBTMStatusStruct);
            m_MCSUSBTMStatus = (MCSUSBTMStatusStruct*)m_pStateMgr->FindOneItem(m_StatusComSynID, stLen);
            if(m_MCSUSBTMStatus == NULL || stLen != sizeof(MCSUSBTMStatusStruct))
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
        if(pMacroObj != NULL)
        {
            QByteArray byteArray;

            pMacroObj->GetParameterBlock(m_ParaComSynID, byteArray);
            memcpy(&m_MCSUSBTMParam, byteArray.data(), sizeof(MCSUSBTMParamStruct));
        }
        ui->pushButton_Modify->setVisible(false);
        ui->pushButton_Set->setVisible(false);

        //显示参数
        ShowPara(1);
        ui->lineEdit_WordLen->setEnabled(false);
    }
}

//设置输入格式
void USBBBE_CodeTMParaSetGUIDlg::InitInput()
{
    //正整数
    QRegExp regExp("[0-9]{1,16}");
    //浮点数
    QRegExp regExp0("[0-9]{1,3}(\\.[0-9]{1,3})?");
    //副载波频率
    ui->lineEdit_SubWaveFr->setValidator(new QRegExpValidator(regExp0, this));
    //交织深度
    QRegExp regExpRSDecodeDeep("[1-5]");
    ui->lineEdit_RSDecodeDeep->setValidator(new QRegExpValidator(regExpRSDecodeDeep, this));
    //码速率
    QRegExp regExpCodeRate("[0-9]{1,5}");
    ui->lineEdit_CodeRate->setValidator(new QRegExpValidator(regExpCodeRate, this));
    //字长
    ui->lineEdit_WordLen->setValidator(new QRegExpValidator(regExp, this));
    //帧长
    QRegExp regExpFrameLen("[0-9]{1,4}");
    ui->lineEdit_FrameLen->setValidator(new QRegExpValidator(regExpFrameLen, this));
    //ID字位置
    ui->lineEdit_idCountPos->setValidator(new QRegExpValidator(regExpFrameLen, this));
    QRegExp regExpOne("[0-9]");
    //帧同步容错搜索容错位数
    ui->lineEdit_FrameSyncFaultTolerant->setValidator(new QRegExpValidator(regExpOne, this));
    //帧同步容错校核容错位数
    ui->lineEdit_FaltToleranceOfFram->setValidator(new QRegExpValidator(regExpOne, this));
    //帧同步锁定容错位数
    ui->lineEdit_FrameSyncFaultTolerantLockBit->setValidator(new QRegExpValidator(regExpOne, this));
    //帧同步容错校核检测帧数
    ui->lineEdit_CheckFrameOfFaultFrameSync->setValidator(new QRegExpValidator(regExpOne, this));
    //帧同步容错锁定帧数
    ui->lineEdit_FrameSyncFaultLockDetection->setValidator(new QRegExpValidator(regExpOne, this));
    //副帧同步容错校核容错位数
    ui->lineEdit_SubFrameSyncFaultTolerant->setValidator(new QRegExpValidator(regExpOne, this));
    //副帧同步容错校核容错位数
    ui->lineEdit_SubFaltToleranceOfFram->setValidator(new QRegExpValidator(regExpOne, this));
    //副帧同步容错锁定容错位数
    ui->lineEdit_SubFrameSyncFaultTolerantLockBit->setValidator(new QRegExpValidator(regExpOne, this));
    //副帧校核帧数
    ui->lineEdit_SubCheckFrameOfFaultFrameSync->setValidator(new QRegExpValidator(regExpOne, this));
    //副帧锁定帧数
    ui->lineEdit_SubFrameSyncFaultLockDetection->setValidator(new QRegExpValidator(regExpOne, this));
    //副帧码组位置
    ui->lineEdit_SubFrmSynGroupPos->setValidator(new QRegExpValidator(regExpFrameLen, this));
    //格式长
    QRegExp regExpFormatLen("[0-9]{1,3}");
    ui->lineEdit_FormatLen->setValidator(new QRegExpValidator(regExpFormatLen, this));

    //字母数字
    QRegExp regExp2("[A-Fa-f0-9]{1,8}");
    QRegExp regExp3("[A-Fa-f0-9]{1,2}");
    //遥测信息标识码
    ui->lineEdit_TelInfoFlag->setValidator(new QRegExpValidator(regExp3, this));
    //帧同步码组
    ui->lineEdit_FrmSyncGroup->setValidator(new QRegExpValidator(regExp2, this));
    //解扰多项式
    ui->lineEdit_RandPoly->setValidator(new QRegExpValidator(regExp2, this));
    //副帧同步码组
    ui->lineEdit_SubFrmGroup->setValidator(new QRegExpValidator(regExp2, this));
    //解扰寄存器初相
    ui->lineEdit_RandPhase->setValidator(new QRegExpValidator(regExp2, this));
}

void USBBBE_CodeTMParaSetGUIDlg::ShowControls()
{
    //向下拉框控件中写入从xml中读取的内容
    int i;
    //帧同步码位置
    for(i = 0; i<DlgMapLineNum[0];i++)
    {
        ui->comboBox_FrameSyncCodePos->addItem(m_DlgMapMean[0][i]);
    }
    ui->comboBox_FrameSyncCodePos->setCurrentIndex(m_DlgParaSetStruct[0][0].InitVal.lval);
    //遥测信息标识码
    UCHAR ucVal = m_DlgParaSetStruct[0][1].InitVal.lval;
    ui->lineEdit_TelInfoFlag->setText(QString::number(ucVal));
    //副载波频率
    ui->lineEdit_SubWaveFr->setText(QString::number(m_DlgParaSetStruct[0][2].InitVal.lval));
    //译码方式
    for( i= 0; i<DlgMapLineNum[1]; i++)
    {
        ui->comboBox_DecodeType->addItem(m_DlgMapMean[1][i]);
    }
    ui->comboBox_DecodeType->setCurrentIndex(m_DlgParaSetStruct[0][3].InitVal.lval);
    //交织深度
    ui->lineEdit_RSDecodeDeep->setText(QString::number(m_DlgParaSetStruct[0][4].InitVal.lval));
    //码型
    for(i = 0; i<DlgMapLineNum[2]; i++)
    {
        ui->comboBox_CodeType->addItem(m_DlgMapMean[2][i]);
    }
    ui->comboBox_CodeType->setCurrentIndex(m_DlgParaSetStruct[0][5].InitVal.lval);
    //卷积码类型
    for(i = 0; i<DlgMapLineNum[3];i++)
    {
        ui->comboBox_RecurrentCodeType->addItem(m_DlgMapMean[3][i]);
    }
    ui->comboBox_RecurrentCodeType->setCurrentIndex(m_DlgParaSetStruct[0][6].InitVal.lval);
    //Viterbi译码输出相位
    for(i = 0; i<DlgMapLineNum[4];i++)
    {
        ui->comboBox_ViterbiPhase->addItem(m_DlgMapMean[4][i]);
    }
    ui->comboBox_ViterbiPhase->setCurrentIndex(m_DlgParaSetStruct[0][7].InitVal.lval);
    //码速率
    ui->lineEdit_CodeRate->setText(QString::number(m_DlgParaSetStruct[0][8].InitVal.lval));
    //R-S码类型
    for(i = 0; i<DlgMapLineNum[5]; i++)
    {
        ui->comboBox_RSType->addItem(m_DlgMapMean[5][i]);
    }
    ui->comboBox_RSType->setCurrentIndex(m_DlgParaSetStruct[0][9].InitVal.lval);
    //RS译码坐标
    for(i = 0; i<DlgMapLineNum[6]; i++)
    {
        ui->comboBox_RSDecodePlot->addItem(m_DlgMapMean[6][i]);
    }
    ui->comboBox_RSDecodePlot->setCurrentIndex(m_DlgParaSetStruct[0][10].InitVal.lval);
    //字长
    ui->lineEdit_WordLen->setText(QString::number(m_DlgParaSetStruct[0][11].InitVal.lval));
    //帧同步码组长
    for(i = 0; i<DlgMapLineNum[7]; i++)
    {
        ui->comboBox_FrameSyncGroupLen->addItem(m_DlgMapMean[7][i]);
    }
    ui->comboBox_FrameSyncGroupLen->setCurrentIndex(m_DlgParaSetStruct[0][12].InitVal.lval);
    //解扰多项式长度
    for(i = 0; i<DlgMapLineNum[8]; i++)
    {
        ui->comboBox_RandPolyLen->addItem(m_DlgMapMean[8][i]);
    }
    ui->comboBox_RandPolyLen->setCurrentIndex(m_DlgParaSetStruct[0][13].InitVal.lval);
    //帧长
    ui->lineEdit_FrameLen->setText(QString::number(m_DlgParaSetStruct[0][14].InitVal.lval));
    //帧同步码组
    ui->lineEdit_FrmSyncGroup->setText(QString::number(m_DlgParaSetStruct[0][15].InitVal.lval,16).toUpper());
    //解扰多项式
    ui->lineEdit_RandPoly->setText(QString::number(m_DlgParaSetStruct[0][16].InitVal.lval,16).toUpper());
    //帧格式
    ui->lineEdit_FormatLen->setText(QString::number(m_DlgParaSetStruct[0][17].InitVal.lval,16).toUpper());
    //解扰控制
    for(i = 0; i<DlgMapLineNum[9]; i++)
    {
        ui->comboBox_RandEn->addItem(m_DlgMapMean[9][i]);
    }
    ui->comboBox_RandEn->setCurrentIndex(m_DlgParaSetStruct[0][18].InitVal.lval);
    //解扰寄存器初相
    ui->lineEdit_RandPhase->setText(QString::number(m_DlgParaSetStruct[0][19].InitVal.lval,16).toUpper());
    //副帧同步方式
    for(i = 0; i<DlgMapLineNum[10]; i++)
    {
        ui->comboBox_SubFrmSyncType->addItem(m_DlgMapMean[10][i]);
    }
    ui->comboBox_SubFrmSyncType->setCurrentIndex(m_DlgParaSetStruct[0][20].InitVal.lval);
    //副帧码组长度
    for(i = 0; i<DlgMapLineNum[11];i++)
    {
        ui->comboBox_SubFrameGroupLen->addItem(m_DlgMapMean[11][i]);
    }
    ui->comboBox_SubFrameGroupLen->setCurrentIndex(m_DlgParaSetStruct[0][21].InitVal.lval);
    //副帧码组位置
    ui->lineEdit_SubFrmSynGroupPos->setText(QString::number(m_DlgParaSetStruct[0][22].InitVal.lval));
    //副帧同步码组
    ui->lineEdit_SubFrmGroup->setText(QString::number(m_DlgParaSetStruct[0][23].InitVal.lval,16).toUpper());
    //ID字位置
    ui->lineEdit_idCountPos->setText(QString::number(m_DlgParaSetStruct[0][24].InitVal.lval));
    //ID字基值
    for(i = 0; i<DlgMapLineNum[12]; i++)
    {
        ui->comboBox_IDBaseValue->addItem(m_DlgMapMean[12][i]);
    }
    ui->comboBox_IDBaseValue->setCurrentIndex(m_DlgParaSetStruct[0][25].InitVal.lval);
    //ID计数方向
    for(i = 0; i<DlgMapLineNum[13]; i++)
    {
        ui->comboBox_idDirect->addItem(m_DlgMapMean[13][i]);
    }
    ui->comboBox_idDirect->setCurrentIndex(m_DlgParaSetStruct[0][26].InitVal.lval);
    //帧同步容错搜索容错位数
    ui->lineEdit_FrameSyncFaultTolerant->setText(QString::number(m_DlgParaSetStruct[1][0].InitVal.lval));
    //帧同步容错校核容错位数
    ui->lineEdit_FaltToleranceOfFram->setText(QString::number(m_DlgParaSetStruct[1][1].InitVal.lval));
    //帧同步锁定容错位数
    ui->lineEdit_FrameSyncFaultTolerantLockBit->setText(QString::number(m_DlgParaSetStruct[1][2].InitVal.lval));
    //帧同步容错校核检测帧数
    ui->lineEdit_CheckFrameOfFaultFrameSync->setText(QString::number(m_DlgParaSetStruct[1][3].InitVal.lval));
    //帧同步容错锁定帧数
    ui->lineEdit_FrameSyncFaultLockDetection->setText(QString::number(m_DlgParaSetStruct[1][4].InitVal.lval));
    //副帧同步容错校核容错位数
    ui->lineEdit_SubFrameSyncFaultTolerant->setText(QString::number(m_DlgParaSetStruct[1][5].InitVal.lval));
    //副帧同步容错校核容错位数
    ui->lineEdit_SubFaltToleranceOfFram->setText(QString::number(m_DlgParaSetStruct[1][6].InitVal.lval));
    //副帧同步容错锁定容错位数
    ui->lineEdit_SubFrameSyncFaultTolerantLockBit->setText(QString::number(m_DlgParaSetStruct[1][7].InitVal.lval));
    //副帧校核帧数
    ui->lineEdit_SubCheckFrameOfFaultFrameSync->setText(QString::number(m_DlgParaSetStruct[1][8].InitVal.lval));
    //副帧锁定帧数
    ui->lineEdit_SubFrameSyncFaultLockDetection->setText(QString::number(m_DlgParaSetStruct[1][9].InitVal.lval));

    //设置label参数显示信息
    QString strTemp;
    //副载波频率
    strTemp = "副载波频率\n[" + QString::number(m_DlgParaSetStruct[0][2].MinVal.lmin / 1000) + "~" +
            QString::number(m_DlgParaSetStruct[0][2].MaxVal.lmax / 1000) + "]kHz";//kHz";
    ui->label_SubWaveFr->setText(strTemp);
    //交织深度
    strTemp = "交织深度[" + QString::number(m_DlgParaSetStruct[0][4].MinVal.lmin) + "~" +
            QString::number(m_DlgParaSetStruct[0][4].MaxVal.lmax)  + "]";
    ui->label_RSDecodeDeep->setText(strTemp);
//    //字长
//    strTemp = "字长[" + QString::number(m_DlgParaSetStruct[0][11].MinVal.lmin) + "~" +
//            QString::number(m_DlgParaSetStruct[0][11].MaxVal.lmax) + "]";//位";
//    ui->label_WordLen->setText(strTemp);
//    //帧长
//    strTemp = "帧长\n[" + QString::number(m_DlgParaSetStruct[0][14].MinVal.lmin) + "~" +
//            QString::number(m_DlgParaSetStruct[0][14].MaxVal.lmax) + "]";//字";
//    ui->label_FrameLen->setText(strTemp);
    //ID字位置
    strTemp = "ID字位置[" + QString::number(m_DlgParaSetStruct[0][24].MinVal.lmin) + "~" +
            QString::number(m_DlgParaSetStruct[0][24].MaxVal.lmax) + "]";//位";
    ui->label_idCountPos->setText(strTemp);
    //帧同步容错搜索容错位数
    strTemp = "帧同步容错搜索容错位数\n[" + QString::number(m_DlgParaSetStruct[1][0].MinVal.lmin) + "~" +
            QString::number(m_DlgParaSetStruct[1][0].MaxVal.lmax) + "]";//位";
    ui->label_FrameSyncFaultTolerant->setText(strTemp);
    //帧同步容错校核容错位数
    strTemp = "帧同步容错校核容错位数\n[" + QString::number(m_DlgParaSetStruct[1][1].MinVal.lmin) + "~" +
            QString::number(m_DlgParaSetStruct[1][1].MaxVal.lmax) + "]";//位";
    ui->label_FaltToleranceOfFram->setText(strTemp);
    //帧同步锁定容错位数
    strTemp = "帧同步容错锁定容错位数\n[" + QString::number(m_DlgParaSetStruct[1][2].MinVal.lmin) + "~" +
            QString::number(m_DlgParaSetStruct[1][2].MaxVal.lmax) + "]";//位";
    ui->label_FrameSyncFaultTolerantLockBit->setText(strTemp);
    //帧同步容错校核检测帧数
    strTemp = "帧同步容错校核检测帧数\n[" + QString::number(m_DlgParaSetStruct[1][3].MinVal.lmin) + "~" +
            QString::number(m_DlgParaSetStruct[1][3].MaxVal.lmax) + "]";//帧";
    ui->label_CheckFrameOfFaultFrameSync->setText(strTemp);
    //帧同步容错锁定帧数
    strTemp = "帧同步容错锁定检测帧数\n[" + QString::number(m_DlgParaSetStruct[1][4].MinVal.lmin) + "~" +
            QString::number(m_DlgParaSetStruct[1][4].MaxVal.lmax) + "]";//帧";
    ui->label_FrameSyncFaultLockDetection->setText(strTemp);
    //副帧同步容错校核容错位数
    strTemp = "副帧同步容错校核容错位数\n[" + QString::number(m_DlgParaSetStruct[1][5].MinVal.lmin) + "~" +
            QString::number(m_DlgParaSetStruct[1][5].MaxVal.lmax) + "]";//位";
    ui->label_SubFrameSyncFaultTolerant->setText(strTemp);
    //副帧同步容错校核容错位数
    strTemp = "副帧同步容错校核容错位数\n[" + QString::number(m_DlgParaSetStruct[1][6].MinVal.lmin) + "~" +
            QString::number(m_DlgParaSetStruct[1][6].MaxVal.lmax) + "]";//位";
    ui->label_SubFaltToleranceOfFram->setText(strTemp);
    //副帧同步容错锁定容错位数
    strTemp = "副帧同步容错锁定容错位数\n[" + QString::number(m_DlgParaSetStruct[1][7].MinVal.lmin) + "~" +
            QString::number(m_DlgParaSetStruct[1][7].MaxVal.lmax) + "]";//位";
    ui->label_SubFrameSyncFaultTolerantLockBit->setText(strTemp);
    //副帧校核帧数
    strTemp = "副帧同步容错校核检测帧数\n[" + QString::number(m_DlgParaSetStruct[1][8].MinVal.lmin) + "~" +
            QString::number(m_DlgParaSetStruct[1][8].MaxVal.lmax) + "]";//帧";
    ui->label_SubCheckFrameOfFaultFrameSync->setText(strTemp);
    //副帧锁定帧数
    strTemp = "副帧同步容错锁定检测帧数\n[" + QString::number(m_DlgParaSetStruct[1][9].MinVal.lmin) + "~" +
            QString::number(m_DlgParaSetStruct[1][9].MaxVal.lmax) + "]";//帧";
    ui->label_SubFrameSyncFaultLockDetection->setText(strTemp);
}

//显示参数
void USBBBE_CodeTMParaSetGUIDlg::ShowPara(int type)
{
    if(type == 0)       //参数设置
    {
        //码同步
        if(m_MCSUSBTMStatus != NULL)
        {
            m_MCSUSBTMParam = m_MCSUSBTMStatus->tParams;
        }
        else
        {
            DWORD stLen = sizeof(MCSUSBTMStatusStruct);
            if(m_pStateMgr != NULL)
                m_MCSUSBTMStatus  = (MCSUSBTMStatusStruct*)m_pStateMgr->FindOneItem(m_StatusComSynID, stLen);
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

    //帧同步码位置
    if (m_MCSUSBTMParam.ucFspPlace > 0)
        ui->comboBox_FrameSyncCodePos->setCurrentIndex(m_MCSUSBTMParam.ucFspPlace - 1);
    //遥测信息标识码
    ui->lineEdit_TelInfoFlag->setText(QString("%1").
                                      arg(m_MCSUSBTMParam.ucDataCode, 2 ,
                                          16, QChar::fromLatin1('0')).toUpper());
    //副载波频率
    ui->lineEdit_SubWaveFr->setText(QString::number(m_MCSUSBTMParam.uiSWFreq / 1000.0));
    //译码方式
    if (m_MCSUSBTMParam.ucCodingMd > 0)
        ui->comboBox_DecodeType->setCurrentIndex(m_MCSUSBTMParam.ucCodingMd - 1);
    //交织深度
    if (m_MCSUSBTMParam.ucInterleav > 0 && m_MCSUSBTMParam.ucInterleav < 6)
        ui->lineEdit_RSDecodeDeep->setText(QString::number(m_MCSUSBTMParam.ucInterleav));
    else
    {
        ui->lineEdit_RSDecodeDeep->setText("1");
    }
    //码型
    if (m_MCSUSBTMParam.uiCodeStyle > 0)
        ui->comboBox_CodeType->setCurrentIndex(m_MCSUSBTMParam.uiCodeStyle - 1);
    //卷积码类型
    if (m_MCSUSBTMParam.ucViterbiMd > 0)
        ui->comboBox_RecurrentCodeType->setCurrentIndex(m_MCSUSBTMParam.ucViterbiMd - 1);
    //Viterbi译码输出相位
    if (m_MCSUSBTMParam.ucG2Inv > 0)
        ui->comboBox_ViterbiPhase->setCurrentIndex(m_MCSUSBTMParam.ucG2Inv - 1);
    //码速率
    ui->lineEdit_CodeRate->setText(QString::number(m_MCSUSBTMParam.uiTM_CodeRate));
    //R-S码类型
    if (m_MCSUSBTMParam.ucRSMd > 0)
        ui->comboBox_RSType->setCurrentIndex(m_MCSUSBTMParam.ucRSMd - 1);
    //RS译码坐标
    if (m_MCSUSBTMParam.ucBasiSel > 0)
        ui->comboBox_RSDecodePlot->setCurrentIndex(m_MCSUSBTMParam.ucBasiSel - 1);
    //字长
    ui->lineEdit_WordLen->setText(QString::number(m_MCSUSBTMParam.ucWordLen));
    //帧同步码组长
    int iFrameSynGLen = 8;
    if (m_MCSUSBTMParam.ucFrameSynGLen > 0)
    {
        ui->comboBox_FrameSyncGroupLen->setCurrentIndex(m_MCSUSBTMParam.ucFrameSynGLen - 1);
        iFrameSynGLen = (m_MCSUSBTMParam.ucFrameSynGLen + 1) * 2;
    }
    //解扰多项式长度
    if (m_MCSUSBTMParam.ucRandLen > 0)
        ui->comboBox_RandPolyLen->setCurrentIndex(m_MCSUSBTMParam.ucRandLen - 1);
    //帧长
    ui->lineEdit_FrameLen->setText(QString::number(m_MCSUSBTMParam.usFrameLen));
    //帧同步码组
    ui->lineEdit_FrmSyncGroup->setText(QString("%1").
                                       arg(m_MCSUSBTMParam.uiFrameSynG, iFrameSynGLen ,
                                           16, QChar::fromLatin1('0')).toUpper());
    //解扰多项式
    ui->lineEdit_RandPoly->setText(QString("%1").
                                   arg(m_MCSUSBTMParam.uiRandPoly, m_MCSUSBTMParam.ucRandLen * 2 ,
                                       16, QChar::fromLatin1('0')).toUpper());
    //格式长
    ui->lineEdit_FormatLen->setText(QString::number(m_MCSUSBTMParam.ucSframeLen));
    //解扰控制
    if (m_MCSUSBTMParam.ucUnturbCtrl > 0)
        ui->comboBox_RandEn->setCurrentIndex(m_MCSUSBTMParam.ucUnturbCtrl - 1);
    //解扰寄存器初相
    ui->lineEdit_RandPhase->setText(QString("%1").
                                    arg(m_MCSUSBTMParam.uiPhase, m_MCSUSBTMParam.ucRandLen * 2 ,
                                        16, QChar::fromLatin1('0')).toUpper());
    //副帧同步方式
    if (m_MCSUSBTMParam.ucSFrameSynMd > 0)
        ui->comboBox_SubFrmSyncType->setCurrentIndex(m_MCSUSBTMParam.ucSFrameSynMd - 1);
    //副帧码组长度
    int iSubFrmSynCodeLen = 8;
    if (m_MCSUSBTMParam.ucCycFrmSynCodeLen > 0)
    {
        ui->comboBox_SubFrameGroupLen->setCurrentIndex(m_MCSUSBTMParam.ucCycFrmSynCodeLen - 1);
        iSubFrmSynCodeLen = m_MCSUSBTMParam.ucCycFrmSynCodeLen * 2;
    }
    //副帧码组位置
    ui->lineEdit_SubFrmSynGroupPos->setText(QString::number(m_MCSUSBTMParam.usCycFrmSynCodePos));
    //副帧同步码组
    ui->lineEdit_SubFrmGroup->setText(QString("%1").
                                      arg(m_MCSUSBTMParam.uiCycFrmSynCode, iSubFrmSynCodeLen,
                                          16, QChar::fromLatin1('0')).toUpper());
    //ID字位置
    ui->lineEdit_idCountPos->setText(QString::number(m_MCSUSBTMParam.usIDSubFrmPos));
    //ID字基值
    if (m_MCSUSBTMParam.ucIDBase > 0)
        ui->comboBox_IDBaseValue->setCurrentIndex(m_MCSUSBTMParam.ucIDBase - 1);
    //ID计数方向
    if (m_MCSUSBTMParam.ucIDDirect > 0)
        ui->comboBox_idDirect->setCurrentIndex(m_MCSUSBTMParam.ucIDDirect - 1);
    //帧同步容错搜索容错位数
    ui->lineEdit_FrameSyncFaultTolerant->setText(QString::number(m_MCSUSBTMParam.ucFrSynSerLiErr));
    //帧同步容错校核容错位数
    ui->lineEdit_FaltToleranceOfFram->setText(QString::number(m_MCSUSBTMParam.ucFrSynVerLiErr));
    //帧同步锁定容错位数
    ui->lineEdit_FrameSyncFaultTolerantLockBit->setText(QString::number(m_MCSUSBTMParam.ucFrSynLoLiErr));
    //帧同步容错校核检测帧数
    ui->lineEdit_CheckFrameOfFaultFrameSync->setText(QString::number(m_MCSUSBTMParam.ucFrSynVerCheck));
    //帧同步容错锁定帧数
    ui->lineEdit_FrameSyncFaultLockDetection->setText(QString::number(m_MCSUSBTMParam.ucFrSynLoCheck));
    //副帧同步容错校核容错位数
    ui->lineEdit_SubFrameSyncFaultTolerant->setText(QString::number(m_MCSUSBTMParam.ucCFSearchErrBits));
    //副帧同步容错校核容错位数
    ui->lineEdit_SubFaltToleranceOfFram->setText(QString::number(m_MCSUSBTMParam.ucCFVerifyErrBits));
    //副帧同步容错锁定容错位数
    ui->lineEdit_SubFrameSyncFaultTolerantLockBit->setText(QString::number(m_MCSUSBTMParam.ucCFLockErrBits));
    //副帧校核帧数
    ui->lineEdit_SubCheckFrameOfFaultFrameSync->setText(QString::number(m_MCSUSBTMParam.ucSubFVerifyFrame));
    //副帧锁定帧数
    ui->lineEdit_SubFrameSyncFaultLockDetection->setText(QString::number(m_MCSUSBTMParam.ucSubFLockFrame));

    on_comboBox_DecodeType_currentIndexChanged(ui->comboBox_DecodeType->currentIndex());
    on_comboBox_RecurrentCodeType_currentIndexChanged(ui->comboBox_RecurrentCodeType->currentIndex());
    on_comboBox_RandEn_currentIndexChanged(ui->comboBox_RandEn->currentIndex());
    on_comboBox_SubFrmSyncType_currentIndexChanged(ui->comboBox_SubFrmSyncType->currentIndex());
}

//设置帧长范围
void USBBBE_CodeTMParaSetGUIDlg::SetFrameLenRange(int iWordLen)
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

void USBBBE_CodeTMParaSetGUIDlg::SetObjInfo(TLxTsspObjectInfo ObjectInfo)
{
    memcpy(&m_ObjectInfo, &ObjectInfo, sizeof(TLxTsspObjectInfo));
    //对象初始化
    //查找对象管理器
    LxTsspObjectManager* pObjectManager = LxTsspObjectManager::Instance();
    if (pObjectManager == NULL)
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
        m_ParaComSynID = GetID(sItem.usStationID, sItem.ucSubSysID, sItem.usDeviceID,
                               sItem.usTID, sItem.ucSN);
        /*************************框架**********************************/

        /*************************状态**********************************/
        m_StatusComSynID = GetID(sItem.usStationID, sItem.ucSubSysID, sItem.usDeviceID,
                                 sItem.usStateType, sItem.ucSN);
        /*************************状态**********************************/
    }
    //日志
    m_pILxTsspLogManager = pObjectManager->GetLogManager();
    m_pStateMgr = pObjectManager->GetStateManager();
    m_pILxTsspMessageManager = pObjectManager->GetMessageManager();
    m_pPublicInfo = pObjectManager->GetPublicInfoManager();
}

//使能或不使能控件
void USBBBE_CodeTMParaSetGUIDlg::EnabledControls(bool bFlag)
{
    /*************************Group1*************************/
    //帧同步码位置
    ui->comboBox_FrameSyncCodePos->setEnabled(bFlag);
    //遥测信息标识码
    ui->lineEdit_TelInfoFlag->setEnabled(bFlag);
    /*************************Group1*************************/

    /*************************Group2*************************/
    //副载波频率
    ui->lineEdit_SubWaveFr->setEnabled(bFlag);
    //译码方式
    ui->comboBox_DecodeType->setEnabled(bFlag);
    //交织深度
    ui->lineEdit_RSDecodeDeep->setEnabled(bFlag);
    //码型
    ui->comboBox_CodeType->setEnabled(bFlag);
    //卷积码类型
    ui->comboBox_RecurrentCodeType->setEnabled(bFlag);
    //Viterbi译码输出相位
    ui->comboBox_ViterbiPhase->setEnabled(bFlag);
    //码速率
    ui->lineEdit_CodeRate->setEnabled(bFlag);
    //R-S码类型
    ui->comboBox_RSType->setEnabled(bFlag);
    //RS译码坐标基
    ui->comboBox_RSDecodePlot->setEnabled(bFlag);
    /*************************Group2*************************/

    /*************************Group3*************************/
    //字长
    ui->lineEdit_WordLen->setEnabled(false);
    //帧同步码组长
    ui->comboBox_FrameSyncGroupLen->setEnabled(bFlag);
    //解扰多项式长度
    ui->comboBox_RandPolyLen->setEnabled(bFlag);
    //帧长
    ui->lineEdit_FrameLen->setEnabled(bFlag);
    //帧同步码组
    ui->lineEdit_FrmSyncGroup->setEnabled(bFlag);
    //解扰多项式
    ui->lineEdit_RandPoly->setEnabled(bFlag);
    //帧格式
    ui->lineEdit_FormatLen->setEnabled(bFlag);
    //解扰控制
    ui->comboBox_RandEn->setEnabled(bFlag);
    //解扰寄存器初相
    ui->lineEdit_RandPhase->setEnabled(bFlag);
    /*************************Group3*************************/

    /*************************Group4*************************/
    //副帧同步方式
    ui->comboBox_SubFrmSyncType->setEnabled(bFlag);
    //副帧码组长度
    ui->comboBox_SubFrameGroupLen->setEnabled(bFlag);
    //副帧码组位置
    ui->lineEdit_SubFrmSynGroupPos->setEnabled(bFlag);
    //副帧同步码组
    ui->lineEdit_SubFrmGroup->setEnabled(bFlag);
    //ID字位置
    ui->lineEdit_idCountPos->setEnabled(bFlag);
    //ID字基值
    ui->comboBox_IDBaseValue->setEnabled(bFlag);
    //ID计数方向
    ui->comboBox_idDirect->setEnabled(bFlag);
    /*************************Group4*************************/

    /*************************Group5*************************/
    //帧同步容错搜索容错位数
    ui->lineEdit_FrameSyncFaultTolerant->setEnabled(bFlag);
    //帧同步容错校核容错位数
    ui->lineEdit_FaltToleranceOfFram->setEnabled(bFlag);
    //帧同步锁定容错位数
    ui->lineEdit_FrameSyncFaultTolerantLockBit->setEnabled(bFlag);
    //帧同步容错校核检测帧数
    ui->lineEdit_CheckFrameOfFaultFrameSync->setEnabled(bFlag);
    //帧同步容错锁定帧数
    ui->lineEdit_FrameSyncFaultLockDetection->setEnabled(bFlag);
    //副帧同步容错校核容错位数
    ui->lineEdit_SubFrameSyncFaultTolerant->setEnabled(bFlag);
    //副帧同步容错校核容错位数
    ui->lineEdit_SubFaltToleranceOfFram->setEnabled(bFlag);
    //副帧同步容错锁定容错位数
    ui->lineEdit_SubFrameSyncFaultTolerantLockBit->setEnabled(bFlag);
    //副帧校核帧数
    ui->lineEdit_SubCheckFrameOfFaultFrameSync->setEnabled(bFlag);
    //副帧锁定帧数
    ui->lineEdit_SubFrameSyncFaultLockDetection->setEnabled(bFlag);
    /*************************Group5*************************/

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
int USBBBE_CodeTMParaSetGUIDlg::SaveToMacro(ILxTsspParameterMacro* pMacroObj)
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

        //副帧同步器参数
        byteArray.resize(sizeof(m_MCSUSBTMParam));
        memcpy(byteArray.data(),&m_MCSUSBTMParam, sizeof(m_MCSUSBTMParam));
        pMacroObj->UpdateParameterBlock(m_ParaComSynID, byteArray);

        //日志信息
        QString strLog;
        strLog += QString("%1: 保存宏成功！").arg(MODULENAME);
        CLog::addLog(strLog);
    }
}
//从界面取参数
void USBBBE_CodeTMParaSetGUIDlg::GetPara()
{
    bool ok;
    //帧同步码位置
    m_MCSUSBTMParam.ucFspPlace = ui->comboBox_FrameSyncCodePos->currentIndex() + 1;
    //遥测信息标识码
    m_MCSUSBTMParam.ucDataCode = ui->lineEdit_TelInfoFlag->text().toInt(&ok, 16);
    //副载波频率
    m_MCSUSBTMParam.uiSWFreq = (UINT)(ui->lineEdit_SubWaveFr->text().toDouble() * 1000);
    //译码方式
    m_MCSUSBTMParam.ucCodingMd = ui->comboBox_DecodeType->currentIndex() + 1;
    //交织深度
    m_MCSUSBTMParam.ucInterleav = ui->lineEdit_RSDecodeDeep->text().toInt();
    //码型
    m_MCSUSBTMParam.uiCodeStyle = ui->comboBox_CodeType->currentIndex() + 1;
    //卷积码类型
    m_MCSUSBTMParam.ucViterbiMd = ui->comboBox_RecurrentCodeType->currentIndex() + 1;
    //Viterbi译码输出相位
    m_MCSUSBTMParam.ucG2Inv = ui->comboBox_ViterbiPhase->currentIndex() + 1;
    //码速率
    m_MCSUSBTMParam.uiTM_CodeRate = ui->lineEdit_CodeRate->text().toInt();
    //R-S码类型
    m_MCSUSBTMParam.ucRSMd = ui->comboBox_RSType->currentIndex() + 1;
    //RS译码坐标
    m_MCSUSBTMParam.ucBasiSel = ui->comboBox_RSDecodePlot->currentIndex() + 1;
    //字长
    m_MCSUSBTMParam.ucWordLen = ui->lineEdit_WordLen->text().toInt();
    //帧同步码组长
    m_MCSUSBTMParam.ucFrameSynGLen = ui->comboBox_FrameSyncGroupLen->currentIndex() + 1;
    //解扰多项式长度
    m_MCSUSBTMParam.ucRandLen = ui->comboBox_RandPolyLen->currentIndex() + 1;
    //帧长
    m_MCSUSBTMParam.usFrameLen = ui->lineEdit_FrameLen->text().toInt();
    //帧同步码组
    QString strTem1 = ui->lineEdit_FrmSyncGroup->text().trimmed();
    char *pTem1 = strTem1.toLocal8Bit().data();
    m_MCSUSBTMParam.uiFrameSynG = StrToSynCode(pTem1);
    //解扰多项式
    m_MCSUSBTMParam.uiRandPoly = ui->lineEdit_RandPoly->text().toUInt(&ok,16);
    //格式长
    m_MCSUSBTMParam.ucSframeLen = ui->lineEdit_FormatLen->text().toInt();
    //解扰控制
    m_MCSUSBTMParam.ucUnturbCtrl = ui->comboBox_RandEn->currentIndex() + 1;
    //解扰寄存器初相
    m_MCSUSBTMParam.uiPhase = ui->lineEdit_RandPhase->text().toUInt(&ok, 16);
    //副帧同步方式
    m_MCSUSBTMParam.ucSFrameSynMd = ui->comboBox_SubFrmSyncType->currentIndex() + 1;
    //副帧码组长度
    m_MCSUSBTMParam.ucCycFrmSynCodeLen = ui->comboBox_SubFrameGroupLen->currentIndex() + 1;
    //副帧码组位置
    m_MCSUSBTMParam.usCycFrmSynCodePos = ui->lineEdit_SubFrmSynGroupPos->text().toInt();
    //副帧同步码组
    strTem1 = ui->lineEdit_SubFrmGroup->text().trimmed();
    pTem1 = strTem1.toLocal8Bit().data();
    m_MCSUSBTMParam.uiCycFrmSynCode = StrToSynCode(pTem1);
    //ID字位置
    m_MCSUSBTMParam.usIDSubFrmPos = ui->lineEdit_idCountPos->text().toInt();
    //ID字基值
    m_MCSUSBTMParam.ucIDBase = ui->comboBox_IDBaseValue->currentIndex() + 1;
    //ID计数方向
    m_MCSUSBTMParam.ucIDDirect = ui->comboBox_idDirect->currentIndex() + 1;
    //帧同步容错搜索容错位数
    m_MCSUSBTMParam.ucFrSynSerLiErr = ui->lineEdit_FrameSyncFaultTolerant->text().toInt();
    //帧同步容错校核容错位数
    m_MCSUSBTMParam.ucFrSynVerLiErr = ui->lineEdit_FaltToleranceOfFram->text().toInt();
    //帧同步锁定容错位数
    m_MCSUSBTMParam.ucFrSynLoLiErr = ui->lineEdit_FrameSyncFaultTolerantLockBit->text().toInt();
    //帧同步容错校核检测帧数
    m_MCSUSBTMParam.ucFrSynVerCheck = ui->lineEdit_CheckFrameOfFaultFrameSync->text().toInt();
    //帧同步容错锁定帧数
    m_MCSUSBTMParam.ucFrSynLoCheck = ui->lineEdit_FrameSyncFaultLockDetection->text().toInt();
    //副帧同步容错校核容错位数
    m_MCSUSBTMParam.ucCFSearchErrBits = ui->lineEdit_SubFrameSyncFaultTolerant->text().toInt();
    //副帧同步容错校核容错位数
    m_MCSUSBTMParam.ucCFVerifyErrBits = ui->lineEdit_SubFaltToleranceOfFram->text().toInt();
    //副帧同步容错锁定容错位数
    m_MCSUSBTMParam.ucCFLockErrBits = ui->lineEdit_SubFrameSyncFaultTolerantLockBit->text().toInt();
    //副帧校核帧数
    m_MCSUSBTMParam.ucSubFVerifyFrame = ui->lineEdit_SubCheckFrameOfFaultFrameSync->text().toInt();
    //副帧锁定帧数
    m_MCSUSBTMParam.ucSubFLockFrame = ui->lineEdit_SubFrameSyncFaultLockDetection->text().toInt();
}

//验证参数范围
int USBBBE_CodeTMParaSetGUIDlg::CheckPara()
{
    QMessageBox msgBox;
    QString strTemp;
    msgBox.setWindowTitle(MODULENAME);
    ///////////////////////////////////////
    QString m_TMInfoFlagCode;
    bool m_brandom;
    QString m_strRandPoly = ui->lineEdit_RandPoly->text();
    QString m_strRandPhase = ui->lineEdit_RandPhase->text();
    int m_ucRandLen = ui->comboBox_RandPolyLen->currentIndex();

    int m_interDep = ui->lineEdit_RSDecodeDeep->text().toInt();

    int tem_strLeng = m_strRandPoly.length();

    int m_ByfrmType = ui->comboBox_SubFrmSyncType->currentIndex();
    QString m_SubFrmRongcuoBitsQS = ui->lineEdit_SubFrameSyncFaultTolerant->text();
    QString m_SubFrmCheckErrBitsQS = ui->lineEdit_SubFaltToleranceOfFram->text();
    QString m_SubFrmLockBitsQS = ui->lineEdit_SubFrameSyncFaultTolerantLockBit->text();
    QString m_SubFrmCheckFrmsQS = ui->lineEdit_SubCheckFrameOfFaultFrameSync->text();
    QString m_SubFrmLockFrmsQS = ui->lineEdit_SubFrameSyncFaultLockDetection->text();
    int m_SubFrmRongcuoBits = ui->lineEdit_SubFrameSyncFaultTolerant->text().toInt();
    int m_SubFrmCheckErrBits = ui->lineEdit_SubFaltToleranceOfFram->text().toInt();
    int m_SubFrmLockBits = ui->lineEdit_SubFrameSyncFaultTolerantLockBit->text().toInt();
    int m_SubFrmCheckFrms = ui->lineEdit_SubCheckFrameOfFaultFrameSync->text().toInt();
    int m_SubFrmLockFrms = ui->lineEdit_SubFrameSyncFaultLockDetection->text().toInt();

    QString m_FrmCode = ui->lineEdit_FrmSyncGroup->text();
    QString m_CheckFrmCountQS = ui->lineEdit_CheckFrameOfFaultFrameSync->text();
    QString m_lockFrmCountQS = ui->lineEdit_FrameSyncFaultLockDetection->text();
    QString m_zhengtongbuRongCuoQS = ui->lineEdit_FrameSyncFaultTolerant->text();
    QString m_zhengtongbuRongcuoJiaoheQS = ui->lineEdit_FaltToleranceOfFram->text();
    QString m_lockRongCuoweiQS = ui->lineEdit_FrameSyncFaultTolerantLockBit->text();
    int m_CheckFrmCount = ui->lineEdit_CheckFrameOfFaultFrameSync->text().toInt();
    int m_lockFrmCount = ui->lineEdit_FrameSyncFaultLockDetection->text().toInt();
    int m_zhengtongbuRongCuo = ui->lineEdit_FrameSyncFaultTolerant->text().toInt();
    int m_zhengtongbuRongcuoJiaohe = ui->lineEdit_FaltToleranceOfFram->text().toInt();
    int m_lockRongCuowei = ui->lineEdit_FrameSyncFaultTolerantLockBit->text().toInt();

    int m_codeStyle = ui->comboBox_DecodeType->currentIndex();
    int m_codeType = ui->comboBox_CodeType->currentIndex();
    ulong m_CodeRate = ui->lineEdit_CodeRate->text().toLong();
    int m_Viterbi = ui->comboBox_RecurrentCodeType->currentIndex();
    ulong ulCodeRateIn = (ULONG)m_CodeRate;
    int nMinCodeRate = 100, nMaxCodeRate = 64000;

    int m_FrmLen = ui->lineEdit_FrameLen->text().toInt();
    bool ok;
    ulong m_CarrierFreq = (ulong)(ui->lineEdit_SubWaveFr->text().toDouble(&ok) * 1000);

    int m_SynLen = ui->comboBox_FrameSyncGroupLen->currentIndex();
    int m_WordLen = ui->lineEdit_WordLen->text().toInt();

    int m_RS;

    UI64   tempSynCode=0;
    UI64   tempSynCode1=0;

    int m_ByfrmLen = ui->lineEdit_FormatLen->text().toInt();

    int m_Interleav = m_interDep;
    int m_VFill;

    int m = 0;

    int temFrm20101104 = 0; //存放公式的计算结果

    int m_temp;

    bool temFlag;
    bool temFlag2;

    int VF_Len;

    int FrmLenMin;

    USHORT nIDPos1,nIDPos2;
    USHORT m_IDPos = ui->lineEdit_idCountPos->text().toInt();
    int m_ByfrmPos = ui->lineEdit_SubFrmSynGroupPos->text().toInt();
    int m_ByfrmCodeLen = ui->comboBox_SubFrameGroupLen->currentIndex();
    QString m_ByfrmCode = ui->lineEdit_SubFrmGroup->text();
    int m_CodePosi = ui->comboBox_FrameSyncCodePos->currentIndex();

    //遥测信息码  m_TMInfoFlagCode
    m_TMInfoFlagCode = ui->lineEdit_TelInfoFlag->text();
    if (m_TMInfoFlagCode.isEmpty())
    {
        strTemp = ("遥测信息码不能为空!");
        goto flag;
    }

    //选择解扰
    m_brandom = ui->comboBox_RandEn->currentText() == "人工设置" ? 1 : 0;
    if (m_brandom)
    {
        //解扰多项式
        if (m_strRandPoly.isEmpty())
        {
            strTemp = ("解扰多项式不可为空，请重新输入！");
            goto flag;
        }
        if (((m_ucRandLen + 1) * 4) < (tem_strLeng * 2))//(m_ulRandPoly > ulMaxRandPoly)
        {
            strTemp = ("输入的解扰多项式大于设定的多项式长度所能表示的最大值，请重新输入！");
            goto flag;
        }
        //解扰初相
        if (m_strRandPhase.isEmpty())
        {
            strTemp = ("解扰初相不可为空，请重新输入！");
            goto flag;
        }

        tem_strLeng = m_strRandPhase.length();
        if (((m_ucRandLen + 1) * 4) < (tem_strLeng * 2))//(m_ulRandPoly > ulMaxRandPoly)
        {
            strTemp = ("输入的解扰初相大于设定的多项式长度所能表示的最大值，请重新输入！");
            goto flag;
        }
    }

    if ((m_ByfrmType == 2) || (m_ByfrmType == 3))
    {
        if(m_SubFrmRongcuoBitsQS.isEmpty() || (m_SubFrmRongcuoBits < 0)||(m_SubFrmRongcuoBits > 7))
        {
            strTemp = ("副帧同步容错搜索容错位数范围为0~7");
            goto flag;
        }
        if(m_SubFrmCheckErrBitsQS.isEmpty() || (m_SubFrmCheckErrBits < 0)||(m_SubFrmCheckErrBits > 7))
        {
            strTemp = ("副帧同步容错校核容错位数范围为0~7");
            goto flag;
        }
        if(m_SubFrmLockBitsQS.isEmpty() || (m_SubFrmLockBits < 0)||(m_SubFrmLockBits > 7))
        {
            strTemp = ("副帧同步容错锁定容错位数范围为0~7");
            goto flag;
        }
    }
    if(m_ByfrmType!=0)
    {

        if(m_SubFrmCheckFrmsQS.isEmpty() || (m_SubFrmCheckFrms < 1)||(m_SubFrmCheckFrms > 8))
        {
            strTemp = ("副帧校核帧数范围为1~8!");
            goto flag;
        }
        if(m_SubFrmLockFrmsQS.isEmpty() || (m_SubFrmLockFrms < 1)||(m_SubFrmLockFrms > 8))
        {
            strTemp = ("副帧锁定帧数范围为1~8!");
            goto flag;
        }
    }

    if(m_FrmCode.isEmpty())
    {
        strTemp = ("子帧码组不可为空!");
        goto flag;
    }
    if(m_CheckFrmCountQS.isEmpty() || (m_CheckFrmCount < 1)||(m_CheckFrmCount > 8))
    {
        strTemp = ("帧同步容错校核检测帧数范围为1~8!");
        goto flag;
    }
    if(m_lockFrmCountQS.isEmpty() || (m_lockFrmCount < 1)||(m_lockFrmCount > 8))
    {
        strTemp = ("帧同步容错锁定检测帧数范围为1~8!");
        goto flag;
    }
    if(m_zhengtongbuRongCuoQS.isEmpty() || (m_zhengtongbuRongCuo < 0)||(m_zhengtongbuRongCuo > 7))
    {
        strTemp = ("帧同步容错搜索容错位数范围为0~7");
        goto flag;
    }
    if(m_zhengtongbuRongcuoJiaoheQS.isEmpty() || (m_zhengtongbuRongcuoJiaohe < 0)||(m_zhengtongbuRongcuoJiaohe > 7))
    {
        strTemp = ("帧同步容错校核容错位数范围为0~7");
        goto flag;
    }
    if(m_lockRongCuoweiQS.isEmpty() || (m_lockRongCuowei < 0)||(m_lockRongCuowei > 7))
    {
        strTemp = ("帧同步容错锁定容错位数范围为0~7");
        goto flag;
    }
    //

    if((m_CarrierFreq > 512000) || (m_CarrierFreq < 5000) || !ok)
    {

        strTemp = ("副载波的取值范围应该为：[5~512]kHz");
        goto flag;
    }


    if((m_codeStyle == 1) || (m_codeStyle == 3))
    {
        if (m_Viterbi == 0)
        {
            nMaxCodeRate /= 2;
        }
        else if (m_Viterbi == 1)
        {
            nMaxCodeRate = nMaxCodeRate * 3 / 4;
        }
    }
    if(m_codeType > 2)
    {
        nMaxCodeRate /= 2;
    }
    if((ulCodeRateIn > nMaxCodeRate) || (ulCodeRateIn < nMinCodeRate))
    {
        strTemp = QString("码率取值范围应该为：[%1~%2]bps").arg(nMinCodeRate).arg(nMaxCodeRate);
        goto flag;
    }

    if(m_CarrierFreq % ulCodeRateIn != 0)
    {
        strTemp = QString("副载波输入值%1Hz不是码率输入值%2bps的整数倍.").arg(m_CarrierFreq).arg(ulCodeRateIn);
        goto flag;
    }

    if( (m_CarrierFreq / ulCodeRateIn < 2) || m_CarrierFreq / ulCodeRateIn > 512)
    {
        strTemp = QString("不满足条件: 2 <= (副载波输入值 / 码率输入值) <= 512.");
        goto flag;
    }

    //帧长
    if((m_codeStyle == 0)||(m_codeStyle == 1))
    {
        if((m_FrmLen < 4) || (m_FrmLen > 1024))
        {
            strTemp = QString("帧长取值范围应该为：[%1~%2]字。").arg(4).arg(1024);
            goto flag;
        }
    }

    //字长
    if((m_WordLen < 8) || (m_WordLen > 8))
    {
        strTemp = ("字长固定为8！");
        goto flag;
    }

    tempSynCode1 = StrToSynCode(m_FrmCode.toStdString().c_str());
    tempSynCode = tempSynCode1<<(64 - ((m_SynLen + 2) * 8));
    tempSynCode = tempSynCode>>(64 - ((m_SynLen + 2) * 8));
    if(tempSynCode != tempSynCode1)
    {

        strTemp = ("输入同步码组的长度大于设置的同步码组长度,请重新输入。");
        goto flag;
    }

    //副帧长度

    if(m_ByfrmType != 0)
    {
        if (ui->lineEdit_FormatLen->text().isEmpty())
        {
            strTemp = "格式长不能为空！";
            goto flag;
        }
        if((m_ByfrmLen < 4) || (m_ByfrmLen > 256))
        {

            strTemp = QString("格式长取值范围应该为：[%1～%2]帧。").arg(4).arg(256);
            goto flag;
        }
    }
    if ((2 == m_codeStyle) || (3 == m_codeStyle))//有RS编码 (1 == m_RS) || (2 == m_RS)
    {
        if ((m_interDep < 1)||(m_interDep > 5))
        {
            strTemp = ("交错深度的范围为1~5!");
            goto flag;
        }
        temFrm20101104 = 255 * m_Interleav + (m_SynLen+2);
        if(temFrm20101104 == m_FrmLen) //此时没有虚拟填充
        {
            m_VFill = 0;
        }
        else
        {
            temFlag = TRUE;
            temFlag2 = TRUE;
            VF_Len = temFrm20101104 - m_FrmLen;
            if(VF_Len < 0)
            {
                temFlag = FALSE;
            }
            if(VF_Len >= 255 * m_Interleav)  //交错深度太大
            {
                temFlag = FALSE;
            }
            if(VF_Len % m_Interleav != 0)
            {
                temFlag2 = FALSE;
            }
            m = VF_Len / m_Interleav;
            if(temFlag2 == FALSE)
            {
                strTemp = ("帧长,交织深度，RS码类型三者不匹配，请重新输入!(总帧长 - 子帧长 = 交错深度*N)");
                goto flag;
            }
            m_RS = ui->comboBox_RSType->currentIndex();
            if(m_RS == 0)
            {
                if((0 >= m)||( m > 222))
                {
                    FrmLenMin = temFrm20101104 - 222 * m_Interleav;
                    strTemp = QString("此时帧长范围应该为: %1 ~ %2，且应满足(总帧长 - 子帧长 = 交错深度*N)")
                            .arg(FrmLenMin).arg(temFrm20101104);
                    goto flag;
                }
            }
            else // m_RS == 1
            {
                if((0 >= m) ||( m > 238))
                {
                    FrmLenMin = temFrm20101104 - 238 * m_Interleav;
                    strTemp = QString("此时帧长范围应该为: %1 ~ %2，且应满足(总帧长 - 子帧长 = 交错深度*N)")
                            .arg(FrmLenMin).arg(temFrm20101104);;
                    goto flag;
                }
            }
            if(temFlag == FALSE)
            {
                strTemp = ("帧长,交织深度，RS码类型三者不匹配，请重新输入!");
                goto flag;
            }
            m_VFill = 1;
        }
    }
    if ((m_codeStyle == 0)||(m_codeStyle == 1))
    {
        if ((2 == m_ByfrmType) || (1 == m_ByfrmType))
        {
            //子帧同步码组位置固定在帧首
            //ID字位置范围
            nIDPos1 = ((m_SynLen + 2) * 8) / m_WordLen;
            if(m_FrmLen < nIDPos1)
            {
                strTemp = ("帧长不能小于子帧码组的长度!");
                goto flag;

            }
            nIDPos2 = (m_FrmLen < 1024 ? m_FrmLen : 1024);
        }
    }


    //ID副帧和循环副帧的副帧位置
    if(m_ByfrmType==1)
    {
        //if ((0 == m_RS) || (1 == m_RS))//子帧同步码组位置固定在帧首  //8-12
        if ((m_codeStyle == 0)||(m_codeStyle == 1))
        {
            if((nIDPos1 + 1) > nIDPos2)
            {
                strTemp = ("ID字位置错误，帧长太小!");
                goto flag;

            }
            if((m_IDPos <= nIDPos1) || (m_IDPos > nIDPos2))
            {
                // 				ss.Format("ID字位置取值范围应该为：[%d~%d]字!", nIDPos1+1, nIDPos2);
                // //           ss.Format("IDPosition input oversteps the range[%d~%d]word!", nIDPos1+1, nIDPos2);
                // 				MessageBox(ss,MyTitleDefineInFrame,MB_OK | MB_ICONWARNING);
                // 				return FALSE;
            }
            if(m_IDPos < 0)
            {
                strTemp = ("ID字位置错误!");
                goto flag;
            }
            if(m_IDPos<1)
            {
                strTemp = ("ID字位置不能小于1.");
                goto flag;
            }
            //此时子帧同步码位置可选 m_CodePosi

            if (m_CodePosi == 0) //子帧同步码组位置帧前
            {
                if (m_IDPos <= ((m_SynLen + 2) * 8) / m_WordLen)
                {
                    strTemp = ("ID字位置不可占据帧同步码组的位置!请重新输入。");
                    goto flag;
                }
                if (m_IDPos > m_FrmLen) //Id字位置大于帧长
                {
                    strTemp = ("ID字位置不可超出子帧长度!");
                    goto flag;
                }
            }
            if (m_CodePosi == 1)//子帧同步码组位置帧尾
            {
                if(m_IDPos > (m_FrmLen - (((m_SynLen + 2) * 8) / m_WordLen))) //ID字位置占据同步码的位置
                {
                    strTemp = ("ID字位置不可超出子帧长度，并且不能占据帧同步码组的位置!请重新输入。");
                    goto flag;
                }
            }
        }
    }
    else if(m_ByfrmType == 2)
    {
        //if ((0 == m_RS) || (1 == m_RS))//子帧同步码组位置固定在帧
        if ((m_codeStyle == 0)||(m_codeStyle == 1))
        {
            if((nIDPos1 + 1) > nIDPos2)
            {
                strTemp = ("副帧码组位置错误，帧长太小!");
                goto flag;

            }
            if(m_ByfrmPos < 1)
            {
                strTemp = ("循环副帧码组位置不可小于1.");
                goto flag;
            }

            m_temp = m_FrmLen-(((m_SynLen + 2) * 8)/m_WordLen)-(((m_ByfrmCodeLen + 1) * 8)/m_WordLen) + 1;
            //此时子帧同步码位置可选 m_CodePosi   2010-09-19
            if (m_CodePosi == 0) //子帧同步码组位置帧前
            {
                if (m_ByfrmPos <= ((m_SynLen + 2) * 8)/m_WordLen)
                {
                    strTemp = ("循环副帧码组位置不可占据帧同步码组的位置。请重新输入。");
                    goto flag;
                }
                if (m_ByfrmPos > (m_FrmLen - (((m_ByfrmCodeLen + 1) * 8)/m_WordLen) + 1)) //
                {
                    strTemp = ("循环副帧码组位置不可超出子帧长度!");
                    goto flag;
                }
            }
            if (m_CodePosi == 1)//子帧同步码组位置帧尾
            {
                if (m_ByfrmPos <= 0)
                {
                    strTemp = ("循环副帧码组位置不能小于等于0。请重新输入。");
                    goto flag;
                }
                if(m_ByfrmPos > m_temp)
                {
                    strTemp = ("循环副帧码组位置不可超出子帧长度，并且不能占据帧同步码组的位置。请重新输入。");
                    goto flag;
                }
            }
        }
        if(m_ByfrmCode == "")
        {
            strTemp = ("副帧码组不能为空!");
            goto flag;
        }

        tempSynCode1 = StrToSynCode(m_ByfrmCode.toStdString().c_str());
        tempSynCode = tempSynCode1<<(64 - ((m_ByfrmCodeLen + 1) * 8));
        tempSynCode = tempSynCode>>(64 - ((m_ByfrmCodeLen + 1) * 8));
        if(tempSynCode != tempSynCode1)
        {
            strTemp = ("输入副帧同步码组的长度大于设置的副帧同步码组长度,请重新输入。");
            goto flag;
        }
    }
    return 1;
flag:
    msgBox.setText(strTemp);
    msgBox.exec();
    return -1;

}
//定时器
void USBBBE_CodeTMParaSetGUIDlg::timerEvent(QTimerEvent *event)
{
    ShowPara(0);
}

//更改参数按钮休息处理函数
void USBBBE_CodeTMParaSetGUIDlg::on_pushButton_Modify_clicked()
{
    if(ui->pushButton_Modify->text() == "更改")
    {
        EnabledControls(true);

        killTimer(m_TimerID);
        on_comboBox_DecodeType_currentIndexChanged(ui->comboBox_DecodeType->currentIndex());
        on_comboBox_SubFrmSyncType_currentIndexChanged(ui->comboBox_SubFrmSyncType->currentIndex());
        on_comboBox_RecurrentCodeType_currentIndexChanged(ui->comboBox_RecurrentCodeType->currentIndex());
        on_comboBox_RandEn_currentIndexChanged(ui->comboBox_RandEn->currentIndex());

        return;
    }

    if(ui->pushButton_Modify->text() == "恢复")
    {
        EnabledControls(false);
        m_TimerID = startTimer(500);
        return;
    }
}

void USBBBE_CodeTMParaSetGUIDlg::on_pushButton_Set_clicked()
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
            byteArray.resize(sizeof(m_MCSUSBTMParam));
            memcpy(byteArray.data(),&m_MCSUSBTMParam,sizeof(MCSUSBTMParamStruct));
            SendMessage((char*)&m_MCSUSBTMParam, sizeof(MCSUSBTMParamStruct), m_usC ,m_usTID);
        }

        //日志信息
        QString strLog;
        strLog += "解调参数设置: 设置参数成功！";
        CLog::addLog(strLog);

        //不使能控件
        EnabledControls(false);
        m_TimerID = startTimer(500);
    }
}

void USBBBE_CodeTMParaSetGUIDlg::closeEvent(QCloseEvent *event)
{
    event->accept();
}

QSize USBBBE_CodeTMParaSetGUIDlg::sizeHint() const
{
    return QSize(this->width(), this->height());
}

void USBBBE_CodeTMParaSetGUIDlg::SubFrmEn(bool bFlag)
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
void USBBBE_CodeTMParaSetGUIDlg::IDSubFrmEn(bool bFlag)
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
void USBBBE_CodeTMParaSetGUIDlg::CycSubFrmEn(bool bFlag)
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
void USBBBE_CodeTMParaSetGUIDlg::ReserveSubFrmEn(bool bFlag)
{

}

void USBBBE_CodeTMParaSetGUIDlg::SendMessage(char* pData, unsigned short  usLen, unsigned short usC, unsigned short usTID)
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

DWORDLONG USBBBE_CodeTMParaSetGUIDlg::StrToSynCode(const char * pStr)
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
//译码方式的监听
void USBBBE_CodeTMParaSetGUIDlg::on_comboBox_DecodeType_currentIndexChanged(int index)
{
    if (ui->pushButton_Modify->text() == "更改")
        return;
    if (index== 0){//关
        ui->comboBox_RecurrentCodeType->setEnabled(false);
        ui->comboBox_RSType->setEnabled(false);
        ui->comboBox_RSDecodePlot->setEnabled(false);
        ui->lineEdit_RSDecodeDeep->setEnabled(false);
        ui->comboBox_ViterbiPhase->setEnabled(false);
        ui->comboBox_SubFrmSyncType->setEnabled(true);
        ui->comboBox_FrameSyncCodePos->setEnabled(true);
    }
    if(index==1){
        ui->comboBox_RecurrentCodeType->setEnabled(true);
        ui->comboBox_RSType->setEnabled(false);
        ui->comboBox_RSDecodePlot->setEnabled(false);
        ui->lineEdit_RSDecodeDeep->setEnabled(false);
        ui->comboBox_ViterbiPhase->setEnabled(true);
        ui->comboBox_SubFrmSyncType->setEnabled(true);
        ui->comboBox_FrameSyncCodePos->setEnabled(true);
        on_comboBox_RecurrentCodeType_currentIndexChanged(ui->comboBox_RecurrentCodeType->currentIndex());
    }
    if(index==2){
        ui->comboBox_RecurrentCodeType->setEnabled(false);
        ui->comboBox_RSType->setEnabled(true);
        ui->comboBox_RSDecodePlot->setEnabled(true);
        ui->lineEdit_RSDecodeDeep->setEnabled(true);
        ui->comboBox_ViterbiPhase->setEnabled(false);
        ui->comboBox_SubFrmSyncType->setEnabled(false);
        ui->comboBox_SubFrmSyncType->setCurrentIndex(0);
        ui->comboBox_FrameSyncCodePos->setEnabled(false);
        ui->comboBox_FrameSyncCodePos->setCurrentIndex(0);
        on_comboBox_SubFrmSyncType_currentIndexChanged(ui->comboBox_SubFrmSyncType->currentIndex());

    }
    if(index==3){
        ui->comboBox_RecurrentCodeType->setEnabled(true);
        ui->comboBox_RSType->setEnabled(true);
        ui->comboBox_RSDecodePlot->setEnabled(true);
        ui->lineEdit_RSDecodeDeep->setEnabled(true);
        ui->comboBox_ViterbiPhase->setEnabled(true);
        ui->comboBox_SubFrmSyncType->setEnabled(false);
        ui->comboBox_SubFrmSyncType->setCurrentIndex(0);
        ui->comboBox_FrameSyncCodePos->setEnabled(false);
        ui->comboBox_FrameSyncCodePos->setCurrentIndex(0);
        on_comboBox_SubFrmSyncType_currentIndexChanged(ui->comboBox_SubFrmSyncType->currentIndex());
        on_comboBox_RecurrentCodeType_currentIndexChanged(ui->comboBox_RecurrentCodeType->currentIndex());
    }

}
//卷积码
void USBBBE_CodeTMParaSetGUIDlg::on_comboBox_RecurrentCodeType_currentIndexChanged(int index)
{
    if (ui->pushButton_Modify->text() == "更改")
        return;
    // 1/2
    if(index == 0){
        if(ui->comboBox_DecodeType->currentIndex() == 1 ||
                ui->comboBox_DecodeType->currentIndex() == 3)
            ui->comboBox_ViterbiPhase->setEnabled(true);
    }
    // 3/4
    if(index== 1){
        ui->comboBox_ViterbiPhase->setEnabled(false);
        ui->comboBox_ViterbiPhase->setCurrentIndex(0);

    }

}


void USBBBE_CodeTMParaSetGUIDlg::on_comboBox_SubFrmSyncType_currentIndexChanged(int index)
{
    if (ui->pushButton_Modify->text() == "更改")
        return;

    if(index==0){//无副帧

        ui->comboBox_SubFrameGroupLen->setEnabled(false);
        ui->lineEdit_SubFrmSynGroupPos->setEnabled(false);
        ui->lineEdit_SubFrmGroup->setEnabled(false);
        ui->comboBox_IDBaseValue->setEnabled(false);
        ui->comboBox_idDirect->setEnabled(false);
        ui->lineEdit_idCountPos->setEnabled(false);
        ui->lineEdit_SubFrameSyncFaultTolerant->setEnabled(false);
        ui->lineEdit_SubFaltToleranceOfFram->setEnabled(false);
        ui->lineEdit_SubFrameSyncFaultTolerantLockBit->setEnabled(false);
        ui->lineEdit_SubCheckFrameOfFaultFrameSync->setEnabled(false);
        ui->lineEdit_SubFrameSyncFaultLockDetection->setEnabled(false);
        ui->lineEdit_FormatLen->setEnabled(false);

    }
    if(index==2){//循环副帧
        ui->comboBox_SubFrameGroupLen->setEnabled(true);
        ui->lineEdit_SubFrmSynGroupPos->setEnabled(true);
        ui->lineEdit_SubFrmGroup->setEnabled(true);
        ui->comboBox_IDBaseValue->setEnabled(false);
        ui->comboBox_idDirect->setEnabled(false);
        ui->lineEdit_idCountPos->setEnabled(false);
        ui->lineEdit_SubFrameSyncFaultTolerant->setEnabled(true);
        ui->lineEdit_SubFaltToleranceOfFram->setEnabled(true);
        ui->lineEdit_SubFrameSyncFaultTolerantLockBit->setEnabled(true);
        ui->lineEdit_SubCheckFrameOfFaultFrameSync->setEnabled(true);
        ui->lineEdit_SubFrameSyncFaultLockDetection->setEnabled(true);

        ui->lineEdit_FormatLen->setEnabled(true);
    }
    if(index==1){//ID副帧

        ui->comboBox_SubFrameGroupLen->setEnabled(false);
        ui->lineEdit_SubFrmSynGroupPos->setEnabled(false);
        ui->lineEdit_SubFrmGroup->setEnabled(false);
        ui->comboBox_IDBaseValue->setEnabled(true);
        ui->comboBox_idDirect->setEnabled(true);
        ui->lineEdit_idCountPos->setEnabled(true);
        ui->lineEdit_SubFrameSyncFaultLockDetection->setEnabled(true);
        ui->lineEdit_SubCheckFrameOfFaultFrameSync->setEnabled(true);
        ui->lineEdit_SubFrameSyncFaultTolerant->setEnabled(false);
        ui->lineEdit_SubFaltToleranceOfFram->setEnabled(false);
        ui->lineEdit_SubFrameSyncFaultTolerantLockBit->setEnabled(false);
        ui->lineEdit_FormatLen->setEnabled(true);


    }
    if(index==3){//反码副帧
        ui->comboBox_SubFrameGroupLen->setEnabled(false);
        ui->lineEdit_SubFrmSynGroupPos->setEnabled(false);
        ui->lineEdit_SubFrmGroup->setEnabled(false);
        ui->comboBox_IDBaseValue->setEnabled(false);
        ui->comboBox_idDirect->setEnabled(false);
        ui->lineEdit_idCountPos->setEnabled(false);
        ui->lineEdit_SubFrameSyncFaultTolerant->setEnabled(true);
        ui->lineEdit_SubFaltToleranceOfFram->setEnabled(true);
        ui->lineEdit_SubFrameSyncFaultTolerantLockBit->setEnabled(true);
        ui->lineEdit_SubCheckFrameOfFaultFrameSync->setEnabled(true);
        ui->lineEdit_SubFrameSyncFaultLockDetection->setEnabled(true);
        ui->lineEdit_FormatLen->setEnabled(true);
    }
}

void USBBBE_CodeTMParaSetGUIDlg::on_comboBox_RandEn_currentIndexChanged(int index)
{
    if (ui->pushButton_Modify->text() == "更改")
        return;
    ui->comboBox_RandPolyLen->setEnabled(false);
    ui->lineEdit_RandPoly->setEnabled(false);
    ui->lineEdit_RandPhase->setEnabled(false);
    if(index==2){//人工
        ui->comboBox_RandPolyLen->setEnabled(true);
        ui->lineEdit_RandPoly->setEnabled(true);
        ui->lineEdit_RandPhase->setEnabled(true);

    }
    else{
        ui->comboBox_RandPolyLen->setEnabled(false);
        ui->lineEdit_RandPoly->setEnabled(false);
        ui->lineEdit_RandPhase->setEnabled(false);

    }
}
