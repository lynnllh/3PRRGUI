#ifndef AXISTWOTHREAD_H
#define AXISTWOTHREAD_H


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


#define AXISTWO 2
#define locationfactor (1.0/2048*10)

class axistwoThread : public QThread
{
    Q_OBJECT
public:
    axistwoThread(QObject *parent=0);
    ~axistwoThread();
    void CheckError(short,QString);
protected:
    void run();
private:
    double axistwoencPostwo;
    double axistwoLastPostwo;
    double axistwoprfPostwo;
    double encdata[6];
    double Enc;
    int switchId;
   // QTimer *timer;
signals:
    void threadtwosignal(double  axistwoencPostwo);
    void timeout();
private slots:
        void EncDataDisplay();

};
#endif // AXISTWOTHREAD_H

