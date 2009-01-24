#include "_tools.h"


int dateswap(QString form, uint unixtime )
{
     QDateTime fromunix;
     fromunix.setTime_t(unixtime); 
     QString numeric = fromunix.toString((const QString)form);
     bool ok; 
     return (int)numeric.toFloat(&ok);
}


QString TimeStampMail()
{
     /* mail rtf Date format! http://www.faqs.org/rfcs/rfc788.html */
     uint unixtime = (uint)time( NULL );
     QDateTime fromunix;
     fromunix.setTime_t(unixtime); 
     QStringList RTFdays = QStringList() << "giorno_NULL" << "Mon" << "Tue" << "Wed" << "Thu" << "Fri" << "Sat" << "Sun";
     QStringList RTFmonth = QStringList() << "mese_NULL" << "Jan" << "Feb" << "Mar" << "Apr" << "May" << "Jun" << "Jul" << "Aug" << "Sep" << "Oct" << "Nov" << "Dec";
     QDate timeroad(dateswap("yyyy",unixtime),dateswap("M",unixtime),dateswap("d",unixtime));
     /*qDebug() << "### RTFdays " << RTFdays.at(timeroad.dayOfWeek());
     qDebug() << "### RTFmonth " << RTFmonth.at(dateswap("M",unixtime));
     qDebug() << "### yyyy " << dateswap("yyyy",unixtime);
     qDebug() << "### M " << dateswap("M",unixtime);
     qDebug() << "### d " << dateswap("d",unixtime);*/
     QStringList rtfd_line;
     rtfd_line.clear();
     rtfd_line.append("Date: ");
     rtfd_line.append(RTFdays.at(timeroad.dayOfWeek()));
     rtfd_line.append(", ");
     rtfd_line.append(QString::number(dateswap("d",unixtime)));
     rtfd_line.append(" ");
     rtfd_line.append(RTFmonth.at(dateswap("M",unixtime)));
     rtfd_line.append(" ");
     rtfd_line.append(QString::number(dateswap("yyyy",unixtime)));
     rtfd_line.append(" ");
     rtfd_line.append(fromunix.toString("hh:mm:ss"));
     rtfd_line.append("");
     /*qDebug() << "### mail rtf Date format " << rtfd_line.join("");*/
return QString(rtfd_line.join(""));
}









QString fulltrim(QString cutestrings)
{
    QStringList list = cutestrings.split(" ");
    QString newpola = list.join("");
return newpola.trimmed();
}


/* return file contenets as qstring */
QString file_get_bin( QString filename )
{
    QString inside = "";  
    QFile file(filename); 
    if (file.exists()) {
                if (file.open(QFile::ReadOnly | QFile::Text)) {
                  inside =file.readAll();
                  file.close();
                }
    }
   return inside;
}




QString encodeBase64( QString xml )
{
    QByteArray text;
    text.append(xml);
    return text.toBase64();
}

QString decodeBase64( QString xml )
{
    QByteArray xcode("");;
    xcode.append(xml);
    QByteArray precode(QByteArray::fromBase64(xcode));
    QString notetxt = precode.data();
    return notetxt;
}


QString traduce( QString t )
{
  return t;
}

bool file_put_contents_latin(QString fullFileName,QString xml)
{
   QTextCodec *codecutf8 = QTextCodec::codecForMib(106);
   QTextCodec *codeclatin1 = QTextCodec::codecForMib(4);
   QString tmpfilework = fullFileName+".tmplocks";
   QByteArray data;
   QFile filetmp(tmpfilework);
   QFile endfile(fullFileName);
   bool ok;
    if(filetmp.open(QIODevice::WriteOnly)) {
            QTextStream outtmp(&filetmp);
            outtmp << xml; /* utf8 save default */
            filetmp.close(); 
        ok = true;
    }
   
    if (filetmp.open(QFile::ReadOnly | QFile::Text)) {
        data = filetmp.readAll();
        filetmp.remove();    /* close e remove */  
        ok = true;
    }
    
    
     if (ok && endfile.open(QFile::WriteOnly | QFile::Text)) {
        QTextStream in(&data);
        in.setAutoDetectUnicode(false);
        in.setCodec(codecutf8);  
        
        QTextStream outfinal(&endfile);
        outfinal.setCodec(codeclatin1);
        outfinal << in.readAll(); /* convert yes or no! */
        endfile.close(); 
         return true; 
     }
   
   
   
return false;    
}



bool file_put_contents_utf8(QString fullFileName,QString xml)
{
   QTextCodec *codecutf8 = QTextCodec::codecForMib(106);
   QTextCodec *codeclatin1 = QTextCodec::codecForMib(4);
   QString tmpfilework = fullFileName+".tmplocks";
   QByteArray data;
   QFile filetmp(tmpfilework);
   QFile endfile(fullFileName);
   bool ok;
    if(filetmp.open(QIODevice::WriteOnly)) {
            QTextStream outtmp(&filetmp);
            outtmp << xml; /* utf8 save default */
            filetmp.close(); 
        ok = true;
    }
   
    if (filetmp.open(QFile::ReadOnly | QFile::Text)) {
        data = filetmp.readAll();
        filetmp.remove();    /* close e remove */  
        ok = true;
    }
    
    
     if (ok && endfile.open(QFile::WriteOnly | QFile::Text)) {
        QTextStream in(&data);
        in.setAutoDetectUnicode(false);
        in.setCodec(codeclatin1);  
        
        QTextStream outfinal(&endfile);
        outfinal.setCodec(codecutf8);
        outfinal << in.readAll(); /* convert yes or no! */
        endfile.close(); 
         return true; 
     }
   
   
   
return false;    
}






QString XML_utf8( QString t )
{
  //the following checks are necessary before exporting
  //strings to XML. see http://hdf.ncsa.uiuc.edu/HDF5/XML/xml_escape_chars.html for details
    /*
  QByteArray ba = t.toUtf8();
  QString text = ba.data();
    */
    /*  __cpu_register__  to find and count on grep */
  QString text = t;
  
  text.replace("\"","&quot;");
  text.replace("'", "&apos;");
  text.replace("<", "&lt;");
  text.replace(">", "&gt;");
  text.replace("\n", "&#10;");
  text.replace("\r", "&#13;");
  text.replace("&", "&amp;");   /*  qt4 toUtf8 dont replace && */
  return text;
}

QString TO_latin_EU( QString t )
{
  //the following checks are necessary before exporting
  //strings to XML. see http://hdf.ncsa.uiuc.edu/HDF5/XML/xml_escape_chars.html for details
  /* QByteArray ba = t.toLatin1();*/
  QString text = t;
  
  text.replace("&quot;","\"");
  text.replace("&apos;", "'");
  text.replace("&lt;", "<");
  text.replace("&gt;", ">");
  text.replace("&#10;", "\n");
  text.replace("&#13;", "\r");
  text.replace("&amp;", "&");   /*  qt4 toUtf8 dont replace && */
  return text;
}






bool is_numeric(QString incomming)
{
    incomming.replace(QString(" "), QString("")); /* trimm */
    QString str2 = incomming;
    bool ok; 
    str2.toFloat(&ok); 
return ok;
}

int get_nummer(QString incomming)
{   int nummer = 0;
    incomming.replace(QString(" "), QString("")); /* trimm */
    QString str2 = incomming;
    bool ok; 
    str2.toFloat(&ok); 
    if (ok) {
    return str2.toInt();
    }
return nummer; 
}


QString  UnixTime2UserTime(int utimenr)
{
//#include <clocale>
//#include <ctime>
    
         /* time_t t = time( 0 );  */
         /* QString locale_user = QLocale::system().name(); */
         setlocale( LC_TIME, "" );
         size_t const buffer_size = 1024;
         char buffer[ buffer_size ];
         char const* format = "%A %d %B %Y %H:%M";
         time_t t = utimenr;  
         strftime( buffer, buffer_size,
                 format, localtime( &t ) );
         QString hack = QString( "%1" ).arg( buffer );
         return hack;
}


/*  user input date checks */
int  getUnixTime(int d , int m , int j)   /* day / mont / year */
{ 
    if (!d > 0 && !d < 32) {
     d =1;
    }
    if (!m > 0 && !m < 13) {
     m =1;
    }  
     if (!j > 1969 && !j < 2016) {  /* grep UPDATE_APPS */
     j =1970;
    }    
    QDateTime date_unix;
    QDate date(j,m,d);
    date_unix.setDate(date);
    return date_unix.toTime_t(); 
}




QString qt2latin1( QString item )
{
   QByteArray ba = item.toLatin1();
    QString hack = ba.data();
    return hack; 
}

const char* qtchar( QString xml )
{
    QByteArray ba = xml.toAscii();
    const char* hack = ba.data();
    return hack;
}

char* qtchars( QString xml )
{
    QByteArray ba = xml.toAscii();
    char* hack = ba.data();
    return hack;
}

QString chars2qt( char* xml )
{
    return QString( "%1" ).arg( xml );
}


QString UsersLocalCodec( QString xml )
{
    QByteArray ba = xml.toAscii();
    /* QTextCodec *codec = QTextCodec::codecForName("ISO 8859-1"); */
    return ba.data();
}




bool file_put_contents_char( char *filename , char *xml )
{
ofstream out(filename);
  if(!out) {
    return false;
   }
  out << xml;
  out.close();
  return true; 
} 

char* file_get_contents_char(char *filename)
{
    long length;
    int count;
    char *buf;
    FILE *f = fopen(filename, "rt");
    if (!f) {
        buf = "";
        return buf;
    }
    length = 0xffff;
    buf = (char*) malloc(length + 1);
    count = fread(buf, 1, length + 1, f);
    fclose(f);
    if (count > length) {
         buf = "file too large (64k limit)";
        return buf;   
    }
    buf[count] = 0;
    return buf;
}



char* int2char( int i_int )
{
    char *buffer = new char[32];
    sprintf(buffer, "%d", i_int);
    return buffer;
}

char* double2char( double i_int )
{
    char *buffer = new char[32];
    sprintf(buffer, "%f", i_int );
    return buffer;
}

QString qt_unixtime(QString line)
{
    return QString( "%1 <=logline=> %2" ).arg( int2char(  (int)time( NULL ) ) , line  );
} 

char* append( char *one , char *dwo )
{
    char *buffer = new char[255];
    sprintf (buffer, "%s%s", one , dwo);
    return buffer;
}


bool file_put_contents_append(QString fullFileName,QString xml)
{
    QString data = xml+"\n";
    QFile f( fullFileName );
	if ( f.open( QFile::Append | QFile::Text ) )
	{
		QTextStream sw( &f );
		sw << data;
		f.close();
		return true;
	}
	return false;
    
}


bool is_file(QString fullFileName)
{
    QFile f( fullFileName );
	if ( f.exists() ) {
    return true;
	} else {
	return false;
    }
}

bool qt_unlink(QString fullFileName)
{
    QFile f( fullFileName );
	if ( is_file( fullFileName ) ) {
       if (f.remove()) {
        return true;
       }
	}
return false;
}

bool file_put_contents(QString fullFileName,QString xml)
{
    QString data = xml;
    QFile f( fullFileName );
	if ( f.open( QFile::WriteOnly | QFile::Text ) )
	{
		QTextStream sw( &f );
		sw << data;
		f.close();
		return true;
	}
	return false;
    
}










QString file_get_line(QString fullFileName,int linenr)
{
    QString inside ="";
    QFile file(fullFileName);
    int countnr = 0;
    if (linenr > 0) {
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
    return inside;
    }
    
    QTextStream in(&file);
    
        while (!in.atEnd()) {   ////// eben nicht am ende
         ++countnr;
            if (countnr == linenr) {
                inside = in.readLine(0);
                if (inside.size() > 0) {
                return inside;  
                }
             break;
            }
        }
    file.close();
    }
return inside;
} 





QString file_get_clean_line(QString fullFileName)
{
    QString inside ="";
    QString alls ="";
    QFile file(fullFileName);
    int countnr = 0;
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
    return alls;
    }
    
    QTextStream in(&file);
    
        while (!in.atEnd()) {
         ++countnr;
         inside = in.readLine(0);  /* line by line (); */
            
            if (inside.size() > 1 && !inside.contains("();", Qt::CaseInsensitive) && !inside.startsWith("#", Qt::CaseInsensitive) && !inside.startsWith("<?", Qt::CaseInsensitive) ) {
                /* alls = alls+"\n" + QString( "Line %1 " ).arg( int2char( countnr ) ) + inside; */
                if (alls.size() > 0) {
                alls = alls+"\n"+ inside;
                } else {
                alls = inside;
                }
            }
        }
        
file.close();
return alls.replace(QString(",);"), QString(");"));
} 








