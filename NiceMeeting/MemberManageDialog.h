#pragma once
#include "MeetingFramelessDialog.h"
#include <QList>

class QListWidget;
class QPushButton;

class MemberManageDialog : public MeetingFramelessDialog
{
	Q_OBJECT

public:
	explicit MemberManageDialog(const QList<unsigned int>& memberUids, QWidget* parent = nullptr);

	void updateMemberList(const QList<unsigned int>& memberUids);

signals:
	void sig_kickUser(unsigned int uid);
	void sig_blockRemoteVideo(unsigned int uid);
	void sig_blockRemoteAudio(unsigned int uid);
	void sig_muteUserAudio(unsigned int uid);

private slots:
	void onKickSelected();
	void onBlockRemoteVideo();
	void onMuteAudio();
	void onMuteRemoteMic();

private:
	unsigned int selectedUid() const;

	QListWidget* m_pMemberList = nullptr;
	QPushButton* m_pBtnKick = nullptr;
	QPushButton* m_pBtnBlockVideo = nullptr;
	QPushButton* m_pBtnMuteAudio = nullptr;
	QPushButton* m_pBtnMuteRemoteMic = nullptr;
};
