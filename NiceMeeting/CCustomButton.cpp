#include "CCustomButton.h"
#include "commons.h"
#include <QEvent>
#include <QFont>
#include <QIcon>
#include <QLabel>
#include <QMouseEvent>
#include <QSizePolicy>
#include <QVBoxLayout>

CCustomButton::CCustomButton(QString text, QString normalImageUrl, QString normalhoverImageUrl, QString clickedImageUrl,
		QString clickedImagehoverUrl, bool clicked, QWidget* parent)
	: QWidget(parent)
	, m_text(text)
	, m_normalImageUrl(normalImageUrl)
	, m_normalhoverImageUrl(normalhoverImageUrl)
	, m_clickedImageUrl(clickedImageUrl)
	, m_clickedImagehoverUrl(clickedImagehoverUrl)
	, m_StateOpen(clicked)
{
	setAttribute(Qt::WA_Hover, true);
	setAutoFillBackground(false);
	setCursor(Qt::PointingHandCursor);
	setMinimumWidth(BOTTONBAR_TOOL_BTN_MIN_WIDTH);
	setFixedHeight(BOTTONBAR_TOOL_BTN_HEIGHT);
	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

	m_pIconLabel = new QLabel(this);
	m_pIconLabel->setFixedSize(BOTTONBAR_TOOL_ICON_SIZE, BOTTONBAR_TOOL_ICON_SIZE);
	m_pIconLabel->setAlignment(Qt::AlignCenter);
	m_pIconLabel->setAttribute(Qt::WA_TransparentForMouseEvents);
	m_pIconLabel->setAutoFillBackground(false);

	m_pTextLabel = new QLabel(m_text, this);
	m_pTextLabel->setAlignment(Qt::AlignCenter);
	m_pTextLabel->setAttribute(Qt::WA_TransparentForMouseEvents);
	m_pTextLabel->setAutoFillBackground(false);
	QFont font(QStringLiteral("Microsoft YaHei"), BOTTONBAR_TOOL_FONT_SIZE);
	m_pTextLabel->setFont(font);
	m_pTextLabel->setStyleSheet(QStringLiteral("color: #333333; background: transparent;"));

	auto* layout = new QVBoxLayout(this);
	layout->setContentsMargins(0, 12, 0, 9);
	layout->setSpacing(BOTTONBAR_ICON_TEXT_SPACING);
	layout->addWidget(m_pIconLabel, 0, Qt::AlignHCenter);
	layout->addWidget(m_pTextLabel, 0, Qt::AlignHCenter);

	updateIcon(false);
}

CCustomButton::~CCustomButton()
{
}

void CCustomButton::setText(const QString& text)
{
	m_text = text;
	if (m_pTextLabel) {
		m_pTextLabel->setText(text);
	}
}

void CCustomButton::setOpen(bool open)
{
	m_StateOpen = open;
	updateIcon(underMouse());
}

void CCustomButton::updateIcon(bool hover)
{
	const QString& url = hover
		? (m_StateOpen ? m_normalhoverImageUrl : m_clickedImagehoverUrl)
		: (m_StateOpen ? m_normalImageUrl : m_clickedImageUrl);
	const QPixmap pixmap = QIcon(url).pixmap(BOTTONBAR_TOOL_ICON_SIZE, BOTTONBAR_TOOL_ICON_SIZE);
	if (m_pIconLabel) {
		m_pIconLabel->setPixmap(pixmap);
	}
}

void CCustomButton::enterEvent(QEvent* event)
{
	m_hovered = true;
	updateIcon(true);
	QWidget::enterEvent(event);
}

void CCustomButton::leaveEvent(QEvent* event)
{
	m_hovered = false;
	updateIcon(false);
	QWidget::leaveEvent(event);
}

void CCustomButton::mousePressEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton) {
		updateIcon(false);
		emit sig_clicked();
		if (underMouse()) {
			updateIcon(true);
		}
		event->accept();
		return;
	}
	QWidget::mousePressEvent(event);
}
