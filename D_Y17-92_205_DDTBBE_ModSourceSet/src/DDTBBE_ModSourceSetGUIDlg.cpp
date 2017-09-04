#include "DDTBBE_ModSourceSetGUIDlg.h"
#include "ui_DDTBBE_ModSourceSetGUIDlg.h"
#include "../../Common/LxTsspCommonDefinition.h"
#include <QCloseEvent>
#include <QMessageBox>
#include <QDateTime>
#include "../../Common/Log.h"
#include <QDebug>

extern quint64 GetID(ushort usStationID, uchar ucSubSysID, ushort usDeviceID, ushort usType,
                     uchar ucSn);
DDTBBE_ModSourceSetGUIDlg::DDTBBE_ModSourceSetGUIDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DDTBBE_ModSourceSetGUIDlg)
{
    ui->setupUi(this);
    m_pStateMgr = NULL;

    m_pHAManager = NULL;
    m_IQSwitch = false;
    m_MCSDDTTMSimulStatus = NULL;
    m_MCSDDTRevStatus = NULL;
    m_MCSDDTTMQParamStatus = NULL;
    m_MCSDDTTMIParamStatus = NULL;
    m_autoMap = false;
    setAttribute(Qt::WA_DeleteOnClose);
}

DDTBBE_ModSourceSetGUIDlg::~DDTBBE_ModSourceSetGUIDlg()
{
    killTimer(m_TimerID);
    delete ui;
}

void DDTBBE_ModSourceSetGUIDlg::setText(const QString& strTitle, ParaStruct xmlsetText[3][60], QString MapMean[50][30], int MapNum[50], uchar Type, ILxTsspParameterMacro* pMacroObj)
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
void DDTBBE_ModSourceSetGUIDlg::InitDLG(int Type, ILxTsspParameterMacro* pMacroObj)
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
            DWORD stLen = sizeof(MCSDDTTMSimulStatusStruct);
            m_MCSDDTTMSimulStatus = (MCSDDTTMSimulStatusStruct*)m_pStateMgr->FindOneItem(m_StatusParaMCSDDTTMSimulParamID, stLen);
            if(m_MCSDDTTMSimulStatus == NULL || stLen != sizeof(MCSDDTTMSimulStatusStruct))
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
            pMacroObj->GetParameterBlock(m_ParaMCSDDTTMSimulParamID, byteArray);
            memcpy(&m_MCSDDTTMSimulParam, byteArray.data(), sizeof(MCSDDTTMSimulParamStruct));
        }
        ui->pushButton_AutoMap->setVisible(false);
        ui->pushButton_Setting->setVisible(false);
        ui->pushButton_Modify->hide();

        //显示参数
        ShowPara(1);
    }
}

//设置输入格式
void DDTBBE_ModSourceSetGUIDlg::InitInput()
{
    QRegExp regExp_Deep("[1-5]{1}");
    QRegExp regExp_Len("[0-9]{1,4}");
    //正整数
    QRegExp regExp("[0-9//-]{1,16}");
    //多普勒变化范围
    QRegExp regExpDopplerRange("[0-9]|[1-9][0-9]|[1-6][0-9]{2,2}");//0～600
    ui->lineEdit_DopplerRange->setValidator(new QRegExpValidator(regExpDopplerRange, this));
    //多普勒变化率
    QRegExp regExpopplerGrident("[0-9]{1,5}");//0～15000
    ui->lineEdit_DopplerGrident->setValidator(new QRegExpValidator(regExpopplerGrident, this));
    //载波输出载噪比
    QRegExp regExpNoiseRatio("[3-7][0-9](\\.[1-9])?|80"); //30～80 小数点后保留1位
    ui->lineEdit_NoiseRatio->setValidator(new QRegExpValidator(regExpNoiseRatio, this));
    //帧长
    ui->lineEdit_OutputPower->setValidator(new QRegExpValidator(regExp, this));
    //I/Q功率比
    QRegExp regExpIORatio("-?([0-9](\\.[0-9])?|10)");//-10～10 小数点后保留1位
    ui->lineEdit_IORatio->setValidator(new QRegExpValidator(regExpIORatio, this));
    //码率
    QRegExp regExpBitRate("([0-9]{1,5})(\\.[0-9]{1,3})");//32~20000 小数点后保留3位
    ui->lineEdit_BitRate->setValidator(new QRegExpValidator(regExpBitRate, this));
    //帧长
    ui->lineEdit_FrameLen->setValidator(new QRegExpValidator(regExp_Len, this));
    //副帧长
    ui->lineEdit_SubFrameLen->setValidator(new QRegExpValidator(regExp_Len, this));
    //副帧码组位置
    ui->lineEdit_SubFrameGroupPos->setValidator(new QRegExpValidator(regExp_Len, this));
    //ID计数位置
    ui->lineEdit_IDCountPos->setValidator(new QRegExpValidator(regExp_Len, this));
    //左旋控制时延
    ui->lineEdit_counterClockDelay->setValidator(new QRegExpValidator(regExp, this));
    //右旋控制时延
    ui->lineEdit_clockWiseDelay->setValidator(new QRegExpValidator(regExp, this));
    //交织深度
    ui->lineEdit_RSmixDeep->setValidator(new QRegExpValidator(regExp_Deep, this));
    ui->lineEdit_ORSmixDeep->setValidator(new QRegExpValidator(regExp_Deep, this));
    //
    QRegExp regExp_AnologData("[A-Fa-f0-9]{1,2}");
    ui->lineEdit_IAnologData->setValidator(new QRegExpValidator(regExp_AnologData, this));
    ui->lineEdit_OAnologData->setValidator(new QRegExpValidator(regExp_AnologData, this));



    //帧长
    ui->lineEdit_OFrameLen->setValidator(new QRegExpValidator(regExp_Len, this));
    //格式副帧长
    ui->lineEdit_OSubFrameLen->setValidator(new QRegExpValidator(regExp_Len, this));
    //码率
    ui->lineEdit_OBitRate->setValidator(new QRegExpValidator(regExpBitRate, this));

    //副帧码组位置
    ui->lineEdit_OSubFrameGroupPos->setValidator(new QRegExpValidator(regExp_Len, this));
    //ID计数位置
    ui->lineEdit_OIDCountPos->setValidator(new QRegExpValidator(regExp_Len, this));

    //字母数字
    QRegExp regExp2("[A-Fa-f0-9]{1,8}");
    //帧同步码组[HEX]
    ui->lineEdit_FrameSyncGroup->setValidator(new QRegExpValidator(regExp2, this));
    //副帧同步码组[HEX]
    ui->lineEdit_SubFrameSyncGroup->setValidator(new QRegExpValidator(regExp2, this));
    //加扰多项式
    ui->lineEdit_DisturbPoly->setValidator(new QRegExpValidator(regExp2, this));
    //加扰初相
    ui->lineEdit_DisturbPhasePos->setValidator(new QRegExpValidator(regExp2, this));

    //帧同步码组[HEX]
    ui->lineEdit_OFrameSyncGroup->setValidator(new QRegExpValidator(regExp2, this));
    //副帧同步码组[HEX]
    ui->lineEdit_OSubFrameSyncGroup->setValidator(new QRegExpValidator(regExp2, this));
    //加扰多项式
    ui->lineEdit_ODisturbPoly->setValidator(new QRegExpValidator(regExp2, this));
    //加扰初相
    ui->lineEdit_ODisturbPhasePos->setValidator(new QRegExpValidator(regExp2, this));
}
void DDTBBE_ModSourceSetGUIDlg::ShowControls()
{
    //向下拉框控件中写入从xml中读取的内容
    int i;
    //载波输出开关
    for(i=0; i<DlgMapLineNum[0]; i++)
    {
        ui->comboBox_WaveSwitch->addItem(m_DlgMapMean[0][i]);
    }
    ui->comboBox_WaveSwitch->setCurrentIndex(m_DlgParaSetStruct[0][0].InitVal.lval);
    //调制开关
    for(i=0; i<DlgMapLineNum[1]; i++)
    {
        ui->comboBox_MudulatingSwitch->addItem(m_DlgMapMean[1][i]);
    }
    ui->comboBox_MudulatingSwitch->setCurrentIndex(m_DlgParaSetStruct[0][1].InitVal.lval);
    //调制体制
    for(i=0; i<DlgMapLineNum[2]; i++)
    {
        ui->comboBox_MudulationSystem->addItem(m_DlgMapMean[2][i]);
    }
    ui->comboBox_MudulationSystem->setCurrentIndex(m_DlgParaSetStruct[0][2].InitVal.lval);
    //噪声输出开关
    for(i=0; i<DlgMapLineNum[4]; i++)
    {
        ui->comboBox_NoiseOutSwitch->addItem(m_DlgMapMean[4][i]);
    }
    ui->comboBox_NoiseOutSwitch->setCurrentIndex(m_DlgParaSetStruct[0][5].InitVal.lval);
    //格雷编码选择
    for(i=0; i<DlgMapLineNum[5]; i++)
    {
        ui->comboBox_GRAYType->addItem(m_DlgMapMean[5][i]);
    }
    ui->comboBox_GRAYType->setCurrentIndex(m_DlgParaSetStruct[0][7].InitVal.lval);
    //调制信号模型
    for(i=0; i<DlgMapLineNum[6]; i++)
    {
        ui->comboBox_IOMethod->addItem(m_DlgMapMean[6][i]);
    }
    ui->comboBox_IOMethod->setCurrentIndex(m_DlgParaSetStruct[0][8].InitVal.lval);
    //功率比有效标志
    for(i=0; i<DlgMapLineNum[7]; i++)
    {
        ui->comboBox_PowerFlag->addItem(m_DlgMapMean[7][i]);
    }
    ui->comboBox_PowerFlag->setCurrentIndex(m_DlgParaSetStruct[0][9].InitVal.lval);
    //I路功率因子
    for(i=0; i<DlgMapLineNum[8]; i++)
    {
        ui->comboBox_IPowerFactor->addItem(m_DlgMapMean[8][i]);
    }
    ui->comboBox_IPowerFactor->setCurrentIndex(m_DlgParaSetStruct[0][11].InitVal.lval);
    //Q路功率因子
    for(i=0; i<DlgMapLineNum[9]; i++)
    {
        ui->comboBox_QPowerFactor->addItem(m_DlgMapMean[9][i]);
    }
    ui->comboBox_QPowerFactor->setCurrentIndex(m_DlgParaSetStruct[0][12].InitVal.lval);
    //多普勒模拟开关
    for(i=0; i<DlgMapLineNum[10]; i++)
    {
        ui->comboBox_DopplerSwitch->addItem(m_DlgMapMean[10][i]);
    }
    ui->comboBox_DopplerSwitch->setCurrentIndex(m_DlgParaSetStruct[0][13].InitVal.lval);
    //调制信号来源
    for(i=0; i<DlgMapLineNum[11]; i++)
    {
        ui->comboBox_WayOfData->addItem(m_DlgMapMean[11][i]);
    }
    ui->comboBox_WayOfData->setCurrentIndex(m_DlgParaSetStruct[0][16].InitVal.lval);

    ui->label_WayOfData->hide();
    ui->comboBox_WayOfData->hide();

    /*************************************I路*****************************************/

    //码型
    for(i=0; i<DlgMapLineNum[12]; i++)
    {
        ui->comboBox_CodePattern->addItem(m_DlgMapMean[12][i]);
    }
    ui->comboBox_CodePattern->setCurrentIndex(m_DlgParaSetStruct[1][0].InitVal.lval);
    //RS编码
    for(i=0; i<DlgMapLineNum[13]; i++)
    {
        ui->comboBox_Enctype->addItem(m_DlgMapMean[13][i]);
    }
    ui->comboBox_Enctype->setCurrentIndex(m_DlgParaSetStruct[1][2].InitVal.lval);
    //卷积码类型
    for(i=0; i<DlgMapLineNum[14]; i++)
    {
        ui->comboBox_ConcodeType->addItem(m_DlgMapMean[14][i]);
    }
    ui->comboBox_ConcodeType->setCurrentIndex(m_DlgParaSetStruct[1][3].InitVal.lval);
    //RS码类型
    for(i=0; i<DlgMapLineNum[15]; i++)
    {
        ui->comboBox_RSType->addItem(m_DlgMapMean[15][i]);
    }
    ui->comboBox_RSType->setCurrentIndex(m_DlgParaSetStruct[1][4].InitVal.lval);
    //RS译码坐标基
    for(i=0; i<DlgMapLineNum[16]; i++)
    {
        ui->comboBox_RScoBase->addItem(m_DlgMapMean[16][i]);
    }
    ui->comboBox_RScoBase->setCurrentIndex(m_DlgParaSetStruct[1][5].InitVal.lval);
    //RS交织深度
    ui->lineEdit_RSmixDeep->setText(QString::number(m_DlgParaSetStruct[1][6].InitVal.lval + 1));
    //卷积输出相位
    for(i=0; i<DlgMapLineNum[18]; i++)
    {
        ui->comboBox_ConoutPhase->addItem(m_DlgMapMean[18][i]);
    }
    ui->comboBox_ConoutPhase->setCurrentIndex(m_DlgParaSetStruct[1][7].InitVal.lval);
    //帧同步码组长
    for(i=0; i<DlgMapLineNum[19]; i++)
    {
        ui->comboBox_FrameSyncGroupLen->addItem(m_DlgMapMean[19][i]);
    }
    ui->comboBox_FrameSyncGroupLen->setCurrentIndex(m_DlgParaSetStruct[1][9].InitVal.lval);
    //帧同步码组位置
    for(i=0; i<DlgMapLineNum[20]; i++)
    {
        ui->comboBox_FrameSyncGroupPos->addItem(m_DlgMapMean[20][i]);
    }
    ui->comboBox_FrameSyncGroupPos->setCurrentIndex(m_DlgParaSetStruct[1][11].InitVal.lval);
    //加扰控制
    for(i=0; i<DlgMapLineNum[21]; i++)
    {
        ui->comboBox_AddDisturbCtrl->addItem(m_DlgMapMean[21][i]);
    }
    ui->comboBox_AddDisturbCtrl->setCurrentIndex(m_DlgParaSetStruct[1][12].InitVal.lval);
    //加扰多项式长度
    for(i=0; i<DlgMapLineNum[22]; i++)
    {
        ui->comboBox_DisturbPolyLen->addItem(m_DlgMapMean[22][i]);
    }
    ui->comboBox_DisturbPolyLen->setCurrentIndex(m_DlgParaSetStruct[1][13].InitVal.lval);
    //副帧码组长度
    for(i=0; i<DlgMapLineNum[24]; i++)
    {
        ui->comboBox_SubFrameGroupLen->addItem(m_DlgMapMean[24][i]);
    }
    ui->comboBox_SubFrameGroupLen->setCurrentIndex(m_DlgParaSetStruct[1][18].InitVal.lval);
    //副帧同步方式
    for(i=0; i<DlgMapLineNum[23]; i++)
    {
        ui->comboBox_SubFrameSyncType->addItem(m_DlgMapMean[23][i]);
    }
    ui->comboBox_SubFrameSyncType->setCurrentIndex(m_DlgParaSetStruct[1][17].InitVal.lval);
    //ID计数基值
    for(i=0; i<DlgMapLineNum[25]; i++)
    {
        ui->comboBox_IDCountBaseValue->addItem(m_DlgMapMean[25][i]);
    }
    ui->comboBox_IDCountBaseValue->setCurrentIndex(m_DlgParaSetStruct[1][22].InitVal.lval);
    //ID计数方向
    for(i=0; i<DlgMapLineNum[26]; i++)
    {
        ui->comboBox_IDCountDir->addItem(m_DlgMapMean[26][i]);
    }
    ui->comboBox_IDCountDir->setCurrentIndex(m_DlgParaSetStruct[1][23].InitVal.lval);
    //数据类型
    for(i=0; i<DlgMapLineNum[27]; i++)
    {
        ui->comboBox_DataType->addItem(m_DlgMapMean[27][i]);
    }
    ui->comboBox_DataType->setCurrentIndex(m_DlgParaSetStruct[1][25].InitVal.lval);


    /*************************************Q路*****************************************/

    //码型
    for(i=0; i<DlgMapLineNum[28]; i++)
    {
        ui->comboBox_OCodePattern->addItem(m_DlgMapMean[28][i]);
    }
    ui->comboBox_OCodePattern->setCurrentIndex(m_DlgParaSetStruct[2][0].InitVal.lval);
    //RS编码
    for(i=0; i<DlgMapLineNum[29]; i++)
    {
        ui->comboBox_OEnctype->addItem(m_DlgMapMean[29][i]);
    }
    ui->comboBox_OEnctype->setCurrentIndex(m_DlgParaSetStruct[2][2].InitVal.lval);
    //卷积码类型
    for(i=0; i<DlgMapLineNum[30]; i++)
    {
        ui->comboBox_OConcodeType->addItem(m_DlgMapMean[30][i]);
    }
    ui->comboBox_OConcodeType->setCurrentIndex(m_DlgParaSetStruct[2][3].InitVal.lval);
    //RS码类型
    for(i=0; i<DlgMapLineNum[31]; i++)
    {
        ui->comboBox_ORSType->addItem(m_DlgMapMean[31][i]);
    }
    ui->comboBox_ORSType->setCurrentIndex(m_DlgParaSetStruct[2][4].InitVal.lval);
    //RS译码坐标基
    for(i=0; i<DlgMapLineNum[32]; i++)
    {
        ui->comboBox_ORScoBase->addItem(m_DlgMapMean[32][i]);
    }
    ui->comboBox_ORScoBase->setCurrentIndex(m_DlgParaSetStruct[2][5].InitVal.lval);
    //RS交织深度
    ui->lineEdit_ORSmixDeep->setText(QString::number(m_DlgParaSetStruct[2][6].InitVal.lval + 1));
    //卷积输出相位
    for(i=0; i<DlgMapLineNum[34]; i++)
    {
        ui->comboBox_OConoutPhase->addItem(m_DlgMapMean[34][i]);
    }
    ui->comboBox_OConoutPhase->setCurrentIndex(m_DlgParaSetStruct[2][7].InitVal.lval);
    //帧同步码组长
    for(i=0; i<DlgMapLineNum[35]; i++)
    {
        ui->comboBox_OFrameSyncGroupLen->addItem(m_DlgMapMean[35][i]);
    }
    ui->comboBox_OFrameSyncGroupLen->setCurrentIndex(m_DlgParaSetStruct[2][9].InitVal.lval);
    //帧同步码组位置
    for(i=0; i<DlgMapLineNum[36]; i++)
    {
        ui->comboBox_OFrameSyncGroupPos->addItem(m_DlgMapMean[36][i]);
    }
    ui->comboBox_OFrameSyncGroupPos->setCurrentIndex(m_DlgParaSetStruct[2][11].InitVal.lval);
    //加扰控制
    for(i=0; i<DlgMapLineNum[37];i++)
    {
        ui->comboBox_OAddDisturbCtrl->addItem(m_DlgMapMean[37][i]);
    }
    ui->comboBox_OAddDisturbCtrl->setCurrentIndex(m_DlgParaSetStruct[2][12].InitVal.lval);
    //加扰多项式长度
    for(i=0; i<DlgMapLineNum[38]; i++)
    {
        ui->comboBox_ODisturbPolyLen->addItem(m_DlgMapMean[38][i]);
    }
    ui->comboBox_ODisturbPolyLen->setCurrentIndex(m_DlgParaSetStruct[2][13].InitVal.lval);
    //副帧码组长度
    for(i=0; i<DlgMapLineNum[40]; i++)
    {
        ui->comboBox_OSubFrameGroupLen->addItem(m_DlgMapMean[40][i]);
    }
    ui->comboBox_OSubFrameGroupLen->setCurrentIndex(m_DlgParaSetStruct[2][18].InitVal.lval);
    //副帧同步方式
    for(i=0; i<DlgMapLineNum[39]; i++)
    {
        ui->comboBox_OSubFrameSyncType->addItem(m_DlgMapMean[39][i]);
    }
    ui->comboBox_OSubFrameSyncType->setCurrentIndex(m_DlgParaSetStruct[2][17].InitVal.lval);
    //ID计数基值
    for(i=0; i<DlgMapLineNum[41]; i++)
    {
        ui->comboBox_OIDCountBaseValue->addItem(m_DlgMapMean[41][i]);
    }
    ui->comboBox_OIDCountBaseValue->setCurrentIndex(m_DlgParaSetStruct[2][22].InitVal.lval);
    //ID计数方向
    for(i=0; i<DlgMapLineNum[42]; i++)
    {
        ui->comboBox_OIDCountDir->addItem(m_DlgMapMean[42][i]);
    }
    ui->comboBox_OIDCountDir->setCurrentIndex(m_DlgParaSetStruct[2][23].InitVal.lval);
    //数据类型
    for(i=0; i<DlgMapLineNum[43]; i++)
    {
        ui->comboBox_ODataType->addItem(m_DlgMapMean[43][i]);
    }
    ui->comboBox_ODataType->setCurrentIndex(m_DlgParaSetStruct[2][25].InitVal.lval);
    //左旋时延控制
    for(i=0; i<DlgMapLineNum[44]; i++)
    {
        ui->comboBox_counterClockCtrl->addItem(m_DlgMapMean[44][i]);
    }
    ui->comboBox_counterClockCtrl->setCurrentIndex(m_DlgParaSetStruct[2][26].InitVal.lval);
    //右旋时延控制
    for(i=0; i<DlgMapLineNum[45]; i++)
    {
        ui->comboBox_clockWiseCtrl->addItem(m_DlgMapMean[45][i]);
    }
    ui->comboBox_clockWiseCtrl->setCurrentIndex(m_DlgParaSetStruct[2][28].InitVal.lval);

    //往静态控件中写入参数缺省值
    //输出功率
    ui->label_OutputPower->hide();
    ui->lineEdit_OutputPower->hide();
    ui->lineEdit_OutputPower->setText(QString::number(m_DlgParaSetStruct[0][4].InitVal.lval));
    ui->label_OutputPower->setText("输出功率\n[" + QString::number(m_DlgParaSetStruct[0][4].MinVal.lmin) + "~"
            + QString::number(m_DlgParaSetStruct[0][4].MaxVal.lmax) + "]dBm");
    //输出载噪比
    ui->lineEdit_NoiseRatio->setText(QString::number(m_DlgParaSetStruct[0][6].InitVal.lval));
    ui->label_NoiseRatio->setText("输出载噪比\n[" + QString::number(m_DlgParaSetStruct[0][6].MinVal.lmin) + "~"
            + QString::number(m_DlgParaSetStruct[0][6].MaxVal.lmax) + "]");
    //IO功率分贝比
    ui->lineEdit_IORatio->setText(QString::number(m_DlgParaSetStruct[0][10].InitVal.lval));
    //多普勒变化范围
    ui->lineEdit_DopplerRange->setText(QString::number(m_DlgParaSetStruct[0][14].InitVal.lval));
    ui->label_DopplerRange->setText("多普勒变化范围\n[" + QString::number(m_DlgParaSetStruct[0][14].MinVal.lmin) + "~"
            + QString::number(m_DlgParaSetStruct[0][14].MaxVal.lmax) + "]");
    //多普勒变化率
    ui->lineEdit_DopplerGrident->setText(QString::number(m_DlgParaSetStruct[0][15].InitVal.lval));
    ui->label_DopplerGrident->setText("多普勒变化率\n[" + QString::number(m_DlgParaSetStruct[0][15].MinVal.lmin) + "~"
            + QString::number(m_DlgParaSetStruct[0][15].MaxVal.lmax) + "]");

    //I路参数

    //码速率
    ui->lineEdit_BitRate->setText(QString::number(m_DlgParaSetStruct[1][1].InitVal.lval));
    //帧长
    ui->lineEdit_FrameLen->setText(QString::number(m_DlgParaSetStruct[1][8].InitVal.lval));
    //帧同步码组
    ui->lineEdit_FrameSyncGroup->setText(QString::number(m_DlgParaSetStruct[1][10].InitVal.lval, 16).toUpper());
    //加扰多项式
    ui->lineEdit_DisturbPoly->setText(QString::number(m_DlgParaSetStruct[1][14].InitVal.lval, 16).toUpper());
    //加扰多项式初相
    ui->lineEdit_DisturbPhasePos->setText(QString::number(m_DlgParaSetStruct[1][15].InitVal.lval, 16).toUpper());
    //副帧长
    ui->lineEdit_SubFrameLen->setText(QString::number(m_DlgParaSetStruct[1][16].InitVal.lval));
    //副帧码组位置
    ui->lineEdit_SubFrameGroupPos->setText(QString::number(m_DlgParaSetStruct[1][19].InitVal.lval));
    //副帧码组
    ui->lineEdit_SubFrameSyncGroup->setText(QString::number(m_DlgParaSetStruct[1][20].InitVal.lval, 16).toUpper());
    //ID计数位置
    ui->lineEdit_IDCountPos->setText(QString::number(m_DlgParaSetStruct[1][21].InitVal.lval));
    //模拟数据
    ui->lineEdit_IAnologData->setText(QString::number(m_DlgParaSetStruct[1][24].InitVal.lval));

    //Q路参数

    //码速率
    ui->lineEdit_OBitRate->setText(QString::number(m_DlgParaSetStruct[2][1].InitVal.lval));
    //帧长
    ui->lineEdit_OFrameLen->setText(QString::number(m_DlgParaSetStruct[2][8].InitVal.lval));
    //帧同步码组
    ui->lineEdit_OFrameSyncGroup->setText(QString::number(m_DlgParaSetStruct[2][10].InitVal.lval, 16).toUpper());
    //加扰多项式
    ui->lineEdit_ODisturbPoly->setText(QString::number(m_DlgParaSetStruct[2][14].InitVal.lval, 16).toUpper());
    //加扰多项式初相
    ui->lineEdit_ODisturbPhasePos->setText(QString::number(m_DlgParaSetStruct[2][15].InitVal.lval, 16).toUpper());
    //副帧长度
    ui->lineEdit_OSubFrameLen->setText(QString::number(m_DlgParaSetStruct[2][16].InitVal.lval));
    //副帧码组位置
    ui->lineEdit_OSubFrameGroupPos->setText(QString::number(m_DlgParaSetStruct[2][19].InitVal.lval));
    //副帧码组
    ui->lineEdit_OSubFrameSyncGroup->setText(QString::number(m_DlgParaSetStruct[2][20].InitVal.lval, 16).toUpper());
    //ID计数位置
    ui->lineEdit_OIDCountPos->setText(QString::number(m_DlgParaSetStruct[2][21].InitVal.lval));
    //模拟数据
    ui->lineEdit_OAnologData->setText(QString::number(m_DlgParaSetStruct[2][24].InitVal.lval));
    //左旋时延
    ui->lineEdit_counterClockDelay->setText(QString::number(m_DlgParaSetStruct[2][27].InitVal.lval));
    //右旋时延
    ui->lineEdit_clockWiseDelay->setText(QString::number(m_DlgParaSetStruct[2][29].InitVal.lval));

}

//显示参数
void DDTBBE_ModSourceSetGUIDlg::ShowPara(int type)
{
    if(type == 0)       //参数设置
    {
        if (m_MCSDDTRevStatus != NULL)
        {
            m_MCSDDTRevParam = m_MCSDDTRevStatus->tParams;
        }
        else
        {
            DWORD stLen1 = sizeof(MCSDDTRevStatusStruct);
            if (m_pStateMgr != NULL){

                m_MCSDDTRevStatus = (MCSDDTRevStatusStruct*)m_pStateMgr->FindOneItem(m_StatusParaMCSDDTRevParamID, stLen1);
            }
        }
        ///////////////////////////////////////////////////////////////////////////////////
        if (m_MCSDDTTMIParamStatus != NULL)
        {
            m_MCSDDTTMIParam = m_MCSDDTTMIParamStatus->tParams;
        }
        else
        {
            DWORD stLen2 = sizeof(MCSDDTTMIStatusStruct);
            if (m_pStateMgr != NULL){

                m_MCSDDTTMIParamStatus = (MCSDDTTMIStatusStruct*)m_pStateMgr->FindOneItem(m_StatusParaMCSDDTTMIParamID, stLen2);
            }
        }
        ////////////////////////////////////////////////////////////////////////////////////
        if (m_MCSDDTTMQParamStatus != NULL)
        {
            m_MCSDDTTMQParam = m_MCSDDTTMQParamStatus->tParams;
        }
        else
        {
            DWORD stLen3 = sizeof(MCSDDTTMQStatusStruct);
            if (m_pStateMgr != NULL){

                m_MCSDDTTMQParamStatus = (MCSDDTTMQStatusStruct*)m_pStateMgr->FindOneItem(m_StatusParaMCSDDTTMQParamID, stLen3);
            }
        }
        //自动映射
        if (m_MCSDDTTMSimulStatus != NULL)
        {
            if(m_autoMap)
            {
                //从接收机单元映射
                m_MCSDDTTMSimulParam.ucModesel = m_MCSDDTRevParam.ucModesel;
                m_MCSDDTTMSimulParam.cRatiodB = m_MCSDDTRevParam.cRatiodB;
                m_MCSDDTTMSimulParam.ucIPowRate = m_MCSDDTRevParam.ucIPowRate;
                m_MCSDDTTMSimulParam.ucQPowRate = m_MCSDDTRevParam.ucQPowRate;
                m_MCSDDTTMSimulParam.ucSignalSel = m_MCSDDTRevParam.ucSignalSel;
                m_MCSDDTTMSimulParam.ucGrayType = m_MCSDDTRevParam.ucGrayType;
                m_MCSDDTTMSimulParam.ucPowerType = m_MCSDDTRevParam.ucPowerType;
                //从解调I路映射
                m_MCSDDTTMSimulParam.ucCodeStyleI = m_MCSDDTTMIParam.ucCodeStyle;
                int factor = m_MCSDDTTMSimulParam.ucSignalSel == 1 ? 2 : 1;
                m_MCSDDTTMSimulParam.usCodeRateI = m_MCSDDTTMIParam.usCodeRate * factor;
                m_MCSDDTTMSimulParam.ucCodingMdI = m_MCSDDTTMIParam.ucCodingMd;
                m_MCSDDTTMSimulParam.ucViterbiMdI = m_MCSDDTTMIParam.ucViterbiMd;
                m_MCSDDTTMSimulParam.ucRSMdI = m_MCSDDTTMIParam.ucRSMd;
                m_MCSDDTTMSimulParam.ucInterleavI = m_MCSDDTTMIParam.ucInterleav;
                m_MCSDDTTMSimulParam.ucG2InvI = m_MCSDDTTMIParam.ucG2Inv;
                m_MCSDDTTMSimulParam.ucBasiSelI = m_MCSDDTTMIParam.ucBasiSel;
                m_MCSDDTTMSimulParam.usFrameLenI = m_MCSDDTTMIParam.usFrameLen;
                m_MCSDDTTMSimulParam.ucFrameSynGLenI = m_MCSDDTTMIParam.ucFrameSynGLen;
                m_MCSDDTTMSimulParam.uiFrameSynGI = m_MCSDDTTMIParam.uiFrameSynG;
                m_MCSDDTTMSimulParam.ucturbCtrlI = m_MCSDDTTMIParam.ucUnturbCtrl;
                m_MCSDDTTMSimulParam.ucRandLenI = m_MCSDDTTMIParam.ucRandLen;
                m_MCSDDTTMSimulParam.ucFspPlaceI = m_MCSDDTTMIParam.ucFspPlace;
                m_MCSDDTTMSimulParam.uiRandPolyI = m_MCSDDTTMIParam.uiRandPoly;
                m_MCSDDTTMSimulParam.uiPhaseI = m_MCSDDTTMIParam.uiPhase;
                m_MCSDDTTMSimulParam.ucSframeLenI = m_MCSDDTTMIParam.ucSframeLen;
                switch (m_MCSDDTTMIParam.ucSFrameSynMd) {//I路副帧方式
                case 1:
                    m_MCSDDTTMSimulParam.ucSFrameSynMdI = 4;
                    break;
                case 4:
                    m_MCSDDTTMSimulParam.ucSFrameSynMdI = 1;
                    break;
                default:
                    m_MCSDDTTMSimulParam.ucSFrameSynMdI = m_MCSDDTTMIParam.ucSFrameSynMd;
                }
                m_MCSDDTTMSimulParam.ucCycFrmSynCodeLenI = m_MCSDDTTMIParam.ucCycFrmSynCodeLen;
                m_MCSDDTTMSimulParam.usCycFrmSynCodePosI = m_MCSDDTTMIParam.usCycFrmSynCodePos;
                m_MCSDDTTMSimulParam.uiCycFrmSynCodeI = m_MCSDDTTMIParam.uiCycFrmSynCode;
                m_MCSDDTTMSimulParam.ucIDBaseI = m_MCSDDTTMIParam.ucIDBase;
                m_MCSDDTTMSimulParam.ucIDDirectI = m_MCSDDTTMIParam.ucIDDirect;
                m_MCSDDTTMSimulParam.usIDSubFrmPosI = m_MCSDDTTMIParam.usIDSubFrmPos;
                //从解调Q路映射
                m_MCSDDTTMSimulParam.ucCodeStyleQ = m_MCSDDTTMQParam.ucCodeStyle;
                m_MCSDDTTMSimulParam.usCodeRateQ = m_MCSDDTTMQParam.usCodeRate;
                m_MCSDDTTMSimulParam.ucCodingMdQ = m_MCSDDTTMQParam.ucCodingMd;
                m_MCSDDTTMSimulParam.ucViterbiMdQ = m_MCSDDTTMQParam.ucViterbiMd;
                m_MCSDDTTMSimulParam.ucRSMdQ = m_MCSDDTTMQParam.ucRSMd;
                m_MCSDDTTMSimulParam.ucInterleavQ = m_MCSDDTTMQParam.ucInterleav;
                m_MCSDDTTMSimulParam.ucG2InvQ = m_MCSDDTTMQParam.ucG2Inv;
                m_MCSDDTTMSimulParam.ucBasiSelQ = m_MCSDDTTMQParam.ucBasiSel;
                m_MCSDDTTMSimulParam.usFrameLenQ = m_MCSDDTTMQParam.usFrameLen;
                m_MCSDDTTMSimulParam.ucFrameSynGLenQ = m_MCSDDTTMQParam.ucFrameSynGLen;
                m_MCSDDTTMSimulParam.uiFrameSynGQ = m_MCSDDTTMQParam.uiFrameSynG;
                m_MCSDDTTMSimulParam.ucturbCtrlQ = m_MCSDDTTMQParam.ucUnturbCtrl;
                m_MCSDDTTMSimulParam.ucRandLenQ = m_MCSDDTTMQParam.ucRandLen;
                m_MCSDDTTMSimulParam.ucFspPlaceQ = m_MCSDDTTMQParam.ucFspPlace;
                m_MCSDDTTMSimulParam.uiRandPolyQ = m_MCSDDTTMQParam.uiRandPoly;
                m_MCSDDTTMSimulParam.uiPhaseQ = m_MCSDDTTMQParam.uiPhase;
                m_MCSDDTTMSimulParam.ucSframeLenQ = m_MCSDDTTMQParam.ucSframeLen;
                switch (m_MCSDDTTMQParam.ucSFrameSynMd) {//Q路副帧同步方式
                case 1:
                    m_MCSDDTTMSimulParam.ucSFrameSynMdQ = 4;
                    break;
                case 4:
                    m_MCSDDTTMSimulParam.ucSFrameSynMdQ = 1;
                    break;
                default:
                    m_MCSDDTTMSimulParam.ucSFrameSynMdQ = m_MCSDDTTMQParam.ucSFrameSynMd;
                }
                m_MCSDDTTMSimulParam.ucCycFrmSynCodeLenQ = m_MCSDDTTMQParam.ucCycFrmSynCodeLen;
                m_MCSDDTTMSimulParam.usCycFrmSynCodePosQ = m_MCSDDTTMQParam.usCycFrmSynCodePos;
                m_MCSDDTTMSimulParam.uiCycFrmSynCodeQ = m_MCSDDTTMQParam.uiCycFrmSynCode;
                m_MCSDDTTMSimulParam.ucIDBaseQ = m_MCSDDTTMQParam.ucIDBase;
                m_MCSDDTTMSimulParam.ucIDDirectQ = m_MCSDDTTMQParam.ucIDDirect;
                m_MCSDDTTMSimulParam.usIDSubFrmPosQ = m_MCSDDTTMQParam.usIDSubFrmPos;

            }
            else
            {
                m_MCSDDTTMSimulParam = m_MCSDDTTMSimulStatus->tParams;
            }
        }
        else
        {
            DWORD stLen = sizeof(MCSDDTTMSimulStatusStruct);
            if (m_pStateMgr != NULL){

                m_MCSDDTTMSimulStatus = (MCSDDTTMSimulStatusStruct*)m_pStateMgr->FindOneItem(m_StatusParaMCSDDTTMSimulParamID, stLen);
            }
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

    //载波输出开关
    if (m_MCSDDTTMSimulParam.ucOnline > 0)
        ui->comboBox_WaveSwitch->setCurrentIndex(m_MCSDDTTMSimulParam.ucOnline - 1);
    //调制开关
    if (m_MCSDDTTMSimulParam.ucModu > 0)
        ui->comboBox_MudulatingSwitch->setCurrentIndex(m_MCSDDTTMSimulParam.ucModu - 1);
    //输出调制体制
    if (m_MCSDDTTMSimulParam.ucModesel > 0)
        ui->comboBox_MudulationSystem->setCurrentIndex(m_MCSDDTTMSimulParam.ucModesel - 1);
    //SPSK星座图
    //    ui->comboBox_SPSKPlanisphere->setCurrentIndex(m_MCSDDTTMSimulParam.ucSPSK);
    //输出功率
    //   ui->lineEdit_OutputPower->setText(QString::number(m_MCSDDTTMSimulParam.ulPower));
    //噪声输出开关
    if (m_MCSDDTTMSimulParam.ucNoiseSw > 0)
        ui->comboBox_NoiseOutSwitch->setCurrentIndex(m_MCSDDTTMSimulParam.ucNoiseSw - 1);
    //输出载噪比
    ui->lineEdit_NoiseRatio->setText(QString::number(m_MCSDDTTMSimulParam.ucCN0));
    //格雷编码选择
    if (m_MCSDDTTMSimulParam.ucGrayType > 0)
        ui->comboBox_GRAYType->setCurrentIndex(m_MCSDDTTMSimulParam.ucGrayType - 1);
    //IQ合路分路
    if (m_MCSDDTTMSimulParam.ucSignalSel > 0)
        ui->comboBox_IOMethod->setCurrentIndex(setIOMethod());
    //功率比有效标志
    if (m_MCSDDTTMSimulParam.ucPowerType > 0)
        ui->comboBox_PowerFlag->setCurrentIndex(m_MCSDDTTMSimulParam.ucPowerType - 1);
    //I/O功率分贝比
    ui->lineEdit_IORatio->setText(QString::number(m_MCSDDTTMSimulParam.cRatiodB));
    //I路功率因子
    if (m_MCSDDTTMSimulParam.ucIPowRate > 0)
        ui->comboBox_IPowerFactor->setCurrentIndex(m_MCSDDTTMSimulParam.ucIPowRate - 1);
    //Q路功率因子
    if (m_MCSDDTTMSimulParam.ucQPowRate > 0)
        ui->comboBox_QPowerFactor->setCurrentIndex(m_MCSDDTTMSimulParam.ucQPowRate - 1);
    //多普勒动态模拟开关
    if (m_MCSDDTTMSimulParam.ucSimSw > 0)
        ui->comboBox_DopplerSwitch->setCurrentIndex(m_MCSDDTTMSimulParam.ucSimSw - 1);
    //多普勒变化范围
    ui->lineEdit_DopplerRange->setText(QString::number(m_MCSDDTTMSimulParam.iSimRange));
    //多普勒变化率
    ui->lineEdit_DopplerGrident->setText(QString::number(m_MCSDDTTMSimulParam.iSimRate));
    //调制信号来源
    //    ui->comboBox_WayOfData->setCurrentIndex(m_MCSDDTTMSimulParam.ucDataSource);
    //左旋控制时延
    //ui->comboBox_counterClockCtrl->setCurrentIndex(m_MCSDDTTMSimulParam.ucTimeDlySwL);
    //左旋时延
    //ui->lineEdit_counterClockDelay->setText(QString::number(m_MCSDDTTMSimulParam.wTimeDlySizeL));
    //右旋控制时延
    //ui->comboBox_clockWiseCtrl->setCurrentIndex(m_MCSDDTTMSimulParam.ucTimeDlySwR);
    //右旋时延
    //ui->lineEdit_clockWiseDelay->setText(QString::number(m_MCSDDTTMSimulParam.wTimeDlySizeR));

    //码型选择
    if (m_MCSDDTTMSimulParam.ucCodeStyleI > 0)
        ui->comboBox_CodePattern->setCurrentIndex(m_MCSDDTTMSimulParam.ucCodeStyleI - 1);
    //码速率
    ui->lineEdit_BitRate->setText(QString::number(m_MCSDDTTMSimulParam.usCodeRateI / 1000.0));
    //I路译码方式
    if (m_MCSDDTTMSimulParam.ucCodingMdI > 0)
    {
        if (m_MCSDDTTMSimulParam.ucCodingMdI == 6)
            ui->comboBox_Enctype->setCurrentIndex(4);
        else
            ui->comboBox_Enctype->setCurrentIndex(m_MCSDDTTMSimulParam.ucCodingMdI - 1);
    }
    //卷积码类型
    if (m_MCSDDTTMSimulParam.ucViterbiMdI > 0)
        ui->comboBox_ConcodeType->setCurrentIndex(m_MCSDDTTMSimulParam.ucViterbiMdI - 1);
    //RS码类型
    if (m_MCSDDTTMSimulParam.ucRSMdI > 0)
        ui->comboBox_RSType->setCurrentIndex(m_MCSDDTTMSimulParam.ucRSMdI - 1);
    //RS译码坐标基
    if (m_MCSDDTTMSimulParam.ucBasiSelI > 0)
        ui->comboBox_RScoBase->setCurrentIndex(m_MCSDDTTMSimulParam.ucBasiSelI - 1);
    //RS交织深度
    if (m_MCSDDTTMSimulParam.ucInterleavI > 0)
        ui->lineEdit_RSmixDeep->setText(QString::number(m_MCSDDTTMSimulParam.ucInterleavI));
    //卷积输出相位
    if (m_MCSDDTTMSimulParam.ucG2InvI > 0)
        ui->comboBox_ConoutPhase->setCurrentIndex(m_MCSDDTTMSimulParam.ucG2InvI - 1);
    //帧长
    ui->lineEdit_FrameLen->setText(QString::number(m_MCSDDTTMSimulParam.usFrameLenI));
    //帧同步码组长
    ui->comboBox_FrameSyncGroupLen->setCurrentText(QString::number(m_MCSDDTTMSimulParam.ucFrameSynGLenI)+ "bits");
    int byteFrameSynGLenI = 8;
    if(m_MCSDDTTMSimulParam.ucFrameSynGLenI == 16 ||
            m_MCSDDTTMSimulParam.ucFrameSynGLenI == 24 ||
            m_MCSDDTTMSimulParam.ucFrameSynGLenI == 32)
    {
        byteFrameSynGLenI = m_MCSDDTTMSimulParam.ucFrameSynGLenI/4;
    }
    //帧同步码组
    if (m_MCSDDTTMSimulParam.uiFrameSynGI != 0)
        ui->lineEdit_FrameSyncGroup->setText(QString("%1").
                                             arg(m_MCSDDTTMSimulParam.uiFrameSynGI,byteFrameSynGLenI,
                                                 16, QChar::fromLatin1('0')).toUpper());
    else
        ui->lineEdit_FrameSyncGroup->setText("0");
    //帧同步码组位置
    if (m_MCSDDTTMSimulParam.ucFspPlaceI > 0)
        ui->comboBox_FrameSyncGroupPos->setCurrentIndex(m_MCSDDTTMSimulParam.ucFspPlaceI - 1);
    //加扰
    if (m_MCSDDTTMSimulParam.ucturbCtrlI > 0)
        ui->comboBox_AddDisturbCtrl->setCurrentIndex(m_MCSDDTTMSimulParam.ucturbCtrlI - 1);
    //加扰多项式长度
    if (m_MCSDDTTMSimulParam.ucRandLenI > 0)
        ui->comboBox_DisturbPolyLen->setCurrentIndex(m_MCSDDTTMSimulParam.ucRandLenI - 1);
    //加扰多项式
    if (m_MCSDDTTMSimulParam.uiRandPolyI != 0)
        ui->lineEdit_DisturbPoly->setText(QString("%1").
                                          arg(m_MCSDDTTMSimulParam.uiRandPolyI, m_MCSDDTTMSimulParam.ucRandLenI * 2,
                                              16, QChar::fromLatin1('0')).toUpper());
    else
        ui->lineEdit_DisturbPoly->setText("0");
    //加扰初相
    if (m_MCSDDTTMSimulParam.uiPhaseI != 0)
        ui->lineEdit_DisturbPhasePos->setText(QString("%1").
                                              arg(m_MCSDDTTMSimulParam.uiPhaseI,m_MCSDDTTMSimulParam.ucRandLenI * 2,
                                                  16, QChar::fromLatin1('0')).toUpper());
    else
        ui->lineEdit_DisturbPhasePos->setText("0");
    //副帧长度
    ui->lineEdit_SubFrameLen->setText(QString::number(m_MCSDDTTMSimulParam.ucSframeLenI));
    //副帧方式
    if (m_MCSDDTTMSimulParam.ucSFrameSynMdI > 0)
        ui->comboBox_SubFrameSyncType->setCurrentIndex(m_MCSDDTTMSimulParam.ucSFrameSynMdI - 1);
    //副帧码组长度
    int byteSubFrameSynGLenI = 8;
    if (m_MCSDDTTMSimulParam.ucCycFrmSynCodeLenI > 0)
        ui->comboBox_SubFrameGroupLen->setCurrentIndex(m_MCSDDTTMSimulParam.ucCycFrmSynCodeLenI - 1);
    byteSubFrameSynGLenI = m_MCSDDTTMSimulParam.ucCycFrmSynCodeLenI * 2;
    //副帧码组位置
    ui->lineEdit_SubFrameGroupPos->setText(QString::number(m_MCSDDTTMSimulParam.usCycFrmSynCodePosI));
    //副帧码组
    if (m_MCSDDTTMSimulParam.uiCycFrmSynCodeI != 0)
        ui->lineEdit_SubFrameSyncGroup->setText(QString("%1").
                                                arg(m_MCSDDTTMSimulParam.uiCycFrmSynCodeI,byteSubFrameSynGLenI,
                                                    16, QChar::fromLatin1('0')).toUpper());
    else
        ui->lineEdit_SubFrameSyncGroup->setText("0");
    //ID计数位置
    ui->lineEdit_IDCountPos->setText(QString::number(m_MCSDDTTMSimulParam.usIDSubFrmPosI));
    //ID计数基值
    if (m_MCSDDTTMSimulParam.ucIDBaseI == 2 || m_MCSDDTTMSimulParam.ucIDBaseI == 1)
        ui->comboBox_IDCountBaseValue->setCurrentText(m_MCSDDTTMSimulParam.ucIDBaseI == 2 ? "0" :"1");
    //ID计数方向
    if (m_MCSDDTTMSimulParam.ucIDDirectI > 0)
        ui->comboBox_IDCountDir->setCurrentIndex(m_MCSDDTTMSimulParam.ucIDDirectI - 1);
    //模拟数据
    ui->lineEdit_IAnologData->setText(QString::number(m_MCSDDTTMSimulParam.uldataI, 16).toUpper());
    //数据类型
    if (m_MCSDDTTMSimulParam.ucdataselI > 0)
        ui->comboBox_DataType->setCurrentIndex(m_MCSDDTTMSimulParam.ucdataselI - 1);

    //码型选择
    if (m_MCSDDTTMSimulParam.ucCodeStyleQ > 0)
        ui->comboBox_OCodePattern->setCurrentIndex(m_MCSDDTTMSimulParam.ucCodeStyleQ - 1);
    //码速率
    ui->lineEdit_OBitRate->setText(QString::number(m_MCSDDTTMSimulParam.usCodeRateQ / 1000.0));
    //  Q路译码选择
    if (m_MCSDDTTMSimulParam.ucCodingMdQ > 0)
    {
        if (m_MCSDDTTMSimulParam.ucCodingMdQ == 6)
            ui->comboBox_OEnctype->setCurrentIndex(4);
        else
            ui->comboBox_OEnctype->setCurrentIndex(m_MCSDDTTMSimulParam.ucCodingMdQ - 1);
    }
    //卷积码类型
    if (m_MCSDDTTMSimulParam.ucViterbiMdQ > 0)
        ui->comboBox_OConcodeType->setCurrentIndex(m_MCSDDTTMSimulParam.ucViterbiMdQ - 1);
    //RS码类型
    if (m_MCSDDTTMSimulParam.ucRSMdQ > 0)
        ui->comboBox_ORSType->setCurrentIndex(m_MCSDDTTMSimulParam.ucRSMdQ - 1);
    //RS译码坐标基
    if (m_MCSDDTTMSimulParam.ucBasiSelQ > 0)
        ui->comboBox_ORScoBase->setCurrentIndex(m_MCSDDTTMSimulParam.ucBasiSelQ - 1);
    //RS交织深度
    if (m_MCSDDTTMSimulParam.ucInterleavQ > 0)
        ui->lineEdit_ORSmixDeep->setText(QString::number(m_MCSDDTTMSimulParam.ucInterleavQ));
    //卷积输出相位
    if (m_MCSDDTTMSimulParam.ucG2InvQ > 0)
        ui->comboBox_OConoutPhase->setCurrentIndex(m_MCSDDTTMSimulParam.ucG2InvQ - 1);
    //帧长
    ui->lineEdit_OFrameLen->setText(QString::number(m_MCSDDTTMSimulParam.usFrameLenQ));
    //帧同步码组长
    ui->comboBox_OFrameSyncGroupLen->setCurrentText(QString::number(m_MCSDDTTMSimulParam.ucFrameSynGLenQ) + "bits");
    int byteFrameSynGLenQ = 8;
    if(m_MCSDDTTMSimulParam.ucFrameSynGLenQ == 16 ||
            m_MCSDDTTMSimulParam.ucFrameSynGLenQ == 24 ||
            m_MCSDDTTMSimulParam.ucFrameSynGLenQ == 32)
    {
        byteFrameSynGLenQ = m_MCSDDTTMSimulParam.ucFrameSynGLenQ/4;
    }
    //帧同步码组
    if (m_MCSDDTTMSimulParam.uiFrameSynGQ != 0)
        ui->lineEdit_OFrameSyncGroup->setText(QString("%1").
                                              arg(m_MCSDDTTMSimulParam.uiFrameSynGQ,byteFrameSynGLenQ,
                                                  16, QChar::fromLatin1('0')).toUpper());
    else
        ui->lineEdit_OFrameSyncGroup->setText("0");
    //帧同步码组位置
    if (m_MCSDDTTMSimulParam.ucFspPlaceQ > 0)
        ui->comboBox_OFrameSyncGroupPos->setCurrentIndex(m_MCSDDTTMSimulParam.ucFspPlaceQ - 1);
    //加扰
    if (m_MCSDDTTMSimulParam.ucturbCtrlQ > 0)
        ui->comboBox_OAddDisturbCtrl->setCurrentIndex(m_MCSDDTTMSimulParam.ucturbCtrlQ - 1);
    //加扰多项式长度
    if (m_MCSDDTTMSimulParam.ucRandLenQ > 0)
        ui->comboBox_ODisturbPolyLen->setCurrentIndex(m_MCSDDTTMSimulParam.ucRandLenQ - 1);
    //加扰多项式
    if (m_MCSDDTTMSimulParam.uiRandPolyQ != 0)
        ui->lineEdit_ODisturbPoly->setText(QString("%1").
                                           arg(m_MCSDDTTMSimulParam.uiRandPolyQ,m_MCSDDTTMSimulParam.ucRandLenQ * 2,
                                               16, QChar::fromLatin1('0')).toUpper());
    else
        ui->lineEdit_ODisturbPoly->setText("0");
    //加扰初相
    if (m_MCSDDTTMSimulParam.uiPhaseQ != 0)
        ui->lineEdit_ODisturbPhasePos->setText(QString("%1").
                                               arg(m_MCSDDTTMSimulParam.uiPhaseQ, m_MCSDDTTMSimulParam.ucRandLenQ * 2,
                                                   16, QChar::fromLatin1('0')).toUpper());
    else
        ui->lineEdit_ODisturbPhasePos->setText("0");
    //副帧长度
    ui->lineEdit_OSubFrameLen->setText(QString::number(m_MCSDDTTMSimulParam.ucSframeLenQ));
    //副帧方式
    if (m_MCSDDTTMSimulParam.ucSFrameSynMdQ > 0)
        ui->comboBox_OSubFrameSyncType->setCurrentIndex(m_MCSDDTTMSimulParam.ucSFrameSynMdQ - 1);
    //副帧码组长度
    int byteSubFrameSynGLenQ = 8;
    if (m_MCSDDTTMSimulParam.ucCycFrmSynCodeLenQ > 0)
    {
        ui->comboBox_OSubFrameGroupLen->setCurrentIndex(m_MCSDDTTMSimulParam.ucCycFrmSynCodeLenQ - 1);
        byteSubFrameSynGLenQ = m_MCSDDTTMSimulParam.ucCycFrmSynCodeLenQ * 2;
    }

    //副帧码组位置
    ui->lineEdit_OSubFrameGroupPos->setText(QString::number(m_MCSDDTTMSimulParam.usCycFrmSynCodePosQ));
    //副帧码组
    if (m_MCSDDTTMSimulParam.uiCycFrmSynCodeQ != 0)
        ui->lineEdit_OSubFrameSyncGroup->setText(QString("%1").
                                                 arg(m_MCSDDTTMSimulParam.uiCycFrmSynCodeQ,byteSubFrameSynGLenQ,
                                                     16, QChar::fromLatin1('0')).toUpper());
    else
        ui->lineEdit_OSubFrameSyncGroup->setText("0");
    //ID计数位置
    ui->lineEdit_OIDCountPos->setText(QString::number(m_MCSDDTTMSimulParam.usIDSubFrmPosQ));
    //ID计数基值
    if (m_MCSDDTTMSimulParam.ucIDBaseQ == 2 || m_MCSDDTTMSimulParam.ucIDBaseQ == 1)
        ui->comboBox_OIDCountBaseValue->setCurrentText(m_MCSDDTTMSimulParam.ucIDBaseQ == 2 ?  "0" : "1");
    //ID计数方向
    if (m_MCSDDTTMSimulParam.ucIDDirectQ > 0)
        ui->comboBox_OIDCountDir->setCurrentIndex(m_MCSDDTTMSimulParam.ucIDDirectQ - 1);
    //模拟数据
    ui->lineEdit_OAnologData->setText(QString::number(m_MCSDDTTMSimulParam.uldataQ, 16).toUpper());
    //数据类型
    if (m_MCSDDTTMSimulParam.ucdataselQ > 0)
        ui->comboBox_ODataType->setCurrentIndex(m_MCSDDTTMSimulParam.ucdataselQ - 1);
    relate();
}


void DDTBBE_ModSourceSetGUIDlg::SetObjInfo(TLxTsspObjectInfo ObjectInfo)
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
        //        int index;
        //        switch (m_ObjectInfo.usLocalDID) {
        //        case 0x400:
        //            index = 0;
        //            break;
        //        case 0x401:
        //            index = 1;
        //            break;
        //        default:
        //            break;
        //        }
        //SUBITEM sItem = pItem->vSubItem.at(index);
        SUBITEM& sItem = *(pItem->getSubItemByDeviceID_SN(m_ObjectInfo.usLocalDID, m_ObjectInfo.ucSN));
        m_usC = sItem.usCMD;
        m_usTID = sItem.usTID;
        /*************************框架**********************************/
        m_ParaMCSDDTTMSimulParamID = GetID(sItem.usStationID,
                                           sItem.ucSubSysID, sItem.usDeviceID,sItem.usTID,
                                           sItem.ucSN);
        /*************************框架**********************************/

        /*************************状态**********************************/
        m_StatusParaMCSDDTTMSimulParamID = GetID(sItem.usStationID,
                                                 sItem.ucSubSysID, sItem.usDeviceID,sItem.usStateType,
                                                 sItem.ucSN);
        /*************************状态**********************************/
    }

    ITEM *pItem1 = m_DeviceMap.getItem("数传接收机参数设置");

    if (pItem1 != NULL && pItem1->vSubItem.size() > 0)
    {
        SUBITEM& sItem1 = *(pItem1->getSubItemByDeviceID_SN(m_ObjectInfo.usLocalDID, m_ObjectInfo.ucSN));
        /*************************框架**********************************/
        m_ParaMCSDDTRevParamID = GetID(sItem1.usStationID,
                                       sItem1.ucSubSysID, sItem1.usDeviceID,sItem1.usTID,
                                       sItem1.ucSN);
        /*************************框架**********************************/

        /*************************状态**********************************/
        m_StatusParaMCSDDTRevParamID = GetID(sItem1.usStationID,
                                             sItem1.ucSubSysID, sItem1.usDeviceID,sItem1.usStateType,
                                             sItem1.ucSN);
        /*************************状态**********************************/
    }

    ITEM *pItem2 = m_DeviceMap.getItem("数传解调参数设置");

    if (pItem2 != NULL && pItem2->vSubItem.size() > 0)
    {
        //I路
        SUBITEM& sItem2 = *(pItem2->getSubItemByDeviceID_SN(m_ObjectInfo.usLocalDID, 0));
        /*************************框架**********************************/
        m_ParaMCSDDTTMIParamID = GetID(sItem2.usStationID,
                                       sItem2.ucSubSysID, sItem2.usDeviceID,sItem2.usTID,
                                       sItem2.ucSN);
        /*************************框架**********************************/

        /*************************状态**********************************/
        m_StatusParaMCSDDTTMIParamID = GetID(sItem2.usStationID,
                                             sItem2.ucSubSysID, sItem2.usDeviceID,sItem2.usStateType,
                                             sItem2.ucSN);
        /*************************状态**********************************/
        //Q路
        SUBITEM& sItem3 = *(pItem2->getSubItemByDeviceID_SN(m_ObjectInfo.usLocalDID, 1));
        /*************************框架**********************************/
        m_ParaMCSDDTTMQParamID = GetID(sItem3.usStationID,
                                       sItem3.ucSubSysID, sItem3.usDeviceID,sItem3.usTID,
                                       sItem3.ucSN);
        /*************************框架**********************************/

        /*************************状态**********************************/
        m_StatusParaMCSDDTTMQParamID = GetID(sItem3.usStationID,
                                             sItem3.ucSubSysID, sItem3.usDeviceID,sItem3.usStateType,
                                             sItem3.ucSN);
        /*************************状态**********************************/
    }

    //日志
    m_pILxTsspLogManager = pObjectManager->GetLogManager();
    m_pStateMgr = pObjectManager->GetStateManager();
    m_pILxTsspMessageManager = pObjectManager->GetMessageManager();
    m_pPublicInfo = pObjectManager->GetPublicInfoManager();
}

//使能或不使能控件
void DDTBBE_ModSourceSetGUIDlg::EnabledControls(bool bFlag)
{
    /*************************Group1*************************/
    //载波输出开关
    ui->comboBox_WaveSwitch->setEnabled(bFlag);
    //调试开关
    ui->comboBox_MudulatingSwitch->setEnabled(bFlag);
    //调制体制
    ui->comboBox_MudulationSystem->setEnabled(bFlag);
    //输出功率
    ui->lineEdit_OutputPower->setEnabled(false);
    //噪声输出开关
    ui->comboBox_NoiseOutSwitch->setEnabled(bFlag);
    //输出载噪比
    ui->lineEdit_NoiseRatio->setEnabled(bFlag);
    //GRAY编码选择
    ui->comboBox_GRAYType->setEnabled(bFlag);
    //I/Q分路合路
    ui->comboBox_IOMethod->setEnabled(bFlag);
    //功率有效标志位
    ui->comboBox_PowerFlag->setEnabled(bFlag);
    //I/O功率比
    ui->lineEdit_IORatio->setEnabled(bFlag);
    //i功率因子
    ui->comboBox_IPowerFactor->setEnabled(bFlag);
    //Q功率因子
    ui->comboBox_QPowerFactor->setEnabled(bFlag);
    //多普勒开关
    ui->comboBox_DopplerSwitch->setEnabled(bFlag);
    //模拟多普勒范围
    ui->lineEdit_DopplerRange->setEnabled(bFlag);//
    //模拟多普勒变化率
    ui->lineEdit_DopplerGrident->setEnabled(bFlag);
    //数据源选择
    ui->comboBox_WayOfData->setEnabled(bFlag);
    //左旋时延控制
    ui->comboBox_counterClockCtrl->setEnabled(bFlag);
    //右旋时延控制
    ui->comboBox_clockWiseCtrl->setEnabled(bFlag);
    //左旋时延大小
    ui->lineEdit_counterClockDelay->setEnabled(bFlag);
    //右旋时延大小
    ui->lineEdit_clockWiseDelay->setEnabled(bFlag);

    /*************************Group1*************************/

    /*************************Group2*************************/
    //码型
    ui->comboBox_CodePattern->setEnabled(bFlag);
    //码速率
    ui->lineEdit_BitRate->setEnabled(bFlag);
    //编码方式
    ui->comboBox_Enctype->setEnabled(bFlag);
    //RS码类型
    ui->comboBox_RSType->setEnabled(bFlag);
    //RS码坐标基
    ui->comboBox_RScoBase->setEnabled(bFlag);
    //RS交织深度
    ui->lineEdit_RSmixDeep->setEnabled(bFlag);
    //卷积码类型
    ui->comboBox_ConcodeType->setEnabled(bFlag);
    //卷积输出相位
    ui->comboBox_ConoutPhase->setEnabled(bFlag);
    //帧长
    ui->lineEdit_FrameLen->setEnabled(bFlag);
    //帧同步码组长
    ui->comboBox_FrameSyncGroupLen->setEnabled(bFlag);
    //帧同步码组
    ui->lineEdit_FrameSyncGroup->setEnabled(bFlag);
    //帧同步码组位置
    ui->comboBox_FrameSyncGroupPos->setEnabled(bFlag);
    //加扰控制
    ui->comboBox_AddDisturbCtrl->setEnabled(bFlag);
    //加扰多项式长度
    ui->comboBox_DisturbPolyLen->setEnabled(bFlag);
    //加扰多项式
    ui->lineEdit_DisturbPoly->setEnabled(bFlag);
    //加扰初相
    ui->lineEdit_DisturbPhasePos->setEnabled(bFlag);
    //副帧长
    ui->lineEdit_SubFrameLen->setEnabled(bFlag);
    //副帧同步方式
    ui->comboBox_SubFrameSyncType->setEnabled(bFlag);
    //副帧码组长度
    ui->comboBox_SubFrameGroupLen->setEnabled(bFlag);
    //副帧码组位置
    ui->lineEdit_SubFrameGroupPos->setEnabled(bFlag);
    //副帧同步码组
    ui->lineEdit_SubFrameSyncGroup->setEnabled(bFlag);
    //ID计数位置
    ui->lineEdit_IDCountPos->setEnabled(bFlag);
    //ID计数基值
    ui->comboBox_IDCountBaseValue->setEnabled(bFlag);
    //ID计数方向
    ui->comboBox_IDCountDir->setEnabled(bFlag);
    //数据模型
    ui->lineEdit_IAnologData->setEnabled(bFlag);
    //数据类型
    ui->comboBox_DataType->setEnabled(bFlag);

    /*************************Group2*************************/

    /*************************Group3*************************/
    //码型
    ui->comboBox_OCodePattern->setEnabled(bFlag);
    //码速率
    ui->lineEdit_OBitRate->setEnabled(bFlag);
    //编码方式
    ui->comboBox_OEnctype->setEnabled(bFlag);
    //卷积码类型
    ui->comboBox_OConcodeType->setEnabled(bFlag);
    //RS码类型
    ui->comboBox_ORSType->setEnabled(bFlag);
    //RS码坐标基
    ui->comboBox_ORScoBase->setEnabled(bFlag);
    //RS交织深度
    ui->lineEdit_ORSmixDeep->setEnabled(bFlag);
    //卷积输出相位
    ui->comboBox_OConoutPhase->setEnabled(bFlag);
    //帧长
    ui->lineEdit_OFrameLen->setEnabled(bFlag);
    //帧同步码组长
    ui->comboBox_OFrameSyncGroupLen->setEnabled(bFlag);
    //帧同步码组
    ui->lineEdit_OFrameSyncGroup->setEnabled(bFlag);
    //帧同步码组位置
    ui->comboBox_OFrameSyncGroupPos->setEnabled(bFlag);
    //加扰控制
    ui->comboBox_OAddDisturbCtrl->setEnabled(bFlag);
    //加扰多项式长度
    ui->comboBox_ODisturbPolyLen->setEnabled(bFlag);
    //加扰多项式
    ui->lineEdit_ODisturbPoly->setEnabled(bFlag);
    //加扰初相
    ui->lineEdit_ODisturbPhasePos->setEnabled(bFlag);
    //副帧长
    ui->lineEdit_OSubFrameLen->setEnabled(bFlag);
    //副帧同步方式
    ui->comboBox_OSubFrameSyncType->setEnabled(bFlag);
    //副帧码组长度
    ui->comboBox_OSubFrameGroupLen->setEnabled(bFlag);
    //副帧码组位置
    ui->lineEdit_OSubFrameGroupPos->setEnabled(bFlag);
    //副帧同步码组
    ui->lineEdit_OSubFrameSyncGroup->setEnabled(bFlag);
    //ID计数位置
    ui->lineEdit_OIDCountPos->setEnabled(bFlag);
    //ID计数基值
    ui->comboBox_OIDCountBaseValue->setEnabled(bFlag);
    //ID计数方向
    ui->comboBox_OIDCountDir->setEnabled(bFlag);
    //数据模型
    ui->lineEdit_OAnologData->setEnabled(bFlag);
    //数据类型
    ui->comboBox_ODataType->setEnabled(bFlag);
    //
    ui->pushButton_AutoMap->setEnabled(bFlag);
    /*************************Group3*************************/

    /*************************帧同步器*************************/
    if (bFlag)
    {
        //副帧方式控件使能
        //EnabledControlsSubFrmType(ui->comboBox_SubFrmType->currentIndex());
        ui->pushButton_Modify->setText("恢复");
        ui->pushButton_Setting->setEnabled(true);
    }
    else
    {
        ui->pushButton_Modify->setText("更改");
        ui->pushButton_Setting->setEnabled(false);
    }
}

//保存参数宏
int DDTBBE_ModSourceSetGUIDlg::SaveToMacro(ILxTsspParameterMacro* pMacroObj)
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
        byteArray.resize(sizeof(m_MCSDDTTMSimulParam));
        memcpy(byteArray.data(),&m_MCSDDTTMSimulParam,sizeof(m_MCSDDTTMSimulParam));
        pMacroObj->UpdateParameterBlock(m_ParaMCSDDTTMSimulParamID,byteArray);

        //日志信息
        TLxTsspLog tmpLog;
        QString strLog;
        tmpLog.ucWarningLevel  = 1;
        strLog += QString("%1: 保存宏成功！").arg(MODULENAME);
        CLog::addLog(strLog, 1);
    }
}
//从界面取参数
void DDTBBE_ModSourceSetGUIDlg::GetPara()
{
    bool ok;
    //载波输出开关
    m_MCSDDTTMSimulParam.ucOnline = ui->comboBox_WaveSwitch->currentIndex() + 1;
    //调制开关
    m_MCSDDTTMSimulParam.ucModu = ui->comboBox_MudulatingSwitch->currentIndex() + 1;
    //输出调制体制
    m_MCSDDTTMSimulParam.ucModesel = ui->comboBox_MudulationSystem->currentIndex() + 1;
    //SPSK星座图
    //    m_MCSDDTTMSimulParam.ucSPSK = ui->comboBox_SPSKPlanisphere->currentIndex();
    //输出功率
    //  m_MCSDDTTMSimulParam.ulPower =  ui->lineEdit_OutputPower->text();
    //噪声输出开关
    m_MCSDDTTMSimulParam.ucNoiseSw = ui->comboBox_NoiseOutSwitch->currentIndex() + 1;
    //输出载噪比
    m_MCSDDTTMSimulParam.ucCN0 = ui->lineEdit_NoiseRatio->text().toInt();
    //格雷编码选择
    m_MCSDDTTMSimulParam.ucGrayType = ui->comboBox_GRAYType->currentIndex() + 1;
    //IQ分路合路
    m_MCSDDTTMSimulParam.ucSignalSel = getucSignalSel();
    //功率比有效标志
    m_MCSDDTTMSimulParam.ucPowerType = ui->comboBox_PowerFlag->currentIndex() + 1;
    //I/O功率分贝比
    m_MCSDDTTMSimulParam.cRatiodB = ui->lineEdit_IORatio->text().toInt();
    //I路功率因子
    m_MCSDDTTMSimulParam.ucIPowRate = ui->comboBox_IPowerFactor->currentIndex() + 1;
    //Q路功率因子
    m_MCSDDTTMSimulParam.ucQPowRate = ui->comboBox_QPowerFactor->currentIndex() + 1;
    //多普勒动态模拟开关
    m_MCSDDTTMSimulParam.ucSimSw = ui->comboBox_DopplerSwitch->currentIndex() + 1;
    //多普勒变化范围
    m_MCSDDTTMSimulParam.iSimRange = ui->lineEdit_DopplerRange->text().toInt();
    //多普勒变化率
    m_MCSDDTTMSimulParam.iSimRate = ui->lineEdit_DopplerGrident->text().toInt();
    //调制信号来源
    //m_MCSDDTTMSimulParam.ucDataSource = ui->comboBox_WayOfData->currentIndex();
    //左旋时延控制
    //m_MCSDDTTMSimulParam.ucTimeDlySwL = ui->comboBox_counterClockCtrl->currentIndex();
    //右旋时延控制
    //m_MCSDDTTMSimulParam.ucTimeDlySwR = ui->comboBox_clockWiseCtrl->currentIndex();
    //左旋时延大小
    //m_MCSDDTTMSimulParam.wTimeDlySizeL = ui->lineEdit_counterClockDelay->text().toInt();
    //右旋时延大小
    //m_MCSDDTTMSimulParam.wTimeDlySizeR = ui->lineEdit_clockWiseDelay->text().toInt();


    //码型选择
    m_MCSDDTTMSimulParam.ucCodeStyleI = ui->comboBox_CodePattern->currentIndex() + 1;
    //码速率
    m_MCSDDTTMSimulParam.usCodeRateI = qRound(ui->lineEdit_BitRate->text().toDouble() * 1000.0);
    //编码选择
    if (ui->comboBox_Enctype->currentIndex() == 4)
        m_MCSDDTTMSimulParam.ucCodingMdI = 6;
    else
        m_MCSDDTTMSimulParam.ucCodingMdI = ui->comboBox_Enctype->currentIndex() + 1;
    //m_MCSDDTTMSimulParam.ucCodingMdI=6;
    //LDPC码率
    m_MCSDDTTMSimulParam.ucLDPCRatioI = 4;
    //卷积码类型
    m_MCSDDTTMSimulParam.ucViterbiMdI = ui->comboBox_ConcodeType->currentIndex() + 1;
    //RS码类型
    m_MCSDDTTMSimulParam.ucRSMdI = ui->comboBox_RSType->currentIndex() + 1;
    //RS译码坐标基
    m_MCSDDTTMSimulParam.ucBasiSelI = ui->comboBox_RScoBase->currentIndex() + 1;
    //RS交织深度
    m_MCSDDTTMSimulParam.ucInterleavI = ui->lineEdit_RSmixDeep->text().toInt();
    //卷积输出相位
    m_MCSDDTTMSimulParam.ucG2InvI = ui->comboBox_ConoutPhase->currentIndex() + 1;
    //帧长
    m_MCSDDTTMSimulParam.usFrameLenI = ui->lineEdit_FrameLen->text().toInt();
    //帧同步码组长
    m_MCSDDTTMSimulParam.ucFrameSynGLenI = 8*(ui->comboBox_FrameSyncGroupLen->currentIndex() + 2);
    //帧同步码组
    char* pTem1 = ui->lineEdit_FrameSyncGroup->text().trimmed().toLocal8Bit().data();
    m_MCSDDTTMSimulParam.uiFrameSynGI = StrToSynCode(pTem1);
    //帧同步码组位置
    m_MCSDDTTMSimulParam.ucFspPlaceI = ui->comboBox_FrameSyncGroupPos->currentIndex() + 1;
    //加扰
    m_MCSDDTTMSimulParam.ucturbCtrlI = ui->comboBox_AddDisturbCtrl->currentIndex() + 1;
    //加扰多项式长度
    m_MCSDDTTMSimulParam.ucRandLenI = ui->comboBox_DisturbPolyLen->currentIndex() + 1;
    //加扰多项式
    m_MCSDDTTMSimulParam.uiRandPolyI = ui->lineEdit_DisturbPoly->text().toUInt(&ok, 16);
    //加扰初相
    m_MCSDDTTMSimulParam.uiPhaseI = ui->lineEdit_DisturbPhasePos->text().toUInt(&ok, 16);
    //副帧长度
    m_MCSDDTTMSimulParam.ucSframeLenI = ui->lineEdit_SubFrameLen->text().toInt(&ok, 10);
    //副帧方式
    m_MCSDDTTMSimulParam.ucSFrameSynMdI = ui->comboBox_SubFrameSyncType->currentIndex() + 1;
    //副帧码组长度
    m_MCSDDTTMSimulParam.ucCycFrmSynCodeLenI = ui->comboBox_SubFrameGroupLen->currentIndex() + 1;
    //副帧码组位置
    m_MCSDDTTMSimulParam.usCycFrmSynCodePosI = ui->lineEdit_SubFrameGroupPos->text().toInt();
    //副帧码组
    char* pTem2 = ui->lineEdit_SubFrameSyncGroup->text().trimmed().toLocal8Bit().data();
    m_MCSDDTTMSimulParam.uiCycFrmSynCodeI = StrToSynCode(pTem2);
    //ID计数位置
    m_MCSDDTTMSimulParam.usIDSubFrmPosI = ui->lineEdit_IDCountPos->text().toInt();
    //ID计数基值
    UCHAR nTemp = ui->comboBox_IDCountBaseValue->currentIndex();
    nTemp == 1 ? "0" : "1";
    m_MCSDDTTMSimulParam.ucIDBaseI = nTemp;
    //ID计数方向
    m_MCSDDTTMSimulParam.ucIDDirectI = ui->comboBox_IDCountDir->currentIndex() + 1;
    //模拟数据
    m_MCSDDTTMSimulParam.uldataI = ui->lineEdit_IAnologData->text().toInt(&ok, 16);
    //数据类型
    m_MCSDDTTMSimulParam.ucdataselI = ui->comboBox_DataType->currentIndex() + 1;

    //码型选择
    m_MCSDDTTMSimulParam.ucCodeStyleQ = ui->comboBox_OCodePattern->currentIndex() + 1;
    //码速率
    m_MCSDDTTMSimulParam.usCodeRateQ = qRound(ui->lineEdit_OBitRate->text().toDouble() * 1000.0);
    //编码选择
    if (ui->comboBox_OEnctype->currentIndex() == 4)
        m_MCSDDTTMSimulParam.ucCodingMdQ = 6;
    else
        m_MCSDDTTMSimulParam.ucCodingMdQ = ui->comboBox_OEnctype->currentIndex() + 1;
    //LDPC码率
    m_MCSDDTTMSimulParam.ucLDPCRatioQ = 4;
    //卷积码类型
    m_MCSDDTTMSimulParam.ucViterbiMdQ = ui->comboBox_OConcodeType->currentIndex() + 1;
    //RS码类型
    m_MCSDDTTMSimulParam.ucRSMdQ = ui->comboBox_ORSType->currentIndex() + 1;
    //RS译码坐标基
    m_MCSDDTTMSimulParam.ucBasiSelQ = ui->comboBox_ORScoBase->currentIndex() + 1;
    //RS交织深度
    m_MCSDDTTMSimulParam.ucInterleavQ = ui->lineEdit_ORSmixDeep->text().toInt();
    //卷积输出相位
    m_MCSDDTTMSimulParam.ucG2InvQ = ui->comboBox_OConoutPhase->currentIndex() + 1;
    //帧长
    m_MCSDDTTMSimulParam.usFrameLenQ = ui->lineEdit_OFrameLen->text().toInt();
    //帧同步码组长
    m_MCSDDTTMSimulParam.ucFrameSynGLenQ = (ui->comboBox_OFrameSyncGroupLen->currentIndex() + 2) * 8;
    //帧同步码组
    char* pTem3 = ui->lineEdit_OFrameSyncGroup->text().trimmed().toLocal8Bit().data();
    m_MCSDDTTMSimulParam.uiFrameSynGQ = StrToSynCode(pTem3);
    //帧同步码组位置
    m_MCSDDTTMSimulParam.ucFspPlaceQ = ui->comboBox_OFrameSyncGroupPos->currentIndex() + 1;
    //加扰
    m_MCSDDTTMSimulParam.ucturbCtrlQ = ui->comboBox_OAddDisturbCtrl->currentIndex() + 1;
    //加扰多项式长度
    m_MCSDDTTMSimulParam.ucRandLenQ = ui->comboBox_ODisturbPolyLen->currentIndex() + 1;
    //加扰多项式
    m_MCSDDTTMSimulParam.uiRandPolyQ = ui->lineEdit_ODisturbPoly->text().toUInt(&ok, 16);
    //加扰初相
    m_MCSDDTTMSimulParam.uiPhaseQ = ui->lineEdit_ODisturbPhasePos->text().toUInt(&ok, 16);
    //副帧长度
    m_MCSDDTTMSimulParam.ucSframeLenQ = ui->lineEdit_OSubFrameLen->text().toInt();
    //副帧方式
    m_MCSDDTTMSimulParam.ucSFrameSynMdQ = ui->comboBox_OSubFrameSyncType->currentIndex() + 1;
    //副帧码组长度
    m_MCSDDTTMSimulParam.ucCycFrmSynCodeLenQ = ui->comboBox_OSubFrameGroupLen->currentIndex() + 1;
    //副帧码组位置
    m_MCSDDTTMSimulParam.usCycFrmSynCodePosQ = ui->lineEdit_OSubFrameGroupPos->text().toInt();
    //副帧码组
    char* pTem4 = ui->lineEdit_OSubFrameSyncGroup->text().trimmed().toLocal8Bit().data();
    m_MCSDDTTMSimulParam.uiCycFrmSynCodeQ = StrToSynCode(pTem4);
    //ID计数位置
    m_MCSDDTTMSimulParam.usIDSubFrmPosQ = ui->lineEdit_OIDCountPos->text().toInt();
    //ID计数基值
    m_MCSDDTTMSimulParam.ucIDBaseQ = ui->comboBox_OIDCountBaseValue->currentIndex() == 0 ? 2 : 1;
    //ID计数方向
    m_MCSDDTTMSimulParam.ucIDDirectQ = ui->comboBox_OIDCountDir->currentIndex() + 1;
    //模拟数据
    m_MCSDDTTMSimulParam.uldataQ = ui->lineEdit_OAnologData->text().toInt(&ok, 16);
    //数据类型
    m_MCSDDTTMSimulParam.ucdataselQ = ui->comboBox_ODataType->currentIndex() + 1;
}

//验证参数范围
int DDTBBE_ModSourceSetGUIDlg::CheckPara()
{
    return CheckParam() ? 1 : -1;
}

//定时器
void DDTBBE_ModSourceSetGUIDlg::timerEvent(QTimerEvent *event)
{
    ShowPara(0);
    showCurrentDlg();
    EnabledControls(false);
}

//更改参数按钮休息处理函数
void DDTBBE_ModSourceSetGUIDlg::on_pushButton_Modify_clicked()
{

    if(ui->pushButton_Modify->text() == "更改")
    {
        EnabledControls(true);
        relate();
        killTimer(m_TimerID);
        return;
    }

    if(ui->pushButton_Modify->text() == "恢复")
    {
        m_autoMap = false;
        EnabledControls(false);
        m_TimerID = startTimer(500);
        return;
    }
}

void DDTBBE_ModSourceSetGUIDlg::on_pushButton_Setting_clicked()
{
    //验证参数范围
    if(CheckPara() == 1)
    {
        //从界面取参数
        GetPara();
        //取当前设置的参数
        if(m_pHAManager != NULL)
        {
            QByteArray byteArray;
            byteArray.resize(sizeof(m_MCSDDTTMSimulParam));
            memcpy(byteArray.data(),&m_MCSDDTTMSimulParam,sizeof(MCSDDTTMSimulParamStruct));
            m_pHAManager->SetParameter(m_ParaMCSDDTTMSimulParamID,byteArray);
            SendMessage((char*)&m_MCSDDTTMSimulParam, sizeof(MCSDDTTMSimulParamStruct), m_usC, m_usTID);
        }

        //日志信息
        TLxTsspLog tmpLog;
        QString strLog;
        memset(&tmpLog, 0, sizeof(TLxTsspLog));
        tmpLog.ucWarningLevel  = 1;
        strLog += QString("%1: 设置参数成功！").arg(MODULENAME);
        CLog::addLog(strLog, 1);
        //不使能控件
        m_autoMap = false;
        EnabledControls(false);
        m_TimerID = startTimer(500);
    }
}

void DDTBBE_ModSourceSetGUIDlg::closeEvent(QCloseEvent *event)
{
    event->accept();
}

QSize DDTBBE_ModSourceSetGUIDlg::sizeHint() const
{
    return QSize(this->width(), this->height());
}

void DDTBBE_ModSourceSetGUIDlg::SendMessage(char* pData, unsigned short  usLen, unsigned short usC, unsigned short usTID)
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

DWORDLONG DDTBBE_ModSourceSetGUIDlg::StrToSynCode(const char * pStr)
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

void DDTBBE_ModSourceSetGUIDlg::on_comboBox_MudulationSystem_currentIndexChanged(int index)
{
    if (ui->pushButton_Modify->text() == "更改")
        return;
    mudulationSystem();
}

void DDTBBE_ModSourceSetGUIDlg::relate()
{
    if(ui->pushButton_Modify->text() == "更改")
    {
        return;
    }
    showCurrentDlg();
}

void DDTBBE_ModSourceSetGUIDlg::showCurrentDlg()
{
    showCurrentDlgSysParam();
    showCurrentDlgI();
    //showCurrentDlgQ()
}
void DDTBBE_ModSourceSetGUIDlg::showCurrentDlgSysParam()
{
    mudulationSystem();
    dopplerSwitch();
    noiseSwitch();
    grayType();
    dataMethod();
}

void DDTBBE_ModSourceSetGUIDlg::showCurrentDlgI()
{
    codingTypeI();
    randEnI();
}
void DDTBBE_ModSourceSetGUIDlg::showCurrentDlgQ()
{
    codingTypeQ();
    randEnQ();
}
void DDTBBE_ModSourceSetGUIDlg::mudulationSystem()
{
    int index = ui->comboBox_MudulationSystem->currentIndex();
    switch(index)
    {
    case 0:
        bpsk();
        break;
    case 1:
        qpsk();
        break;
    case 2:
        uqpsk();
        break;
    case 3:
        sqpsk();
    default:
        break;
    }
}

void DDTBBE_ModSourceSetGUIDlg::dopplerSwitch()//模拟多普勒开关
{
    if (ui->comboBox_DopplerSwitch->currentText() == "开")
    {
        ui->lineEdit_DopplerRange->setEnabled(true);
        ui->lineEdit_DopplerGrident->setEnabled(true);
    }
    else if (ui->comboBox_DopplerSwitch->currentText() == "关")
    {
        ui->lineEdit_DopplerRange->setEnabled(false);
        ui->lineEdit_DopplerGrident->setEnabled(false);
    }
}


void DDTBBE_ModSourceSetGUIDlg::noiseSwitch()//噪声开关
{
    if (ui->comboBox_NoiseOutSwitch->currentText() == "开")
    {
        ui->lineEdit_NoiseRatio->setEnabled(true);
    }
    else
    {
        ui->lineEdit_NoiseRatio->setEnabled(false);
    }
}

void DDTBBE_ModSourceSetGUIDlg::grayType()//格雷编码选择
{
    if (ui->comboBox_GRAYType->currentText() == "无格雷")
    {
        //I路码型
        ui->comboBox_CodePattern->clear();
        ui->comboBox_CodePattern->addItems(QStringList() << "NRZ-L" << "NRZ-M" << "NRZ-S");
        if (m_MCSDDTTMSimulParam.ucCodeStyleI > 0)
            ui->comboBox_CodePattern->setCurrentIndex(m_MCSDDTTMSimulParam.ucCodeStyleI - 1);
        else
            ui->comboBox_CodePattern->setCurrentIndex(0);

        ui->comboBox_OCodePattern->clear();
        ui->comboBox_OCodePattern->addItems(QStringList() << "NRZ-L" << "NRZ-M" << "NRZ-S");

        if (m_MCSDDTTMSimulParam.ucCodeStyleQ > 0)
            ui->comboBox_OCodePattern->setCurrentIndex(m_MCSDDTTMSimulParam.ucCodeStyleQ - 1);
        else
            ui->comboBox_OCodePattern->setCurrentIndex(0);
    }
    else
    {
        //I路码型
        ui->comboBox_CodePattern->clear();
        ui->comboBox_CodePattern->addItems(QStringList() << "NRZ-L");
        ui->comboBox_CodePattern->setCurrentIndex(0);

        ui->comboBox_OCodePattern->clear();
        ui->comboBox_OCodePattern->addItems(QStringList() << "NRZ-L");
        ui->comboBox_OCodePattern->setCurrentIndex(0);
    }
}

void DDTBBE_ModSourceSetGUIDlg::bpsk()
{
    m_IQSwitch = false;
    ui->groupBox_RoadQ->setEnabled(false);
    ui->groupBox_RoadI->setEnabled(true);
    ui->comboBox_IOMethod->setEnabled(true);
    ui->comboBox_IOMethod->clear();
    ui->comboBox_IOMethod->addItem("I路独立");
    ui->comboBox_IOMethod->setCurrentText("I路独立");
    ui->comboBox_GRAYType->setEnabled(false);
    ui->comboBox_GRAYType->setCurrentIndex(8);
    ui->comboBox_counterClockCtrl->setEnabled(true);
    if (ui->comboBox_counterClockCtrl->currentText() == "关")
    {
        ui->lineEdit_counterClockDelay->setEnabled(false);
    }
    else
    {
        ui->lineEdit_counterClockDelay->setEnabled(true);
    }
    ui->comboBox_clockWiseCtrl->setEnabled(false);
    ui->lineEdit_clockWiseDelay->setEnabled(false);
    ui->comboBox_PowerFlag->setEnabled(false);
    ui->lineEdit_IORatio->setEnabled(false);
    ui->comboBox_IPowerFactor->setEnabled(false);
    ui->comboBox_QPowerFactor->setEnabled(false);
}

void DDTBBE_ModSourceSetGUIDlg::qpsk()
{
    ui->groupBox_RoadQ->setEnabled(false);
    ui->groupBox_RoadI->setEnabled(true);
    ui->comboBox_IOMethod->setEnabled(true);
    if (!m_IQSwitch)
    {
        ui->comboBox_IOMethod->clear();
        ui->comboBox_IOMethod->addItems(QStringList() << "I/Q合路" << "I/Q分路");
        ui->comboBox_IOMethod->setCurrentText("I/Q合路");
    }
    else if (m_MCSDDTTMSimulParam.ucSignalSel == 1 || m_MCSDDTTMSimulParam.ucSignalSel == 2)
        ui->comboBox_IOMethod->setCurrentIndex(m_MCSDDTTMSimulParam.ucSignalSel - 1);
    m_IQSwitch = true;
    ioMethod();
    ui->comboBox_GRAYType->setEnabled(true);
    //时延
    //    ui->comboBox_counterClockCtrl->setEnabled(true);
    //    if (ui->comboBox_counterClockCtrl->currentText() == "关")
    //    {
    //        ui->lineEdit_counterClockDelay->setEnabled(false);
    //    }
    //    else
    //    {
    //        ui->lineEdit_counterClockDelay->setEnabled(true);
    //    }
    //    ui->comboBox_clockWiseCtrl->setEnabled(false);
    //    ui->lineEdit_clockWiseDelay->setEnabled(false);
    //功率方式
    ui->comboBox_PowerFlag->setEnabled(false);
    ui->lineEdit_IORatio->setEnabled(false);
    ui->comboBox_IPowerFactor->setEnabled(false);
    ui->comboBox_QPowerFactor->setEnabled(false);
}
void DDTBBE_ModSourceSetGUIDlg::sqpsk()
{
    ui->groupBox_RoadQ->setEnabled(false);
    ui->groupBox_RoadI->setEnabled(true);
    ui->comboBox_IOMethod->setEnabled(true);
    if (!m_IQSwitch)
    {
        ui->comboBox_IOMethod->clear();
        ui->comboBox_IOMethod->addItems(QStringList() << "I/Q合路" << "I/Q分路");
        ui->comboBox_IOMethod->setCurrentText("I/Q合路");
    }
    //数据输出方式，0：无效，1：I/Q合路，2：I/Q独立，3：I路单独
    else
    {
        int index = 0;
        switch(m_MCSDDTTMSimulParam.ucSignalSel)
        {
        case 1:
            index = 0;
            break;
        case 2:
            index = 1;
            break;
        default:
            index = getucSignalSel() - 1;
        }
        ui->comboBox_IOMethod->setCurrentIndex(index);
    }
    m_IQSwitch = true;
    ioMethod();
    ui->comboBox_GRAYType->setEnabled(false);
    ui->comboBox_GRAYType->setCurrentIndex(8);
    //leftTimeDelay();
    ui->comboBox_clockWiseCtrl->setEnabled(false);
    ui->lineEdit_clockWiseDelay->setEnabled(false);
    ui->comboBox_PowerFlag->setEnabled(false);
    ui->lineEdit_IORatio->setEnabled(false);
    ui->comboBox_IPowerFactor->setEnabled(false);
    ui->comboBox_QPowerFactor->setEnabled(false);

}
void DDTBBE_ModSourceSetGUIDlg::uqpsk()
{
    m_IQSwitch = false;
    ui->groupBox_RoadQ->setEnabled(true);
    ui->groupBox_RoadI->setEnabled(true);
    ui->comboBox_IOMethod->clear();
    ui->comboBox_IOMethod->setEnabled(true);
    ui->comboBox_IOMethod->addItems(QStringList() << "I/Q分路");
    ui->comboBox_IOMethod->setCurrentText("I/Q分路");
    ui->comboBox_GRAYType->setEnabled(false);
    ui->comboBox_GRAYType->setCurrentIndex(8);
    m_bCheckQ = true;
    //leftTimeDelay();
    //rightTimeDelay();
    powerFlag();
}
void DDTBBE_ModSourceSetGUIDlg::powerFlag()//功率比有效标志
{
    ui->comboBox_PowerFlag->setEnabled(true);
    if(ui->comboBox_PowerFlag->currentText() == "分贝有效")
    {
        ui->lineEdit_IORatio->setEnabled(true);
        ui->comboBox_IPowerFactor->setEnabled(false);
        ui->comboBox_QPowerFactor->setEnabled(false);
    }
    else if (ui->comboBox_PowerFlag->currentText() == "比例有效")
    {
        ui->lineEdit_IORatio->setEnabled(false);
        ui->comboBox_IPowerFactor->setEnabled(true);
        ui->comboBox_QPowerFactor->setEnabled(true);
    }
}

void DDTBBE_ModSourceSetGUIDlg::codingTypeI()//I路译码方式
{
    if (ui->comboBox_Enctype->currentText() == "关")
    {
        ui->lineEdit_FrameLen->setEnabled(true);
        ui->comboBox_FrameSyncGroupLen->setEnabled(true);
        ui->comboBox_ConcodeType->setEnabled(false);
        ui->comboBox_RSType->setEnabled(false);
        ui->comboBox_RScoBase->setEnabled(false);
        ui->lineEdit_RSmixDeep->setEnabled(false);
        ui->comboBox_ConoutPhase->setEnabled(false);
        ui->comboBox_FrameSyncGroupPos->setEnabled(true);
        subFrameSynTypeI();
    }
    else if (ui->comboBox_Enctype->currentText() == "卷积编码")
    {
        ui->lineEdit_FrameLen->setEnabled(true);
        ui->comboBox_FrameSyncGroupLen->setEnabled(true);
        ui->comboBox_RSType->setEnabled(false);
        ui->comboBox_RScoBase->setEnabled(false);
        ui->lineEdit_RSmixDeep->setEnabled(false);
        ui->comboBox_FrameSyncGroupPos->setEnabled(true);
        subFrameSynTypeI();
        viterbiPhaseI();
    }
    else if (ui->comboBox_Enctype->currentText() == "R-S编码")
    {
        ui->lineEdit_FrameLen->setEnabled(true);
        ui->comboBox_FrameSyncGroupLen->setEnabled(true);
        ui->comboBox_ConcodeType->setEnabled(false);
        ui->comboBox_ConoutPhase->setEnabled(false);
        ui->comboBox_FrameSyncGroupPos->setEnabled(false);
        ui->comboBox_FrameSyncGroupPos->setCurrentText("帧首");
        ui->groupBox_subFrame->setEnabled(false);
        rsCodingI();
    }
    else if (ui->comboBox_Enctype->currentText() == "R-S和卷积级联编码")
    {
        ui->lineEdit_FrameLen->setEnabled(true);
        ui->comboBox_FrameSyncGroupLen->setEnabled(true);
        ui->comboBox_ConcodeType->setEnabled(true);
        ui->comboBox_RSType->setEnabled(true);
        ui->comboBox_RScoBase->setEnabled(true);
        ui->lineEdit_RSmixDeep->setEnabled(true);
        ui->comboBox_ConoutPhase->setEnabled(true);
        ui->comboBox_FrameSyncGroupPos->setEnabled(false);
        ui->comboBox_FrameSyncGroupPos->setCurrentText("帧首");
        ui->groupBox_subFrame->setEnabled(false);
    }
    else if (ui->comboBox_Enctype->currentText() == "LDPC编码")//添加LDPC 20170604
    {
        ui->lineEdit_FrameLen->setText("1024");
        ui->lineEdit_FrameLen->setEnabled(false);
        ui->comboBox_FrameSyncGroupLen->setCurrentIndex(2);
        ui->comboBox_FrameSyncGroupLen->setEnabled(false);
        ui->comboBox_ConcodeType->setEnabled(false);
        ui->comboBox_RSType->setEnabled(false);
        ui->comboBox_RScoBase->setEnabled(false);
        ui->lineEdit_RSmixDeep->setEnabled(false);
        ui->comboBox_ConoutPhase->setEnabled(false);
        ui->comboBox_FrameSyncGroupPos->setEnabled(false);
        ui->comboBox_FrameSyncGroupPos->setCurrentText("帧首");
        ui->groupBox_subFrame->setEnabled(false);
    }
}

void DDTBBE_ModSourceSetGUIDlg::codingTypeQ()//Q路译码方式
{
    if (ui->comboBox_OEnctype->currentText() == "关")
    {
        ui->lineEdit_OFrameLen->setEnabled(true);
        ui->comboBox_OFrameSyncGroupLen->setEnabled(true);
        ui->comboBox_OConcodeType->setEnabled(false);
        ui->comboBox_ORSType->setEnabled(false);
        ui->comboBox_ORScoBase->setEnabled(false);
        ui->lineEdit_ORSmixDeep->setEnabled(false);
        ui->comboBox_OConoutPhase->setEnabled(false);
        ui->comboBox_OFrameSyncGroupPos->setEnabled(true);
        subFrameSynTypeQ();
    }
    else if (ui->comboBox_OEnctype->currentText() == "卷积编码")
    {
        ui->lineEdit_OFrameLen->setEnabled(true);
        ui->comboBox_OFrameSyncGroupLen->setEnabled(true);
        ui->comboBox_ORSType->setEnabled(false);
        ui->comboBox_ORScoBase->setEnabled(false);
        ui->lineEdit_ORSmixDeep->setEnabled(false);
        ui->comboBox_OFrameSyncGroupPos->setEnabled(true);
        ui->comboBox_OFrameSyncGroupPos->setEnabled(true);
        subFrameSynTypeQ();
        viterbiPhaseQ();
    }
    else if (ui->comboBox_OEnctype->currentText() == "R-S编码")
    {
        ui->lineEdit_OFrameLen->setEnabled(true);
        ui->comboBox_OFrameSyncGroupLen->setEnabled(true);
        ui->comboBox_OConcodeType->setEnabled(false);
        ui->comboBox_OConoutPhase->setEnabled(false);
        ui->comboBox_OFrameSyncGroupPos->setEnabled(false);
        ui->comboBox_OFrameSyncGroupPos->setCurrentText("帧首");
        ui->groupBox_OSubFrame->setEnabled(false);
        rsCodingQ();
    }
    else if (ui->comboBox_OEnctype->currentText() == "R-S和卷积级联编码")
    {
        ui->lineEdit_OFrameLen->setEnabled(true);
        ui->comboBox_OFrameSyncGroupLen->setEnabled(true);
        ui->comboBox_OConcodeType->setEnabled(true);
        ui->comboBox_ORSType->setEnabled(true);
        ui->comboBox_ORScoBase->setEnabled(true);
        ui->lineEdit_ORSmixDeep->setEnabled(true);
        ui->comboBox_OConoutPhase->setEnabled(true);
        ui->comboBox_OFrameSyncGroupPos->setEnabled(false);
        ui->comboBox_OFrameSyncGroupPos->setCurrentText("帧首");
        ui->groupBox_OSubFrame->setEnabled(false);
    }
    else if (ui->comboBox_OEnctype->currentText() == "LDPC编码")//添加LDPC 20170604
    {
        ui->lineEdit_OFrameLen->setText("1024");
        ui->lineEdit_OFrameLen->setEnabled(false);
        ui->comboBox_OFrameSyncGroupLen->setCurrentIndex(2);
        ui->comboBox_OFrameSyncGroupLen->setEnabled(false);
        ui->comboBox_OConcodeType->setEnabled(false);
        ui->comboBox_ORSType->setEnabled(false);
        ui->comboBox_ORScoBase->setEnabled(false);
        ui->lineEdit_ORSmixDeep->setEnabled(false);
        ui->comboBox_OConoutPhase->setEnabled(false);
        ui->comboBox_OFrameSyncGroupPos->setEnabled(false);
        ui->comboBox_OFrameSyncGroupPos->setCurrentText("帧首");
        ui->groupBox_OSubFrame->setEnabled(false);
    }
}

void DDTBBE_ModSourceSetGUIDlg::randEnI()//加扰控制
{
    if (ui->comboBox_AddDisturbCtrl->currentText() == "人工设置")
    {
        ui->comboBox_DisturbPolyLen->setEnabled(true);
        ui->lineEdit_DisturbPoly->setEnabled(true);
        ui->lineEdit_DisturbPhasePos->setEnabled(true);
    }
    else
    {
        ui->comboBox_DisturbPolyLen->setEnabled(false);
        ui->lineEdit_DisturbPoly->setEnabled(false);
        ui->lineEdit_DisturbPhasePos->setEnabled(false);
    }
}

void DDTBBE_ModSourceSetGUIDlg::randEnQ()//加扰控制
{
    if (ui->comboBox_OAddDisturbCtrl->currentText() == "人工设置")
    {
        ui->comboBox_ODisturbPolyLen->setEnabled(true);
        ui->lineEdit_ODisturbPoly->setEnabled(true);
        ui->lineEdit_ODisturbPhasePos->setEnabled(true);
    }
    else
    {
        ui->comboBox_ODisturbPolyLen->setEnabled(false);
        ui->lineEdit_ODisturbPoly->setEnabled(false);
        ui->lineEdit_ODisturbPhasePos->setEnabled(false);
    }
}

void DDTBBE_ModSourceSetGUIDlg::subFrameSynTypeI()//副帧方式
{
    ui->groupBox_subFrame->setEnabled(true);
    ui->comboBox_SubFrameSyncType->setEnabled(true);
    if (ui->comboBox_SubFrameSyncType->currentText() == "ID副帧")
    {
        ui->lineEdit_SubFrameLen->setEnabled(true);
        ui->lineEdit_IDCountPos->setEnabled(true);
        ui->comboBox_IDCountBaseValue->setEnabled(true);
        ui->comboBox_IDCountDir->setEnabled(true);
        ui->comboBox_SubFrameGroupLen->setEnabled(false);
        ui->lineEdit_SubFrameGroupPos->setEnabled(false);
        ui->lineEdit_SubFrameSyncGroup->setEnabled(false);
    }
    else if (ui->comboBox_SubFrameSyncType->currentText() == "无副帧")
    {
        ui->lineEdit_SubFrameLen->setEnabled(false);
        ui->lineEdit_IDCountPos->setEnabled(false);
        ui->comboBox_IDCountBaseValue->setEnabled(false);
        ui->comboBox_IDCountDir->setEnabled(false);
        ui->comboBox_SubFrameGroupLen->setEnabled(false);
        ui->lineEdit_SubFrameGroupPos->setEnabled(false);
        ui->lineEdit_SubFrameSyncGroup->setEnabled(false);
    }
    else if (ui->comboBox_SubFrameSyncType->currentText() == "循环副帧")
    {
        ui->lineEdit_SubFrameLen->setEnabled(true);
        ui->lineEdit_IDCountPos->setEnabled(false);
        ui->comboBox_IDCountBaseValue->setEnabled(false);
        ui->comboBox_IDCountDir->setEnabled(false);
        ui->comboBox_SubFrameGroupLen->setEnabled(true);
        ui->lineEdit_SubFrameGroupPos->setEnabled(true);
        ui->lineEdit_SubFrameSyncGroup->setEnabled(true);
    }
    else if (ui->comboBox_SubFrameSyncType->currentText() == "反码副帧")
    {
        ui->lineEdit_SubFrameLen->setEnabled(true);
        ui->lineEdit_IDCountPos->setEnabled(false);
        ui->comboBox_IDCountBaseValue->setEnabled(false);
        ui->comboBox_IDCountDir->setEnabled(false);
        ui->comboBox_SubFrameGroupLen->setEnabled(false);
        ui->lineEdit_SubFrameGroupPos->setEnabled(false);
        ui->lineEdit_SubFrameSyncGroup->setEnabled(false);
    }
}

void DDTBBE_ModSourceSetGUIDlg::subFrameSynTypeQ()//副帧方式
{
    ui->groupBox_OSubFrame->setEnabled(true);
    ui->comboBox_OSubFrameSyncType->setEnabled(true);
    if (ui->comboBox_OSubFrameSyncType->currentText() == "ID副帧")
    {
        ui->lineEdit_OSubFrameLen->setEnabled(true);
        ui->lineEdit_OIDCountPos->setEnabled(true);
        ui->comboBox_OIDCountBaseValue->setEnabled(true);
        ui->comboBox_OIDCountDir->setEnabled(true);
        ui->comboBox_OSubFrameGroupLen->setEnabled(false);
        ui->lineEdit_OSubFrameGroupPos->setEnabled(false);
        ui->lineEdit_OSubFrameSyncGroup->setEnabled(false);
    }
    else if (ui->comboBox_OSubFrameSyncType->currentText() == "无副帧")
    {
        ui->lineEdit_OSubFrameLen->setEnabled(false);
        ui->lineEdit_OIDCountPos->setEnabled(false);
        ui->comboBox_OIDCountBaseValue->setEnabled(false);
        ui->comboBox_OIDCountDir->setEnabled(false);
        ui->comboBox_OSubFrameGroupLen->setEnabled(false);
        ui->lineEdit_OSubFrameGroupPos->setEnabled(false);
        ui->lineEdit_OSubFrameSyncGroup->setEnabled(false);
    }
    else if (ui->comboBox_OSubFrameSyncType->currentText() == "循环副帧")
    {
        ui->lineEdit_OSubFrameLen->setEnabled(true);
        ui->lineEdit_OIDCountPos->setEnabled(false);
        ui->comboBox_OIDCountBaseValue->setEnabled(false);
        ui->comboBox_OIDCountDir->setEnabled(false);
        ui->comboBox_OSubFrameGroupLen->setEnabled(true);
        ui->lineEdit_OSubFrameGroupPos->setEnabled(true);
        ui->lineEdit_OSubFrameSyncGroup->setEnabled(true);
    }
    else if (ui->comboBox_OSubFrameSyncType->currentText() == "反码副帧")
    {
        ui->lineEdit_OSubFrameLen->setEnabled(true);
        ui->lineEdit_OIDCountPos->setEnabled(false);
        ui->comboBox_OIDCountBaseValue->setEnabled(false);
        ui->comboBox_OIDCountDir->setEnabled(false);
        ui->comboBox_OSubFrameGroupLen->setEnabled(false);
        ui->lineEdit_OSubFrameGroupPos->setEnabled(false);
        ui->lineEdit_OSubFrameSyncGroup->setEnabled(false);
    }
}


void DDTBBE_ModSourceSetGUIDlg::ioMethod()
{
    if (ui->comboBox_IOMethod->currentText() == "I/Q合路")
    {
        ui->groupBox_RoadQ->setEnabled(false);
        m_bCheckQ = FALSE;
    }
    else if (ui->comboBox_IOMethod->currentText() == "I/Q分路")
    {
        m_bCheckQ = TRUE;
        ui->groupBox_RoadQ->setEnabled(true);
        showCurrentDlgQ();
        rightTimeDelay();
    }
}

void DDTBBE_ModSourceSetGUIDlg::leftTimeDelay()//左延时控制
{
    ui->comboBox_counterClockCtrl->setEnabled(true);
    if (ui->comboBox_counterClockCtrl->currentText() == "开")
    {
        ui->lineEdit_counterClockDelay->setEnabled(true);
    }
    else
    {
        ui->lineEdit_counterClockDelay->setEnabled(false);
    }
}

void DDTBBE_ModSourceSetGUIDlg::rightTimeDelay()//右延时控制
{
    ui->comboBox_clockWiseCtrl->setEnabled(true);
    if (ui->comboBox_clockWiseCtrl->currentText() == "开")
    {
        ui->lineEdit_clockWiseDelay->setEnabled(true);
    }
    else
    {
        ui->lineEdit_clockWiseDelay->setEnabled(false);
    }
}

void DDTBBE_ModSourceSetGUIDlg::dataMethod()//数据源类型
{
    ui->comboBox_WayOfData->currentText() == "内调制";
    ui->comboBox_DataType->setEnabled(true);
    ui->comboBox_ODataType->setEnabled(true);
    simulDataI();
    simulDataQ();
}

void DDTBBE_ModSourceSetGUIDlg::simulDataI()//数据类型
{
    if (ui->comboBox_DataType->currentText() == "固定数")
    {
        ui->lineEdit_IAnologData->setEnabled(true);
    }
    else
    {
        ui->lineEdit_IAnologData->setEnabled(false);
    }
}

void DDTBBE_ModSourceSetGUIDlg::simulDataQ()//数据类型
{
    if (ui->comboBox_ODataType->currentText() == "固定数")
    {
        ui->lineEdit_OAnologData->setEnabled(true);
    }
    else
    {
        ui->lineEdit_OAnologData->setEnabled(false);
    }
}

void DDTBBE_ModSourceSetGUIDlg::viterbiPhaseI()//卷积译码
{
    ui->comboBox_ConcodeType->setEnabled(true);
    if (ui->comboBox_ConcodeType->currentText() == "(7,1/2)")
    {
        ui->comboBox_ConoutPhase->setEnabled(true);
    }
    else if (ui->comboBox_ConcodeType->currentText() == "(7,3/4)")
    {
        ui->comboBox_ConoutPhase->setEnabled(false);
        ui->comboBox_ConoutPhase->setCurrentText("正相");
    }
}

void DDTBBE_ModSourceSetGUIDlg::viterbiPhaseQ()//卷积译码
{
    ui->comboBox_OConcodeType->setEnabled(true);
    if (ui->comboBox_OConcodeType->currentText() == "(7,1/2)")
    {
        ui->comboBox_OConoutPhase->setEnabled(true);
    }
    else if (ui->comboBox_OConcodeType->currentText() == "(7,3/4)")
    {
        ui->comboBox_OConoutPhase->setEnabled(false);
        ui->comboBox_OConoutPhase->setCurrentText("正相");
    }
}

void DDTBBE_ModSourceSetGUIDlg::rsCodingI()//rs译码
{
    ui->comboBox_RSType->setEnabled(true);
    ui->comboBox_RScoBase->setEnabled(true);
    ui->lineEdit_RSmixDeep->setEnabled(true);
}

void DDTBBE_ModSourceSetGUIDlg::rsCodingQ()//rs译码
{
    ui->comboBox_ORSType->setEnabled(true);
    ui->comboBox_ORScoBase->setEnabled(true);
    ui->lineEdit_ORSmixDeep->setEnabled(true);
}

//槽函数
void DDTBBE_ModSourceSetGUIDlg::on_comboBox_DopplerSwitch_currentIndexChanged(int index)
{
    if (ui->pushButton_Modify->text() == "更改")
        return;
    dopplerSwitch();
}

void DDTBBE_ModSourceSetGUIDlg::on_comboBox_NoiseOutSwitch_currentIndexChanged(int index)
{
    if (ui->pushButton_Modify->text() == "更改")
        return;
    noiseSwitch();
}


void DDTBBE_ModSourceSetGUIDlg::on_comboBox_counterClockCtrl_currentIndexChanged(int index)
{
    if (ui->pushButton_Modify->text() == "更改")
        return;
    leftTimeDelay();
}

void DDTBBE_ModSourceSetGUIDlg::on_comboBox_clockWiseCtrl_currentIndexChanged(int index)
{
    if (ui->pushButton_Modify->text() == "更改")
        return;
    rightTimeDelay();
}


void DDTBBE_ModSourceSetGUIDlg::on_comboBox_WayOfData_currentIndexChanged(int index)
{
    if (ui->pushButton_Modify->text() == "更改")
        return;
    dataMethod();
}

void DDTBBE_ModSourceSetGUIDlg::on_comboBox_DataType_currentIndexChanged(int index)
{
    if (ui->pushButton_Modify->text() == "更改")
        return;
    simulDataI();
}

void DDTBBE_ModSourceSetGUIDlg::on_comboBox_ODataType_currentIndexChanged(int index)
{
    if (ui->pushButton_Modify->text() == "更改")
        return;
    simulDataQ();
}

void DDTBBE_ModSourceSetGUIDlg::on_comboBox_Enctype_currentIndexChanged(int index)
{
    if (ui->pushButton_Modify->text() == "更改")
        return;
    codingTypeI();
}

void DDTBBE_ModSourceSetGUIDlg::on_comboBox_ConcodeType_currentIndexChanged(int index)
{
    if (ui->pushButton_Modify->text() == "更改")
        return;
    viterbiPhaseI();
}

void DDTBBE_ModSourceSetGUIDlg::on_comboBox_AddDisturbCtrl_currentIndexChanged(int index)
{
    if (ui->pushButton_Modify->text() == "更改")
        return;
    randEnI();
}

void DDTBBE_ModSourceSetGUIDlg::on_comboBox_SubFrameSyncType_currentIndexChanged(int index)
{
    if (ui->pushButton_Modify->text() == "更改")
        return;
    subFrameSynTypeI();
}



void DDTBBE_ModSourceSetGUIDlg::on_comboBox_OEnctype_currentIndexChanged(int index)
{
    if (ui->pushButton_Modify->text() == "更改")
        return;
    codingTypeQ();
}

void DDTBBE_ModSourceSetGUIDlg::on_comboBox_OConcodeType_currentIndexChanged(int index)
{
    if (ui->pushButton_Modify->text() == "更改")
        return;
    viterbiPhaseQ();
}

void DDTBBE_ModSourceSetGUIDlg::on_comboBox_OAddDisturbCtrl_currentIndexChanged(int index)
{
    if (ui->pushButton_Modify->text() == "更改")
        return;
    randEnQ();
}

void DDTBBE_ModSourceSetGUIDlg::on_comboBox_OSubFrameSyncType_currentIndexChanged(int index)
{
    if (ui->pushButton_Modify->text() == "更改")
        return;
    subFrameSynTypeQ();
}

void DDTBBE_ModSourceSetGUIDlg::on_comboBox_IOMethod_currentIndexChanged(int index)
{
    if (ui->pushButton_Modify->text() == "更改" || -1 == index)
        return;
    ioMethod();
}

void DDTBBE_ModSourceSetGUIDlg::on_comboBox_PowerFlag_currentIndexChanged(int index)
{
    if (ui->pushButton_Modify->text() == "更改")
        return;
    powerFlag();
}

void DDTBBE_ModSourceSetGUIDlg::on_comboBox_GRAYType_currentIndexChanged(int index)
{
    if (ui->pushButton_Modify->text() == "更改")
        return;
    grayType();
}

void DDTBBE_ModSourceSetGUIDlg::on_pushButton_AutoMap_clicked()
{
    m_autoMap = true;
    ShowPara(0);
}

UCHAR DDTBBE_ModSourceSetGUIDlg::getucSignalSel()
{
    switch (ui->comboBox_MudulationSystem->currentIndex())
    {
    case 0:
        return 3;
    case 1:
    case 3:
        return ui->comboBox_IOMethod->currentIndex() + 1;
    case 2:
        return 2;
    default:
        return 1;//如果出错
    }
}

int DDTBBE_ModSourceSetGUIDlg::setIOMethod()
{
    //调制体制：0：无效，1：BPSK，2：QPSK，3：UQPSK，4：SQPSK
    switch (m_MCSDDTTMSimulParam.ucModesel)
    {
    case 0:
    case 1:
    case 3:
        return 0;
    default:
        return m_MCSDDTTMSimulParam.ucSignalSel - 1;     //数据输出方式，0：无效，1：I/Q合路，2：I/Q独立，3：I路单独
    }
}
