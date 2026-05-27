#include "MeetingChatDialog.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QListWidgetItem>
#include <QPushButton>
#include <QTextEdit>

MeetingChatDialog::MeetingChatDialog(unsigned int localUid, QWidget* parent)
	: MeetingFramelessDialog(QString::fromUtf8(u8"\u804a\u5929"), parent)
	, m_localUid(localUid)
{
	setMinimumSize(420, 520);

	m_pMessageView = new QTextEdit(this);
	m_pMessageView->setReadOnly(true);
	m_pMessageView->setStyleSheet(QStringLiteral("background-color: #FAFAFA; font-size: 13px;"));

	m_pTargetList = new QListWidget(this);
	m_pTargetList->setMaximumHeight(90);
	auto* allItem = new QListWidgetItem(QString::fromUtf8(u8"\u5168\u4f53\u6210\u5458\uff08\u9891\u9053\u804a\u5929\uff09"));
	allItem->setData(Qt::UserRole, 0u);
	allItem->setSelected(true);
	m_pTargetList->addItem(allItem);

	m_pInputEdit = new QLineEdit(this);
	m_pInputEdit->setPlaceholderText(QString::fromUtf8(u8"\u8f93\u5165\u6d88\u606f\u540e\u56de\u8f66\u6216\u70b9\u53d1\u9001"));
	m_pBtnSend = new QPushButton(QString::fromUtf8(u8"\u53d1\u9001"), this);

	auto* inputRow = new QHBoxLayout();
	inputRow->addWidget(m_pInputEdit, 1);
	inputRow->addWidget(m_pBtnSend);

	bodyLayout()->addWidget(new QLabel(QString::fromUtf8(u8"\u804a\u5929\u8bb0\u5f55"), this));
	bodyLayout()->addWidget(m_pMessageView, 1);
	bodyLayout()->addWidget(new QLabel(QString::fromUtf8(u8"\u53d1\u9001\u7ed9"), this));
	bodyLayout()->addWidget(m_pTargetList);
	bodyLayout()->addLayout(inputRow);

	connect(m_pBtnSend, &QPushButton::clicked, this, &MeetingChatDialog::onSendClicked);
	connect(m_pInputEdit, &QLineEdit::returnPressed, this, &MeetingChatDialog::onSendClicked);
}

void MeetingChatDialog::appendMessage(unsigned int fromUid, unsigned int targetUid, const QString& text)
{
	QString prefix = (fromUid == m_localUid) ? QString::fromUtf8(u8"\u6211")
		: QString::fromUtf8(u8"UID %1").arg(fromUid);
	prefix += (targetUid > 0) ? QString::fromUtf8(u8" -> UID %1").arg(targetUid)
		: QString::fromUtf8(u8" -> \u5168\u4f53");
	m_pMessageView->append(prefix + QString::fromUtf8(u8": ") + text);
}

void MeetingChatDialog::updateMemberList(const QList<unsigned int>& memberUids)
{
	while (m_pTargetList->count() > 1) {
		delete m_pTargetList->takeItem(1);
	}
	for (unsigned int uid : memberUids) {
		if (uid == m_localUid) {
			continue;
		}
		auto* item = new QListWidgetItem(QString::fromUtf8(u8"\u5355\u804a UID %1").arg(uid));
		item->setData(Qt::UserRole, uid);
		m_pTargetList->addItem(item);
	}
}

void MeetingChatDialog::onSendClicked()
{
	const QString text = m_pInputEdit->text().trimmed();
	if (text.isEmpty()) {
		return;
	}
	unsigned int targetUid = 0;
	if (auto* item = m_pTargetList->currentItem()) {
		targetUid = item->data(Qt::UserRole).toUInt();
	}
	emit sig_sendMessage(text, targetUid);
	m_pInputEdit->clear();
}
