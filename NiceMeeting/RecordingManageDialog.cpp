#include "RecordingManageDialog.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QVariant>

RecordingManageDialog::RecordingManageDialog(unsigned int localUid,
	const QList<unsigned int>& remoteUids,
	unsigned int recordingUid,
	bool screenRecordingActive,
	QWidget* parent)
	: MeetingFramelessDialog(QString::fromUtf8(u8"\u5f55\u5236"), parent)
	, m_localUid(localUid)
	, m_recordingUid(recordingUid)
	, m_screenRecordingActive(screenRecordingActive)
{
	setMinimumSize(460, 320);

	auto* hintLabel = new QLabel(
		QString::fromUtf8(u8"\u9009\u62e9\u8981\u5f55\u5236\u7684\u5185\u5bb9\uff1a\u89c6\u9891\u6d41\uff08\u4ec5\u5355\u8def\u753b\u9762\uff09\u6216\u4f1a\u8bae\u753b\u9762\uff08\u6240\u89c1\u5373\u6240\u5f55\uff09\u3002\u5de6\u4fa7\u5c0f\u7a97\u4e5f\u53ef\u70b9\u300c\u5f55\u300d\u3002"),
		this);
	hintLabel->setWordWrap(true);

	m_pStreamContainer = new QWidget(this);
	m_pStreamLayout = new QVBoxLayout(m_pStreamContainer);
	m_pStreamLayout->setContentsMargins(0, 0, 0, 0);
	m_pStreamLayout->setSpacing(8);

	auto* screenRow = new QWidget(this);
	auto* screenLabel = new QLabel(QString::fromUtf8(u8"\u4f1a\u8bae\u753b\u9762\uff08\u4e3b\u7a97\u53e3 + \u5df2\u6253\u5f00\u5f39\u7a97\uff09"), screenRow);
	m_pScreenToggleButton = new QPushButton(m_screenRecordingActive
		? QString::fromUtf8(u8"\u505c\u6b62\u5f55\u5236")
		: QString::fromUtf8(u8"\u5f00\u59cb\u5f55\u5236"), screenRow);
	connect(m_pScreenToggleButton, &QPushButton::clicked, this, &RecordingManageDialog::onScreenToggleClicked);

	auto* screenLayout = new QHBoxLayout(screenRow);
	screenLayout->setContentsMargins(0, 0, 0, 0);
	screenLayout->addWidget(screenLabel, 1);
	screenLayout->addWidget(m_pScreenToggleButton);

	bodyLayout()->addWidget(hintLabel);
	bodyLayout()->addWidget(m_pStreamContainer, 1);
	bodyLayout()->addWidget(screenRow);

	rebuildStreamRows(localUid, remoteUids);
}

void RecordingManageDialog::rebuildStreamRows(unsigned int localUid, const QList<unsigned int>& remoteUids)
{
	m_localUid = localUid;
	m_toggleButtons.clear();

	while (QLayoutItem* item = m_pStreamLayout->takeAt(0)) {
		if (QWidget* widget = item->widget()) {
			widget->deleteLater();
		}
		delete item;
	}

	auto addStreamRow = [this](unsigned int uid, bool isLocal) {
		auto* row = new QWidget(m_pStreamContainer);
		const QString role = isLocal ? QString::fromUtf8(u8"\u672c\u673a\u4e3b\u89c6\u9891")
			: QString::fromUtf8(u8"\u8fdc\u7aef\u5c0f\u7a97");
		auto* nameLabel = new QLabel(QString::fromUtf8(u8"UID %1\uff08%2\uff09").arg(uid).arg(role), row);
		auto* toggleButton = new QPushButton(m_recordingUid == uid
			? QString::fromUtf8(u8"\u505c\u6b62\u5f55\u5236")
			: QString::fromUtf8(u8"\u5f00\u59cb\u5f55\u5236"), row);
		toggleButton->setProperty("recordUid", QVariant::fromValue(uid));
		connect(toggleButton, &QPushButton::clicked, this, &RecordingManageDialog::onToggleClicked);

		auto* rowLayout = new QHBoxLayout(row);
		rowLayout->setContentsMargins(0, 0, 0, 0);
		rowLayout->addWidget(nameLabel, 1);
		rowLayout->addWidget(toggleButton);
		m_pStreamLayout->addWidget(row);
		m_toggleButtons.insert(uid, toggleButton);
	};

	addStreamRow(localUid, true);
	for (unsigned int uid : remoteUids) {
		addStreamRow(uid, false);
	}
}

void RecordingManageDialog::updateRemoteMembers(unsigned int localUid, const QList<unsigned int>& remoteUids)
{
	rebuildStreamRows(localUid, remoteUids);
}

void RecordingManageDialog::setRecordingUid(unsigned int uid)
{
	m_recordingUid = uid;
	for (auto it = m_toggleButtons.constBegin(); it != m_toggleButtons.constEnd(); ++it) {
		it.value()->setText(it.key() == uid
			? QString::fromUtf8(u8"\u505c\u6b62\u5f55\u5236")
			: QString::fromUtf8(u8"\u5f00\u59cb\u5f55\u5236"));
	}
}

void RecordingManageDialog::setScreenRecordingActive(bool active)
{
	m_screenRecordingActive = active;
	if (m_pScreenToggleButton) {
		m_pScreenToggleButton->setText(active
			? QString::fromUtf8(u8"\u505c\u6b62\u5f55\u5236")
			: QString::fromUtf8(u8"\u5f00\u59cb\u5f55\u5236"));
	}
}

void RecordingManageDialog::onToggleClicked()
{
	auto* button = qobject_cast<QPushButton*>(sender());
	if (!button) {
		return;
	}
	const unsigned int uid = button->property("recordUid").toUInt();
	if (m_recordingUid == uid) {
		emit sig_stopRecording(uid);
	} else {
		emit sig_startRecording(uid);
	}
}

void RecordingManageDialog::onScreenToggleClicked()
{
	if (m_screenRecordingActive) {
		emit sig_stopScreenRecording();
	} else {
		emit sig_startScreenRecording();
	}
}
