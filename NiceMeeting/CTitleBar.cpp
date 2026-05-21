#include "CTitleBar.h"
#include <qt_windows.h>
#include <QHBoxLayout>
#include <QVBoxLayout>

namespace {
const char* kTitleBarRes = ":/titlebar/resources/mainwidget/titlebar/";
const int kTitleIconPx = 20;   // Í¼±êÏÔÊ¾³ß´ç£»°´Å¥ÈÔ 32¡Á32 ·½±ãµã»÷
const char* kTitleBtnStyle =
	"QPushButton { border: none; background: transparent; }"
	"QPushButton:hover { background-color: rgba(99, 99, 99, 100); }";

void setupTitleButton(QPushButton* btn, const char* iconFile)
{
	btn->setFlat(true);
	btn->setFixedSize(32, 32);
	btn->setIcon(QIcon(QString(kTitleBarRes) + iconFile));
	btn->setIconSize(QSize(kTitleIconPx, kTitleIconPx));
	btn->setStyleSheet(kTitleBtnStyle);
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
	this->setFixedHeight(32 + 15 * 2);
	this->setStyleSheet("background-color:rgb(54,54,54)");	

	m_pLogo = new QLabel(this);
	m_pLogo->setFixedSize(32, 32);
	QPixmap pixmap(QString(kTitleBarRes) + "logo.png");
	QPixmap scaled = pixmap.scaled(kTitleIconPx, kTitleIconPx, Qt::KeepAspectRatio, Qt::SmoothTransformation);
	m_pLogo->setPixmap(scaled);
	m_pLogo->setAlignment(Qt::AlignCenter);

	m_pTitleTextLabel = new QLabel(this);
	m_pTitleTextLabel->setText(u8"NiceMeeting");
	m_pTitleTextLabel->setFixedSize(200, 40); 
	m_pTitleTextLabel->setStyleSheet(
			"QLabel{font-family:Microsoft YaHei;\
			font-size:18px;\
			color:#BDC8E2;}"
			);

	m_pMaxButton = new QPushButton(this);
	m_pMaxButton->setFixedSize(32, 32);
	m_pMaxButton->setIconSize(QSize(32, 32));
	m_pMaxButton->setStyleSheet(
		"QPushButton {"
		"	 background-image:url(:/titlebar/resources/mainwidget/titlebar/max.svg);"
		"    border: none;"
		"}"
		"QPushButton:hover {"
		"    background-image: url(:/titlebar/resources/mainwidget/titlebar/max_hover.svg);"
		"    background-color: rgba(99, 99, 99, 100);"
		"}"
	);

	m_pSetButton = new QPushButton(this);
	m_pSetButton->setFixedSize(32, 32);
	m_pSetButton->setIconSize(QSize(32, 32));
	m_pSetButton->setStyleSheet(
		"QPushButton {"
		"	 background-image:url(:/titlebar/resources/mainwidget/titlebar/normal.svg);"
		"    border: none;"
		"}"
		"QPushButton:hover {"
		"    background-image: url(:/titlebar/resources/mainwidget/titlebar/normal_hover.svg);"
		"    background-color: rgba(99, 99, 99, 100);"
		"}"
	);

	m_pMinButton = new QPushButton(this);
	m_pMinButton->setFixedSize(32, 32);
	m_pMinButton->setIconSize(QSize(32, 32));
	m_pMinButton->setStyleSheet(
		"QPushButton {"
		"	 background-image:url(:/titlebar/resources/mainwidget/titlebar/min.svg);"
		"    border: none;"
		"}"
		"QPushButton:hover {"
		"    background-image: url(:/titlebar/resources/mainwidget/titlebar/min_hover.svg);"
		"    background-color: rgba(99, 99, 99, 100);"
		"}"
	);

	m_pCloseButton = new QPushButton(this);
	m_pCloseButton->setFixedSize(32, 32);
	m_pCloseButton->setIconSize(QSize(32, 32));
	m_pCloseButton->setStyleSheet(
		"QPushButton {"
		"	 background-image:url(:/titlebar/resources/mainwidget/titlebar/close.svg);"
		"    border: none;"
		"}"
		"QPushButton:hover {"
		"    background-image: url(:/titlebar/resources/mainwidget/titlebar/close_hover.svg);"
		"    background-color: rgba(99, 99, 99, 100);"
		"}"
	);

	QHBoxLayout* pHlay = new QHBoxLayout(this);
	pHlay->addWidget(m_pLogo);
	pHlay->addWidget(m_pTitleTextLabel);
	pHlay->addStretch();
	pHlay->addWidget(m_pSetButton);
	pHlay->addWidget(m_pMaxButton);
	pHlay->addWidget(m_pMinButton);
	pHlay->addWidget(m_pCloseButton);

	pHlay->setContentsMargins(5, 5, 5, 5);

	connect(m_pMinButton, &QPushButton::clicked, this, &CTitleBar::OnClicked);
	connect(m_pMaxButton, &QPushButton::clicked, this, &CTitleBar::OnClicked);
	connect(m_pCloseButton, &QPushButton::clicked, this, &CTitleBar::OnClicked);
}

void CTitleBar::mousePressEvent(QMouseEvent* event)
{
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
		if (pWindow ->isMaximized())
		{
			pWindow->showNormal();
			m_pMaxButton->setIcon(QIcon(":/MainWidget/resources/max.svg"));
			m_pMaxButton->setIconSize(QSize(32, 32));
			m_pMaxButton->setStyleSheet(
				"QPushButton {"
				"    border: none;"
				"    background: transparent;"
				"}"
				"QPushButton:hover {"
				"    image: url(:/MainWidget/resources/max-hover.svg);"
				"    background-color: rgba(99, 99, 99, 100);"
				"}"
			);
		}
		else {
			pWindow->showMaximized();
			m_pMaxButton->setIcon(QIcon(":/MainWidget/resources/normal.svg"));
			m_pMaxButton->setIconSize(QSize(32, 32));
			m_pMaxButton->setStyleSheet(
				"QPushButton {"
				"    border: none;"
				"    background: transparent;"
				"}"
				"QPushButton:hover {"
				"    image: url(:/MainWidget/resources/normal_hover.svg);"
				"    background-color: rgba(99, 99, 99, 100);"
				"}"
			);
		}
	}
	else if (pButton == m_pCloseButton)
	{
		emit sig_close();
	}
}
