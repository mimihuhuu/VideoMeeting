#include "CLogDialog.h"
#include <QMouseEvent>

CLogDialog::CLogDialog(QWidget *parent)
    : QDialog(parent)
{
    ui.setupUi(this);
	this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinMaxButtonsHint);

	ui.horizontalLayout->setContentsMargins(0, 0, 0, 0);

	ui.lineEdit_roomID->setText("1234");
	ui.widget_right->setStyleSheet("background-image:url(:/NiceMeeting/resources/login/right_image.png)");
	ui.widget_right->setFixedSize(540, 840);

	ui.btnMin->setText("");
	ui.btnMin->setFixedSize(32, 32);
	ui.btnMin->setStyleSheet(
		"QPushButton {"
		"	 background-image:url(:/NiceMeeting/resources/login/min.svg);"
		"    border: none;"
		"}"
		"QPushButton:hover {"
		"    background-image: url(:/NiceMeeting/resources/login/min_hover.svg);"
		"    background-color: rgba(99, 99, 99, 100);"
		"}"
	);
		
	ui.btnClose->setText("");
	ui.btnClose->setFixedSize(32, 32);
	ui.btnClose->setStyleSheet(
		"QPushButton {"
		"	 background-image:url(:/NiceMeeting/resources/login/close.svg);"
		"    border: none;"
		"}"
		"QPushButton:hover {"
		"    background-image: url(:/NiceMeeting/resources/login/close_hover.svg);"
		"    background-color: rgba(99, 99, 99, 100);"
		"}"
	);

	ui.BtnNetStatus->setText("");
	ui.BtnNetStatus->setFixedSize(32, 32);
	ui.BtnNetStatus->setFlat(true);
	ui.BtnNetStatus->setStyleSheet(
		"QPushButton {"
		"    border: none;"
		"    background: transparent;"
		"    background-image: url(:/NiceMeeting/resources/login/login_netstatus.svg);"
		"    background-repeat: no-repeat;"
		"    background-position: center;"
		"}"
	);

	ui.lineEdit_roomID->setFixedHeight(50);
	ui.lineEdit_pwd->setFixedHeight(50);
	ui.lineEdit_username->setFixedHeight(50);
	ui.btnJion->setFixedHeight(50);

	ui.BtnSetting->setText("");
	ui.BtnSetting->setFixedSize(32, 32);
	ui.BtnSetting->setStyleSheet(
		"QPushButton {"
		"    border: none;"
		"    background: transparent;"
		"    background-image: url(:/NiceMeeting/resources/login/login_set.svg);"
		"    background-repeat: no-repeat;"
		"    background-position: center;"
		"}"
		"QPushButton:hover {"
		"    background-image: url(:/NiceMeeting/resources/login/login_set_hover.svg);"
		"    background-color: rgba(99, 99, 99, 100);"
		"}"
	);

	ui.label_logo->setText("");
	ui.label_logo->setFixedSize(40, 40);
	ui.label_logo->setStyleSheet(
		"QLabel {"
		"    border: none;"
		"    background: transparent;"
		"    background-image: url(:/NiceMeeting/resources/login/logo.png);"
		"    background-repeat: no-repeat;"
		"    background-position: center;"
		"}"
	);

	ui.label_name->setText(u8"NICE MEETING");
	ui.label_name->setStyleSheet("font-size:22px");

}

CLogDialog::~CLogDialog()
{
    
}

QString CLogDialog::getRoomID() const
{

	return ui.lineEdit_roomID->text();
}

void CLogDialog::mousePressEvent(QMouseEvent* event)
{
	mouse_pos = event->globalPos();
	window_pos = this->pos();
	diff_pos = mouse_pos - window_pos;
}

void CLogDialog::mouseMoveEvent(QMouseEvent* event)
{
	QPoint pos = event->globalPos();
	this->move(pos - diff_pos);
}

void CLogDialog::on_btnJion_clicked()
{
	QString roomID = ui.lineEdit_roomID->text();
	if (roomID.isEmpty())
	{
		return;
	}
	accept();   
}
