/* This file and smtp.h, _tools.h, _tools.cpp
 * was modified where needed... 
 * - added multiple rcpt (QStringList to)
 * - modified Latin1 to Utf8
 * - modified user-agent strings
 * - added port (int port)
 * - added isErr...
 * 
 * the original code is at
 * http://www.qtcentre.org/forum/f-qt-programming-2/t-sending-email-using-qt-2221.html
 * by patrik08
 *
 * modifications by Kaya...
 */

#include "smtp.h"

Smtp::Smtp(QString smtphost, int port, QString smtpusername, QString smtppass)
{
    // qDebug() << "### Class Smtp start"; 
    this->smtphost = smtphost;
    this->smtpusername = smtpusername;
    this->smtppass = smtppass; 
	this->port = port;
}
void Smtp::SetConfig(QString smtphost, int port, QString smtpusername, QString smtppass)
{
    this->smtphost = smtphost;
    this->smtpusername = smtpusername;
    this->smtppass = smtppass; 
	this->port = port;
}

void Smtp::Send( const QString &from, const QStringList &to, const QString &subject, const QString &body )
{
    int waittime = 5 * 1000;
    this->from = from;
    rcpt = to;
    ErrorMSG.clear();
	isErr = false;
    Timeout = waittime;
    linesend = 0;
    isconnect = false;
    
    
    message = TimeStampMail()+"\n";
    message.append("User-Agent: Kara Kaplı Defter (Using Qt4 TcpSocket)\n");
    message.append("X-Accept-Language: tr-TR, tr\n");
    message.append("MIME-Version: 1.0\n");
    message.append("To: ");
    for (int i=0;i<rcpt.size(); ++i)
	{
		message.append(rcpt.at(i));
		if (rcpt.size() < i) message.append(", ");
	}
	message.append("\n");
    message.append("From: "+from+" <" + from + ">\n");
    message.append("Subject: " + subject + "\n");
    message.append("Content-Type: text/plain; charset=UTF8;\n");
    message.append("Content-transfer-encoding: 7BIT\n\n\n\n");
    message.append(body);
    message.replace( QString::fromUtf8( "\n" ), QString::fromUtf8( "\r\n" ) );
    message.replace( QString::fromUtf8( "\r\n.\r\n" ),QString::fromUtf8( "\r\n..\r\n" ) );
    
    // qDebug() << "### Launch mail compose....  "  << from << to << subject << body << "\n\nport: " << port;
    // qDebug() << "### Config server smtp connect to......  "  << smtphost;
    smtpsocket = new QTcpSocket(this);
    connect( this, SIGNAL(ConnectorSuccess()), this ,SLOT(ReadLiner())); 
    connect( this, SIGNAL(SendLine()), this ,SLOT(PutSendLine())); 
    if (smtphost.size() > 0) {    
    smtpsocket->connectToHost(smtphost,port);
    } else {
    smtpsocket->connectToHost("localhost",port);   // aaah, buna ihtiyacımız var mı ki?
    }
    if (smtpsocket->waitForConnected(Timeout)) {
    // qDebug() <<"### connected on  " << smtphost; 
         if (smtpsocket->waitForReadyRead(Timeout)) {
          // qDebug() <<"### emit from waitForReadyRead connect go can read";
          isconnect = true;
          emit ConnectorSuccess();
          } 
    } else {
		isErr = true;
        emit ErrorCloseAll();
    }
   
}

void Smtp::ReadLiner()
{
    if (isconnect) {
        // qDebug() << "### socketType = " << smtpsocket->socketType();
        // qDebug() << "### ReadLiner is start by textstream ";
        t = new QTextStream( smtpsocket );
        int loops = 0;
        while (!t->atEnd()) {
        loops++;
        response = t->readLine();
        // qDebug() << loops << " in line  " << response;
        }
        if (response.size() > 0) {
        RemoteServerName = response;
        mailstatus = response.left(3);
        // qDebug() << "###Status=" << mailstatus;
                if (mailstatus == "220") {
                response="";
                linesend = 1;
                emit SendLine();
                }
        } else {
		   isErr = true;
           emit ErrorCloseAll(); 
        }
    }
}

Smtp::~Smtp()

{
    // qDebug() << "### Class Smtp stop and delete "; 
}

/* LINE SENDER  */
void Smtp::PutSendLine()
{
    int current = linesend;
    // qDebug() <<"### Go and Send line " << linesend;
    switch(current) {
      case 1:
          response = SendLineAndGrab("ehlo localhost");
           if (response.size() > 0) {
               ErrorMSG.append(response);
               // qDebug() << "1---- " << response;
                linesend = 2;
                emit SendLine();
           } else {
			   isErr = true;
                // qDebug() << "Connection loost";
           }
          response ="";
      break;
      case 2:
          response = SendLineAndGrab("AUTH LOGIN");
           if (response.size() > 0) {
           ErrorMSG.append(response);
                // qDebug() << "2---- " << response;
                linesend = 3;
                emit SendLine();
           } else {
			   isErr = true;
                // qDebug() << "Connection loost";
           }
          response ="";
      break;
      case 3:
          response = SendLineAndGrab(encodeBase64(smtpusername));   /* username send */
           if (response.size() > 0) {
           ErrorMSG.append(response);
                // qDebug() << "3---- " << response;
                linesend = 4;
                emit SendLine();
           } else {
			    isErr = true;
                // qDebug() << "Connection loost";
           }
          response ="";
      break;
      case 4:
          response = SendLineAndGrab(encodeBase64(smtppass));     /* pass send */
           // qDebug() << "4---- " << response;
           if (response.size() > 0) {
           ErrorMSG.append(response);
                if (response.contains("ok", Qt::CaseInsensitive)) {
                   linesend = 5;
                   emit SendLine();
                } else {
				   isErr = true;
                   // qDebug() << "icerdeki if Connection loost";
                }
           } else {
			   isErr = true;
                // qDebug() << "4'teki dis Connection loost";
           }
          response ="";
      break;
      case 5:
          response = SendLineAndGrab("MAIL FROM: "+from);
           // qDebug() << "5---- " << response;
           if (response.size() > 0) {
                linesend = 6;
                emit SendLine();               
           }
          
      break;
      case 6:
		  for (int i=0;i<rcpt.size(); ++i)
		  {
			response = SendLineAndGrab("RCPT TO: " + rcpt.at(i));
			// qDebug() << "6---- " << response;
		  }
           if (response.size() > 0) {
                ErrorMSG.append(response);
                response ="";
                response = SendLineAndGrab("DATA");
                         if (!response.contains("not", Qt::CaseInsensitive)) {
                         ErrorMSG.append(response);
                         response ="";             
                         linesend = 7;
                         emit SendLine();                      
                         }
           }
          response ="";
      break;
      case 7:
          response = SendLineAndGrab(message+"\r\n.");
           // qDebug() << "7---- " << response;
           if (response.size() && response.contains("ok", Qt::CaseInsensitive) ) {
                ErrorMSG.append(response);
                linesend = 8;
                emit SendLine();               
           }
           response ="";
      break;
      case 8:
          SendLineAndGrab("QUIT");
      break;
      default:
      // qDebug() << "Last line ...";
      /*emit ErrorCloseAll();*/
      return;
      break;
    }   
}

/* SENDER AND RECIVER  */
QString Smtp::SendLineAndGrab(QString senddata)
{
    QString incommingData = "";
    if (isconnect) {
   QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    int current = linesend;
    int loops = 0;
    
    // qDebug() << "####Send" << current << "Now => " << senddata;
    *t << senddata << "\r\n";
     t->flush();
        if (senddata != "QUIT") {
          if (smtpsocket->waitForReadyRead(Timeout)) {
                    while (!t->atEnd()) {
                    loops++;
                    QString opera = t->readLine()+"\n";
                    incommingData = opera + incommingData;
                    // qDebug() << loops << "|#" << opera << "#|";
                    }
          } 
        } else {
          QApplication::restoreOverrideCursor();
          delete smtpsocket;
          delete t;
          isconnect = false;
          emit SuccessQuit();
          return incommingData;
        }
    QApplication::restoreOverrideCursor();
      } else {
		  isErr = true;
      emit ErrorCloseAll();
      }
return incommingData;
}
 
void Smtp::disconnected()
{
    // qDebug() <<"disconneted";
    // qDebug() << "error "  << smtpsocket->errorString();
	// qDebug() << "isErr true oluyor, satir 283";
	isErr = true;
}
void Smtp::connected()
{
    output.append("connected");
    // qDebug() << "Connected ";
}

QString Smtp::encodeBase64( QString xml )
{
    QByteArray text;
    text.append(xml);
    return text.toBase64();
}

QString Smtp::decodeBase64( QString xml )
{
    QByteArray xcode("");;
    xcode.append(xml);
    QByteArray precode(QByteArray::fromBase64(xcode));
    QString notetxt = precode.data();
    return notetxt;
}


QStringList Smtp::GetErrorlist()
{
    return ErrorMSG;
}




/*
220 remote.com ESMTP
ehlo localhost
250-remote.com
250-STARTTLS
250-PIPELINING
250-8BITMIME
250-SIZE 0
250 AUTH LOGIN PLAIN CRAM-MD5
http://www.projektfarm.com/en/support/howto/postfix_smtp_auth_tls.html
*/


