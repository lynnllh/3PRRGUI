#ifndef AXISFIVETHREAD_H
#define AXISFIVETHREAD_H


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


#define AXISFIVE 5
#define locationfactor (1.0/2048*10)

class axisfiveThread : public QThread
{
    Q_OBJECT
public:
    axisfiveThread(QObject *parent=0);
    ~axisfiveThread();
    void CheckError(short,QString);
protected:
    void run();
private:
    double axisfiveencPosfive;
    double axisfiveLastPosfive;
    double axisfiveprfPosfive;
    double encdata[6];
    double Enc;
    int switchId;
   // QTimer *timer;
signals:
    void threadfivesignal(double  axisfiveencPosfive);
    void timeout();
private slots:
        void EncDataDisplay();

};
#endif // AXISFIVETHREAD_H

