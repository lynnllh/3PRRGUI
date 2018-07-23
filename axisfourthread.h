#ifndef AXISFOURTHREAD_H
#define AXISFOURTHREAD_H


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


#define AXISFOUR 4
#define locationfactor (1.0/2048*10)

class axisfourThread : public QThread
{
    Q_OBJECT
public:
    axisfourThread(QObject *parent=0);
    ~axisfourThread();
    void CheckError(short,QString);
protected:
    void run();
private:
    double axisfourencPosfour;
    double axisfourLastPosfour;
    double axisfourprfPosfour;
    double encdata[6];
    double Enc;
    int switchId;
   // QTimer *timer;
signals:
    void threadfoursignal(double  axisfourencPosfour);
    void timeout();
private slots:
        void EncDataDisplay();

};
#endif // AXISFOURTHREAD_H

