#include "MemberManageDialog.h"

#include <QHBoxLayout>
#include <QListWidgetItem>
#include <QMessageBox>
#include <QPushButton>

MemberManageDialog::MemberManageDialog(const QList<unsigned int>& memberUids, QWidget* parent)
	: MeetingFramelessDialog(QString::fromUtf8(u8"\u6210\u5458\u7ba1\u7406"), parent)
{
	setMinimumSize(620, 460);

	m_pMemberList = new QListWidget(this);
	m_pMemberList->setSelectionMode(QAbstractItemView::SingleSelection);
	m_pMemberList->setStyleSheet(
		QStringLiteral("QListWidget { background-color: rgb(255,255,255); border: 1px solid rgb(202,199,198); }"));

	m_pBtnKick = new QPushButton(QString::fromUtf8(u8"\u8e22\u51fa\u4f1a\u8bae"), this);
	m_pBtnBlockVideo = new QPushButton(QString::fromUtf8(u8"\u5c4f\u853d\u89c6\u9891"), this);
	m_pBtnMuteAudio = new QPushButton(QString::fromUtf8(u8"\u5c4f\u853d\u58f0\u97f3"), this);
	m_pBtnMuteRemoteMic = new QPushButton(QString::fromUtf8(u8"\u7981\u6b62\u53d1\u8a00"), this);

	auto* bottomLayout = new QHBoxLayout();
	bottomLayout->addWidget(m_pBtnKick);
	bottomLayout->addWidget(m_pBtnBlockVideo);
	bottomLayout->addWidget(m_pBtnMuteAudio);
	bottomLayout->addWidget(m_pBtnMuteRemoteMic);
	bottomLayout->addStretch();

	bodyLayout()->addWidget(m_pMemberList, 1);
	bodyLayout()->addLayout(bottomLayout);

	connect(m_pBtnKick, &QPushButton::clicked, this, &MemberManageDialog::onKickSelected);
	connect(m_pBtnBlockVideo, &QPushButton::clicked, this, &MemberManageDialog::onBlockRemoteVideo);
	connect(m_pBtnMuteAudio, &QPushButton::clicked, this, &MemberManageDialog::onMuteAudio);
	connect(m_pBtnMuteRemoteMic, &QPushButton::clicked, this, &MemberManageDialog::onMuteRemoteMic);

	updateMemberList(memberUids);
}

void MemberManageDialog::updateMemberList(const QList<unsigned int>& memberUids)
{
	m_pMemberList->clear();

	for (unsigned int uid : memberUids) {
		auto* item = new QListWidgetItem(QString::fromUtf8(u8"\u7528\u6237 UID: %1").arg(uid));
		item->setData(Qt::UserRole, uid);
		m_pMemberList->addItem(item);
	}

	if (m_pMemberList->count() == 0) {
		auto* item = new QListWidgetItem(QString::fromUtf8(u8"\u6682\u65e0\u5176\u4ed6\u6210\u5458"));
		item->setFlags(Qt::NoItemFlags);
		m_pMemberList->addItem(item);
	}

	const bool hasMembers = !memberUids.isEmpty();
	m_pBtnKick->setEnabled(hasMembers);
	m_pBtnBlockVideo->setEnabled(hasMembers);
	m_pBtnMuteAudio->setEnabled(hasMembers);
	m_pBtnMuteRemoteMic->setEnabled(hasMembers);
}

unsigned int MemberManageDialog::selectedUid() const
{
	auto* item = m_pMemberList->currentItem();
	if (!item || !(item->flags() & Qt::ItemIsSelectable)) {
		return 0;
	}
	return item->data(Qt::UserRole).toUInt();
}

void MemberManageDialog::onKickSelected()
{
	const unsigned int uid = selectedUid();
	if (uid == 0) {
		QMessageBox::warning(this, QString::fromUtf8(u8"\u63d0\u793a"),
			QString::fromUtf8(u8"\u8bf7\u5148\u9009\u62e9\u8981\u64cd\u4f5c\u7684\u6210\u5458"));
		return;
	}
	if (QMessageBox::question(this, QString::fromUtf8(u8"\u786e\u8ba4\u8e22\u51fa"),
		QString::fromUtf8(u8"\u786e\u5b9a\u8981\u8e22\u51fa UID %1 \u5417\uff1f").arg(uid)) != QMessageBox::Yes) {
		return;
	}
	emit sig_kickUser(uid);
}

void MemberManageDialog::onBlockRemoteVideo()
{
	const unsigned int uid = selectedUid();
	if (uid == 0) {
		QMessageBox::warning(this, QString::fromUtf8(u8"\u63d0\u793a"),
			QString::fromUtf8(u8"\u8bf7\u5148\u9009\u62e9\u8981\u64cd\u4f5c\u7684\u6210\u5458"));
		return;
	}
	if (QMessageBox::question(this, QString::fromUtf8(u8"\u786e\u8ba4"),
		QString::fromUtf8(u8"\u786e\u5b9a\u5728\u672c\u673a\u5c4f\u853d UID %1 \u7684\u89c6\u9891\u5417\uff1f").arg(uid))
		!= QMessageBox::Yes) {
		return;
	}
	emit sig_blockRemoteVideo(uid);
}

void MemberManageDialog::onMuteAudio()
{
	const unsigned int uid = selectedUid();
	if (uid == 0) {
		QMessageBox::warning(this, QString::fromUtf8(u8"\u63d0\u793a"),
			QString::fromUtf8(u8"\u8bf7\u5148\u9009\u62e9\u8981\u64cd\u4f5c\u7684\u6210\u5458"));
		return;
	}
	if (QMessageBox::question(this, QString::fromUtf8(u8"\u786e\u8ba4"),
		QString::fromUtf8(u8"\u786e\u5b9a\u5728\u672c\u673a\u5c4f\u853d UID %1 \u7684\u58f0\u97f3\u5417\uff1f").arg(uid))
		!= QMessageBox::Yes) {
		return;
	}
	emit sig_blockRemoteAudio(uid);
}

void MemberManageDialog::onMuteRemoteMic()
{
	const unsigned int uid = selectedUid();
	if (uid == 0) {
		QMessageBox::warning(this, QString::fromUtf8(u8"\u63d0\u793a"),
			QString::fromUtf8(u8"\u8bf7\u5148\u9009\u62e9\u8981\u64cd\u4f5c\u7684\u6210\u5458"));
		return;
	}
	if (QMessageBox::question(this, QString::fromUtf8(u8"\u786e\u8ba4"),
		QString::fromUtf8(u8"\u786e\u5b9a\u8981\u7981\u6b62 UID %1 \u53d1\u8a00\u5417\uff1f").arg(uid))
		!= QMessageBox::Yes) {
		return;
	}
	emit sig_muteUserAudio(uid);
}
