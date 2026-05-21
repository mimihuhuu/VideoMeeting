#include "CLeftVideoList.h"
#include "CSmallVideWidget.h"
#include "commons.h"
#include <QVBoxLayout>

CLeftVideoList::CLeftVideoList(QWidget* p)
	: QWidget(p)
{
	this->setFixedWidth(SMALL_VIDEO_WIDGET_WIDTH + 10);
	setAttribute(Qt::WA_StyledBackground, true);
	setStyleSheet("background-color:rgb(200,200,200);"); 

	m_pVideoListWidget = new QListWidget(this);
	m_pVideoListWidget->setStyleSheet("background-color:rgb(120,220,120);");

	QVBoxLayout* pVLayout = new QVBoxLayout(this);
	pVLayout->addWidget(m_pVideoListWidget);
	pVLayout->setContentsMargins(5,5,5,5);
}

CLeftVideoList::~CLeftVideoList()
{

}

void CLeftVideoList::addVideoWidget(CSmallVideWidget* pSmall)
{
	QListWidgetItem* pItem = new QListWidgetItem();
	m_pVideoListWidget->addItem(pItem);	
	m_pVideoListWidget->setItemWidget(pItem,pSmall);
}
