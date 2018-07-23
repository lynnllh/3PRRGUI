#ifndef PIDWIDGET_H
#define PIDWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QMessageBox>
#include "gts.h"
#include <QCheckBox>
#include <QLineEdit>
#include <QtDebug>
#include <QFile>
#include <QTextStream>
#include <QIODevice>
#include <QTimer>
#include "axisonethread.h"
#include "axistwothread.h"
#include "axisthreethread.h"
#include "axisfourthread.h"
#include "axisfivethread.h"
#include "axissixthread.h"
#include <Vector>



#define locationfactor (1.0/2048*10)
#define velocityfactor (1000.0/2048*10)
#define accfactor (1000000.0/2048*10)
#define decfactor (1000000.0/2048*10)

namespace Ui {
class PIDWidget;
}

class PIDWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PIDWidget(QWidget *parent = 0);
    ~PIDWidget();
    void CheckError(short,QString);
    void configure(int,QString);


private slots:
    void on_OpenControlButton_clicked();

    void on_CloseButton_clicked();

    void on_DeltaRobotAcheckBox_clicked();

    void on_DeltaRobotBcheckBox_clicked();

    void on_WriteInStateButton_clicked();

    void on_AxisFourcheckBox_stateChanged(int arg1);

    void on_AxisFivecheckBox_stateChanged(int arg1);

    void on_AxisSixcheckBox_stateChanged(int arg1);

    void on_AxisOnecheckBox_stateChanged(int arg1);

    void on_AxisTwocheckBox_stateChanged(int arg1);

    void on_AxisThreecheckBox_stateChanged(int arg1);



    void on_ClearStateButton_clicked();

    void on_EnableButton_clicked();

    void on_ReturnZeroButton_clicked();

    void EncDataDisplay();

    void on_Outputdatabutton_clicked();

public slots:
    void threadoneslot(double axisoneencPosone);
    void threadtwoslot(double axistwoencPostwo);
    void threadthreeslot(double axisthreeencPosthree);
    void threadfourslot(double axisfourencPosfour);
    void threadfiveslot(double axisfiveencPosfive);
    void threadsixslot(double axissixencPossix);


private:
    Ui::PIDWidget *ui;
     QTimer *timer ;
     double Encdata[6];
     double enc[6];
     axisoneThread * threadone;
     axistwoThread * threadtwo;
     axisthreeThread * threadthree;
     axisfourThread * threadfour;
     axisfiveThread * threadfive;
     axissixThread * threadsix;

     std::vector<QString> encvecone;  //设立输出缓冲区
     std::vector<QString> encvectwo;
     std::vector<QString> encvecthree;
     std::vector<QString> encvecfour;
     std::vector<QString> encvecfive;
     std::vector<QString> encvecsix;
     std::vector<QString> timecount;
     long timeco=0;
     QString MiddleVectorStr;

     double prfPosone;
     double prfPostwo;
     double prfPosthree;
     double prfPosfour;
     double prfPosfive;
     double prfPossix;

     double encPosone;
     double encPostwo;
     double encPosthree;
     double encPosfour;
     double encPosfive;
     double encPossix;

     double LastPosone;
     double LastPostwo;
     double LastPosthree;
     double LastPosfour;
     double LastPosfive;
     double LastPossix;



    struct data
    {
        enum {axisOne=1,axisTwo,axisThree,axisFour,axisFive,axisSix};

        double AxisOneAcc;
        double AxisTwoAcc;
        double AxisThreeAcc;
        double AxisFourAcc;
        double AxisFiveAcc;
        double AxisSixAcc;

        double AxisOneDec;
        double AxisTwoDec;
        double AxisThreeDec;
        double AxisFourDec;
        double AxisFiveDec;
        double AxisSixDec;

        int AxisOneSmt;
        int AxisTwoSmt;
        int AxisThreeSmt;
        int AxisFourSmt;
        int AxisFiveSmt;
        int AxisSixSmt;

        double AixsOneTargetLocation;
        double AixsTwoTargetLocation;
        double AixsThreeTargetLocation;
        double AixsFourTargetLocation;
        double AixsFiveTargetLocation;
        double AixsSixTargetLocation;

        double AxisOneTargetVelocity;
        double AxisTwoTargetVelocity;
        double AxisThreeTargetVelocity;
        double AxisFourTargetVelocity;
        double AxisFiveTargetVelocity;
        double AxisSixTargetVelocity;




    }datas;
};

#endif // PIDWIDGET_H
