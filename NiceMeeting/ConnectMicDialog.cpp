#include "ConnectMicDialog.h"

#include <QHBoxLayout>
#include <QListWidgetItem>
#include <QMessageBox>
#include <QPushButton>

ConnectMicDialog::ConnectMicDialog(const QList<unsigned int>& memberUids, QWidget* parent)
	: MeetingFramelessDialog(QString::fromUtf8(u8"\u8fde\u9ea6"), parent)
{
	setMinimumSize(480, 420);

	m_pMemberList = new QListWidget(this);
	m_pMemberList->setSelectionMode(QAbstractItemView::SingleSelection);
	m_pMemberList->setStyleSheet(
		QStringLiteral("QListWidget { background-color: rgb(255,255,255); border: 1px solid rgb(202,199,198); }"));

	for (unsigned int uid : memberUids) {
		auto* item = new QListWidgetItem(QString::fromUtf8(u8"\u7528\u6237 UID: %1").arg(uid));
		item->setData(Qt::UserRole, uid);
		m_pMemberList->addItem(item);
	}

	if (m_pMemberList->count() == 0) {
		auto* item = new QListWidgetItem(QString::fromUtf8(u8"\u6682\u65e0\u5176\u4ed6\u6210\u5458"));
		item->setFlags(Qt::NoItemFlags);
		m_pMemberList->addItem(item);
	} else {
		m_pMemberList->setCurrentRow(0);
	}

	m_pBtnConnect = new QPushButton(QString::fromUtf8(u8"\u53d1\u8d77\u8fde\u9ea6"), this);
	m_pBtnConnect->setEnabled(!memberUids.isEmpty());

	auto* bottomLayout = new QHBoxLayout();
	bottomLayout->addStretch();
	bottomLayout->addWidget(m_pBtnConnect);

	bodyLayout()->addWidget(m_pMemberList, 1);
	bodyLayout()->addLayout(bottomLayout);

	connect(m_pBtnConnect, &QPushButton::clicked, this, &ConnectMicDialog::onConnectSelected);
	connect(m_pMemberList, &QListWidget::itemDoubleClicked, this, [this](QListWidgetItem* item) {
		if (!item || !(item->flags() & Qt::ItemIsSelectable)) {
			return;
		}
		m_selectedUid = item->data(Qt::UserRole).toUInt();
		accept();
	});
}

unsigned int ConnectMicDialog::selectedUid() const
{
	return m_selectedUid;
}

void ConnectMicDialog::onConnectSelected()
{
	auto* item = m_pMemberList->currentItem();
	if (!item || !(item->flags() & Qt::ItemIsSelectable)) {
		QMessageBox::warning(this, QString::fromUtf8(u8"\u63d0\u793a"),
			QString::fromUtf8(u8"\u8bf7\u5148\u9009\u62e9\u8981\u8fde\u9ea6\u7684\u6210\u5458"));
		return;
	}

	m_selectedUid = item->data(Qt::UserRole).toUInt();
	if (m_selectedUid == 0) {
		return;
	}
	accept();
}
