#include "MeetingFramelessDialog.h"
#include "commons.h"

#include <QHBoxLayout>
#include <QIcon>
#include <QLabel>
#include <QMouseEvent>
#include <QPushButton>
#include <QVBoxLayout>

namespace {
const char* kTitleBarRes = ":/titlebar/resources/mainwidget/titlebar/";

QString titleImageButtonStyle(const char* normalIcon, const char* hoverIcon, int iconSize)
{
	return QString(
		"QPushButton {"
		"    border: none;"
		"    background: transparent;"
		"    background-image: url(%1%2);"
		"    background-repeat: no-repeat;"
		"    background-position: center;"
		"    background-size: %4px %4px;"
		"}"
		"QPushButton:hover {"
		"    background-color: rgba(99, 99, 99, 100);"
		"    background-image: url(%1%3);"
		"    background-repeat: no-repeat;"
		"    background-position: center;"
		"    background-size: %4px %4px;"
		"}")
		.arg(kTitleBarRes)
		.arg(normalIcon)
		.arg(hoverIcon)
		.arg(iconSize);
}

void setupTitleImageButton(QPushButton* btn, const char* normalIcon, const char* hoverIcon,
	int buttonSize, int iconSize)
{
	btn->setFlat(true);
	btn->setText(QString());
	btn->setIcon(QIcon());
	btn->setFixedSize(buttonSize, buttonSize);
	btn->setStyleSheet(titleImageButtonStyle(normalIcon, hoverIcon, iconSize));
	btn->setCursor(Qt::PointingHandCursor);
}
}

MeetingFramelessDialog::MeetingFramelessDialog(const QString& title, QWidget* parent)
	: QDialog(parent)
{
	setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
	setAttribute(Qt::WA_StyledBackground, true);
	setStyleSheet(QStringLiteral("background-color: rgb(255, 255, 255);"));

	m_pTitleBar = new QWidget(this);
	m_pTitleBar->setFixedHeight(SHARE_SCREEN_DIALOG_TITLE_HEIGHT);
	m_pTitleBar->setAttribute(Qt::WA_StyledBackground, true);
	m_pTitleBar->setStyleSheet(QStringLiteral("background-color: rgb(54, 54, 54);"));

	m_pTitleLabel = new QLabel(title, m_pTitleBar);
	m_pTitleLabel->setStyleSheet(
		QStringLiteral("font-family: 'Microsoft YaHei'; font-size: %1px; color: #FFFFFF; background: transparent;")
			.arg(SHARE_SCREEN_DIALOG_TITLE_FONT_SIZE));

	auto* closeButton = new QPushButton(m_pTitleBar);
	setupTitleImageButton(closeButton, "close.svg", "close_hover.svg",
		SHARE_SCREEN_DIALOG_CLOSE_SIZE, SHARE_SCREEN_DIALOG_CLOSE_ICON_SIZE);
	connect(closeButton, &QPushButton::clicked, this, &QDialog::close);

	auto* titleLayout = new QHBoxLayout(m_pTitleBar);
	titleLayout->setContentsMargins(16, 0, 0, 0);
	titleLayout->setSpacing(0);
	titleLayout->addWidget(m_pTitleLabel, 1, Qt::AlignVCenter);
	titleLayout->addWidget(closeButton, 0, Qt::AlignVCenter);

	m_pBody = new QWidget(this);
	m_pBody->setAttribute(Qt::WA_StyledBackground, true);
	m_pBody->setStyleSheet(QStringLiteral("background-color: rgb(255, 255, 255);"));
	m_pBodyLayout = new QVBoxLayout(m_pBody);
	m_pBodyLayout->setContentsMargins(12, 12, 12, 12);
	m_pBodyLayout->setSpacing(10);

	auto* mainLayout = new QVBoxLayout(this);
	mainLayout->setContentsMargins(0, 0, 0, 0);
	mainLayout->setSpacing(0);
	mainLayout->addWidget(m_pTitleBar);
	mainLayout->addWidget(m_pBody, 1);
}

MeetingFramelessDialog::~MeetingFramelessDialog()
{
}

QVBoxLayout* MeetingFramelessDialog::bodyLayout() const
{
	return m_pBodyLayout;
}

void MeetingFramelessDialog::mousePressEvent(QMouseEvent* event)
{
	if (event->button() != Qt::LeftButton || !m_pTitleBar) {
		QDialog::mousePressEvent(event);
		return;
	}
	if (!m_pTitleBar->geometry().contains(event->pos())) {
		QDialog::mousePressEvent(event);
		return;
	}

	m_dragging = true;
	m_windowPos = pos();
	m_mousePos = event->globalPos();
	event->accept();
}

void MeetingFramelessDialog::mouseMoveEvent(QMouseEvent* event)
{
	if (!m_dragging) {
		QDialog::mouseMoveEvent(event);
		return;
	}
	move(m_windowPos + event->globalPos() - m_mousePos);
	event->accept();
}

void MeetingFramelessDialog::mouseReleaseEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton) {
		m_dragging = false;
	}
	QDialog::mouseReleaseEvent(event);
}
