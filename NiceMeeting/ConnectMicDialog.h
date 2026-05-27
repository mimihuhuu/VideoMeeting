#pragma once

#include "MeetingFramelessDialog.h"
#include <QList>

class QListWidget;
class QPushButton;

class ConnectMicDialog : public MeetingFramelessDialog
{
	Q_OBJECT

public:
	explicit ConnectMicDialog(const QList<unsigned int>& memberUids, QWidget* parent = nullptr);

	unsigned int selectedUid() const;

private slots:
	void onConnectSelected();

private:
	QListWidget* m_pMemberList = nullptr;
	QPushButton* m_pBtnConnect = nullptr;
	unsigned int m_selectedUid = 0;
};
