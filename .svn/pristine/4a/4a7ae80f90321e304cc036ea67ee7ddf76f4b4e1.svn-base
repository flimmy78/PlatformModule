#include "USBBBE_ZKsetGUIDlg.h"
#include "ui_USBBBE_ZKsetGUIDlg.h"
#include <QCloseEvent>
#include <QMessageBox>
#include <QDateTime>
#include <QtCore>
#include "../../Common/Log.h"


int USBBBE_ZKsetGUIDlg::msgBoxFunction(QMessageBox &msgBox, QString msg, BOOL ret)
{
    msgBox.setText(msg);
    msgBox.exec();
    return ret;
}
int USBBBE_ZKsetGUIDlg::CheckParam()
{
    QMessageBox msgBox;
    msgBox.setWindowTitle(MODULENAME);
    QString str;
    if(ui->lineEdit_BeforeFillLen->text().isEmpty())
    {
        return msgBoxFunction(msgBox,"前补序列长度输入为空，请重新输入!", FALSE);
    }
    if(ui->lineEdit_LateFillLen->text().isEmpty())
    {
        return msgBoxFunction(msgBox,"后补序列长度输入为空，请重新输入!", FALSE);
    }
    if(ui->lineEdit_RandPolyLen->text().isEmpty())
    {
        return msgBoxFunction(msgBox,"加扰多项式长度输入为空，请重新输入!", FALSE);
    }
    int m_ExeSampleLen = ui->lineEdit_ExeCodeLen->text().toInt();
	if (m_ExeSampleLen < 1 || m_ExeSampleLen > 8)
	{
        str = "执行编码长度范围为[1～8]字节，请重新输入！"; //  ???
        return msgBoxFunction(msgBox,str,FALSE);
	}
    int m_FSampleLen = ui->lineEdit_BeforeFillLen->text().toInt();
    if (m_FSampleLen < 0 || m_FSampleLen > 8192)
    {
        str = "前补序列长度范围为[0～8192]字节，请重新输入！";
        return msgBoxFunction(msgBox,str,FALSE);
    }
    int m_PSampleLen = ui->lineEdit_LateFillLen->text().toInt();
    if (m_PSampleLen < 0 || m_PSampleLen > 8192)
    {
        str = "后补序列长度范围为[0～8192]字节,请重新输入！";
        return msgBoxFunction(msgBox,str,FALSE);
	}
    int m_nInternal = ui->lineEdit_BufferGap->text().toInt();
    if (m_nInternal < 100 || m_nInternal > 10000) // old = [0～10000]ms   ???
    {
        str = "指令保护间隔范围为[100～10000]ms，请重新输入！";
        return msgBoxFunction(msgBox,str,FALSE);
    }

    int m_nCodeRate = ui->lineEdit_CodeRate->text().toInt();
    if (m_nCodeRate < 100 || m_nCodeRate > 8000)
    {
        str = "码率输入范围为[100～8000]bps,请重新输入！";
        return msgBoxFunction(msgBox,str,FALSE);
    }
	
    // hfn 20101112
    int m_nSubCar1F = ui->lineEdit_SubWaveFre->text().toDouble();
    if (m_nSubCar1F < 2 || m_nSubCar1F > 100)
    {
        str = "副载波1频（f1）的输入范围为[2～100]kHz,请重新输入！"; //  数据1频
        return msgBoxFunction(msgBox,str,FALSE);
    }

    //调制频率 	// hfn 20101112
    int m_ucModuType = ui->comboBox_MudulationSys->currentIndex();
    DOUBLE m_nSubCar0F = ui->lineEdit_ModulationFre->text().toDouble();
    if (m_ucModuType ==1) //PCM/FSK
    {
        if (m_nSubCar0F < 2 || m_nSubCar0F > 100 )
        {
            str = "数据0频的输入范围为[2～100]kHz,请重新输入！";
            return msgBoxFunction(msgBox,str,FALSE);
        }
    }
    else
    {
        if (m_nSubCar0F < 2 || m_nSubCar0F > 100)
        {
            str = "调制频率的输入范围为[2～100]kHz,请重新输入！";
            return msgBoxFunction(msgBox,str,FALSE);
        }
    }

    if (m_ucModuType == 0)//hfn 20100815  // hfn 20101112 m_nSubCar0F
    {
        UINT tempnSubCar0F = qRound(m_nSubCar0F * 1000);
        if(tempnSubCar0F % m_nCodeRate != 0 ||
                tempnSubCar0F / m_nCodeRate <  4 ||	tempnSubCar0F / m_nCodeRate > 1024 )
        {
            str = "调制频率必须能被码率整除,且调制频率除以码率后的值在4与1024之间,请重新输入！";
            return msgBoxFunction(msgBox,str,FALSE);
        }
    }
    int m_nSubCarExecF = ui->lineEdit_SubWaveExeFre->text().toDouble();
    if (m_nSubCarExecF < 2 || m_nSubCarExecF > 100 )
    {
        str = "执行频率的输入范围为[2～100]kHz,请重新输入！";
        return msgBoxFunction(msgBox,str,FALSE);
    }

    int m_nSubCarFillF = ui->lineEdit_SubWaveFillFre->text().toDouble();
    if (m_nSubCarFillF < 2 || m_nSubCarFillF > 100 )
    {
        str = "填充频率的输入范围为[2～100]kHz,请重新输入！";
        return msgBoxFunction(msgBox,str,FALSE);
    }
    QString m_szPSample = ui->lineEdit_LateFillCode->text();
    if (m_szPSample.isEmpty())
    {
        str = "后补序列不能为空,请重新输入！";
        return msgBoxFunction(msgBox,str,FALSE);
    }

    QString m_szFSample = ui->lineEdit_BeforeFillCode->text();
    if (m_szFSample.isEmpty())
    {
        str = "前补序列不能为空,请重新输入！";
        return msgBoxFunction(msgBox,str,FALSE);
    }

    QString m_szExeSample = ui->lineEdit_ExeCode->text();
    if (m_szExeSample.isEmpty())
    {
        str = "执行序列不能为空,请重新输入！";
        return msgBoxFunction(msgBox, str, FALSE);
    }

    QString m_szSpareSample = ui->lineEdit_FreeCode->text();
    if (m_szSpareSample.isEmpty())
    {
        str = "空闲序列不能为空,请重新输入！";
        return msgBoxFunction(msgBox, str , FALSE);
    }
    //yan add

    if (m_ucModuType ==0) //PCMPSK
    {
        if (m_szFSample.size() > (m_FSampleLen*2))
        {
            if (m_FSampleLen)
            {
                str = "前补序列输入长度不能大于设置长度,请重新输入！";
                return msgBoxFunction(msgBox, str, FALSE);
            }
        }
        if (m_szPSample.size() > (m_PSampleLen * 2))
		{
            if (m_PSampleLen)
            {
                str = "后补序列输入长度不能大于设置长度,请重新输入！";
                return msgBoxFunction(msgBox, str, FALSE);
            }
		}
        if (m_szExeSample.size()>(m_ExeSampleLen*2))
        {
            if (m_ExeSampleLen)
            {
                str = "执行序列输入长度不能大于设置长度,请重新输入！";
                return msgBoxFunction(msgBox, str, FALSE);
            }
        }
	}

    int m_ucRandom = ui->comboBox_RandCtrl->currentIndex();
	if (m_ucRandom ==1) 
	{
        QString m_szRandPoly = ui->lineEdit_RandPoly->text();
        if (m_szRandPoly.isEmpty())
		{
				str = "加扰多项式输入不可为空,请重新输入!";
            return msgBoxFunction(msgBox, str, FALSE);
		}
        QString m_szRandPhase = ui->lineEdit_RandPhase->text();
        if (m_szRandPhase == "")
        {
            str = "加扰初相输入不可为空,请重新输入!";
            return msgBoxFunction(msgBox, str, FALSE);
        }
        int m_RandLen = ui->lineEdit_RandPolyLen->text().toInt();
        if (m_szRandPoly.size() > (m_RandLen * 2))
		{
			if (m_RandLen)
			{	
                str = "加扰多项式输入长度不能大于设置长度,请重新输入！";
                return msgBoxFunction(msgBox, str, FALSE);
			}
		}
        if (m_szRandPhase.size()>(m_RandLen * 2))
        {
            if (m_RandLen)
            {
                str = "加扰初相输入长度不能大于设置长度,请重新输入！";
                return msgBoxFunction(msgBox,str, FALSE);
            }
        }
	}

    int m_ucCodingCtl = ui->comboBox_BCHEncode->currentIndex();
	if (m_ucCodingCtl ==1) 
	{
        int m_ucAddStartSeq  = ui->comboBox_BeginCtrl->currentIndex();
        if(m_ucAddStartSeq ==1)
        {
            QString m_szStartSeq = ui->lineEdit_BeginPattern->text();
            if (m_szStartSeq == "")
            {
                str = "起始序列图样输入不可为空,请重新输入!";
                return msgBoxFunction(msgBox, str, FALSE);
            }
        }
        int m_ucAddEndSeq    = ui->comboBox_LateCtrl->currentIndex();
        if(m_ucAddEndSeq==1)
        {
            QString m_EndSeq = ui->lineEdit_LatePattern->text();
            if (m_EndSeq == "")
            {
                str = "结尾序列图样输入不可为空,请重新输入!";
                return msgBoxFunction(msgBox, str, FALSE);
            }
        }
	}
	//end
	return 1;
}
