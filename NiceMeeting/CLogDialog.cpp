#include "CLogDialog.h"
#include "commons.h"
#include <QIcon>
#include <QMouseEvent>
#include <QPixmap>

namespace {
const char* kTitleBarRes = ":/titlebar/resources/mainwidget/titlebar/";
}

CLogDialog::CLogDialog(QWidget *parent)
    : QDialog(parent)
{
    ui.setupUi(this);
	setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
	setFixedSize(LOGIN_DIALOG_WIDTH, LOGIN_DIALOG_HEIGHT);

	ui.horizontalLayout->setContentsMargins(0, 0, 0, 0);
	ui.horizontalLayout->setSpacing(0);
	ui.LeftList_Vlayout->setContentsMargins(9, 0, 9, 9);
	ui.LeftList_Vlayout->setSpacing(6);
	ui.verticalLayout_2->setContentsMargins(0, 0, 0, 0);
	ui.horizontalLayout_5->setContentsMargins(0, 0, 0, 0);
	ui.horizontalLayout_4->setSpacing(8);
	ui.BigVideoWidget_Vlayout->setContentsMargins(0, 0, 0, 0);
	ui.BigVideoWidget_Vlayout->setSpacing(0);
	ui.BigVideoWidget_Top->setContentsMargins(0, 0, 0, 0);
	ui.BigVideoWidget_Top->setSpacing(0);
	ui.BigVideoWidget_Top->setAlignment(Qt::AlignTop);

	ui.lineEdit_roomID->setText("1234");

	ui.widget_right->setMinimumWidth(LOGIN_DIALOG_RIGHT_WIDTH);
	ui.widget_right->setMaximumWidth(LOGIN_DIALOG_RIGHT_WIDTH);
	ui.widget_right->setStyleSheet(
		"background-image: url(:/NiceMeeting/resources/login/right_image.png);"
		"background-repeat: no-repeat;"
		"background-position: center;"
		"background-size: 100% 100%;"
	);

	ui.btnMin->setFlat(true);
	ui.btnMin->setText("");
	ui.btnMin->setIcon(QIcon());
	ui.btnMin->setFixedSize(46, 46);
	ui.btnMin->setCursor(Qt::PointingHandCursor);
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
	ui.btnClose->setCursor(Qt::PointingHandCursor);
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

	ui.lineEdit_roomID->resize(ui.lineEdit_roomID->width(), 50);
	ui.lineEdit_pwd->resize(ui.lineEdit_pwd->width(), 50);
	ui.lineEdit_username->resize(ui.lineEdit_username->width(), 50);
	ui.btnJion->resize(ui.btnJion->width(), 50);

	ui.BtnSetting->setFlat(true);
	ui.BtnSetting->setText("");
	ui.BtnSetting->setFixedSize(32, 32);
	ui.BtnSetting->setCursor(Qt::PointingHandCursor);
	ui.BtnSetting->setStyleSheet(
		"QPushButton {"
		"    border: none;"
		"    background: transparent;"
		"    background-image: url(:/NiceMeeting/resources/login/setting.svg);"
		"    background-repeat: no-repeat;"
		"    background-position: center;"
		"}"
		"QPushButton:hover {"
		"    background-image: url(:/NiceMeeting/resources/login/setting_hover.svg);"
		"    background-color: rgba(99, 99, 99, 100);"
		"}"
	);

	ui.label_logo->setText("");
	ui.label_logo->setFixedSize(TITLE_BAR_LOGO_SIZE, TITLE_BAR_LOGO_SIZE);
	ui.label_logo->setAlignment(Qt::AlignCenter);
	const QPixmap logoPixmap(QString(kTitleBarRes) + "logo.png");
	ui.label_logo->setPixmap(logoPixmap.scaled(TITLE_BAR_LOGO_SIZE, TITLE_BAR_LOGO_SIZE,
		Qt::KeepAspectRatio, Qt::SmoothTransformation));

	ui.label_name->setText(u8"NiceMeeting");
	ui.label_name->setStyleSheet(
		QStringLiteral("QLabel { font-family: Microsoft YaHei; font-size: %1px; color: #BDC8E2; }")
			.arg(TITLE_BAR_FONT_SIZE));

	connect(ui.btnMin, &QPushButton::clicked, this, &CLogDialog::on_btnMin_clicked);
	connect(ui.btnClose, &QPushButton::clicked, this, &CLogDialog::on_btnClose_clicked);
}

CLogDialog::~CLogDialog()
{
}

QString CLogDialog::getRoomID() const
{
	return ui.lineEdit_roomID->text();
}

bool CLogDialog::isCameraEnabled() const
{
	return ui.checkBox->isChecked();
}

bool CLogDialog::isMicEnabled() const
{
	return ui.checkBox_2->isChecked();
}

void CLogDialog::mousePressEvent(QMouseEvent* event)
{
	if (event->button() != Qt::LeftButton) {
		QDialog::mousePressEvent(event);
		return;
	}
	mouse_pos = event->globalPos();
	window_pos = this->pos();
	diff_pos = mouse_pos - window_pos;
	event->accept();
}

void CLogDialog::mouseMoveEvent(QMouseEvent* event)
{
	if (!(event->buttons() & Qt::LeftButton)) {
		QDialog::mouseMoveEvent(event);
		return;
	}
	move(event->globalPos() - diff_pos);
	event->accept();
}

void CLogDialog::on_btnJion_clicked()
{
	const QString roomID = ui.lineEdit_roomID->text();
	if (roomID.isEmpty()) {
		return;
	}
	accept();
}

void CLogDialog::on_btnMin_clicked()
{
	showMinimized();
}

void CLogDialog::on_btnClose_clicked()
{
	reject();
}
