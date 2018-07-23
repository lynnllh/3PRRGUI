#ifndef AXISSIXTHREAD_H
#define AXISSIXTHREAD_H


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


#define AXISSIX 6
#define locationfactor (1.0/2048*10)

class axissixThread : public QThread
{
    Q_OBJECT
public:
    axissixThread(QObject *parent=0);
    ~axissixThread();
    void CheckError(short,QString);
protected:
    void run();
private:
    double axissixencPossix;
    double axissixLastPossix;
    double axissixprfPossix;
    double encdata[6];
    double Enc;
    int switchId;
   // QTimer *timer;
signals:
    void threadsixsignal(double  axissixencPossix);
    void timeout();
private slots:
        void EncDataDisplay();

};
#endif // AXISSIXTHREAD_H

