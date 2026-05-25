#include "CBigVideoWidget.h"
#include "commons.h"
#include <QSizePolicy>

CBigVideoWidget::CBigVideoWidget(QWidget* p)
	: QWidget(p)
{
	resize(BIG_VIDEO_DEFAULT_WIDTH, BIG_VIDEO_DEFAULT_HEIGHT);
	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
	setAttribute(Qt::WA_StyledBackground, true);
	setAttribute(Qt::WA_NativeWindow);
	setAttribute(Qt::WA_DontCreateNativeAncestors);
	setStyleSheet("background-color: rgb(26, 26, 26);");
}

CBigVideoWidget::~CBigVideoWidget()
{
}

HWND CBigVideoWidget::getHWND() const
{
	return reinterpret_cast<HWND>(winId());
}

bool CBigVideoWidget::hasHeightForWidth() const
{
	return true;
}

int CBigVideoWidget::heightForWidth(int w) const
{
	return (w * VIDEO_ASPECT_DEN) / VIDEO_ASPECT_NUM;
}
