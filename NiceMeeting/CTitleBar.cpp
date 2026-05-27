#include "CTitleBar.h"
#include "commons.h"
#include <qt_windows.h>
#include <QHBoxLayout>
#include <QIcon>
#include <QString>

namespace {
const char* kTitleBarRes = ":/titlebar/resources/mainwidget/titlebar/";

QString titleImageButtonStyle(const char* normalIcon, const char* hoverIcon)
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
		.arg(TITLE_BAR_ICON_SIZE);
}

void setupTitleImageButton(QPushButton* btn, const char* normalIcon, const char* hoverIcon)
{
	btn->setFlat(true);
	btn->setText(QString());
	btn->setIcon(QIcon());
	btn->setFixedSize(TITLE_BAR_BUTTON_SIZE, TITLE_BAR_BUTTON_SIZE);
	btn->setStyleSheet(titleImageButtonStyle(normalIcon, hoverIcon));
}
} // namespace

CTitleBar::CTitleBar(QWidget* p)
{
	initUI();
}
 
CTitleBar::~CTitleBar()
{
}

void CTitleBar::initUI()
{
	setAttribute(Qt::WA_StyledBackground);
	setFixedHeight(TITLE_BAR_HEIGHT);
	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	setStyleSheet("background-color:rgb(54,54,54)");	

	m_pLogo = new QLabel(this);
	m_pLogo->setFixedSize(TITLE_BAR_LOGO_SIZE, TITLE_BAR_LOGO_SIZE);
	QPixmap pixmap(QString(kTitleBarRes) + "logo.png");
	QPixmap scaled = pixmap.scaled(TITLE_BAR_LOGO_SIZE, TITLE_BAR_LOGO_SIZE,
		Qt::KeepAspectRatio, Qt::SmoothTransformation);
	m_pLogo->setPixmap(scaled);
	m_pLogo->setAlignment(Qt::AlignCenter);

	m_pTitleTextLabel = new QLabel(this);
	m_pTitleTextLabel->setText(u8"NiceMeeting");
	m_pTitleTextLabel->setStyleSheet(
		QStringLiteral("QLabel { font-family: Microsoft YaHei; font-size: %1px; color: #BDC8E2; }")
			.arg(TITLE_BAR_FONT_SIZE));

	m_pMaxButton = new QPushButton(this);
	setupTitleImageButton(m_pMaxButton, "max.svg", "max_hover.svg");

	m_pMinButton = new QPushButton(this);
	setupTitleImageButton(m_pMinButton, "min.svg", "min_hover.svg");

	m_pCloseButton = new QPushButton(this);
	setupTitleImageButton(m_pCloseButton, "close.svg", "close_hover.svg");

	QHBoxLayout* pHlay = new QHBoxLayout(this);
	pHlay->setContentsMargins(TITLE_BAR_H_MARGIN, 0, 0, 0);
	pHlay->setSpacing(0);
	pHlay->setAlignment(Qt::AlignVCenter);
	pHlay->addWidget(m_pLogo, 0, Qt::AlignVCenter);
	pHlay->addSpacing(8);
	pHlay->addWidget(m_pTitleTextLabel, 0, Qt::AlignVCenter);
	pHlay->addStretch();
	pHlay->addWidget(m_pMaxButton, 0, Qt::AlignTop);
	pHlay->addWidget(m_pMinButton, 0, Qt::AlignTop);
	pHlay->addWidget(m_pCloseButton, 0, Qt::AlignTop);

	connect(m_pMinButton, &QPushButton::clicked, this, &CTitleBar::OnClicked);
	connect(m_pMaxButton, &QPushButton::clicked, this, &CTitleBar::OnClicked);
	connect(m_pCloseButton, &QPushButton::clicked, this, &CTitleBar::OnClicked);
}

void CTitleBar::mousePressEvent(QMouseEvent* event)
{
	Q_UNUSED(event);
	if (ReleaseCapture()) 
	{
		QWidget* pWindow = this->window();
		if (pWindow->isTopLevel())
		{
			SendMessage(HWND(pWindow->winId()), WM_SYSCOMMAND, SC_MOVE + HTCAPTION, 0);
		}
	}
}

void CTitleBar::mouseDoubleClickEvent(QMouseEvent* event)
{
	Q_UNUSED(event);
	m_pMaxButton->clicked();
}

void CTitleBar::OnClicked()
{
	QPushButton* pButton = qobject_cast<QPushButton*>(sender());

	QWidget* pWindow = this->window();

	if (pButton == m_pMinButton)
	{
		pWindow->showMinimized();
	}
	else if (pButton == m_pMaxButton)
	{
		if (pWindow->isMaximized())
		{
			pWindow->showNormal();
			m_pMaxButton->setStyleSheet(titleImageButtonStyle("max.svg", "max_hover.svg"));
		}
		else
		{
			pWindow->showMaximized();
			m_pMaxButton->setStyleSheet(titleImageButtonStyle("normal.svg", "normal_hover.svg"));
		}
	}
	else if (pButton == m_pCloseButton)
	{
		emit sig_close();
	}
}
