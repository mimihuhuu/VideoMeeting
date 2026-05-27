#include "ShareScreenDialog.h"

#include <QHBoxLayout>
#include <QIcon>
#include <QMessageBox>
#include <QMouseEvent>
#include <QSizePolicy>
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

const char* kNavButtonStyleTemplate =
	"QPushButton {"
	"    border: 1px solid #CCCCCC;"
	"    background-color: #F5F5F5;"
	"    color: #333333;"
	"    font-family: 'Microsoft YaHei';"
	"    font-size: %1px;"
	"    font-weight: bold;"
	"}"
	"QPushButton:hover { background-color: #EAEAEA; }"
	"QPushButton:pressed { background-color: #DDDDDD; }"
	"QPushButton:disabled { color: #BBBBBB; background-color: #F8F8F8; }";

QPushButton* createNavButton(QWidget* parent)
{
	auto* button = new QPushButton(parent);
	button->setFixedSize(SHARE_SCREEN_CAROUSEL_ROW_HEIGHT, SHARE_SCREEN_CAROUSEL_ROW_HEIGHT);
	button->setStyleSheet(QString::fromLatin1(kNavButtonStyleTemplate).arg(SHARE_SCREEN_DIALOG_NAV_FONT_SIZE));
	button->setCursor(Qt::PointingHandCursor);
	return button;
}

QLabel* createPreviewLabel(QWidget* parent)
{
	auto* label = new QLabel(parent);
	label->setFixedSize(SHARE_WINDOW_ITEM_WIDTH, SHARE_WINDOW_ITEM_HEIGHT);
	label->setAlignment(Qt::AlignCenter);
	label->setStyleSheet(QStringLiteral("background-color: #F0F0F0; border: 1px solid rgb(202, 199, 198);"));
	return label;
}

QLabel* createNameLabel(QWidget* parent)
{
	auto* label = new QLabel(parent);
	label->setFixedHeight(SHARE_WINDOW_ITEM_CAPTION_HEIGHT);
	label->setAlignment(Qt::AlignCenter);
	label->setWordWrap(true);
	label->setStyleSheet(
		QStringLiteral("font-family: Microsoft YaHei; font-size: %1px; color: #333333;")
			.arg(SHARE_SCREEN_DIALOG_NAME_FONT_SIZE));
	return label;
}

QPixmap scaledPreview(const QPixmap& pixmap)
{
	if (pixmap.isNull()) {
		return pixmap;
	}
	return pixmap.scaled(SHARE_WINDOW_ITEM_WIDTH, SHARE_WINDOW_ITEM_HEIGHT,
		Qt::KeepAspectRatio, Qt::SmoothTransformation);
}

QHBoxLayout* createCarouselRow(QPushButton* prevButton, QWidget* centerWidget, QPushButton* nextButton)
{
	auto* layout = new QHBoxLayout();
	layout->setSpacing(8);
	layout->setContentsMargins(0, 0, 0, 0);
	layout->addWidget(prevButton, 0, Qt::AlignVCenter);
	layout->addWidget(centerWidget, 1, Qt::AlignHCenter);
	layout->addWidget(nextButton, 0, Qt::AlignVCenter);
	return layout;
}

} // namespace

ShareScreenDialog::ShareScreenDialog(QWidget* parent)
	: QDialog(parent)
{
	setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
	setAttribute(Qt::WA_StyledBackground);
	setFixedSize(SHARE_SCREEN_DIALOG_WIDTH, SHARE_SCREEN_DIALOG_HEIGHT);
	setStyleSheet(QStringLiteral("background-color: rgb(255, 255, 255);"));

	m_pTitleWidget = new QWidget(this);
	m_pTitleWidget->setFixedHeight(SHARE_SCREEN_DIALOG_TITLE_HEIGHT);
	m_pTitleWidget->setAttribute(Qt::WA_StyledBackground, true);
	m_pTitleWidget->setStyleSheet(QStringLiteral("background-color: rgb(54, 54, 54);"));

	m_pTitleLabel = new QLabel(m_pTitleWidget);
	m_pTitleLabel->setStyleSheet(
		QStringLiteral("font-family: Microsoft YaHei; font-size: %1px; color: #FFFFFF; background-color: transparent;")
			.arg(SHARE_SCREEN_DIALOG_TITLE_FONT_SIZE));
	m_pTitleLabel->setText(QString::fromUtf8(u8"\u5171\u4eab\u5c4f\u5e55"));

	m_pBtnClose = new QPushButton(m_pTitleWidget);
	setupTitleImageButton(m_pBtnClose, "close.svg", "close_hover.svg",
		SHARE_SCREEN_DIALOG_CLOSE_SIZE, SHARE_SCREEN_DIALOG_CLOSE_ICON_SIZE);

	auto* pTitleHLay = new QHBoxLayout(m_pTitleWidget);
	pTitleHLay->setContentsMargins(16, 0, 0, 0);
	pTitleHLay->setSpacing(0);
	pTitleHLay->addWidget(m_pTitleLabel, 1, Qt::AlignVCenter);
	pTitleHLay->addWidget(m_pBtnClose, 0, Qt::AlignVCenter);

	m_pLabel_Screen = new QLabel(this);
	m_pLabel_Screen->setText(QString::fromUtf8(u8"\u684c\u9762"));
	m_pLabel_Screen->setFixedHeight(SHARE_SCREEN_DIALOG_SECTION_LABEL_HEIGHT);
	m_pLabel_Screen->setAlignment(Qt::AlignCenter);
	m_pLabel_Screen->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	m_pLabel_Screen->setStyleSheet(
		QStringLiteral("font-family: Microsoft YaHei; font-size: %1px; color: #333333; font-weight: bold;")
			.arg(SHARE_SCREEN_DIALOG_SECTION_FONT_SIZE));

	m_pScreenPreview = createPreviewLabel(this);
	m_pScreenName = createNameLabel(this);
	m_pBtnScreenPrev = createNavButton(this);
	m_pBtnScreenNext = createNavButton(this);
	m_pBtnScreenPrev->setText(QStringLiteral("<"));
	m_pBtnScreenNext->setText(QStringLiteral(">"));

	auto* pScreenCenter = new QWidget(this);
	auto* pScreenCenterLay = new QVBoxLayout(pScreenCenter);
	pScreenCenterLay->setContentsMargins(0, 0, 0, 0);
	pScreenCenterLay->setSpacing(8);
	pScreenCenterLay->addWidget(m_pScreenPreview, 0, Qt::AlignHCenter);
	pScreenCenterLay->addWidget(m_pScreenName, 0, Qt::AlignHCenter);

	m_pLabel_App = new QLabel(this);
	m_pLabel_App->setText(QString::fromUtf8(u8"\u5e94\u7528\u7a0b\u5e8f"));
	m_pLabel_App->setFixedHeight(SHARE_SCREEN_DIALOG_SECTION_LABEL_HEIGHT);
	m_pLabel_App->setAlignment(Qt::AlignCenter);
	m_pLabel_App->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	m_pLabel_App->setStyleSheet(
		QStringLiteral("font-family: Microsoft YaHei; font-size: %1px; color: #333333; font-weight: bold;")
			.arg(SHARE_SCREEN_DIALOG_SECTION_FONT_SIZE));

	m_pAppPreview = createPreviewLabel(this);
	m_pAppName = createNameLabel(this);
	m_pBtnAppPrev = createNavButton(this);
	m_pBtnAppNext = createNavButton(this);
	m_pBtnAppPrev->setText(QStringLiteral("<"));
	m_pBtnAppNext->setText(QStringLiteral(">"));

	auto* pAppCenter = new QWidget(this);
	auto* pAppCenterLay = new QVBoxLayout(pAppCenter);
	pAppCenterLay->setContentsMargins(0, 0, 0, 0);
	pAppCenterLay->setSpacing(8);
	pAppCenterLay->addWidget(m_pAppPreview, 0, Qt::AlignHCenter);
	pAppCenterLay->addWidget(m_pAppName, 0, Qt::AlignHCenter);

	auto* pScreenColumn = new QVBoxLayout();
	pScreenColumn->setSpacing(8);
	pScreenColumn->setContentsMargins(0, 0, 0, 0);
	pScreenColumn->addWidget(m_pLabel_Screen);
	pScreenColumn->addLayout(createCarouselRow(m_pBtnScreenPrev, pScreenCenter, m_pBtnScreenNext));

	auto* pAppColumn = new QVBoxLayout();
	pAppColumn->setSpacing(8);
	pAppColumn->setContentsMargins(0, 0, 0, 0);
	pAppColumn->addWidget(m_pLabel_App);
	pAppColumn->addLayout(createCarouselRow(m_pBtnAppPrev, pAppCenter, m_pBtnAppNext));

	auto* pContentHLay = new QHBoxLayout();
	pContentHLay->setSpacing(16);
	pContentHLay->addLayout(pScreenColumn, 1);
	pContentHLay->addLayout(pAppColumn, 1);

	m_pBtnStart = new QPushButton(this);
	m_pBtnStart->setText(QString::fromUtf8(u8"\u5f00\u59cb\u5171\u4eab"));
	m_pBtnStart->setFixedHeight(SHARE_SCREEN_DIALOG_START_BTN_HEIGHT);
	m_pBtnStart->setMinimumWidth(SHARE_SCREEN_DIALOG_START_BTN_MIN_WIDTH);
	m_pBtnStart->setStyleSheet(
		QStringLiteral("font-family: Microsoft YaHei; font-size: %1px;")
			.arg(SHARE_SCREEN_DIALOG_START_BTN_FONT_SIZE));

	auto* pBottomHLay = new QHBoxLayout();
	pBottomHLay->addStretch();
	pBottomHLay->addWidget(m_pBtnStart);
	pBottomHLay->setContentsMargins(0, 0, 0, 0);

	auto* pMainVLay = new QVBoxLayout(this);
	pMainVLay->setSpacing(0);
	pMainVLay->setContentsMargins(0, 0, 0, SHARE_SCREEN_DIALOG_PADDING);
	pMainVLay->addWidget(m_pTitleWidget);
	pMainVLay->addSpacing(SHARE_SCREEN_DIALOG_PADDING);

	auto* pBodyWidget = new QWidget(this);
	auto* pBodyVLay = new QVBoxLayout(pBodyWidget);
	pBodyVLay->setSpacing(SHARE_SCREEN_DIALOG_PADDING);
	pBodyVLay->setContentsMargins(SHARE_SCREEN_DIALOG_PADDING, 0, SHARE_SCREEN_DIALOG_PADDING, 0);
	pBodyVLay->addLayout(pContentHLay);
	pBodyVLay->addLayout(pBottomHLay);

	pMainVLay->addWidget(pBodyWidget, 1);

	connect(m_pBtnClose, &QPushButton::clicked, this, &ShareScreenDialog::onClose);
	connect(m_pBtnStart, &QPushButton::clicked, this, &ShareScreenDialog::onStartShare);
	connect(m_pBtnScreenPrev, &QPushButton::clicked, this, &ShareScreenDialog::onScreenPrev);
	connect(m_pBtnScreenNext, &QPushButton::clicked, this, &ShareScreenDialog::onScreenNext);
	connect(m_pBtnAppPrev, &QPushButton::clicked, this, &ShareScreenDialog::onAppPrev);
	connect(m_pBtnAppNext, &QPushButton::clicked, this, &ShareScreenDialog::onAppNext);
}

ShareScreenDialog::~ShareScreenDialog()
{
}

void ShareScreenDialog::initListWidget(const VecWindowShareInfo& vec)
{
	clearData();

	for (const auto& ele : vec) {
		ShareScreenItem item;
		item.name = ele.name;
		item.pixmap = ele.pixmap;
		item.sourceId = ele.sourceId;
		if (ele.windowType == window_share_info::Screen) {
			m_screenItems.push_back(item);
		} else {
			m_appItems.push_back(item);
		}
	}

	m_screenIndex = m_screenItems.empty() ? -1 : 0;
	m_appIndex = m_appItems.empty() ? -1 : 0;
	if (!m_screenItems.empty()) {
		m_selectedType = 0;
	} else if (!m_appItems.empty()) {
		m_selectedType = 1;
	} else {
		m_selectedType = -1;
	}

	updateScreenPreview();
	updateAppPreview();
}

void ShareScreenDialog::clearData()
{
	m_screenItems.clear();
	m_appItems.clear();
	m_screenIndex = -1;
	m_appIndex = -1;
	m_selectedType = -1;
}

int ShareScreenDialog::wrapIndex(int index, int count, int delta) const
{
	if (count <= 0) {
		return -1;
	}
	return (index + delta + count) % count;
}

void ShareScreenDialog::setSelectedColumn(int type)
{
	m_selectedType = type;
}

void ShareScreenDialog::updateScreenPreview()
{
	const bool hasItems = !m_screenItems.empty() && m_screenIndex >= 0;
	m_pBtnScreenPrev->setEnabled(hasItems);
	m_pBtnScreenNext->setEnabled(hasItems);
	if (!hasItems) {
		m_pScreenPreview->clear();
		m_pScreenName->setText(QString::fromUtf8(u8"\u6682\u65e0\u53ef\u7528\u684c\u9762"));
		return;
	}

	const ShareScreenItem& item = m_screenItems[static_cast<size_t>(m_screenIndex)];
	m_pScreenPreview->setPixmap(scaledPreview(item.pixmap));
	m_pScreenName->setText(item.name);
}

void ShareScreenDialog::updateAppPreview()
{
	const bool hasItems = !m_appItems.empty() && m_appIndex >= 0;
	m_pBtnAppPrev->setEnabled(hasItems);
	m_pBtnAppNext->setEnabled(hasItems);
	if (!hasItems) {
		m_pAppPreview->clear();
		m_pAppName->setText(QString::fromUtf8(u8"\u6682\u65e0\u53ef\u7528\u7a0b\u5e8f"));
		return;
	}

	const ShareScreenItem& item = m_appItems[static_cast<size_t>(m_appIndex)];
	m_pAppPreview->setPixmap(scaledPreview(item.pixmap));
	m_pAppName->setText(item.name);
}

void ShareScreenDialog::onScreenPrev()
{
	if (m_screenItems.empty()) {
		return;
	}
	m_screenIndex = wrapIndex(m_screenIndex, static_cast<int>(m_screenItems.size()), -1);
	setSelectedColumn(0);
	updateScreenPreview();
}

void ShareScreenDialog::onScreenNext()
{
	if (m_screenItems.empty()) {
		return;
	}
	m_screenIndex = wrapIndex(m_screenIndex, static_cast<int>(m_screenItems.size()), 1);
	setSelectedColumn(0);
	updateScreenPreview();
}

void ShareScreenDialog::onAppPrev()
{
	if (m_appItems.empty()) {
		return;
	}
	m_appIndex = wrapIndex(m_appIndex, static_cast<int>(m_appItems.size()), -1);
	setSelectedColumn(1);
	updateAppPreview();
}

void ShareScreenDialog::onAppNext()
{
	if (m_appItems.empty()) {
		return;
	}
	m_appIndex = wrapIndex(m_appIndex, static_cast<int>(m_appItems.size()), 1);
	setSelectedColumn(1);
	updateAppPreview();
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

void ShareScreenDialog::onClose()
{
	clearData();
	close();
}

void ShareScreenDialog::onStartShare()
{
	if (m_selectedType < 0) {
		QMessageBox::warning(this, QString::fromUtf8(u8"\u63d0\u793a"),
			QString::fromUtf8(u8"\u8bf7\u5148\u9009\u62e9\u8981\u5171\u4eab\u7684\u5185\u5bb9"));
		return;
	}

	qint64 sourceId = 0;
	if (m_selectedType == 0) {
		if (m_screenIndex < 0 || m_screenIndex >= static_cast<int>(m_screenItems.size())) {
			QMessageBox::warning(this, QString::fromUtf8(u8"\u63d0\u793a"),
				QString::fromUtf8(u8"\u8bf7\u9009\u62e9\u8981\u5171\u4eab\u7684\u684c\u9762"));
			return;
		}
		sourceId = m_screenItems[static_cast<size_t>(m_screenIndex)].sourceId;
	} else if (m_selectedType == 1) {
		if (m_appIndex < 0 || m_appIndex >= static_cast<int>(m_appItems.size())) {
			QMessageBox::warning(this, QString::fromUtf8(u8"\u63d0\u793a"),
				QString::fromUtf8(u8"\u8bf7\u9009\u62e9\u8981\u5171\u4eab\u7684\u5e94\u7528\u7a0b\u5e8f"));
			return;
		}
		sourceId = m_appItems[static_cast<size_t>(m_appIndex)].sourceId;
	}

	if (sourceId == 0) {
		QMessageBox::warning(this, QString::fromUtf8(u8"\u63d0\u793a"),
			QString::fromUtf8(u8"\u65e0\u6548\u7684\u5171\u4eab\u76ee\u6807"));
		return;
	}

	emit sig_StartShare(m_selectedType, sourceId);
	clearData();
	close();
}
