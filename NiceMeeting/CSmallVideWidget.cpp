#include "CSmallVideWidget.h"
#include "commons.h"

#include <QResizeEvent>
#include <QShowEvent>

CSmallVideWidget::CSmallVideWidget(unsigned int memberUid, QWidget* parent)
	: QWidget(parent)
	, m_memberUid(memberUid)
{
	setFixedSize(SMALL_VIDEO_WIDGET_WIDTH, SMALL_VIDEO_WIDGET_HEIGHT);
	setAttribute(Qt::WA_StyledBackground, true);
	setAttribute(Qt::WA_NativeWindow);
	setAttribute(Qt::WA_DontCreateNativeAncestors);
	setStyleSheet(QStringLiteral("background-color: rgb(26, 26, 26);"));
}

CSmallVideWidget::~CSmallVideWidget()
{
}

HWND CSmallVideWidget::videoHwnd() const
{
	return reinterpret_cast<HWND>(winId());
}

void CSmallVideWidget::resizeEvent(QResizeEvent* event)
{
	QWidget::resizeEvent(event);
	emit sig_videoSurfaceChanged();
}

void CSmallVideWidget::showEvent(QShowEvent* event)
{
	QWidget::showEvent(event);
	emit sig_videoSurfaceChanged();
}
