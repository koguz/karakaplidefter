#ifndef SMTP_H
#define SMTP_H
#include <QApplication>
#include <QTcpSocket>
#include <QString>
#include <QTextStream>
#include <QDebug>
#include <QMessageBox>
#include <QAbstractSocket>
#include <QDateTime>
#include <QDate>
#include <QLocale>

#include "_tools.h"

/*
      USAGE!
      Smtp *newMail  = new Smtp("remoteserver","username","pass");
      newMail->Send("from@address.com","to@address.com"," Your Subject","My body text");
      class emit ErrorCloseAll() / SuccessQuit()   or get QStringList ErrorMSG; to read error and data....
      
*/

class Smtp : public QObject
{
    Q_OBJECT
    
public:
    Smtp(QString smtphost, int port, QString smtpusername, QString smtppass);
    ~Smtp();
    void SetConfig(QString smtphost, int port, QString smtpusername, QString smtppass);
    void Send( const QString &from, const QStringList &to,const QString &subject, const QString &body );
    int linesend;
    QStringList ErrorMSG;
    bool isErr;
    QStringList GetErrorlist();
signals:
    void status( const QString &);
    void ConnectorSuccess();
    void SendLine();
    void ErrorCloseAll();
    void SuccessQuit();

private slots:
    void disconnected();
    void connected();
    void ReadLiner();
    void PutSendLine();
private:
    bool isconnect;
    QString smtphost;
    int port;
    QString smtpusername;
    QString smtppass;
    QString message;
    QString output;
    QString RemoteServerName;
    QString mailstatus;
    QTextStream *t;
    QTcpSocket *smtpsocket;
    QString from;
    QStringList rcpt;
    QString response;
    QString SendLineAndGrab(QString senddata);
    int Timeout;
    QString encodeBase64( QString xml );
    QString decodeBase64( QString xml );
    
 
};

#endif

 


