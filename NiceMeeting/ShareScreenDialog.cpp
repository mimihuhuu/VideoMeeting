#include "ShareScreenDialog.h"
#include "commons.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMouseEvent>
#include <QMessageBox>

namespace {
const char* kShareScreenRes = ":/mainwidget/resources/mainwidget/sharescreen/";
} // namespace

ShareScreenDialog::ShareScreenDialog(QWidget* parent)
	: QDialog(parent)
{
	setWindowFlags(Qt::FramelessWindowHint | Qt::SubWindow | Qt::Tool);
	setAttribute(Qt::WA_StyledBackground);

	m_pTitleWidget = new QWidget(this);
	m_pTitleWidget->resize(800, 42);
	m_pTitleWidget->setStyleSheet("background-color: rgb(54, 54, 54);");

	m_pTitleLabel = new QLabel(m_pTitleWidget);
	m_pTitleLabel->setStyleSheet(
		"font-family: Microsoft YaHei; font-size: 16px; color: #FFFFFF; background-color: transparent;");
	m_pTitleLabel->setText(u8"\u8bf7\u9009\u62e9\u9700\u8981\u5171\u4eab\u7684\u5185\u5bb9");

	const QString minBtnStyle = QString(
		"QPushButton { background-image: url(%1min.svg); background-position: center; "
		"background-repeat: no-repeat; border: none; }"
		"QPushButton:hover { background-color: rgb(104, 104, 104); "
		"background-image: url(%1min.svg); }").arg(kShareScreenRes);

	const QString closeBtnStyle = QString(
		"QPushButton { background-image: url(%1close.svg); background-position: center; "
		"background-repeat: no-repeat; border: none; }"
		"QPushButton:hover { background-color: rgb(104, 104, 104); "
		"background-image: url(%1close.svg); }").arg(kShareScreenRes);

	m_pBtnMin = new QPushButton(m_pTitleWidget);
	m_pBtnMin->resize(32, 32);
	m_pBtnMin->setStyleSheet(minBtnStyle);

	m_pBtnClose = new QPushButton(m_pTitleWidget);
	m_pBtnClose->resize(32, 32);
	m_pBtnClose->setStyleSheet(closeBtnStyle);

	QHBoxLayout* pTitleHLay = new QHBoxLayout(m_pTitleWidget);
	pTitleHLay->addWidget(m_pTitleLabel);
	pTitleHLay->addStretch();
	pTitleHLay->addWidget(m_pBtnMin);
	pTitleHLay->addWidget(m_pBtnClose);
	pTitleHLay->setContentsMargins(5, 5, 5, 5);

	QVBoxLayout* pMainVLay = new QVBoxLayout(this);
	pMainVLay->setSpacing(10);

	m_pLabel_Screen = new QLabel(this);
	m_pLabel_Screen->setText(u8"\u684c\u9762");
	m_pLabel_Screen->resize(m_pLabel_Screen->width(), 32);

	const int vScrollBarWidth = 9;
	const QString qssLW = QString(
		"QScrollBar { width: %1px; background: rgba(255, 255, 255, 100%%); margin: 0px; }"
		"QScrollBar::handle:vertical { width: 8px; background: rgba(162, 163, 165, 100%%); "
		"border-radius: 4px; min-height: 60; }"
		"QScrollBar::handle:vertical:hover { width: 8px; background: rgba(115, 118, 118, 100%%); "
		"border-radius: 4px; min-height: 60; }"
		"QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical "
		"{ background: rgba(255, 255, 255, 100%%); border-radius: 4px; }"
		"QScrollBar::top-arrow:vertical, QScrollBar::bottom-arrow:vertical "
		"{ border: none; background: none; }"
		"QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical "
		"{ border: none; background: none; }"
		"QListWidget { background-color: rgb(255, 255, 255); border-style: solid; "
		"border-right-width: 1px; border-right-color: rgb(202, 199, 198); }")
		.arg(vScrollBarWidth);

	const int LWSPACE = 20;
	m_pLWScreen = new QListWidget(this);
	m_pLWScreen->setSpacing(LWSPACE);
	const int w1 = SHARE_WINDOW_ITEM_WIDTH * 4 + 5 * LWSPACE;
	const int h1 = SHARE_WINDOW_ITEM_HEIGHT + 45 + LWSPACE * 2 + 10;
	m_pLWScreen->resize(w1, h1);
	m_pLWScreen->setIconSize(QSize(SHARE_WINDOW_ITEM_WIDTH, SHARE_WINDOW_ITEM_HEIGHT + 45));
	m_pLWScreen->setViewMode(QListView::IconMode);
	m_pLWScreen->setWrapping(true);
	m_pLWScreen->setFlow(QListView::LeftToRight);
	m_pLWScreen->setStyleSheet(qssLW);

	m_pLabel_App = new QLabel(this);
	m_pLabel_App->resize(m_pLabel_App->width(), 32);
	m_pLabel_App->setText(u8"\u5e94\u7528\u7a0b\u5e8f");

	m_pLWApp = new QListWidget(this);
	m_pLWApp->setSpacing(LWSPACE);
	const int w2 = w1;
	const int h2 = (SHARE_WINDOW_ITEM_HEIGHT + 45) * 2 + 4 * LWSPACE;
	m_pLWApp->resize(w2, h2);
	m_pLWApp->setIconSize(QSize(SHARE_WINDOW_ITEM_WIDTH, SHARE_WINDOW_ITEM_HEIGHT + 45));
	m_pLWApp->setViewMode(QListView::IconMode);
	m_pLWApp->setWrapping(true);
	m_pLWApp->setFlow(QListView::LeftToRight);
	m_pLWApp->setStyleSheet(qssLW);

	m_pBtnCancel = new QPushButton(this);
	m_pBtnCancel->setText(u8"\u53d6\u6d88");
	m_pBtnCancel->resize(m_pBtnCancel->width(), 48);

	m_pBtnStart = new QPushButton(this);
	m_pBtnStart->setText(u8"\u5f00\u59cb\u5171\u4eab");
	m_pBtnStart->resize(m_pBtnStart->width(), 48);

	QHBoxLayout* pBottomHLay = new QHBoxLayout();
	pBottomHLay->addStretch();
	pBottomHLay->addWidget(m_pBtnCancel);
	pBottomHLay->addWidget(m_pBtnStart);
	pBottomHLay->setContentsMargins(0, 0, 0, 0);

	pMainVLay->addWidget(m_pTitleWidget);
	pMainVLay->addWidget(m_pLabel_Screen);
	pMainVLay->addWidget(m_pLWScreen);
	pMainVLay->addWidget(m_pLabel_App);
	pMainVLay->addWidget(m_pLWApp);
	pMainVLay->addLayout(pBottomHLay);
	pMainVLay->setContentsMargins(0, 0, 0, 0);

	connect(m_pBtnMin, &QPushButton::clicked, this, &ShareScreenDialog::onMin);
	connect(m_pBtnClose, &QPushButton::clicked, this, &ShareScreenDialog::onClose);
	connect(m_pBtnCancel, &QPushButton::clicked, this, &ShareScreenDialog::onCancel);
	connect(m_pBtnStart, &QPushButton::clicked, this, &ShareScreenDialog::onStartShare);
	connect(m_pLWScreen, &QListWidget::itemClicked, this, &ShareScreenDialog::onDesktopItemSelected);
	connect(m_pLWApp, &QListWidget::itemClicked, this, &ShareScreenDialog::onAppItemSelected);
}

ShareScreenDialog::~ShareScreenDialog()
{
}

void ShareScreenDialog::initListWidget(const VecWindowShareInfo& vec)
{
	clearLists();
	clearMap();

	int desktopId = 0;
	int appId = 0;

	for (const auto& ele : vec)
	{
		auto* item = new QListWidgetItem(QIcon(ele.pixmap), ele.name);
		item->setSizeHint(QSize(SHARE_WINDOW_ITEM_WIDTH, SHARE_WINDOW_ITEM_HEIGHT + 45));

		if (ele.windowType == window_share_info::Screen)
		{
			m_pLWScreen->addItem(item); 
			m_mapScreen[desktopId++] = ele.sourceId;
		}
		else
		{
			m_pLWApp->addItem(item);
			m_mapApp[appId++] = ele.sourceId;
		}
	}
}

void ShareScreenDialog::clearMap()
{
	m_mapScreen.clear();
	m_mapApp.clear();
}

void ShareScreenDialog::clearLists()
{
	if (m_pLWScreen) {
		m_pLWScreen->clear();
	}
	if (m_pLWApp) {
		m_pLWApp->clear();
	}
	m_selectedType = -1;
}

void ShareScreenDialog::mousePressEvent(QMouseEvent* event)
{
	if (event->button() != Qt::LeftButton || !m_pTitleWidget) {
		QDialog::mousePressEvent(event);
		return;
	}
	if (!m_pTitleWidget->geometry().contains(event->pos())) {
		QDialog::mousePressEvent(event);
		return;
	}

	m_dragging = true;
	m_windowPos = pos();
	m_mousePos = event->globalPos();
	m_dPos = m_mousePos - m_windowPos;
	event->accept();
}

void ShareScreenDialog::mouseMoveEvent(QMouseEvent* event)
{
	if (!m_dragging || !(event->buttons() & Qt::LeftButton)) {
		QDialog::mouseMoveEvent(event);
		return;
	}
	move(event->globalPos() - m_dPos);
	event->accept();
}

void ShareScreenDialog::mouseReleaseEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton) {
		m_dragging = false;
	}
	QDialog::mouseReleaseEvent(event);
}

void ShareScreenDialog::onMin()
{
	showMinimized();
}

void ShareScreenDialog::onClose()
{
	clearLists();
	clearMap();
	close();
}

void ShareScreenDialog::onCancel()
{
	clearLists();
	clearMap();
	close();
}

void ShareScreenDialog::onStartShare()
{
	if (m_selectedType < 0) {
		QMessageBox::warning(this, u8"\u63d0\u793a", u8"\u8bf7\u5148\u9009\u62e9\u8981\u5171\u4eab\u7684\u5185\u5bb9");
		return;
	}

	qint64 sourceId = 0;

	if (m_selectedType == 0)
	{
		const int shareIndex = m_pLWScreen->currentRow();
		if (shareIndex < 0 || m_mapScreen.find(shareIndex) == m_mapScreen.end()) {
			QMessageBox::warning(this, u8"\u63d0\u793a", u8"\u8bf7\u9009\u62e9\u8981\u5171\u4eab\u7684\u684c\u9762");
			return;
		}
		sourceId = m_mapScreen[shareIndex];
	}
	else if (m_selectedType == 1)
	{
		const int shareIndex = m_pLWApp->currentRow();
		if (shareIndex < 0 || m_mapApp.find(shareIndex) == m_mapApp.end()) {
			QMessageBox::warning(this, u8"\u63d0\u793a", u8"\u8bf7\u9009\u62e9\u8981\u5171\u4eab\u7684\u5e94\u7528\u7a0b\u5e8f");
			return;
		}
		sourceId = m_mapApp[shareIndex];
	}

	if (sourceId == 0) {
		QMessageBox::warning(this, u8"\u63d0\u793a", u8"\u65e0\u6548\u7684\u5171\u4eab\u76ee\u6807");
		return;
	}

	emit sig_StartShare(m_selectedType, sourceId);
	clearLists();
	clearMap();
	close();
}

void ShareScreenDialog::onDesktopItemSelected(QListWidgetItem* item)
{
	Q_UNUSED(item);
	m_selectedType = 0;
	m_pLWApp->clearSelection();
}

void ShareScreenDialog::onAppItemSelected(QListWidgetItem* item)
{
	Q_UNUSED(item);
	m_selectedType = 1;
	m_pLWScreen->clearSelection();
}
