#include "CLeftVideoList.h"
#include "CSmallVideWidget.h"
#include "commons.h"
#include <QVBoxLayout>
#include <QFrame>

CLeftVideoList::CLeftVideoList(QWidget* p)
	: QWidget(p)
{
	resize(SMALL_VIDEO_WIDGET_WIDTH, 480);
	setAttribute(Qt::WA_StyledBackground, true);
	setStyleSheet("background-color: #2A2A2A;");

	m_pVideoListWidget = new QListWidget(this);
	m_pVideoListWidget->setFrameShape(QFrame::NoFrame);
	m_pVideoListWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	m_pVideoListWidget->setSpacing(8);
	m_pVideoListWidget->setStyleSheet("background-color: #1A1A1A;");

	QVBoxLayout* pVLayout = new QVBoxLayout(this);
	pVLayout->setContentsMargins(0, 0, 0, 0);
	pVLayout->setSpacing(0);
	pVLayout->addWidget(m_pVideoListWidget);
}

CLeftVideoList::~CLeftVideoList()
{
}

void CLeftVideoList::addVideoWidget(CSmallVideWidget* pSmall)
{
	if (!pSmall || !m_pVideoListWidget) {
		return;
	}
	pSmall->setParent(m_pVideoListWidget);
	pSmall->show();

	QListWidgetItem* pItem = new QListWidgetItem(m_pVideoListWidget);
	pItem->setSizeHint(pSmall->size());
	m_pVideoListWidget->addItem(pItem);
	m_pVideoListWidget->setItemWidget(pItem, pSmall);
}
