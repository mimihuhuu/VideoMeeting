#pragma once
#include "MeetingFramelessDialog.h"

class QLabel;

class InviteDialog : public MeetingFramelessDialog
{
	Q_OBJECT

public:
	explicit InviteDialog(const QString& roomId, QWidget* parent = nullptr);

private:
	QLabel* m_pRoomLabel = nullptr;
};
