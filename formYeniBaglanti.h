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
#include "ui_formYeniBaglanti.h"

class FormYeniBaglanti:public QDialog, Ui::formYeniBaglanti
{
	Q_OBJECT
	public:
	// QTcpSocket *tcpSocket;
	// quint16 blockSize;
	bool isUpdate;
	int sektorId;
	QString updateId;
	FormYeniBaglanti(QWidget *parent, QString id=QString::null):QDialog(parent)
	{
		setupUi(this);
		isUpdate = false;
		sektorId = 0;
		
		refreshCombo();
		
		if (id != QString::null)
		{
			// fill in the blanks
			isUpdate = true;
			
			updateId = id;
			
			QSqlDatabase db = QSqlDatabase::database();
			if(!db.isOpen()) db.open();
			
			QSqlQuery q;
			q.exec("SELECT * FROM baglanti WHERE id=" + id);
			q.next();
			
			lineBaglanti->setText(q.value(1).toString());
			lineEposta->setText(q.value(3).toString());
			lineTelefon->setText(q.value(4).toString());
			textAdres->setPlainText(q.value(5).toString());
			lineVergiNo->setText(q.value(6).toString());
			lineVergiDairesi->setText(q.value(7).toString());
			comboSektor->setCurrentIndex(comboSektor->findData(q.value(2)));
		}
		
		connect(okButton, SIGNAL(clicked()), this, SLOT(slotKaydet()));
		connect(buttonEkle, SIGNAL(clicked()), this, SLOT(slotEkle()));
		connect(lineSektor, SIGNAL(returnPressed()), this, SLOT(slotEkle()));
		
	}
	
	void refreshCombo()
	{
		comboSektor->clear();
		QSqlDatabase db = QSqlDatabase::database();
		if (!db.isOpen()) db.open();
		
		QSqlQuery q;
		q.exec("SELECT * FROM sektor ORDER BY ad");
		while(q.next())
			comboSektor->addItem(q.value(1).toString(), q.value(0).toInt());
	}
	
	public slots:
	void slotKaydet()
	{
		if (comboSektor->count() == 0)
		{
			QMessageBox::warning(this, "Dikkat!", QString::fromUtf8("Bir sektör belirtmemişsiniz.\n"
			"Bir sektör adı girip \"Ekle\" düğmesine basın ve \n"
			"tekrar deneyin."));
			return;
		}
		if (lineBaglanti->text() == QString::null || lineBaglanti->text() == "")
		{
			QMessageBox::critical(this, "Hata!", QString::fromUtf8("En azından bağlantı adını girmelisiniz!"));
			return;
		}
		QSqlDatabase db = QSqlDatabase::database();
		if (!db.isOpen()) db.open();
		
		if (isUpdate)
		{
			QSqlQuery q;
			q.prepare("UPDATE baglanti SET ad=?, sektorId=?, mail=?, telefon=?, adres=?, vergiNo=?, vergiDairesi=? WHERE id=?");
			q.bindValue(0, lineBaglanti->text());
			q.bindValue(1, comboSektor->itemData(comboSektor->currentIndex()).toString());
			q.bindValue(2, lineEposta->text());
			q.bindValue(3, lineTelefon->text());
			q.bindValue(4, textAdres->toPlainText());
			q.bindValue(5, lineVergiNo->text());
			q.bindValue(6, lineVergiDairesi->text());
			q.bindValue(7, updateId);
/* 			QString sqlString = "UPDATE baglanti SET ad='" + lineBaglanti->text() + "', sektorId='" + 
 * 			comboSektor->itemData(comboSektor->currentIndex()).toString() + "', mail='" + lineEposta->text() + 
 * 			"', telefon='" + lineTelefon->text() + "', adres='" + textAdres->toPlainText() + 
 * 			"', vergiNo='" + lineVergiNo->text() + "', vergiDairesi='" + lineVergiDairesi->text() + 
 * 			"' WHERE id=" + updateId;
 * 			q.exec(sqlString);
 */
			q.exec();
		}
		else
		{
			QSqlQuery q;
			q.prepare("INSERT INTO baglanti(ad, sektorId, mail, telefon, adres, vergiNo, vergiDairesi) VALUES(?,?,?,?,?,?,?)");
			q.bindValue(0, lineBaglanti->text());
			q.bindValue(1, comboSektor->itemData(comboSektor->currentIndex()).toString());
			q.bindValue(2, lineEposta->text());
			q.bindValue(3, lineTelefon->text());
			q.bindValue(4, textAdres->toPlainText());
			q.bindValue(5, lineVergiNo->text());
			q.bindValue(6, lineVergiDairesi->text());
/* 			QString sqlString = "INSERT INTO baglanti(ad, sektorId, mail, telefon, adres, vergiNo, vergiDairesi) VALUES('" 
 * 			+ lineBaglanti->text() + "', '"  + comboSektor->itemData(comboSektor->currentIndex()).toString() + "', '"
 * 			+ lineEposta->text() + "', '" + lineTelefon->text() + "', '" + textAdres->toPlainText() + "', '" 
 * 			+ lineVergiNo->text() + "', '" + lineVergiDairesi->text() + "')";
 * 			q.exec(sqlString);
 */
			q.exec();
		}
		accept();
	}
	
	void slotEkle()
	{
		if (lineSektor->text() == QString::null || lineSektor->text() == "")
		{
			QMessageBox::critical(this, "Hata!", QString::fromUtf8("Lütfen bir sektör adı giriniz"));
			return;
		}
		QSqlDatabase db = QSqlDatabase::database();
		if (!db.isOpen()) db.open();
		
		QSqlQuery q;
		q.prepare("INSERT INTO sektor(ad) VALUES(?)");
		q.bindValue(0, lineSektor->text());
		q.exec();
		refreshCombo();
		lineSektor->clear();
	}
	
};


