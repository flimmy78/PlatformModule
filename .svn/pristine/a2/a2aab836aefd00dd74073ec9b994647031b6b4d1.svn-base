#include "SS2BBE_TMSimu1ParaSetGUIDlg.h"
#include "ui_SS2BBE_TMSimu1ParaSetGUIDlg.h"
#include <QCloseEvent>
#include <QMessageBox>
#include <QDateTime>
#include <QtCore>
#include "../../Common/Log.h"
//副载波频率值域
const   UINT    CARRIERFREQ_MIN = 5;
const   UINT    CARRIERFREQ_MAX = 512;

//码率值域
const   ULONG    BITSRATE_MIN = 100;
const   ULONG    BITSRATE_MAX = 64000;

//码组长度值域
const   BYTE   PATTERNLEN_MAX = 32;//
const   BYTE   PATTERNLEN_MIN = 8;

//帧长值域
const   USHORT   FRAMELEN_MIN = 4;
const   USHORT   FRAMELEN_MAX = 2048;

//字长值域
const   BYTE   WORDLEN_MAX = 16;
const   BYTE   WORDLEN_MIN = 4;

//副帧长度值域
const   USHORT   SUBFRMLEN_MAX = 256;
const   USHORT   SUBFRMLEN_MIN = 4;

//副帧码组长度值域
const   BYTE   SUBFPATTERNLEN_MAX = 32;
const   BYTE   SUBFPATTERNLEN_MIN = 8;
QString strTmp;

int SS2BBE_TMSimu1ParaSetGUIDlg::msgBoxFunction(QMessageBox &msgBox, QString msg, BOOL ret)
{
    msgBox.setText(msg);
    msgBox.exec();
    return ret;
}
BOOL SS2BBE_TMSimu1ParaSetGUIDlg::checkStringLen(const int lenInByte, QString& str)
{
    return str.size() <=  2*lenInByte && !str.isEmpty();
}
BOOL SS2BBE_TMSimu1ParaSetGUIDlg::CheckParam()
{
    QMessageBox msgBox;
    msgBox.setWindowTitle(MODULENAME);
    QString strTmp;
    int JiSuanFrmLen =0;
    //固定数设置 m_ConstVal.Format("%X",EmData.usConstVal);
    QString m_ConstVal = ui->lineEdit_ConstSetting->text();
    if(m_ConstVal.isEmpty())
    {
        return msgBoxFunction(msgBox,"数据常值输入为空，请重新输入!", FALSE);
    }
    //字长
    int m_WordLen = ui->lineEdit_WordLen->text().toInt();
    if(m_WordLen < WORDLEN_MIN || m_WordLen > WORDLEN_MAX)
    {
        return msgBoxFunction(msgBox,"字长输入值超出范围[4~16]", FALSE);
    }
    //字长与固定数的关系
    int m_DataType = ui->comboBox_DataSourceType->currentIndex();
    bool ok = false;
    if(m_DataType == 0)  //固定数
    {
        int num = qPow(2, m_WordLen) - 1;
        QString numStr = QString::number(num, 16);
        if (num < m_ConstVal.toInt(&ok, 16))
        {
            strTmp = QString("固定数输入超出输入范围[%1～0x%2]，请重新输入！")
                    .arg(0).arg(numStr);
            return msgBoxFunction(msgBox,strTmp, FALSE);
        }
    }

    //选择解扰
    int m_bRandom = ui->comboBox_RandEn->currentIndex();
    if (m_bRandom)
    {
        int m_RandLen = ui->comboBox_RandPolyLen->currentIndex();
        //解扰多项式长度
        if ((m_RandLen + 1) < 1 || (m_RandLen + 1) > 4)
        {
            return msgBoxFunction(msgBox,"加扰多项式长度取值范围为：[8～32]位，请重新输入！", FALSE);
        }
        QString m_RandPoly = ui->lineEdit_RandPoly->text();
        //解扰多项式
        if(!checkStringLen(m_RandLen + 1, m_RandPoly))
        {
            return msgBoxFunction(msgBox,"输入的加扰多项式为空或大于设定的多项式长度所能表示的最大值，请重新输入！", FALSE);
        }

        //解扰初相
        QString m_RandPhase = ui->lineEdit_RandPhase->text();
        if(!checkStringLen(m_RandLen + 1, m_RandPhase))
        {
            return msgBoxFunction(msgBox,"输入的加扰初相为空或大于设定的多项式长度所能表示的最大值，请重新输入！", FALSE);
        }
    }
    /*
    int m_CarrierFreq = ui->lineEdit_SubWaveFr->text().toInt();
    if((m_CarrierFreq>CARRIERFREQ_MAX) || (m_CarrierFreq<CARRIERFREQ_MIN))
    {
        return msgBoxFunction(msgBox,"副载波输入范围应为[5~512]kHz.", FALSE);
    }
    */

    //码率
    int nMinCodeRate=BITSRATE_MIN,nMaxCodeRate=BITSRATE_MAX;
    int m_EncodeStyle = ui->comboBox_DecodeType->currentIndex();

    if((m_EncodeStyle==1)||(m_EncodeStyle==3))
    {
        int m_JJCode = ui->comboBox_RecurrentCodeType->currentIndex();
        if (m_JJCode==0)
        {
            nMaxCodeRate/=2;
        }
        else if (m_JJCode==1)
        {
            nMaxCodeRate=nMaxCodeRate*3/4;
        }
    }
    int m_CodeType = ui->comboBox_CodeType->currentIndex();
    if(m_CodeType>2)
    {
        nMaxCodeRate/=2;
    }
    QString sCodeRate = ui->lineEdit_CodeRate->text();
    int m_CodeRate = sCodeRate.toInt();
    if(sCodeRate.isEmpty()||(m_CodeRate>BITSRATE_MAX) || (m_CodeRate<BITSRATE_MIN))
    {
        return msgBoxFunction(msgBox,QString("码率不能为空且输入范围应为[%1~%2]bps.").arg(BITSRATE_MIN).arg(BITSRATE_MAX), FALSE);
    }
    /*
    if(((UINT)(((m_CarrierFreq+0.0005)*1000)) % ((UINT)m_CodeRate))!=0)
    {
        return msgBoxFunction(msgBox,QString("副载波输入值 %1 kHz不是码率输入值 %2 bps的整数倍.").arg(m_CarrierFreq).arg(m_CodeRate), FALSE);
    }

    if((((((UINT)((m_CarrierFreq+0.0005)*1000)/((UINT)m_CodeRate)))<2) || (((UINT)((m_CarrierFreq+0.0005)*1000)/((UINT)m_CodeRate)))>512) )
    {
        return msgBoxFunction(msgBox,QString("不满足要求: 2<=(副载波输入值 %1  / 码率输入值 %2 )<=512.").arg(m_CarrierFreq).arg(m_CodeRate), FALSE);
    }
*/
    //码组长度
    int m_SynLen = ui->comboBox_FrameSyncGroupLen->currentIndex();
    if((((m_SynLen+2)*8)<PATTERNLEN_MIN) || (((m_SynLen+2)*8)>PATTERNLEN_MAX))
    {
        return msgBoxFunction(msgBox,QString("码组长度输入范围应为[%1~%2]bit.").arg(PATTERNLEN_MIN).arg(PATTERNLEN_MAX), FALSE);
    }

    //码组长度与字长的关系
    if((((m_SynLen+2)*8)%m_WordLen)!=0)
    {
        return msgBoxFunction(msgBox,QString("码组长不是字长的整数倍。"), FALSE);
    }

    QString m_FrmCode = ui->lineEdit_FrmSyncGroup->text();
    if(m_FrmCode.isEmpty())
    {
        return msgBoxFunction(msgBox,QString("同步码组不能为空，请重新输入。"), FALSE);
    }
    UI64   tempSynCode = 0;
    UI64   tempSynCode1 = 0;
    tempSynCode1 = StrToSynCode(m_FrmCode.toStdString().c_str());
    tempSynCode = tempSynCode1<<(64-((m_SynLen+2)*8));
    tempSynCode = tempSynCode>>(64-((m_SynLen+2)*8));
    if(tempSynCode != tempSynCode1)
    {
        return msgBoxFunction(msgBox,QString("输入同步码组的长度大于设置的同步码组长度,请重新输入。"), FALSE);
    }

    //副帧长度
    int m_ByfrmType = ui->comboBox_SubFrmSyncType->currentIndex();
    if(m_ByfrmType!=0)
    {
        int m_ByfrmLen = ui->lineEdit_FDormatLen->text().toInt();
        if((m_ByfrmLen < SUBFRMLEN_MIN) || (m_ByfrmLen > SUBFRMLEN_MAX))
        {
            return msgBoxFunction(msgBox,QString("副帧长度输入范围应为[%1~%2]帧。").arg(SUBFRMLEN_MIN).arg(SUBFRMLEN_MAX), FALSE);
        }
    }

    //有RS下, 有无虚拟填充，计算出的帧长
    //m_Interleav 交错深度
    int m_Interleav = ui->lineEdit_RSDecodeDeep->text().toInt();
    if ((m_Interleav<1)||(m_Interleav > 5))
    {
        return msgBoxFunction(msgBox,QString("交错深度的范围为1~5!"), FALSE);
    }
    //帧长
    int m_FrmLen = ui->lineEdit_FrameLen->text().toInt();
    if ((m_EncodeStyle == 0) ||((m_EncodeStyle == 1)))
    {
        if((m_FrmLen < FRAMELEN_MIN) || (m_FrmLen > FRAMELEN_MAX))
        {
            return msgBoxFunction(msgBox,QString("帧长输入范围应为[%1~%2]字.").arg(FRAMELEN_MIN).arg(FRAMELEN_MAX), FALSE);
        }
    }

    //20150402
    int m_VFill = -1;  //虚拟填充的

    // add 帧长与rs编码相关
    if ((m_EncodeStyle == 2) || (m_EncodeStyle == 3))//有RS编码
    {
        //交错深度
        if((m_Interleav < 1) || (m_Interleav > 5))//20080617 交错深度可配置
        {
            strTmp = ("交错深度输入值超出输入范围[1～5]，请重新输入！");
            return msgBoxFunction(msgBox, strTmp, FALSE);
        }
        int temFrm20101104 = 0;
        temFrm20101104 = 255 * m_Interleav + (m_SynLen + 2);//将bit转换成字节以符合公式
        //////////////////////////
        int FrmLenMin;
        int m_ComRS = ui->comboBox_RSType->currentIndex();
        if(m_ComRS == 0) //20150707 edit 原为1
        {
            FrmLenMin = temFrm20101104 - 222*m_Interleav;
        }
        else // m_ComRS == 1
        {
            FrmLenMin = temFrm20101104 - 238*m_Interleav;
        }
        if((m_FrmLen > temFrm20101104) ||(m_FrmLen < FrmLenMin))
        {
            strTmp = QString("RS译码时帧长超出范围[%1～%2]，且应满足(帧长 - 帧同步码组长度 = 交错深度*N)，请重新输入！")
                    .arg(FrmLenMin).arg(temFrm20101104);;
            return msgBoxFunction(msgBox, strTmp, FALSE);
        }
        else
        {
            if(((temFrm20101104 - m_FrmLen) % m_Interleav) != 0)
            {

                strTmp = QString("帧长，交错深度，RS码类型三者不匹配，请重新输入！(帧长 - 帧同步码组长度 = 交错深度*N)");
                return msgBoxFunction(msgBox, strTmp, FALSE);
            }
            else
            {
                if((temFrm20101104 - m_FrmLen) == 0)
                {
                    m_VFill = 0;
                }
                else
                {
                    m_VFill = 1;
                }
            }
        }
    }


    USHORT nIDPos1,nIDPos2;
    if ((m_EncodeStyle == 0)||(m_EncodeStyle == 1)) //有RS编码时才判断m_EncodeStyle
    {
        if ((m_ByfrmType == 1 )||(m_ByfrmType == 2))
        {
            //子帧同步码组位置固定在帧首
            //ID字位置范围
            nIDPos1 = ((m_SynLen+2)*8) / m_WordLen;
            if(m_FrmLen < nIDPos1)
            {
                return msgBoxFunction(msgBox,"帧长不应该小于码组长！",FALSE);
            }
            nIDPos2 = (m_FrmLen < 1024 ? m_FrmLen : 1024);
        }
    }


    //ID副帧和循环副帧的副帧位置 8-10 ID及同步码组按固定在帧尾处理
    int m_SycCodePosi = ui->comboBox_FrmPlace->currentIndex();
    if(m_ByfrmType==1)
    {
        if ((m_EncodeStyle == 0)||(m_EncodeStyle == 1))
        {
            if((nIDPos1+1) > nIDPos2)
            {
                return msgBoxFunction(msgBox,"帧长太短,同步码组位置输入错误!",FALSE);
            }
            int m_IDPos = ui->lineEdit_IDCountPos->text().toInt();
            if(m_IDPos < 0)
            {
                return msgBoxFunction(msgBox,"ID字位置错误!",FALSE);
            }
            if(m_IDPos<1)
            {
                return msgBoxFunction(msgBox,"ID副帧码组位置不能小于1.",FALSE);
            }
            //此时子帧同步码位置可选 m_CodePosi   2010-09-19
            if (m_SycCodePosi == 0) //子帧同步码组位置帧前
            {
                if (m_IDPos <= ((m_SynLen+2)*8)/m_WordLen)
                {
                    return msgBoxFunction(msgBox,"ID字位置不可占据帧同步码组的位置。请重新输入。",FALSE);
                }
                if (m_IDPos > (m_FrmLen)) //Id字位置大于帧长
                {
                    return msgBoxFunction(msgBox,"ID字位置不可超出子帧长度.",FALSE);
                }
            }
            if (m_SycCodePosi == 1)//子帧同步码组位置帧尾
            {
                if(m_IDPos>(m_FrmLen-(((m_SynLen+2)*8)/m_WordLen))) //ID字位置占据同步码的位置
                {
                    return msgBoxFunction(msgBox,"ID字位置不可超出子帧长度，并且不能占据帧同步码组的位置。请重新输入。",FALSE);
                }
            }
        }
    }
    else if(m_ByfrmType==2)
    {
        if ((m_EncodeStyle == 0)||(m_EncodeStyle == 1))
        {
            if((nIDPos1+1) > nIDPos2)
            {
                return msgBoxFunction(msgBox,"帧长太短,同步码组位置输入错误!",FALSE);
            }
            int m_ByfrmPos = ui->lineEdit_SubFrmSynGroupPos->text().toInt();
            if(m_ByfrmPos<1)
            {
                return msgBoxFunction(msgBox,"循环副帧码组位置不能小于1.", FALSE);
            }
            int m_ByfrmCodeLen = ui->comboBox_SubFrameGroupLen->currentIndex();
            //             帧长 - 同步码组长度 - 副帧码组长度
            int m_temp = m_FrmLen-(((m_SynLen+2)*8)/m_WordLen)-(((m_ByfrmCodeLen + 1)*8)/m_WordLen)+1;
            //此时子帧同步码位置可选 m_CodePosi   2010-09-19
            if (m_SycCodePosi == 0) //子帧同步码组位置帧前
            {
                if (m_ByfrmPos <= ((m_SynLen+2)*8)/m_WordLen)
                {
                    return msgBoxFunction(msgBox,"循环副帧码组位置不可占据帧同步码组的位置。请重新输入。", FALSE);
                }
                if (m_ByfrmPos > (m_FrmLen - (((m_ByfrmCodeLen+1)*8)/m_WordLen) +1)) //
                {
                    return msgBoxFunction(msgBox,"循环副帧码组位置不可超出子帧长度，", FALSE);
                }
            }
            if (m_SycCodePosi == 1)//子帧同步码组位置帧尾
            {
                if (m_ByfrmPos <= 0)
                {
                    return msgBoxFunction(msgBox,"循环副帧码组位置不能小于等于0。请重新输入。", FALSE);
                }
                if(m_ByfrmPos>m_temp)
                {
                    return msgBoxFunction(msgBox,"循环副帧码组位置不可超出子帧长度，并且不能占据帧同步码组的位置。请重新输入。", FALSE);
                }
            }

            if((((m_ByfrmCodeLen+1)*8)<SUBFPATTERNLEN_MIN) || (((m_ByfrmCodeLen+1)*8)>SUBFPATTERNLEN_MAX))
            {
                return msgBoxFunction(msgBox,QString("副帧码组长度输入范围应为[%1~%2]bit.").arg(SUBFPATTERNLEN_MIN).arg(SUBFPATTERNLEN_MAX), FALSE);
            }
            QString m_ByfrmCode = ui->lineEdit_SubFrmGroup->text();
            if(m_ByfrmCode.isEmpty())
            {
                return msgBoxFunction(msgBox,QString("副帧码组为空!"), FALSE);
            }
            UI64   tempSynCode=0;
            UI64   tempSynCode1=0;
            tempSynCode1 = StrToSynCode(m_ByfrmCode.toStdString().c_str());
            tempSynCode =tempSynCode1<<(64-((m_ByfrmCodeLen+1)*8));
            tempSynCode =tempSynCode>>(64-((m_ByfrmCodeLen+1)*8));
            if(tempSynCode != tempSynCode1)
            {
                return msgBoxFunction(msgBox,QString("输入副帧同步码组的长度大于设置的副帧同步码组长度,请重新输入。"), FALSE);
            }
        }
    }
    return TRUE;
}


