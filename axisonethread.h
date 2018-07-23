#ifndef AXISONETHREAD_H
#define AXISONETHREAD_H

#include <QThread>
#include <QFile>
#include <QIODevice>
#include <QTextStream>
#include <QMessageBox>
#include <QWidget>
#include "gts.h"
#include <QString>
#include <windows.h>
#include <QTimer>


#define AXISONE 1
#define locationfactor (1.0/2048*10)

class axisoneThread : public QThread
{
    Q_OBJECT
public:
    axisoneThread(QObject *parent=0);
    ~axisoneThread();
    void CheckError(short,QString);
protected:
    void run();
private:
    double axisoneencPosone;
    double axisoneLastPosone;
    double axisoneprfPosone;
    double encdata[6];
    double Enc;
    int switchId;
   // QTimer *timer;
signals:
    void threadonesignal(double  axisoneencPosone);
    void timeout();
private slots:
        void EncDataDisplay();

};

#endif // AXISONETHREAD_H

