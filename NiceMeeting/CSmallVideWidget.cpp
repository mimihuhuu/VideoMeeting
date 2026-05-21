#include "CSmallVideWidget.h"
#include "commons.h"

CSmallVideWidget::CSmallVideWidget(QWidget* p)
	: QWidget(p)
{
	setFixedSize(SMALL_VIDEO_WIDGET_WIDTH, SMALL_VIDEO_WIDGET_HEIGHT);
	setAttribute(Qt::WA_StyledBackground, true); 
}

CSmallVideWidget::~CSmallVideWidget()
{

}