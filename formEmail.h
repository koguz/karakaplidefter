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
#include <QSettings>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>
#include "ui_formEmail.h"
#include "smtp.h"

class FormEmail:public QDialog, Ui::formEmail
{
	Q_OBJECT
	public:
	QString ePosta, smtp;
	int port;
	QString smtpUser;
	QString smtpPass;
	
	
	FormEmail(QWidget *parent):QDialog(parent)
	{
		setupUi(this);
		refreshCombo();
		
		QSettings ayarlar("KuzeyKutbu.Org", "KaraKapliDefter");
		ePosta = ayarlar.value("genel/eposta").toString();
		smtp = ayarlar.value("eposta/smtp").toString();
		port = ayarlar.value("eposta/port", 25).toInt();
		
		smtpUser = ayarlar.value("eposta/kullanici").toString();
		smtpPass = QByteArray::fromBase64(ayarlar.value("eposta/parola").toByteArray()).data();
		
		connect(btnGonder, SIGNAL(clicked()), this, SLOT(sendEmail()));		
	}
	
	public slots:
	
	void sendEmail()
	{
		QStringList kimlere;
		
		if (!lineKime->text().isEmpty())
			kimlere = lineKime->text().split(",", QString::SkipEmptyParts);
		
		if (comboSektor->currentText() != QString::fromUtf8("Sektör Seçin"))
		{
			QString sid = comboSektor->itemData(comboSektor->currentIndex()).toString();
			QSqlDatabase db = QSqlDatabase::database();
			if(!db.isOpen()) db.open();
			
			QSqlQuery q;
			q.exec("SELECT mail FROM baglanti WHERE sektorId=" + sid);
			while(q.next())
			{
				kimlere << q.value(0).toString();
			}
		}
		
		qDebug() << kimlere;
		
		Smtp *newMail = new Smtp(smtp, port, smtpUser, smtpPass);
		newMail->Send(ePosta, kimlere, lineKonu->text(), metinGonder->toPlainText());
		if (newMail->isErr)
		{
			QMessageBox::critical(this, "Hata!", 
				QString::fromUtf8("Posta gönderirken hata oluştu"));
		}
		else accept();
	}
	
	void refreshCombo()
	{
		comboSektor->clear();
		QSqlDatabase db = QSqlDatabase::database();
		if (!db.isOpen()) db.open();
		
		QSqlQuery q;
		q.exec("SELECT * FROM sektor ORDER BY ad");
		comboSektor->addItem(QString::fromUtf8("Sektör Seçin"), 0);
		while(q.next())
			comboSektor->addItem(q.value(1).toString(), q.value(0).toInt());
	}
	
};


