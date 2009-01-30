/*    
    Kara Kaplı Defter
    Copyright (C) 2006 & 2009 Kaya Oğuz

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
#include <QMainWindow>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QTextDocument>
#include <QPrinter>
#include <QPrintDialog>
#include <QDate>
#include <QSettings>
#include "ui_anaPencere.h"
#include "formEmail.h"
#include "formYeniBaglanti.h"
#include "formYeniSiparis.h"
#include "formAyarlar.h"
#include "formRapor.h"
#include "formArama.h"
#include "formYardim.h"

class AnaPencere:public QMainWindow, Ui::MainWindow
{
	Q_OBJECT
	public:
	QColor red;
	QColor beyaz;
			
	AnaPencere():QMainWindow()
	{
		setupUi(this);
				
		red.setNamedColor("#990000");
		beyaz.setNamedColor("#FFFFFF");

		/*
		QAction *actionEdit;
		QAction *actionDel;
		QAction *actionArama;
    	*/
		
		connect(actionAbout, SIGNAL(activated()), this, SLOT(slotAbout()));
		connect(actionEmail, SIGNAL(activated()), this, SLOT(slotEmail()));
		connect(actionNewContact, SIGNAL(activated()), this, SLOT(slotNewContact()));
		connect(actionNewOrder, SIGNAL(activated()), this, SLOT(slotNewOrder()));
		connect(actionPrint, SIGNAL(activated()), this, SLOT(slotPrint()));
		connect(actionQuit, SIGNAL(activated()), this, SLOT(close()));
		connect(actionAyarlar, SIGNAL(activated()), this, SLOT(slotAyarlar()));
		connect(actionDel, SIGNAL(activated()), this, SLOT(slotCancel()));
		connect(actionEdit, SIGNAL(activated()), this, SLOT(slotEdit()));
		connect(actionArama, SIGNAL(activated()), this, SLOT(slotArama()));
		connect(actionHelp, SIGNAL(activated()), this, SLOT(slotYardim()));
		
		connect(buttonClear, SIGNAL(clicked()), this, SLOT(slotResetSearch()));
		connect(lineAra, SIGNAL(returnPressed()), this, SLOT(slotAra()));
		connect(treeSonuclar, SIGNAL(itemClicked(QTreeWidgetItem *, int)), this, SLOT(slotDetay(QTreeWidgetItem *, int)));
		connect(actionDaily, SIGNAL(activated()), this, SLOT(slotDaily()));
	}
	
	public slots:
		
	void slotYardim()
	{
		FormYardim *y = new FormYardim(this);
		y->exec();
	}
	
	void slotArama()
	{
		FormArama *a = new FormArama(this);
		if (a->exec() == QDialog::Accepted)
		{
			treeSonuclar->clear();
			QSqlDatabase db = QSqlDatabase::database();
			if(!db.isOpen()) db.open();
			
			if (!a->searchMusteri.isEmpty())
			{
				QSqlQuery q;
				q.exec(a->searchMusteri);
				while (q.next())
				{
					QTreeWidgetItem *i = new QTreeWidgetItem(treeSonuclar);
					i->setText(0, QString::fromUtf8("Müşteriler"));
					i->setText(1, q.value(1).toString());
					i->setStatusTip(0, q.value(0).toString());
				}
			}
			if (!a->searchSiparis.isEmpty())
			{
				QSqlQuery q2;
				q2.exec(a->searchSiparis);
				while(q2.next())
				{
					QTreeWidgetItem *i = new QTreeWidgetItem(treeSonuclar);
					i->setText(0, QString::fromUtf8("Siparişler"));
					QString aciklama;
					aciklama = q2.value(1).toString() + 
						" - " + q2.value(2).toString() + " - S.No:" + 
						q2.value(0).toString() + " - " + q2.value(3).toString();
					i->setText(1, aciklama);
					if (q2.value(4).toInt() == 0)
					{
						i->setBackgroundColor(0, red);
						i->setBackgroundColor(1, red);
						i->setTextColor(1, beyaz);
						i->setTextColor(0, beyaz);
					}
					i->setStatusTip(0, q2.value(0).toString());
				}
			}
		}
	}
	
	void slotEdit()
	{
		QTreeWidgetItem *item = treeSonuclar->currentItem();
		if (item == 0)
			return;
		if (item->text(0) == QString::fromUtf8("Müşteriler"))
		{
			FormYeniBaglanti *f = new FormYeniBaglanti(this, item->statusTip(0));
			f->exec();
			return;
		}
		if (item->text(0) == QString::fromUtf8("Siparişler"))
		{
			FormYeniSiparis *s = new FormYeniSiparis(this, item->statusTip(0));
			s->exec();
		}
	}
	
	void slotCancel()
	{
		QTreeWidgetItem *item = treeSonuclar->currentItem();
		if (item == 0 || item->text(0) != QString::fromUtf8("Siparişler")) 
			return;
		
		if (
			QMessageBox::question(this, "Emin misiniz?", 
			QString::fromUtf8("Siparişi iptal etmek üzeresiniz.\n"
			"Devam edelim mi?"),
			QMessageBox::Yes,
			QMessageBox::No | QMessageBox::Default,
			QMessageBox::NoButton) == QMessageBox::Yes)
		{
			// aaa bu siparişin bütün hepsini silip, 1 tane eklemek lazim
			QString id = item->statusTip(0);
			QSqlDatabase db = QSqlDatabase::database();
			if (!db.isOpen()) db.open();
			
			QSqlQuery q;
			q.exec("DELETE FROM siparisListe WHERE siparisId=" + id);
			q.exec("INSERT INTO siparisListe(siparisId, tur) VALUES(" + id + ", '" + 
			QString::fromUtf8("SİPARİŞ İPTAL EDİLDİ!") + "')");
			QMessageBox::information(this, "Bilgi", QString::fromUtf8("Sipariş iptal edildi!"));
		}
		
	}
	
	void slotDaily()
	{
		FormRapor *rap = new FormRapor(this);
		if (rap->exec() == QDialog::Accepted)
		{
			QSqlDatabase db = QSqlDatabase::database();
			if(!db.isOpen()) db.open();
			
			QString tarih;
			if (rap->isGun)
			{
				tarih = "tarih='" + rap->gun + "'";
			}
			else
			{
				tarih = "tarih like '" + rap->ay + "%'";
			}
			
			QSqlQuery q;
			q.exec("SELECT siparis.id, ad, tur, tarih, (toplam-onodeme) FROM siparis left join baglanti on baglanti.id=baglantiId left join siparisListe on siparisId=siparis.id where " + tarih + " group by siparis.id ORDER BY tarih");
			treeSonuclar->clear();
			while(q.next())
			{
				QTreeWidgetItem *i = new QTreeWidgetItem(treeSonuclar);
				i->setText(0, QString::fromUtf8("Siparişler"));
				i->setText(1, q.value(1).toString() + " - " + q.value(2).toString() + " - S.No:" + q.value(0).toString() + " - " + q.value(3).toString());
				i->setStatusTip(0, q.value(0).toString());
				if (q.value(4).toInt() == 0)
				{
					i->setBackgroundColor(0, red);
					i->setBackgroundColor(1, red);
					i->setTextColor(1, beyaz);
					i->setTextColor(0, beyaz);
				}
			}	
		}
		/*
		
		lineAra->clear();
		*/
	}
	
	void slotAyarlar()
	{
		FormAyarlar *form = new FormAyarlar(this);
		form->exec();
	}
	
	void slotPrint()
	{
		QSettings ayarlar("KuzeyKutbu.Org", "KaraKapliDefter");
		QPrinter::PageSize printerSayfa;
		QString ayar = ayarlar.value("printer/sayfa", "A4").toString();
		if (ayar == "A3")
			printerSayfa = QPrinter::A3;
		else if (ayar == "A4")
			printerSayfa = QPrinter::A4;
		else if (ayar == "A5")
			printerSayfa = QPrinter::A5;
		else 
			printerSayfa = QPrinter::A4;
		QTextDocument *doc = textDetay->document();
		QPrinter printer(QPrinter::HighResolution);
		
		QPrintDialog *dialog = new QPrintDialog(&printer, this);
		dialog->setWindowTitle(QString::fromUtf8("Yazdır"));
		if(dialog->exec() ==QDialog::Accepted)
		{
			printer.setFullPage(true);
			printer.setPageSize(printerSayfa);
			doc->print(&printer);
		}
	}
	
	
	void slotDetay(QTreeWidgetItem *item, int col, QString sid="0")
	{
		QSqlDatabase db = QSqlDatabase::database();
		if (!db.isOpen()) db.open();
		
		QString hangisi;
		QString id;
		
		/* Bu fonksiyon en son girilen siparişte de lazım olacak... */
		if (sid == "0")
		{
			id = item->statusTip(0);
			hangisi = item->text(0);
		}
		else 
		{
			hangisi = QString::fromUtf8("Siparişler");
			id = sid;
		}
		
		if (hangisi == QString::fromUtf8("Müşteriler"))
		{
			QSqlQuery q("SELECT baglanti.ad, sektor.ad, mail, telefon, adres, vergiNo, vergiDairesi FROM baglanti, sektor WHERE baglanti.id=" + id + " AND baglanti.sektorId=sektor.id");
			q.next();
			textDetay->setHtml(
			QString::fromUtf8("<b>Müşteri Bilgileri</b><hr>") + 
			"<table width='100%'>" + 
			QString::fromUtf8("<tr><td><b>Adı Soyadı:</b></td><td>") + q.value(0).toString() + "</td></tr>" +
			QString::fromUtf8("<tr><td><b>Sektör</b></td><td>") + q.value(1).toString() + "</td></tr>" + 
			QString::fromUtf8("<tr><td><b>E-Posta</b></td><td>") + q.value(2).toString() + "</td></tr>" + 
			QString::fromUtf8("<tr><td><b>Telefon</b></td><td>") + q.value(3).toString() + "</td></tr>" + 
			QString::fromUtf8("<tr><td><b>Adres</b></td><td>") + q.value(4).toString().replace("\n", "<br>", Qt::CaseInsensitive) + "</td></tr>" + 
			QString::fromUtf8("<tr><td><b>Vergi No</b></td><td>") + q.value(5).toString() + "</td></tr>" + 
			QString::fromUtf8("<tr><td><b>Vergi Dairesi</b></td><td>") + q.value(6).toString() + "</td></tr>" + 
			"</table>"
			);	
		}
		else if(hangisi == QString::fromUtf8("Siparişler"))
		{
			QSqlQuery q("select siparis.id, ad, faturaNo, toplam, onodeme, (toplam-onodeme), tarih, teslim, tur, adet, ekler, fiyat, tedarik from siparis left join baglanti on baglanti.id=baglantiId left join siparisListe on siparisId=siparis.id where siparis.id=" + id);
			bool firstRun = true;
			QString yaz = "";
			while(q.next())
			{
				if (firstRun) { yaz = 
				QString::fromUtf8("<b>Sipariş Bilgileri</b><hr>") + 
				"<table width='100%'>" + 
				QString::fromUtf8("<tr><td><b>Sipariş Tarihi:</b></td><td>") + q.value(6).toString() + "</td></tr>" +
				QString::fromUtf8("<tr><td><b>Teslim Tarihi:</b></td><td>") + q.value(7).toString() + "</td></tr>" +
				QString::fromUtf8("<tr><td><b>Fatura No:</b></td><td>") + q.value(2).toString() + "</td></tr>" +
				QString::fromUtf8("<tr><td><b>Sipariş No:</b></td><td>") + q.value(0).toString() + "</td></tr>" +
				QString::fromUtf8("<tr><td><b>Adı Soyadı:</b></td><td>") + q.value(1).toString() + "</td></tr>" + 
				QString::fromUtf8("<tr><td><b>Ödenen</b></td><td>") + q.value(4).toString() + "</td></tr>" + 
				QString::fromUtf8("<tr><td><b>Kalan</b></td><td>") + q.value(5).toString() + "</td></tr>" + 
				QString::fromUtf8("<tr><td><b>Toplam</b></td><td>") + q.value(3).toString() + "</td></tr>" + 
				"<tr><td colspan='2'><hr noshade size='1'></td></td></tr>" + 
				"<tr><td colspan='2'><table width='100%' cellspacing='2' cellpadding='6' border='1'>" + 
				"<tr><td colspan='5'><b>" + QString::fromUtf8("Sipariş Listesi") + "</b></td></tr>" +
				"<tr><td><b>" + QString::fromUtf8("Tür") + "</b></td>" + 
				"<td><b>Adet</b></td><td><b>Fiyat</b></td><td><b>Tedarik F.</b></td><td><b>" + QString::fromUtf8("Ek İstekler") + 
				"</b></td></tr>";
				firstRun = false;
				} 
				yaz += 
				QString::fromUtf8("<tr><td>") + q.value(8).toString() + "</td>" + 
				QString::fromUtf8("<td>") + q.value(9).toString() + "</td>" + 
				QString::fromUtf8("<td>") + q.value(11).toString() + "</td>" + 
				QString::fromUtf8("<td>") + q.value(12).toString() + "</td>" + 
				QString::fromUtf8("<td>") + q.value(10).toString() + "</td></tr>";
				
			}
			yaz += "</table></td></tr></table>";
			textDetay->setHtml(yaz);
		}
	}
	
	void slotAra()
	{
		QSqlDatabase db = QSqlDatabase::database();
		if(!db.isOpen()) db.open();
		
		QString aranan = lineAra->text();
		
		QSqlQuery q;
		QString query;
		
		if (comboAra->currentText() == QString::fromUtf8("Müşteriler"))
			query = "SELECT id, ad FROM baglanti WHERE ad LIKE '%" + aranan + "%' OR mail LIKE '%" + aranan + "%' OR adres LIKE '%" + aranan + "%' ORDER BY ad";
		else if (comboAra->currentText() == QString::fromUtf8("Siparişler"))
		{
			// query = "select siparis.id, ad, faturaNo, toplam, onodeme, (toplam-onodeme), tur, adet, ekler from siparis left join baglanti on baglanti.id=baglantiId left join siparisListe on siparisId=siparis.id group by siparis.id; where ad LIKE '%" + aranan + "%' OR tur LIKE '%" + aranan + "%' OR ekler LIKE '%" + aranan + "%'";
			query = "select siparis.id, ad, tur, tarih, (toplam-onodeme) from siparis left join baglanti on baglanti.id=baglantiId left join siparisListe on siparisId=siparis.id where ad LIKE '%" + aranan + "%' OR tur LIKE '%" + aranan + "%' OR ekler LIKE '%" + aranan + "%' GROUP BY siparis.id";
		}
		else
		{
			QMessageBox::critical(this, "Hata!", QString::fromUtf8("Buraya hiç gelmemeliydi...\nNeler oluyor?\nİşlem iptal edilecek..."));
			return;
		}
		
		q.exec(query);
		treeSonuclar->clear();
		bool fit;
		while(q.next())
		{
			QTreeWidgetItem *i = new QTreeWidgetItem(treeSonuclar);
			fit = false;
			i->setText(0, comboAra->currentText());
			QString aciklama;
			if (comboAra->currentText() == QString::fromUtf8("Siparişler"))
			{
				aciklama = q.value(1).toString() + " - " + q.value(2).toString() + " - S.No:" + q.value(0).toString() + " - " + q.value(3).toString();
				if (q.value(4).toInt() == 0) fit = true;
			}
			else if (comboAra->currentText() == QString::fromUtf8("Müşteriler"))
				aciklama = q.value(1).toString();
			i->setText(1, aciklama);
			i->setStatusTip(0, q.value(0).toString());
			if (fit) 
			{
				i->setBackgroundColor(0, red);
				i->setBackgroundColor(1, red);
				i->setTextColor(1, beyaz);
				i->setTextColor(0, beyaz);
			}
		}
		lineAra->clear();
	}
	
	void slotResetSearch()
	{
		lineAra->clear();
		treeSonuclar->clear();
	}
	
	void slotNewOrder()
	{
		QSqlDatabase db = QSqlDatabase::database();
		if(!db.isOpen()) db.open();
		
		QSqlQuery q;
		q.exec("SELECT id, ad FROM baglanti ORDER BY ad");
		int toplam=0; // sqlite does not support "number of rows returned" - peh!
		while(q.next()) toplam++;
		
		if (toplam == 0)
		{
			QMessageBox::warning(this, QString::fromUtf8("Uyarı!"), 
			QString::fromUtf8("Şu an kayıtlı bir müşteri yok!\nÖnce müşteri " 
			"kaydetmeniz önerilir!"));
			return;
		}
	
		FormYeniSiparis *order = new FormYeniSiparis(this);
		if(order->exec() == QDialog::Accepted)
		{
			// burada bu yeni siparişi detay ekranında göstermeli
			q.exec("SELECT id FROM siparis ORDER BY id DESC LIMIT 1");
			q.next();
			slotDetay(0, 0, q.value(0).toString());
		}
	}
	
	void slotNewContact()
	{
		FormYeniBaglanti *yeni = new FormYeniBaglanti(this);
		if(yeni->exec() == QDialog::Accepted)
		{
			// refresh lists
			
		}
	}
	
	void slotAbout()
	{
		QMessageBox::about(this, QString::fromUtf8("Kara Kaplı Defter Hakkında"), 
		QString::fromUtf8("Bir sipariş takip programı\nSürüm: 0.6\n\nKaya Oğuz (c) 2006 & 2009\nhttp://www.kuzeykutbu.org"));
	}
	
	void slotEmail()
	{
		FormEmail *f = new FormEmail(this);
		f->exec();
	}
};


