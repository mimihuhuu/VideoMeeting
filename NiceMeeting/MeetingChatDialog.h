#pragma once
#include "MeetingFramelessDialog.h"
#include <QList>
#include <QString>

class QListWidget;
class QLineEdit;
class QPushButton;
class QTextEdit;

class MeetingChatDialog : public MeetingFramelessDialog
{
	Q_OBJECT

public:
	explicit MeetingChatDialog(unsigned int localUid, QWidget* parent = nullptr);

	void appendMessage(unsigned int fromUid, unsigned int targetUid, const QString& text);
	void updateMemberList(const QList<unsigned int>& memberUids);

signals:
	void sig_sendMessage(const QString& text, unsigned int targetUid);

private slots:
	void onSendClicked();

private:
	unsigned int m_localUid = 0;
	QTextEdit* m_pMessageView = nullptr;
	QLineEdit* m_pInputEdit = nullptr;
	QPushButton* m_pBtnSend = nullptr;
	QListWidget* m_pTargetList = nullptr;
};
