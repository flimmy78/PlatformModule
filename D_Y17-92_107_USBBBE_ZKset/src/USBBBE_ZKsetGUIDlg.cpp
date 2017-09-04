#include "USBBBE_ZKsetGUIDlg.h"
#include "ui_USBBBE_ZKsetGUIDlg.h"
#include <QCloseEvent>
#include <QMessageBox>
#include <QDateTime>
#include "../../Common/Log.h"

extern quint64 GetID(ushort usStationID, uchar ucSubSysID, ushort usDeviceID, ushort usType,
                     uchar ucSn);
USBBBE_ZKsetGUIDlg::USBBBE_ZKsetGUIDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::USBBBE_ZKsetGUIDlg)
{
    ui->setupUi(this);
    m_pStateMgr = NULL;
    m_pILxTsspMessageManager = NULL;
    m_pILxTsspLogManager = NULL;
    m_pPublicInfo = NULL;
    m_pHAManager = NULL;

    m_MCSUSBTCStatus = NULL;
    setAttribute(Qt::WA_DeleteOnClose);
}

USBBBE_ZKsetGUIDlg::~USBBBE_ZKsetGUIDlg()
{
    killTimer(m_TimerID);
    delete ui;
}

void USBBBE_ZKsetGUIDlg::setText(const QString& strTitle, ParaStruct xmlsetText[3][60], QString MapMean[50][30], int MapNum[50], uchar Type, ILxTsspParameterMacro* pMacroObj)
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
void USBBBE_ZKsetGUIDlg::InitDLG(int Type, ILxTsspParameterMacro* pMacroObj)
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
            m_pHAManager->GetParameter(m_ParaMCSUSBTCID, byteArray);
            memcpy(&m_MCSUSBTCParam, byteArray.data(), sizeof(MCSUSBTCParamStruct));
        }
        if(m_pStateMgr != NULL)
        {
            DWORD stLen = sizeof(MCSUSBTCStatusStruct);
            m_MCSUSBTCStatus = (MCSUSBTCStatusStruct*)m_pStateMgr->FindOneItem(m_StatusParaMCSUSBTCID, stLen);
            if(m_MCSUSBTCStatus == NULL || stLen != sizeof(MCSUSBTCStatusStruct))
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

            pMacroObj->GetParameterBlock(m_ParaMCSUSBTCID, byteArray);
            memcpy(&m_MCSUSBTCParam, byteArray.data(), sizeof(MCSUSBTCParamStruct));
        }
        ui->pushButton_Modify->setVisible(false);
        ui->pushButton_Set->setVisible(false);

        //显示参数
        ShowPara(1);
        on_comboBox_MudulationSys_currentIndexChanged(ui->comboBox_MudulationSys->currentIndex());
        on_comboBox_TeleCtrlPat_currentIndexChanged(ui->comboBox_TeleCtrlPat->currentIndex());
        on_comboBox_BCHEncode_currentIndexChanged(ui->comboBox_BCHEncode->currentIndex());
        on_comboBox_RandCtrl_currentIndexChanged(ui->comboBox_RandCtrl->currentIndex());
        on_lineEdit_ExeCodeLen_textChanged(ui->lineEdit_ExeCodeLen->text());
    }
}

//设置输入格式
void USBBBE_ZKsetGUIDlg::InitInput()
{
    //正整数
    QRegExp regExp("[0-9]{1,8}");
    QRegExp regExp0("[0-9\\.]{1,16}");
    //码速率
    QRegExp regExp_CodeRate("[0-9]{3,4}");
    ui->lineEdit_CodeRate->setValidator(new QRegExpValidator(regExp_CodeRate, this));
    //调制频率
    QRegExp regExp_ModulationFre("[0-9]{1,3}(\\.)?([0-9]{1})");
    ui->lineEdit_ModulationFre->setValidator(new QRegExpValidator(regExp_ModulationFre, this));
    //副载波频1频率（f1）
    ui->lineEdit_SubWaveFre->setValidator(new QRegExpValidator(regExp_ModulationFre, this));
    //副载频填充频率（f填充）
    ui->lineEdit_SubWaveFillFre->setValidator(new QRegExpValidator(regExp_ModulationFre, this));
    //副载频执行频率（f执行）
    ui->lineEdit_SubWaveExeFre->setValidator(new QRegExpValidator(regExp_ModulationFre, this));

    QRegExp regExpFillLen("[0-9]{1,4}");
    //前补序列长度
    ui->lineEdit_BeforeFillLen->setValidator(new QRegExpValidator(regExpFillLen, this));
    //后补序列长度
    ui->lineEdit_LateFillLen->setValidator(new QRegExpValidator(regExpFillLen, this));
    //执行编码长度
    QRegExp regExe_CodeLen("[0-9]{1}");
    ui->lineEdit_ExeCodeLen->setValidator(new QRegExpValidator(regExe_CodeLen, this));
    //指令保护间隙
    QRegExp regExe_BufferGap("[0-9]{3,5}");
    ui->lineEdit_BufferGap->setValidator(new QRegExpValidator(regExe_BufferGap, this));
    //加扰多项式长度
    QRegExp regExp01("[1-4]{1,1}");
    ui->lineEdit_RandPolyLen->setValidator(new QRegExpValidator(regExp01, this));

    //字母数字
    QRegExp regExp2("[A-Fa-f0-9]{1,8}");
    QRegExp regExp21("[A-Fa-f0-9]{1,2}");
    //前补序列码样（Hex）
    ui->lineEdit_BeforeFillCode->setValidator(new QRegExpValidator(regExp21, this));
    //后补序列码样（Hex）
    ui->lineEdit_LateFillCode->setValidator(new QRegExpValidator(regExp21, this));
    //空闲序列码样（Hex）
    ui->lineEdit_FreeCode->setValidator(new QRegExpValidator(regExp21, this));
    //加扰多项式（Hex）
    ui->lineEdit_RandPoly->setValidator(new QRegExpValidator(regExp2, this));
    //加扰初相
    ui->lineEdit_RandPhase->setValidator(new QRegExpValidator(regExp2, this));

    //字母数字
    QRegExp regExp3("[A-Fa-f0-9]{1,16}");
    QRegExp regExp4("[A-Fa-f0-9]{1,4}");
    //起始序列图样
    ui->lineEdit_BeginPattern->setValidator(new QRegExpValidator(regExp4, this));
    //结尾序列图样
    ui->lineEdit_LatePattern->setValidator(new QRegExpValidator(regExp3, this));
}

void USBBBE_ZKsetGUIDlg::ShowControls()
{
    //向下拉框控件中写入从xml中读取的内容
    int i;
    //副载波调制体制
    for(i=0; i<DlgMapLineNum[0];i++)
    {
        ui->comboBox_MudulationSys->addItem(m_DlgMapMean[0][i]);
    }
    ui->comboBox_MudulationSys->setCurrentIndex(m_DlgParaSetStruct[0][0].InitVal.lval);
    //遥控工作模式
    for(i=0; i<DlgMapLineNum[1];i++)
    {
        ui->comboBox_TeleCtrlPat->addItem(m_DlgMapMean[1][i]);
    }
    ui->comboBox_TeleCtrlPat->setCurrentIndex(m_DlgParaSetStruct[0][1].InitVal.lval);
    //工作期启动方式
    for(i=0; i<DlgMapLineNum[2];i++)
    {
        ui->comboBox_StartingMethod->addItem(m_DlgMapMean[2][i]);
    }
    ui->comboBox_StartingMethod->setCurrentIndex(m_DlgParaSetStruct[0][2].InitVal.lval);
    //码型
    for(i=0; i<DlgMapLineNum[3];i++)
    {
        ui->comboBox_CodeType->addItem(m_DlgMapMean[3][i]);
    }
    ui->comboBox_CodeType->setCurrentIndex(m_DlgParaSetStruct[0][3].InitVal.lval);
    //BCH编码
    for(i=0; i<DlgMapLineNum[4];i++)
    {
        ui->comboBox_BCHEncode->addItem(m_DlgMapMean[4][i]);
    }
    ui->comboBox_BCHEncode->setCurrentIndex(m_DlgParaSetStruct[0][17].InitVal.lval);
    //BCH起始序列控制
    for(i=0; i<DlgMapLineNum[5];i++)
    {
        ui->comboBox_BeginCtrl->addItem(m_DlgMapMean[5][i]);
    }
    ui->comboBox_BeginCtrl->setCurrentIndex(m_DlgParaSetStruct[0][18].InitVal.lval);
    //加扰控制
    for(i=0; i<DlgMapLineNum[6];i++)
    {
        ui->comboBox_RandCtrl->addItem(m_DlgMapMean[6][i]);
    }
    ui->comboBox_RandCtrl->setCurrentIndex(m_DlgParaSetStruct[0][20].InitVal.lval);
    //结尾序列控制
    for(i=0; i<DlgMapLineNum[7];i++)
    {
        ui->comboBox_LateCtrl->addItem(m_DlgMapMean[7][i]);
    }
    ui->comboBox_LateCtrl->setCurrentIndex(m_DlgParaSetStruct[0][21].InitVal.lval);
    //往静态控件中写入参数缺省值
    //码速率
    ui->lineEdit_CodeRate->setText(QString::number(m_DlgParaSetStruct[0][4].InitVal.lval / 1000.0, 'f', 3));
    ui->label_CodeRate->setText("码速率\n[" + QString::number(m_DlgParaSetStruct[0][4].MinVal.lmin) +
            "~" + QString::number(m_DlgParaSetStruct[0][4].MaxVal.lmax) + "]bps");
    //调制频率
    ui->lineEdit_ModulationFre->setText(QString::number(m_DlgParaSetStruct[0][5].InitVal.lval / 1000.0, 'f', 3));
    ui->label_ModulationFre->setText("调制频率\n[" + QString::number(m_DlgParaSetStruct[0][5].MinVal.lmin / 1000) +
            "~" + QString::number(m_DlgParaSetStruct[0][5].MaxVal.lmax / 1000) + "]kHz");
    //副载波频1频率（f1）
    ui->lineEdit_SubWaveFre->setText(QString::number(m_DlgParaSetStruct[0][6].InitVal.lval / 1000, 'f', 3));
    ui->label_SubWaveFre->setText("副载波频1频率(f1)\n[" + QString::number(m_DlgParaSetStruct[0][6].MinVal.lmin / 1000) +
            "~" + QString::number(m_DlgParaSetStruct[0][6].MaxVal.lmax / 1000) + "]kHz");
    //副载波填充频率（f填充）
    ui->lineEdit_SubWaveFillFre->setText(QString::number(m_DlgParaSetStruct[0][7].InitVal.lval / 1000, 'f', 3));
    ui->label_SubWaveFillFre->setText("副载波填充频率(f填充)\n[" + QString::number(m_DlgParaSetStruct[0][7].MinVal.lmin / 1000) +
            "~" + QString::number(m_DlgParaSetStruct[0][7].MaxVal.lmax / 1000) + "]kHz");
    //副载波执行频率（f执行）
    ui->lineEdit_SubWaveExeFre->setText(QString::number(m_DlgParaSetStruct[0][8].InitVal.lval / 1000, 'f', 3));
    ui->label_SubWaveExeFre->setText("副载波执行频率(f执行)\n[" + QString::number(m_DlgParaSetStruct[0][8].MinVal.lmin / 1000) +
            "~" + QString::number(m_DlgParaSetStruct[0][8].MaxVal.lmax / 1000) + "]kHz");
    //后补序列样码
    ui->lineEdit_LateFillCode->setText(QString::number(m_DlgParaSetStruct[0][10].InitVal.lval, 16).toUpper());
    //前补序列样码
    ui->lineEdit_BeforeFillCode->setText(QString::number(m_DlgParaSetStruct[0][9].InitVal.lval, 16).toUpper());
    //执行序列样码
    ui->lineEdit_ExeCode->setText(QString::number(m_DlgParaSetStruct[0][11].InitVal.lval, 16).toUpper());
    //空闲序列样码
    ui->lineEdit_FreeCode->setText(QString::number(m_DlgParaSetStruct[0][12].InitVal.lval, 16).toUpper());
    //前补序列长度
    ui->lineEdit_BeforeFillLen->setText(QString::number(m_DlgParaSetStruct[0][13].InitVal.lval));
    ui->label_BeforeFillLen->setText("前补序列长度\n[" + QString::number(m_DlgParaSetStruct[0][13].MinVal.lmin) +
            "~" + QString::number(m_DlgParaSetStruct[0][13].MaxVal.lmax) + "]");
    //后补序列长度
    ui->lineEdit_LateFillLen->setText(QString::number(m_DlgParaSetStruct[0][14].InitVal.lval));
    ui->label_LateFillLen->setText("后补序列长度\n[" + QString::number(m_DlgParaSetStruct[0][14].MinVal.lmin) +
            "~" + QString::number(m_DlgParaSetStruct[0][14].MaxVal.lmax) + "]");
    //执行编码长度
    ui->lineEdit_ExeCodeLen->setText(QString::number(m_DlgParaSetStruct[0][15].InitVal.lval));
    ui->label_ExeCodeLen->setText("执行编码长度\n[" + QString::number(m_DlgParaSetStruct[0][15].MinVal.lmin) +
            "~" + QString::number(m_DlgParaSetStruct[0][15].MaxVal.lmax) + "]");
    //指令保护间隙
    ui->lineEdit_BufferGap->setText(QString::number(m_DlgParaSetStruct[0][16].InitVal.lval));
    ui->label_BufferGap->setText("指令保护间隙\n[" + QString::number(m_DlgParaSetStruct[0][16].MinVal.lmin) +
            "~" + QString::number(m_DlgParaSetStruct[0][16].MaxVal.lmax) + "]");
    //起始序列图样
    ui->lineEdit_BeginPattern->setText(QString::number(m_DlgParaSetStruct[0][19].InitVal.lval,16).toUpper());
    //结尾序列图样
    ui->lineEdit_LatePattern->setText(QString::number(m_DlgParaSetStruct[0][22].InitVal.lval,16).toUpper());
    //加扰多项式长度
    ui->lineEdit_RandPolyLen->setText(QString::number(m_DlgParaSetStruct[0][23].InitVal.lval));
    ui->label_RandPolyLen->setText("加扰多项式长度\n[" + QString::number(m_DlgParaSetStruct[0][23].MinVal.lmin) +
            "~" + QString::number(m_DlgParaSetStruct[0][23].MaxVal.lmax) + "]字节");
    //加扰多项式
    ui->lineEdit_RandPoly->setText(QString::number(m_DlgParaSetStruct[0][24].InitVal.lval,16).toUpper());
    //加扰初相
    ui->lineEdit_RandPhase->setText(QString::number(m_DlgParaSetStruct[0][25].InitVal.lval,16).toUpper());
}

//显示参数
void USBBBE_ZKsetGUIDlg::ShowPara(int type)
{
    if(type == 0)       //参数设置
    {
        //副帧同步器
        if(m_MCSUSBTCStatus != NULL)
        {
            m_MCSUSBTCParam = m_MCSUSBTCStatus->tParams;
        }
        else
        {
            DWORD stLen = sizeof(MCSUSBTCStatusStruct);
            if(m_pStateMgr != NULL)
                m_MCSUSBTCStatus  = (MCSUSBTCStatusStruct*)m_pStateMgr->FindOneItem(m_StatusParaMCSUSBTCID, stLen);
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
    /********************************参数设置*********************************/
    //调制体制 码型
    if(m_MCSUSBTCParam.ucModStyle == 1)//psk
    {
        ui->label_ModulationFre->setText("调制频率");
        ui->comboBox_MudulationSys->setCurrentIndex(0);
        if(ui->comboBox_CodeType->count() == 7)
            ui->comboBox_CodeType->removeItem(0);
        if(2 <= m_MCSUSBTCParam.ucCodeStyle &&
                m_MCSUSBTCParam.ucCodeStyle <= 7)
        {
            ui->comboBox_CodeType->setCurrentIndex(m_MCSUSBTCParam.ucCodeStyle - 2);
        }else
        {
            ui->comboBox_CodeType->setCurrentIndex(0);
        }
    }
    else//fsk
    {
        ui->label_ModulationFre->setText("副载波0频 (f0)");
        if(ui->comboBox_CodeType->count() == 6)
        {
            QIcon qIcon;
            ui->comboBox_CodeType->insertItem(0, qIcon, "RZ-L");
        }
        ui->comboBox_MudulationSys->setCurrentIndex(1);
        if(1 <= m_MCSUSBTCParam.ucCodeStyle &&
                m_MCSUSBTCParam.ucCodeStyle <= 7)
        {
            ui->comboBox_CodeType->setCurrentIndex(m_MCSUSBTCParam.ucCodeStyle - 1);
        }else
        {
            ui->comboBox_CodeType->setCurrentIndex(0);
        }
    }

    //遥控工作模式
    ui->comboBox_TeleCtrlPat->setCurrentIndex(m_MCSUSBTCParam.ucSWaveMd - 1);
    //工作期启动方式
    ui->comboBox_StartingMethod->setCurrentIndex(m_MCSUSBTCParam.ucTCWorkCyc - 1);

    //BCH编码
    ui->comboBox_BCHEncode->setCurrentIndex(m_MCSUSBTCParam.ucBCHCode - 1);
    //BCH起始序列控制
    ui->comboBox_BeginCtrl->setCurrentIndex(m_MCSUSBTCParam.ucBCHFQueueCheck - 1);
    //加扰控制
    ui->comboBox_RandCtrl->setCurrentIndex(m_MCSUSBTCParam.ucturbCtrl - 1);
    //结尾序列控制
    ui->comboBox_LateCtrl->setCurrentIndex(m_MCSUSBTCParam.ucBCHBQueueCheck - 1);

    //码速率
    ui->lineEdit_CodeRate->setText(QString::number(m_MCSUSBTCParam.uiCodeRate));
    //调制频率
    ui->lineEdit_ModulationFre->setText(QString::number(m_MCSUSBTCParam.uiSwaveFreq_1 / 1000.0));
    //副载波频1频率（f1）
    ui->lineEdit_SubWaveFre->setText(QString::number(m_MCSUSBTCParam.uiSwaveFreq_2 / 1000.0));
    //副载波填充频率（f填充）
    ui->lineEdit_SubWaveFillFre->setText(QString::number(m_MCSUSBTCParam.uiSwaveFreq_3 / 1000.0));
    //副载波执行频率（f执行）
    ui->lineEdit_SubWaveExeFre->setText(QString::number(m_MCSUSBTCParam.uiSwaveFreq_4 / 1000.0));
    //后补序列样码
    ui->lineEdit_LateFillCode->setText(QString::number(m_MCSUSBTCParam.ucBCodeMinU, 16).toUpper());
    //前补序列样码
    ui->lineEdit_BeforeFillCode->setText(QString::number(m_MCSUSBTCParam.ucFCodeMinU, 16).toUpper());
    //执行序列样码
    ui->lineEdit_ExeCode->setText(QString::number(m_MCSUSBTCParam.ucECodeMinU, 16).toUpper());
    //空闲序列样码
    ui->lineEdit_FreeCode->setText(QString::number(m_MCSUSBTCParam.ucIndleCode, 16).toUpper());
    //前补序列长度
    ui->lineEdit_BeforeFillLen->setText(QString::number(m_MCSUSBTCParam.usFQueueLen));
    //后补序列长度
    ui->lineEdit_LateFillLen->setText(QString::number(m_MCSUSBTCParam.usBQueueLen));
    //执行编码长度
    if(m_MCSUSBTCParam.ucECodeLen >= 8 && m_MCSUSBTCParam.ucECodeLen <= 64)
        ui->lineEdit_ExeCodeLen->setText(QString::number(m_MCSUSBTCParam.ucECodeLen / 8));
    else
        ui->lineEdit_ExeCodeLen->setText(QString::number(1));
    //指令保护间隙
    ui->lineEdit_BufferGap->setText(QString::number(m_MCSUSBTCParam.usTCProctC));
    //起始序列图样
    ui->lineEdit_BeginPattern->setText(QString::number(m_MCSUSBTCParam.usBCHFQueue, 16).toUpper());
    //结尾序列图样
    ui->lineEdit_LatePattern->setText(QString::number(m_MCSUSBTCParam.usBCHBQueue, 16).toUpper());
    //加扰多项式长度
    ui->lineEdit_RandPolyLen->setText(QString::number(m_MCSUSBTCParam.ucRandLen));
    //加扰多项式
    ui->lineEdit_RandPoly->setText(QString::number(m_MCSUSBTCParam.uiRandPoly, 16).toUpper());
    //加扰初相
    ui->lineEdit_RandPhase->setText(QString::number(m_MCSUSBTCParam.uiPhase, 16).toUpper());

    //前补序列C7
    ui->checkBox_BFillG1->setChecked(m_MCSUSBTCParam.ucC7FQueueCheck == 1 ? true : false);
    //后补序列C7
    ui->checkBox_LFillG1->setChecked(m_MCSUSBTCParam.ucC7BQueueCheck == 1 ? true : false);
    //    //前补序列C8
    //    ui->checkBox_BFillG2->setChecked(m_MCSUSBTCParam.ucC8FQueueCheck == 1 ? true : false);
    //    //后补序列C8
    //    ui->checkBox_LFillG2->setChecked(m_MCSUSBTCParam.ucC8BQueueCheck == 1 ? true : false);
    //前补序列C9
    ui->checkBox_BFillG3->setChecked(m_MCSUSBTCParam.ucC9FQueueCheck == 1 ? true : false);
    //后补序列C9
    ui->checkBox_LFillG3->setChecked(m_MCSUSBTCParam.ucC9BQueueCheck == 1 ? true : false);
    //前补序列A1
    ui->checkBox_BFillG4->setChecked(m_MCSUSBTCParam.ucA1FQueueCheck == 1 ? true : false);
    //后补序列A1
    ui->checkBox_LFillG4->setChecked(m_MCSUSBTCParam.ucA1BQueueCheck == 1 ? true : false);
}

//设置帧长范围
void USBBBE_ZKsetGUIDlg::SetFrameLenRange(int iWordLen)
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

void USBBBE_ZKsetGUIDlg::SetObjInfo(TLxTsspObjectInfo ObjectInfo)
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
        //码同步
        m_ParaMCSUSBTCID = GetID(sItem.usStationID,
                                 sItem.ucSubSysID, sItem.usDeviceID, sItem.usTID,
                                 sItem.ucSN);
        /*************************框架**********************************/

        /*************************状态**********************************/
        m_StatusParaMCSUSBTCID = GetID(sItem.usStationID,
                                       sItem.ucSubSysID, sItem.usDeviceID, sItem.usStateType,
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
void USBBBE_ZKsetGUIDlg::EnabledControls(bool bFlag)
{
    /*************************Group1*************************/
    //调制体制
    ui->comboBox_MudulationSys->setEnabled(bFlag);
    //遥控工作模式
    ui->comboBox_TeleCtrlPat->setEnabled(bFlag);
    //工作期启动方式
    ui->comboBox_StartingMethod->setEnabled(bFlag);
    //码型
    ui->comboBox_CodeType->setEnabled(bFlag);
    //码速率
    ui->lineEdit_CodeRate->setEnabled(bFlag);
    //调制频率
    ui->lineEdit_ModulationFre->setEnabled(bFlag);
    //副载波频率
    ui->lineEdit_SubWaveFre->setEnabled(bFlag);
    //副载波填充频
    ui->lineEdit_SubWaveFillFre->setEnabled(bFlag);
    //副载波执行频
    ui->lineEdit_SubWaveExeFre->setEnabled(bFlag);
    /*************************Group1*************************/

    /*************************Group2*************************/
    //前补序列码样
    ui->lineEdit_BeforeFillCode->setEnabled(bFlag);
    //后补序列码样
    ui->lineEdit_LateFillCode->setEnabled(bFlag);
    //帧同步码组
    ui->lineEdit_ExeCode->setEnabled(bFlag);
    //空闲序列码样
    ui->lineEdit_FreeCode->setEnabled(bFlag);
    //前补序列长度
    ui->lineEdit_BeforeFillLen->setEnabled(bFlag);
    //后补序列长度
    ui->lineEdit_LateFillLen->setEnabled(bFlag);
    //执行编码长度
    ui->lineEdit_ExeCodeLen->setEnabled(bFlag);
    //解扰初相
    ui->lineEdit_BufferGap->setEnabled(bFlag);
    //BCH编码
    ui->comboBox_BCHEncode->setEnabled(bFlag);
    //起始序列控制
    ui->comboBox_BeginCtrl->setEnabled(bFlag);
    //起始序列图样
    ui->lineEdit_BeginPattern->setEnabled(bFlag);
    //加扰控制
    ui->comboBox_RandCtrl->setEnabled(bFlag);
    //结尾序列控制
    ui->comboBox_LateCtrl->setEnabled(bFlag);
    //结尾序列图样
    ui->lineEdit_LatePattern->setEnabled(bFlag);
    //加扰多项式长度
    ui->lineEdit_RandPolyLen->setEnabled(bFlag);
    //加扰多项式
    ui->lineEdit_RandPoly->setEnabled(bFlag);
    //加扰多项式
    ui->lineEdit_RandPhase->setEnabled(bFlag);
    //checkBoxes
    ui->checkBox_BFillG1->setEnabled(bFlag);
    ui->checkBox_BFillG2->setEnabled(bFlag);
    ui->checkBox_BFillG3->setEnabled(bFlag);
    ui->checkBox_BFillG4->setEnabled(bFlag);
    ui->checkBox_LFillG1->setEnabled(bFlag);
    ui->checkBox_LFillG2->setEnabled(bFlag);
    ui->checkBox_LFillG3->setEnabled(bFlag);
    ui->checkBox_LFillG4->setEnabled(bFlag);

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
int USBBBE_ZKsetGUIDlg::SaveToMacro(ILxTsspParameterMacro* pMacroObj)
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
        byteArray.resize(sizeof(m_MCSUSBTCParam));
        memcpy(byteArray.data(),&m_MCSUSBTCParam,sizeof(MCSUSBTCParamStruct));
        pMacroObj->UpdateParameterBlock(m_ParaMCSUSBTCID,byteArray);

        //日志信息
        TLxTsspLog tmpLog;
        QString strLog;
        tmpLog.ucWarningLevel  = 1;
        strLog += QString("%1: 保存宏成功！").arg(MODULENAME);
        CLog::addLog(strLog, 1);
    }
}
//从界面取参数
void USBBBE_ZKsetGUIDlg::GetPara()
{
    bool ok;
    //调制体制 码型
    m_MCSUSBTCParam.ucModStyle = ui->comboBox_MudulationSys->currentIndex() == 0 ? 1 : 2;
    if(m_MCSUSBTCParam.ucModStyle == 1)//psk
    {
        m_MCSUSBTCParam.ucCodeStyle = ui->comboBox_CodeType->currentIndex() + 2;
    }
    else//fsk
    {
        m_MCSUSBTCParam.ucCodeStyle = ui->comboBox_CodeType->currentIndex() + 1;
    }

    //遥控工作模式
    m_MCSUSBTCParam.ucSWaveMd = ui->comboBox_TeleCtrlPat->currentIndex() + 1;
    //工作期启动方式
    m_MCSUSBTCParam.ucTCWorkCyc = ui->comboBox_StartingMethod->currentIndex() + 1;

    //BCH编码
    m_MCSUSBTCParam.ucBCHCode = ui->comboBox_BCHEncode->currentIndex() + 1;
    //BCH起始序列控制
    m_MCSUSBTCParam.ucBCHFQueueCheck = ui->comboBox_BeginCtrl->currentIndex() + 1;
    //加扰控制
    m_MCSUSBTCParam.ucturbCtrl = ui->comboBox_RandCtrl->currentIndex() + 1;
    //结尾序列控制
    m_MCSUSBTCParam.ucBCHBQueueCheck = ui->comboBox_LateCtrl->currentIndex() + 1;

    //码速率
    m_MCSUSBTCParam.uiCodeRate = ui->lineEdit_CodeRate->text().toInt();
    //调制频率
    m_MCSUSBTCParam.uiSwaveFreq_1 = qRound(ui->lineEdit_ModulationFre->text().toDouble() * 1000);
    //副载波频1频率（f1）
    m_MCSUSBTCParam.uiSwaveFreq_2 = qRound(ui->lineEdit_SubWaveFre->text().toDouble() * 1000);
    //副载波填充频率（f填充）
    m_MCSUSBTCParam.uiSwaveFreq_3 = qRound(ui->lineEdit_SubWaveFillFre->text().toDouble() * 1000);
    //副载频执行频率（f执行）
    m_MCSUSBTCParam.uiSwaveFreq_4 = qRound(ui->lineEdit_SubWaveExeFre->text().toDouble() * 1000);
    //后补序列样码
    m_MCSUSBTCParam.ucBCodeMinU = StrToSynCode(ui->lineEdit_LateFillCode->text().toStdString().c_str());
    //前补序列样码
    m_MCSUSBTCParam.ucFCodeMinU = StrToSynCode(ui->lineEdit_BeforeFillCode->text().toStdString().c_str());
    //执行序列样码
    m_MCSUSBTCParam.ucECodeMinU = StrToSynCode(ui->lineEdit_ExeCode->text().toStdString().c_str());
    //空闲序列样码
    m_MCSUSBTCParam.ucIndleCode = ui->lineEdit_FreeCode->text().toInt(&ok, 16);
    //前补序列长度
    m_MCSUSBTCParam.usFQueueLen = ui->lineEdit_BeforeFillLen->text().toInt();
    //后补序列长度
    m_MCSUSBTCParam.usBQueueLen = ui->lineEdit_LateFillLen->text().toInt();
    //执行编码长度
    m_MCSUSBTCParam.ucECodeLen = ui->lineEdit_ExeCodeLen->text().toInt() * 8;
    //指令保护间隙
    m_MCSUSBTCParam.usTCProctC = ui->lineEdit_BufferGap->text().toInt();
    //起始序列图样
    m_MCSUSBTCParam.usBCHFQueue = ui->lineEdit_BeginPattern->text().toInt(&ok, 16);
    //结尾序列图样
    m_MCSUSBTCParam.usBCHBQueue = (ui->lineEdit_LatePattern->text().toULongLong(&ok, 16));
    //加扰多项式长度
    m_MCSUSBTCParam.ucRandLen = ui->lineEdit_RandPolyLen->text().toInt();
    //加扰多项式
    m_MCSUSBTCParam.uiRandPoly = ui->lineEdit_RandPoly->text().toUInt(&ok, 16);
    //加扰初相
    m_MCSUSBTCParam.uiPhase = ui->lineEdit_RandPhase->text().toUInt(&ok, 16);

    //前补序列C7
    m_MCSUSBTCParam.ucC7FQueueCheck =ui->checkBox_BFillG1->isChecked() == true ? 1 : 2;
    //后补序列C7
    m_MCSUSBTCParam.ucC7BQueueCheck =ui->checkBox_LFillG1->isChecked() == true  ? 1 : 2;
    //    //前补序列C8
    //    m_MCSUSBTCParam.ucC8FQueueCheck =ui->checkBox_BFillG2->isChecked() == true  ? 1 : 2;
    //    //后补序列C8
    //    m_MCSUSBTCParam.ucC8BQueueCheck =ui->checkBox_LFillG2->isChecked() == true  ? 1 : 2;
    //前补序列C9
    m_MCSUSBTCParam.ucC9FQueueCheck =ui->checkBox_BFillG3->isChecked() == true  ? 1 : 2;
    //后补序列C9
    m_MCSUSBTCParam.ucC9BQueueCheck =ui->checkBox_LFillG3->isChecked() == true  ? 1 : 2;
    //前补序列A1
    m_MCSUSBTCParam.ucA1FQueueCheck =ui->checkBox_BFillG4->isChecked() == true  ? 1 : 2;
    //后补序列A1
    m_MCSUSBTCParam.ucA1BQueueCheck =ui->checkBox_LFillG4->isChecked() == true  ? 1 : 2;
}

//验证参数范围
int USBBBE_ZKsetGUIDlg::CheckPara()
{
    return CheckParam();
}

//定时器
void USBBBE_ZKsetGUIDlg::timerEvent(QTimerEvent *event)
{
    ShowPara(0);
}

//更改参数按钮休息处理函数
void USBBBE_ZKsetGUIDlg::on_pushButton_Modify_clicked()
{
    if(ui->pushButton_Modify->text() == "更改")
    {
        EnabledControls(true);
        killTimer(m_TimerID);
        on_comboBox_MudulationSys_currentIndexChanged(ui->comboBox_MudulationSys->currentIndex());
        on_comboBox_TeleCtrlPat_currentIndexChanged(ui->comboBox_TeleCtrlPat->currentIndex());
        on_comboBox_BCHEncode_currentIndexChanged(ui->comboBox_BCHEncode->currentIndex());
        on_comboBox_RandCtrl_currentIndexChanged(ui->comboBox_RandCtrl->currentIndex());

        on_comboBox_BeginCtrl_currentIndexChanged( ui->comboBox_BeginCtrl->currentIndex());
        on_comboBox_LateCtrl_currentIndexChanged(ui->comboBox_LateCtrl->currentIndex() );

        on_lineEdit_ExeCodeLen_textChanged(ui->lineEdit_ExeCodeLen->text());
        on_checkBox_BFillG1_clicked();
        on_checkBox_LFillG1_clicked();
        return;
    }

    if(ui->pushButton_Modify->text() == "恢复")
    {
        EnabledControls(false);
        m_TimerID = startTimer(500);
        return;
    }
}

void USBBBE_ZKsetGUIDlg::on_pushButton_Set_clicked()
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
            byteArray.resize(sizeof(m_MCSUSBTCParam));
            memcpy(byteArray.data(),&m_MCSUSBTCParam,sizeof(MCSUSBTCParamStruct));
            SendMessage((char*)&m_MCSUSBTCParam, sizeof(MCSUSBTCParamStruct), m_usC, m_usTID);
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

void USBBBE_ZKsetGUIDlg::closeEvent(QCloseEvent *event)
{
    event->accept();
}

QSize USBBBE_ZKsetGUIDlg::sizeHint() const
{
    return QSize(this->width(), this->height());
}

/*
//副帧方式响应函数
void USBBBE_ZKsetGUIDlg::on_comboBox_SubFrmType_currentIndexChanged(int index)
{
//    //副帧方式控件使能
//    if (ui->pushButton_Modify->text() != "更改" || iParaSetFlag != 0)
//    {
//        EnabledControlsSubFrmType(index);   //副帧方式控件使能
//    }
}

//根据副帧类型，使能副帧控件
void USBBBE_ZKsetGUIDlg::EnabledControlsSubFrmType(int iSubFrm)
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

void USBBBE_ZKsetGUIDlg::SubFrmEn(bool bFlag)
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
void USBBBE_ZKsetGUIDlg::IDSubFrmEn(bool bFlag)
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
void USBBBE_ZKsetGUIDlg::CycSubFrmEn(bool bFlag)
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
void USBBBE_ZKsetGUIDlg::ReserveSubFrmEn(bool bFlag)
{

}

void USBBBE_ZKsetGUIDlg::SendMessage(char* pData, unsigned short  usLen, unsigned short usC, unsigned short usTID)
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

DWORDLONG USBBBE_ZKsetGUIDlg::StrToSynCode(const char * pStr)
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
//调制监听
void USBBBE_ZKsetGUIDlg::on_comboBox_MudulationSys_currentIndexChanged(int index)
{
    if (ui->pushButton_Modify->text() == "更改")
        return;
    if(index==0){
        ui->label_ModulationFre->setText("调制频率");
        ui->lineEdit_SubWaveFre->setEnabled(false);
        ui->lineEdit_SubWaveFillFre->setEnabled(false);
        ui->lineEdit_SubWaveExeFre->setEnabled(false);

        ui->comboBox_CodeType->clear();
        ui->comboBox_CodeType->addItem("NRZ-L");
        ui->comboBox_CodeType->addItem("NRZ-M");
        ui->comboBox_CodeType->addItem("NRZ-S");
        ui->comboBox_CodeType->addItem("Biφ-L");
        ui->comboBox_CodeType->addItem("Biφ-M");
        ui->comboBox_CodeType->addItem("Biφ-S");
        if (m_MCSUSBTCParam.ucCodeStyle < 2)
            ui->comboBox_CodeType->setCurrentIndex(0);
        else
            ui->comboBox_CodeType->setCurrentIndex(m_MCSUSBTCParam.ucCodeStyle - 2);

        ui->lineEdit_BeforeFillCode->setEnabled(true);
        ui->lineEdit_LateFillCode->setEnabled(true);
        ui->lineEdit_ExeCode->setEnabled(true);

        ui->lineEdit_FreeCode->setEnabled(true);
        ui->lineEdit_BeforeFillLen->setEnabled(true);
        ui->lineEdit_LateFillLen->setEnabled(true);
        ui->lineEdit_ExeCodeLen->setEnabled(true);
        ui->lineEdit_BufferGap->setEnabled(true);

        ui->checkBox_BFillG1->setEnabled(true);
        ui->checkBox_LFillG1->setEnabled(true);

        ui->checkBox_BFillG2->setEnabled(false);
        ui->checkBox_LFillG2->setEnabled(false);

        ui->checkBox_BFillG3->setEnabled(true);
        ui->checkBox_LFillG3->setEnabled(true);

        ui->checkBox_BFillG4->setEnabled(false);
        ui->checkBox_LFillG4->setEnabled(false);
    }
    else{
        ui->lineEdit_SubWaveFre->setEnabled(true);
        ui->lineEdit_SubWaveFillFre->setEnabled(true);
        ui->lineEdit_SubWaveExeFre->setEnabled(true);

        ui->label_ModulationFre->setText("副载波0频 (f0)");
        ui->comboBox_CodeType->clear();
        ui->comboBox_CodeType->addItem("RZ-L");
        ui->comboBox_CodeType->addItem("NRZ-L");
        ui->comboBox_CodeType->addItem("NRZ-M");
        ui->comboBox_CodeType->addItem("NRZ-S");
        ui->comboBox_CodeType->addItem("Biφ-L");
        ui->comboBox_CodeType->addItem("Biφ-M");
        ui->comboBox_CodeType->addItem("Biφ-S");
        ui->comboBox_CodeType->setCurrentIndex(m_MCSUSBTCParam.ucCodeStyle - 1);

        ui->checkBox_BFillG1->setEnabled(false);
        ui->checkBox_LFillG1->setEnabled(false);

        ui->checkBox_BFillG2->setEnabled(false);
        ui->checkBox_LFillG2->setEnabled(false);

        ui->checkBox_BFillG3->setEnabled(false);
        ui->checkBox_LFillG3->setEnabled(false);

        ui->checkBox_BFillG4->setEnabled(false);
        ui->checkBox_LFillG4->setEnabled(false);

        ui->lineEdit_BeforeFillCode->setEnabled(false);
        ui->lineEdit_LateFillCode->setEnabled(false);
        ui->lineEdit_ExeCode->setEnabled(false);

        ui->lineEdit_FreeCode->setEnabled(true);
        ui->lineEdit_BeforeFillLen->setEnabled(false);
        ui->lineEdit_LateFillLen->setEnabled(false);
        ui->lineEdit_ExeCodeLen->setEnabled(false);
        ui->lineEdit_BufferGap->setEnabled(true);

        ui->checkBox_BFillG1->setEnabled(false);
        ui->checkBox_LFillG1->setEnabled(false);

        ui->checkBox_BFillG2->setEnabled(false);
        ui->checkBox_LFillG2->setEnabled(false);

        ui->checkBox_BFillG3->setEnabled(false);
        ui->checkBox_LFillG3->setEnabled(false);

        ui->checkBox_BFillG4->setEnabled(false);
        ui->checkBox_LFillG4->setEnabled(false);
    }
}

void USBBBE_ZKsetGUIDlg::on_comboBox_TeleCtrlPat_currentIndexChanged(int index)
{
    if (ui->pushButton_Modify->text() == "更改")
        return;
    if(index==0){
        ui->comboBox_StartingMethod->setEnabled(true);
    }
    else
    {
        ui->comboBox_StartingMethod->setEnabled(false);
        ui->comboBox_StartingMethod->setCurrentIndex(0);
    }

}

void USBBBE_ZKsetGUIDlg::on_comboBox_BCHEncode_currentIndexChanged(int index)
{
    if (ui->pushButton_Modify->text() == "更改")
        return;
    if(index==0){
        ui->comboBox_BeginCtrl->setEnabled(false);
        ui->comboBox_LateCtrl->setEnabled(false);
        ui->lineEdit_LatePattern->setEnabled(false);
        ui->lineEdit_BeginPattern->setEnabled(false);
    }else
    {
        ui->comboBox_BeginCtrl->setEnabled(true);
        ui->comboBox_LateCtrl->setEnabled(true);
        on_comboBox_BeginCtrl_currentIndexChanged(ui->comboBox_BeginCtrl->currentIndex());
        on_comboBox_LateCtrl_currentIndexChanged(ui->comboBox_LateCtrl->currentIndex());
    }

}

void USBBBE_ZKsetGUIDlg::on_comboBox_RandCtrl_currentIndexChanged(int index)
{
    if (ui->pushButton_Modify->text() == "更改")
        return;
    if(index==0){
        ui->lineEdit_RandPolyLen->setEnabled(false);
        ui->lineEdit_RandPoly->setEnabled(false);
        ui->lineEdit_RandPhase->setEnabled(false);
    }else
    {
        ui->lineEdit_RandPolyLen->setEnabled(true);
        ui->lineEdit_RandPoly->setEnabled(true);
        ui->lineEdit_RandPhase->setEnabled(true);
    }
}

void USBBBE_ZKsetGUIDlg::on_comboBox_BeginCtrl_currentIndexChanged(int index)
{
    if (ui->pushButton_Modify->text() == "更改")
        return;
    if(index==1 && ui->comboBox_BCHEncode->currentIndex() == 1){
        ui->lineEdit_BeginPattern->setEnabled(true);
    }else
    {
        ui->lineEdit_BeginPattern->setEnabled(false);
    }

}

void USBBBE_ZKsetGUIDlg::on_comboBox_LateCtrl_currentIndexChanged(int index)
{
    if (ui->pushButton_Modify->text() == "更改")
        return;
    if(index==1 && ui->comboBox_BCHEncode->currentIndex() == 1){
        ui->lineEdit_LatePattern->setEnabled(true);
    }else
    {
        ui->lineEdit_LatePattern->setEnabled(false);
    }
}



void USBBBE_ZKsetGUIDlg::on_checkBox_BFillG1_clicked()
{
    if (ui->pushButton_Modify->text() == "更改")
        return;
    if(ui->checkBox_BFillG1->checkState()){
        ui->checkBox_BFillG4->setChecked(true);
    }
    else{
        ui->checkBox_BFillG4->setChecked(false);
    }


}

void USBBBE_ZKsetGUIDlg::on_checkBox_LFillG1_clicked()
{
    if (ui->pushButton_Modify->text() == "更改")
        return;


    if(ui->checkBox_LFillG1->checkState()){
        ui->checkBox_LFillG4->setChecked(true);
    }
    else{
        ui->checkBox_LFillG4->setChecked(false);
    }
}

void USBBBE_ZKsetGUIDlg::on_lineEdit_ExeCodeLen_textChanged(const QString &arg1)
{
    //执行序列码样（Hex）
    int num = arg1.toInt();
    QRegExp regExp(QString("[A-Fa-f0-9]{1,%1}").arg(num * 2));
    ui->lineEdit_ExeCode->setValidator(new QRegExpValidator(regExp, this));
}
