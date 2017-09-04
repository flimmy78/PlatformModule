#include "DDTBBE_ModSourceSetGUIDlg.h"
#include "ui_DDTBBE_ModSourceSetGUIDlg.h"
#include <stdio.h>
#include <QMessageBox>
int RoundToInt(double lfValue){
    const double eps=1.0E-6;
    if(lfValue>eps)
        return (int)(lfValue+0.5);
    else
        return (int)(lfValue-0.5);
}
int DDTBBE_ModSourceSetGUIDlg::msgBoxFunction(QMessageBox &msgBox, QString msg, BOOL ret)
{
    msgBox.setText(msg);
    msgBox.exec();
    return ret;
}
int DDTBBE_ModSourceSetGUIDlg::FspLenIndexToValue(int nIndex)
{
    INT nValue = 16;
    switch(nIndex)
    {
    case 0:
        nValue = 16;
        break;
    case 1:
        nValue = 24;
        break;
    case 2:
        nValue = 32;
        break;
    default:
        break;
    }

    return nValue;
}
int DDTBBE_ModSourceSetGUIDlg::CheckParam()
{

    QMessageBox msgBox;
    msgBox.setWindowTitle(MODULENAME);

    QString strTmp;


    if(ui->lineEdit_NoiseRatio->text().toFloat() < 30 || (ui->lineEdit_NoiseRatio->text().toFloat() > 80) )
    {
        strTmp = QString("输出载噪比范围:[30～80]，单位[dBHz]，请重新输入!");
        msgBox.setText(strTmp);
        msgBox.exec();

        return FALSE;
    }
    int m_cLevel=ui->lineEdit_OutputPower->text().toInt();
    if( ( m_cLevel < -80 ) || ( m_cLevel > 0 ) )
    {
        strTmp = QString("输出功率范围:[-80～0]，单位[dBm]，请重新输入!");
        msgBox.setText(strTmp);
        msgBox.exec();

        return FALSE;
    }
    int m_ucModesel = ui-> comboBox_MudulationSystem->currentIndex();
    if ((m_ucModesel < 0) || (m_ucModesel > 4))
    {

        strTmp = QString("请选择调制体制!");
        msgBox.setText(strTmp);
        msgBox.exec();

        return FALSE;
    }
    QString m_ucSignalSel=ui->comboBox_IOMethod->currentText();
    if(ui->lineEdit_IORatio->text().isEmpty())
    {
        return msgBoxFunction(msgBox,"I/Q功率比输入为空，请重新输入!", FALSE);
    }
    if (m_ucSignalSel.isEmpty())
    {
        strTmp = QString( "请选择I/Q分路合路!");
        msgBox.setText(strTmp);
        msgBox.exec();
        return FALSE;
    }
    int m_ucPowerType=ui->comboBox_PowerFlag->currentIndex();
    int m_lfRatiodB=ui->lineEdit_IORatio->text().toInt();
    if(m_ucPowerType == 0)
    {
        if(( m_lfRatiodB < -10.0 ) || ( m_lfRatiodB > 10.0 ))
        {
            strTmp = QString( "I/Q功率分贝比[-10~10]dB，请重新输入!");
            msgBox.setText(strTmp);
            msgBox.exec();
            return FALSE;
        }
    }

    int m_ucModu=ui->comboBox_MudulatingSwitch->currentIndex();
    if ((m_ucModu < 0) || (m_ucModu > 1))
    {
        strTmp = QString( "请选择调制开关!");
        msgBox.setText(strTmp);
        msgBox.exec();

        return FALSE;
    }
    int m_ucOnline=ui->comboBox_WaveSwitch->currentIndex();
    if ((m_ucOnline < 0) || (m_ucOnline > 1))
    {
        strTmp = QString("请选择载波输出开关!");
        msgBox.setText(strTmp);
        msgBox.exec();

        return FALSE;
    }

    int m_ucNoiseSw=ui->comboBox_NoiseOutSwitch->currentIndex();
    if ((m_ucNoiseSw < 0) || (m_ucNoiseSw > 1))
    {
        strTmp = QString("请选择噪声输出开关!");
        msgBox.setText(strTmp);
        msgBox.exec();


        return FALSE;
    }
    int m_ucSimSw=ui->comboBox_DopplerSwitch->currentIndex();
    if ((m_ucSimSw < 0) || (m_ucSimSw > 1))
    {

        strTmp = QString("请选择模拟多普勒开关!");
        msgBox.setText(strTmp);
        msgBox.exec();

        return FALSE;
    }

    if(ui->lineEdit_DopplerRange->text().isEmpty())
    {
        return msgBoxFunction(msgBox,"模拟多普勒范围输入为空，请重新输入!", FALSE);
    }
    if(ui->lineEdit_DopplerGrident->text().isEmpty())
    {
        return msgBoxFunction(msgBox,"模拟多普勒变化率输入为空，请重新输入!", FALSE);
    }
    if(m_ucSimSw == 0)
    {
        int m_usSimRange=ui->lineEdit_DopplerRange->text().toInt();
        if ((m_usSimRange < 0) || (m_usSimRange > 600))
        {

            strTmp = QString("模拟多普勒范围:[0～600]kHz，单位[kHz]，请重新输入!");
            msgBox.setText(strTmp);
            msgBox.exec();
            return FALSE;
        }

        if ((ui->lineEdit_DopplerGrident->text().toInt() < 0) || (ui->lineEdit_DopplerGrident->text().toInt() > 15000))
        {
            strTmp = QString("模拟多普勒变化率范围:[0～15000]Hz/s，请重新输入!");
            msgBox.setText(strTmp);
            msgBox.exec();
            return FALSE;
        }
    }


    if (1 == m_ucModesel) // QPSK
    {
        int m_ucGrayType=ui->comboBox_GRAYType->currentIndex();
        if ((m_ucGrayType < 0) || (m_ucGrayType > 8))
        {
            strTmp = QString("请选择Gray编码选择!");
            msgBox.setText(strTmp);
            msgBox.exec();


            return FALSE;
        }
    }

    if ((m_ucPowerType < 0) || (m_ucPowerType > 1))
    {
        strTmp = QString("请选择功率比有效标志!");
        msgBox.setText(strTmp);
        msgBox.exec();


        return FALSE;
    }

    if(m_ucPowerType == 0)
    {
        if( ( m_lfRatiodB < -10.0 ) || ( m_lfRatiodB > 10.0 ) )
        {
            strTmp = QString("I/Q功率分贝比[-10~10]dB，请重新输入!");
            msgBox.setText(strTmp);
            msgBox.exec();


            return FALSE;
        }
    }
    else
    {
        int m_ucIPowRate=ui->comboBox_IPowerFactor->currentIndex();
        if( ( m_ucIPowRate < 0 ) || ( m_ucIPowRate > 9 ) )
        {
            strTmp = QString("I路功率因子:[1～10]，请重新输入!");
            msgBox.setText(strTmp);
            msgBox.exec();



            return FALSE;
        }
        int m_ucQPowRate= ui->comboBox_QPowerFactor->currentIndex();
        if( ( m_ucQPowRate < 0 ) || ( m_ucQPowRate > 9 ) )
        {
            strTmp = QString("Q路功率因子:[1～10]，请重新输入!");
            msgBox.setText(strTmp);
            msgBox.exec();


            return FALSE;
        }
    }

    ULONG tempData, tempData1;
    ULONG tempCodeRate1, tempCodeRate2;
    USHORT JiSuanFrmLen = 0;
    USHORT FrmSizeMin_I = 4;
    USHORT FrmSizeMin_Q = 4;
    USHORT FrmSizeMax_I = 2048;
    USHORT FrmSizeMax_Q = 2048;

    // I路参数
    int m_ucFspPlace_I = ui->comboBox_FrameSyncGroupPos->currentIndex();
    if ((m_ucFspPlace_I < 0) || (m_ucFspPlace_I > 1)) // 帧同步码组位置
    {

        strTmp = QString("请选择帧同步码组位置！");
        msgBox.setText(strTmp);
        msgBox.exec();


        return FALSE;
    }

    JiSuanFrmLen = 0;
    int m_ucWordLen_I = 8;
    if( ( m_ucWordLen_I < 4 ) || ( m_ucWordLen_I > 16 ) )
    {
        strTmp = QString("字长范围[4~16]，重新输入!");
        msgBox.setText(strTmp);
        msgBox.exec();


        return FALSE;
    }

    // 码率范围计算
    tempCodeRate1 = 64000;
    tempCodeRate2 = 20000000;
    if (m_ucModesel == 0) //BPSK
    {
        tempCodeRate1 = tempCodeRate1 / 2;
        tempCodeRate2 = tempCodeRate2 / 2;
    }
    else if ((m_ucModesel == 1) || (m_ucModesel == 3))
    {
        if(m_ucSignalSel == "I/Q分路")
        {
            tempCodeRate1 = tempCodeRate1 / 2;
            tempCodeRate2 = tempCodeRate2 / 2;
        }
    }
    else if (m_ucModesel == 2)
    {
        tempCodeRate1 = tempCodeRate1 / 2;
        tempCodeRate2 = tempCodeRate2 / 2;
    }

    // 根据卷积码类型调整码率范围
    int m_ucEncodeType_I = ui->comboBox_Enctype->currentIndex();
    if ((1 == m_ucEncodeType_I) || (3 == m_ucEncodeType_I))
    {
        int m_ucConvol_I = ui->comboBox_ConcodeType->currentIndex();
        if(0 == m_ucConvol_I)
        {
            //			tempCodeRate1 = tempCodeRate1/2;
            tempCodeRate2 = tempCodeRate2 / 2;
        }
        else if(1 == m_ucConvol_I)
        {
            //			tempCodeRate1 = tempCodeRate1*3/4;
            tempCodeRate2 = tempCodeRate2 * 3/4;
        }
    }
    float m_lfCodeRate_I=ui->lineEdit_BitRate->text().toFloat();
    if((m_lfCodeRate_I * 1000 < tempCodeRate1) || (m_lfCodeRate_I * 1000 > tempCodeRate2))
    {
        if(tempCodeRate1 < 100000)
        {

            strTmp= QString("I路模拟源，码速率范围[%1~%2]kbps，请重新输入!").arg(tempCodeRate1 / 1000).arg(tempCodeRate2 / 1000);

        }
        else
        {

            strTmp= QString("I路模拟源，码速率范围[%1~%2]kbps，请重新输入!").arg(tempCodeRate1 / 1000).arg(tempCodeRate2 / 1000);


        }
        msgBox.setText(strTmp);
        msgBox.exec();
        return FALSE;
    }

    if( ( m_ucEncodeType_I < 0 ) || ( m_ucEncodeType_I > 6 ) )
    {

        strTmp = QString("请选择编码方式!");
        msgBox.setText(strTmp);
        msgBox.exec();
        return FALSE;
    }

    // 帧同步码组长
    int m_ucFspLen_I=ui->comboBox_FrameSyncGroupLen->currentIndex();
    if ((m_ucFspLen_I < 0) || (m_ucFspLen_I > 2))
    {

        strTmp = QString("请选择帧同步码组长！");
        msgBox.setText(strTmp);
        msgBox.exec();

        return FALSE;
    }

    ///////////////////////////////////////////////////////////////////////////

    //int m_ucWordLen_I = 8;


    // 注释掉原先的帧长计算，改用下述方法
    m_ucFspLen_I = ui->comboBox_FrameSyncGroupLen->currentIndex();
    INT nTempFspLen = m_ucFspLen_I + 2;//帧码组字节数
    INT nK = ui->comboBox_RSType->currentIndex() == 0? 223 : 239;
    int m_ucInterleaver_I = ui->lineEdit_RSmixDeep->text().toInt();
    INT nMaxVFLen = (nK-1) * m_ucInterleaver_I;
    if(ui->lineEdit_FrameLen->text().isEmpty())
    {
        return msgBoxFunction(msgBox, "帧长不能为空！", FALSE);
    }
    int m_usFrmLen_I = ui->lineEdit_FrameLen->text().toInt();
    int m_nVFLen_I = 255 * m_ucInterleaver_I+nTempFspLen - m_usFrmLen_I;

    // 帧长与RS编码有关，须判断完RS编码再判
    if ((2 == m_ucEncodeType_I) || (3 == m_ucEncodeType_I)) // RS译码或RS和Viterbi级联译码
    {
        FrmSizeMax_I = 255 * m_ucInterleaver_I + nTempFspLen;
        FrmSizeMin_I = 255 * m_ucInterleaver_I + nTempFspLen - nMaxVFLen;
        int m_ucInterleaver_I=ui->lineEdit_RSmixDeep->text().toInt();
        if ((m_ucInterleaver_I < 1) || (m_ucInterleaver_I > 5))
        {
            strTmp = QString("RS交织深度范围[1~5]，请重新输入！");
            msgBox.setText(strTmp);
            msgBox.exec();
            return FALSE;
        }
        // 增加虚拟填充和交织深度关系判断
        if (m_nVFLen_I % m_ucInterleaver_I != 0)
        {

            strTmp = "帧长，交织深度，RS码类型三者不匹配，请重新输入!\n\
                    (255*交织深度 + 帧码组长(字节) - 帧长(字节) = 交织深度*N)";

                     return msgBoxFunction(msgBox, strTmp, false);

        }
    }
    if(m_usFrmLen_I < FrmSizeMin_I || m_usFrmLen_I > FrmSizeMax_I )
    {
        strTmp = QString("此时帧长范围应该为: %1 ~ %2").arg(FrmSizeMin_I)
                .arg(FrmSizeMax_I);
        return msgBoxFunction(msgBox, strTmp, false);
    }

    QString m_dwlFsp_I = ui->lineEdit_FrameSyncGroup->text();
    UI64  tempSynCode = 0;
    UI64  tempSynCode1 = 0;
    UINT  tempSynCode2 = 0;
    UINT  tempSynCode3 = 0;
    tempSynCode1 = StrToSynCode(m_dwlFsp_I.toStdString().c_str());
    tempSynCode = tempSynCode1<<(64 - FspLenIndexToValue(m_ucFspLen_I));
    tempSynCode = tempSynCode>>(64 - FspLenIndexToValue(m_ucFspLen_I));
    if(tempSynCode != tempSynCode1)
    {
        strTmp = "I路模拟源，输入的子帧码组的长度大于子帧码组长!";
        return msgBoxFunction(msgBox, strTmp, false);
    }
    m_dwlFsp_I = ui->lineEdit_FrameSyncGroup->text();
    if(m_dwlFsp_I.isEmpty())
    {
        strTmp = ("I路模拟源，帧同步码组不能为空!");
        return msgBoxFunction(msgBox, strTmp, false);
    }

    // 加扰控制
    int m_ucRandom_I = ui->comboBox_AddDisturbCtrl->currentIndex();
    if ((m_ucRandom_I < 0) || (m_ucRandom_I > 2))
    {
        strTmp = "请选择加扰控制！";
        return msgBoxFunction(msgBox, strTmp, false);
    }

    if(2 == m_ucRandom_I) // 人工设置
    {
        QString m_ucRandPoly_I = ui->lineEdit_DisturbPoly->text();//加扰多项式
        if(m_ucRandPoly_I.isEmpty())
        {

            strTmp = ("I路模拟源，加扰多项式不能为空!");
            return msgBoxFunction(msgBox, strTmp, false);
        }
        tempSynCode2 = (ULONG)StrToSynCode(m_ucRandPoly_I.toStdString().c_str());
        int m_ucRandPolyLen_I = ui->comboBox_DisturbPolyLen->currentIndex();
        tempSynCode3 =tempSynCode2<<(32-(m_ucRandPolyLen_I+1)*8);
        tempSynCode3 =tempSynCode3>>(32-(m_ucRandPolyLen_I+1)*8);
        if(tempSynCode2 != tempSynCode3)
        {
            strTmp = QString("I路模拟源，加扰多项式的长度大于%1字节，请重新输入!")
                    .arg(QString::number(m_ucRandPolyLen_I + 1));
            return msgBoxFunction(msgBox, strTmp, false);
        }

        QString m_ulRandPhase_I = ui->lineEdit_DisturbPhasePos->text();//加扰初相
        if(m_ulRandPhase_I.isEmpty())
        {

            strTmp = ("I路模拟源，加扰初相不能为空!");
            return msgBoxFunction(msgBox, strTmp, false);
        }
        tempSynCode2 = (ULONG)StrToSynCode(m_ulRandPhase_I.toStdString().c_str());
        int m_ucRandPhaseLen_I = (ui->comboBox_DisturbPolyLen->currentIndex() + 1);//字节数
        tempSynCode3 =tempSynCode2<<(32 - m_ucRandPhaseLen_I*8);
        tempSynCode3 =tempSynCode3>>(32 - m_ucRandPhaseLen_I*8);
        if(tempSynCode2 != tempSynCode3)
        {
            strTmp = QString("I路模拟源，加扰初相的长度大于%1字节，重新输入!").arg(m_ucRandPhaseLen_I);
            return msgBoxFunction(msgBox, strTmp, false);
        }
    }
    int m_ucSubFrmType_I = ui->comboBox_SubFrameSyncType->currentIndex();
    if (m_ucSubFrmType_I < 0 || m_ucSubFrmType_I > 3)
    {
        strTmp = ("请选择副帧同步方式！");
        return msgBoxFunction(msgBox, strTmp, false);
    }
    int m_usSubFrmLen_I = ui->lineEdit_SubFrameLen->text().toInt();
    if( ( m_usSubFrmLen_I < 4 ) || ( m_usSubFrmLen_I > 256 ) )
    {
        strTmp = ("I路模拟源，副帧长范围为[4~256]，重新输入!");
        return msgBoxFunction(msgBox, strTmp, false);
    }

    //ID副帧和循环副帧的副帧位置
    m_ucFspPlace_I = ui->comboBox_FrameSyncGroupPos->currentIndex();
    m_usFrmLen_I = ui->lineEdit_FrameLen->text().toInt();//帧长
    m_ucFspLen_I = ui->comboBox_FrameSyncGroupLen->currentIndex() + 2;//帧同步码组长
    if (1 == m_ucSubFrmType_I )
    {
        int m_usIdPlace_I = ui->lineEdit_IDCountPos->text().toInt();
        if(!checkIDSubFrm(m_usFrmLen_I, m_ucFspLen_I, m_ucFspPlace_I,m_usIdPlace_I))
        {
            strTmp = ("I路模拟源，ID 字位置不能小于1，ID 字位置不能大于子帧长，并且占用同步码组的位置!");
            return msgBoxFunction(msgBox, strTmp, false);
        }
    }
    else if(2 == m_ucSubFrmType_I) //循环副帧的副帧位置
    {
        int m_usCycFspPlace_I = ui->lineEdit_SubFrameGroupPos->text().toInt();
        int m_iCycFspLen_I = ui->comboBox_SubFrameGroupLen->currentIndex() + 1;
        if(((m_iCycFspLen_I) < 8/8) || ((m_iCycFspLen_I) > 32/8))
        {
            strTmp = ("循环副帧同步码组长[1~4]字节!");
            return msgBoxFunction(msgBox, strTmp, false);
        }
        if(!checkCycSubFrm(m_usFrmLen_I,m_ucFspLen_I,m_ucFspPlace_I,m_iCycFspLen_I,m_usCycFspPlace_I))
        {
            strTmp = ("I路模拟源，循环码组位置不能小于1，循环码组位置不能大于子帧长，并且不能占用子帧码组位置!");
            return msgBoxFunction(msgBox, strTmp, false);
        }
        QString m_dwlCycFsp_I = ui->lineEdit_SubFrameSyncGroup->text();
        if(m_dwlCycFsp_I.isEmpty())
        {
            strTmp = (" 循环码组不能空!");
            return msgBoxFunction(msgBox, strTmp, false);
        }
        tempSynCode1 = (ULONG)StrToSynCode(m_dwlCycFsp_I.toStdString().c_str());
        tempSynCode =tempSynCode1<<(64-(m_iCycFspLen_I)*8);
        tempSynCode =tempSynCode>>(64-(m_iCycFspLen_I)*8);
        if(tempSynCode != tempSynCode1)
        {
            strTmp = ("循环码组的长不能大于设置的码组长!");
            return msgBoxFunction(msgBox, strTmp, false);
        }
    }
    int m_ucdatasel_I = ui->comboBox_DataType->currentIndex();
    if(0 == m_ucdatasel_I)
    {
        QString m_uldata_I = ui->lineEdit_IAnologData->text();
        if(m_uldata_I.isEmpty())
        {
            strTmp = ("I路模拟源，固定数不能为空!");
            return msgBoxFunction(msgBox, strTmp, false);
        }

        tempData = (ULONG)StrToSynCode(m_uldata_I.toStdString().c_str());
        tempData1 =tempData<<(sizeof(ULONG)*8 - m_ucWordLen_I);
        tempData1 =tempData1>>(sizeof(ULONG)*8 - m_ucWordLen_I);
        if(tempData != tempData1)
        {
            strTmp = ("I路模拟源，固定数的长大于字长，重新输入!");
            return msgBoxFunction(msgBox, strTmp, FALSE);
        }
    }

//    int m_ucdatasel_Q = ui->comboBox_ODataType->currentIndex();
//    if(0 == m_ucdatasel_Q)
//    {
//        QString m_uldata_Q = ui->lineEdit_OAnologData->text();
//        if(m_uldata_Q.isEmpty())
//        {
//            strTmp = QString("Q路模拟源，固定数不能为空!");
//            return msgBoxFunction(msg, strTmp, FALSE);
//        }
//        tempData = (ULONG)StrToSynCode(m_uldata_Q.toStdString().c_str());
//        tempData1 =tempData<<(sizeof(ULONG)*8 - m_ucWordLen_Q);
//        tempData1 =tempData1>>(sizeof(ULONG)*8 - m_ucWordLen_Q);
//        if (tempData != tempData1)
//        {
//            strTmp=QString("Q路模拟源，固定数的长大于字长，重新输入!");
//            return msgBoxFunction(msg, strTmp, FALSE);
//        }
//    }

    // 无Q路则检查通过
    if(!m_bCheckQ)
    {
        return TRUE;
    }

    int m_ucWordLen_Q =8;
    // Q路参数
    int m_ucFspPlace_Q = ui->comboBox_OFrameSyncGroupPos->currentIndex();
    if ((m_ucFspPlace_Q < 0) || (m_ucFspPlace_Q > 1)) // 帧同步码组位置
    {
        strTmp = ("请选择帧同步码组位置！");
        return msgBoxFunction(msgBox, strTmp, false);
    }

    JiSuanFrmLen = 0;
    if( ( m_ucWordLen_Q < 4 ) || ( m_ucWordLen_Q > 16 ) )
    {
        strTmp = ("字长范围[4~16]，重新输入!");
        return msgBoxFunction(msgBox, strTmp, false);
    }

    // 码率范围计算
    qint64 tempCodeRate3 = 64000;
    qint64 tempCodeRate4 = 20000000;

    if (m_ucModesel == 0) //BPSK
    {
        tempCodeRate3 = tempCodeRate3/2;
        tempCodeRate4 = tempCodeRate4/2;
    }
    else if ((m_ucModesel == 1) || (m_ucModesel == 2))
    {
        if(m_ucSignalSel == "I/Q分路")
        {
            tempCodeRate3 = tempCodeRate3/2;
            tempCodeRate4 = tempCodeRate4/2;
        }
    }
    else if (m_ucModesel == 3)
    {
        tempCodeRate3 = tempCodeRate3/2;
        tempCodeRate4 = tempCodeRate4/2;
    }
    /////////////////////////////////////////////////////////////
    // 根据卷积码类型调整码率范围1950
    int m_ucEncodeType_Q = ui->comboBox_OEnctype->currentIndex();
    int m_ucConvol_Q = ui->comboBox_OConcodeType->currentIndex();
    if ((1 == m_ucEncodeType_Q) || (3 == m_ucEncodeType_Q))
    {
        if(0 == m_ucConvol_Q)
        {
            //			tempCodeRate3 = tempCodeRate3/2;
            tempCodeRate4 = tempCodeRate4/2;
        }
        else if(1 == m_ucConvol_Q)
        {
            //			tempCodeRate3 = tempCodeRate3*3/4;
            tempCodeRate4 = tempCodeRate4*3/4;
        }
    }
    double m_lfCodeRate_Q = ui->lineEdit_OBitRate->text().toDouble();
    if((m_lfCodeRate_Q*1000 < tempCodeRate3) || (m_lfCodeRate_Q*1000 > tempCodeRate4))
    {
        strTmp = QString("Q路模拟源，码速率范围[%1~%2]kbps，请重新输入!").arg(tempCodeRate3/1000).arg(tempCodeRate4/1000);
        return msgBoxFunction(msgBox,strTmp, FALSE);
    }

    if( ( m_ucEncodeType_Q < 0 ) || ( m_ucEncodeType_Q > 6 ) )
    {
        return msgBoxFunction(msgBox, "请选择编码方式!", FALSE);
    }
    QMessageBox& msg = msgBox;
    // 帧同步码组长
    int m_usFrmLen_Q = ui->lineEdit_OFrameLen->text().toInt();
    int m_ucFspLen_Q = ui->comboBox_OFrameSyncGroupLen->currentIndex();
    if ((m_ucFspLen_Q < 0) || (m_ucFspLen_Q > 2))
    {
        return msgBoxFunction(msg, "请选择帧同步码组长！!", FALSE);
    }

    /*	if (m_ucFspLen_Q % m_ucWordLen_Q !=0 )
        {
            MessageBox("帧同步码组长应该是字长的整数倍!",MyTitleDefineInFrame,MB_OK | MB_ICONWARNING);
            SetIDCFocus(IDC_COMBO_FSPLEN_Q);
            return FALSE;
        }
    */
    int m_ucInterleaver_Q = ui->lineEdit_ORSmixDeep->text().toInt();
    if ((2 == m_ucEncodeType_Q) || (3 == m_ucEncodeType_Q)) // RS译码或RS和Viterbi级联译码
    {
        if ((m_ucInterleaver_Q < 1) || (m_ucInterleaver_Q > 5))
        {
            return msgBoxFunction(msg, "RS交织深度范围[1~5]，请重新输入！", FALSE);
        }

        // Michael Woo 20101105系统联试修改 704所联试旁小黄楼
        // 注释掉原先的帧长计算，改用下述方法
        INT nTempFspLen = m_ucFspLen_Q + 2;
        INT nK = ui->comboBox_ORSType->currentIndex() == 0 ? 223 : 239;
        INT nMaxVFLen = (nK - 1)*m_ucInterleaver_Q;
        FrmSizeMax_Q = 255*m_ucInterleaver_Q + nTempFspLen;
        FrmSizeMin_Q = 255*m_ucInterleaver_Q + nTempFspLen - nMaxVFLen;
        int m_nVFLen_Q = 255*m_ucInterleaver_Q + nTempFspLen - m_usFrmLen_Q;

        // Michael Woo 20101105系统联试修改704所联试旁小黄楼
        // 增加虚拟填充和交织深度关系判断
        if (m_nVFLen_Q % m_ucInterleaver_Q != 0)
        {
            strTmp = "帧长，交织深度，RS码类型三者不匹配，请重新输入!\n\
                    (255*交织深度 + 帧码组长(字节) - 帧长(字节) = 交织深度*N)";
                     return msgBoxFunction(msg, strTmp, FALSE);
        }
        // 帧长与RS编码有关，须判断完RS编码再判
        m_usFrmLen_Q = ui->lineEdit_OFrameLen->text().toInt();
        if(m_usFrmLen_Q < FrmSizeMin_Q || m_usFrmLen_Q > FrmSizeMax_Q)
        {
            strTmp = QString("此时帧长范围应该为: %1 ~ %2").arg(FrmSizeMin_Q).arg(FrmSizeMax_Q);
            return msgBoxFunction(msg, strTmp, FALSE);
        }
    }
    if (m_usFrmLen_Q < FrmSizeMin_Q || m_usFrmLen_Q > FrmSizeMax_Q)
    {
        strTmp = QString("此时帧长范围应该为: %1 ~ %2").arg(FrmSizeMin_Q).arg(FrmSizeMax_Q);
        return msgBoxFunction(msg, strTmp, FALSE);
    }

    tempSynCode = 0;
    tempSynCode1 = 0;
    tempSynCode2 = 0;
    tempSynCode3 = 0;
    QString m_dwlFsp_Q = ui->lineEdit_OFrameSyncGroup->text();
    m_ucFspLen_Q = ui->comboBox_OFrameSyncGroupLen->currentIndex();
    tempSynCode1 = StrToSynCode(m_dwlFsp_Q.toStdString().c_str());
    tempSynCode = tempSynCode1<<(64 - FspLenIndexToValue(m_ucFspLen_Q));
    tempSynCode = tempSynCode>>(64 - FspLenIndexToValue(m_ucFspLen_Q));
    if(tempSynCode != tempSynCode1)
    {
        strTmp = QString("Q路模拟源，输入的子帧码组的长度大于子帧码组长!");
        return msgBoxFunction(msg, strTmp, FALSE);
    }

    if(m_dwlFsp_Q.isEmpty())
    {
        strTmp = QString("Q路模拟源，帧同步码组不能为空!");
        return msgBoxFunction(msg, strTmp, FALSE);
    }

    // 加扰控制
    int m_ucRandom_Q = ui->comboBox_OAddDisturbCtrl->currentIndex();
    if ((m_ucRandom_Q < 0) || (m_ucRandom_Q > 2))
    {
        strTmp=QString("请选择加扰控制！");
        return msgBoxFunction(msg, strTmp, FALSE);
    }

    if(2 == m_ucRandom_Q) // 人工设置
    {
        int m_ucRandPolyLen_Q = ui->comboBox_ODisturbPolyLen->currentIndex();
        if(m_ucRandPolyLen_Q < 0 || m_ucRandPolyLen_Q > 3)
        {
            strTmp=QString("请选择Q路模拟源加扰多项式长度!");
            return msgBoxFunction(msg, strTmp, FALSE);
        }
        QString m_ucRandPoly_Q = ui->lineEdit_ODisturbPoly->text();
        if(m_ucRandPoly_Q.isEmpty())
        {
            strTmp=QString("Q路模拟源，加扰多项式不能为空!");
            return msgBoxFunction(msg, strTmp, FALSE);
        }
        tempSynCode2 = (ULONG)StrToSynCode(m_ucRandPoly_Q.toStdString().c_str());
        tempSynCode3 =tempSynCode2<<(32-(m_ucRandPolyLen_Q+1)*8);
        tempSynCode3 =tempSynCode3>>(32-(m_ucRandPolyLen_Q+1)*8);
        if(tempSynCode2 != tempSynCode3)
        {
            strTmp=QString("请Q路模拟源，加扰多项式的长度大于%1字节，请重新输入!").arg(m_ucRandPolyLen_Q + 1);
            return msgBoxFunction(msg, strTmp, FALSE);
        }
        QString m_ulRandPhase_Q = ui->lineEdit_ODisturbPhasePos->text();
        if(m_ulRandPhase_Q.isEmpty())
        {
            strTmp=QString("Q路模拟源，加扰初相不能为空!");
            return msgBoxFunction(msg, strTmp, FALSE);
        }
        tempSynCode2 = (ULONG)StrToSynCode(m_ulRandPhase_Q.toStdString().c_str());
        int m_ucRandPhaseLen_Q = (ui->comboBox_ODisturbPolyLen->currentIndex() + 1);//字节数
        tempSynCode3 =tempSynCode2<<(32 - m_ucRandPhaseLen_Q*8);
        tempSynCode3 =tempSynCode3>>(32 - m_ucRandPhaseLen_Q*8);
        if(tempSynCode2 != tempSynCode3)
        {
            strTmp=QString("Q路模拟源，加扰初相的长度大于%1字节，重新输入!").arg(m_ucRandPhaseLen_Q);
            return msgBoxFunction(msg, strTmp, FALSE);
        }
    }
    int m_ucSubFrmType_Q = ui->comboBox_OSubFrameSyncType->currentIndex();
    if ((m_ucSubFrmType_Q < 0) || (m_ucSubFrmType_Q > 3))
    {
        strTmp=QString("请选择副帧同步方式！");
        return msgBoxFunction(msg, strTmp, FALSE);
    }
    int m_usSubFrmLen_Q = ui->lineEdit_OSubFrameLen->text().toInt();
    if( ( m_usSubFrmLen_Q < 4 ) || ( m_usSubFrmLen_Q > 256 ) )
    {
        strTmp=QString("Q路模拟源，副帧长范围为[4~256]，重新输入!");
        return msgBoxFunction(msg, strTmp, FALSE);
    }

    //字节
    int frmLenQ = ui->lineEdit_OFrameLen->text().toInt();
    int fspLenQ = ui->comboBox_OFrameSyncGroupLen->currentIndex() + 2;

    //ID副帧和循环副帧的副帧位置
    if (1 == m_ucSubFrmType_Q )
    {

        int m_usIdPlace_Q = ui->lineEdit_OIDCountPos->text().toInt();
        if(!checkIDSubFrm(frmLenQ,fspLenQ,m_ucFspPlace_Q,m_usIdPlace_Q))
        {
            strTmp = QString("Q路模拟源，ID字位置不能小于1，ID 字位置不能大于子帧长，并且占用同步码组的位置!");
            return msgBoxFunction(msg, strTmp, FALSE);
        }
    }
    else if(2 == m_ucSubFrmType_Q)
    {
        int m_iCycFspLen_Q = ui->comboBox_OSubFrameGroupLen->currentIndex() + 1;
        if(((m_iCycFspLen_Q) < 8/8) || ((m_iCycFspLen_Q) > 32/8))
        {
            strTmp=QString("循环副帧同步码组长[1~4]字节!");
            return msgBoxFunction(msg, strTmp, FALSE);
        }
        int subFspPosQ = ui->lineEdit_OSubFrameGroupPos->text().toInt();
        if(!checkCycSubFrm(frmLenQ,fspLenQ,m_ucFspPlace_Q,m_iCycFspLen_Q,subFspPosQ))
        {
            strTmp=QString("Q路模拟源，循环码组位置不能小于1，循环码组位置不能大于子帧长，并且不能占用子帧码组位置!");
            return msgBoxFunction(msg, strTmp, FALSE);
        }
        QString m_dwlCycFsp_Q = ui->lineEdit_OSubFrameSyncGroup->text();
        if(m_dwlCycFsp_Q.isEmpty())
        {
            strTmp=QString("循环码组不能空!");
            return msgBoxFunction(msg, strTmp, FALSE);
        }
        tempSynCode1 = StrToSynCode(m_dwlCycFsp_Q.toStdString().c_str());
        tempSynCode =tempSynCode1<<(64-(m_iCycFspLen_Q)*8);
        tempSynCode =tempSynCode>>(64-(m_iCycFspLen_Q)*8);
        if(tempSynCode != tempSynCode1)
        {
            strTmp=QString("循环码组的长不能大于设置的码组长!");
            return msgBoxFunction(msg, strTmp, FALSE);
        }
    }
    int m_ucdatasel_Q = ui->comboBox_ODataType->currentIndex();
    if(0 == m_ucdatasel_Q)
    {
        QString m_uldata_Q = ui->lineEdit_OAnologData->text();
        if(m_uldata_Q.isEmpty())
        {
            strTmp = QString("Q路模拟源，固定数不能为空!");
            return msgBoxFunction(msg, strTmp, FALSE);
        }
        tempData = (ULONG)StrToSynCode(m_uldata_Q.toStdString().c_str());
        tempData1 =tempData<<(sizeof(ULONG)*8-m_ucWordLen_Q);
        tempData1 =tempData1>>(sizeof(ULONG)*8-m_ucWordLen_Q);
        if (tempData != tempData1)
        {
            strTmp=QString("Q路模拟源，固定数的长大于字长，重新输入!");
            return msgBoxFunction(msg, strTmp, FALSE);
        }
    }
    return 1;
}


bool DDTBBE_ModSourceSetGUIDlg::checkCycSubFrm(int frmLen, int fspLen, int fspPos, int subFspLen, int subFspPos)
{
    if(fspPos==1) // 帧尾
    {
        return subFspPos > 0 && subFspPos <= frmLen - fspLen - subFspLen + 1;
    }
    else//帧首
    {
        return fspLen < subFspPos && frmLen - subFspLen + 1 >= subFspPos;
    }
}
bool DDTBBE_ModSourceSetGUIDlg::checkIDSubFrm(int frmLen, int fspLen, int fspPos,int countPos)
{
    if(fspPos==1) // 帧尾
    {
        return  countPos >= 1 && countPos <= frmLen - fspLen;
    }
    else //帧首
    {
        return countPos <= frmLen && countPos > fspLen;
    }
}

