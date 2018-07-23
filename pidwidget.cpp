#include "pidwidget.h"
#include "ui_pidwidget.h"
#include <windows.h>
#include "conio.h"



/*//////////////////////////////
 *函数名：CheckError
 *功能：出错提示函数，如果输入的sRtn为非零，则说明该功能未实现
       跳出一个对话框，提示出错信息。str为提示信息。
//////////////////////////////*/
void PIDWidget::CheckError(short sRtn,QString str)
{
    if (sRtn)
    {
        QString str1;
        QMessageBox::warning(this,"Error information",str+str1.setNum(sRtn));
    }
    else
    {
        return;
    }
}



/*/////////////////////////////////////
 * 函数名：configure
 * 功能：配置轴为报警输出无效，正负限位无效,更新状态
 *      使能，位置清零，规划位置清零,设置点位模式
 ////////////////////////////////////*/
void PIDWidget::configure(int axis,QString str)
{
    short sRtn;

    //sRtn = GT_Reset();
    //CheckError(sRtn,str+"GT_Reset has a problem: ");

    //sRtn = GT_StepDir(axis);
    //CheckError(sRtn,str+"GT_StepDir has a problem: ");

    sRtn=GT_AlarmOff(axis);
    CheckError(sRtn,str+"GT_AlarmOff has a problem: ");

    sRtn=GT_LmtsOff(axis,-1);
    CheckError(sRtn,str+"GT_LmtsOff has a problem: ");

    sRtn=GT_ClrSts(1,8);
    CheckError(sRtn,str+"GT_ClrSts has a problem: ");

    sRtn=GT_AxisOn(axis);
    CheckError(sRtn,str+"GT_AxisOn has a problem: ");

    //sRtn=GT_ZeroPos(axis);
    //CheckError(sRtn,str+"GT_ZeroPos has a problem: ");

    sRtn=GT_SetPrfPos(axis,0);
    CheckError(sRtn,str+"GT_SetPrfPos has a problem: ");

    sRtn=GT_PrfTrap(axis);
    CheckError(sRtn,str+"GT_PrfTrap has a problem: ");



}


//******************************构造函数
PIDWidget::PIDWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PIDWidget)
{
    ui->setupUi(this);
    threadone=new axisoneThread(this);
    threadtwo=new axistwoThread(this);
    threadthree=new axisthreeThread(this);
    threadfour=new axisfourThread(this);
    threadfive=new axisfiveThread(this);
    threadsix=new axissixThread(this);
    connect(threadone,SIGNAL(threadonesignal(double)),this,SLOT(threadoneslot(double)));
    connect(threadtwo,SIGNAL(threadtwosignal(double)),this,SLOT(threadtwoslot(double)));
    connect(threadthree,SIGNAL(threadthreesignal(double)),this,SLOT(threadthreeslot(double)));
    connect(threadfour,SIGNAL(threadfoursignal(double)),this,SLOT(threadfourslot(double)));
    connect(threadfive,SIGNAL(threadfivesignal(double)),this,SLOT(threadfiveslot(double)));
    connect(threadsix,SIGNAL(threadsixsignal(double)),this,SLOT(threadsixslot(double)));
}


//*****************************析构函数
PIDWidget::~PIDWidget()
{
    delete ui;
    delete threadone;
    delete threadtwo;
    delete threadthree;
    delete threadfour;
    delete threadfive;
    delete threadsix;

}


/*////////////////////////////////////
 *函数名：on_OpenControlButton_clicked
 *功能:打开运动控制器并初始化环境,并进行初始赋值
 *
///////////////////////////////////*/
void PIDWidget::on_OpenControlButton_clicked()
{
    short sRtn;
    QString str="GT_Open have a problem: ";
    sRtn = GT_Open();
    CheckError(sRtn,str);

    datas.AxisOneAcc=0;
    datas.AxisTwoAcc=0;
    datas.AxisThreeAcc=0;
    datas.AxisFourAcc=0;
    datas.AxisFiveAcc=0;
    datas.AxisSixAcc=0;

    datas.AxisOneDec=0;
    datas.AxisTwoDec=0;
    datas.AxisThreeDec=0;
    datas.AxisFourDec=0;
    datas.AxisFiveDec=0;
    datas.AxisSixDec=0;

    datas.AxisOneSmt=0;
    datas.AxisTwoSmt=0;
    datas.AxisThreeSmt=0;
    datas.AxisFourSmt=0;
    datas.AxisFiveSmt=0;
    datas.AxisSixSmt=0;

    datas.AixsOneTargetLocation=0;
    datas.AixsTwoTargetLocation=0;
    datas.AixsThreeTargetLocation=0;
    datas.AixsFourTargetLocation=0;
    datas.AixsFiveTargetLocation=0;
    datas.AixsSixTargetLocation=0;

    datas.AxisOneTargetVelocity=0;
    datas.AxisTwoTargetVelocity=0;
    datas.AxisThreeTargetVelocity=0;
    datas.AxisFourTargetVelocity=0;
    datas.AxisFiveTargetVelocity=0;
    datas.AxisSixTargetVelocity=0;

    timer = new QTimer(this);



    prfPosone=0;
    prfPostwo=0;
    prfPosthree=0;
    prfPosfour=0;
    prfPosfive=0;
    prfPossix=0;

    encPosone=0;
    encPostwo=0;
    encPosthree=0;
    encPosfour=0;
    encPosfive=0;
    encPossix=0;

    LastPosone=0;
    LastPostwo=0;
    LastPosthree=0;
    LastPosfour=0;
    LastPosfive=0;
    LastPossix=0;

    //清空缓冲区
    encvecone.clear();
    encvectwo.clear();
    encvecthree.clear();
    encvecfour.clear();
    encvecfive.clear();
    encvecsix.clear();
    timecount.clear();
    timeco=0;

   //清空文件
    QFile fileone("Encoder_data_one.txt");

    if (!fileone.open(QIODevice::WriteOnly|QIODevice::Text))
        QMessageBox::information(this,"Information","File don't open!");
    fileone.close();
    /////////////////////
    QFile filetwo("Encoder_data_two.txt");

    if (!filetwo.open(QIODevice::WriteOnly|QIODevice::Text))
        QMessageBox::information(this,"Information","File don't open!");
    filetwo.close();
    ////////////////
    QFile filethree("Encoder_data_three.txt");

    if (!filethree.open(QIODevice::WriteOnly|QIODevice::Text))
        QMessageBox::information(this,"Information","File don't open!");
    filethree.close();
    //////////////////////
    QFile filefour("Encoder_data_four.txt");

    if (!filefour.open(QIODevice::WriteOnly|QIODevice::Text))
        QMessageBox::information(this,"Information","File don't open!");
    filefour.close();
    //////////////
    QFile filefive("Encoder_data_five.txt");

    if (!filefive.open(QIODevice::WriteOnly|QIODevice::Text))
        QMessageBox::information(this,"Information","File don't open!");
    filefive.close();
    /////////////
    QFile filesix("Encoder_data_six.txt");

    if (!filesix.open(QIODevice::WriteOnly|QIODevice::Text))
        QMessageBox::information(this,"Information","File don't open!");
    filesix.close();
    /////////////

}


/*///////////////////////////////////
 * 函数名：on_CloseButton_clicked
 * 功能：关闭使能并进行出错检查,并把编码器的值写入到txt文件中
 //////////////////////////////////*/
void PIDWidget::on_CloseButton_clicked()
{
    short sRtn;
    QString str="GT_AxisOff have a problem: ";
    if (ui->DeltaRobotAcheckBox->isChecked())      //查看有没有选中并联机器人A
    {
        if (ui->AxisOnecheckBox->isChecked())    //查看有没有选中轴1，如果选中则释放轴1的使能并进行检查出错
        {
            sRtn = GT_AxisOff(datas.axisOne);
            CheckError(sRtn,"axisOne_"+str);
        }
        if (ui->AxisTwocheckBox->isChecked())    //查看有没有选中轴2，如果选中则释放轴2的使能并进行检查出错
        {
            sRtn = GT_AxisOff(datas.axisTwo);
            CheckError(sRtn,"axisTwo_"+str);
        }
        if (ui->AxisThreecheckBox->isChecked())   //查看有没有选中轴3，如果选中则释放轴3的使能并进行检查出错
        {
            sRtn = GT_AxisOff(datas.axisThree);
            CheckError(sRtn,"axisThree_"+str);
        }
        if (!ui->AxisOnecheckBox->isChecked()&&!ui->AxisTwocheckBox->isChecked()&&!ui->AxisThreecheckBox->isChecked())//如果都没有选中则跳出消息框
        {
            QMessageBox::information(this,"Information","DeltaRobotA has no axis selected!");
        }
    }
    if (ui->DeltaRobotBcheckBox->isChecked())      //查看有没有选中并联机器人B
    {
        if (ui->AxisFourcheckBox->isChecked())   //查看有没有选中轴4，如果选中则释放轴4的使能并进行检查出错
        {
            sRtn = GT_AxisOff(datas.axisFour);
            CheckError(sRtn,"axisFour_"+str);
        }
        if (ui->AxisFivecheckBox->isChecked())  //查看有没有选中轴5，如果选中则释放轴5的使能并进行检查出错
        {
            sRtn = GT_AxisOff(datas.axisFive);
            CheckError(sRtn,"axisFive_"+str);
        }
        if (ui->AxisSixcheckBox->isChecked())   //查看有没有选中轴6，如果选中则释放轴6使能并进行检查出错
        {
            sRtn = GT_AxisOff(datas.axisSix);
            CheckError(sRtn,"axisSix_"+str);
        }
        if (!ui->AxisFourcheckBox->isChecked()&&!ui->AxisFivecheckBox->isChecked()&&!ui->AxisSixcheckBox->isChecked())//如果都没有选中轴，则跳出消息框
        {
            QMessageBox::information(this,"Information","DeltaRobotB has no axis selected");
        }
    }
    if (!ui->DeltaRobotAcheckBox->isChecked()&&!ui->DeltaRobotBcheckBox->isChecked())  //如果都没有选中则跳出一个信息框
    {
        QMessageBox::information(this,"Information","No robot is selected!");
    }

    //把编码器的值写入txt文件中


    QFile fileA("data.txt");

    if (!fileA.open(QIODevice::WriteOnly|QIODevice::Text))
        QMessageBox::information(this,"Information","File don't open!");

       QTextStream out(&fileA);

      out<<ui->RobotAAxisXlineEdit->text()<<'\n';
      out<<ui->RobotAAxisYlineEdit->text()<<'\n';
      out<<ui->RobotAAxisZlineEdit->text()<<'\n';
      out<<ui->RobotBAxisXlineEdit->text()<<'\n';
      out<<ui->RobotBAxisYlineEdit->text()<<'\n';
      out<<ui->RobotBAxisZlineEdit->text()<<'\n';

       fileA.close();

       //清空缓冲区
       encvecone.clear();
       encvectwo.clear();
       encvecthree.clear();
       encvecfour.clear();
       encvecfive.clear();
       encvecsix.clear();
       timecount.clear();


}


//##################################################################################################
/*///////////////////////////////////////////////
 * 函数名：on_DeltaRobotAcheckBox_released
 * 功能：当并联机器人A复选框的按钮消失时，下面轴的复选框重置为0
 /////////////////////////////////////////////*/
void PIDWidget::on_DeltaRobotAcheckBox_clicked()
{
    if (!ui->DeltaRobotAcheckBox->isChecked())
    {
    ui->AxisOnecheckBox->setCheckState(Qt::Unchecked);
    ui->AxisTwocheckBox->setCheckState(Qt::Unchecked);
    ui->AxisThreecheckBox->setCheckState(Qt::Unchecked);
    }
    else
    {
        return;
    }
}


/*///////////////////////////////////////////////
 * 函数名：on_DeltaRobotBcheckBox_released
 * 功能：当并联机器人B复选框的按钮消失时，下面轴的复选框重置为0
 /////////////////////////////////////////////*/
void PIDWidget::on_DeltaRobotBcheckBox_clicked()
{
    if (!ui->DeltaRobotBcheckBox->isChecked())
    {
    ui->AxisFourcheckBox->setCheckState(Qt::Unchecked);
    ui->AxisFivecheckBox->setCheckState(Qt::Unchecked);
    ui->AxisSixcheckBox->setCheckState(Qt::Unchecked);
    }
    else
    {
        return;
    }

}


/*//////////////////////////////////////////////
 * 函数名：on_WriteInStateButton_clicked
 * 功能：输入框中的值写入到datas这个结构中
 /////////////////////////////////////////////*/
void PIDWidget::on_WriteInStateButton_clicked()
{
    if (ui->DeltaRobotAcheckBox->isChecked())
    {
        if (ui->AxisOnecheckBox->isChecked())
        {
            datas.AxisOneAcc=((ui->AxisOneAcclineEdit->text()).toDouble())/accfactor;
            datas.AxisOneDec=((ui->AxisOneDeclineEdit->text()).toDouble())/decfactor;
            datas.AxisOneSmt=(ui->AxisOneSmtlineEdit->text()).toInt();
            datas.AixsOneTargetLocation=((ui->AxisOneTargetLocationlineEdit->text()).toDouble())/locationfactor;
            datas.AxisOneTargetVelocity=((ui->AxisOneTargetVelocitylineEdit->text()).toDouble())/velocityfactor;

        }
        if (ui->AxisTwocheckBox->isChecked())
        {
            datas.AxisTwoAcc=((ui->AxisTwoAcclineEdit->text()).toDouble())/accfactor;
            datas.AxisTwoDec=((ui->AxisTwoDeclineEdit->text()).toDouble())/decfactor;
            datas.AxisTwoSmt=(ui->AxisTwoSmtlineEdit->text()).toInt();
            datas.AixsTwoTargetLocation=((ui->AxisTwoTargetLocationlineEdit->text()).toDouble())/locationfactor;
            datas.AxisTwoTargetVelocity=((ui->AxisTwoTargetVelocitylineEdit->text()).toDouble())/velocityfactor;

        }
        if (ui->AxisThreecheckBox->isChecked())
        {
            datas.AxisThreeAcc=((ui->AxisThreeAcclineEdit->text()).toDouble())/accfactor;
            datas.AxisThreeDec=((ui->AxisThreeDeclineEdit->text()).toDouble())/decfactor;
            datas.AxisThreeSmt=(ui->AxisThreeSmtlineEdit->text()).toInt();
            datas.AixsThreeTargetLocation=((ui->AxisThreeTargetLocationlineEdit->text()).toDouble())/locationfactor;
            datas.AxisThreeTargetVelocity=((ui->AxisThreeTargetVelocitylineEdit->text()).toDouble())/velocityfactor;

        }
        if (!ui->AxisOnecheckBox->isChecked() && !ui->AxisTwocheckBox->isChecked() && !ui->AxisThreecheckBox->isChecked())
        {
            QMessageBox::information(this,"Information","DeltaRobotA has no axis selected!");
        }
    }
    if (ui->DeltaRobotBcheckBox->isChecked())
    {
        if (ui->AxisFourcheckBox->isChecked())
        {
            datas.AxisFourAcc=((ui->AxisFourAcclineEdit->text()).toDouble())/accfactor;
            datas.AxisFourDec=((ui->AxisFourDeclineEdit->text()).toDouble())/decfactor;
            datas.AxisFourSmt=(ui->AxisFourSmtlineEdit->text()).toInt();
            datas.AixsFourTargetLocation=((ui->AxisFourTargetLocationlineEdit->text()).toDouble())/locationfactor;
            datas.AxisFourTargetVelocity=((ui->AxisFourTargetVelocitylineEdit->text()).toDouble())/velocityfactor;

        }
        if (ui->AxisFivecheckBox->isChecked())
        {
            datas.AxisFiveAcc=((ui->AxisFiveAcclineEdit->text()).toDouble())/accfactor;
            datas.AxisFiveDec=((ui->AxisFiveDeclineEdit->text()).toDouble())/decfactor;
            datas.AxisFiveSmt=(ui->AxisFiveSmtlineEdit->text()).toInt();
            datas.AixsFiveTargetLocation=((ui->AxisFiveTargetLocationlineEdit->text()).toDouble())/locationfactor;
            datas.AxisFiveTargetVelocity=((ui->AxisFiveTargetVelocitylineEdit->text()).toDouble())/velocityfactor;

        }
        if (ui->AxisSixcheckBox->isChecked())
        {
            datas.AxisSixAcc=((ui->AxisSixAcclineEdit->text()).toDouble())/accfactor;
            datas.AxisSixDec=((ui->AxisSixDeclineEdit->text()).toDouble())/decfactor;
            datas.AxisSixSmt=(ui->AxisSixSmtlineEdit->text()).toInt();
            datas.AixsSixTargetLocation=((ui->AxisSixTargetLocationlineEdit->text()).toDouble())/locationfactor;
            datas.AxisSixTargetVelocity=((ui->AxisSixTargetVelocitylineEdit->text()).toDouble())/velocityfactor;

        }
        if (!ui->AxisFourcheckBox->isChecked() && !ui->AxisFivecheckBox->isChecked() && !ui->AxisSixcheckBox->isChecked())
        {
            QMessageBox::information(this,"Information","DeltaRobotB has no axis selected!");
        }
    }
    if (!ui->DeltaRobotAcheckBox->isChecked() && !ui->DeltaRobotBcheckBox->isChecked())
    {
        QMessageBox::information(this,"Information","No robot is selected!");
    }


}


/*///////////////////////////////////////////////
 * 函数名：on_AxisFourcheckBox_clicked
 * 功能：当复选框被选中时，只读状态被取消
 //////////////////////////////////////////////*/
void PIDWidget::on_AxisFourcheckBox_stateChanged(int arg1)
{
    if (ui->AxisFourcheckBox->isChecked())
    {
        ui->AxisFourAcclineEdit->setReadOnly(false);
        ui->AxisFourDeclineEdit->setReadOnly(false);
        ui->AxisFourSmtlineEdit->setReadOnly(false);
        ui->AxisFourTargetLocationlineEdit->setReadOnly(false);
        ui->AxisFourTargetVelocitylineEdit->setReadOnly(false);
    }
    else
    {
        ui->AxisFourAcclineEdit->setReadOnly(true);
        ui->AxisFourDeclineEdit->setReadOnly(true);
        ui->AxisFourSmtlineEdit->setReadOnly(true);
        ui->AxisFourTargetLocationlineEdit->setReadOnly(true);
        ui->AxisFourTargetVelocitylineEdit->setReadOnly(true);
    }
}



/*///////////////////////////////////////////////
 * 函数名：on_AxisFivecheckBox_clicked
 * 功能：当复选框被选中时，只读状态被取消
 //////////////////////////////////////////////*/
void PIDWidget::on_AxisFivecheckBox_stateChanged(int arg1)
{

    if (ui->AxisFivecheckBox->isChecked())
    {
        ui->AxisFiveAcclineEdit->setReadOnly(false);
        ui->AxisFiveDeclineEdit->setReadOnly(false);
        ui->AxisFiveSmtlineEdit->setReadOnly(false);
        ui->AxisFiveTargetLocationlineEdit->setReadOnly(false);
        ui->AxisFiveTargetVelocitylineEdit->setReadOnly(false);
    }
    else
    {
        ui->AxisFiveAcclineEdit->setReadOnly(true);
        ui->AxisFiveDeclineEdit->setReadOnly(true);
        ui->AxisFiveSmtlineEdit->setReadOnly(true);
        ui->AxisFiveTargetLocationlineEdit->setReadOnly(true);
        ui->AxisFiveTargetVelocitylineEdit->setReadOnly(true);
    }
}



/*///////////////////////////////////////////////
 * 函数名：on_AxisSixcheckBox_clicked
 * 功能：当复选框被选中时，只读状态被取消
 //////////////////////////////////////////////*/
void PIDWidget::on_AxisSixcheckBox_stateChanged(int arg1)
{
    if (ui->AxisSixcheckBox->isChecked())
    {
        ui->AxisSixAcclineEdit->setReadOnly(false);
        ui->AxisSixDeclineEdit->setReadOnly(false);
        ui->AxisSixSmtlineEdit->setReadOnly(false);
        ui->AxisSixTargetLocationlineEdit->setReadOnly(false);
        ui->AxisSixTargetVelocitylineEdit->setReadOnly(false);
    }
    else
    {
        ui->AxisSixAcclineEdit->setReadOnly(true);
        ui->AxisSixDeclineEdit->setReadOnly(true);
        ui->AxisSixSmtlineEdit->setReadOnly(true);
        ui->AxisSixTargetLocationlineEdit->setReadOnly(true);
        ui->AxisSixTargetVelocitylineEdit->setReadOnly(true);
    }
}




/*///////////////////////////////////////////////
 * 函数名：on_AxisOnecheckBox_clicked
 * 功能：当复选框被选中时，只读状态被取消
 //////////////////////////////////////////////*/
void PIDWidget::on_AxisOnecheckBox_stateChanged(int arg1)
{
    if (ui->AxisOnecheckBox->isChecked())
    {
        ui->AxisOneAcclineEdit->setReadOnly(false);
        ui->AxisOneDeclineEdit->setReadOnly(false);
        ui->AxisOneSmtlineEdit->setReadOnly(false);
        ui->AxisOneTargetLocationlineEdit->setReadOnly(false);
        ui->AxisOneTargetVelocitylineEdit->setReadOnly(false);
    }
    else
    {
        ui->AxisOneAcclineEdit->setReadOnly(true);
        ui->AxisOneDeclineEdit->setReadOnly(true);
        ui->AxisOneSmtlineEdit->setReadOnly(true);
        ui->AxisOneTargetLocationlineEdit->setReadOnly(true);
        ui->AxisOneTargetVelocitylineEdit->setReadOnly(true);
    }
}




/*///////////////////////////////////////////////
 * 函数名：on_AxisTwocheckBox_clicked
 * 功能：当复选框被选中时，只读状态被取消
 //////////////////////////////////////////////*/
void PIDWidget::on_AxisTwocheckBox_stateChanged(int arg1)
{
    if (ui->AxisTwocheckBox->isChecked())
    {
        ui->AxisTwoAcclineEdit->setReadOnly(false);
        ui->AxisTwoDeclineEdit->setReadOnly(false);
        ui->AxisTwoSmtlineEdit->setReadOnly(false);
        ui->AxisTwoTargetLocationlineEdit->setReadOnly(false);
        ui->AxisTwoTargetVelocitylineEdit->setReadOnly(false);
    }
    else
    {
        ui->AxisTwoAcclineEdit->setReadOnly(true);
        ui->AxisTwoDeclineEdit->setReadOnly(true);
        ui->AxisTwoSmtlineEdit->setReadOnly(true);
        ui->AxisTwoTargetLocationlineEdit->setReadOnly(true);
        ui->AxisTwoTargetVelocitylineEdit->setReadOnly(true);
    }
}



/*///////////////////////////////////////////////
 * 函数名：on_AxisThreecheckBox_clicked
 * 功能：当复选框被选中时，只读状态被取消
 //////////////////////////////////////////////*/
void PIDWidget::on_AxisThreecheckBox_stateChanged(int arg1)
{
    if (ui->AxisThreecheckBox->isChecked())
    {
        ui->AxisThreeAcclineEdit->setReadOnly(false);
        ui->AxisThreeDeclineEdit->setReadOnly(false);
        ui->AxisThreeSmtlineEdit->setReadOnly(false);
        ui->AxisThreeTargetLocationlineEdit->setReadOnly(false);
        ui->AxisThreeTargetVelocitylineEdit->setReadOnly(false);
    }
    else
    {
        ui->AxisThreeAcclineEdit->setReadOnly(true);
        ui->AxisThreeDeclineEdit->setReadOnly(true);
        ui->AxisThreeSmtlineEdit->setReadOnly(true);
        ui->AxisThreeTargetLocationlineEdit->setReadOnly(true);
        ui->AxisThreeTargetVelocitylineEdit->setReadOnly(true);
    }
}




/*//////////////////////////////////////////////
 * 函数名：on_ClearStateButton_clicked
 * 功能：输入框中数字清空，复选框清空，datas内的变量初始化为0
 /////////////////////////////////////////////*/
void PIDWidget::on_ClearStateButton_clicked()
{
    if (ui->DeltaRobotAcheckBox->isChecked())
    {
        ui->DeltaRobotAcheckBox->setCheckState(Qt::Unchecked);
    }
    if (ui->DeltaRobotBcheckBox->isChecked())
    {
        ui->DeltaRobotBcheckBox->setCheckState(Qt::Unchecked);
    }
    if (ui->AxisOnecheckBox->isChecked())
    {
        ui->AxisOnecheckBox->setCheckState(Qt::Unchecked);
    }
    if (ui->AxisTwocheckBox->isChecked())
    {
        ui->AxisTwocheckBox->setCheckState(Qt::Unchecked);
    }
    if (ui->AxisThreecheckBox->isChecked())
    {
        ui->AxisThreecheckBox->setCheckState(Qt::Unchecked);
    }
    if (ui->AxisFourcheckBox->isChecked())
    {
        ui->AxisFourcheckBox->setCheckState(Qt::Unchecked);
    }
    if (ui->AxisFivecheckBox->isChecked())
    {
        ui->AxisFivecheckBox->setCheckState(Qt::Unchecked);
    }
    if (ui->AxisSixcheckBox->isChecked())
    {
        ui->AxisSixcheckBox->setCheckState(Qt::Unchecked);
    }

    datas.AxisOneAcc=0;
    datas.AxisTwoAcc=0;
    datas.AxisThreeAcc=0;
    datas.AxisFourAcc=0;
    datas.AxisFiveAcc=0;
    datas.AxisSixAcc=0;

    datas.AxisOneDec=0;
    datas.AxisTwoDec=0;
    datas.AxisThreeDec=0;
    datas.AxisFourDec=0;
    datas.AxisFiveDec=0;
    datas.AxisSixDec=0;

    datas.AxisOneSmt=0;
    datas.AxisTwoSmt=0;
    datas.AxisThreeSmt=0;
    datas.AxisFourSmt=0;
    datas.AxisFiveSmt=0;
    datas.AxisSixSmt=0;

    datas.AixsOneTargetLocation=0;
    datas.AixsTwoTargetLocation=0;
    datas.AixsThreeTargetLocation=0;
    datas.AixsFourTargetLocation=0;
    datas.AixsFiveTargetLocation=0;
    datas.AixsSixTargetLocation=0;

    datas.AxisOneTargetVelocity=0;
    datas.AxisTwoTargetVelocity=0;
    datas.AxisThreeTargetVelocity=0;
    datas.AxisFourTargetVelocity=0;
    datas.AxisFiveTargetVelocity=0;
    datas.AxisSixTargetVelocity=0;
}
//#####################################################################################################



/*////////////////////////////////////////////
 * 函数名：on_EnableButton_clicked
 * 功能：初始化并且把值赋给运动控制卡启动点位模式并使能
 *      先清空缓冲区，然后运动结束后把缓冲区读数输出到
 *      文件
 ///////////////////////////////////////////*/
void PIDWidget::on_EnableButton_clicked()
{
    short sRtn;

    TTrapPrm trapone;
    TTrapPrm traptwo;
    TTrapPrm trapthree;
    TTrapPrm trapfour;
    TTrapPrm trapfive;
    TTrapPrm trapsix;
    //清空缓冲区
    encvecone.clear();
    encvectwo.clear();
    encvecthree.clear();
    encvecfour.clear();
    encvecfive.clear();
    encvecsix.clear();
    timecount.clear();

    if (ui->DeltaRobotAcheckBox->isChecked())
    {
        if (ui->AxisOnecheckBox->isChecked())
        {

            configure(datas.axisOne,"axisOne_");

            sRtn=GT_GetTrapPrm(datas.axisOne,&trapone);
            CheckError(sRtn,"axisOne_GT_GetTrapPrm has a problem: ");

            trapone.acc=datas.AxisOneAcc;
            trapone.dec=datas.AxisOneDec;
            trapone.smoothTime=datas.AxisOneSmt;

            sRtn=GT_GetTrapPrm(datas.axisOne,&trapone);
            CheckError(sRtn,"axisOne_GT_GetTrapPrm has a problem: ");

            //qDebug()<<trapone.acc<<endl<<trapone.dec<<endl<<trapone.smoothTime;

            sRtn=GT_SetPos(datas.axisOne,datas.AixsOneTargetLocation);
            CheckError(sRtn,"axisOne_GT_SetPos has a problem: ");

            sRtn=GT_SetVel(datas.axisOne,datas.AxisOneTargetVelocity);
            CheckError(sRtn,"axisOne_GT_SetVel has a problem: ");

            sRtn=GT_Update(1<<(datas.axisOne-1));

            //qDebug()<<(1<<(datas.axisOne-1));

            CheckError(sRtn,"axisOne_GT_Update has a problem: ");

            //qDebug()<<datas.AxisOneAcc<<endl<<datas.AxisOneDec<<endl<<datas.AxisOneSmt<<endl<<datas.AixsOneTargetLocation<<endl<<datas.AxisOneTargetVelocity;





        }
        if (ui->AxisTwocheckBox->isChecked())
        {
            configure(datas.axisTwo,"axisTwo_");

            sRtn=GT_GetTrapPrm(datas.axisTwo,&traptwo);
            CheckError(sRtn,"axisTwo_GT_GetTrapPrm has a problem: ");

            traptwo.acc=datas.AxisTwoAcc;
            traptwo.dec=datas.AxisTwoDec;
            traptwo.smoothTime=datas.AxisTwoSmt;

            sRtn=GT_GetTrapPrm(datas.axisTwo,&traptwo);
            CheckError(sRtn,"axisTwo_GT_GetTrapPrm has a problem: ");

            sRtn=GT_SetPos(datas.axisTwo,datas.AixsTwoTargetLocation);
            CheckError(sRtn,"axisTwo_GT_SetPos has a problem: ");

            sRtn=GT_SetVel(datas.axisTwo,datas.AxisTwoTargetVelocity);
            CheckError(sRtn,"axisTwo_GT_SetVel has a problem: ");

            sRtn=GT_Update(1<<(datas.axisTwo-1));
            CheckError(sRtn,"axisTwo_GT_Update has a problem: ");
        }
        if (ui->AxisThreecheckBox->isChecked())
        {
            configure(datas.axisThree,"axisThree_");

            sRtn=GT_GetTrapPrm(datas.axisThree,&trapthree);
            CheckError(sRtn,"axisThree_GT_GetTrapPrm has a problem: ");

            trapthree.acc=datas.AxisThreeAcc;
            trapthree.dec=datas.AxisThreeDec;
            trapthree.smoothTime=datas.AxisThreeSmt;

            sRtn=GT_GetTrapPrm(datas.axisThree,&trapthree);
            CheckError(sRtn,"axisThree_GT_GetTrapPrm has a problem: ");

            sRtn=GT_SetPos(datas.axisThree,datas.AixsThreeTargetLocation);
            CheckError(sRtn,"axisThree_GT_SetPos has a problem: ");

            sRtn=GT_SetVel(datas.axisThree,datas.AxisThreeTargetVelocity);
            CheckError(sRtn,"axisThree_GT_SetVel has a problem: ");

            sRtn=GT_Update(1<<(datas.axisThree-1));
            CheckError(sRtn,"axisThree_GT_Update has a problem: ");

        }
        if (!ui->AxisOnecheckBox->isChecked() && !ui->AxisTwocheckBox->isChecked() && !ui->AxisThreecheckBox->isChecked())
        {
            QMessageBox::information(this,"Information","DeltaRobotA has no axis selected!");
        }
    }
    if (ui->DeltaRobotBcheckBox->isChecked())
    {
        if (ui->AxisFourcheckBox->isChecked())
        {
            configure(datas.axisFour,"axisFour_");

            sRtn=GT_GetTrapPrm(datas.axisFour,&trapfour);
            CheckError(sRtn,"axisFour_GT_GetTrapPrm has a problem: ");

            trapfour.acc=datas.AxisFourAcc;
            trapfour.dec=datas.AxisFourDec;
            trapfour.smoothTime=datas.AxisFourSmt;

            sRtn=GT_GetTrapPrm(datas.axisFour,&trapfour);
            CheckError(sRtn,"axisFour_GT_GetTrapPrm has a problem: ");

            sRtn=GT_SetPos(datas.axisFour,datas.AixsFourTargetLocation);
            CheckError(sRtn,"axisFour_GT_SetPos has a problem: ");

            sRtn=GT_SetVel(datas.axisFour,datas.AxisFourTargetVelocity);
            CheckError(sRtn,"axisFour_GT_SetVel has a problem: ");

            sRtn=GT_Update(1<<(datas.axisFour-1));
            CheckError(sRtn,"axisFour_GT_Update has a problem: ");

        }
        if (ui->AxisFivecheckBox->isChecked())
        {
            configure(datas.axisFive,"axisFive_");\

            sRtn=GT_GetTrapPrm(datas.axisFive,&trapfive);
            CheckError(sRtn,"axisFive_GT_GetTrapPrm has a problem: ");

            trapfive.acc=datas.AxisFiveAcc;
            trapfive.dec=datas.AxisFiveDec;
            trapfive.smoothTime=datas.AxisFiveSmt;

            sRtn=GT_GetTrapPrm(datas.axisFive,&trapfive);
            CheckError(sRtn,"axisFive_GT_GetTrapPrm has a problem: ");

            sRtn=GT_SetPos(datas.axisFive,datas.AixsFiveTargetLocation);
            CheckError(sRtn,"axisFive_GT_SetPos has a problem: ");

            sRtn=GT_SetVel(datas.axisFive,datas.AxisFiveTargetVelocity);
            CheckError(sRtn,"axisFive_GT_SetVel has a problem: ");

            sRtn=GT_Update(1<<(datas.axisFive-1));
            CheckError(sRtn,"axisFive_GT_Update has a problem: ");
        }
        if (ui->AxisSixcheckBox->isChecked())
        {
            configure(datas.axisSix,"axisSix_");

            sRtn=GT_GetTrapPrm(datas.axisSix,&trapsix);
            CheckError(sRtn,"axisSix_GT_GetTrapPrm has a problem: ");

            trapsix.acc=datas.AxisSixAcc;
            trapsix.dec=datas.AxisSixDec;
            trapsix.smoothTime=datas.AxisSixSmt;

            sRtn=GT_GetTrapPrm(datas.axisSix,&trapsix);
            CheckError(sRtn,"axisSix_GT_GetTrapPrm has a problem: ");

            sRtn=GT_SetPos(datas.axisSix,datas.AixsSixTargetLocation);
            CheckError(sRtn,"axisSix_GT_SetPos has a problem: ");

            sRtn=GT_SetVel(datas.axisSix,datas.AxisSixTargetVelocity);
            CheckError(sRtn,"axisSix_GT_SetVel has a problem: ");

            sRtn=GT_Update(1<<(datas.axisSix-1));
            CheckError(sRtn,"axisSix_GT_Update has a problem: ");
        }
        if (!ui->AxisFourcheckBox->isChecked() && !ui->AxisFivecheckBox->isChecked() && !ui->AxisSixcheckBox->isChecked())
        {
            QMessageBox::information(this,"Information","DeltaRobotB has no axis selected!");
        }

    }
    if (!ui->DeltaRobotAcheckBox->isChecked() && !ui->DeltaRobotBcheckBox->isChecked())
    {
        QMessageBox::information(this,"Information","No robot is selected!");
    }


    //使用定时器把编码器的读数反应到输入框中
    connect(timer, SIGNAL(timeout()), this, SLOT(EncDataDisplay()));
    timer->start(10);


}


/*/////////////////////////////////////////////
 * 函数名：EncDatadisplay
 * 功能：timer的槽函数，当时间溢出时调用该槽函数,用于显示
 *      编码器读数，并将实际编码器读数输出到缓冲区
 ////////////////////////////////////////////*/
void PIDWidget::EncDataDisplay()
{

        QString strnum;
        short sRtn;

        if (timer->isActive())
        {
            ui->RobotAAxisXlineEdit->clear();
            sRtn=GT_GetPrfPos(datas.axisOne,&prfPosone);
           // sRtn=GT_GetEncPos(datas.axisOne,&encPosone);
            encPosone=LastPosone+prfPosone;
            ui->RobotAAxisXlineEdit->setText(strnum.setNum(encPosone*locationfactor));

            ui->RobotAAxisYlineEdit->clear();
            sRtn=GT_GetPrfPos(datas.axisTwo,&prfPostwo);
            //sRtn=GT_GetEncPos(datas.axisTwo,&encPostwo);
            encPostwo=LastPostwo+prfPostwo;
            ui->RobotAAxisYlineEdit->setText(strnum.setNum(encPostwo*locationfactor));

            ui->RobotAAxisZlineEdit->clear();
            sRtn=GT_GetPrfPos(datas.axisThree,&prfPosthree);
            //sRtn=GT_GetEncPos(datas.axisThree,&encPosthree);
            encPosthree=LastPosthree+prfPosthree;
            ui->RobotAAxisZlineEdit->setText(strnum.setNum(encPosthree*locationfactor));

            ui->RobotBAxisXlineEdit->clear();
            sRtn=GT_GetPrfPos(datas.axisFour,&prfPosfour);
            //sRtn=GT_GetEncPos(datas.axisFour,&encPosfour);
            encPosfour=LastPosfour+prfPosfour;
            ui->RobotBAxisXlineEdit->setText(strnum.setNum(encPosfour*locationfactor));

            ui->RobotBAxisYlineEdit->clear();
            sRtn=GT_GetPrfPos(datas.axisFive,&prfPosfive);
            //sRtn=GT_GetEncPos(datas.axisFive,&encPosfive);
            encPosfive=LastPosfive+prfPosfive;
            ui->RobotBAxisYlineEdit->setText(strnum.setNum(encPosfive*locationfactor));

            ui->RobotBAxisZlineEdit->clear();
            sRtn=GT_GetPrfPos(datas.axisSix,&prfPossix);
            //sRtn=GT_GetEncPos(datas.axisSix,&encPossix);
            encPossix=LastPossix+prfPossix;
            ui->RobotBAxisZlineEdit->setText(strnum.setNum(encPossix*locationfactor));

            timeco=timeco+10;
            GT_GetEncPos(1,&enc[0],6);   //将编码器读数输入到缓冲区数组中
            encvecone.push_back(MiddleVectorStr.setNum(enc[0]));
            encvectwo.push_back(MiddleVectorStr.setNum(enc[1]));
            encvecthree.push_back(MiddleVectorStr.setNum(enc[2]));
            encvecfour.push_back(MiddleVectorStr.setNum(enc[3]));
            encvecfive.push_back(MiddleVectorStr.setNum(enc[4]));
            encvecsix.push_back(MiddleVectorStr.setNum(enc[5]));
            timecount.push_back(MiddleVectorStr.setNum(timeco));

            if ((long)prfPosone==(long)datas.AixsOneTargetLocation && (long)prfPostwo==(long)datas.AixsTwoTargetLocation && (long)prfPosthree==(long)datas.AixsThreeTargetLocation && (long)prfPosfour==(long)datas.AixsFourTargetLocation && (long)prfPosfive==(long)datas.AixsFiveTargetLocation && (long)prfPossix==(long)datas.AixsSixTargetLocation)
            {
                timer->stop();

            }
            if (!timer->isActive())
            {

            LastPosone=encPosone;
            LastPostwo=encPostwo;
            LastPosthree=encPosthree;
            LastPosfour=encPosfour;
            LastPosfive=encPosfive;
            LastPossix=encPossix;


            }
        }

}





/*///////////////////////////////////////////
 * 函数名：on_ReturnZeroButton_clicked
 * 功能：读取data.txt中编码器的读数，然后进行回零
 //////////////////////////////////////////*/
void PIDWidget::on_ReturnZeroButton_clicked()
{
    QFile fileA("data.txt");

    if (!fileA.open(QIODevice::ReadOnly|QIODevice::Text))
        QMessageBox::information(this,"Information","File don't open!");

       QTextStream in(&fileA);

       for (int i=0;i<6;i++)
       {
           in>>Encdata[i];
       }
       fileA.close();

    if (Encdata[0])
    {
    threadone->start();
    }
    if (Encdata[1])
    {
    threadtwo->start();
    }

    if (Encdata[2])
    {
    threadthree->start();
    }

    if (Encdata[3])
    {
    threadfour->start();
    }

    if (Encdata[4])
    {
    threadfive->start();
    }

    if (Encdata[5])
    {
    threadsix->start();
    }


}

/*//////////////////////////////////////////
 * 函数名：
 * 功能：axisoneThread中threadonesignal对应的槽函数
 *      用来显示编码器的读数
 /////////////////////////////////////////*/
//一轴必须要有运动
void PIDWidget::threadoneslot(double  axisoneencPosone)
{
    double Pos;
    ui->RobotAAxisXlineEdit->clear();
    Pos=axisoneencPosone*locationfactor;
    ui->RobotAAxisXlineEdit->setText(QString::number(Pos));
    LastPosone=0;

    timeco=timeco+10;
    GT_GetEncPos(1,&enc[0],6);   //将编码器读数输入到缓冲区数组中
    encvecone.push_back(MiddleVectorStr.setNum(enc[0]));
    encvectwo.push_back(MiddleVectorStr.setNum(enc[1]));
    encvecthree.push_back(MiddleVectorStr.setNum(enc[2]));
    encvecfour.push_back(MiddleVectorStr.setNum(enc[3]));
    encvecfive.push_back(MiddleVectorStr.setNum(enc[4]));
    encvecsix.push_back(MiddleVectorStr.setNum(enc[5]));
    timecount.push_back(MiddleVectorStr.setNum(timeco));
}

/*//////////////////////////////////////////
 * 函数名：
 * 功能：axistwoThread中threadtwosignal对应的槽函数
 *      用来显示编码器的读数
 /////////////////////////////////////////*/
void PIDWidget::threadtwoslot(double  axistwoencPostwo)
{
    double Pos;
    ui->RobotAAxisYlineEdit->clear();
    Pos=axistwoencPostwo*locationfactor;
    ui->RobotAAxisYlineEdit->setText(QString::number(Pos));
    LastPostwo=0;
}

/*//////////////////////////////////////////
 * 函数名：
 * 功能：axisthreeThread中threadthreesignal对应的槽函数
 *      用来显示编码器的读数
 /////////////////////////////////////////*/
void PIDWidget::threadthreeslot(double  axisthreeencPosthree)
{
    double Pos;
    ui->RobotAAxisZlineEdit->clear();
    Pos=axisthreeencPosthree*locationfactor;
    ui->RobotAAxisZlineEdit->setText(QString::number(Pos));
    LastPosthree=0;
}

/*//////////////////////////////////////////
 * 函数名：
 * 功能：axisfourThread中threadfoursignal对应的槽函数
 *      用来显示编码器的读数
 /////////////////////////////////////////*/
void PIDWidget::threadfourslot(double  axisfourencPosfour)
{
    double Pos;
    ui->RobotBAxisXlineEdit->clear();
    Pos=axisfourencPosfour*locationfactor;
    ui->RobotBAxisXlineEdit->setText(QString::number(Pos));
    LastPosfour=0;
}

/*//////////////////////////////////////////
 * 函数名：
 * 功能：axisfiveThread中threadfivesignal对应的槽函数
 *      用来显示编码器的读数
 /////////////////////////////////////////*/
void PIDWidget::threadfiveslot(double  axisfiveencPosfive)
{
    double Pos;
    ui->RobotBAxisYlineEdit->clear();
    Pos=axisfiveencPosfive*locationfactor;
    ui->RobotBAxisYlineEdit->setText(QString::number(Pos));
    LastPosfive=0;
}

/*//////////////////////////////////////////
 * 函数名：
 * 功能：axissixThread中threadsixsignal对应的槽函数
 *      用来显示编码器的读数
 /////////////////////////////////////////*/
void PIDWidget::threadsixslot(double  axissixencPossix)
{
    double Pos;
    ui->RobotBAxisZlineEdit->clear();
    Pos=axissixencPossix*locationfactor;
    ui->RobotBAxisZlineEdit->setText(QString::number(Pos));
    LastPossix=0;
}

/*//////////////////////////////////////////
 * 函数名：on_Outputdatabutton_clicked()
 * 功能：输出缓冲区数据
 /////////////////////////////////////////*/
void PIDWidget::on_Outputdatabutton_clicked()
{
    //输出缓冲区数据
    QFile fileone("Encoder_data_one.txt");

    if (!fileone.open(QIODevice::Append|QIODevice::Text))
        QMessageBox::information(this,"Information","File don't open!");

       QTextStream outone(&fileone);

       outone<<"A beautiful line----------------------"<<'\n';
       int i;
       for(i=0;i<encvecone.size();i++)
       {
           outone<<encvecone[i]<<"     "<<timecount[i]<<'\n';

       }
       fileone.close();

       //////////////////////

       QFile filetwo("Encoder_data_two.txt");

       if (!filetwo.open(QIODevice::Append|QIODevice::Text))
           QMessageBox::information(this,"Information","File don't open!");

          QTextStream outtwo(&filetwo);

          outtwo<<"A beautiful line----------------------"<<'\n';

          for(i=0;i<encvectwo.size();i++)
          {
              outtwo<<encvectwo[i]<<"     "<<timecount[i]<<'\n';

          }
          filetwo.close();

          //////////////////////

          QFile filethree("Encoder_data_three.txt");

          if (!filethree.open(QIODevice::Append|QIODevice::Text))
              QMessageBox::information(this,"Information","File don't open!");

             QTextStream outthree(&filethree);

             outthree<<"A beautiful line----------------------"<<'\n';

             for(i=0;i<encvecthree.size();i++)
             {
                 outthree<<encvecthree[i]<<"     "<<timecount[i]<<'\n';

             }
             filethree.close();

             //////////////////////

             QFile filefour("Encoder_data_four.txt");

             if (!filefour.open(QIODevice::Append|QIODevice::Text))
                 QMessageBox::information(this,"Information","File don't open!");

                QTextStream outfour(&filefour);

                outfour<<"A beautiful line----------------------"<<'\n';

                for(i=0;i<encvecfour.size();i++)
                {
                    outfour<<encvecfour[i]<<"     "<<timecount[i]<<'\n';

                }
                filefour.close();

                //////////////////////

                QFile filefive("Encoder_data_five.txt");

                if (!filefive.open(QIODevice::Append|QIODevice::Text))
                    QMessageBox::information(this,"Information","File don't open!");

                   QTextStream outfive(&filefive);

                   outfive<<"A beautiful line----------------------"<<'\n';

                   for(i=0;i<encvecfive.size();i++)
                   {
                       outfive<<encvecfive[i]<<"     "<<timecount[i]<<'\n';

                   }
                   filefive.close();

                   //////////////////////

                   QFile filesix("Encoder_data_six.txt");

                   if (!filesix.open(QIODevice::Append|QIODevice::Text))
                       QMessageBox::information(this,"Information","File don't open!");

                      QTextStream outsix(&filesix);

                      outsix<<"A beautiful line----------------------"<<'\n';

                      for(i=0;i<encvecsix.size();i++)
                      {
                          outsix<<encvecsix[i]<<"     "<<timecount[i]<<'\n';

                      }
                      filesix.close();


}
