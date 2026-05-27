#pragma once

#include <QWidget>
#include <QShowEvent>
#include <QEvent>
#include "CFrameLessWidgetBase.h"
#include "CBigVideoWidget.h"
#include "CLeftVideoList.h"
#include "CBottonBar.h"
#include "CTitleBar.h"
#include "CAgoraObject.h"
#include "CAgoraKickService.h"
#include "MeetingChatDialog.h"
#include "MemberManageDialog.h"
#include "RecordingManageDialog.h"
#include "InviteDialog.h"
#include "ScreenRegionRecorder.h"

class CmainWidget : public CFrameLessWidgetBase
{
	Q_OBJECT

public:
	CmainWidget(QWidget* p = nullptr);
	~CmainWidget() override;

	void joinroom(const QString& roomID);
	void setJoinMediaOptions(bool cameraEnabled, bool micEnabled);

private:
	void initUI();
	void showEvent(QShowEvent* event) override;
	void changeEvent(QEvent* event) override;
	void updateMainVideoRenderMode();
	void syncLocalMicUi(bool micEnabled);
	void applyInitialAudioPolicy();
	void applyJoinMediaOptions();
	void applyConnectMicRemoteAudio(unsigned int fromUid, unsigned int targetUid, bool unmute);
	QRect collectMeetingRecordRegion() const;
	void showRecordingManageDialog();
	void startScreenRegionRecording();
	void stopScreenRegionRecording(bool notifyUser = true);
	QString currentAppId() const;
	void refreshMemberDialogs();

private slots:
	void on_LocalJoinSuccess(const QString& qsChannel, unsigned int uid, int elapsed);
	void on_RemoteJoined(uid_t uid, int elapsed);
	void on_RemoteOffline(unsigned int uid, USER_OFFLINE_REASON_TYPE reason);
	void on_enableVideo(bool enabled);
	void on_enableAudio(bool enabled);
	void on_connectMic(bool enabled);
	void on_EndMeeting();
	void on_shareScreen();
	void on_invite();
	void on_manageMembers();
	void on_chat();
	void on_recordScreen();
	void on_settings();
	void on_toggleParticipantRecording(unsigned int uid);
	void on_chatSendMessage(const QString& text, unsigned int targetUid);
	void on_chatMessageReceived(unsigned int fromUid, unsigned int targetUid, const QString& text);
	void on_controlCommandReceived(unsigned int fromUid, unsigned int targetUid, const QString& command);
	void on_kickUser(unsigned int uid);
	void on_blockRemoteAudio(unsigned int uid);
	void on_blockRemoteVideo(unsigned int uid);
	void on_muteMemberAudio(unsigned int uid);
	void on_kickFinished(bool success, const QString& message);

private:
	void statShareScreen(int type, qint64 sourceId);

private:
	CBigVideoWidget* m_pBigVideoWidget = nullptr;
	CLeftVideoList* m_pLeftVideoList = nullptr;
	CBottonBar* m_pBottonBar = nullptr;
	CTitleBar* m_pTitleBar = nullptr;

	CAgoraObject* m_pAgoraObject = nullptr;
	CAgoraKickService* m_pKickService = nullptr;
	ScreenRegionRecorder* m_pScreenRegionRecorder = nullptr;
	MeetingChatDialog* m_pChatDialog = nullptr;
	MemberManageDialog* m_pMemberDialog = nullptr;
	RecordingManageDialog* m_pRecordingDialog = nullptr;
	QString m_roomId;
	bool m_centeredOnShow = false;
	bool m_localPreviewActive = false;
	bool m_joinWithCamera = false;
	bool m_joinWithMic = false;
	bool m_meetingRecordingActive = false;
	bool m_screenRegionRecordingActive = false;
	unsigned int m_connectMicTargetUid = 0;
};
