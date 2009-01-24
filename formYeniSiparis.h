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
#include <QDate>
#include <QTreeWidgetItemIterator>
#include "ui_formYeniSiparis.h"

class FormYeniSiparis:public QDialog, Ui::formYeniSiparis
{
	Q_OBJECT
	public:
	bool editMode;
	bool isUpdate;
	bool changed;
	QString ssid;
	FormYeniSiparis(QWidget *parent, QString id=QString::null):QDialog(parent)
	{
		setupUi(this);
		ssid = id;
		editMode = false;
		isUpdate = false;
		changed = false;
		
		dateTeslim->setDate(QDate::currentDate());
		treeSiparis->setToolTip(QString::fromUtf8(
		"Sağdaki düğmeler ile sipariş ekleyebilir ve silebilirsiniz.\n"));
		
		comboBaglanti->clear();
		QSqlDatabase db = QSqlDatabase::database();
		if(!db.isOpen()) db.open();
		
		QSqlQuery q;
		q.exec("SELECT id, ad FROM baglanti ORDER BY ad");
		while(q.next())
			comboBaglanti->addItem(q.value(1).toString(), q.value(0).toInt());
		
		if (id != QString::null)
		{
			isUpdate = true;
			// dolduralım
			QSqlQuery r;
			r.exec("SELECT * FROM siparis WHERE id=" + id);
			r.next();
			QStringList a = r.value(5).toString().split("-");
			dateTeslim->setDate(QDate(a.at(0).toInt(),a.at(1).toInt(),a.at(2).toInt()));
			comboBaglanti->setCurrentIndex(comboBaglanti->findData(r.value(1)));
			lineFatura->setText(r.value(2).toString());
			lineOdeme->setText(r.value(3).toString());
			lineToplam->setText(r.value(4).toString());
			
			r.exec("SELECT * FROM siparisListe WHERE siparisId=" + id);
			while(r.next())
			{
				QTreeWidgetItem *it = new QTreeWidgetItem(treeSiparis);
				it->setText(0, r.value(2).toString());
				it->setText(1, r.value(3).toString());
				it->setText(2, r.value(4).toString());
				it->setText(3, r.value(5).toString());
				it->setToolTip(3, r.value(5).toString());
			}
			
		}
		
		
		connect(okButton, SIGNAL(clicked()), this, SLOT(slotKaydet()));
// 		connect(lineFiyat, SIGNAL(textEdited(const QString &)), this, SLOT(slotHesapla(const QString &)));
// 		connect(spinAdet, SIGNAL(valueChanged(const QString &)), this, SLOT(slotHesapla(const QString &)));
		connect(btnEkle, SIGNAL(clicked()), this, SLOT(slotEkle()));
		connect(btnSil,  SIGNAL(clicked()), this, SLOT(slotSil()));
		
	}
		
	void Hesapla()
	{
		double toplam = 0;
		QTreeWidgetItemIterator it(treeSiparis);
		while(*it)
		{
			QTreeWidgetItem* i = *it;
			// toplam += i->text(2).toDouble() * i->text(1).toDouble();
			toplam += i->text(2).toDouble();
			++it;
		}
		lineToplam->setText(QString::number(toplam));
	}
	
	public slots:
	
	void slotKaydet()
	{
		if (treeSiparis->topLevelItemCount() == 0)
		{
			QMessageBox::critical(this, "Hata!",
			QString::fromUtf8("Sipariş listesi boş!"));
			return;
		}
		QSqlDatabase db = QSqlDatabase::database();
		if(!db.isOpen()) db.open();
		
		QDate simdi = QDate::currentDate();
        QString tarih = simdi.toString("yyyy-MM-dd");
		
		QSqlQuery q;
		QString queryString;
		if (isUpdate)
		{
			q.prepare("UPDATE siparis SET baglantiId=?, faturaNo=?, onodeme=?, toplam=?, teslim=? where id=?");
			q.bindValue(0, comboBaglanti->itemData(comboBaglanti->currentIndex()).toString());
			q.bindValue(1, lineFatura->text());
			q.bindValue(2, lineOdeme->text());
			q.bindValue(3, lineToplam->text());
			q.bindValue(4, dateTeslim->date().toString("yyyy-MM-dd"));
			q.bindValue(5, ssid);
		}
		else
		{
			q.prepare("INSERT INTO siparis(baglantiId, faturaNo, onodeme, toplam, tarih, teslim) VALUES(?, ?, ?, ?, ?, ?)");
			q.bindValue(0, comboBaglanti->itemData(comboBaglanti->currentIndex()).toString());
			q.bindValue(1, lineFatura->text());
			q.bindValue(2, lineOdeme->text());
			q.bindValue(3, lineToplam->text());
			q.bindValue(4, tarih);
			q.bindValue(5, dateTeslim->date().toString("yyyy-MM-dd"));
		}
		q.exec();
		
		QString sid;
		
		if (isUpdate)
		{
			sid = ssid;
			if (changed)
				q.exec("DELETE FROM siparisListe WHERE siparisId=" + ssid);
		}
		else
		{
			q.exec("SELECT id FROM siparis ORDER BY id DESC LIMIT 1");
			q.next();
			sid = q.value(0).toString();
		}
		if (changed)
		{
			QTreeWidgetItemIterator it(treeSiparis);
			while(*it)
			{
				QTreeWidgetItem* i = *it;
				q.prepare("INSERT INTO siparisListe(siparisId, tur, adet, fiyat, ekler) VALUES(?, ?, ?, ?, ?)");
				q.bindValue(0, sid);
				q.bindValue(1, i->text(0));
				q.bindValue(2, i->text(1));
				q.bindValue(3, i->text(2));
				q.bindValue(4, i->text(3));
				q.exec();
				++it;
			}
		}
		accept();
	}

	
	void temizle()
	{
		lineTur->clear();
		spinAdet->setValue(1);
		lineFiyat->clear();
		textIstek->clear();
	}
	
	bool isNumeric(QString m)
	{
		for (int i=0;i<m.size(); i++)
		{
			if (!m.at(i).isDigit())
			{
				// sayı mı?? rakam cocum...
				return false;
			}
		}
		return true;
	}
	
	void slotEkle()
	{
		// check req fields.
		if (lineTur->text() == "" || lineTur->text() == QString::null)
		{
			QMessageBox::warning(this, "Hata!", 
			QString::fromUtf8("Lütfen sipariş \"Tür\"ünü giriniz."));
			return;
		}
		if (lineFiyat->text() == "" || lineFiyat->text() == QString::null)
		{
			QMessageBox::warning(this, "Hata!", 
			QString::fromUtf8("Bir \"Fiyat\" girmemişsiniz.\nÜcretsiz işler için '0' lira girebilirsiniz :)"));
			return;
		}
		if(!isNumeric(lineFiyat->text()))
		{
			QMessageBox::warning(this, "Hata!", 
			QString::fromUtf8("Fiyat alanına sadece rakam girebilirsiniz."));
			return;
		}
		
		changed = true;
		
		QTreeWidgetItem *it = new QTreeWidgetItem(treeSiparis);
		it->setText(0, lineTur->text());
		it->setText(1, QString::number(spinAdet->value()));
		it->setText(2, lineFiyat->text());
		it->setText(3, textIstek->toPlainText());
		it->setToolTip(3, textIstek->toPlainText());
		
		temizle();
		Hesapla();
	}
	
	void slotSil()
	{
		QTreeWidgetItem *it = treeSiparis->currentItem();
		if (it == 0)
		{
			QMessageBox::warning(this, "Hata!", 
			QString::fromUtf8("Önce sipariş seçiniz."));
			return;
		}
		delete it;
		Hesapla();
		changed = true;
	}
	
};
