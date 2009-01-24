#ifndef _TOOLS_H
#define _TOOLS_H

#include <QString>
#include <QTextStream>
#include <QDebug>
#include <QObject>
#include <QLatin1String>
#include <QTextCodec>
#include <QByteArray>
#include <QApplication>
#include <QFile>
#include <QDateTime>
#include <QDate>
#include <QStringList>
#include <QTextStream>
#include <QDebug>
#include <QMessageBox>
#include <QAbstractSocket>
#include <QDateTime>
#include <QDate>
#include <QLocale>



/* configs libs agree hawe a look on path lib_sqlite_qt4 */
#ifndef _PROGRAM_SHORT_NAME
#define _PROGRAM_SHORT_NAME "swisse_crm"
#endif
#ifndef WORK_CACHEDIR
#define WORK_CACHEDIR \
              QString( "%1/.%2/" ).arg( QDir::homePath() , _PROGRAM_SHORT_NAME )
#endif
#ifndef ROW_DB_CACHE
#define ROW_DB_CACHE \
              QString( "%1_db_row.dat" ).arg( WORK_CACHEDIR )
#endif
#ifndef SQLITE_CLASS_LOG
#define SQLITE_CLASS_LOG \
              QString( "_running_sqlclass.log" ).arg( WORK_CACHEDIR )
#endif
#ifndef QUERY_RUNNING_LOG
#define QUERY_RUNNING_LOG \
              QString( "%1_running_query.log" ).arg( WORK_CACHEDIR )
#endif
#ifndef BAD_QUERY_RUNNING_LOG
#define BAD_QUERY_RUNNING_LOG \
              QString( "%1_running_bad_query.log" ).arg( WORK_CACHEDIR )
#endif
/* configs libs agree hawe a look on path lib_sqlite_qt4 */


#include <iostream>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string>
#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <time.h>


using namespace std;
int dateswap(QString form, uint unixtime );
QString TimeStampMail();
QString fulltrim(QString cutestrings);
QString file_get_bin( QString filename );
QString encodeBase64( QString xml );
QString decodeBase64( QString xml );
QString TO_latin_EU( QString t );
QString XML_utf8( QString t );
QString traduce( QString t );
int get_nummer(QString incomming);
bool is_numeric(QString incomming);
QString  UnixTime2UserTime(int utimenr);
int  getUnixTime(int d , int m , int j);
QString qt2latin1( QString item );
QString file_get_clean_line(QString fullFileName);
bool file_put_contents(QString fullFileName,QString xml);
bool file_put_contents_utf8(QString fullFileName,QString xml);
bool file_put_contents_latin(QString fullFileName,QString xml);
QString file_get_line(QString fullFileName,int linenr);
char* file_get_contents_char(char *filename);
bool file_put_contents_char( char *filename , char *xml );
bool is_file(QString fullFileName);
bool qt_unlink(QString fullFileName);
bool file_put_contents_append(QString fullFileName,QString xml);
QString qt_unixtime(QString line);
char* double2char( double i_int );
char* int2char( int i_int );

QString chars2qt( char* xml );
char* qtchars( QString xml );
const char* qtchar( QString xml );
char* append( char *one , char *dwo );
QString UsersLocalCodec( QString xml );
#endif

/*
file_put_contents("conf.dat", QString( "%1/db_setting/" ).arg( QDir::homePath() ) );
*/
