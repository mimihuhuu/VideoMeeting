#include "CSmallVideWidget.h"
#include "commons.h"

CSmallVideWidget::CSmallVideWidget(QWidget* p)
	: QWidget(p)
{
	resize(SMALL_VIDEO_WIDGET_WIDTH, SMALL_VIDEO_WIDGET_HEIGHT);
	setAttribute(Qt::WA_StyledBackground, true);
	setAttribute(Qt::WA_NativeWindow);
	setAttribute(Qt::WA_DontCreateNativeAncestors);
	setStyleSheet("background-color: rgb(26, 26, 26);");
}

CSmallVideWidget::~CSmallVideWidget()
{
}
