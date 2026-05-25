#include "CTitleBar.h"
#include "commons.h"
#include <qt_windows.h>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QIcon>
#include <QString>

namespace {
const char* kTitleBarRes = ":/titlebar/resources/mainwidget/titlebar/";
const int kTitleIconPx = 20;   // ͼ����ʾ�ߴ磻��ť�� 32��32 ������
QString titleImageButtonStyle(const char* normalIcon, const char* hoverIcon)
{
	return QString(
		"QPushButton {"
		"    border: none;"
		"    background: transparent;"
		"    background-image: url(%1%2);"
		"    background-repeat: no-repeat;"
		"    background-position: center;"
		"}"
		"QPushButton:hover {"
		"    background-color: rgba(99, 99, 99, 100);"
		"    background-image: url(%1%3);"
		"    background-repeat: no-repeat;"
		"    background-position: center;"
		"}")
		.arg(kTitleBarRes)
		.arg(normalIcon)
		.arg(hoverIcon);
}

void setupTitleImageButton(QPushButton* btn, const char* normalIcon, const char* hoverIcon)
{
	btn->setFlat(true);
	btn->setText(QString());
	btn->setIcon(QIcon());
	btn->setFixedSize(46, TITLE_BAR_HEIGHT);
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
	m_pLogo->resize(32, 32);
	QPixmap pixmap(QString(kTitleBarRes) + "logo.png");
	QPixmap scaled = pixmap.scaled(kTitleIconPx, kTitleIconPx, Qt::KeepAspectRatio, Qt::SmoothTransformation);
	m_pLogo->setPixmap(scaled);
	m_pLogo->setAlignment(Qt::AlignCenter);

	m_pTitleTextLabel = new QLabel(this);
	m_pTitleTextLabel->setText(u8"NiceMeeting");
	m_pTitleTextLabel->resize(200, 40); 
	m_pTitleTextLabel->setStyleSheet(
			"QLabel{font-family:Microsoft YaHei;\
			font-size:18px;\
			color:#BDC8E2;}"
			);

	m_pMaxButton = new QPushButton(this);
	setupTitleImageButton(m_pMaxButton, "max.svg", "max_hover.svg");

	m_pSetButton = new QPushButton(this);
	setupTitleImageButton(m_pSetButton, "normal.svg", "normal_hover.svg");

	m_pMinButton = new QPushButton(this);
	setupTitleImageButton(m_pMinButton, "min.svg", "min_hover.svg");

	m_pCloseButton = new QPushButton(this);
	setupTitleImageButton(m_pCloseButton, "close.svg", "close_hover.svg");

	QHBoxLayout* pHlay = new QHBoxLayout(this);
	pHlay->addWidget(m_pLogo);
	pHlay->addWidget(m_pTitleTextLabel);
	pHlay->addStretch();
	pHlay->addWidget(m_pSetButton);
	pHlay->addWidget(m_pMaxButton);
	pHlay->addWidget(m_pMinButton);
	pHlay->addWidget(m_pCloseButton);

	pHlay->setContentsMargins(12, 0, 0, 0);
	pHlay->setSpacing(0);
	pHlay->setAlignment(Qt::AlignVCenter);

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
