#include "CBigVideoWidget.h"

CBigVideoWidget::CBigVideoWidget(QWidget* p)
	: QWidget(p)
{
	this->setMinimumSize(800, 600);
	setAttribute(Qt::WA_StyledBackground, true);
	setStyleSheet("background-color:rgb(170,130,230);");
}

CBigVideoWidget::~CBigVideoWidget()
{
	
}

HWND CBigVideoWidget::getHWND() const
{
	return (HWND)(this->winId());
}
