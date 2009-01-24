/*    
    Kara Kaplı Defter
    Copyright (C) 2006  Kaya Oğuz

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/
#include <QApplication>
#include <QSettings>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QDir>
#include <QFile>
#include "AnaPencere.h"

static bool createConnection(QString path, bool first)
{
	bool silindi = !QFile::exists(path);
	QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
	db.setDatabaseName(path);
	if (!db.open())
	{
		QMessageBox::critical(0, QString::fromUtf8("Veritabanı Hatası"),
		QString::fromUtf8("Lütfen bağlantılarınızı kontrol ediniz"));
		return false;
	}
	if (first || silindi)
	{
		QMessageBox::information(0, QString::fromUtf8("Bilgilendirme"), 
		QString::fromUtf8("Veritabanı yaratıldı:") + path + QString::fromUtf8("\nBu dosyayı silmeyin ve taşımayın, kayıtlarınız bu dosyada durmaktadır."));
		QSqlQuery q;
		q.exec("CREATE TABLE baglanti (id INTEGER PRIMARY KEY, ad TEXT, sektorId INTEGER, mail TEXT, telefon TEXT, adres TEXT, vergiNo TEXT, vergiDairesi TEXT)");
		q.exec("CREATE TABLE sektor (id INTEGER PRIMARY KEY, ad TEXT)");
		q.exec("CREATE TABLE siparis (id INTEGER PRIMARY KEY, baglantiId INTEGER, faturaNo TEXT, onodeme INTEGER, toplam INTEGER, tarih DATE, teslim DATE)");
		q.exec("CREATE TABLE siparisListe (id INTEGER PRIMARY KEY, siparisId INTEGER, tur TEXT, adet INTEGER, fiyat INTEGER, ekler TEXT)");
	}
	return true;
}

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	/*
	Veritabanı olayları
	
	Veritabanı yoksa, yarat, varsa açmaya çalış..
	QSettings kullan...
	*/
	QSettings ayarlar("KuzeyKutbu.Org", "KaraKapliDefter");
	QString dbPath = ayarlar.value("database/dbPath").toString();
	
	bool run = false;
	
	if (dbPath == QString::null)
	{
		dbPath = QDir::homePath() + "/defter.db";
		ayarlar.setValue("database/dbPath", dbPath);
		createConnection(dbPath, true);
	}
	
	if (!run) createConnection(dbPath, false);
	
	AnaPencere p;
	p.show();
	return a.exec();
}
