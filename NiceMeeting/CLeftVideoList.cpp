#include "CLeftVideoList.h"
#include "CSmallVideWidget.h"
#include "commons.h"
#include <QAbstractItemView>
#include <QFrame>
#include <QVBoxLayout>

CLeftVideoList::CLeftVideoList(QWidget* p)
	: QWidget(p)
{
	setFixedWidth(SMALL_VIDEO_WIDGET_WIDTH);
	setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
	setAttribute(Qt::WA_StyledBackground, true);
	setStyleSheet(QStringLiteral("background-color: rgb(26, 26, 26);"));

	m_pVideoListWidget = new QListWidget(this);
	m_pVideoListWidget->setFrameShape(QFrame::NoFrame);
	m_pVideoListWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	m_pVideoListWidget->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
	m_pVideoListWidget->setSpacing(SMALL_VIDEO_LIST_ITEM_SPACING);
	m_pVideoListWidget->setStyleSheet(
		QStringLiteral("QListWidget { background-color: rgb(26, 26, 26); border: none; padding: 0px; margin: 0px; outline: none; }"
			"QListWidget::item { padding: 0px; margin: 0px; border: none; }"
			"QAbstractScrollArea::viewport { background-color: rgb(26, 26, 26); border: none; }"));

	QVBoxLayout* pVLayout = new QVBoxLayout(this);
	pVLayout->setContentsMargins(0, 0, 0, 0);
	pVLayout->setSpacing(0);
	pVLayout->addWidget(m_pVideoListWidget);
}

CLeftVideoList::~CLeftVideoList()
{
}

void CLeftVideoList::addVideoWidget(unsigned int uid, CSmallVideWidget* pSmall)
{
	if (!pSmall || !m_pVideoListWidget) {
		return;
	}

	QListWidgetItem* pItem = new QListWidgetItem(m_pVideoListWidget);
	pItem->setData(Qt::UserRole, uid);
	pItem->setFlags(Qt::ItemIsEnabled);
	pItem->setSizeHint(QSize(SMALL_VIDEO_WIDGET_WIDTH, SMALL_VIDEO_WIDGET_HEIGHT));
	m_pVideoListWidget->addItem(pItem);
	m_pVideoListWidget->setItemWidget(pItem, pSmall);
	pSmall->setFixedSize(SMALL_VIDEO_WIDGET_WIDTH, SMALL_VIDEO_WIDGET_HEIGHT);
	pSmall->move(0, 0);
	pSmall->show();
}

bool CLeftVideoList::removeVideoWidget(unsigned int uid)
{
	if (!m_pVideoListWidget) {
		return false;
	}

	for (int i = 0; i < m_pVideoListWidget->count(); ++i) {
		QListWidgetItem* pItem = m_pVideoListWidget->item(i);
		if (!pItem || pItem->data(Qt::UserRole).toUInt() != uid) {
			continue;
		}

		if (QWidget* pWidget = m_pVideoListWidget->itemWidget(pItem)) {
			m_pVideoListWidget->removeItemWidget(pItem);
			delete pWidget;
		}
		delete m_pVideoListWidget->takeItem(i);
		if (m_pVideoListWidget->count() > 0) {
			m_pVideoListWidget->scrollToItem(m_pVideoListWidget->item(0));
		}
		return true;
	}
	return false;
}

bool CLeftVideoList::hasVideoWidget(unsigned int uid) const
{
	return videoWidget(uid) != nullptr;
}

CSmallVideWidget* CLeftVideoList::videoWidget(unsigned int uid) const
{
	if (!m_pVideoListWidget) {
		return nullptr;
	}

	for (int i = 0; i < m_pVideoListWidget->count(); ++i) {
		const QListWidgetItem* pItem = m_pVideoListWidget->item(i);
		if (!pItem || pItem->data(Qt::UserRole).toUInt() != uid) {
			continue;
		}
		return qobject_cast<CSmallVideWidget*>(
			m_pVideoListWidget->itemWidget(const_cast<QListWidgetItem*>(pItem)));
	}
	return nullptr;
}

QList<unsigned int> CLeftVideoList::remoteMemberUids() const
{
	QList<unsigned int> uids;
	if (!m_pVideoListWidget) {
		return uids;
	}

	for (int i = 0; i < m_pVideoListWidget->count(); ++i) {
		const QListWidgetItem* pItem = m_pVideoListWidget->item(i);
		if (pItem) {
			uids.append(pItem->data(Qt::UserRole).toUInt());
		}
	}
	return uids;
}
