#include "CLogDialog.h"
#include "commons.h"
#include <QIcon>
#include <QMouseEvent>

CLogDialog::CLogDialog(QWidget *parent)
    : QDialog(parent)
{
    ui.setupUi(this);
	this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinMaxButtonsHint);
	setFixedSize(LOGIN_DIALOG_WIDTH, LOGIN_DIALOG_HEIGHT);

	ui.horizontalLayout->setContentsMargins(0, 0, 0, 0);
	ui.BigVideoWidget_Vlayout->setContentsMargins(0, 0, 0, 0);
	ui.BigVideoWidget_Vlayout->setSpacing(0);
	ui.BigVideoWidget_Top->setContentsMargins(0, 0, 0, 0);
	ui.BigVideoWidget_Top->setSpacing(0);
	ui.BigVideoWidget_Top->setAlignment(Qt::AlignTop);

	ui.lineEdit_roomID->setText("1234");
	ui.widget_right->setStyleSheet(
		"background-image: url(:/NiceMeeting/resources/login/right_image.png);"
		"background-repeat: no-repeat;"
		"background-position: center;"
	);
	ui.widget_right->resize(LOGIN_DIALOG_RIGHT_WIDTH, LOGIN_DIALOG_RIGHT_HEIGHT);

	ui.btnMin->setFlat(true);
	ui.btnMin->setText("");
	ui.btnMin->setIcon(QIcon());
	ui.btnMin->setFixedSize(46, 46);
	ui.btnMin->setStyleSheet(
		"QPushButton {"
		"    border: none;"
		"    background: transparent;"
		"    background-image: url(:/NiceMeeting/resources/login/min.svg);"
		"    background-repeat: no-repeat;"
		"    background-position: center;"
		"}"
		"QPushButton:hover {"
		"    background-image: url(:/NiceMeeting/resources/login/min_hover.svg);"
		"    background-repeat: no-repeat;"
		"    background-position: center;"
		"    background-color: rgba(99, 99, 99, 100);"
		"}"
	);

	ui.btnClose->setFlat(true);
	ui.btnClose->setText("");
	ui.btnClose->setIcon(QIcon());
	ui.btnClose->setFixedSize(46, 46);
	ui.btnClose->setStyleSheet(
		"QPushButton {"
		"    border: none;"
		"    background: transparent;"
		"    background-image: url(:/NiceMeeting/resources/login/close.svg);"
		"    background-repeat: no-repeat;"
		"    background-position: center;"
		"}"
		"QPushButton:hover {"
		"    background-image: url(:/NiceMeeting/resources/login/close_hover.svg);"
		"    background-repeat: no-repeat;"
		"    background-position: center;"
		"    background-color: rgba(99, 99, 99, 100);"
		"}"
	);

	ui.BtnNetStatus->setText("");
	ui.BtnNetStatus->resize(32, 32);
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

	ui.lineEdit_roomID->resize(ui.lineEdit_roomID->width(), 50);
	ui.lineEdit_pwd->resize(ui.lineEdit_pwd->width(), 50);
	ui.lineEdit_username->resize(ui.lineEdit_username->width(), 50);
	ui.btnJion->resize(ui.btnJion->width(), 50);

	ui.BtnSetting->setText("");
	ui.BtnSetting->resize(32, 32);
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
	ui.label_logo->resize(40, 40);
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
