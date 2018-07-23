#include "axisthreethread.h"


/*//////////////////////////////
 *函数名：CheckError
 *功能：出错提示函数，如果输入的sRtn为非零，则说明该功能未实现
       跳出一个对话框，提示出错信息。str为提示信息。
//////////////////////////////*/
void axisthreeThread::CheckError(short sRtn,QString str)
{
    if (sRtn)
    {
        QString str1;
        QMessageBox::warning(NULL,"Error information",str+str1.setNum(sRtn));
    }
    else
    {
        return;
    }
}



axisthreeThread::axisthreeThread(QObject *parent)
{
    connect(this,SIGNAL(timeout()),this,SLOT(EncDataDisplay()));
}

axisthreeThread::~axisthreeThread()
{}

/*//////////////////////////////////////
 * 函数名：run
 * 功能：读取data.txt中第一轴的编码器位置，反方向
 *      运动，并显示编码器示数变化
 /////////////////////////////////////*/
void axisthreeThread::run()
{
    QFile fileA("data.txt");

    axisthreeencPosthree=0;
    axisthreeLastPosthree=0;
    axisthreeprfPosthree=0;
    switchId=1;

    if (!fileA.open(QIODevice::ReadOnly|QIODevice::Text))
        QMessageBox::information(NULL,"Information","File don't open!");

       QTextStream in(&fileA);



       for (int i=0;i<6;i++)
       {
           in>>encdata[i];
       }

       fileA.close();

       // 例程 5-1 点位运动

           short sRtn;
           TTrapPrm trap;

           // 打开运动控制器
           sRtn = GT_Open();
           // 指令返回值检测，请查阅例2-1
           CheckError(sRtn,"axisthree_GT_Open has a problem: ");
           // 配置运动控制器
           // 注意：配置文件取消了各轴的报警和限位
           sRtn=GT_AlarmOff(AXISTHREE);
           CheckError(sRtn,"axisthree_GT_AlarmOff has a problem: ");

           sRtn=GT_LmtsOff(AXISTHREE,-1);
           CheckError(sRtn,"axisthree_GT_LmtsOff has a problem: ");
           // 清除各轴的报警和限位
           sRtn = GT_ClrSts(1, 8);
           CheckError(sRtn,"axisthree_GT_ClrSts has a problem: ");
           // 伺服使能
           sRtn = GT_AxisOn(AXISTHREE);
           CheckError(sRtn,"axisthree_GT_AxisOn has a problem: ");
            /*
           // 位置清零
           sRtn = GT_ZeroPos(AXISTHREE);
           commandhandler("GT_ZeroPos", sRtn);
           */
           // AXIS轴规划位置清零
           sRtn = GT_SetPrfPos(AXISTHREE, 0);
           CheckError(sRtn,"axisthree_GT_SetPrdPos has a problem: ");

           // 将AXIS轴设为点位模式
           sRtn = GT_PrfTrap(AXISTHREE);
           CheckError(sRtn,"axisthree_PrfTrap has a problem: ");
           // 读取点位运动参数
           sRtn = GT_GetTrapPrm(AXISTHREE, &trap);
           CheckError(sRtn,"axisthree_GetTrapPrm has a problem: ");
           trap.acc = 0.25;
           trap.dec = 0.125;
           trap.smoothTime = 25;
           // 设置点位运动参数
           sRtn = GT_SetTrapPrm(AXISTHREE, &trap);
           CheckError(sRtn,"axisthree_GetTrapPrm has a problem: ");
           // 设置AXIS轴的目标位置
           Enc=-1*encdata[2]/locationfactor;
           sRtn = GT_SetPos(AXISTHREE, Enc);
           CheckError(sRtn,"axisthree_SetPos has a problem: ");
           // 设置AXIS轴的目标速度
           sRtn = GT_SetVel(AXISTHREE, 5);
           CheckError(sRtn,"axisthree_SetVel has a problem: ");
           // 启动AXIS轴的运动
           sRtn = GT_Update(1<<(AXISTHREE-1));
           CheckError(sRtn,"axisthree_Update has a problem: ");



           axisthreeLastPosthree=encdata[2]/locationfactor;


            while (switchId)
            {
                Sleep(10);
                emit timeout();
            }




}


void axisthreeThread::EncDataDisplay()
{

    short sRtn;

    if (switchId)
    {

        sRtn=GT_GetPrfPos(AXISTHREE,&axisthreeprfPosthree);
       // sRtn=GT_GetEncPos(datas.axisthree,&encPosthree);
        axisthreeencPosthree=axisthreeLastPosthree+axisthreeprfPosthree;
        emit threadthreesignal(axisthreeencPosthree);

        if ((long)axisthreeprfPosthree==(long)Enc )
        {
            switchId=0;
           // axisthreeLastPosthree=axisthreeencPosthree;
            Sleep(500);
            sRtn = GT_AxisOff(AXISTHREE);


        }

    }
}
