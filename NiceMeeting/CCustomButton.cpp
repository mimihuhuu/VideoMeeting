#include "CCustomButton.h"
#include "commons.h"
#include <QEvent>
#include <QFont>
#include <QSizePolicy>

CCustomButton::CCustomButton(QString text, QString normalImageUrl, QString normalhoverImageUrl, QString clickedImageUrl,
		QString clickedImagehoverUrl, bool clicked, QWidget* parent)
	: QToolButton(parent)
	, m_text(text)
	, m_normalImageUrl(normalImageUrl)
	, m_normalhoverImageUrl(normalhoverImageUrl)
	, m_clickedImageUrl(clickedImageUrl)
	, m_clickedImagehoverUrl(clickedImagehoverUrl)
	, m_StateOpen(clicked)
{
	setAttribute(Qt::WA_StyledBackground, true);
	setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
	setIconSize(QSize(BOTTONBAR_TOOL_ICON_SIZE, BOTTONBAR_TOOL_ICON_SIZE));
	setText(m_text);
	setCheckable(true);
	setIcon(QIcon(m_StateOpen ? m_normalImageUrl : m_clickedImageUrl));

	QFont font(QStringLiteral("Microsoft YaHei"), 12);
	setFont(font);
	setFixedSize(BOTTONBAR_TOOL_BTN_WIDTH, BOTTONBAR_TOOL_BTN_HEIGHT);
	setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

	setStyleSheet(
		"QToolButton {"
		"    border: none;"
		"    background-color: transparent;"
		"    color: #333333;"
		"    font-family: 'Microsoft YaHei';"
		"    font-size: 12px;"
		"    padding: 0px 2px;"
		"}"
		"QToolButton:hover {"
		"    background-color: rgba(0, 0, 0, 0.06);"
		"}"
		"QToolButton:pressed {"
		"    background-color: rgba(0, 0, 0, 0.10);"
		"}");
}

CCustomButton::~CCustomButton()
{
}

bool CCustomButton::event(QEvent* e)
{
	if (this->isChecked()) {
		return QWidget::event(e);
	}
	switch (e->type())
	{
	case QEvent::Enter:
		setIcon(QIcon(m_StateOpen ? m_normalhoverImageUrl : m_clickedImagehoverUrl));
		return true;
	case QEvent::MouseButtonPress:
		setIcon(QIcon(m_StateOpen ? m_normalImageUrl : m_clickedImageUrl));
		setChecked(true);
		emit sig_clicked();
		return true;
	case QEvent::Leave:
		setIcon(QIcon(m_StateOpen ? m_normalImageUrl : m_clickedImageUrl));
		return true;
	default:
		break;
	}
	return QWidget::event(e);
}
