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
#include <QDialog>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include "ui_formArama.h"

class FormArama:public QDialog, Ui::formArama
{
	Q_OBJECT
	public:
	QString searchMusteri;
	QString searchSiparis;
	
	FormArama(QWidget *parent):QDialog(parent)
	{
		setupUi(this);
		
		dateSiparis->setDate(QDate::currentDate());
		dateTeslim->setDate(QDate::currentDate());
		dateBasla->setDate(QDate::currentDate());
		dateBitis->setDate(QDate::currentDate());
		
		comboSektor->clear();
		QSqlDatabase db = QSqlDatabase::database();
		if (!db.isOpen()) db.open();
		
		QSqlQuery q;
		q.exec("SELECT * FROM sektor ORDER BY ad");
		while(q.next())
			comboSektor->addItem(q.value(1).toString(), q.value(0).toInt());
		
		connect(okButton, SIGNAL(clicked()), this, SLOT(slotAccept()));
		
	}
	
	public slots:
	/* iki tane search olayı var... 
	 * müşteri arama basit, ama siparişi ararken müşteriyi de eklememiz lazım
	 */
	void slotAccept()
	{
		if(!cbMusteri->isChecked() &&
			!cbSiparis->isChecked() &&
			!cbTarih->isChecked() &&
			!cbTarih2->isChecked())
		{
			QMessageBox::critical(this, "Hata!", QString::fromUtf8("Hiçbir arama alanı seçilmemiş!\nTekrar deneyin."));
			return;
		}
		
		if (dateBasla->date() > dateBitis->date())
		{
			QMessageBox::critical(this, "Hata!", 
			QString::fromUtf8("Başlangıç ve bitiş tarihlerini kontrol edin!\nBitiş tarihi başlangıç tarihinden önce olamaz."));
			return;
		}
		// siparis.id, ad, tur, tarih, (toplam-onodeme)
		// siparis.id, faturaNo, tur, ekler
		searchSiparis = "SELECT siparis.id, ad, tur, tarih, (toplam-onodeme) FROM siparis, baglanti LEFT JOIN  siparisListe ON siparisId=siparis.id WHERE"; 
		bool myFlag = false;
		
		if (cbMusteri->isChecked())
		{
			searchMusteri += "SELECT * FROM baglanti WHERE sektorId=" + comboSektor->itemData(comboSektor->currentIndex()).toString()
			+ " OR ad LIKE '%" + 
				lineMusteri->text() + "%' OR adres LIKE '%" + lineAdres->text() + "%' ORDER BY ad";
			
			searchSiparis += " (sektorId='" + comboSektor->itemData(comboSektor->currentIndex()).toString() + "' OR " 
			+ "ad LIKE '%" + lineMusteri->text() + "%' OR adres LIKE '%" + lineAdres->text() + "%')";
			myFlag = true;
		}
		
		if (cbSiparis->isChecked())
		{
			if (myFlag)
				searchSiparis += " AND ";
			searchSiparis += " (tur LIKE '%" + lineSiparis->text() + "%' OR faturaNo='" + lineFatura->text() + 
			"' OR ekler LIKE '%" + lineEk->text() + "%') ";
			myFlag = true;
		}
		
		
		if (cbTarih->isChecked())
		{
			if (myFlag) 
				searchSiparis += " AND ";
			searchSiparis += "(tarih='" + dateSiparis->date().toString("yyyy-MM-dd") + "' AND teslim='" + 
			dateTeslim->date().toString("yyyy-MM-dd") + "') ";
			myFlag = true;
		}
		
		if (cbTarih2->isChecked())
		{
			if (myFlag)
				searchSiparis += " AND ";
			searchSiparis += " (tarih > '" + dateBasla->date().toString("yyyy-MM-dd") + "' AND tarih < '" + 
			dateBitis->date().toString("yyyy-MM-dd") + "') ";
		}
		
		searchSiparis += " GROUP BY siparis.id";
		
		accept();
		
	}
	
	
};

