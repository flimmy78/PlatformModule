#include "USBBBE_CodeTMEmuSetGUIDlg.h"
#include "ui_USBBBE_CodeTMEmuSetGUIDlg.h"
#include <QCloseEvent>
#include <QMessageBox>
#include <QDateTime>
#include "../../Common/Log.h"

extern quint64 GetID(ushort usStationID, uchar ucSubSysID, ushort usDeviceID, ushort usType,
                     uchar ucSn);
USBBBE_CodeTMEmuSetGUIDlg::USBBBE_CodeTMEmuSetGUIDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::USBBBE_CodeTMEmuSetGUIDlg)
{
    ui->setupUi(this);
    m_pStateMgr = NULL;
    m_pILxTsspMessageManager = NULL;
    m_pILxTsspLogManager = NULL;
    m_pPublicInfo = NULL;
    m_pHAManager = NULL;
    m_MCSUSBTMSimulStatus = NULL;
    m_TMStatuID = 0;
    m_autoMap = false;
    setAttribute(Qt::WA_DeleteOnClose);
}

USBBBE_CodeTMEmuSetGUIDlg::~USBBBE_CodeTMEmuSetGUIDlg()
{
    killTimer(m_TimerID);
    delete ui;
}

void USBBBE_CodeTMEmuSetGUIDlg::setText(const QString& strTitle, ParaStruct xmlsetText[3][60], QString MapMean[50][30], int MapNum[50], uchar Type, ILxTsspParameterMacro* pMacroObj)
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
void USBBBE_CodeTMEmuSetGUIDlg::InitDLG(int Type, ILxTsspParameterMacro* pMacroObj)
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
            m_pHAManager->GetParameter(m_ParaMCSUSBTMSimulID, byteArray);
            memcpy(&m_MCSUSBTMSimulParam, byteArray.data(), sizeof(MCSUSBTMSimulParamStruct));
        }
        if(m_pStateMgr != NULL)
        {
            DWORD stLen = sizeof(MCSUSBTMSimulStatusStruct);
            m_MCSUSBTMSimulStatus = (MCSUSBTMSimulStatusStruct*)m_pStateMgr->FindOneItem(m_StatusParaMCSUSBTMSimulID, stLen);
            if(m_MCSUSBTMSimulStatus == NULL || stLen != sizeof(MCSUSBTMSimulStatusStruct))
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
            pMacroObj->GetParameterBlock(m_ParaMCSUSBTMSimulID, byteArray);
            memcpy(&m_MCSUSBTMSimulParam, byteArray.data(), sizeof(MCSUSBTMSimulParamStruct));
        }
        ui->pushButton_Modify->setVisible(false);
        ui->pushButton_Set->setVisible(false);
        ui->lineEdit_WordLen->setEnabled(false);
        //2017/07/27 gzl 添加自动映射按钮隐藏
        ui->pushButton_autoMap->setVisible(false);
        //显示参数
        ShowPara(1);
    }
}

//设置输入格式
void USBBBE_CodeTMEmuSetGUIDlg::InitInput()
{
    //正整数
    QRegExp regExp("[0-9]{1,16}");
    QRegExp regExp0("[0-9\\.]{1,7}");
    //副载波频率
    ui->lineEdit_SubWaveFr->setValidator(new QRegExpValidator(regExp0, this));
    //交织深度
     QRegExp regExp_RSDecodeDeep("[1-5]{1}");
    ui->lineEdit_RSDecodeDeep->setValidator(new QRegExpValidator(regExp_RSDecodeDeep, this));
    //码速率
    QRegExp regExp_CodeRate("[0-9]{1,5}");
    ui->lineEdit_CodeRate->setValidator(new QRegExpValidator(regExp_CodeRate, this));
    //字长
     QRegExp regExp_WordLen("[0-9]{1,2}");
    ui->lineEdit_WordLen->setValidator(new QRegExpValidator(regExp_WordLen, this));
    //帧长
    QRegExp regExp_FrameLen("[0-9]{1,4}");
    ui->lineEdit_FrameLen->setValidator(new QRegExpValidator(regExp_FrameLen, this));
    //解扰寄存器初相
    ui->lineEdit_RandPhase->setValidator(new QRegExpValidator(regExp, this));
    //ID字位置
    ui->lineEdit_IDCountPos->setValidator(new QRegExpValidator(regExp_FrameLen, this));
    //帧格式
    QRegExp regExp_FormatLen("[0-9]{1,3}");
    ui->lineEdit_FormatLen->setValidator(new QRegExpValidator(regExp_FormatLen, this));
    //副帧码组位置
    ui->lineEdit_SubFrmSynGroupPos->setValidator(new QRegExpValidator(regExp_FrameLen, this));

    //字母数字
    QRegExp regExp2("[A-Fa-f0-9]{1,8}");
    //副帧同步码组
    ui->lineEdit_SubFrmGroup->setValidator(new QRegExpValidator(regExp2, this));
    //帧同步码组
    ui->lineEdit_FrmSyncGroup->setValidator(new QRegExpValidator(regExp2, this));
    //解扰多项式
    ui->lineEdit_RandPoly->setValidator(new QRegExpValidator(regExp2, this));
    //固定数
    QRegExp regExp_ConstSetting("[A-Fa-f0-9]{1,2}");
    ui->lineEdit_ConstSetting->setValidator(new QRegExpValidator(regExp_ConstSetting, this));
    //加扰初相(Hex)
    ui->lineEdit_RandPhase->setValidator(new QRegExpValidator(regExp2, this));
}

void USBBBE_CodeTMEmuSetGUIDlg::ShowControls()
{
    //向下拉框控件中写入从xml中读取的内容
    int i;
    //帧同步码位置
    for(i=0; i<DlgMapLineNum[0];i++)
    {
        ui->comboBox_FrmPlace->addItem(m_DlgMapMean[0][i]);
    }
    ui->comboBox_FrmPlace->setCurrentIndex(m_DlgParaSetStruct[0][0].InitVal.lval);
    //副载波频率
    ui->lineEdit_SubWaveFr->setText(QString::number(m_DlgParaSetStruct[0][1].InitVal.lval));
    //码型
    for(i=0; i<DlgMapLineNum[1];i++)
    {
        ui->comboBox_CodeType->addItem(m_DlgMapMean[1][i]);
    }
    ui->comboBox_CodeType->setCurrentIndex(m_DlgParaSetStruct[0][2].InitVal.lval);
    //码速率
    ui->lineEdit_CodeRate->setText(QString::number(m_DlgParaSetStruct[0][3].InitVal.lval));
    //编码方式
    for(i=0; i<DlgMapLineNum[2];i++)
    {
        ui->comboBox_DecodeType->addItem(m_DlgMapMean[2][i]);
    }
    ui->comboBox_DecodeType->setCurrentIndex(m_DlgParaSetStruct[0][4].InitVal.lval);
    //卷积码类型
    for(i=0; i<DlgMapLineNum[3];i++)
    {
        ui->comboBox_RecurrentCodeType->addItem(m_DlgMapMean[3][i]);
    }
    ui->comboBox_RecurrentCodeType->setCurrentIndex(m_DlgParaSetStruct[0][5].InitVal.lval);
    //卷积输出相位
    for(i=0; i<DlgMapLineNum[4];i++)
    {
        ui->comboBox_OutputPhase->addItem(m_DlgMapMean[4][i]);
    }
    ui->comboBox_OutputPhase->setCurrentIndex(m_DlgParaSetStruct[0][6].InitVal.lval);
    //R-S码类型
    for(i=0; i<DlgMapLineNum[5];i++)
    {
        ui->comboBox_RSType->addItem(m_DlgMapMean[5][i]);
    }
    ui->comboBox_RSType->setCurrentIndex(m_DlgParaSetStruct[0][7].InitVal.lval);
    //R-S编码坐标基
    for(i=0; i<DlgMapLineNum[6];i++)
    {
        ui->comboBox_RSEncodePlot->addItem(m_DlgMapMean[6][i]);
    }
    ui->comboBox_RSEncodePlot->setCurrentIndex(m_DlgParaSetStruct[0][8].InitVal.lval);
    //交织深度
    ui->lineEdit_RSDecodeDeep->setText(QString::number(m_DlgParaSetStruct[0][9].InitVal.lval));
    //数据源选择
    for(i=0; i<DlgMapLineNum[7];i++)
    {
        ui->comboBox_DataSource->addItem(m_DlgMapMean[7][i]);
    }
    ui->comboBox_DataSource->setCurrentIndex(m_DlgParaSetStruct[0][10].InitVal.lval);
    //数据源类型
    for(i=0; i<DlgMapLineNum[8];i++)
    {
        ui->comboBox_DataSourceType->addItem(m_DlgMapMean[8][i]);
    }
    ui->comboBox_DataSourceType->setCurrentIndex(m_DlgParaSetStruct[0][11].InitVal.lval);
    //固定数设置
    ui->lineEdit_ConstSetting->setText(QString::number(m_DlgParaSetStruct[0][12].InitVal.lval));
    //字长
    ui->lineEdit_WordLen->setText(QString::number(m_DlgParaSetStruct[0][13].InitVal.lval));
    //帧长
    ui->lineEdit_FrameLen->setText(QString::number(m_DlgParaSetStruct[0][14].InitVal.lval));
    //格式长
    ui->lineEdit_FormatLen->setText(QString::number(m_DlgParaSetStruct[0][15].InitVal.lval));
    //帧同步码组长
    for(i=0; i<DlgMapLineNum[9];i++)
    {
        ui->comboBox_FrameSyncGroupLen->addItem(m_DlgMapMean[9][i]);
    }
    ui->comboBox_FrameSyncGroupLen->setCurrentIndex(m_DlgParaSetStruct[0][16].InitVal.lval);
    //帧同步码组
    ui->lineEdit_FrmSyncGroup->setText(QString::number(m_DlgParaSetStruct[0][17].InitVal.lval,16).toUpper());
    //加扰使能
    for(i=0; i<DlgMapLineNum[10];i++)
    {
        ui->comboBox_RandEn->addItem(m_DlgMapMean[10][i]);
    }
    ui->comboBox_RandEn->setCurrentIndex(m_DlgParaSetStruct[0][18].InitVal.lval);
    //加扰多项式长度
    for(i=0; i<DlgMapLineNum[11];i++)
    {
        ui->comboBox_RandPolyLen->addItem(m_DlgMapMean[11][i]);
    }
    ui->comboBox_RandPolyLen->setCurrentIndex(m_DlgParaSetStruct[0][19].InitVal.lval);
    //解扰多项式
    ui->lineEdit_RandPoly->setText(QString::number(m_DlgParaSetStruct[0][20].InitVal.lval,16).toUpper());
    //加扰初相
    ui->lineEdit_RandPhase->setText(QString::number(m_DlgParaSetStruct[0][21].InitVal.lval,16).toUpper());
    //副帧同步方式
    for(i=0; i<DlgMapLineNum[12]; i++)
    {
        ui->comboBox_SubFrmSyncType->addItem(m_DlgMapMean[12][i]);
    }
    ui->comboBox_SubFrmSyncType->setCurrentIndex(m_DlgParaSetStruct[0][22].InitVal.lval);
    //副帧码组长度
    for(i=0; i<DlgMapLineNum[13];i++)
    {
        ui->comboBox_SubFrameGroupLen->addItem(m_DlgMapMean[13][i]);
    }
    ui->comboBox_SubFrameGroupLen->setCurrentIndex(m_DlgParaSetStruct[0][23].InitVal.lval);
    //副帧码组位置
    ui->lineEdit_SubFrmSynGroupPos->setText(QString::number(m_DlgParaSetStruct[0][24].InitVal.lval));
    //副帧同步码组
    ui->lineEdit_SubFrmGroup->setText(QString::number(m_DlgParaSetStruct[0][25].InitVal.lval,16).toUpper());
    //ID字位置
    ui->lineEdit_IDCountPos->setText(QString::number(m_DlgParaSetStruct[0][26].InitVal.lval));
    //ID字基值
    for(i=0; i<DlgMapLineNum[14]; i++)
    {
        ui->comboBox_IDBaseValue->addItem(m_DlgMapMean[14][i]);
    }
    ui->comboBox_IDBaseValue->setCurrentIndex(m_DlgParaSetStruct[0][27].InitVal.lval);
    //ID计数方向
    for(i=0; i<DlgMapLineNum[15]; i++)
    {
        ui->comboBox_idDirect->addItem(m_DlgMapMean[15][i]);
    }
    ui->comboBox_idDirect->setCurrentIndex(m_DlgParaSetStruct[0][28].InitVal.lval);

    /**********************************参数**********************************/
    //设置label参数显示信息
    QString strTemp;
    //副载波频率
    strTemp = "副载波频率\n[" + QString::number(m_DlgParaSetStruct[0][1].MinVal.lmin / 1000 )+"~"+
            QString::number(m_DlgParaSetStruct[0][1].MaxVal.lmax / 1000) + "]kHz";
    ui->label_SubWaveFr->setText(strTemp);
    //交织深度
    strTemp = "交织深度[" + QString::number(m_DlgParaSetStruct[0][9].MinVal.lmin)+"~"+
            QString::number(m_DlgParaSetStruct[0][9].MaxVal.lmax)  + "]";
    ui->label_RSDecodeDeep->setText(strTemp);
//    //码速率
//    strTemp = "码速率\n[" + QString::number(m_DlgParaSetStruct[0][3].MinVal.lmin)+"~"+
//            QString::number(m_DlgParaSetStruct[0][3].MaxVal.lmax) + "]bps";
//    ui->label_CodeRate->setText(strTemp);
    //字长
//    strTemp = "字长[" + QString::number(m_DlgParaSetStruct[0][13].MinVal.lmin)+"~"+
//            QString::number(m_DlgParaSetStruct[0][13].MaxVal.lmax) + "]位";
//    ui->label_WordLen->setText(strTemp);
    //帧长
//    strTemp = "帧长\n[" + QString::number(m_DlgParaSetStruct[0][14].MinVal.lmin)+"~"+
//            QString::number(m_DlgParaSetStruct[0][14].MaxVal.lmax) + "]";//字";
//    ui->label_FrameLen->setText(strTemp);
    //格式长
//    strTemp = "格式长\n[" + QString::number(m_DlgParaSetStruct[0][15].MinVal.lmin)+"~"+
//            QString::number(m_DlgParaSetStruct[0][15].MaxVal.lmax) + "]";
//    ui->label_FormatLen->setText(strTemp);
    //ID字位置
//    strTemp = "ID字位置[" + QString::number(m_DlgParaSetStruct[0][26].MinVal.lmin)+"~"+
//            QString::number(m_DlgParaSetStruct[0][26].MaxVal.lmax) + "]";//位";
//    ui->label_IDCountPos->setText(strTemp);
}

//显示参数
void USBBBE_CodeTMEmuSetGUIDlg::ShowPara(int type)
{
    if(type == 0)
    {
        if(m_MCSUSBTMSimulStatus != NULL)
        {
            if(m_autoMap)
            {
                DWORD stLen = sizeof(MCSUSBTMStatusStruct);
                if(m_pStateMgr != NULL)
                    m_MCSUSBTMStatus  = (MCSUSBTMStatusStruct*)m_pStateMgr->FindOneItem(m_TMStatuID, stLen);
                if(!m_MCSUSBTMStatus)return;
                //由遥测单元映射来
                m_MCSUSBTMSimulParam.ucBasiSel = m_MCSUSBTMStatus->tParams.ucBasiSel;

                m_MCSUSBTMSimulParam.ucCodingMd= m_MCSUSBTMStatus->tParams.ucCodingMd;

                m_MCSUSBTMSimulParam.ucCRC= m_MCSUSBTMStatus->tParams.ucCRC;

                m_MCSUSBTMSimulParam.ucCycFrmSynCodeLen= m_MCSUSBTMStatus->tParams.ucCycFrmSynCodeLen;

                m_MCSUSBTMSimulParam.ucDataCheck= m_MCSUSBTMStatus->tParams.ucDataCode;

                //m_MCSUSBTMSimulParam.ucdatasel= m_MCSUSBTMStatus->tParams.;

                m_MCSUSBTMSimulParam.ucFrameSynGLen= m_MCSUSBTMStatus->tParams.ucFrameSynGLen;

                m_MCSUSBTMSimulParam.ucFspPlace= m_MCSUSBTMStatus->tParams.ucFspPlace;

                m_MCSUSBTMSimulParam.ucG2Inv= m_MCSUSBTMStatus->tParams.ucG2Inv;

                m_MCSUSBTMSimulParam.ucIDBase= m_MCSUSBTMStatus->tParams.ucIDBase;

                m_MCSUSBTMSimulParam.ucIDDirect= m_MCSUSBTMStatus->tParams.ucIDDirect;

                m_MCSUSBTMSimulParam.ucInterleav= m_MCSUSBTMStatus->tParams.ucInterleav;

                m_MCSUSBTMSimulParam.ucLDPCRatio= m_MCSUSBTMStatus->tParams.ucLDPCRatio;

                m_MCSUSBTMSimulParam.ucRandLen= m_MCSUSBTMStatus->tParams.ucRandLen;
                m_MCSUSBTMSimulParam.ucRSMd= m_MCSUSBTMStatus->tParams.ucRSMd;

                m_MCSUSBTMSimulParam.ucSframeLen= m_MCSUSBTMStatus->tParams.ucSframeLen;

                m_MCSUSBTMSimulParam.ucSFrameSynMd= m_MCSUSBTMStatus->tParams.ucSFrameSynMd;

                m_MCSUSBTMSimulParam.ucturbCtrl= m_MCSUSBTMStatus->tParams.ucUnturbCtrl;
                m_MCSUSBTMSimulParam.ucTurboRatio= m_MCSUSBTMStatus->tParams.ucTurboRatio;

                m_MCSUSBTMSimulParam.ucViterbiMd= m_MCSUSBTMStatus->tParams.ucViterbiMd;
                m_MCSUSBTMSimulParam.ucWordLen= m_MCSUSBTMStatus->tParams.ucWordLen;
                m_MCSUSBTMSimulParam.uiCodeStyle= m_MCSUSBTMStatus->tParams.uiCodeStyle;
                m_MCSUSBTMSimulParam.uiCycFrmSynCode= m_MCSUSBTMStatus->tParams.uiCycFrmSynCode;
               // m_MCSUSBTMSimulParam.uidata= m_MCSUSBTMStatus->tParams.uid;
                m_MCSUSBTMSimulParam.uiFrameSynG= m_MCSUSBTMStatus->tParams.uiFrameSynG;
                m_MCSUSBTMSimulParam.uiPhase= m_MCSUSBTMStatus->tParams.uiPhase;
                m_MCSUSBTMSimulParam.uiRandPoly= m_MCSUSBTMStatus->tParams.uiRandPoly;
                m_MCSUSBTMSimulParam.uiSWFreq= m_MCSUSBTMStatus->tParams.uiSWFreq;
                m_MCSUSBTMSimulParam.uiTM_CodeRate= m_MCSUSBTMStatus->tParams.uiTM_CodeRate;
                m_MCSUSBTMSimulParam.usCycFrmSynCodePos= m_MCSUSBTMStatus->tParams.usCycFrmSynCodePos;
                m_MCSUSBTMSimulParam.usFrameLen= m_MCSUSBTMStatus->tParams.usFrameLen;
                m_MCSUSBTMSimulParam.usIDSubFrmPos= m_MCSUSBTMStatus->tParams.usIDSubFrmPos;
            }
            else
            {
                m_MCSUSBTMSimulParam = m_MCSUSBTMSimulStatus->tParams;
            }

        }
        else
        {
            DWORD stLen = sizeof(MCSUSBTMSimulStatusStruct);
            if(m_pStateMgr != NULL)
                m_MCSUSBTMSimulStatus  = (MCSUSBTMSimulStatusStruct*)m_pStateMgr->FindOneItem(m_StatusParaMCSUSBTMSimulID, stLen);
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
    /**********************************参数**********************************/
    //帧同步码位置
    if (m_MCSUSBTMSimulParam.ucFspPlace > 0)
        ui->comboBox_FrmPlace->setCurrentIndex(m_MCSUSBTMSimulParam.ucFspPlace - 1);
    //码型
    if (m_MCSUSBTMSimulParam.uiCodeStyle > 0)
        ui->comboBox_CodeType->setCurrentIndex(m_MCSUSBTMSimulParam.uiCodeStyle - 1);
    //编码方式
    if (m_MCSUSBTMSimulParam.ucCodingMd > 0)
        ui->comboBox_DecodeType->setCurrentIndex(m_MCSUSBTMSimulParam.ucCodingMd - 1);
    //RS码类型
    if (m_MCSUSBTMSimulParam.ucRSMd > 0)
        ui->comboBox_RSType->setCurrentIndex(m_MCSUSBTMSimulParam.ucRSMd - 1);
    //RS编码坐标基
    if (m_MCSUSBTMSimulParam.ucBasiSel > 0)
        ui->comboBox_RSEncodePlot->setCurrentIndex(m_MCSUSBTMSimulParam.ucBasiSel - 1);
    //数据源选择
    if (m_MCSUSBTMSimulParam.ucDataCheck > 0)
        ui->comboBox_DataSource->setCurrentIndex(m_MCSUSBTMSimulParam.ucDataCheck - 1);
    //交织深度
    if (0 < m_MCSUSBTMSimulParam.ucInterleav && m_MCSUSBTMSimulParam.ucInterleav < 6)
    {
        ui->lineEdit_RSDecodeDeep->setText(QString::number(m_MCSUSBTMSimulParam.ucInterleav));
    }
    else
    {
        ui->lineEdit_RSDecodeDeep->setText("1");
    }
    //卷积输出相位
    if (m_MCSUSBTMSimulParam.ucG2Inv > 0)
        ui->comboBox_OutputPhase->setCurrentIndex(m_MCSUSBTMSimulParam.ucG2Inv - 1);
    //卷积码类型
    if (m_MCSUSBTMSimulParam.ucViterbiMd > 0)
        ui->comboBox_RecurrentCodeType->setCurrentIndex(m_MCSUSBTMSimulParam.ucViterbiMd - 1);
    //数据源类型
    if (m_MCSUSBTMSimulParam.ucdatasel > 0)
        ui->comboBox_DataSourceType->setCurrentIndex(m_MCSUSBTMSimulParam.ucdatasel - 1);
    //帧同步码组长
    int byteFrameSynGLen = 8;
    if (m_MCSUSBTMSimulParam.ucFrameSynGLen > 0)
    {
        ui->comboBox_FrameSyncGroupLen->setCurrentIndex(m_MCSUSBTMSimulParam.ucFrameSynGLen - 1);
        byteFrameSynGLen = (m_MCSUSBTMSimulParam.ucFrameSynGLen + 1) * 2;
    }
    //加扰使能
    if (m_MCSUSBTMSimulParam.ucturbCtrl > 0)
        ui->comboBox_RandEn->setCurrentIndex(m_MCSUSBTMSimulParam.ucturbCtrl - 1);
    //加扰多项式长度
    if (m_MCSUSBTMSimulParam.ucRandLen > 0)
        ui->comboBox_RandPolyLen->setCurrentIndex(m_MCSUSBTMSimulParam.ucRandLen - 1);
    //副帧码组长度
    int byteSubFrameSynGLen = 8;
    if (m_MCSUSBTMSimulParam.ucCycFrmSynCodeLen > 0)
    {
        ui->comboBox_SubFrameGroupLen->setCurrentIndex(m_MCSUSBTMSimulParam.ucCycFrmSynCodeLen - 1);
        byteSubFrameSynGLen = m_MCSUSBTMSimulParam.ucCycFrmSynCodeLen * 2;
    }
    //副帧同步方式
    if (m_MCSUSBTMSimulParam.ucSFrameSynMd > 0)
        ui->comboBox_SubFrmSyncType->setCurrentIndex(m_MCSUSBTMSimulParam.ucSFrameSynMd - 1);
    //ID字基值
    if (m_MCSUSBTMSimulParam.ucIDBase > 0)
        ui->comboBox_IDBaseValue->setCurrentIndex(m_MCSUSBTMSimulParam.ucIDBase - 1);
    //ID计数方向
    if (m_MCSUSBTMSimulParam.ucIDDirect > 0)
        ui->comboBox_idDirect->setCurrentIndex(m_MCSUSBTMSimulParam.ucIDDirect - 1);

    //副载波频率
    ui->lineEdit_SubWaveFr->setText(QString::number(m_MCSUSBTMSimulParam.uiSWFreq / 1000.0, 'f', 3));
    //码速率
    ui->lineEdit_CodeRate->setText(QString::number(m_MCSUSBTMSimulParam.uiTM_CodeRate));
    //固定数设置
    ui->lineEdit_ConstSetting->setText(QString("%1").
                                       arg(m_MCSUSBTMSimulParam.uidata, 2,
                                           16, QChar::fromLatin1('0')).toUpper());
    //帧长
    ui->lineEdit_FrameLen->setText(QString::number(m_MCSUSBTMSimulParam.usFrameLen));
    //字长
    ui->lineEdit_WordLen->setText(QString::number(m_MCSUSBTMSimulParam.ucWordLen));
    //格式长
    ui->lineEdit_FormatLen->setText(QString::number(m_MCSUSBTMSimulParam.ucSframeLen));
    //帧同步码组
    ui->lineEdit_FrmSyncGroup->setText(QString("%1").
                                       arg(m_MCSUSBTMSimulParam.uiFrameSynG, byteFrameSynGLen,
                                           16, QChar::fromLatin1('0')).toUpper());
    //加扰多项式
    ui->lineEdit_RandPoly->setText(QString("%1").
                                   arg(m_MCSUSBTMSimulParam.uiRandPoly,m_MCSUSBTMSimulParam.ucRandLen * 2,
                                       16, QChar::fromLatin1('0')).toUpper());
    //加扰初相
    ui->lineEdit_RandPhase->setText(QString("%1").
                                    arg(m_MCSUSBTMSimulParam.uiPhase,m_MCSUSBTMSimulParam.ucRandLen * 2,
                                        16, QChar::fromLatin1('0')).toUpper());
    //副帧码组
    ui->lineEdit_SubFrmGroup->setText(QString("%1").
                                      arg(m_MCSUSBTMSimulParam.uiCycFrmSynCode, byteSubFrameSynGLen,
                                          16, QChar::fromLatin1('0')).toUpper());
    //副帧码组位置
    ui->lineEdit_SubFrmSynGroupPos->setText(QString::number(m_MCSUSBTMSimulParam.usCycFrmSynCodePos));
    //ID计数位置
    ui->lineEdit_IDCountPos->setText(QString::number(m_MCSUSBTMSimulParam.usIDSubFrmPos));

    on_comboBox_DecodeType_currentIndexChanged(ui->comboBox_DecodeType->currentIndex());
    on_comboBox_RecurrentCodeType_currentIndexChanged(ui->comboBox_RecurrentCodeType->currentIndex());
    on_comboBox_RandEn_currentIndexChanged(ui->comboBox_RandEn->currentIndex());
    on_comboBox_DataSource_currentIndexChanged(ui->comboBox_DataSource->currentIndex());
}

void USBBBE_CodeTMEmuSetGUIDlg::SetObjInfo(TLxTsspObjectInfo ObjectInfo)
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
        SUBITEM& sItem = *(pItem->getSubItemByDeviceID_SN(m_ObjectInfo.usLocalDID, m_ObjectInfo.ucSN));
        m_usC = sItem.usCMD;
        m_usTID = sItem.usTID;
        /*************************框架**********************************/
        //副帧同步器
        m_ParaMCSUSBTMSimulID = GetID(sItem.usStationID,
                                      sItem.ucSubSysID, sItem.usDeviceID,sItem.usTID,
                                      sItem.ucSN);
        /*************************框架**********************************/

        /*************************状态**********************************/
        m_StatusParaMCSUSBTMSimulID = GetID(sItem.usStationID,
                                            sItem.ucSubSysID, sItem.usDeviceID, sItem.usStateType,
                                            sItem.ucSN);
        /*************************状态**********************************/
    }

    pItem = m_DeviceMap.getItem(AUTO_MAP_MODULENAME);
    if (pItem->vSubItem.size() > 0)
    {
        SUBITEM& sItem = *(pItem->getSubItemByDeviceID_SN(m_ObjectInfo.usLocalDID, m_ObjectInfo.ucSN));
        m_TMStatuID = GetID(sItem.usStationID,
                                            sItem.ucSubSysID, sItem.usDeviceID, sItem.usStateType,
                                            sItem.ucSN);
    }

    //日志
    m_pILxTsspLogManager = pObjectManager->GetLogManager();
    m_pStateMgr = pObjectManager->GetStateManager();
    m_pILxTsspMessageManager = pObjectManager->GetMessageManager();
    m_pPublicInfo = pObjectManager->GetPublicInfoManager();
}

//使能或不使能控件
void USBBBE_CodeTMEmuSetGUIDlg::EnabledControls(bool bFlag)
{
    /*************************Group1*************************/
    ui->comboBox_FrmPlace->setEnabled(bFlag);
    //数据输出方式
    ui->comboBox_OutputPhase->setEnabled(bFlag);
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
    ui->comboBox_RSEncodePlot->setEnabled(bFlag);
    //RS译码交织深度
    ui->lineEdit_RSDecodeDeep->setEnabled(bFlag);
    //卷积输出相位
    ui->comboBox_OutputPhase->setEnabled(bFlag);
    //副载波频率
    ui->lineEdit_SubWaveFr->setEnabled(bFlag);
    ui->comboBox_DataSource->setEnabled(bFlag);
    //数据源类型
    ui->comboBox_DataSourceType->setEnabled(bFlag);
    //固定数设置
    ui->lineEdit_ConstSetting->setEnabled(bFlag);
    /*************************Group1*************************/

    /*************************Group2*************************/
    //帧长
    ui->lineEdit_FrameLen->setEnabled(bFlag);
    //帧同步码组长
    ui->comboBox_FrameSyncGroupLen->setEnabled(bFlag);
    //帧同步码组
    ui->lineEdit_FrmSyncGroup->setEnabled(bFlag);
    //字长
    ui->lineEdit_WordLen->setEnabled(false);
    //格式长
    ui->lineEdit_FormatLen->setEnabled(false);
    //加扰使能
    ui->comboBox_RandEn->setEnabled(bFlag);
    //加扰多项式长
    ui->comboBox_RandPolyLen->setEnabled(bFlag);
    //加扰多项式
    ui->lineEdit_RandPoly->setEnabled(bFlag);
    //加扰初相
    ui->lineEdit_RandPhase->setEnabled(bFlag);
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
    ui->lineEdit_IDCountPos->setEnabled(bFlag);
    //自动映射
    ui->pushButton_autoMap->setEnabled(bFlag);

    /*************************Group2*************************/

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
int USBBBE_CodeTMEmuSetGUIDlg::SaveToMacro(ILxTsspParameterMacro* pMacroObj)
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
        byteArray.resize(sizeof(m_MCSUSBTMSimulParam));
        memcpy(byteArray.data(),&m_MCSUSBTMSimulParam,sizeof(m_MCSUSBTMSimulParam));
        pMacroObj->UpdateParameterBlock(m_ParaMCSUSBTMSimulID, byteArray);

        //日志信息
        QString strLog;
        //tmpLog.ucWarningLevel  = 1;
        strLog += QString("%1: 保存宏成功！").arg(MODULENAME);
        CLog::addLog(strLog);
    }
}
//从界面取参数
void USBBBE_CodeTMEmuSetGUIDlg::GetPara()
{
    bool ok;
    //帧同步位置
    m_MCSUSBTMSimulParam.ucFspPlace = ui->comboBox_FrmPlace->currentIndex() + 1;
    //码率
    m_MCSUSBTMSimulParam.uiTM_CodeRate = ui->lineEdit_CodeRate->text().toInt();
    //字长
    m_MCSUSBTMSimulParam.ucWordLen = ui->lineEdit_WordLen->text().toInt();
    //帧长
    m_MCSUSBTMSimulParam.usFrameLen = ui->lineEdit_FrameLen->text().toInt();
    //帧同步码组
    char* pTem1 = ui->lineEdit_FrmSyncGroup->text().trimmed().toLocal8Bit().data();
    m_MCSUSBTMSimulParam.uiFrameSynG = StrToSynCode(pTem1);
    //帧同步码组长
    m_MCSUSBTMSimulParam.ucFrameSynGLen = ui->comboBox_FrameSyncGroupLen->currentIndex() + 1;
    //码型
    m_MCSUSBTMSimulParam.uiCodeStyle = ui->comboBox_CodeType->currentIndex() + 1;
    //副载波频率
    m_MCSUSBTMSimulParam.uiSWFreq = (UINT)(ui->lineEdit_SubWaveFr->text().toDouble() * 1000);
    //固定数设置
    m_MCSUSBTMSimulParam.uidata = ui->lineEdit_ConstSetting->text().toInt(&ok, 16);
    //编码方式
    m_MCSUSBTMSimulParam.ucCodingMd = ui->comboBox_DecodeType->currentIndex() + 1;
    //RS码类型
    m_MCSUSBTMSimulParam.ucRSMd = ui->comboBox_RSType->currentIndex() + 1;
    //RS编码坐标基
    m_MCSUSBTMSimulParam.ucBasiSel = ui->comboBox_RSEncodePlot->currentIndex() + 1;
    //数据源选择
    m_MCSUSBTMSimulParam.ucDataCheck = ui->comboBox_DataSource->currentIndex() + 1;
    //数据源类型
    m_MCSUSBTMSimulParam.ucdatasel = ui->comboBox_DataSourceType->currentIndex() + 1;
    //交织深度
    m_MCSUSBTMSimulParam.ucInterleav = ui->lineEdit_RSDecodeDeep->text().toInt();
    //卷积输出相位
    m_MCSUSBTMSimulParam.ucG2Inv = ui->comboBox_OutputPhase->currentIndex() + 1;
    //卷积码类型
    m_MCSUSBTMSimulParam.ucViterbiMd = ui->comboBox_RecurrentCodeType->currentIndex() + 1;
    //格式长
    m_MCSUSBTMSimulParam.ucSframeLen = ui->lineEdit_FormatLen->text().toInt();
    //加扰使能
    m_MCSUSBTMSimulParam.ucturbCtrl = ui->comboBox_RandEn->currentIndex() + 1;
    //加扰多项式
    m_MCSUSBTMSimulParam.uiRandPoly = ui->lineEdit_RandPoly->text().toUInt(&ok, 16);
    //加扰初相
    m_MCSUSBTMSimulParam.uiPhase = ui->lineEdit_RandPhase->text().toUInt(&ok, 16);
    //加扰多项式长度
    m_MCSUSBTMSimulParam.ucRandLen = ui->comboBox_RandPolyLen->currentIndex() + 1;
    //副帧码组长度
    m_MCSUSBTMSimulParam.ucCycFrmSynCodeLen = ui->comboBox_SubFrameGroupLen->currentIndex() + 1;
    //副帧码组
    char* pTem2 = ui->lineEdit_SubFrmGroup->text().trimmed().toLocal8Bit().data();
    m_MCSUSBTMSimulParam.uiCycFrmSynCode = StrToSynCode(pTem2);
    //副帧码组位置
    m_MCSUSBTMSimulParam.usCycFrmSynCodePos = ui->lineEdit_SubFrmSynGroupPos->text().toInt();
    //ID字基值
    m_MCSUSBTMSimulParam.ucIDBase = ui->comboBox_IDBaseValue->currentIndex() + 1;
    //ID计数方向
    m_MCSUSBTMSimulParam.ucIDDirect = ui->comboBox_idDirect->currentIndex() + 1;
    //ID字位置
    m_MCSUSBTMSimulParam.usIDSubFrmPos = ui->lineEdit_IDCountPos->text().toInt();
    //副帧同步方式
    m_MCSUSBTMSimulParam.ucSFrameSynMd = ui->comboBox_SubFrmSyncType->currentIndex() + 1;
}

//验证参数范围
int USBBBE_CodeTMEmuSetGUIDlg::CheckPara()
{
    return CheckParam() == TRUE ? 1 : -1;
}

//定时器
void USBBBE_CodeTMEmuSetGUIDlg::timerEvent(QTimerEvent *event)
{
    ShowPara(0);
}

//更改参数按钮休息处理函数
void USBBBE_CodeTMEmuSetGUIDlg::on_pushButton_Modify_clicked()
{
    if(ui->pushButton_Modify->text() == "更改")
    {
        EnabledControls(true);
        killTimer(m_TimerID);
        on_comboBox_DecodeType_currentIndexChanged(ui->comboBox_DecodeType->currentIndex());
        on_comboBox_SubFrmSyncType_currentIndexChanged(ui->comboBox_SubFrmSyncType->currentIndex());
        on_comboBox_RecurrentCodeType_currentIndexChanged(ui->comboBox_RecurrentCodeType->currentIndex());
        on_comboBox_RandEn_currentIndexChanged(ui->comboBox_RandEn->currentIndex());
        on_comboBox_DataSource_currentIndexChanged(ui->comboBox_DataSource->currentIndex());
        on_comboBox_DataSourceType_currentIndexChanged(ui->comboBox_DataSourceType->currentIndex());
        return;
    }

    if(ui->pushButton_Modify->text() == "恢复")
    {
        EnabledControls(false);
        m_autoMap = false;
        m_TimerID = startTimer(500);
        return;
    }
}

void USBBBE_CodeTMEmuSetGUIDlg::on_pushButton_Set_clicked()
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
            byteArray.resize(sizeof(m_MCSUSBTMSimulParam));
            memcpy(byteArray.data(),&m_MCSUSBTMSimulParam,sizeof(MCSUSBTMSimulParamStruct));
            SendMessage((char*)&m_MCSUSBTMSimulParam, sizeof(MCSUSBTMSimulParamStruct), m_usC, m_usTID);
        }

        //日志信息
        QString strLog;
        //tmpLog.ucWarningLevel  = 1;
        strLog += QString("%1: 设置参数成功！").arg(MODULENAME);
        CLog::addLog(strLog);

        //不使能控件
        EnabledControls(false);
        m_TimerID = startTimer(500);
        m_autoMap = false;
    }
}

void USBBBE_CodeTMEmuSetGUIDlg::closeEvent(QCloseEvent *event)
{
    event->accept();
}

QSize USBBBE_CodeTMEmuSetGUIDlg::sizeHint() const
{
    return QSize(this->width(), this->height());
}

void USBBBE_CodeTMEmuSetGUIDlg::SubFrmEn(bool bFlag)
{
    //副帧长度
    //    ui->label_SubFrmLen->setEnabled(bFlag);
    //    ui->lineEdit_SubFrmLen->setEnabled(bFlag);on_comboBox_DataSourceType_currentIndexChanged(ui->comboBox_DataSourceType->currentIndex());

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
void USBBBE_CodeTMEmuSetGUIDlg::IDSubFrmEn(bool bFlag)
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
void USBBBE_CodeTMEmuSetGUIDlg::CycSubFrmEn(bool bFlag)
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
void USBBBE_CodeTMEmuSetGUIDlg::ReserveSubFrmEn(bool bFlag)
{

}

void USBBBE_CodeTMEmuSetGUIDlg::SendMessage(char* pData, unsigned short  usLen, unsigned short usC, unsigned short usTID)
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

DWORDLONG USBBBE_CodeTMEmuSetGUIDlg::StrToSynCode(const char * pStr)
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


//编码方式
void USBBBE_CodeTMEmuSetGUIDlg::on_comboBox_DecodeType_currentIndexChanged(int index)
{
    if (ui->pushButton_Modify->text() == "更改")
        return;
    if (index== 0){//关
        ui->comboBox_FrmPlace->setEnabled(true);
        ui->comboBox_RecurrentCodeType->setEnabled(false);
        ui->comboBox_RSType->setEnabled(false);
        ui->lineEdit_RSDecodeDeep->setEnabled(false);
        ui->comboBox_OutputPhase->setEnabled(false);
        ui->comboBox_RSEncodePlot->setEnabled(false);
        ui->comboBox_SubFrmSyncType->setEnabled(true);
        on_comboBox_SubFrmSyncType_currentIndexChanged(ui->comboBox_SubFrmSyncType->currentIndex());
    }
    if(index==1){

        ui->comboBox_FrmPlace->setEnabled(true);
        ui->comboBox_RecurrentCodeType->setEnabled(true);
        ui->comboBox_RSType->setEnabled(false);
        ui->lineEdit_RSDecodeDeep->setEnabled(false);
        ui->comboBox_OutputPhase->setEnabled(true);
        ui->comboBox_RSEncodePlot->setEnabled(false);
        ui->comboBox_SubFrmSyncType->setEnabled(true);
        on_comboBox_SubFrmSyncType_currentIndexChanged(ui->comboBox_SubFrmSyncType->currentIndex());
    }
    if(index==2){
        ui->comboBox_FrmPlace->setEnabled(false);
        ui->comboBox_FrmPlace->setCurrentIndex(0);
        ui->comboBox_RecurrentCodeType->setEnabled(false);
        ui->comboBox_RSType->setEnabled(true);
        ui->lineEdit_RSDecodeDeep->setEnabled(true);
        ui->comboBox_OutputPhase->setEnabled(false);
        ui->comboBox_RSEncodePlot->setEnabled(true);
        ui->comboBox_SubFrmSyncType->setEnabled(false);
        ui->comboBox_SubFrmSyncType->setCurrentIndex(0);
        on_comboBox_SubFrmSyncType_currentIndexChanged(ui->comboBox_SubFrmSyncType->currentIndex());

    }
    if(index==3){
        ui->comboBox_FrmPlace->setEnabled(false);
        ui->comboBox_FrmPlace->setCurrentIndex(0);
        ui->comboBox_RecurrentCodeType->setEnabled(true);
        ui->comboBox_RSType->setEnabled(true);
        ui->lineEdit_RSDecodeDeep->setEnabled(true);
        ui->comboBox_OutputPhase->setEnabled(true);
        ui->comboBox_RSEncodePlot->setEnabled(true);
        ui->comboBox_SubFrmSyncType->setEnabled(false);
        ui->comboBox_SubFrmSyncType->setCurrentIndex(0);
        on_comboBox_SubFrmSyncType_currentIndexChanged(ui->comboBox_SubFrmSyncType->currentIndex());
    }
}
//卷积码
void USBBBE_CodeTMEmuSetGUIDlg::on_comboBox_RecurrentCodeType_currentIndexChanged(int index)
{
    if (ui->pushButton_Modify->text() == "更改")
        return;
    // 1/2
    if(index== 0){
        if(ui->comboBox_DecodeType->currentIndex() == 1 ||
                ui->comboBox_DecodeType->currentIndex() == 3)
            ui->comboBox_OutputPhase->setEnabled(true);
    }
    // 3/4
    if(index== 1){
        ui->comboBox_OutputPhase->setEnabled(false);
        ui->comboBox_OutputPhase->setCurrentIndex(0);

    }
}

void USBBBE_CodeTMEmuSetGUIDlg::on_comboBox_RandEn_currentIndexChanged(int index)
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

void USBBBE_CodeTMEmuSetGUIDlg::on_comboBox_SubFrmSyncType_currentIndexChanged(int index)
{
    if (ui->pushButton_Modify->text() == "更改")
        return;

    if(index==0){//无副帧

        ui->comboBox_SubFrameGroupLen->setEnabled(false);
        ui->lineEdit_SubFrmSynGroupPos->setEnabled(false);
        ui->lineEdit_SubFrmGroup->setEnabled(false);
        ui->comboBox_IDBaseValue->setEnabled(false);
        ui->comboBox_idDirect->setEnabled(false);
        ui->lineEdit_IDCountPos->setEnabled(false);



        ui->lineEdit_FormatLen->setEnabled(false);

    }
    if(index==2){//循环副帧
        ui->comboBox_SubFrameGroupLen->setEnabled(true);
        ui->lineEdit_SubFrmSynGroupPos->setEnabled(true);
        ui->lineEdit_SubFrmGroup->setEnabled(true);
        ui->comboBox_IDBaseValue->setEnabled(false);
        ui->comboBox_idDirect->setEnabled(false);
        ui->lineEdit_IDCountPos->setEnabled(false);



        ui->lineEdit_FormatLen->setEnabled(true);
    }
    if(index==1){//ID副帧

        ui->comboBox_SubFrameGroupLen->setEnabled(false);
        ui->lineEdit_SubFrmSynGroupPos->setEnabled(false);
        ui->lineEdit_SubFrmGroup->setEnabled(false);
        ui->comboBox_IDBaseValue->setEnabled(true);
        ui->comboBox_idDirect->setEnabled(true);
        ui->lineEdit_IDCountPos->setEnabled(true);




        ui->lineEdit_FormatLen->setEnabled(true);


    }
    if(index==3){//反码副帧
        ui->comboBox_SubFrameGroupLen->setEnabled(false);
        ui->lineEdit_SubFrmSynGroupPos->setEnabled(false);
        ui->lineEdit_SubFrmGroup->setEnabled(false);
        ui->comboBox_IDBaseValue->setEnabled(false);
        ui->comboBox_idDirect->setEnabled(false);
        ui->lineEdit_IDCountPos->setEnabled(false);


        ui->lineEdit_FormatLen->setEnabled(true);
    }
}
//数据源
void USBBBE_CodeTMEmuSetGUIDlg::on_comboBox_DataSource_currentIndexChanged(int index)
{
    if (ui->pushButton_Modify->text() == "更改")
        return;
    if(index==0){
        ui->comboBox_DataSourceType->setEnabled(true);
        on_comboBox_DataSourceType_currentIndexChanged(ui->comboBox_DataSourceType->currentIndex());
    }
    if(index==1){
        ui->comboBox_DataSourceType->setEnabled(false);
        ui->lineEdit_ConstSetting->setEnabled(false);
        on_comboBox_DataSourceType_currentIndexChanged(ui->comboBox_DataSourceType->currentIndex());


    }

}
void USBBBE_CodeTMEmuSetGUIDlg::on_comboBox_DataSourceType_currentIndexChanged(int index)
{
    if (ui->pushButton_Modify->text() == "更改")
        return;
    if(index==0 && ui->comboBox_DataSource->currentIndex()==0){
        ui->lineEdit_ConstSetting->setEnabled(true);

    }
    else{
        ui->lineEdit_ConstSetting->setEnabled(false);
    }
}

void USBBBE_CodeTMEmuSetGUIDlg::on_pushButton_autoMap_clicked()
{
    m_autoMap = true;
    ui->pushButton_autoMap->setEnabled(false);
    ShowPara(0);
}
