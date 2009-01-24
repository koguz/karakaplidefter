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
#include <QDate>
#include "ui_formRapor.h"

class FormRapor:public QDialog, Ui::formRapor
{
	Q_OBJECT
	public:
	
	bool isGun;
	QString gun;
	QString ay;
	
	FormRapor(QWidget *parent):QDialog(parent)
	{
		setupUi(this);
		
		dateDay->setDate(QDate::currentDate());
		dateMonth->setDate(QDate::currentDate());
		
		connect(okButton, SIGNAL(clicked()), this, SLOT(slotKapa()));
	}	
	public slots:
	void slotKapa()
	{
		if (!radioDaily->isChecked() && !radioMonthly->isChecked())
		{
			QMessageBox::critical(this, "Hata!", QString::fromUtf8("Bu durumda rapor çıkaramam!"));
			return;
		}
		if (dateDay->isEnabled()) 
		{
			gun = dateDay->date().toString("yyyy-MM-dd");
			isGun = true;
		}
		if (dateMonth->isEnabled())
		{
			ay = dateMonth->date().toString("yyyy-MM");
			isGun = false;
		}
		accept();
	}
};


