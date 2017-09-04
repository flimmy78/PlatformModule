#include "DDTBBE_QCodFrmSynSetGUIDlg.h"
#include "ui_DDTBBE_QCodFrmSynSetGUIDlg.h"
#include <QCloseEvent>
#include <QMessageBox>
#include <QDateTime>
#include <QDebug>

extern quint64 GetID(ushort usStationID, uchar ucSubSysID, ushort usDeviceID, ushort usType,
                     uchar ucSn);
DDTBBE_CodFrmSynSetGUIDlg::DDTBBE_CodFrmSynSetGUIDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DDTBBE_CodFrmSynSetGUIDlg)
{
    ui->setupUi(this);
    m_pStateMgr = NULL;
    m_pILxTsspMessageManager = NULL;
    m_pILxTsspLogManager = NULL;
    m_pPublicInfo = NULL;
    m_pHAManager = NULL;
    //m_killTimer = false;
    m_MCSDDTTMIParamStatus = NULL;
    m_MCSDDTRevStatus = NULL;
    setAttribute(Qt::WA_DeleteOnClose);
}

DDTBBE_CodFrmSynSetGUIDlg::~DDTBBE_CodFrmSynSetGUIDlg()
{
    killTimer(m_TimerID);
    delete ui;
}

void DDTBBE_CodFrmSynSetGUIDlg::setText(const QString& strTitle, ParaStruct xmlsetText[3][60], QString MapMean[50][30], int MapNum[50], uchar Type, ILxTsspParameterMacro* pMacroObj)
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
void DDTBBE_CodFrmSynSetGUIDlg::InitDLG(int Type, ILxTsspParameterMacro* pMacroObj)
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
            m_pHAManager->GetParameter(m_ParaMCSDDTTMIParamID, byteArray);
            memcpy(&m_MCSDDTTMIParam, byteArray.data(), sizeof(MCSDDTTMIParamStruct));
        }

        if(m_pStateMgr != NULL)
        {
            DWORD stLen = sizeof(MCSDDTTMIStatusStruct);
            m_MCSDDTTMIParamStatus = (MCSDDTTMIStatusStruct*)m_pStateMgr->FindOneItem(m_StatusParaMCSDDTTMIParamID, stLen);
            if(m_MCSDDTTMIParamStatus == NULL || stLen != sizeof(MCSDDTTMIStatusStruct))
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
            pMacroObj->GetParameterBlock(m_ParaMCSDDTTMIParamID, byteArray);
            memcpy(&m_MCSDDTTMIParam, byteArray.data(), sizeof(MCSDDTTMIParamStruct));
            byteArray.clear();
            pMacroObj->GetParameterBlock(m_ParaRecSetParamID, byteArray);
            memcpy(&m_MCSDDTRevParam, byteArray.data(), sizeof(m_MCSDDTRevParam));
        }
        ui->pushButton_Modify->setVisible(false);
        ui->pushButton_Set->setVisible(false);

        //显示参数
        ShowPara(1);
    }
}

//设置输入格式
void DDTBBE_CodFrmSynSetGUIDlg::InitInput()
{
    //正整数
    QRegExp regExp("[0-9]{1,16}");
    QRegExp regExp_CodeRate("[0-9]{1,5}");
    QRegExp regExp_FrameLen("[0-9]{1,4}");
    //码速率
    ui->lineEdit_CodeRate->setValidator(new QRegExpValidator(regExp_CodeRate, this));
    //帧长
    ui->lineEdit_FrameLen->setValidator(new QRegExpValidator(regExp_FrameLen, this));
    //副帧长
    ui->lineEdit_SubFrmLen->setValidator(new QRegExpValidator(regExp_FrameLen, this));
    //副帧码组位置
    ui->lineEdit_SubFrmSynGroupPos->setValidator(new QRegExpValidator(regExp_FrameLen, this));
    //ID计数位置
    ui->lineEdit_idCountPos->setValidator(new QRegExpValidator(regExp_FrameLen, this));

    //字母数字
    QRegExp regExp2("[A-Fa-f0-9]{1,8}");
    //帧同步码组
    ui->lineEdit_FrmSyncGroup->setValidator(new QRegExpValidator(regExp2, this));
    //解扰初相
    ui->lineEdit_RandPhase->setValidator(new QRegExpValidator(regExp2, this));
    //解扰多项式
    ui->lineEdit_RandPoly->setValidator(new QRegExpValidator(regExp2, this));
    //副帧同步码组
    ui->lineEdit_SubFrmGroup->setValidator(new QRegExpValidator(regExp2, this));
}

void DDTBBE_CodFrmSynSetGUIDlg::ShowControls()
{
    //向下拉框控件中写入从xml中读取的内容

    int i;
    //码型
    const int size  = m_MCSDDTRevParam.ucGrayType  == 9 ? 3 : 1;
    ui->comboBox_CodeType->clear();
    for(i=0; i<size;i++)
    {
        ui->comboBox_CodeType->addItem(m_DlgMapMean[0][i]);
    }
    ui->comboBox_CodeType->setCurrentIndex(m_DlgParaSetStruct[0][0].InitVal.lval);
    //数据输出方式
    ui->comboBox_OutputType->clear();
    for(i=0; i<DlgMapLineNum[1];i++)
    {
        ui->comboBox_OutputType->addItem(m_DlgMapMean[1][i]);
    }
    ui->comboBox_OutputType->setCurrentIndex(m_DlgParaSetStruct[0][2].InitVal.lval);
    //位流
    const int sizeTmp  = m_DlgParaSetStruct[0][2].InitVal.lval == 0 ? 2 : 5;

    //译码方式
    ui->comboBox_DecodeType->clear();
    for(i=0; i<sizeTmp;i++)
    {
        ui->comboBox_DecodeType->addItem(m_DlgMapMean[3][i]);
    }
    ui->comboBox_DecodeType->setCurrentIndex(m_DlgParaSetStruct[0][4].InitVal.lval);
    //卷积码类型
    ui->comboBox_RecurrentCodeType->clear();
    for(i=0; i<DlgMapLineNum[4];i++)
    {
        ui->comboBox_RecurrentCodeType->addItem(m_DlgMapMean[4][i]);
    }
    ui->comboBox_RecurrentCodeType->setCurrentIndex(m_DlgParaSetStruct[0][5].InitVal.lval);
    //RS码型
    ui->comboBox_RSType->clear();
    for(i=0; i<DlgMapLineNum[5];i++)
    {
        ui->comboBox_RSType->addItem(m_DlgMapMean[5][i]);
    }
    ui->comboBox_RSType->setCurrentIndex(m_DlgParaSetStruct[0][6].InitVal.lval);
    //RS码坐标
    ui->comboBox_RSDecodePlot->clear();
    for(i=0; i<DlgMapLineNum[6];i++)
    {
        ui->comboBox_RSDecodePlot->addItem(m_DlgMapMean[6][i]);
    }
    ui->comboBox_RSDecodePlot->setCurrentIndex(m_DlgParaSetStruct[0][7].InitVal.lval);
    //交织深度
    ui->comboBox_RSDecodeDeep->clear();
    for(i=0; i<DlgMapLineNum[7];i++)
    {
        ui->comboBox_RSDecodeDeep->addItem(m_DlgMapMean[7][i]);
    }
    ui->comboBox_RSDecodeDeep->setCurrentIndex(m_DlgParaSetStruct[0][8].InitVal.lval);
    //维特比输出相位
    ui->comboBox_ViterbiPhase->clear();
    for(i=0; i<DlgMapLineNum[8];i++)
    {
        ui->comboBox_ViterbiPhase->addItem(m_DlgMapMean[8][i]);
    }
    ui->comboBox_ViterbiPhase->setCurrentIndex(m_DlgParaSetStruct[0][9].InitVal.lval);
    //帧码组长
    ui->comboBox_FrameSyncGroupLen->clear();
    for(i=0; i<DlgMapLineNum[9];i++)
    {
        qDebug()<<m_DlgMapMean[9][i];
        ui->comboBox_FrameSyncGroupLen->addItem(m_DlgMapMean[9][i]);
    }
    ui->comboBox_FrameSyncGroupLen->setCurrentIndex(m_DlgParaSetStruct[1][1].InitVal.lval);
    qDebug()<<m_DlgParaSetStruct[1][1].InitVal.lval;
    //解扰控制
    ui->comboBox_RandEn->clear();
    for(i=0; i<DlgMapLineNum[10];i++)
    {
        ui->comboBox_RandEn->addItem(m_DlgMapMean[10][i]);
    }
    ui->comboBox_RandEn->setCurrentIndex(m_DlgParaSetStruct[1][3].InitVal.lval);
    //解扰多项式长度
    ui->comboBox_RandPolyLen->clear();
    for(i=0; i<DlgMapLineNum[11];i++)
    {
        ui->comboBox_RandPolyLen->addItem(m_DlgMapMean[11][i]);
    }
    ui->comboBox_RandPolyLen->setCurrentIndex(m_DlgParaSetStruct[1][4].InitVal.lval);
    //帧码组位置
    ui->comboBox_FrmPlace->clear();
    for(i=0; i<DlgMapLineNum[12];i++)
    {
        ui->comboBox_FrmPlace->addItem(m_DlgMapMean[12][i]);
    }
    ui->comboBox_FrmPlace->setCurrentIndex(m_DlgParaSetStruct[1][5].InitVal.lval);
    //副帧方式
    ui->comboBox_SubFrmSyncType->clear();
    for(i=0; i<DlgMapLineNum[13];i++)
    {
        ui->comboBox_SubFrmSyncType->addItem(m_DlgMapMean[13][i]);
    }
    ui->comboBox_SubFrmSyncType->setCurrentIndex(m_DlgParaSetStruct[1][9].InitVal.lval);
    //副帧码组长度
    ui->comboBox_SubFrameGroupLen->clear();
    for(i=0; i<DlgMapLineNum[14];i++)
    {
        ui->comboBox_SubFrameGroupLen->addItem(m_DlgMapMean[14][i]);
    }
    ui->comboBox_SubFrameGroupLen->setCurrentIndex(m_DlgParaSetStruct[1][10].InitVal.lval);
    //ID计数基值
    ui->comboBox_IDBaseValue->clear();
    for(i=0; i<DlgMapLineNum[15];i++)
    {
        ui->comboBox_IDBaseValue->addItem(m_DlgMapMean[15][i]);
    }
    ui->comboBox_IDBaseValue->setCurrentIndex(m_DlgParaSetStruct[1][13].InitVal.lval);
    //ID计数方向
    ui->comboBox_idDirect->clear();
    for(i=0; i<DlgMapLineNum[16];i++)
    {
        ui->comboBox_idDirect->addItem(m_DlgMapMean[16][i]);
    }
    ui->comboBox_OutputType->setCurrentIndex(m_DlgParaSetStruct[1][14].InitVal.lval);
    //帧同步容错搜索容错位数
    ui->comboBox_FrameSyncFaultTolerant->clear();
    for(i=0; i<DlgMapLineNum[17];i++)
    {
        ui->comboBox_FrameSyncFaultTolerant->addItem(m_DlgMapMean[17][i]);
    }
    ui->comboBox_FrameSyncFaultTolerant->setCurrentIndex(m_DlgParaSetStruct[2][0].InitVal.lval);
    //帧同步容错校核容错位数
    ui->comboBox_FaltToleranceOfFrameSync->clear();
    for(i=0; i<DlgMapLineNum[18];i++)
    {
        ui->comboBox_FaltToleranceOfFrameSync->addItem(m_DlgMapMean[18][i]);
    }
    ui->comboBox_FaltToleranceOfFrameSync->setCurrentIndex(m_DlgParaSetStruct[2][1].InitVal.lval);
    //帧同步锁定
    ui->comboBox_FrameSyncFaultTolerantLockBit->clear();
    for(i=0; i<DlgMapLineNum[19];i++)
    {
        ui->comboBox_FrameSyncFaultTolerantLockBit->addItem(m_DlgMapMean[19][i]);
    }
    ui->comboBox_FrameSyncFaultTolerantLockBit->setCurrentIndex(m_DlgParaSetStruct[2][2].InitVal.lval);
    //帧同步容错校核检测帧数
    ui->comboBox_CheckFrameOfFaultFrameSync->clear();
    for(i=0; i<DlgMapLineNum[20];i++)
    {
        ui->comboBox_CheckFrameOfFaultFrameSync->addItem(m_DlgMapMean[20][i]);
    }
    ui->comboBox_CheckFrameOfFaultFrameSync->setCurrentIndex(m_DlgParaSetStruct[2][3].InitVal.lval);
    //帧同步容错SUODING
    ui->comboBox_FrameSyncFaultLockDetection->clear();
    for(i=0; i<DlgMapLineNum[21];i++)
    {
        ui->comboBox_FrameSyncFaultLockDetection->addItem(m_DlgMapMean[21][i]);
    }
    ui->comboBox_FrameSyncFaultLockDetection->setCurrentIndex(m_DlgParaSetStruct[2][4].InitVal.lval);
    //副帧同步容错校核容错位数
    ui->comboBox_SubFrameSyncFaultTolerant->clear();
    for(i=0; i<DlgMapLineNum[22];i++)
    {
        ui->comboBox_SubFrameSyncFaultTolerant->addItem(m_DlgMapMean[22][i]);
    }
    ui->comboBox_SubFrameSyncFaultTolerant->setCurrentIndex(m_DlgParaSetStruct[2][5].InitVal.lval);
    //副帧同步容错jiaohe容错位数
    ui->comboBox_SubFaltToleranceOfFrameSync->clear();
    for(i=0; i<DlgMapLineNum[23];i++)
    {
        ui->comboBox_SubFaltToleranceOfFrameSync->addItem(m_DlgMapMean[23][i]);
    }
    ui->comboBox_SubFaltToleranceOfFrameSync->setCurrentIndex(m_DlgParaSetStruct[2][6].InitVal.lval);
    //副帧同步容错锁定容错位数
    ui->comboBox_SubFrameSyncFaultTolerantLockBit->clear();
    for(i=0; i<DlgMapLineNum[24];i++)
    {
        ui->comboBox_SubFrameSyncFaultTolerantLockBit->addItem(m_DlgMapMean[24][i]);
    }
    ui->comboBox_SubFrameSyncFaultTolerantLockBit->setCurrentIndex(m_DlgParaSetStruct[2][7].InitVal.lval);
    //副帧校核帧数
    ui->comboBox_SubCheckFrameOfFaultFrameSync->clear();
    for(i=0; i<DlgMapLineNum[25];i++)
    {
        ui->comboBox_SubCheckFrameOfFaultFrameSync->addItem(m_DlgMapMean[25][i]);
    }
    ui->comboBox_SubCheckFrameOfFaultFrameSync->setCurrentIndex(m_DlgParaSetStruct[2][8].InitVal.lval);
    //副帧锁定帧数
    ui->comboBox_SubFrameSyncFaultLockDetection->clear();
    for(i=0; i<DlgMapLineNum[26];i++)
    {
        ui->comboBox_SubFrameSyncFaultLockDetection->addItem(m_DlgMapMean[26][i]);
    }
    ui->comboBox_SubFrameSyncFaultLockDetection->setCurrentIndex(m_DlgParaSetStruct[2][9].InitVal.lval);
    //往静态控件中写入参数缺省值
    //码速率
    ui->lineEdit_CodeRate->setText(QString::number(m_DlgParaSetStruct[0][1].InitVal.lval));
    //    ui->label_CodeRate->setText("码速率\n["+QString::number(m_DlgParaSetStruct[0][1].MinVal.lmin) + "~" + QString::number(m_DlgParaSetStruct[0][1].MaxVal.lmax) + "]");
    //帧长
    ui->lineEdit_FrameLen->setText(QString::number(m_DlgParaSetStruct[1][0].InitVal.lval));
    //ui->label_FrameLen->setText("帧长\n["+QString::number(m_DlgParaSetStruct[1][0].MinVal.lmin) + "~" + QString::number(m_DlgParaSetStruct[1][0].MaxVal.lmax) + "]");
    //帧同步码组
    ui->lineEdit_FrmSyncGroup->setText(QString::number(m_DlgParaSetStruct[1][2].InitVal.lval,16).toUpper());
    //解扰初相
    ui->lineEdit_RandPhase->setText(QString::number(m_DlgParaSetStruct[1][6].InitVal.lval,16).toUpper());
    //解扰多项式
    ui->lineEdit_RandPoly->setText(QString::number(m_DlgParaSetStruct[1][7].InitVal.lval,16).toUpper());
    //副帧长
    ui->lineEdit_SubFrmLen->setText(QString::number(m_DlgParaSetStruct[1][8].InitVal.lval));
    ui->label_SubFrmLen->setText("副帧长\n["+QString::number(m_DlgParaSetStruct[1][8].MinVal.lmin) + "~" + QString::number(m_DlgParaSetStruct[1][8].MaxVal.lmax) + "]");
    //副帧码组位置
    ui->lineEdit_SubFrmSynGroupPos->setText(QString::number(m_DlgParaSetStruct[1][11].InitVal.lval));
    ui->label_SubFrmSynGroupPos->setText("副帧码组位置\n["+QString::number(m_DlgParaSetStruct[1][11].MinVal.lmin) + "~" + QString::number(m_DlgParaSetStruct[1][11].MaxVal.lmax) + "]");
    //副帧同步码组
    ui->lineEdit_SubFrmGroup->setText(QString::number(m_DlgParaSetStruct[1][12].InitVal.lval,16).toUpper());
    //ID计数位置
    ui->lineEdit_idCountPos->setText(QString::number(m_DlgParaSetStruct[1][15].InitVal.lval));
    ui->label_idCountPos->setText("ID计数位置\n["+QString::number(m_DlgParaSetStruct[1][15].MinVal.lmin) + "~" + QString::number(m_DlgParaSetStruct[1][15].MaxVal.lmax) + "]");
}

//显示参数
void DDTBBE_CodFrmSynSetGUIDlg::ShowPara(int type)
{

    if(type == 0)       //参数设置
    {
        DWORD stLen1 = sizeof(MCSDDTRevStatusStruct);
        m_MCSDDTRevStatus = (MCSDDTRevStatusStruct*)m_pStateMgr->FindOneItem(m_StatusParaRecSetParamID, stLen1);
        if(m_MCSDDTRevStatus != NULL)
            m_MCSDDTRevParam = m_MCSDDTRevStatus->tParams;
        DWORD stLen = sizeof(MCSDDTTMIStatusStruct);
        if (m_pStateMgr != NULL)
        {
            m_MCSDDTTMIParamStatus = (MCSDDTTMIStatusStruct*)m_pStateMgr->FindOneItem(m_StatusParaMCSDDTTMIParamID, stLen);
            m_MCSDDTTMIParam = m_MCSDDTTMIParamStatus->tParams;
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

    //码型
    ui->comboBox_CodeType->clear();
    const int size  = m_MCSDDTRevParam.ucGrayType == 9 ? 3 : 1;
    for(int i = 0; i < size; i++)
    {
        ui->comboBox_CodeType->addItem(m_DlgMapMean[0][i]);
    }
    ui->comboBox_CodeType->setCurrentIndex(m_MCSDDTTMIParam.ucCodeStyle - 1);
    //码率
    ui->lineEdit_CodeRate->setText(QString::number(m_MCSDDTTMIParam.usCodeRate / 1000));
    //数据输出方式
    ui->comboBox_OutputType->setCurrentIndex(m_MCSDDTTMIParam.ucBitOutEn - 1);
    const int sizeTmp  = m_MCSDDTTMIParam.ucBitOutEn == 1 ? 2 : 5;

    //译码方式
    ui->comboBox_DecodeType->clear();
    for(int i = 0; i < sizeTmp; i++)
    {
        ui->comboBox_DecodeType->addItem(m_DlgMapMean[3][i]);
    }
    ui->comboBox_DecodeType->setCurrentIndex(m_MCSDDTTMIParam.ucCodingMd - 1);
    //码环带宽
    //ui->comboBox_CodeBn->setCurrentIndex(m_MCSDDTTMIParam.ucCodeBn);
    //译码方式
    if (m_MCSDDTTMIParam.ucCodingMd == 6)
        ui->comboBox_DecodeType->setCurrentIndex(4);
    else
        ui->comboBox_DecodeType->setCurrentIndex(m_MCSDDTTMIParam.ucCodingMd - 1);

    if(ui->comboBox_DecodeType->currentIndex()%2 == 1){
        //卷积码类型
            ui->comboBox_RecurrentCodeType->setCurrentIndex(m_MCSDDTTMIParam.ucViterbiMd - 1);
        //维特比输出相位
            ui->comboBox_ViterbiPhase->setCurrentIndex(m_MCSDDTTMIParam.ucG2Inv - 1);
    }else{
        //卷积码类型
        ui->comboBox_RecurrentCodeType->setCurrentIndex(m_MCSDDTTMIParam.ucViterbiMd == 1 ? 0 : 1);
        //维特比输出相位
        ui->comboBox_ViterbiPhase->setCurrentIndex(m_MCSDDTTMIParam.ucG2Inv == 1 ? 0 : 1);
    }

    //RS码类型
    if(0==m_MCSDDTTMIParam.ucRSMd){
        ui->comboBox_RSType->setCurrentIndex(0);

    }else{
        ui->comboBox_RSType->setCurrentIndex(m_MCSDDTTMIParam.ucRSMd - 1);
    }

    //RS译码坐标基
    if(0 == m_MCSDDTTMIParam.ucBasiSel)
    {
        ui->comboBox_RSDecodePlot->setCurrentIndex(0);
    }
    else
    {
        ui->comboBox_RSDecodePlot->setCurrentIndex(m_MCSDDTTMIParam.ucBasiSel-1);
    }
    //RS译码交织深度
    if(m_MCSDDTTMIParam.ucInterleav == 0)
    {
        ui->comboBox_RSDecodeDeep->setCurrentIndex(0);
    }else{
        ui->comboBox_RSDecodeDeep->setCurrentIndex(m_MCSDDTTMIParam.ucInterleav - 1);
    }

    //帧长
    ui->lineEdit_FrameLen->setText(QString::number(m_MCSDDTTMIParam.usFrameLen));
    //帧同步码组长
    if(0 != m_MCSDDTTMIParam.ucFrameSynGLen)
        ui->comboBox_FrameSyncGroupLen->setCurrentIndex(m_MCSDDTTMIParam.ucFrameSynGLen / 8 - 2);
    //帧同步码组
    ui->lineEdit_FrmSyncGroup->setText(QString::number(m_MCSDDTTMIParam.uiFrameSynG, 16).toUpper());
    //解扰控制
    if(0 != m_MCSDDTTMIParam.ucUnturbCtrl)
        ui->comboBox_RandEn->setCurrentIndex(m_MCSDDTTMIParam.ucUnturbCtrl - 1);
    //解扰多项式长度
    ui->comboBox_RandPolyLen->setCurrentIndex(m_MCSDDTTMIParam.ucRandLen - 1);
    //帧同步码组位置
    if(0 != m_MCSDDTTMIParam.ucFspPlace)
        ui->comboBox_FrmPlace->setCurrentIndex(m_MCSDDTTMIParam.ucFspPlace - 1);
    //解扰初相
    ui->lineEdit_RandPhase->setText(QString::number(m_MCSDDTTMIParam.uiPhase, 16).toUpper());
    //解扰多项式
    ui->lineEdit_RandPoly->setText(QString::number(m_MCSDDTTMIParam.uiRandPoly, 16).toUpper());
    //副帧长
    ui->lineEdit_SubFrmLen->setText(QString::number(m_MCSDDTTMIParam.ucSframeLen));
    //副帧方式
    ui->comboBox_SubFrmSyncType->setCurrentIndex(m_MCSDDTTMIParam.ucSFrameSynMd - 1);
    //副帧同步码组长度
    ui->comboBox_SubFrameGroupLen->setCurrentIndex(m_MCSDDTTMIParam.ucCycFrmSynCodeLen - 1);
    //副帧码组位置
    ui->lineEdit_SubFrmSynGroupPos->setText(QString::number(m_MCSDDTTMIParam.usCycFrmSynCodePos));
    //副帧同步码组
    ui->lineEdit_SubFrmGroup->setText(QString::number(m_MCSDDTTMIParam.uiCycFrmSynCode, 16).toUpper());
    //ID字基值
    if(0 != m_MCSDDTTMIParam.ucIDBase)
        ui->comboBox_IDBaseValue->setCurrentIndex(m_MCSDDTTMIParam.ucIDBase - 1);
    //ID计数方向
    if(0 != m_MCSDDTTMIParam.ucIDDirect)
        ui->comboBox_idDirect->setCurrentIndex(m_MCSDDTTMIParam.ucIDDirect - 1);
    //ID计数位置
    ui->lineEdit_idCountPos->setText(QString::number(m_MCSDDTTMIParam.usIDSubFrmPos));

    //帧同步容错搜索容错位数
    ui->comboBox_FrameSyncFaultTolerant->setCurrentIndex(m_MCSDDTTMIParam.ucFrSynSerLiErr);
    //帧同步容错校核容错位数
    ui->comboBox_FaltToleranceOfFrameSync->setCurrentIndex(m_MCSDDTTMIParam.ucFrSynVerLiErr);
    //帧同步容错锁定容错位数
    ui->comboBox_FrameSyncFaultTolerantLockBit->setCurrentIndex(m_MCSDDTTMIParam.ucFrSynLoLiErr);
    //帧同步容错校核检测帧数
    if (0 != m_MCSDDTTMIParam.ucFrSynVerCheck)
        ui->comboBox_CheckFrameOfFaultFrameSync->setCurrentIndex(m_MCSDDTTMIParam.ucFrSynVerCheck - 1);
    //帧同步容错检测容错帧数
    if (0 != m_MCSDDTTMIParam.ucFrSynLoCheck)
        ui->comboBox_FrameSyncFaultLockDetection->setCurrentIndex(m_MCSDDTTMIParam.ucFrSynLoCheck - 1);
    //副帧同步容错搜索容错位数
    ui->comboBox_SubFrameSyncFaultTolerant->setCurrentIndex(m_MCSDDTTMIParam.ucCFSearchErrBits);
    //副帧同步容错校核容错位数
    ui->comboBox_SubFaltToleranceOfFrameSync->setCurrentIndex(m_MCSDDTTMIParam.ucCFVerifyErrBits);
    //副帧同步容错锁定容错位数
    ui->comboBox_SubFrameSyncFaultTolerantLockBit->setCurrentIndex(m_MCSDDTTMIParam.ucCFLockErrBits);
    //副帧同步容错校核检测帧数
    if (0 != m_MCSDDTTMIParam.ucSubFVerifyFrame)
        ui->comboBox_SubCheckFrameOfFaultFrameSync->setCurrentIndex(m_MCSDDTTMIParam.ucSubFVerifyFrame - 1);
    //副帧同步容错锁定检测帧数
    if (0 != m_MCSDDTTMIParam.ucSubFLockFrame)
        ui->comboBox_SubFrameSyncFaultLockDetection->setCurrentIndex(m_MCSDDTTMIParam.ucSubFLockFrame - 1);

    on_comboBox_OutputType_currentIndexChanged(ui->comboBox_OutputType->currentIndex());
    on_comboBox_RandEn_currentIndexChanged(ui->comboBox_RandEn->currentIndex());
    on_comboBox_SubFrmSyncType_currentIndexChanged(ui->comboBox_SubFrmSyncType->currentIndex());
    on_comboBox_DecodeType_currentIndexChanged(ui->comboBox_DecodeType->currentIndex());
    on_comboBox_RecurrentCodeType_currentIndexChanged(ui->comboBox_RecurrentCodeType->currentIndex());

}

//设置帧长范围
void DDTBBE_CodFrmSynSetGUIDlg::SetFrameLenRange(int iWordLen)
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

void DDTBBE_CodFrmSynSetGUIDlg::SetObjInfo(TLxTsspObjectInfo ObjectInfo)
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
        SUBITEM& sItem = *(pItem->getSubItemByDeviceID_SN(m_ObjectInfo.usLocalDID, 1));
        m_usC = sItem.usCMD;
        m_usTID = sItem.usTID;
        m_ucSN = sItem.ucSN;
        /*************************框架**********************************/
        m_ParaMCSDDTTMIParamID = GetID(sItem.usStationID, sItem.ucSubSysID, sItem.usDeviceID,
                                       sItem.usTID, sItem.ucSN);
        /*************************框架**********************************/

        /*************************状态**********************************/
        m_StatusParaMCSDDTTMIParamID = GetID(sItem.usStationID, sItem.ucSubSysID, sItem.usDeviceID,
                                             sItem.usStateType, sItem.ucSN);
        /*************************状态**********************************/
    }

    ITEM *pItem1 = m_DeviceMap.getItem("数传接收机参数设置");
    if (pItem1->vSubItem.size() > 0)
    {
        //SUBITEM sItem1 = pItem1->vSubItem.at(0);
        SUBITEM& sItem1 = *(pItem1->getSubItemByDeviceID_SN(m_ObjectInfo.usLocalDID, 0));
        m_usC1 = sItem1.usCMD;
        m_usTID1 = sItem1.usTID;
        /*************************框架**********************************/
        m_ParaRecSetParamID = GetID(sItem1.usStationID, sItem1.ucSubSysID, sItem1.usDeviceID,
                                    sItem1.usTID, sItem1.ucSN);
        /*************************框架**********************************/

        /*************************状态**********************************/
        m_StatusParaRecSetParamID = GetID(sItem1.usStationID, sItem1.ucSubSysID, sItem1.usDeviceID,
                                          sItem1.usStateType, sItem1.ucSN);
        /*************************状态**********************************/
    }

    //日志
    m_pILxTsspLogManager = pObjectManager->GetLogManager();
    m_pStateMgr = pObjectManager->GetStateManager();
    m_pILxTsspMessageManager = pObjectManager->GetMessageManager();
    m_pPublicInfo = pObjectManager->GetPublicInfoManager();
}

//使能或不使能控件
void DDTBBE_CodFrmSynSetGUIDlg::EnabledControls(bool bFlag)
{
    /*************************Group1*************************/
    //数据输出方式
    ui->comboBox_OutputType->setEnabled(bFlag);
    //码型
    ui->comboBox_CodeType->setEnabled(bFlag);
    //译码方式
    ui->comboBox_DecodeType->setEnabled(bFlag);
    //码速率
    ui->lineEdit_CodeRate->setEnabled(bFlag);
    //卷积码类型
    ui->comboBox_RecurrentCodeType->setEnabled(bFlag);
    //RS码类型
    ui->comboBox_RSType->setEnabled(bFlag);
    //RS码坐标
    ui->comboBox_RSDecodePlot->setEnabled(bFlag);
    //RS译码交织深度
    ui->comboBox_RSDecodeDeep->setEnabled(bFlag);
    //维特比译码输出相位
    ui->comboBox_ViterbiPhase->setEnabled(false);
    //    //码环带宽
    //    ui->comboBox_CodeBn->setEnabled(bFlag);
    /*************************Group1*************************/

    /*************************Group2*************************/
    //帧长
    ui->lineEdit_FrameLen->setEnabled(bFlag);
    //帧同步码组长
    ui->comboBox_FrameSyncGroupLen->setEnabled(bFlag);
    //帧同步码组
    ui->lineEdit_FrmSyncGroup->setEnabled(bFlag);
    //帧同步码组位置
    ui->comboBox_FrmPlace->setEnabled(bFlag);
    //解扰使能
    ui->comboBox_RandEn->setEnabled(bFlag);
    //解扰多项式长
    ui->comboBox_RandPolyLen->setEnabled(bFlag);
    //解扰多项式
    ui->lineEdit_RandPoly->setEnabled(bFlag);
    //解扰初相
    ui->lineEdit_RandPhase->setEnabled(bFlag);
    //副帧长
    ui->lineEdit_SubFrmLen->setEnabled(bFlag);
    //副帧码组长
    ui->comboBox_SubFrameGroupLen->setEnabled(bFlag);
    //副帧码组
    ui->lineEdit_SubFrmGroup->setEnabled(bFlag);
    //副帧同步方式
    ui->comboBox_SubFrmSyncType->setEnabled(bFlag);
    //副帧码组位置
    ui->lineEdit_SubFrmSynGroupPos->setEnabled(bFlag);
    //ID字基值
    ui->comboBox_IDBaseValue->setEnabled(bFlag);
    //ID计数方向
    ui->comboBox_idDirect->setEnabled(bFlag);
    //ID计数位置
    ui->lineEdit_idCountPos->setEnabled(bFlag);

    /*************************Group2*************************/

    /*************************Group3*************************/
    //帧同步容错搜索容错位数
    ui->comboBox_FrameSyncFaultTolerant->setEnabled(bFlag);
    //帧同步容错校核容错位数
    ui->comboBox_FaltToleranceOfFrameSync->setEnabled(bFlag);
    //帧同步容错锁定容错位数
    ui->comboBox_FrameSyncFaultTolerantLockBit->setEnabled(bFlag);
    //帧同步容错校核检测帧数
    ui->comboBox_CheckFrameOfFaultFrameSync->setEnabled(bFlag);
    //帧同步容错锁定检测帧数
    ui->comboBox_FrameSyncFaultLockDetection->setEnabled(bFlag);
    //副帧同步容错搜索容错位数
    ui->comboBox_SubFrameSyncFaultTolerant->setEnabled(bFlag);
    //副帧同步容错校核容错位数
    ui->comboBox_SubFaltToleranceOfFrameSync->setEnabled(bFlag);
    //副帧同步容错锁定容错位数
    ui->comboBox_SubFrameSyncFaultTolerantLockBit->setEnabled(bFlag);
    //副帧同步容错校核检测帧数
    ui->comboBox_SubCheckFrameOfFaultFrameSync->setEnabled(bFlag);
    //副帧同步容错锁定检测帧数
    ui->comboBox_SubFrameSyncFaultLockDetection->setEnabled(bFlag);
    /*************************Group3*************************/

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
int DDTBBE_CodFrmSynSetGUIDlg::SaveToMacro(ILxTsspParameterMacro* pMacroObj)
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
        byteArray.resize(sizeof(m_MCSDDTTMIParam));
        memcpy(byteArray.data(),&m_MCSDDTTMIParam,sizeof(m_MCSDDTTMIParam));
        pMacroObj->UpdateParameterBlock(m_ParaMCSDDTTMIParamID,byteArray);

        //日志信息
        TLxTsspLog tmpLog;
        QString strLog;
        memset(&tmpLog, 0, sizeof(TLxTsspLog));
        tmpLog.ucWarningLevel  = 1;
        strLog += "数传解调参数设置: 保存宏成功！";
        strcpy(tmpLog.szContent, strLog.toUtf8().data());
        tmpLog.time = QDateTime::currentDateTime().toTime_t();
        CLog::addLog(strLog);
        //m_pILxTsspLogManager->Add(tmpLog);
        InitDLG(1, pMacroObj);
    }
}
//从界面取参数
void DDTBBE_CodFrmSynSetGUIDlg::GetPara()
{
    /***********************************G1****************************/
    //码型
    m_MCSDDTTMIParam.ucCodeStyle = ui->comboBox_CodeType->currentIndex() + 1;
    //码环带宽
    //    m_MCSDDTTMIParam.usCodeBn = ui->comboBox_CodeBn->currentIndex();
    //码率
    m_MCSDDTTMIParam.usCodeRate = ui->lineEdit_CodeRate->text().toInt() * 1000;
    //数据输出方式
    m_MCSDDTTMIParam.ucBitOutEn = ui->comboBox_OutputType->currentIndex() + 1;
    //译码方式
    if (ui->comboBox_DecodeType->currentIndex() == 4)
        m_MCSDDTTMIParam.ucCodingMd = 6;
    else
        m_MCSDDTTMIParam.ucCodingMd = ui->comboBox_DecodeType->currentIndex() + 1;
    //LDPC码率
    m_MCSDDTTMIParam.ucLDPCRatio = 4; //添加LDPC码率  20170604
    //卷积码类型
    m_MCSDDTTMIParam.ucViterbiMd = ui->comboBox_RecurrentCodeType->currentIndex() + 1;
    //RS码类型
    m_MCSDDTTMIParam.ucRSMd = ui->comboBox_RSType->currentIndex() + 1;
    //RS译码坐标
    m_MCSDDTTMIParam.ucBasiSel = ui->comboBox_RSDecodePlot->currentIndex() + 1;
    //RS交织深度
    m_MCSDDTTMIParam.ucInterleav = ui->comboBox_RSDecodeDeep->currentIndex() + 1;
    //维特比译码输出相位
    m_MCSDDTTMIParam.ucG2Inv = ui->comboBox_ViterbiPhase->currentIndex()+1;
    /***********************************G1****************************/

    /***********************************G2****************************/
    bool ok;
    //帧长
    m_MCSDDTTMIParam.usFrameLen = ui->lineEdit_FrameLen->text().toInt();
    //帧同步码组长
    m_MCSDDTTMIParam.ucFrameSynGLen = (ui->comboBox_FrameSyncGroupLen->currentIndex() +2)*8 ;
    //帧同步码组
    char* pTem1 = ui->lineEdit_FrmSyncGroup->text().trimmed().toLocal8Bit().data();
    m_MCSDDTTMIParam.uiFrameSynG = StrToSynCode(pTem1);
    //帧同步码组位置
    m_MCSDDTTMIParam.ucFspPlace = ui->comboBox_FrmPlace->currentIndex() + 1;
    //解扰使能
    m_MCSDDTTMIParam.ucUnturbCtrl = ui->comboBox_RandEn->currentIndex() + 1;
    //解扰多项式长
    m_MCSDDTTMIParam.ucRandLen = ui->comboBox_RandPolyLen->currentIndex() + 1;
    //解扰多项式
    m_MCSDDTTMIParam.uiRandPoly = ui->lineEdit_RandPoly->text().toUInt(&ok,16);
    //解扰初相
    m_MCSDDTTMIParam.uiPhase = ui->lineEdit_RandPhase->text().toUInt(&ok,16);
    //副帧长
    m_MCSDDTTMIParam.ucSframeLen = ui->lineEdit_SubFrmLen->text().toInt();
    //副帧码组长
    m_MCSDDTTMIParam.ucCycFrmSynCodeLen = ui->comboBox_SubFrameGroupLen->currentIndex() + 1;
    //副帧码组
    char* pTem2 = ui->lineEdit_SubFrmGroup->text().trimmed().toLocal8Bit().data();
    m_MCSDDTTMIParam.uiCycFrmSynCode = StrToSynCode(pTem2);
    //副帧同步方式
    m_MCSDDTTMIParam.ucSFrameSynMd = ui->comboBox_SubFrmSyncType->currentIndex() + 1;
    //副帧码组位置
    m_MCSDDTTMIParam.usCycFrmSynCodePos = ui->lineEdit_SubFrmSynGroupPos->text().toInt();
    //ID字基值
    m_MCSDDTTMIParam.ucIDBase = ui->comboBox_IDBaseValue->currentIndex() + 1;
    //ID计数方向
    m_MCSDDTTMIParam.ucIDDirect = ui->comboBox_idDirect->currentIndex() + 1;
    //ID计数位置
    m_MCSDDTTMIParam.usIDSubFrmPos = ui->lineEdit_idCountPos->text().toInt();
    /***********************************G2****************************/

    /********************************G3*********************************/
    //帧同步容错搜索容错位数
    m_MCSDDTTMIParam.ucFrSynSerLiErr = ui->comboBox_FrameSyncFaultTolerant->currentIndex();
    //帧同步容错校核容错位数
    m_MCSDDTTMIParam.ucFrSynVerLiErr = ui->comboBox_FaltToleranceOfFrameSync->currentIndex();
    //帧同步容错锁定容错位数
    m_MCSDDTTMIParam.ucFrSynLoLiErr = ui->comboBox_FrameSyncFaultTolerantLockBit->currentIndex();
    //帧同步容错校核检测帧数
    m_MCSDDTTMIParam.ucFrSynVerCheck = ui->comboBox_CheckFrameOfFaultFrameSync->currentIndex() + 1;
    //帧同步容错锁定检测帧数
    m_MCSDDTTMIParam.ucFrSynLoCheck = ui->comboBox_FrameSyncFaultLockDetection->currentIndex() +1;
    //副帧同步容错搜索容错位数
    m_MCSDDTTMIParam.ucCFSearchErrBits = ui->comboBox_SubFrameSyncFaultTolerant->currentIndex();
    //副帧同步容错校核容错位数
    m_MCSDDTTMIParam.ucCFVerifyErrBits = ui->comboBox_SubFaltToleranceOfFrameSync->currentIndex();
    //副帧同步容错锁定容错位数
    m_MCSDDTTMIParam.ucCFLockErrBits = ui->comboBox_SubFrameSyncFaultTolerantLockBit->currentIndex();
    //副帧同步容错校核检测帧数
    m_MCSDDTTMIParam.ucSubFVerifyFrame = ui->comboBox_SubCheckFrameOfFaultFrameSync->currentIndex() + 1;
    //副帧同步容错锁定检测帧数
    m_MCSDDTTMIParam.ucSubFLockFrame = ui->comboBox_SubFrameSyncFaultLockDetection->currentIndex() +1;
    /********************************G3*********************************/
}

//验证参数范围
int DDTBBE_CodFrmSynSetGUIDlg::CheckPara()
{
    return CheckDecodeType() == true ? 1 : -1;
}

//定时器
void DDTBBE_CodFrmSynSetGUIDlg::timerEvent(QTimerEvent *event)
{
    ShowPara(0);
}

//更改参数按钮休息处理函数
void DDTBBE_CodFrmSynSetGUIDlg::on_pushButton_Modify_clicked()
{
    if(ui->pushButton_Modify->text() == "更改")
    {
        m_killTimer = true;
        killTimer(m_TimerID);
        EnabledControls(true);
        on_comboBox_OutputType_currentIndexChanged(ui->comboBox_OutputType->currentIndex());
        on_comboBox_RandEn_currentIndexChanged(ui->comboBox_RandEn->currentIndex());
        on_comboBox_SubFrmSyncType_currentIndexChanged(ui->comboBox_SubFrmSyncType->currentIndex());
        on_comboBox_DecodeType_currentIndexChanged(ui->comboBox_DecodeType->currentIndex());
        on_comboBox_RecurrentCodeType_currentIndexChanged(ui->comboBox_RecurrentCodeType->currentIndex());


        return;
    }

    if(ui->pushButton_Modify->text() == "恢复")
    {
        EnabledControls(false);
        m_TimerID = startTimer(500);
        return;
    }
}

void DDTBBE_CodFrmSynSetGUIDlg::on_pushButton_Set_clicked()
{
    //m_killTimer = false;
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
            byteArray.resize(sizeof(m_MCSDDTTMIParam));
            SendMessage((char*)&m_MCSDDTTMIParam, sizeof(MCSDDTTMIParamStruct), m_usC, m_usTID, m_ucSN);
        }

        //日志信息
        TLxTsspLog tmpLog;
        QString strLog;
        memset(&tmpLog, 0, sizeof(TLxTsspLog));
        tmpLog.ucWarningLevel  = 1;
        strLog += "解调参数设置: 设置参数成功！";
        strcpy(tmpLog.szContent, strLog.toUtf8().data());
        tmpLog.time = QDateTime::currentDateTime().toTime_t();

        m_pILxTsspLogManager->Add(tmpLog);

        //不使能控件
        EnabledControls(false);
        m_TimerID = startTimer(500);
    }
}

void DDTBBE_CodFrmSynSetGUIDlg::closeEvent(QCloseEvent *event)
{
    event->accept();
}

QSize DDTBBE_CodFrmSynSetGUIDlg::sizeHint() const
{
    return QSize(this->width(), this->height());
}

/*
//副帧方式响应函数
void DDTBBE_CodFrmSynSetGUIDlg::on_comboBox_SubFrmType_currentIndexChanged(int index)
{
//    //副帧方式控件使能
//    if (ui->pushButton_Modify->text() != "更改" || iParaSetFlag != 0)
//    {
//        EnabledControlsSubFrmType(index);   //副帧方式控件使能
//    }
}

//根据副帧类型，使能副帧控件
void DDTBBE_CodFrmSynSetGUIDlg::EnabledControlsSubFrmType(int iSubFrm)
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

void DDTBBE_CodFrmSynSetGUIDlg::SubFrmEn(bool bFlag)
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
void DDTBBE_CodFrmSynSetGUIDlg::IDSubFrmEn(bool bFlag)
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
void DDTBBE_CodFrmSynSetGUIDlg::CycSubFrmEn(bool bFlag)
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
void DDTBBE_CodFrmSynSetGUIDlg::ReserveSubFrmEn(bool bFlag)
{

}

void DDTBBE_CodFrmSynSetGUIDlg::SendMessage(char* pData, USHORT usLen, USHORT usC, USHORT usTID, USHORT usSN)
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
    header.C = 0x200;

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

DWORDLONG DDTBBE_CodFrmSynSetGUIDlg::StrToSynCode(const char * pStr)
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
//数据输出方式的监听
void DDTBBE_CodFrmSynSetGUIDlg::on_comboBox_OutputType_currentIndexChanged(int index)
{
    if (ui->pushButton_Modify->text() == "更改")
        return;

    //第一部分*
    ui->comboBox_DecodeType->setEnabled(false);
    ui->comboBox_RecurrentCodeType->setEnabled(false);
    ui->comboBox_RSType->setEnabled(false);
    ui->comboBox_RSDecodeDeep->setEnabled(false);
    ui->comboBox_ViterbiPhase->setEnabled(false);
    ui->comboBox_RSDecodePlot->setEnabled(false);
    //第二部分
    ui->lineEdit_FrameLen->setEnabled(false);
    ui->comboBox_FrameSyncGroupLen->setEnabled(false);
    ui->lineEdit_FrmSyncGroup->setEnabled(false);
    ui->comboBox_RandEn->setEnabled(false);
    ui->comboBox_RandPolyLen->setEnabled(false);
    ui->comboBox_FrmPlace->setEnabled(false);
    ui->lineEdit_RandPoly->setEnabled(false);
    ui->lineEdit_RandPhase->setEnabled(false);
    //第三部分

    ui->lineEdit_SubFrmLen->setEnabled(false);
    ui->comboBox_SubFrmSyncType->setEnabled(false);
    ui->comboBox_SubFrameGroupLen->setEnabled(false);
    ui->lineEdit_SubFrmSynGroupPos->setEnabled(false);
    ui->lineEdit_SubFrmGroup->setEnabled(false);
    ui->comboBox_IDBaseValue->setEnabled(false);
    ui->comboBox_idDirect->setEnabled(false);
    ui->lineEdit_idCountPos->setEnabled(false);
    //第四部分
    ui->comboBox_FrameSyncFaultTolerant->setEnabled(false);
    ui->comboBox_FaltToleranceOfFrameSync->setEnabled(false);
    ui->comboBox_FrameSyncFaultTolerantLockBit->setEnabled(false);
    ui->comboBox_CheckFrameOfFaultFrameSync->setEnabled(false);
    ui->comboBox_FrameSyncFaultLockDetection->setEnabled(false);

    ui->comboBox_SubFrameSyncFaultTolerant->setEnabled(false);
    ui->comboBox_SubFaltToleranceOfFrameSync->setEnabled(false);
    ui->comboBox_SubFrameSyncFaultTolerantLockBit->setEnabled(false);
    ui->comboBox_SubCheckFrameOfFaultFrameSync->setEnabled(false);
    ui->comboBox_SubFrameSyncFaultLockDetection->setEnabled(false);
    const int sizeTmp  = index == 1 ? 5 : 2;
    //译码方式
    ui->comboBox_DecodeType->clear();
    if(index == 0){//位流
        for(int i = 0; i < sizeTmp;i++)
        {
            ui->comboBox_DecodeType->addItem(m_DlgMapMean[3][i]);
        }

        if(m_MCSDDTTMIParam.ucCodingMd > 2)
        {
            ui->comboBox_DecodeType->setCurrentIndex(0);
        }
        else
        {
            ui->comboBox_DecodeType->setCurrentIndex(m_MCSDDTTMIParam.ucCodingMd - 1);
        }
        ui->comboBox_DecodeType->setEnabled(true);
        on_comboBox_DecodeType_currentIndexChanged(ui->comboBox_DecodeType->currentIndex());

    }
    if(index == 1){//帧格式
        for(int i=0; i < sizeTmp;i++)
        {
            ui->comboBox_DecodeType->addItem(m_DlgMapMean[3][i]);
        }
        if (m_MCSDDTTMIParam.ucCodingMd == 6)
            ui->comboBox_DecodeType->setCurrentIndex(4);
        else
            ui->comboBox_DecodeType->setCurrentIndex(m_MCSDDTTMIParam.ucCodingMd - 1);
        ui->comboBox_DecodeType->setEnabled(true);
        on_comboBox_DecodeType_currentIndexChanged(ui->comboBox_DecodeType->currentIndex());
        ui->comboBox_FrameSyncFaultTolerant->setEnabled(true);
        ui->comboBox_FaltToleranceOfFrameSync->setEnabled(true);
        ui->comboBox_FrameSyncFaultTolerantLockBit->setEnabled(true);
        ui->comboBox_CheckFrameOfFaultFrameSync->setEnabled(true);
        ui->comboBox_FrameSyncFaultLockDetection->setEnabled(true);
        on_comboBox_RandEn_currentIndexChanged(ui->comboBox_RandEn->currentIndex());
    }
}
//译码方式的监听
void DDTBBE_CodFrmSynSetGUIDlg::on_comboBox_DecodeType_currentIndexChanged(int index)
{
    if (ui->pushButton_Modify->text() == "更改")
        return;


    if (index == 0){
        if(ui->comboBox_OutputType->currentIndex()==0){//位流
            ui->comboBox_RecurrentCodeType->setEnabled(false);
            ui->comboBox_RSType->setEnabled(false);
            ui->comboBox_RSDecodeDeep->setEnabled(false);
            ui->comboBox_ViterbiPhase->setEnabled(false);
            ui->comboBox_RSDecodePlot->setEnabled(false);
            //ui->comboBox_RecurrentCodeType->setCurrentIndex(0);
            ui->comboBox_ViterbiPhase->setCurrentIndex(ui->comboBox_ViterbiPhase->findText("正相"));


        }
        if(ui->comboBox_OutputType->currentIndex() == 1){//帧
            ui->comboBox_RecurrentCodeType->setEnabled(false);
            ui->comboBox_RSType->setEnabled(false);
            ui->comboBox_RSDecodeDeep->setEnabled(false);
            ui->comboBox_RSDecodePlot->setEnabled(false);
            //ui->comboBox_RecurrentCodeType->setCurrentIndex(0);
            ui->comboBox_ViterbiPhase->setCurrentIndex(ui->comboBox_ViterbiPhase->findText("正相"));
            ui->comboBox_ViterbiPhase->setEnabled(false);

            //第二部分
            ui->lineEdit_FrameLen->setEnabled(true);
            ui->comboBox_FrameSyncGroupLen->setEnabled(true);
            ui->lineEdit_FrmSyncGroup->setEnabled(true);
            ui->comboBox_FrmPlace->setEnabled(true);
            ui->comboBox_RandEn->setEnabled(true);
            //第三部分
            ui->comboBox_SubFrmSyncType->setEnabled(true);
        }
    }
    //Viterbi
    if (index == 1){

        if(ui->comboBox_OutputType->currentIndex() == 0){
            ui->comboBox_RecurrentCodeType->setEnabled(true);
            on_comboBox_RecurrentCodeType_currentIndexChanged(ui->comboBox_RecurrentCodeType->currentIndex());
        }
        if(ui->comboBox_OutputType->currentIndex() == 1){
            //第一部分
            ui->comboBox_RecurrentCodeType->setEnabled(true);
            ui->comboBox_RSType->setEnabled(false);
            ui->comboBox_RSDecodeDeep->setEnabled(false);
            ui->comboBox_RSDecodePlot->setEnabled(false);
            on_comboBox_RecurrentCodeType_currentIndexChanged(ui->comboBox_RecurrentCodeType->currentIndex());
            //第二部分
            ui->lineEdit_FrameLen->setEnabled(true);
            ui->comboBox_FrameSyncGroupLen->setEnabled(true);
            ui->lineEdit_FrmSyncGroup->setEnabled(true);
            ui->comboBox_FrmPlace->setEnabled(true);
            ui->comboBox_RandEn->setEnabled(true);
            //第三部分
            ui->comboBox_SubFrmSyncType->setEnabled(true);
            //第四部分
        }

    }
    //R-S译码
    if(index == 2){
        //第一部分
        ui->comboBox_RecurrentCodeType->setEnabled(false);
        ui->comboBox_ViterbiPhase->setEnabled(false);
        ui->comboBox_RSType->setEnabled(true);
        ui->comboBox_RSDecodeDeep->setEnabled(true);
        ui->comboBox_RSDecodePlot->setEnabled(true);

        //第二部分
        ui->lineEdit_FrameLen->setEnabled(true);
        ui->comboBox_FrameSyncGroupLen->setEnabled(true);
        ui->lineEdit_FrmSyncGroup->setEnabled(true);
        ui->comboBox_FrmPlace->setEnabled(false);
        ui->comboBox_FrmPlace->setCurrentIndex(0);
        ui->comboBox_RandEn->setEnabled(true);
        //第三部分
        ui->lineEdit_SubFrmLen->setEnabled(false);
        ui->comboBox_SubFrmSyncType->setEnabled(false);
        ui->comboBox_SubFrameGroupLen->setEnabled(false);
        ui->lineEdit_SubFrmSynGroupPos->setEnabled(false);
        ui->lineEdit_SubFrmGroup->setEnabled(false);
        ui->comboBox_IDBaseValue->setEnabled(false);
        ui->comboBox_idDirect->setEnabled(false);
        ui->lineEdit_idCountPos->setEnabled(false);
        //第四部分
        ui->comboBox_SubFrmSyncType->setCurrentIndex(ui->comboBox_SubFrmSyncType->findText("无副帧"));
    }
    //R-S和Viterbi级联译码
    if(index == 3){
        //第一部分
        ui->comboBox_RecurrentCodeType->setEnabled(true);
        ui->comboBox_ViterbiPhase->setEnabled(true);
        ui->comboBox_RSType->setEnabled(true);
        ui->comboBox_RSDecodeDeep->setEnabled(true);
        ui->comboBox_RSDecodePlot->setEnabled(true);

        //第二部分
        ui->lineEdit_FrameLen->setEnabled(true);
        ui->comboBox_FrameSyncGroupLen->setEnabled(true);
        ui->lineEdit_FrmSyncGroup->setEnabled(true);
        ui->comboBox_FrmPlace->setEnabled(false);
        ui->comboBox_FrmPlace->setCurrentIndex(0);
        ui->comboBox_RandEn->setEnabled(true);
        //第三部分
        ui->lineEdit_SubFrmLen->setEnabled(false);
        ui->comboBox_SubFrmSyncType->setEnabled(false);
        ui->comboBox_SubFrameGroupLen->setEnabled(false);
        ui->lineEdit_SubFrmSynGroupPos->setEnabled(false);
        ui->lineEdit_SubFrmGroup->setEnabled(false);
        ui->comboBox_IDBaseValue->setEnabled(false);
        ui->comboBox_idDirect->setEnabled(false);
        ui->lineEdit_idCountPos->setEnabled(false);
        //第四部分
        ui->comboBox_SubFrmSyncType->setCurrentIndex(ui->comboBox_SubFrmSyncType->findText("无副帧"));
        on_comboBox_RecurrentCodeType_currentIndexChanged(ui->comboBox_RecurrentCodeType->currentIndex());

    }
    //LDPC译码  20170604添加
    if(index == 4){
        //第一部分
        ui->comboBox_RecurrentCodeType->setEnabled(false);
        ui->comboBox_ViterbiPhase->setEnabled(false);
        ui->comboBox_RSType->setEnabled(false);
        ui->comboBox_RSDecodeDeep->setEnabled(false);
        ui->comboBox_RSDecodePlot->setEnabled(false);

        //第二部分
        ui->lineEdit_FrameLen->setEnabled(false);
        ui->lineEdit_FrameLen->setText("1024");
        ui->comboBox_FrameSyncGroupLen->setEnabled(false);
        ui->comboBox_FrameSyncGroupLen->setCurrentIndex(2);
        ui->lineEdit_FrmSyncGroup->setEnabled(true);
        ui->comboBox_FrmPlace->setEnabled(false);
        ui->comboBox_FrmPlace->setCurrentIndex(0);
        ui->comboBox_RandEn->setEnabled(true);
        on_comboBox_RandEn_currentIndexChanged(ui->comboBox_RandEn->currentIndex());
        //第三部分
        ui->lineEdit_SubFrmLen->setEnabled(false);
        ui->comboBox_SubFrmSyncType->setEnabled(false);
        ui->comboBox_SubFrameGroupLen->setEnabled(false);
        ui->lineEdit_SubFrmSynGroupPos->setEnabled(false);
        ui->lineEdit_SubFrmGroup->setEnabled(false);
        ui->comboBox_IDBaseValue->setEnabled(false);
        ui->comboBox_idDirect->setEnabled(false);
        ui->lineEdit_idCountPos->setEnabled(false);
        //第四部分
        ui->comboBox_SubFrmSyncType->setEnabled(false);
        ui->comboBox_SubFrmSyncType->setCurrentIndex(ui->comboBox_SubFrmSyncType->findText("无副帧"));
        on_comboBox_SubFrmSyncType_currentIndexChanged(ui->comboBox_SubFrmSyncType->currentIndex());

    }
    on_comboBox_RecurrentCodeType_currentIndexChanged(ui->comboBox_RecurrentCodeType->currentIndex());
}
//卷积码类型的监听
void DDTBBE_CodFrmSynSetGUIDlg::on_comboBox_RecurrentCodeType_currentIndexChanged(int index)
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
    if(index == 1){
        ui->comboBox_ViterbiPhase->setEnabled(false);
        ui->comboBox_ViterbiPhase->setCurrentIndex(ui->comboBox_ViterbiPhase->findText("正相"));

    }
}
//解扰使能监听
void DDTBBE_CodFrmSynSetGUIDlg::on_comboBox_RandEn_currentIndexChanged(int index)
{
    if (ui->pushButton_Modify->text() == "更改")
        return;
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
//副帧的同步方式
void DDTBBE_CodFrmSynSetGUIDlg::on_comboBox_SubFrmSyncType_currentIndexChanged(int index)
{
    if (ui->pushButton_Modify->text() == "更改")
        return;
    //第三部分
    ui->lineEdit_SubFrmLen->setEnabled(false);
    ui->comboBox_SubFrameGroupLen->setEnabled(false);
    ui->lineEdit_SubFrmSynGroupPos->setEnabled(false);
    ui->lineEdit_SubFrmGroup->setEnabled(false);
    ui->comboBox_IDBaseValue->setEnabled(false);
    ui->comboBox_idDirect->setEnabled(false);
    ui->lineEdit_idCountPos->setEnabled(false);
    //第四部分
    ui->comboBox_FrameSyncFaultTolerant->setEnabled(false);
    ui->comboBox_FaltToleranceOfFrameSync->setEnabled(false);
    ui->comboBox_FrameSyncFaultTolerantLockBit->setEnabled(false);
    ui->comboBox_CheckFrameOfFaultFrameSync->setEnabled(false);
    ui->comboBox_FrameSyncFaultLockDetection->setEnabled(false);

    ui->comboBox_SubFrameSyncFaultTolerant->setEnabled(false);
    ui->comboBox_SubFaltToleranceOfFrameSync->setEnabled(false);
    ui->comboBox_SubFrameSyncFaultTolerantLockBit->setEnabled(false);
    ui->comboBox_SubCheckFrameOfFaultFrameSync->setEnabled(false);
    ui->comboBox_SubFrameSyncFaultLockDetection->setEnabled(false);
    if(index==0){//无副帧
        ui->comboBox_FrameSyncFaultTolerant->setEnabled(true);
        ui->comboBox_FaltToleranceOfFrameSync->setEnabled(true);
        ui->comboBox_FrameSyncFaultTolerantLockBit->setEnabled(true);
        ui->comboBox_CheckFrameOfFaultFrameSync->setEnabled(true);
        ui->comboBox_FrameSyncFaultLockDetection->setEnabled(true);
        ui->lineEdit_SubFrmLen->setEnabled(false);
    }
    if(index==2){//循环副帧
        ui->lineEdit_SubFrmLen->setEnabled(true);
        ui->lineEdit_SubFrmSynGroupPos->setEnabled(true);
        ui->comboBox_SubFrameGroupLen->setEnabled(true);
        ui->lineEdit_SubFrmGroup->setEnabled(true);

        ui->comboBox_FrameSyncFaultTolerant->setEnabled(true);
        ui->comboBox_FaltToleranceOfFrameSync->setEnabled(true);
        ui->comboBox_FrameSyncFaultTolerantLockBit->setEnabled(true);
        ui->comboBox_CheckFrameOfFaultFrameSync->setEnabled(true);
        ui->comboBox_FrameSyncFaultLockDetection->setEnabled(true);

        ui->comboBox_SubFrameSyncFaultTolerant->setEnabled(true);
        ui->comboBox_SubFaltToleranceOfFrameSync->setEnabled(true);
        ui->comboBox_SubFrameSyncFaultTolerantLockBit->setEnabled(true);
        ui->comboBox_SubCheckFrameOfFaultFrameSync->setEnabled(true);
        ui->comboBox_SubFrameSyncFaultLockDetection->setEnabled(true);
    }
    if(index==1){//ID副帧
        ui->lineEdit_SubFrmLen->setEnabled(true);
        ui->comboBox_IDBaseValue->setEnabled(true);
        ui->comboBox_idDirect->setEnabled(true);
        ui->lineEdit_idCountPos->setEnabled(true);

        ui->comboBox_FrameSyncFaultTolerant->setEnabled(true);
        ui->comboBox_FaltToleranceOfFrameSync->setEnabled(true);
        ui->comboBox_FrameSyncFaultTolerantLockBit->setEnabled(true);
        ui->comboBox_CheckFrameOfFaultFrameSync->setEnabled(true);
        ui->comboBox_FrameSyncFaultLockDetection->setEnabled(true);

        ui->comboBox_SubFrameSyncFaultLockDetection->setEnabled(true);
        ui->comboBox_SubCheckFrameOfFaultFrameSync->setEnabled(true);
    }
    if(index == 3){//反码副帧
        ui->lineEdit_SubFrmLen->setEnabled(true);
        ui->comboBox_FrameSyncFaultTolerant->setEnabled(true);
        ui->comboBox_FaltToleranceOfFrameSync->setEnabled(true);
        ui->comboBox_FrameSyncFaultTolerantLockBit->setEnabled(true);
        ui->comboBox_CheckFrameOfFaultFrameSync->setEnabled(true);
        ui->comboBox_FrameSyncFaultLockDetection->setEnabled(true);

        ui->comboBox_SubFrameSyncFaultTolerant->setEnabled(true);
        ui->comboBox_SubFaltToleranceOfFrameSync->setEnabled(true);
        ui->comboBox_SubFrameSyncFaultTolerantLockBit->setEnabled(true);
        ui->comboBox_SubCheckFrameOfFaultFrameSync->setEnabled(true);
        ui->comboBox_SubFrameSyncFaultLockDetection->setEnabled(true);
    }
}

BOOL DDTBBE_CodFrmSynSetGUIDlg::CheckDecodeType()
{
    QMessageBox msgBox;
    msgBox.setWindowTitle(MODULENAME);
    QString strTmp;
    USHORT FrmSizeMin = 4;
    USHORT FrmSizeMax = 4096;
    int m_usFrameLen = ui->lineEdit_FrameLen->text().toInt();
    int nFSPLen = (ui->comboBox_FrameSyncGroupLen->currentIndex() + 2) * 8;//comboBox_FrameSyncGroupLen
    int m_nDecodeType = ui->comboBox_DecodeType->currentIndex();
    if((ui->comboBox_DecodeType->currentIndex() == 2) || (ui->comboBox_DecodeType->currentIndex() == 3))
    {
        int k;
        if(ui->comboBox_RSType->currentIndex() == 0)
        {
            k = 223;
        }
        else
        {
            k = 239;
        }
        int m_ucInterleaver = ui->comboBox_RSDecodeDeep->currentText().toInt();
        int m_ucWordLen = 8;
        int nMaxFrmLen = 255 * m_ucInterleaver + nFSPLen/m_ucWordLen;
        int nMinFrmLen = nMaxFrmLen - (k-1) * m_ucInterleaver;
        if((m_usFrameLen > nMaxFrmLen) || (m_usFrameLen < nMinFrmLen))
        {
            strTmp = QString("此时帧长应为[%1~%2]字，请重新输入!(总帧长 - 子帧长 = 交错深度*N)").arg(nMinFrmLen).arg(nMaxFrmLen);
            msgBox.setText(strTmp);
            msgBox.exec();
            return FALSE;
        }
        else if(m_usFrameLen < nMaxFrmLen)
        {
            int nVFLen = nMaxFrmLen - m_usFrameLen;
            if(nVFLen % m_ucInterleaver != 0)
            {
                strTmp = QString("帧长，交织深度，RS码类型三者不匹配，且应满足(总帧长 - 子帧长 = 交错深度*N)，请重新输入!");
                msgBox.setText(strTmp);
                msgBox.exec();
                return FALSE;
            }
        }
        //FrmSizeMax = m_usFrameLen;
        //        if( ( m_usFrameLen < nMinFrmLen ) || ( m_usFrameLen > nMaxFrmLen ) )
        //        {
        //            strTmp = QString("此时帧长范围为[%1~%2]字，重新输入!").arg(nMinFrmLen).arg(nMaxFrmLen);
        //            msgBox.setText(strTmp);
        //            msgBox.exec();
        //            return FALSE;
        //        }
    }
    if( ( m_usFrameLen < FrmSizeMin ) || ( m_usFrameLen > FrmSizeMax ) )
    {
        strTmp = QString("此时帧长范围为[%1~%2]字，重新输入!").arg(FrmSizeMin).arg(FrmSizeMax);
        msgBox.setText(strTmp);
        msgBox.exec();
        return FALSE;
    }

    if(ui->lineEdit_FrmSyncGroup->text().isEmpty())
    {
        strTmp = QString("子帧同步码组不能为空!");
        msgBox.setText(strTmp);
        msgBox.exec();
        return FALSE;
    }
    UI64 tempSynCode = 0;
    UI64 tempSynCode1 = 0;
    QString m_dwlFrmSyncCode = ui->lineEdit_FrmSyncGroup->text();
    tempSynCode1 = StrToSynCode(m_dwlFrmSyncCode.toStdString().c_str());
    tempSynCode = tempSynCode1<<(64 - nFSPLen);
    tempSynCode = tempSynCode>>(64 - nFSPLen);
    if(tempSynCode != tempSynCode1)
    {
        strTmp = QString("输入帧同步码组的长度大于设置的帧同步码组长度,请重新输入!");
        msgBox.setText(strTmp);
        msgBox.exec();
        return FALSE;//输入帧同步码组的长度大于设置的帧同步码组长度,请重新输入
    }

    int nMinCodeRate = 32, nMaxCodeRate = 10000;
    if ((m_MCSDDTRevStatus) && (m_MCSDDTRevStatus->tParams.ucSignalSel == 1))
    {
        //nMinCodeRate=64;
        //YJG change 20120725 IQ合路最低码率按照单路码率32kbps
        nMinCodeRate = 32;
        nMaxCodeRate = 20000;
    }

    if((m_nDecodeType == 1) || (m_nDecodeType == 3))
    {
        if (ui->comboBox_RecurrentCodeType->currentIndex() == 0)
        {
            nMaxCodeRate /= 2;
        }
        else if (ui->comboBox_RecurrentCodeType->currentIndex() == 1)
        {
            nMaxCodeRate=nMaxCodeRate * 3/4;
        }
    }
    int m_ulCodeRate = ui->lineEdit_CodeRate->text().toInt();
    if((m_ulCodeRate < nMinCodeRate) || (m_ulCodeRate > nMaxCodeRate))
    {
        strTmp = QString("码率范围为[%1~%2]kbps，请重新输入!").arg(nMinCodeRate).arg(nMaxCodeRate);
        msgBox.setText(strTmp);
        msgBox.exec();
        return FALSE;
    }
    //2010-03-30 解扰长度
    UINT  tempSynCode2 = 0;
    UINT tempSynCode3 = 0;
    if(ui->comboBox_RandEn->currentIndex() == 2)
    {
        strTmp = ui->lineEdit_RandPoly->text();
        if(strTmp.isEmpty())
        {
            strTmp = QString("解扰多项式不能为空，请重新输入!");
            msgBox.setText(strTmp);
            msgBox.exec();
            return FALSE;
        }
        int m_nRandLen = ui->comboBox_RandPolyLen->currentIndex();
        tempSynCode2 = (long)StrToSynCode(strTmp.toStdString().c_str());
        tempSynCode3 = tempSynCode2<<(32 - (m_nRandLen + 1)*8);
        tempSynCode3 = tempSynCode3>>(32 - (m_nRandLen + 1)*8);
        if(tempSynCode2 != tempSynCode3)
        {
            strTmp = QString("解扰多项式长度大于设定的长度,请重新输入!");
            msgBox.setText(strTmp);
            msgBox.exec();
            return FALSE;
        }

        //2010 0323 添加解扰初相
        QString m_Phase = ui->lineEdit_RandPhase->text();
        if(m_Phase.isEmpty())
        {
            strTmp = QString("解扰初相不能为空，请重新输入!");
            msgBox.setText(strTmp);
            msgBox.exec();
            return FALSE;
        }

        int nPolyLenSet = (m_nRandLen + 1) * 8;
        int nPolyLenInput = m_Phase.length() * 4;
        if(nPolyLenInput > nPolyLenSet)
        {
            strTmp = QString("解扰初相长度大于设置长度，请重新输入!");
            msgBox.setText(strTmp);
            msgBox.exec();
            return FALSE;
        }

        tempSynCode2 = (long)StrToSynCode(m_Phase.toStdString().c_str());
        tempSynCode3 = tempSynCode2<<(32);
        tempSynCode3 = tempSynCode3>>(32);
        if(tempSynCode2 != tempSynCode3)
        {
            strTmp = QString("解扰初相的长度大于32位,请重新输入!");
            msgBox.setText(strTmp);
            msgBox.exec();
            return FALSE;
        }
    }

    int m_ucSubFrmType = ui->comboBox_SubFrmSyncType->currentIndex();
    if(m_ucSubFrmType != 0)
    {
        int m_usSubFrmLen = ui->lineEdit_SubFrmLen->text().toInt();
        if( ( m_usSubFrmLen < 4 ) || ( m_usSubFrmLen > 256 ) )
        {
            strTmp = QString("副帧长范围:[4～256]帧,请重新输入!");
            msgBox.setText(strTmp);
            msgBox.exec();
            return FALSE;
        }
    }

    //////////////////////////////////////////////////////
    //ID副帧和循环副帧的副帧位置
    int m_usIDSubFrmPos = ui->lineEdit_idCountPos->text().toInt();
    //int m_nDecodeType=  ui->lineEdit_SubFrmSynGroupPos->text().toInt();
    int m_nFspPlace = ui->comboBox_FrmPlace->currentIndex();
    if(m_ucSubFrmType == 1)
    {
        if(m_usIDSubFrmPos<1)
        {
            strTmp = QString("ID 字位置不能小于1!");
            msgBox.setText(strTmp);
            msgBox.exec();
            return FALSE;
        }

        if((m_nDecodeType == 0)||(m_nDecodeType == 1))
        {

            if(m_nFspPlace == 1)
            {

                if(m_usIDSubFrmPos > (m_usFrameLen - (nFSPLen / 8)))
                {
                    strTmp = QString("ID字位置不可超出子帧长度并且不能占用帧同步码组位置，请重新输入!");
                    msgBox.setText(strTmp);
                    msgBox.exec();
                    return FALSE;
                }
            }
            else
            {
                if((m_usIDSubFrmPos<((nFSPLen/8)+1))||(m_usIDSubFrmPos>m_usFrameLen))
                {
                    strTmp = QString("ID字位置不可超出子帧长度并且不能占用帧同步码组位置，请重新输入!");
                    msgBox.setText(strTmp);
                    msgBox.exec();

                    return FALSE;
                }
            }
        }
        else
        {
            if((m_usIDSubFrmPos<((nFSPLen/8)+1))||(m_usIDSubFrmPos>m_usFrameLen))
            {
                strTmp = QString("ID字位置不可超出子帧长度并且不能占用帧同步码组位置，请重新输入!");
                msgBox.setText(strTmp);
                msgBox.exec();
                return FALSE;
            }
        }

    }
    else if(m_ucSubFrmType==2)//循环副帧
    {
        int m_usCycFrmSynCodePos = ui->lineEdit_SubFrmSynGroupPos->text().toInt();
        int m_ucCycFrmSynCodeLen = ui->comboBox_SubFrameGroupLen->currentIndex() + 1;
        if(m_usCycFrmSynCodePos<1)
        {
            strTmp = QString("循环副帧码组位置不可小于1，请重新输入!");
            msgBox.setText(strTmp);
            msgBox.exec();
            return FALSE;
        }

        if((m_nDecodeType == 0)||(m_nDecodeType == 1))
        {
            if(m_nFspPlace == 1)//帧尾
            {
                if(m_usCycFrmSynCodePos > (m_usFrameLen - (nFSPLen / 8) - m_ucCycFrmSynCodeLen + 1))
                {
                    strTmp = QString("循环副帧码组位置不可超出子帧长度并且不能占用帧同步码组位置，请重新输入!");
                    msgBox.setText(strTmp);
                    msgBox.exec();
                    return FALSE;
                }
            }
            else if (m_nFspPlace == 0)//帧首
            {
                if((m_usCycFrmSynCodePos < (nFSPLen / 8 + 1)) || (m_usCycFrmSynCodePos > (m_usFrameLen - m_ucCycFrmSynCodeLen + 1)))
                {
                    strTmp = QString("循环副帧码组位置不可超出子帧长度并且不能占用帧同步码组位置，请重新输入!");
                    msgBox.setText(strTmp);
                    msgBox.exec();
                    return FALSE;
                }
            }
        }
        //        else
        //        {

        //            if((m_usCycFrmSynCodePos < (nFSPLen / 8 + 1))||(m_usCycFrmSynCodePos > (m_usFrameLen - m_ucCycFrmSynCodeLen + 1)))
        //            {
        //                strTmp = QString("循环副帧码组位置不可超出子帧长度并且不能占用帧同步码组位置，请重新输入!");
        //                msgBox.setText(strTmp);
        //                msgBox.exec();
        //                return FALSE;
        //            }
        //        }

        if( ( m_ucCycFrmSynCodeLen < 1 ) || ( m_ucCycFrmSynCodeLen > 4 ) )
        {
            strTmp = QString("循环副帧同步码组长度范围:[1～4]字节,请重新输入!");
            msgBox.setText(strTmp);
            msgBox.exec();
            return FALSE;
        }

        QString m_dwlCycFrmSyncCode=   ui->lineEdit_SubFrmGroup->text();
        if( ui->lineEdit_SubFrmGroup->text().isEmpty())
        {
            strTmp = QString("循环副帧同步码组输入不可为空，请重新输入!");
            msgBox.setText(strTmp);
            msgBox.exec();
            return FALSE;
        }

        tempSynCode1 = StrToSynCode(m_dwlCycFrmSyncCode.toStdString().c_str());
        tempSynCode =tempSynCode1<<(64-m_ucCycFrmSynCodeLen*8);
        tempSynCode =tempSynCode>>(64-m_ucCycFrmSynCodeLen*8);
        if(tempSynCode != tempSynCode1)
        {
            strTmp = QString("输入循环副帧同步码组的长度大于设置的循环副帧同步码组长度,请重新输入!");
            msgBox.setText(strTmp);
            msgBox.exec();
            return FALSE;
        }
    }
    return TRUE;
}
