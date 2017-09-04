#include "SS2BBE_TMParaSetGUIDlg.h"
#include "ui_SS2BBE_TMParaSetGUIDlg.h"
#include <QCloseEvent>
#include <QMessageBox>
#include <QDateTime>
#include "../../Common/Log.h"


int SS2BBE_TMParaSetGUIDlg::CheckParam()
{
    QMessageBox msgBox;
    QString strTemp;
    ///////////////////////////////////////
    QString m_TMInfoFlagCode;
    bool m_brandom;
    QString m_strRandPoly = ui->lineEdit_RandPoly->text();
    QString m_strRandPhase = ui->lineEdit_RandPhase->text();
    int m_ucRandLen = ui->comboBox_RandPolyLen->currentIndex();

    int m_interDep = ui->lineEdit_RSDecodeDeep->text().toInt();

    int tem_strLeng = m_strRandPoly.length();

    int m_ByfrmType = ui->comboBox_SubFrmSyncType->currentIndex();
    int m_SubFrmRongcuoBits = ui->comboBox_SubFrameSyncFaultTolerant->currentIndex();
    int m_SubFrmCheckErrBits = ui->comboBox_SubFaltToleranceOfFrameSync->currentIndex();
    int m_SubFrmLockBits = ui->comboBox_SubFrameSyncFaultTolerantLockBit->currentIndex();
    int m_SubFrmCheckFrms = ui->comboBox_SubCheckFrameOfFaultFrameSync->currentIndex() + 1;
    int m_SubFrmLockFrms = ui->comboBox_SubFrameSyncFaultLockDetection->currentIndex() + 1;

    QString m_FrmCode = ui->lineEdit_FrmSyncGroup->text();
    int m_CheckFrmCount = ui->comboBox_CheckFrameOfFaultFrameSync->currentIndex() + 1;
    int m_lockFrmCount = ui->comboBox_FrameSyncFaultLockDetection->currentIndex() + 1;
    int m_zhengtongbuRongCuo = ui->comboBox_FrameSyncFaultTolerant->currentIndex();
    int m_zhengtongbuRongcuoJiaohe = ui->comboBox_FaltToleranceOfFrameSync->currentIndex();
    int m_lockRongCuowei = ui->comboBox_FrameSyncFaultTolerantLockBit->currentIndex();

    int m_codeStyle = ui->comboBox_DecodeType->currentIndex();
    int m_codeType = ui->comboBox_CodeType->currentIndex();
    ulong m_CodeRate = ui->lineEdit_CodeRate->text().toLong();
    int m_Viterbi = ui->comboBox_ViterbType->currentIndex();
    ulong ulCodeRateIn = (ULONG)m_CodeRate;
    int nMinCodeRate = 100, nMaxCodeRate = 64000;

    int m_FrmLen = ui->lineEdit_FrameLen->text().toInt();

    //ulong m_CarrierFreq = ui->lineEdit_SubWaveFr->text().toLong();

    int m_SynLen = ui->comboBox_FrameSyncGroupLen->currentIndex();
    int m_WordLen = ui->lineEdit_WordLen->text().toInt();

    int m_RS = ui->comboBox_RSType->currentIndex();

    UI64   tempSynCode=0;
    UI64   tempSynCode1=0;

    int m_ByfrmLen = ui->lineEdit_SubFrameLen->text().toInt();

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

    //选择解扰
    m_brandom = ui->comboBox_RandEn->currentText() == "人工解扰" ? 1 : 0;
    if (m_brandom)
    {
        //解扰多项式
        if (m_strRandPoly.isEmpty())
        {
            strTemp = ("解扰多项式不可为空，请重新输入！");
            goto flag;
        }
        if ((m_ucRandLen + 1)* 2 < tem_strLeng)//(m_ulRandPoly > ulMaxRandPoly)
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
        if ((m_ucRandLen + 1)*2 < tem_strLeng)//(m_ulRandPoly > ulMaxRandPoly)
        {
            strTemp = ("输入的解扰初相大于设定的多项式长度所能表示的最大值，请重新输入！");
            goto flag;
        }
    }

    if(ui->lineEdit_RSDecodeDeep->text().isEmpty())
    {
        strTemp = ("交错深度不能为空!");
        goto flag;
    }
    if ((m_interDep < 1)||(m_interDep > 5))
    {
        strTemp = ("交错深度的范围为1~5!");
        goto flag;
    }

    if ((m_ByfrmType == 2) || (m_ByfrmType == 3))
    {
        if((m_SubFrmRongcuoBits < 0)||(m_SubFrmRongcuoBits > 7))
        {

            strTemp = ("副帧同步容错搜索容错位数范围为0~7");
            goto flag;
        }
        if((m_SubFrmCheckErrBits < 0)||(m_SubFrmCheckErrBits > 7))
        {

            strTemp = ("副帧同步容错校核容错位数范围为0~7");
            goto flag;
        }
        if((m_SubFrmLockBits < 0)||(m_SubFrmLockBits > 7))
        {

            strTemp = ("副帧同步容错锁定容错位数范围为0~7");
            goto flag;
        }
    }
    if(m_ByfrmType!=0)
    {

        if((m_SubFrmCheckFrms<1)||(m_SubFrmCheckFrms > 8))
        {

            strTemp = ("副帧校核帧数范围为1~8!");
            goto flag;
        }
        if((m_SubFrmLockFrms < 1)||(m_SubFrmLockFrms > 8))
        {

            strTemp = ("副帧锁定帧数范围为1~8!");
            goto flag;
        }
    }

    if(m_FrmCode == "")
    {
        strTemp = ("子帧码组不可为空!");
        goto flag;
    }
    if((m_CheckFrmCount<1)||(m_CheckFrmCount > 8))
    {
        strTemp = ("帧同步容错校核检测帧数范围为1~8!");
        goto flag;
    }
    if((m_lockFrmCount < 1)||(m_lockFrmCount > 8))
    {
        strTemp = ("帧同步容错锁定检测帧数范围为1~8!");
        goto flag;
    }
    if((m_zhengtongbuRongCuo < 0)||(m_zhengtongbuRongCuo > 7))
    {
        strTemp = ("帧同步容错搜索容错位数范围为0~7");
        goto flag;
    }
    if((m_zhengtongbuRongcuoJiaohe < 0)||(m_zhengtongbuRongcuoJiaohe > 7))
    {
        strTemp = ("帧同步容错校核容错位数范围为0~7");
        goto flag;
    }
    if((m_lockRongCuowei < 0)||(m_lockRongCuowei > 7))
    {
        strTemp = ("帧同步容错锁定容错位数范围为0~7");
        goto flag;
    }
    //

    //    if((m_CarrierFreq > 512000) || (m_CarrierFreq < 5000))
    //    {

    //        strTemp = ("副载波的取值范围应该为：[5000~512000]Hz");
    //        goto flag;
    //    }


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
    if(ui->lineEdit_CodeRate->text().isEmpty())
    {
        strTemp = QString("码率不能为空。");
        goto flag;
    }
    if((ulCodeRateIn > 64000) || (ulCodeRateIn < 100))
    {
        strTemp = QString("码率取值范围应该为：[%1~%2]bps").arg(100).arg(64000);
        goto flag;
    }

    //    if(m_CarrierFreq % ulCodeRateIn != 0)
    //    {
    //        strTemp = QString("副载波输入值%1Hz不是码率输入值%2bps的整数倍.").arg(m_CarrierFreq).arg(ulCodeRateIn);
    //        goto flag;
    //    }

    //    if( (m_CarrierFreq / ulCodeRateIn < 2) || m_CarrierFreq / ulCodeRateIn > 512)
    //    {
    //        strTemp = QString("不满足条件: 2 <= (副载波输入值 / 码率输入值) <= 512.");
    //        goto flag;
    //    }

    //帧长
    if((m_codeStyle == 0)||(m_codeStyle == 1))
    {
        if(ui->lineEdit_FrameLen->text().isEmpty())
        {
            strTemp = QString("帧长不能为空。");
            goto flag;
        }
        if((m_FrmLen < 4) || (m_FrmLen > 2048))
        {
            strTemp = QString("帧长取值范围应该为：[%1~%2]字。").arg(4).arg(2048);
            goto flag;
        }
    }
    //字长
    if(ui->lineEdit_WordLen->text().isEmpty())
    {
        strTemp = ("字长不能为空。");
        goto flag;
    }
    if((m_WordLen < 4) || (m_WordLen > 16))
    {
        strTemp = ("字长为范围时4～16.");
        goto flag;
    }
    else if ((m_SynLen + 2) * 8 % m_WordLen != 0)
    {
        strTemp = "帧同步码组长是字长的整数倍！";
        goto flag;
    }
    tempSynCode1 = StrToSynCode(m_FrmCode.toStdString().c_str());
    tempSynCode =tempSynCode1<<(64 - ((m_SynLen + 2)*8));
    tempSynCode =tempSynCode>>(64 - ((m_SynLen + 2)*8));
    if(tempSynCode != tempSynCode1)
    {

        strTemp = ("输入同步码组的长度大于设置的同步码组长度,请重新输入。");
        goto flag;
    }
    // add 帧长与rs编码相关
    if ((m_codeStyle == 2) || (m_codeStyle == 3))//有RS编码
    {
        //交错深度
        if((m_Interleav < 1) || (m_Interleav > 5))//20080617 交错深度可配置
        {
            strTemp = ("交错深度输入值超出输入范围[1～5]，请重新输入！");
            goto flag;
        }
        temFrm20101104 = 255 * m_Interleav + (m_SynLen + 2);//将bit转换成字节以符合公式
        //////////////////////////
        if(m_RS == 0) //20150707 edit 原为1
        {
            FrmLenMin = temFrm20101104 - 222*m_Interleav;
        }
        else // m_RS == 1
        {
            FrmLenMin = temFrm20101104 - 238*m_Interleav;
        }
        if((m_FrmLen>temFrm20101104) ||(m_FrmLen<FrmLenMin))
        {

            strTemp = QString("RS译码时帧长超出范围[%1～%2]，且应满足(帧长 - 帧同步码组长度 = 交错深度*N)，请重新输入！")
                    .arg(FrmLenMin).arg(temFrm20101104);
            goto flag;
        }
        else
        {
            if(((temFrm20101104 - m_FrmLen) % m_Interleav) != 0)
            {

                strTemp = QString("帧长，交错深度，RS码类型三者不匹配，请重新输入！(帧长 - 帧同步码组长度 = 交错深度*N)");
                goto flag;
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
    if ((m_codeStyle == 0)||(m_codeStyle == 1))
    {
        if ((2 == m_ByfrmType) || (1 == m_ByfrmType))
        {
            //子帧同步码组位置固定在帧首
            //ID字位置范围
            nIDPos1 = ((m_SynLen + 2)*8)/ m_WordLen;
            if(m_FrmLen < nIDPos1)
            {
                strTemp = ("帧长不能小于子帧码组的长度!");
                goto flag;

            }
            nIDPos2 = (m_FrmLen < 1024 ? m_FrmLen : 1024);
        }
    }

    //副帧长度
    if ((m_codeStyle == 0)||(m_codeStyle == 1)){
        if(m_ByfrmType != 0){
            if(ui->lineEdit_SubFrameLen->text().isEmpty())
            {
                strTemp = ("格式(副帧)长度不能为空!");
                goto flag;
            }
            if((m_ByfrmLen < 4) || (m_ByfrmLen > 256))
            {
                strTemp = QString("副帧长取值范围应该为：[%1~%2]帧。").arg(4).arg(256);
                goto flag;
            }
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
            if(ui->lineEdit_idCountPos->text().isEmpty())
            {
                strTemp = ("ID字位置不能为空!");
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
            if(ui->lineEdit_SubFrmSynGroupPos->text().isEmpty())
            {
                strTemp = ("循环副帧码组位置不能为空.");
                goto flag;
            }
            if(m_ByfrmPos < 1)
            {
                strTemp = ("循环副帧码组位置不可小于1.");
                goto flag;
            }

            m_temp = m_FrmLen-(((m_SynLen + 2)*8)/m_WordLen)-(((m_ByfrmCodeLen + 1)*8)/m_WordLen) + 1;
            //此时子帧同步码位置可选 m_CodePosi   2010-09-19
            if (m_CodePosi == 0) //子帧同步码组位置帧前
            {
                if (m_ByfrmPos <= ((m_SynLen + 2)*8)/m_WordLen)
                {
                    strTemp = ("循环副帧码组位置不可占据帧同步码组的位置。请重新输入。");
                    goto flag;
                }
                if (m_ByfrmPos > (m_FrmLen - (((m_ByfrmCodeLen + 1)*8)/m_WordLen) +1)) //
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

            if(m_ByfrmCode.isEmpty())
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
    }
    return 1;
flag:
    msgBox.setWindowTitle(MODULENAME);
    msgBox.setText(strTemp);
    msgBox.exec();
    return -1;

}
