#include "CBottonBar.h"

CBottonBar::CBottonBar(QWidget* p)
	: QWidget(p)
{
	this->setFixedHeight(100);
	setAttribute(Qt::WA_StyledBackground, true);
	setStyleSheet("background-color:rgb(130,160,160);");
}

CBottonBar::~CBottonBar()
{

}
