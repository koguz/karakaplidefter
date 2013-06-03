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
#include "ui_formAyarlar.h"

// 28.06.2006 - bostanlı - üçkuyular feribotu :)

class FormAyarlar:public QDialog, Ui::formAyarlar
{
	Q_OBJECT
	public:
	FormAyarlar(QWidget *parent):QDialog(parent)
	{
		setupUi(this);
		
		// fill in the blanks
		QSettings ayarlar("KuzeyKutbu.Org", "KaraKapliDefter");
		lineSirket->setText(ayarlar.value("genel/sirket").toString());
		lineAdres->setText(ayarlar.value("genel/adres").toString());
		lineTelefon->setText(ayarlar.value("genel/telefon").toString());
		lineUrl->setText(ayarlar.value("genel/url").toString());
		linePosta->setText(ayarlar.value("genel/eposta").toString());
		comboPrinter->setCurrentIndex(comboPrinter->findText(ayarlar.value("printer/sayfa").toString(), Qt::MatchExactly));		
		connect(okButton, SIGNAL(clicked()), this, SLOT(slotKaydet()));
	}
	
	public slots:
	void slotKaydet()
	{
		QSettings ayarlar("KuzeyKutbu.Org", "KaraKapliDefter");
		ayarlar.setValue("genel/sirket", lineSirket->text());
		ayarlar.setValue("genel/adres", lineAdres->text());
		ayarlar.setValue("genel/telefon", lineTelefon->text());
		ayarlar.setValue("genel/url", lineUrl->text());
		ayarlar.setValue("genel/eposta", linePosta->text());
		ayarlar.setValue("printer/sayfa", comboPrinter->currentText());
		accept();
	}
	
	
	
};

