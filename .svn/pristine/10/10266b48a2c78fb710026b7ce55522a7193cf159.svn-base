#include "SSBBE_TMParaSetGUIDlg.h"
#include "ui_SSBBE_TMParaSetGUIDlg.h"
#include <QCloseEvent>
#include <QMessageBox>
#include <QDateTime>
#include "../../Common/Log.h"


int SSBBE_TMParaSetGUIDlg::CheckParam()
{
    QMessageBox msgBox;
    QString strTemp;
    msgBox.setWindowTitle(MODULENAME);
    ///////////////////////////////////////
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
    int nMinCodeRate = 32, nMaxCodeRate = 64000;
    int nMaxFrmLen = 2048, nMinFrmLen = 4;
    int m_FrmLen = ui->lineEdit_FrameLen->text().toInt();
    int m_SynLen = ui->comboBox_FrameSyncGroupLen->currentIndex();
    int m_WordLen = ui->lineEdit_WordLen->text().toInt();
    int m_RS;

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

    if(ui->lineEdit_FrameDataFlag->text().isEmpty())
    {
        return msgBoxFunction(msgBox,"数据帧标志码输入为空，请重新输入!", FALSE);
    }
    if(ui->lineEdit_RandPoly->text().isEmpty())
    {
        return msgBoxFunction(msgBox,"解扰多项式输入为空，请重新输入!", FALSE);
    }
    if(ui->lineEdit_RandPhase->text().isEmpty())
    {
        return msgBoxFunction(msgBox,"解扰初项输入为空，请重新输入!", FALSE);
    }
    int tmp = ui->lineEdit_SatelliteLockWave->text().toInt();
    if(ui->lineEdit_SatelliteLockWave->text().isEmpty() || tmp < 1  || tmp > 2048)
    {
        return msgBoxFunction(msgBox,"卫星载波锁定波道输入范围1～2048，请重新输入!", FALSE);
    }
    if(ui->lineEdit_SatelliteLock->text().isEmpty())
    {
        return msgBoxFunction(msgBox,"卫星载波锁定位输入为空，请重新输入!", FALSE);
    }
    if (ui->lineEdit_SatelliteLock->text().toInt() < 0 || ui->lineEdit_SatelliteLock->text().toInt() > m_WordLen)
    {
        strTemp = QString("卫星载波锁定位范围为%1～%2").arg(0).arg(m_WordLen);
        return msgBoxFunction(msgBox, strTemp, FALSE);
    }
    tmp = ui->lineEdit_SatelliteLongCodeWave->text().toInt();
    if(ui->lineEdit_SatelliteLongCodeWave->text().isEmpty() || tmp < 1  || tmp > 2048)
    {
        return msgBoxFunction(msgBox,"卫星长码锁定波道输入范围1～2048，请重新输入!", FALSE);
    }
    if(ui->lineEdit_SatelliteLongCode->text().isEmpty())
    {
        return msgBoxFunction(msgBox,"卫星长码锁定位输入为空，请重新输入!", FALSE);
    }
    if (ui->lineEdit_SatelliteLongCode->text().toInt() < 0 || ui->lineEdit_SatelliteLongCode->text().toInt() > m_WordLen)
    {
        strTemp = QString("卫星长码锁定位范围为%1～%2").arg(0).arg(m_WordLen);
        return msgBoxFunction(msgBox, strTemp, FALSE);
    }
    tmp = ui->lineEdit_SatelliteShortLockWave->text().toInt();
    if(ui->lineEdit_SatelliteShortLockWave->text().isEmpty() || tmp < 1  || tmp > 2048)
    {
        return msgBoxFunction(msgBox,"卫星短码锁定波道输入范围1～2048，请重新输入!", FALSE);
    }
    if(ui->lineEdit_SatelliteShortLock->text().isEmpty())
    {
        return msgBoxFunction(msgBox,"卫星短码锁定位输入为空，请重新输入!", FALSE);
    }
    if (ui->lineEdit_SatelliteShortLock->text().toInt() < 0 || ui->lineEdit_SatelliteShortLock->text().toInt() > m_WordLen)
    {
        strTemp = QString("卫星短码锁定位范围为%1～%2").arg(0).arg(m_WordLen);
        return msgBoxFunction(msgBox, strTemp, FALSE);
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
        if (((m_ucRandLen + 1)*4) < (tem_strLeng * 2))//(m_ulRandPoly > ulMaxRandPoly)
        {
            strTemp = ("输入的解扰初相大于设定的多项式长度所能表示的最大值，请重新输入！");
            goto flag;
        }
    }

    if ((m_ByfrmType == 2) || (m_ByfrmType == 3))//副帧同步方式为循环副帧或反码副帧
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
    if(m_ByfrmType != 0)//有副帧时
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

    if(m_FrmCode.isEmpty())
    {
        strTemp = ("子帧码组不可为空!");
        goto flag;
    }
    if((m_CheckFrmCount < 1)||(m_CheckFrmCount > 8))
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

    if((ulCodeRateIn > nMaxCodeRate) || (ulCodeRateIn < nMinCodeRate))
    {
        strTemp = QString("码率取值范围应该为：[%1~%2]bps").arg(nMinCodeRate).arg(nMaxCodeRate);
        goto flag;
    }

    //帧长
    if((m_codeStyle == 0)||(m_codeStyle == 1))
    {
        if((m_FrmLen < nMinFrmLen) || (m_FrmLen > nMaxFrmLen))
        {
            strTemp = QString("帧长取值范围应该为：[%1~%2]字。").arg(nMinFrmLen).arg(nMaxFrmLen);
            goto flag;
        }
    }

    tempSynCode1 = StrToSynCode(m_FrmCode.toStdString().c_str());
    tempSynCode = tempSynCode1<<(64 - ((m_SynLen + 2) * 8));
    tempSynCode = tempSynCode>>(64 - ((m_SynLen + 2) * 8));
    if(tempSynCode != tempSynCode1)
    {

        strTemp = ("输入同步码组的长度大于设置的同步码组长度,请重新输入。");
        goto flag;
    }
    //字长
    if((m_WordLen < 4) || (m_WordLen > 16))
    {
        strTemp = ("字长范围是4～16！");
        goto flag;
    }
    else if ((m_SynLen + 2)*8 % m_WordLen != 0)
    {
        strTemp = "帧码组长不等于字长的整数倍，请重新输入！";
        goto flag;
    }

    //副帧长度

    if(m_ByfrmType != 0)
    {
        if((m_ByfrmLen < 4) || (m_ByfrmLen > 256))
        {

            strTemp = QString("副帧长取值范围应该为：[%1～%2]帧。").arg(4).arg(256);
            goto flag;
        }
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
        m_RS = ui->comboBox_RSType->currentIndex();
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

    /*if ((2 == m_codeStyle) || (3 == m_codeStyle))//有RS编码 (1 == m_RS) || (2 == m_RS)
    {
        if ((m_interDep < 1)||(m_interDep > 5))//验证交织深度
        {
            strTemp = ("交错深度的范围为1~5!");
            goto flag;
        }
        temFrm20101104 = 255 * m_Interleav + (m_SynLen + 2);
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
            if(VF_Len >= 255*m_Interleav)  //交错深度太大
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
    }*/
    if ((m_codeStyle == 0)||(m_codeStyle == 1))
    {
        if ((2 == m_ByfrmType) || (1 == m_ByfrmType))//副帧方式为ID或循环副帧
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
    if(m_ByfrmType == 1)
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
            if(m_IDPos < 1)
            {
                strTemp = ("ID字位置不能小于1.");
                goto flag;
            }
            //此时子帧同步码位置可选 m_CodePosi

            if (m_CodePosi == 0) //子帧同步码组位置帧首
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

            m_temp = m_FrmLen - (((m_SynLen + 2) * 8)/m_WordLen) - (((m_ByfrmCodeLen + 1) * 8)/m_WordLen) + 1;
            //此时子帧同步码位置可选
            if (m_CodePosi == 0) //子帧同步码组位置帧前
            {
                if (m_ByfrmPos <= ((m_SynLen + 2)*8)/m_WordLen)
                {
                    strTemp = ("循环副帧码组位置不可占据帧同步码组的位置。请重新输入。");
                    goto flag;
                }
                if (m_ByfrmPos > (m_FrmLen - (((m_ByfrmCodeLen + 1)*8)/m_WordLen) + 1)) //
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
