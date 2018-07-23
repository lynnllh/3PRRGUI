#ifndef AXISTHREETHREAD_H
#define AXISTHREETHREAD_H


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


#define AXISTHREE 3
#define locationfactor (1.0/2048*10)

class axisthreeThread : public QThread
{
    Q_OBJECT
public:
    axisthreeThread(QObject *parent=0);
    ~axisthreeThread();
    void CheckError(short,QString);
protected:
    void run();
private:
    double axisthreeencPosthree;
    double axisthreeLastPosthree;
    double axisthreeprfPosthree;
    double encdata[6];
    double Enc;
    int switchId;
   // QTimer *timer;
signals:
    void threadthreesignal(double  axisthreeencPosthree);
    void timeout();
private slots:
        void EncDataDisplay();

};
#endif // AXISTHREETHREAD_H

